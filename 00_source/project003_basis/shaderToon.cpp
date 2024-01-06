//============================================================
//
//	トゥーンシェーダーヘッダー [shaderToon.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "shaderToon.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char* EFFECT_FX = "Normal.fx";	// バンプマップのエフェクトファイル
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CToonShader *CToonShader::m_pShader = nullptr;	// シェーダー情報

//************************************************************
//	親クラス [CToonShader] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CToonShader::CToonShader() :
	m_pLightLocalDirect	(nullptr),	// 平行光源の方向ベクトル
	m_pCameraVec		(nullptr)	// カメラベクトル
{

}

//============================================================
//	デストラクタ
//============================================================
CToonShader::~CToonShader()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CToonShader::Init(void)
{
	// 変数を宣言
	D3DCAPS9 caps;	// ハードウェア機能
	HRESULT hr;		// 異常終了の確認用

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイス情報
	LPD3DXBUFFER pError = nullptr;	// コンパイルエラー情報
	LPD3DXEFFECT pEffect = nullptr;	// エフェクト設定用

	// メンバ変数を初期化
	m_pLightLocalDirect	= nullptr;	// 平行光源の方向ベクトル
	m_pCameraVec		= nullptr;	// カメラベクトル

	// シェーダーの初期化
	if (FAILED(CShader::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// ハードウェア機能を取得
	pDevice->GetDeviceCaps(&caps);

	if (caps.VertexShaderVersion >= D3DVS_VERSION(2, 0)
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
			SetTechnique(pEffect->GetTechniqueByName("NormalMap"));

			// 各マトリックスのグローバル変数を取得・設定
			SetMatrixWorld(pEffect->GetParameterByName(nullptr, "g_worldMat"));		// ワールドマトリックス
			SetMatrixView(pEffect->GetParameterByName(nullptr, "g_viewMat"));		// ビューマトリックス
			SetMatrixProjection(pEffect->GetParameterByName(nullptr, "g_projMat"));	// プロジェクションマトリックス

			// グローバル変数を取得
			m_pLightLocalDirect	= pEffect->GetParameterByName(nullptr, "g_LightLocalDirect");	// 平行光源の方向ベクトル
			m_pCameraVec		= pEffect->GetParameterByName(nullptr, "cameraVec");			// カメラベクトル
		}
		else
		{ // 読込に失敗した場合

			// 読み込み失敗を返す
			return E_FAIL;
		}
	}
	else
	{ // バージョンが使用不可な場合

		// 古いバージョンによる失敗を返す
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CToonShader::Uninit(void)
{
	// シェーダーの終了
	CShader::Uninit();
}

//============================================================
//	生成処理
//============================================================
CToonShader *CToonShader::Create(void)
{
	// インスタンス使用中
	assert(m_pShader == nullptr);

	// トゥーンシェーダーの生成
	m_pShader = new CToonShader;
	if (m_pShader == nullptr)
	{ // 生成に失敗した場合

		assert(false);
		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// トゥーンシェーダーの初期化
		if (FAILED(m_pShader->Init()))
		{ // 初期化に失敗した場合

			// トゥーンシェーダーの破棄
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
CToonShader *CToonShader::GetInstance(void)
{
	// インスタンス未使用
	assert(m_pShader != nullptr);

	// トゥーンシェーダーのポインタを返す
	return m_pShader;
}

//============================================================
//	破棄処理
//============================================================
void CToonShader::Release(void)
{
	// トゥーンシェーダーの終了
	assert(m_pShader != nullptr);
	m_pShader->Uninit();

	// メモリ開放
	SAFE_DELETE(m_pShader);
}
