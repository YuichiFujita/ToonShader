//============================================================
//
//	ティーポット処理 [teapot.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "teapot.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "shaderToon.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char *MODEL_FILE[] =	// モデル定数
	{
		"data\\MODEL\\TEAPOT\\teapot000.x",			// ティーポット
		//"data\\MODEL\\OBSTACLE\\obstacle025.x",	// ティーポット
	};

	const int PRIORITY = 0;	// ティーポットの優先順位
}

//************************************************************
//	スタティックアサート
//************************************************************
static_assert(NUM_ARRAY(MODEL_FILE) == CTeapot::MODEL_MAX, "ERROR : Model Count Mismatch");

//************************************************************
//	子クラス [CTeapot] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CTeapot::CTeapot() : CObjectModel(CObject::LABEL_TEAPOT, CObject::DIM_3D, PRIORITY)
{

}

//============================================================
//	デストラクタ
//============================================================
CTeapot::~CTeapot()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CTeapot::Init(void)
{
	// オブジェクトモデルの初期化
	if (FAILED(CObjectModel::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// モデルを登録・割当
	BindModel(MODEL_FILE[MODEL_TEAPOT]);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CTeapot::Uninit(void)
{
	// オブジェクトモデルの終了
	CObjectModel::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CTeapot::Update(void)
{
	SetVec3Rotation(GetVec3Rotation() + D3DXVECTOR3(0.01f, 0.01f, 0.01f));
	//SetAllMaterial(material::White());

	// オブジェクトモデルの更新
	CObjectModel::Update();
}

//============================================================
//	描画処理
//============================================================
void CTeapot::Draw(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice	= CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイス情報
	CTexture	*pTexture		= CManager::GetInstance()->GetTexture();	// テクスチャ情報
	CToonShader	*pToonShader	= CToonShader::GetInstance();	// トゥーンシェーダー情報

	if (pDevice == nullptr || pTexture == nullptr || pToonShader == nullptr)
	{ // 情報が無いものがあった場合

		// オブジェクトモデルの描画
		CObjectModel::Draw();

		// 処理を抜ける
		assert(false);
		return;
	}

	if (pToonShader->IsEffectOK())
	{ // エフェクトが使用可能な場合

		// 描画開始
		pToonShader->Begin();

		// マトリックス情報を設定
		pToonShader->SetMatrix(GetPtrMtxWorld());

		// ライト方向を設定
		pToonShader->SetLightDirect(GetPtrMtxWorld(), 0);

		// テクスチャ使用を設定
		pToonShader->SetUseTexture(GetModelData().pTextureID[0]);

		// マテリアルを設定
		pToonShader->SetMaterial(GetMaterial(0));

		// 状態変更の伝達
		pToonShader->CommitChanges();

		// パスを設定
		pToonShader->BeginPass(0);

		// オブジェクトモデルの描画
		CObjectModel::Draw();

		// 描画終了
		pToonShader->EndPass();
		pToonShader->End();
	}
}

//============================================================
//	生成処理
//============================================================
CTeapot *CTeapot::Create
(
	const D3DXVECTOR3& rPos,	// 位置
	const D3DXVECTOR3& rRot		// 向き
)
{
	// ポインタを宣言
	CTeapot *pTeapot = nullptr;	// ティーポット生成用

	if (pTeapot == nullptr)
	{ // 使用されていない場合

		// メモリ確保
		pTeapot = new CTeapot;	// ティーポット
	}
	else { assert(false); return nullptr; }	// 使用中

	if (pTeapot != nullptr)
	{ // 確保に成功している場合

		// ティーポットの初期化
		if (FAILED(pTeapot->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pTeapot;
			pTeapot = nullptr;

			// 失敗を返す
			return nullptr;
		}

		// 位置を設定
		pTeapot->SetVec3Position(rPos);

		// 向きを設定
		pTeapot->SetVec3Rotation(rRot);

		// 確保したアドレスを返す
		return pTeapot;
	}
	else { assert(false); return nullptr; }	// 確保失敗
}
