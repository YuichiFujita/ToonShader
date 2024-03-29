//============================================================
//
//	ゲーム画面処理 [sceneGame.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "sceneGame.h"
#include "manager.h"
#include "sound.h"
#include "camera.h"

#include "gameManager.h"
#include "timerManager.h"
#include "stage.h"
#include "pause.h"
#include "player.h"

// TODO
#include "teapot.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	namespace timerUI
	{
#ifdef _DEBUG

		const int TIME_LIMIT = 0;	// 制限時間

#else	// NDEBUG

		const int TIME_LIMIT = 240;	// 制限時間

#endif	// _DEBUG

		const D3DXVECTOR3 POS			= D3DXVECTOR3(710.0f, 50.0f, 0.0f);	// タイマー位置
		const D3DXVECTOR3 VAL_SIZE		= D3DXVECTOR3(72.0f, 96.0f, 0.0f);	// タイマー数字大きさ
		const D3DXVECTOR3 PART_SIZE		= D3DXVECTOR3(42.0f, 96.0f, 0.0f);	// タイマー区切り大きさ
		const D3DXVECTOR3 VAL_SPACE		= D3DXVECTOR3(VAL_SIZE.x * 0.85f, 0.0f, 0.0f);	// タイマー数字空白
		const D3DXVECTOR3 PART_SPACE	= D3DXVECTOR3(PART_SIZE.x * 0.85f, 0.0f, 0.0f);	// タイマー区切り空白
	}
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CGameManager	*CSceneGame::m_pGameManager		= nullptr;	// ゲームマネージャー
CTimerManager	*CSceneGame::m_pTimerManager	= nullptr;	// タイマーマネージャー
CPause			*CSceneGame::m_pPause			= nullptr;	// ポーズ

//************************************************************
//	子クラス [CSceneGame] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CSceneGame::CSceneGame(const EMode mode) : CScene(mode)
{

}

//============================================================
//	デストラクタ
//============================================================
CSceneGame::~CSceneGame()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CSceneGame::Init(void)
{
	//--------------------------------------------------------
	//	初期生成
	//--------------------------------------------------------
	// タイマーマネージャーの生成
	m_pTimerManager = CTimerManager::Create
	( // 引数
		CTimerManager::TIME_SEC,	// 設定タイム
		timerUI::TIME_LIMIT,		// 制限時間
		timerUI::POS,				// 位置
		timerUI::VAL_SIZE,			// 数字の大きさ
		timerUI::PART_SIZE,			// 区切りの大きさ
		timerUI::VAL_SPACE,			// 数字の空白
		timerUI::PART_SPACE			// 区切りの空白
	);
	if (m_pTimerManager == nullptr)
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// ロゴの自動描画をONにする
	m_pTimerManager->SetEnableLogoDraw(true);

	// シーンの初期化
	CScene::Init();		// ステージ・プレイヤーの生成

	// ゲームマネージャーの生成
	m_pGameManager = CGameManager::Create();
	if (m_pGameManager == nullptr)
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// ポーズの生成
	m_pPause = CPause::Create();
	if (m_pPause == nullptr)
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// TODO
	CTeapot::Create(D3DXVECTOR3(0.0f, 100.0f, 0.0f), VEC3_ZERO);

	//--------------------------------------------------------
	//	初期設定
	//--------------------------------------------------------
	// カメラを設定
	GET_MANAGER->GetCamera()->SetState(CCamera::STATE_FOLLOW);	// カメラを追従状態に設定	// TODO

	// プレイヤーを出現
	CScene::GetPlayer()->SetSpawn();

	// BGMの再生
	GET_MANAGER->GetSound()->Play(CSound::LABEL_BGM_GAME);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CSceneGame::Uninit(void)
{
	// ゲームマネージャーの破棄
	SAFE_REF_RELEASE(m_pGameManager);

	// タイマーマネージャーの破棄
	SAFE_REF_RELEASE(m_pTimerManager);

	// ポーズの破棄
	SAFE_REF_RELEASE(m_pPause);

	// シーンの終了
	CScene::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CSceneGame::Update(void)
{
	// ゲームマネージャーの更新
	assert(m_pGameManager != nullptr);
	m_pGameManager->Update();

	// タイマーマネージャーの更新
	assert(m_pTimerManager != nullptr);
	m_pTimerManager->Update();

	// ポーズの更新
	assert(m_pPause != nullptr);
	m_pPause->Update();

	if (!m_pPause->IsPause())
	{ // ポーズ中ではない場合

		// シーンの更新
		CScene::Update();
	}

#ifdef _DEBUG

	else
	{ // ポーズ中の場合

		if (GET_MANAGER->GetCamera()->GetState() == CCamera::STATE_CONTROL)
		{ // カメラが操作状態の場合

			// カメラの更新
			GET_MANAGER->GetCamera()->Update();
		}
	}

#endif	// _DEBUG
}

//============================================================
//	描画処理
//============================================================
void CSceneGame::Draw(void)
{

}

//============================================================
//	ゲームマネージャー取得処理
//============================================================
CGameManager *CSceneGame::GetGameManager(void)
{
	// インスタンス未使用
	assert(m_pGameManager != nullptr);

	// ゲームマネージャーのポインタを返す
	return m_pGameManager;
}

//============================================================
//	タイマーマネージャー取得処理
//============================================================
CTimerManager *CSceneGame::GetTimerManager(void)
{
	// インスタンス未使用
	assert(m_pTimerManager != nullptr);

	// タイマーマネージャーのポインタを返す
	return m_pTimerManager;
}

//============================================================
//	ポーズ取得処理
//============================================================
CPause *CSceneGame::GetPause(void)
{
	// インスタンス未使用
	assert(m_pPause != nullptr);

	// ポーズのポインタを返す
	return m_pPause;
}
