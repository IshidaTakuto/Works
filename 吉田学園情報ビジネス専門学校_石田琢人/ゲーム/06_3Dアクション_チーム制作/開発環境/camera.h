//*****************************************************************************
//
//     カメラの処理[camera.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _CAMERA_H_
#define _CAMERA_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//    カメラクラスの定義
//*****************************************************************************
class CCamera
{
public:    // 誰でもアクセス可能
	CCamera();
	~CCamera();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void SetCamera(void);

	void SetPosV(const D3DXVECTOR3 posV);
	void SetPosR(const D3DXVECTOR3 posR);

	D3DXVECTOR3 GetPosV(void);
	D3DXVECTOR3 GetPosR(void);
	D3DXVECTOR3 GetRot(void);

	void SetDefaultCamera(D3DXVECTOR3 rot);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	void GameCamera(void);
	void TitleCamera(void);
	void ResultCamera(void);
	void RankingCamera(void);

	D3DXVECTOR3  m_PosV;          // 現在の視点
	D3DXVECTOR3  m_PosR;          // 現在の注視点
	D3DXVECTOR3  m_Rot;           // 現在の向き
	D3DXVECTOR3  m_DestRot;       // 目的の向き
	D3DXVECTOR3  m_VecU;          // 上方向ベクトル
	D3DXMATRIX   m_MtxProjection; // プロジェクションマトリックス
	D3DXMATRIX   m_MtxView;       // ビューマトリックス
	D3DVIEWPORT9 m_ViewPort;      // ビューポート(描画領域)
	float        m_fLength;       // 距離
};

#endif