//============================================================
//
//	スクロールメッシュリング処理 [scrollMeshRing.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "scrollMeshRing.h"

//************************************************************
//	子クラス [CScrollMeshRing] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CScrollMeshRing::CScrollMeshRing(const CObject::ELabel label, const EDim dimension, const int nPriority) : CObjectMeshRing(label, dimension, nPriority),
	m_fTexU		(0.0f),	// テクスチャ横座標の開始位置
	m_fTexV		(0.0f),	// テクスチャ縦座標の開始位置
	m_fMoveU	(0.0f),	// テクスチャ横座標の移動量
	m_fMoveV	(0.0f)	// テクスチャ縦座標の移動量
{

}

//============================================================
//	デストラクタ
//============================================================
CScrollMeshRing::~CScrollMeshRing()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CScrollMeshRing::Init(void)
{
	// メンバ変数を初期化
	m_fTexU  = 0.0f;	// テクスチャ横座標の開始位置
	m_fTexV  = 0.0f;	// テクスチャ縦座標の開始位置
	m_fMoveU = 0.0f;	// テクスチャ横座標の移動量
	m_fMoveV = 0.0f;	// テクスチャ縦座標の移動量

	// メッシュリングの初期化
	if (FAILED(CObjectMeshRing::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// スクロールのテクスチャ座標の設定
	CObjectMeshRing::SetScrollTex(m_fTexU, m_fTexV);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CScrollMeshRing::Uninit(void)
{
	// メッシュリングの終了
	CObjectMeshRing::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CScrollMeshRing::Update(void)
{
	// スクロールを加算
	m_fTexU += m_fMoveU;
	m_fTexV += m_fMoveV;

	if (m_fTexU > 1.0f)
	{ // 1.0fより大きくなった場合

		// 開始地点を補正
		m_fTexU -= 1.0f;
	}

	if (m_fTexV > 1.0f)
	{ // 1.0fより大きくなった場合

		// 開始地点を補正
		m_fTexV -= 1.0f;
	}

	// メッシュリングの更新
	CObjectMeshRing::Update();

	// スクロールのテクスチャ座標の設定
	CObjectMeshRing::SetScrollTex(m_fTexU, m_fTexV);
}

//============================================================
//	描画処理
//============================================================
void CScrollMeshRing::Draw(void)
{
	// メッシュリングの描画
	CObjectMeshRing::Draw();
}

//============================================================
//	生成処理
//============================================================
CScrollMeshRing *CScrollMeshRing::Create
(
	const float fMoveU,			// 横座標の移動量
	const float fMoveV,			// 縦座標の移動量
	const D3DXVECTOR3& rPos,	// 位置
	const D3DXVECTOR3& rRot,	// 向き
	const D3DXVECTOR2& rSize,	// 大きさ
	const D3DXCOLOR& rCol,		// 色
	const POSGRID2& rPart		// 分割数
)
{
	// スクロールメッシュリングの生成
	CScrollMeshRing *pScrollMeshRing = new CScrollMeshRing;
	if (pScrollMeshRing == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// スクロールメッシュリングの初期化
		if (FAILED(pScrollMeshRing->Init()))
		{ // 初期化に失敗した場合

			// スクロールメッシュリングの破棄
			SAFE_DELETE(pScrollMeshRing);
			return nullptr;
		}

		// 位置を設定
		pScrollMeshRing->SetVec3Position(rPos);

		// 向きを設定
		pScrollMeshRing->SetVec3Rotation(rRot);

		// 大きさを設定
		pScrollMeshRing->SetVec2Sizing(rSize);

		// 色を設定
		pScrollMeshRing->SetColor(rCol);

		// 横座標の移動量を設定
		pScrollMeshRing->SetMoveU(fMoveU);

		// 縦座標の移動量を設定
		pScrollMeshRing->SetMoveV(fMoveV);

		// 分割数を設定
		if (FAILED(pScrollMeshRing->SetPattern(rPart)))
		{ // 分割数の設定に失敗した場合

			// スクロールメッシュリングの破棄
			SAFE_DELETE(pScrollMeshRing);
			return nullptr;
		}

		// 確保したアドレスを返す
		return pScrollMeshRing;
	}
}

//============================================================
//	横座標の開始位置設定処理
//============================================================
void CScrollMeshRing::SetTexU(const float fTexU)
{
	// 引数の横座標の開始位置を代入
	m_fTexU = fTexU;

	// スクロールのテクスチャ座標の設定
	CObjectMeshRing::SetScrollTex(m_fTexU, m_fTexV);
}

//============================================================
//	縦座標の開始位置設定処理
//============================================================
void CScrollMeshRing::SetTexV(const float fTexV)
{
	// 引数の縦座標の開始位置を代入
	m_fTexV = fTexV;

	// スクロールのテクスチャ座標の設定
	CObjectMeshRing::SetScrollTex(m_fTexU, m_fTexV);
}

//============================================================
//	横座標の移動量設定処理
//============================================================
void CScrollMeshRing::SetMoveU(const float fMoveU)
{
	// 引数の横座標の移動量を代入
	m_fMoveU = fMoveU;

	// スクロールのテクスチャ座標の設定
	CObjectMeshRing::SetScrollTex(m_fTexU, m_fTexV);
}

//============================================================
//	横座標の移動量取得処理
//============================================================
float CScrollMeshRing::GetMoveU(void) const
{
	// テクスチャ横座標の移動量を返す
	return m_fMoveU;
}

//============================================================
//	縦座標の移動量設定処理
//============================================================
void CScrollMeshRing::SetMoveV(const float fMoveV)
{
	// 引数の縦座標の移動量を代入
	m_fMoveV = fMoveV;

	// スクロールのテクスチャ座標の設定
	CObjectMeshRing::SetScrollTex(m_fTexU, m_fTexV);
}

//============================================================
//	縦座標の移動量取得処理
//============================================================
float CScrollMeshRing::GetMoveV(void) const
{
	// テクスチャ縦座標の移動量を返す
	return m_fMoveV;
}
