//=============================================================================
//
// モデル処理 [object.h]
// Author : Ryo Sugimoto
//
//=============================================================================
#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "main.h"
#include "game.h"

//*****************************************************************************
// * マクロ宣言 *
//*****************************************************************************
#define	MAX_ENEMY_APPEAR	(16)		//最大敵出現地の数

//*****************************************************************************
// * 構造体宣言 *
//*****************************************************************************
typedef enum
{//モデルの種類
	//キャラ作成
	OBJECT_TYPE_000_FLOOR = 0,		//床

	//+ 森ステージ
	OBJECT_TYPE_100_FLOOR,			//床
	OBJECT_TYPE_100_GATE,			//門
	OBJECT_TYPE_100_FENCE,			//柵
	OBJECT_TYPE_100_BACK,			//バック
	OBJECT_TYPE_100_CROSS,			//横
	OBJECT_TYPE_100_FLOOR_L,		//床拡張
	OBJECT_TYPE_100_WOOD,			//木

	//+ 城下町
	OBJECT_TYPE_200_FLOOR,			//床
	OBJECT_TYPE_200_SIDE_FLOOR,			//床
	OBJECT_TYPE_200_SIDE_FLOOR2,			//床
	OBJECT_TYPE_200_BRIDGE,//橋
	OBJECT_TYPE_200_CHAIN,//鎖
	OBJECT_TYPE_200_HUNSUI,			//噴水
	OBJECT_TYPE_200_HOME1,			//家1
	OBJECT_TYPE_200_HOME2,			//家2
	OBJECT_TYPE_200_IDO,			//井戸
	OBJECT_TYPE_200_CHURCH,			//教会
	OBJECT_TYPE_200_BABYLON,			//門
	OBJECT_TYPE_200_GAKE,			//崖
	OBJECT_TYPE_200_DOUZOU,			//銅像
	OBJECT_TYPE_200_GATE,			//銅像

	//+ 城内1階
	OBJECT_TYPE_300_STAGE,//床
	OBJECT_TYPE_300_WALL,//壁
	OBJECT_TYPE_300_WALL02,//奥壁
	OBJECT_TYPE_300_WALL03,//奥横壁
	OBJECT_TYPE_300_WALL04,//壁&扉
	OBJECT_TYPE_300_WALL05,//壁
	OBJECT_TYPE_300_WALL06,//壁
	OBJECT_TYPE_300_TENJOU,//天井
	OBJECT_TYPE_300_KAIDAN,//階段
	OBJECT_TYPE_300_HASIRA,//柱
	OBJECT_TYPE_300_TABLE,//机
	OBJECT_TYPE_300_CHAIR,//椅子
	OBJECT_TYPE_300_BOOK,//椅子

	//+ 城内2階
	OBJECT_TYPE_301_FLOOR,//床
	OBJECT_TYPE_301_WALL,//壁
	OBJECT_TYPE_301_WALL02,//壁
	OBJECT_TYPE_301_WALL03,//壁&扉
	OBJECT_TYPE_301_WALL_K,//壁
	OBJECT_TYPE_301_WALL_K02,//壁
	OBJECT_TYPE_301_KAIDAN,//階段
	OBJECT_TYPE_301_DOOR,//ドア
	OBJECT_TYPE_301_KAIGA,//絵画
	OBJECT_TYPE_301_KAIGA02,//絵画
	OBJECT_TYPE_301_KAIGA_KENOU,//絵画
	OBJECT_TYPE_301_YOROI,//甲冑
	//+ 王室
	MAP_TYPE_400_FLOOR,//床
	MAP_TYPE_400_DAIZA,//台座
	MAP_TYPE_400_GYOKUZA,//玉座
	MAP_TYPE_400_HASIRA02,//玉座

	OBJECT_TYPE_999_Sword0,	//剣
	OBJECT_TYPE_999_Sword1,	//剣
	OBJECT_TYPE_999_Sword2,	//剣

	OBJECT_TYPE_999_PARROW,		//大きい矢印
	OBJECT_TYPE_999_PARROWS,	//矢印　プレイヤー
	OBJECT_TYPE_999_EARROWS,	//矢印　エネミー

	
	OBJECT_TYPE_MAX	//最大

}OBJECT_TYPE;
typedef enum
{//モデルの種類
	OBJECT_USE_BLOCK = 0,		//ただの物体
	OBJECT_USE_BACK,			//判定のない物体
	OBJECT_USE_BACK0,			//背景 回る
	OBJECT_USE_NO,				//見えない
	OBJECT_USE_MAP,				//マップ用

	OBJECT_USE_ITEM,			//アイテムスタート
	OBJECT_USE_ITEM_HEEL0,		//回復アイテム0
	OBJECT_USE_ITEM_HEEL1,		//回復アイテム0
	OBJECT_USE_ITEM_HEEL2,		//回復アイテム0
	OBJECT_USE_ITEM_GIFT0,		//ステ上昇アイテム0
	OBJECT_USE_ITEM_GIFT1,		//ステ上昇アイテム0
	OBJECT_USE_ITEM_GIFT2,		//ステ上昇アイテム0

}OBJECT_USE;
typedef enum
{//モデルの種類
	OBJECT_STATE_NONE = 0,		//何もない
	OBJECT_STATE_SWAYING,		//揺れる
	OBJECT_STATE_SPIN,			//回る

}OBJECT_STATE;

typedef struct
{//種類ごとのモデル情報保存
	LPD3DXMESH		MeshObject = NULL;		//メッシュ情報へのポインタ
	LPD3DXBUFFER	pBuffMatObject = NULL;	//マテリアル情報へのポインタ
	LPDIRECT3DTEXTURE9		pTextureObject = NULL;
	DWORD			nNumMatObject = NULL;	//マテリアルの情報数
	float			SlengthX;				//影の長さ
	float			SlengthZ;				//影の長さ
	char			aText[48];				//テキストの名前

}OBJECT_DRAW;
typedef struct
{//モデルの情報
	D3DXMATRIX		mtxWorldObject;			//ワールドマトリックス
	D3DXVECTOR3		InitPos;				//始めの場所
	D3DXVECTOR3		pos;					//場所
	D3DXVECTOR3		rot;					//角度
	OBJECT_TYPE		type;					//モデルの種類 自分
	OBJECT_STATE	state;					//状態
	OBJECT_USE		use;					//使い方
	D3DXVECTOR3		length;					//判定の長さ
	int				nCntState;				//管理用
	int				nCntShadow;				//自分の影の番号
	bool			bUse;					//使用 or 未使用

}OBJECT_INFO;

//*****************************************************************************
// * プロトタイプ宣言 *
//*****************************************************************************
void InitObject(void);
void UninitObject(void);
void LoadObject(int type);
void UpdateObject(void);
void DrawObject(void);
void DrawObject_MAP(int nMode);
int CollisionObject(int nCntModel);
int CollisionObject_Plus(MODEL_SETTING *set);
void CollisionObjectS(D3DXVECTOR3 *pos);
void HitItem(int nCntModel, int nCntObject);
int SetObject(D3DXVECTOR3 pos, D3DXVECTOR3 rot, OBJECT_TYPE type, OBJECT_USE use, D3DXVECTOR3 length, int nShadow);
void SettingObject(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nNumber);
void BreakObject(int nMode);
void SetMap_Change(D3DXVECTOR3 Entrance, D3DXVECTOR3 Export, int nLengthX, int nLengthZ, MAP_TYPE MapNumber);
void BreakMap_Change(void);
void UpdateMap_Change(int nCntModel, int nMode);
void SetLoad_Map(D3DXVECTOR3 Export, MAP_TYPE nMapNumber);
MAP_CHANGE *GetMap_Change(void);
LOAD_MAP *GetLoad_Map(void);
void Enemy_Appear(D3DXVECTOR3 Entrance, int nNumber);
void BreakAppear(void);
ENEMY_APPEAR *GetEnemy_Appear(void);
OBJECT_INFO *GetObject(void);
#endif
