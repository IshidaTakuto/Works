//*****************************************************************************
//
//     マップの処理[map.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _MAP_H_
#define _MAP_H_

//*****************************************************************************
//     インクルードファイル
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
#define MAX_WATER          (10)          // 配置できる水の最大数

//*****************************************************************************
//    前方宣言
//*****************************************************************************
class CField;
class CWater;
class CModelManager;
class CCylinder;
class CDome;

//*****************************************************************************
//     マップクラスの定義
//*****************************************************************************
class CMap
{
public:    // 誰でもアクセス可能
	CMap();
	~CMap();

	static CMap *Create(char *aFileName);

	void Init(char *aFileName);
	void Uninit(void);
	void SetObjectDefault(void);

	char *GetMapName(void);

	D3DXVECTOR3 GetDefaultPlayerPos(void);
	D3DXVECTOR3 GetDefaultPlayerRot(void);
	D3DXVECTOR3 GetDefaultCameraRot(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	void FieldLoad(char *aFieldFileName);
	void WaterLoad(char *aWaterFileName);
	void ObjectLoad(char *aObjectFileName);
	void MountainLoad(char *aMountainFileName);
	void SkyLoad(char *aSkyFileName);

	CModelManager  *m_pModelManager;                         // モデル管轄クラスへのポインタ
	CField         *m_pField;                                // 地面クラスへのポインタ
	CWater         *m_pWater[MAX_WATER];                     // 水クラスへのポインタ
	CCylinder      *m_pMoutain;                              // 円筒クラスへのポインタ
	CDome          *m_pSky;                                  // 半球クラスへのポインタ
	char           m_aFileName[256];                         // 読み込むスクリプトファイル名
	D3DXVECTOR3    m_DefaultPlayerPos;                       // プレイヤーの初期座標
	D3DXVECTOR3    m_DefaultPlayerRot;                       // プレイヤーの初期向き
	D3DXVECTOR3    m_DefaultCameraRot;                       // カメラの初期向き
	char           m_aObjectFileName[256];                   // オブジェクトデータのスクリプトファイル名
};

#endif