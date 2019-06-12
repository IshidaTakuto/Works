//=============================================================================
//
// プレイヤー処理 [player.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _SETPLAYER_H_
#define _SETPLAYER_H_

#include "scene.h"
#include "modelObject.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SETPLAYER_PRIORITY		(4)

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CModel;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CSetPlayer : public CScene
{
public:
	CSetPlayer(int nPriority = SETPLAYER_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_SETPLAYER);
	~CSetPlayer();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CSetPlayer *Create(CModelObjectManager::TYPE type);
	void SetModel(char **pModelName, int nNumModel);

	D3DXVECTOR3 GetPos(void) { return m_pos; };
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; };

	D3DXVECTOR3 GetRot(void) { return m_rot; };
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; };

	bool Range(D3DXVECTOR3 pos);

private:
	void SetObject(void);
	const void SetModelInfo(CModelObjectManager::TYPE typeOld, int nTypeOld);

	D3DXVECTOR3 m_pos;						// 位置
	D3DXVECTOR3 m_rot;						// 向き
	D3DXMATRIX	m_mtxWorld;					// ワールドマトリックス
	float		m_fSpeed;					// 速度調整
	float		m_fRange;					// 選択範囲
	float		m_fHeight;					// 上げる高さ
	int			m_nType;					// モデル番号
	CModelObjectManager::TYPE	m_type;		// 設置するオブジェクトの種類
	CModel		**m_pModel;					// モデルクラスのポインタ変数
	int			m_nNumModel;				// モデル数
};

#endif
