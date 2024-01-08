//============================================================
//
//	エッジシェーダーヘッダー [shaderEdge.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _SHADER_EDGE_H_
#define _SHADER_EDGE_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "shader.h"

//************************************************************
//	前方宣言
//************************************************************
class CObject2D;	// オブジェクト2Dクラス

//************************************************************
//	クラス定義
//************************************************************
// エッジシェーダークラス
class CEdgeShader : public CShader
{
public:
	// コンストラクタ
	CEdgeShader();

	// デストラクタ
	~CEdgeShader() override;

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了

	// メンバ関数
	void Render(LPDIRECT3DTEXTURE9 pBackBuffer, LPDIRECT3DTEXTURE9 pZBuffer);	// 縁取りレンダリング
	void SetRefValue(const float fLimit);	// エッジ基準値設定

	// 静的メンバ関数
	static CEdgeShader *Create(void);		// 生成
	static CEdgeShader *GetInstance(void);	// 取得
	static void Release(void);				// 破棄

private:
	// メンバ関数
	void SetTexelSize(const D3DXVECTOR2& rTexel);	// テクセルサイズ設定

	// 静的メンバ変数
	static CEdgeShader *m_pShader;	// シェーダー情報

	// メンバ変数
	CObject2D *m_pDrawEdge;	// 縁取り描画用の2Dポリゴン
	D3DXHANDLE m_pTexel;	// テクセルサイズ
	D3DXHANDLE m_pLimit;	// エッジ基準値
};

#endif	// _SHADER_H_
