//============================================================
//
//	エッジシェーダーヘッダー [shaderEdge.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "shaderEdge.h"
#include "manager.h"
#include "renderer.h"
#include "object2D.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char* EFFECT_FX = "shaderEdgeUE5.fx";	// エッジシェーダーのエフェクトファイル
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CEdgeShader *CEdgeShader::m_pShader = nullptr;	// シェーダー情報

//************************************************************
//	親クラス [CEdgeShader] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEdgeShader::CEdgeShader() :
	m_pDrawEdge	(nullptr),	// 縁取り描画用の2Dポリゴン
	m_pTexel	(nullptr),	// テクセルサイズ
	m_pLimit	(nullptr)	// エッジ基準値
{

}

//============================================================
//	デストラクタ
//============================================================
CEdgeShader::~CEdgeShader()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CEdgeShader::Init(void)
{
	// 変数を宣言
	D3DCAPS9 caps;	// ハードウェア機能
	HRESULT hr;		// 異常終了の確認用

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイス情報
	LPD3DXBUFFER pError  = nullptr;	// コンパイルエラー情報
	LPD3DXEFFECT pEffect = nullptr;	// エフェクト設定用

	// メンバ変数を初期化
	m_pDrawEdge = nullptr;	// 縁取り描画用の2Dポリゴン
	m_pTexel	= nullptr;	// テクセルサイズ
	m_pLimit	= nullptr;	// エッジ基準値

	// シェーダーの初期化
	if (FAILED(CShader::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 縁取り描画用の2Dポリゴンの生成
	m_pDrawEdge = CObject2D::Create(SCREEN_CENT, SCREEN_SIZE);
	if (m_pDrawEdge == nullptr)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// ラベルをスクリーンに設定
	m_pDrawEdge->SetLabel(CObject::LABEL_SCREEN);

	// 自動描画をOFFにする
	m_pDrawEdge->SetEnableDraw(false);

	// ハードウェア機能を取得
	pDevice->GetDeviceCaps(&caps);

	if (caps.VertexShaderVersion >= D3DVS_VERSION(1, 1)
	&&  caps.PixelShaderVersion  >= D3DPS_VERSION(2, 0))
	{ // 頂点・ピクセルシェーダのバージョンが使用可能な場合

		// エフェクトファイルの読込
		hr = D3DXCreateEffectFromFile
		( // 引数
			pDevice,	// デバイスへのポインタ
			EFFECT_FX,	// エフェクトファイル
			nullptr,	// プリプロセッサ定義
			nullptr,	// インクルード操作
			0,			// 読込オプションフラグ
			nullptr,	// グローバル変数インターフェース
			&pEffect,	// エフェクトインターフェース
			&pError		// コンパイルエラー情報
		);
		if (SUCCEEDED(hr))
		{ // 読込に成功した場合

			// エフェクト情報を設定
			SetEffect(pEffect);

			// テクニック関数を取得・設定
			SetTechnique(pEffect->GetTechniqueByName("TShader"));

			// グローバル変数を取得
			m_pTexel = pEffect->GetParameterByName(nullptr, "m_Texel");	// テクセルサイズ
			m_pLimit = pEffect->GetParameterByName(nullptr, "m_Limit");	// エッジ基準値

			// テクセルサイズを設定
			SetTexelSize(TEXEL_SIZE);
		}
		else
		{ // 読込に失敗した場合

			// 読み込み失敗を返す
			assert(false);
			return E_FAIL;
		}
	}
	else
	{ // バージョンが使用不可な場合

		// 古いバージョンによる失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CEdgeShader::Uninit(void)
{
	// 縁取り描画用の2Dポリゴンの終了
	SAFE_UNINIT(m_pDrawEdge);

	// シェーダーの終了
	CShader::Uninit();
}

//============================================================
//	縁取りレンダリング処理
//============================================================
void CEdgeShader::Render(LPDIRECT3DTEXTURE9 pBackBuffer, LPDIRECT3DTEXTURE9 pZBuffer)
{
	if (!IsEffectOK()) { assert(false); return; }	// エフェクト未使用

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイス情報

	pDevice->SetTexture(0, pBackBuffer);
	pDevice->SetTexture(1, pZBuffer);

	Begin();
	BeginPass(0);

	m_pDrawEdge->Draw();

	EndPass();
	End();

	//pDevice->SetTexture(0, nullptr);
	pDevice->SetTexture(1, nullptr);
}

//============================================================
//	エッジ基準値の設定処理
//============================================================
void CEdgeShader::SetRefValue(const float fLimit)
{
	if (!IsEffectOK()) { assert(false); return; }	// エフェクト未使用

	// エフェクトにエッジ基準値を設定
	GetEffect()->SetFloat(m_pLimit, fLimit);
}

//============================================================
//	生成処理
//============================================================
CEdgeShader *CEdgeShader::Create(void)
{
	// インスタンス使用中
	assert(m_pShader == nullptr);

	// エッジシェーダーの生成
	m_pShader = new CEdgeShader;
	if (m_pShader == nullptr)
	{ // 生成に失敗した場合

		assert(false);
		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// エッジシェーダーの初期化
		if (FAILED(m_pShader->Init()))
		{ // 初期化に失敗した場合

			// エッジシェーダーの破棄
			SAFE_DELETE(m_pShader);
			return nullptr;
		}

		// 確保したアドレスを返す
		return m_pShader;
	}
}

//============================================================
//	取得処理
//============================================================
CEdgeShader *CEdgeShader::GetInstance(void)
{
	// インスタンス未使用
	assert(m_pShader != nullptr);

	// エッジシェーダーのポインタを返す
	return m_pShader;
}

//============================================================
//	破棄処理
//============================================================
void CEdgeShader::Release(void)
{
	// エッジシェーダーの終了
	assert(m_pShader != nullptr);
	m_pShader->Uninit();

	// メモリ開放
	SAFE_DELETE(m_pShader);
}

//============================================================
//	テクセルサイズの設定処理
//============================================================
void CEdgeShader::SetTexelSize(const D3DXVECTOR2& rTexel)
{
	if (!IsEffectOK()) { assert(false); return; }	// エフェクト未使用

	// エフェクトにテクセルサイズを設定
	GetEffect()->SetValue(m_pTexel, (LPCVOID)&rTexel, sizeof(float) * 2);
}
