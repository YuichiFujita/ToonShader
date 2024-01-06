//============================================================
//
//	トゥーンシェーダーヘッダー [shaderToon.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _SHADER_TOON_H_
#define _SHADER_TOON_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "shader.h"

//************************************************************
//	クラス定義
//************************************************************
// トゥーンシェーダークラス
class CToonShader : public CShader
{
public:
	// コンストラクタ
	CToonShader();

	// デストラクタ
	~CToonShader() override;

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了

	// 静的メンバ関数
	static CToonShader *Create(void);		// 生成
	static CToonShader *GetInstance(void);	// 取得
	static void Release(void);				// 破棄

private:
	// 静的メンバ変数
	static CToonShader *m_pShader;	// シェーダー情報

	// メンバ変数
	D3DXHANDLE m_pLightLocalDirect;	// 平行光源の方向ベクトル
	D3DXHANDLE m_pCameraVec;		// カメラベクトル
};

#endif	// _SHADER_H_
