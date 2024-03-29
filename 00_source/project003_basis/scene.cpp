//============================================================
//
//	シーン処理 [scene.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "scene.h"
#include "manager.h"
#include "renderer.h"
#include "light.h"
#include "camera.h"
#include "sceneTitle.h"
#include "sceneTutorial.h"
#include "sceneGame.h"
#include "sceneResult.h"
#include "sceneRanking.h"

#include "stage.h"
#include "player.h"

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CStage	*CScene::m_pStage	= nullptr;	// ステージ
CPlayer	*CScene::m_pPlayer	= nullptr;	// プレイヤーオブジェクト

//************************************************************
//	親クラス [CScene] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CScene::CScene(const EMode mode) : m_mode(mode)
{

}

//============================================================
//	デストラクタ
//============================================================
CScene::~CScene()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CScene::Init(void)
{
	// ステージの生成
	m_pStage = CStage::Create(m_mode);
	if (m_pStage == nullptr)
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// プレイヤーオブジェクトの生成
	m_pPlayer = CPlayer::Create(m_mode);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CScene::Uninit(void)
{
	// ステージの破棄
	SAFE_REF_RELEASE(m_pStage);

	// 終了済みポインタをNULLにする
	m_pPlayer = nullptr;	// プレイヤーオブジェクト
}

//============================================================
//	更新処理
//============================================================
void CScene::Update(void)
{
	// ポインタを宣言
	CManager	*pManager	= GET_MANAGER;				// マネージャー
	CLight		*pLight		= pManager->GetLight();		// ライト
	CCamera		*pCamera	= pManager->GetCamera();	// カメラ
	CRenderer	*pRenderer	= pManager->GetRenderer();	// レンダラー

	// ステージの更新
	assert(m_pStage != nullptr);
	m_pStage->Update();

	// ライトの更新
	assert(pLight != nullptr);
	pLight->Update();

	// カメラの更新
	assert(pCamera != nullptr);
	pCamera->Update();

	// レンダラーの更新
	assert(pRenderer != nullptr);
	pRenderer->Update();
}

//============================================================
//	生成処理
//============================================================
CScene *CScene::Create(EMode mode)
{
	// ポインタを宣言
	CScene *pScene = nullptr;	// シーン情報

	// シーンの生成
	switch (mode)
	{ // モードごとの処理
	case MODE_TITLE:

		// タイトル画面を生成
		pScene = new CSceneTitle(mode);

		break;

	case MODE_TUTORIAL:

		// チュートリアル画面を生成
		pScene = new CSceneTutorial(mode);

		break;

	case MODE_GAME:

		// ゲーム画面を生成
		pScene = new CSceneGame(mode);

		break;

	case MODE_RESULT:

		// リザルト画面を生成
		pScene = new CSceneResult(mode);

		break;

	case MODE_RANKING:

		// ランキング画面を生成
		pScene = new CSceneRanking(mode);

		break;

	default:	// 例外処理
		assert(false);
		break;
	}

	if (pScene == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// シーンの初期化
		if (FAILED(pScene->Init()))
		{ // 初期化に失敗した場合

			// プレイヤーの破棄
			SAFE_DELETE(pScene);
			return nullptr;
		}

		// 確保したアドレスを返す
		return pScene;
	}
}

//============================================================
//	破棄処理
//============================================================
void CScene::Release(CScene *&prScene)
{
	// シーンの終了
	assert(prScene != nullptr);
	prScene->Uninit();

	// メモリ開放
	SAFE_DELETE(prScene);
}

//============================================================
//	ステージ取得処理
//============================================================
CStage *CScene::GetStage(void)
{
	// インスタンス未使用
	assert(m_pStage != nullptr);

	// ステージのポインタを返す
	return m_pStage;
}

//============================================================
//	プレイヤー取得処理
//============================================================
CPlayer *CScene::GetPlayer(void)
{
	// プレイヤーのポインタを返す
	return m_pPlayer;
}

//============================================================
//	モードの設定処理
//============================================================
void CScene::SetMode(const EMode mode)
{
	// 引数のモードを設定
	m_mode = mode;
}

//============================================================
//	モード取得処理
//============================================================
CScene::EMode CScene::GetMode(void) const
{
	// 現在のモードを返す
	return m_mode;
}
