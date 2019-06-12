//=============================================================================
//
// カメラ処理 [camera.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CCamera
{
public:
	typedef enum
	{
		TYPE_PLANE,		// 戦闘機用
		TYPE_SHIP,		// 戦艦用
		TYPE_TANK,		// 戦車用
		TYPE_MAX
	}TYPE;

	CCamera();
	~CCamera();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Set(void);

	D3DXMATRIX GetProjection(void) { return m_mtxProjection; };
	D3DXMATRIX GetView(void) { return m_mtxView; };
	D3DXVECTOR3 GetPosV(void) { return m_posV; };
	D3DXVECTOR3 GetPosR(void) { return m_posR; };
	float GetAngle(void) { return m_fAngle; };

	void SetPosV(D3DXVECTOR3 posV) { m_posV = posV; };

private:
	D3DXVECTOR3 m_posV;				// 視点
	D3DXVECTOR3 m_posR;				// 注視点
	D3DXVECTOR3 m_vecU;				// 上方向ベクトル
	D3DXMATRIX	m_mtxProjection;	// プロジェクションマトリックス	（テレビ）
	D3DXMATRIX	m_mtxView;			// ビューマトリックス			（カメラマン）
	float		m_fLength;			// 注視点との距離
	float		m_fAngle;			// 角度
};

#endif
