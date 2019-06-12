//=============================================================================
//
// カメラ処理 [camera.h]
// Author : 杉本　涼
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef enum
{//モデルの種類
	CAMERA_STATE_NONE = 0,	//動かせない
	CAMERA_STATE_GAME,		//ゲーム
	CAMERA_STATE_MOVIE,		//ムービー

}CAMERA_STATE;
typedef enum
{//モデルの種類
	CAMERA_VIEW_IGNORE = 0,	//何にも関わらない
	CAMERA_VIEW_FOLLOW,		//追従
	CAMERA_VIEW_WATCH,		//追わずに見るだけ

}CAMERA_VIEW;
typedef enum
{//モデルの種類
	SCENE_VIEW_NORMAL = 0,	//通常通り動く
	SCENE_VIEW_STOP,		//モデルのみ止まる

}SCENE_VIEW;
typedef enum
{//モデルの種類
	SCENE_TYPE_NO = 0,	//戻る　間違えようの対処
	SCENE_TYPE_TBOSS,	//中ボス前のシーン
	SCENE_TYPE_BOSS,	//ボス前のシーン

}SCENE_TYPE;

//*****************************************************************************
// カメラの構造体
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 posV;			//視点
	D3DXVECTOR3 posR;			//注視点
	D3DXVECTOR3 vecU;			//上方向のベクトル

	D3DXVECTOR3 goal;			//向かう位置
	D3DXVECTOR3 targetpos;		//追従する場所
	D3DXVECTOR3 rot;			//角度
	D3DXVECTOR3 addpos;			//カメラの水平移動分
	float		fDistance;		//カメラと注視点の距離
	float		fGMagnification;	//目的にいくまでの倍率

	CAMERA_STATE	state;		//状態
	CAMERA_VIEW		ViewC;		//見る状態 カメラ
	SCENE_VIEW		ViewS;		//見る状態 カメラ以外(モデル等)
	SCENE_TYPE		scene;			//何のシーンか
	int				nCntState;		//ムービーの進行度
	int				nCntModel;		//何のキャラを追従するか
	int				nCntTime;		//シーンのフレーム数記録

	D3DXMATRIX mtxProjection;	//プロジェクションマトリックス
	D3DXMATRIX mtxView;			//ビューマトリックス
	D3DVIEWPORT9 viewport;		//カメラ設定用


}Camera;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);
void ResetCamera(void);
void SetCamera(int nCount);
void SetScene(SCENE_TYPE type);
void UpdateScene(void);
void Scene_TBOSS(void);
void Scene_BOSS(void);
Camera *GetCamera(void);

#endif
