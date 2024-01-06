//============================================================
//
//	スクロールメッシュフィールドヘッダー [scrollMeshField.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _SCROLL_MESHFIELD_H_
#define _SCROLL_MESHFIELD_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "objectMeshField.h"

//************************************************************
//	クラス定義
//************************************************************
// スクロールメッシュフィールドクラス
class CScrollMeshField : public CObjectMeshField
{
public:
	// コンストラクタ
	explicit CScrollMeshField(const CObject::ELabel label = LABEL_NONE, const EDim dimension = DIM_3D, const int nPriority = object::DEFAULT_PRIO);

	// デストラクタ
	~CScrollMeshField() override;

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画

	// 静的メンバ関数
	static CScrollMeshField *Create	// 生成
	( // 引数
		const float fMoveU,			// 横座標の移動量
		const float fMoveV,			// 縦座標の移動量
		const D3DXVECTOR3& rPos,	// 位置
		const D3DXVECTOR3& rRot,	// 向き
		const D3DXVECTOR2& rSize,	// 大きさ
		const D3DXCOLOR& rCol,		// 色
		const POSGRID2& rPart		// 分割数
	);

	// メンバ関数
	void SetTexU(const float fTexU);	// 横座標の開始位置設定
	void SetTexV(const float fTexV);	// 縦座標の開始位置設定
	void SetMoveU(const float fMoveU);	// 横座標の移動量設定
	float GetMoveU(void) const;			// 横座標の移動量取得
	void SetMoveV(const float fMoveV);	// 縦座標の移動量設定
	float GetMoveV(void) const;			// 縦座標の移動量取得

private:
	// メンバ変数
	float m_fTexU;	// テクスチャ横座標の開始位置
	float m_fTexV;	// テクスチャ縦座標の開始位置
	float m_fMoveU;	// テクスチャ横座標の移動量
	float m_fMoveV;	// テクスチャ縦座標の移動量
};

#endif	// _SCROLL_MESHFIELD_H_
