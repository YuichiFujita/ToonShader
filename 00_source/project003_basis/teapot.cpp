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
	const char *TEXTURE_FILE[] =	// テクスチャ定数
	{
		"dataSHADER\\\\TEXTURE\\normal5.png",	// 法線テクスチャ
	};
	const char *MODEL_FILE[] =		// モデル定数
	{
		"data\\MODEL\\TEAPOT\\teapot005.x",		// ティーポット
	};

	const int PRIORITY = 0;	// ティーポットの優先順位
}

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
	// 変数配列を宣言
	D3DVERTEXELEMENT9 aDecl[] =	// 頂点データ定義
	{
		{ 0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT,  0 },
		{ 0, 24, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0 },
		{ 0, 36, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0 },
		{ 0, 48, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		D3DDECL_END()
	};

	// オブジェクトモデルの初期化
	if (FAILED(CObjectModel::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// モデルを登録・割当
	BindModel(MODEL_FILE[MODEL_TEAPOT]);

	//// モデルの頂点データを変換
	//if (FAILED(SetVertexDecl(&aDecl[0])))
	//{ // 変換に失敗した場合

	//	// 失敗を返す
	//	return E_FAIL;
	//}

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
	// オブジェクトモデルの更新
	CObjectModel::Update();
}

//============================================================
//	描画処理
//============================================================
void CTeapot::Draw(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイス情報
	CTexture *pTexture = CManager::GetInstance()->GetTexture();	// テクスチャ情報
	CToonShader *pToonShader = CToonShader::GetInstance();		// トゥーンシェーダー情報

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

		// ステージ1のテクスチャを設定
		pDevice->SetTexture(1, pTexture->GetTexture(pTexture->Regist(TEXTURE_FILE[TEXTURE_NORMAL])));	// 法線テクスチャ

		// 描画開始
		pToonShader->Begin();

		// マトリックス情報を設定
		pToonShader->SetMatrix(GetPtrMtxWorld());

		// 状態変更の伝達
		pToonShader->CommitChanges();

		// パスを設定
		pToonShader->BeginPass(0);

		// オブジェクトモデルの描画
		CObjectModel::Draw();

		// 描画終了
		pToonShader->EndPass();
		pToonShader->End();

		// ステージ1のテクスチャを初期化
		pDevice->SetTexture(1, nullptr);
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
