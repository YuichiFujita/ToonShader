//============================================================
//
//	オブジェクトマネージャーヘッダー [objectManager.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _OBJECT_MANAGER_H_
#define _OBJECT_MANAGER_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include <list>

//************************************************************
//	クラス定義
//************************************************************
// オブジェクトマネージャークラス
template<class T> class CObjectManager
{
public:
	// コンストラクタ
	CObjectManager();

	// デストラクタ
	~CObjectManager();

	// イテレーター型エイリアス宣言
	using AIterator = std::_List_iterator<std::_List_val<std::_List_simple_types<T*>>>;

	// メンバ関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	AIterator AddList(T *pObject);				// リスト追加
	void DeleteList(const AIterator iterator);	// リスト削除
	int GetNumAll(void);			// リスト内の要素数取得
	std::list<T*> GetList(void);	// リスト取得

	// 静的メンバ関数
	static CObjectManager *Create(void);	// 生成
	static void Release(CObjectManager *&prObjectManager);	// 破棄

private:
	// メンバ変数
	std::list<T*> m_list;	// オブジェクトリスト
};

//************************************************************
//	親クラス [CObjectManager] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
template<class T> CObjectManager<T>::CObjectManager()
{
	// オブジェクトリストの全要素を削除
	m_list.clear();
}

//============================================================
//	デストラクタ
//============================================================
template<class T> CObjectManager<T>::~CObjectManager()
{

}

//============================================================
//	初期化処理
//============================================================
template<class T> HRESULT CObjectManager<T>::Init(void)
{
	// オブジェクトリストの全要素を削除
	m_list.clear();

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
template<class T> void CObjectManager<T>::Uninit(void)
{
	// オブジェクトリストの全要素を削除
	m_list.clear();
}

//============================================================
//	リスト追加処理
//============================================================
template<class T>
std::_List_iterator<std::_List_val<std::_List_simple_types<T*>>> CObjectManager<T>::AddList(T *pObject)
{
	// リストの最後尾にオブジェクトを追加し、イテレーターを返す
	return m_list.insert(m_list.end(), pObject);
}

//============================================================
//	リスト削除処理
//============================================================
template<class T> void CObjectManager<T>::DeleteList(const AIterator iterator)
{
	// 引数イテレーターの要素をリストから削除
	m_list.erase(iterator);
}

//============================================================
//	リスト内の要素数の取得処理
//============================================================
template<class T> int CObjectManager<T>::GetNumAll(void)
{
	// リスト内の要素数を返す
	return m_list.size();
}

//============================================================
//	リスト取得処理
//============================================================
template<class T> std::list<T*> CObjectManager<T>::GetList(void)
{
	// オブジェクトリストを返す
	return m_list;
}

//============================================================
//	生成処理
//============================================================
template<class T> CObjectManager<T> *CObjectManager<T>::Create(void)
{
	// オブジェクトマネージャーの生成
	CObjectManager *pObjectManager = new CObjectManager;
	if (pObjectManager == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// オブジェクトマネージャーの初期化
		if (FAILED(pObjectManager->Init()))
		{ // 初期化に失敗した場合

			// オブジェクトマネージャーの破棄
			SAFE_DELETE(pObjectManager);
			return nullptr;
		}

		// 確保したアドレスを返す
		return pObjectManager;
	}
}

//============================================================
//	破棄処理
//============================================================
template<class T> void CObjectManager<T>::Release(CObjectManager *&prObjectManager)
{
	// オブジェクトマネージャーの終了
	assert(prObjectManager != nullptr);
	prObjectManager->Uninit();

	// メモリ開放
	SAFE_DELETE(prObjectManager);
}

#endif	// _OBJECT_MANAGER_H_
