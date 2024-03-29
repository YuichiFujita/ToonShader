//============================================================
//
//	プレイヤー処理 [player.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "player.h"
#include "manager.h"
#include "sceneGame.h"
#include "gameManager.h"
#include "renderer.h"
#include "sound.h"
#include "camera.h"
#include "texture.h"
#include "collision.h"
#include "fade.h"

#include "multiModel.h"
#include "orbit.h"
#include "shadow.h"
#include "orbit.h"
#include "object2D.h"
#include "timerManager.h"
#include "rankingManager.h"
#include "stage.h"
#include "field.h"

#include "effect3D.h"
#include "particle3D.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char *MODEL_FILE[] =	// モデルファイル
	{
		"data\\MODEL\\PLAYER\\00_waist.x",	// 腰
		"data\\MODEL\\PLAYER\\01_body.x",	// 体
		"data\\MODEL\\PLAYER\\02_head.x",	// 頭
		"data\\MODEL\\PLAYER\\03_armUL.x",	// 左上腕
		"data\\MODEL\\PLAYER\\04_armUR.x",	// 右上腕
		"data\\MODEL\\PLAYER\\05_armDL.x",	// 左下腕
		"data\\MODEL\\PLAYER\\06_armDR.x",	// 右下腕
		"data\\MODEL\\PLAYER\\07_handL.x",	// 左手
		"data\\MODEL\\PLAYER\\08_handR.x",	// 右手
		"data\\MODEL\\PLAYER\\09_legUL.x",	// 左太もも
		"data\\MODEL\\PLAYER\\10_legUR.x",	// 右太もも
		"data\\MODEL\\PLAYER\\11_legDL.x",	// 左脛
		"data\\MODEL\\PLAYER\\12_legDR.x",	// 右脛
		"data\\MODEL\\PLAYER\\13_footL.x",	// 左足
		"data\\MODEL\\PLAYER\\14_footR.x",	// 右足
	};

	const char *SETUP_TXT = "data\\TXT\\player.txt";	// セットアップテキスト相対パス

	const int	PRIORITY	= 3;		// プレイヤーの優先順位
	const float	MOVE		= 2.8f;		// 移動量
	const float	JUMP		= 21.0f;	// ジャンプ上昇量
	const float	GRAVITY		= 1.0f;		// 重力
	const float	RADIUS		= 20.0f;	// 半径
	const float	HEIGHT		= 100.0f;	// 縦幅
	const float	REV_ROTA	= 0.15f;	// 向き変更の補正係数
	const float	ADD_MOVE	= 0.08f;	// 非アクション時の速度加算量
	const float	JUMP_REV	= 0.16f;	// 通常状態時の空中の移動量の減衰係数
	const float	LAND_REV	= 0.16f;	// 通常状態時の地上の移動量の減衰係数
	const float	SPAWN_ADD_ALPHA	= 0.03f;	// スポーン状態時の透明度の加算量

	const D3DXVECTOR3 DMG_ADDROT	= D3DXVECTOR3(0.04f, 0.0f, -0.02f);	// ダメージ状態時のプレイヤー回転量
	const D3DXVECTOR3 SHADOW_SIZE	= D3DXVECTOR3(80.0f, 0.0f, 80.0f);	// 影の大きさ

	const COrbit::SOffset ORBIT_OFFSET = COrbit::SOffset(D3DXVECTOR3(0.0f, 15.0f, 0.0f), D3DXVECTOR3(0.0f, -15.0f, 0.0f), XCOL_CYAN);	// オフセット情報
	const int ORBIT_PART = 20;	// 分割数
}

//************************************************************
//	スタティックアサート
//************************************************************
static_assert(NUM_ARRAY(MODEL_FILE) == CPlayer::MODEL_MAX, "ERROR : Model Count Mismatch");

//************************************************************
//	子クラス [CPlayer] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CPlayer::CPlayer() : CObjectChara(CObject::LABEL_PLAYER, CObject::DIM_3D, PRIORITY),
	m_pShadow		(nullptr),		// 影の情報
	m_pOrbit		(nullptr),		// 軌跡の情報
	m_oldPos		(VEC3_ZERO),	// 過去位置
	m_move			(VEC3_ZERO),	// 移動量
	m_destRot		(VEC3_ZERO),	// 目標向き
	m_state			(STATE_NONE),	// 状態
	m_bJump			(false),		// ジャンプ状況
	m_nCounterState	(0)				// 状態管理カウンター
{

}

//============================================================
//	デストラクタ
//============================================================
CPlayer::~CPlayer()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CPlayer::Init(void)
{
	// メンバ変数を初期化
	m_pShadow		= nullptr;		// 影の情報
	m_pOrbit		= nullptr;		// 軌跡の情報
	m_oldPos		= VEC3_ZERO;	// 過去位置
	m_move			= VEC3_ZERO;	// 移動量
	m_destRot		= VEC3_ZERO;	// 目標向き
	m_state			= STATE_NONE;	// 状態
	m_bJump			= true;			// ジャンプ状況
	m_nCounterState	= 0;			// 状態管理カウンター

	// オブジェクトキャラクターの初期化
	if (FAILED(CObjectChara::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// セットアップの読み込み
	LoadSetup();

	// モデル情報の設定
	SetModelInfo();

	// 影の生成
	m_pShadow = CShadow::Create(CShadow::TEXTURE_NORMAL, SHADOW_SIZE, this);
	if (m_pShadow == nullptr)
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 軌跡の生成
	m_pOrbit = COrbit::Create
	( // 引数
		GetMultiModel(MODEL_BODY)->GetPtrMtxWorld(),	// 親マトリックス
		ORBIT_OFFSET,	// オフセット情報
		ORBIT_PART		// 分割数
	);
	if (m_pOrbit == nullptr)
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CPlayer::Uninit(void)
{
	// 影の終了
	m_pShadow->DeleteObjectParent();	// 親オブジェクトを削除
	SAFE_UNINIT(m_pShadow);

	// 軌跡の終了
	SAFE_UNINIT(m_pOrbit);

	// オブジェクトキャラクターの終了
	CObjectChara::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CPlayer::Update(void)
{
	// 変数を宣言
	EMotion currentMotion = MOTION_IDOL;	// 現在のモーション

	// 過去位置の更新
	UpdateOldPosition();

	switch (m_state)
	{ // 状態ごとの処理
	case STATE_NONE:
		break;

	case STATE_SPAWN:

		// スポーン状態時の更新
		currentMotion = UpdateSpawn();

		break;

	case STATE_NORMAL:

		// 通常状態の更新
		currentMotion = UpdateNormal();

		break;

	default:
		assert(false);
		break;
	}

	// 影の更新
	m_pShadow->Update();

	// 軌跡の更新
	m_pOrbit->Update();

	// モーション・オブジェクトキャラクターの更新
	UpdateMotion(currentMotion);
}

//============================================================
//	描画処理
//============================================================
void CPlayer::Draw(void)
{
	// オブジェクトキャラクターの描画
	CObjectChara::Draw();
}

//============================================================
//	ヒット処理
//============================================================
void CPlayer::Hit(void)
{
	// 変数を宣言
	D3DXVECTOR3 posPlayer = GetVec3Position();	// プレイヤー位置
	D3DXVECTOR3 rotPlayer = GetVec3Rotation();	// プレイヤー向き

#if 0

	if (IsDeath() != true)
	{ // 死亡フラグが立っていない場合

		if (m_state == STATE_NORMAL)
		{ // 通常状態の場合

			// カウンターを初期化
			m_nCounterState = 0;

			// 待機モーションを設定
			SetMotion(MOTION_IDOL);

			// 爆発パーティクルを生成
			CParticle3D::Create(CParticle3D::TYPE_SMALL_EXPLOSION, D3DXVECTOR3(posPlayer.x, posPlayer.y + basic::HEIGHT * 0.5f, posPlayer.z));

			// サウンドの再生
			GET_MANAGER->GetSound()->Play(CSound::LABEL_SE_HIT);	// ヒット音
		}
	}

#endif
}

//============================================================
//	状態の設定処理
//============================================================
void CPlayer::SetState(const int nState)
{
	if (nState > NONE_IDX && nState < STATE_MAX)
	{ // 範囲内の場合

		// 引数の状態を設定
		m_state = (EState)nState;
	}
	else { assert(false); }
}

//============================================================
//	状態取得処理
//============================================================
int CPlayer::GetState(void) const
{
	// 状態を返す
	return m_state;
}

//============================================================
//	半径取得処理
//============================================================
float CPlayer::GetRadius(void) const
{
	// 半径を返す
	return RADIUS;
}

//============================================================
//	縦幅取得処理
//============================================================
float CPlayer::GetHeight(void) const
{
	// 縦幅を返す
	return HEIGHT;
}

//============================================================
//	更新状況の設定処理
//============================================================
void CPlayer::SetEnableUpdate(const bool bUpdate)
{
	// 引数の更新状況を設定
	CObject::SetEnableUpdate(bUpdate);		// 自身
	m_pShadow->SetEnableUpdate(bUpdate);	// 影
}

//============================================================
//	描画状況の設定処理
//============================================================
void CPlayer::SetEnableDraw(const bool bDraw)
{
	// 引数の描画状況を設定
	CObject::SetEnableDraw(bDraw);		// 自身
	m_pShadow->SetEnableDraw(bDraw);	// 影
}

//============================================================
//	マトリックス計算結果の取得処理
//============================================================
D3DXMATRIX CPlayer::CalcMtxWorld(void) const
{
	// 変数を宣言
	D3DXMATRIX  mtxRot, mtxTrans, mtxWorld;		// 計算用マトリックス
	D3DXVECTOR3 posPlayer = GetVec3Position();	// プレイヤー位置
	D3DXVECTOR3 rotPlayer = GetVec3Rotation();	// プレイヤー向き

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rotPlayer.y, rotPlayer.x, rotPlayer.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, posPlayer.x, posPlayer.y, posPlayer.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	// ワールドマトリックスを返す
	return mtxWorld;
}

//============================================================
//	生成処理
//============================================================
CPlayer *CPlayer::Create(CScene::EMode mode)
{
	// ポインタを宣言
	CPlayer *pPlayer = nullptr;	// プレイヤー情報

	// プレイヤーの生成
	switch (mode)
	{ // モードごとの処理
	case CScene::MODE_TITLE:
	case CScene::MODE_RESULT:
	case CScene::MODE_RANKING:

		break;

	case CScene::MODE_TUTORIAL:
	case CScene::MODE_GAME:

		// メモリ確保
		pPlayer = new CPlayer;	// プレイヤー

		break;

	default:	// 例外処理
		assert(false);
		break;
	}

	if (pPlayer == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// プレイヤーの初期化
		if (FAILED(pPlayer->Init()))
		{ // 初期化に失敗した場合

			// プレイヤーの破棄
			SAFE_DELETE(pPlayer);
			return nullptr;
		}

		// 確保したアドレスを返す
		return pPlayer;
	}
}

//============================================================
//	出現の設定処理
//============================================================
void CPlayer::SetSpawn(void)
{
	// 変数を宣言
	D3DXVECTOR3 set = VEC3_ZERO;	// 引数設定用

	// 情報を初期化
	SetState(STATE_SPAWN);		// スポーン状態の設定
	SetMotion(MOTION_IDOL);		// 待機モーションを設定

	// カウンターを初期化
	m_nCounterState = 0;	// 状態管理カウンター

	// 位置を設定
	SetVec3Position(set);

	// 向きを設定
	SetVec3Rotation(set);
	m_destRot = set;

	// 移動量を初期化
	m_move = VEC3_ZERO;

	// マテリアルを再設定
	ResetMaterial();

	// 透明度を透明に再設定
	SetAlpha(0.0f);

	// 描画を再開
	SetEnableDraw(true);

	// 追従カメラの目標位置の設定
	GET_MANAGER->GetCamera()->SetDestFollow();

	// サウンドの再生
	GET_MANAGER->GetSound()->Play(CSound::LABEL_SE_SPAWN);	// 生成音
}

//============================================================
//	スポーン状態時の更新処理
//============================================================
CPlayer::EMotion CPlayer::UpdateSpawn(void)
{
	// 変数を宣言
	EMotion currentMotion = MOTION_IDOL;	// 現在のモーション

	// フェードアウト状態時の更新
	if (UpdateFadeOut(SPAWN_ADD_ALPHA))
	{ // 不透明になり切った場合

		// 状態を設定
		SetState(STATE_NORMAL);
	}

	// 現在のモーションを返す
	return currentMotion;
}

//============================================================
//	通常状態時の更新処理
//============================================================
CPlayer::EMotion CPlayer::UpdateNormal(void)
{
	// 変数を宣言
	EMotion currentMotion = MOTION_IDOL;		// 現在のモーション
	D3DXVECTOR3 posPlayer = GetVec3Position();	// プレイヤー位置
	D3DXVECTOR3 rotPlayer = GetVec3Rotation();	// プレイヤー向き

	// ポインタを宣言
	CStage *pStage = CScene::GetStage();	// ステージ情報
	if (pStage == nullptr)
	{ // ステージが使用されていない場合

		// 処理を抜ける
		assert(false);
		return MOTION_IDOL;
	}

	// 移動操作
	currentMotion = UpdateMove();

	// 重力の更新
	UpdateGravity();

	// 位置更新
	UpdatePosition(posPlayer);

	// 着地判定
	UpdateLanding(posPlayer);

	// 向き更新
	UpdateRotation(rotPlayer);

	// ステージ範囲外の補正
	pStage->LimitPosition(posPlayer, RADIUS);

	// 位置を反映
	SetVec3Position(posPlayer);

	// 向きを反映
	SetVec3Rotation(rotPlayer);

	// 現在のモーションを返す
	return currentMotion;
}

//============================================================
//	過去位置の更新処理
//============================================================
void CPlayer::UpdateOldPosition(void)
{
	// 過去位置を更新
	m_oldPos = GetVec3Position();
}

//============================================================
//	移動量・目標向きの更新処理
//============================================================
CPlayer::EMotion CPlayer::UpdateMove(void)
{
	// 移動量を更新
	m_move.x += sinf(m_destRot.y + D3DX_PI) * 1.0f;
	m_move.z += cosf(m_destRot.y + D3DX_PI) * 1.0f;

	// 目標向きを設定
	//m_destRot.y = atan2f(m_move.x, m_move.z);

	// 待機モーションを返す
	return MOTION_IDOL;
}

//============================================================
//	重力の更新処理
//============================================================
void CPlayer::UpdateGravity(void)
{
	// 重力を加算
	m_move.y -= GRAVITY;
}

//============================================================
//	着地状況の更新処理
//============================================================
bool CPlayer::UpdateLanding(D3DXVECTOR3& rPos)
{
	// 変数を宣言
	bool bLand = false;	// 着地状況

	// ジャンプしている状態にする
	m_bJump = true;

	// 地面・制限位置の着地判定
	if (CScene::GetStage()->LandFieldPosition(rPos, m_move)
	||  CScene::GetStage()->LandLimitPosition(rPos, m_move, 0.0f))
	{ // プレイヤーが着地していた場合

		// 着地している状態にする
		bLand = true;

		// ジャンプしていない状態にする
		m_bJump = false;
	}

	// 着地状況を返す
	return bLand;
}

//============================================================
//	位置の更新処理
//============================================================
void CPlayer::UpdatePosition(D3DXVECTOR3& rPos)
{
	// 移動量を加算
	rPos += m_move;

	// 移動量を減衰
	if (m_bJump)
	{ // 空中の場合

		m_move.x += (0.0f - m_move.x) * JUMP_REV;
		m_move.z += (0.0f - m_move.z) * JUMP_REV;
	}
	else
	{ // 地上の場合

		m_move.x += (0.0f - m_move.x) * LAND_REV;
		m_move.z += (0.0f - m_move.z) * LAND_REV;
	}
}

//============================================================
//	向きの更新処理
//============================================================
void CPlayer::UpdateRotation(D3DXVECTOR3& rRot)
{
	// 変数を宣言
	float fDiffRot = 0.0f;	// 差分向き

	// 目標向きの正規化
	useful::NormalizeRot(m_destRot.y);

	// 目標向きまでの差分を計算
	fDiffRot = m_destRot.y - rRot.y;

	// 差分向きの正規化
	useful::NormalizeRot(fDiffRot);

	// 向きの更新
	rRot.y += fDiffRot * REV_ROTA;

	// 向きの正規化
	useful::NormalizeRot(rRot.y);
}

//============================================================
//	モーション・オブジェクトキャラクターの更新処理
//============================================================
void CPlayer::UpdateMotion(int nMotion)
{
	if (IsDeath())
	{ // 死亡している場合

		return;
	}

	// 変数を宣言
	int nAnimMotion = GetMotionType();	// 現在再生中のモーション

	if (nMotion != NONE_IDX)
	{ // モーションが設定されている場合

		if (IsMotionLoop(nAnimMotion))
		{ // ループするモーションだった場合

			if (nAnimMotion != nMotion)
			{ // 現在のモーションが再生中のモーションと一致しない場合

				// 現在のモーションの設定
				SetMotion(nMotion);
			}
		}
	}

	// オブジェクトキャラクターの更新
	CObjectChara::Update();
}

//============================================================
//	フェードアウト状態時の更新処理
//============================================================
bool CPlayer::UpdateFadeOut(const float fAdd)
{
	// 変数を宣言
	bool bAlpha = false;		// 透明状況
	float fAlpha = GetAlpha();	// 透明度

	// プレイヤー自身の描画を再開
	CObject::SetEnableDraw(true);

	// 透明度を上げる
	fAlpha += fAdd;

	if (fAlpha >= GetMaxAlpha())
	{ // 透明度が上がり切った場合

		// 透明度を補正
		fAlpha = GetMaxAlpha();

		// 不透明になり切った状態にする
		bAlpha = true;
	}

	// 透明度を設定
	SetAlpha(fAlpha);

	// 透明状況を返す
	return bAlpha;
}

//============================================================
//	フェードイン状態時の更新処理
//============================================================
bool CPlayer::UpdateFadeIn(const float fSub)
{
	// 変数を宣言
	bool bAlpha = false;		// 透明状況
	float fAlpha = GetAlpha();	// 透明度

	// 透明度を下げる
	fAlpha -= fSub;

	if (fAlpha <= 0.0f)
	{ // 透明度が下がり切った場合

		// 透明度を補正
		fAlpha = 0.0f;

		// 透明になり切った状態にする
		bAlpha = true;

		// プレイヤー自身の描画を停止
		CObject::SetEnableDraw(false);
	}

	// 透明度を設定
	SetAlpha(fAlpha);

	// 透明状況を返す
	return bAlpha;
}

//============================================================
//	セットアップ処理
//============================================================
void CPlayer::LoadSetup(void)
{
	// 変数を宣言
	CMotion::SMotionInfo info;		// ポーズの代入用
	D3DXVECTOR3 pos = VEC3_ZERO;	// 位置の代入用
	D3DXVECTOR3 rot = VEC3_ZERO;	// 向きの代入用
	int nID			= 0;	// インデックスの代入用
	int nParentID	= 0;	// 親インデックスの代入用
	int nNowPose	= 0;	// 現在のポーズ番号
	int nNowKey		= 0;	// 現在のキー番号
	int nLoop		= 0;	// ループのON/OFFの変換用
	int nEnd		= 0;	// テキスト読み込み終了の確認用

	// 変数配列を宣言
	char aString[MAX_STRING];	// テキストの文字列の代入用

	// ポインタを宣言
	FILE *pFile;	// ファイルポインタ

	// ポーズ代入用の変数を初期化
	memset(&info, 0, sizeof(info));

	// ファイルを読み込み形式で開く
	pFile = fopen(SETUP_TXT, "r");

	if (pFile != nullptr)
	{ // ファイルが開けた場合

		do
		{ // 読み込んだ文字列が EOF ではない場合ループ

			// ファイルから文字列を読み込む
			nEnd = fscanf(pFile, "%s", &aString[0]);	// テキストを読み込みきったら EOF を返す

			// キャラクターの設定
			if (strcmp(&aString[0], "CHARACTERSET") == 0)
			{ // 読み込んだ文字列が CHARACTERSET の場合

				do
				{ // 読み込んだ文字列が END_CHARACTERSET ではない場合ループ

					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "PARTSSET") == 0)
					{ // 読み込んだ文字列が PARTSSET の場合

						do
						{ // 読み込んだ文字列が END_PARTSSET ではない場合ループ

							// ファイルから文字列を読み込む
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "INDEX") == 0)
							{ // 読み込んだ文字列が INDEX の場合

								fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
								fscanf(pFile, "%d", &nID);			// モデルのインデックスを読み込む
							}
							else if (strcmp(&aString[0], "PARENT") == 0)
							{ // 読み込んだ文字列が PARENT の場合

								fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
								fscanf(pFile, "%d", &nParentID);	// モデルの親のインデックスを読み込む
							}
							else if (strcmp(&aString[0], "POS") == 0)
							{ // 読み込んだ文字列が POS の場合

								fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
								fscanf(pFile, "%f", &pos.x);		// X座標を読み込む
								fscanf(pFile, "%f", &pos.y);		// Y座標を読み込む
								fscanf(pFile, "%f", &pos.z);		// Z座標を読み込む
							}
							else if (strcmp(&aString[0], "ROT") == 0)
							{ // 読み込んだ文字列が ROT の場合

								fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
								fscanf(pFile, "%f", &rot.x);		// X向きを読み込む
								fscanf(pFile, "%f", &rot.y);		// Y向きを読み込む
								fscanf(pFile, "%f", &rot.z);		// Z向きを読み込む
							}
						} while (strcmp(&aString[0], "END_PARTSSET") != 0);	// 読み込んだ文字列が END_PARTSSET ではない場合ループ

						// パーツ情報の設定
						CObjectChara::SetPartsInfo(nID, nParentID, pos, rot, MODEL_FILE[nID]);
					}
				} while (strcmp(&aString[0], "END_CHARACTERSET") != 0);		// 読み込んだ文字列が END_CHARACTERSET ではない場合ループ
			}

			// モーションの設定
			else if (strcmp(&aString[0], "MOTIONSET") == 0)
			{ // 読み込んだ文字列が MOTIONSET の場合

				// 現在のポーズ番号を初期化
				nNowPose = 0;

				do
				{ // 読み込んだ文字列が END_MOTIONSET ではない場合ループ

					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "LOOP") == 0)
					{ // 読み込んだ文字列が LOOP の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%d", &nLoop);		// ループのON/OFFを読み込む

						// 読み込んだ値をbool型に変換
						info.bLoop = (nLoop == 0) ? false : true;
					}
					else if (strcmp(&aString[0], "NUM_KEY") == 0)
					{ // 読み込んだ文字列が NUM_KEY の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%d", &info.nNumKey);	// キーの総数を読み込む
					}
					else if (strcmp(&aString[0], "KEYSET") == 0)
					{ // 読み込んだ文字列が KEYSET の場合

						// 現在のキー番号を初期化
						nNowKey = 0;

						do
						{ // 読み込んだ文字列が END_KEYSET ではない場合ループ

							// ファイルから文字列を読み込む
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "FRAME") == 0)
							{ // 読み込んだ文字列が FRAME の場合

								fscanf(pFile, "%s", &aString[0]);						// = を読み込む (不要)
								fscanf(pFile, "%d", &info.aKeyInfo[nNowPose].nFrame);	// キーが切り替わるまでのフレーム数を読み込む
							}
							else if (strcmp(&aString[0], "KEY") == 0)
							{ // 読み込んだ文字列が KEY の場合

								do
								{ // 読み込んだ文字列が END_KEY ではない場合ループ

									// ファイルから文字列を読み込む
									fscanf(pFile, "%s", &aString[0]);

									if (strcmp(&aString[0], "POS") == 0)
									{ // 読み込んだ文字列が POS の場合

										fscanf(pFile, "%s", &aString[0]);									// = を読み込む (不要)
										fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].pos.x);	// X位置を読み込む
										fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].pos.y);	// Y位置を読み込む
										fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].pos.z);	// Z位置を読み込む

										// 読み込んだ位置にパーツの初期位置を加算
										info.aKeyInfo[nNowPose].aKey[nNowKey].pos += GetPartsPosition(nNowKey);
									}
									else if (strcmp(&aString[0], "ROT") == 0)
									{ // 読み込んだ文字列が ROT の場合

										fscanf(pFile, "%s", &aString[0]);									// = を読み込む (不要)
										fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].rot.x);	// X向きを読み込む
										fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].rot.y);	// Y向きを読み込む
										fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].rot.z);	// Z向きを読み込む

										// 読み込んだ向きにパーツの初期向きを加算
										info.aKeyInfo[nNowPose].aKey[nNowKey].rot += GetPartsRotation(nNowKey);

										// 初期向きを正規化
										useful::NormalizeRot(info.aKeyInfo[nNowPose].aKey[nNowKey].rot.x);
										useful::NormalizeRot(info.aKeyInfo[nNowPose].aKey[nNowKey].rot.y);
										useful::NormalizeRot(info.aKeyInfo[nNowPose].aKey[nNowKey].rot.z);
									}

								} while (strcmp(&aString[0], "END_KEY") != 0);	// 読み込んだ文字列が END_KEY ではない場合ループ

								// 現在のキー番号を加算
								nNowKey++;
							}
						} while (strcmp(&aString[0], "END_KEYSET") != 0);	// 読み込んだ文字列が END_KEYSET ではない場合ループ

						// 現在のポーズ番号を加算
						nNowPose++;
					}
				} while (strcmp(&aString[0], "END_MOTIONSET") != 0);	// 読み込んだ文字列が END_MOTIONSET ではない場合ループ

				// モーション情報の設定
				CObjectChara::SetMotionInfo(info);
			}
		} while (nEnd != EOF);	// 読み込んだ文字列が EOF ではない場合ループ
		
		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(nullptr, "プレイヤーセットアップファイルの読み込みに失敗！", "警告！", MB_ICONWARNING);
	}
}
