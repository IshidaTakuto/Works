//=============================================================================
//
// モデル処理 [model.h]
// Author : Ryo Sugimoto
//
//=============================================================================
#ifndef _MODEL_H_
#define _MODEL_H_

#include "main.h"

//*****************************************************************************
// * マクロ宣言 *
//*****************************************************************************
#define	MAX_MODEL_MOTION	(40)	//モーションの数

#define MAX_KEY	(12)	//最大キーフレーム数
#define MAX_PARTS	(28)	//最大キーフレーム数
#define MAX_ENEMY	(96)	//一度に出せる最大の敵の数

#define ENEMY_ATIME	(100)	//雑魚敵が攻撃するランドTIME
#define SKILL_TIME	(1800)	//バフの持続時間

#define	INIT_ATK			(115.0f)	//初期攻撃力
#define	INIT_INT			(115.0f)	//初期攻撃力
#define	INIT_DFF			(70.0f)		//初期防御力
#define	INIT_RES			(70.0f)	//初期攻撃力
#define	INIT_SPD			(70.0f)		//初期速さ
#define	INIT_LUK			(25.0f)		//初期運
#define	INIT_HP				(1000.0f)		//初期ライフ
#define	INIT_SP				(60.0f)		//初期スキルポイント
//*****************************************************************************
// * 構造体宣言 *
//*****************************************************************************
typedef enum
{
	MODEL_USE_PLAYER = 0,	//プレイヤー
	MODEL_USE_TUTORIAL,		//チュートリアル専用
	MODEL_USE_OBJECT,		//オブジェクト
	MODEL_USE_Title,		//タイトルマン
	MODEL_USE_Pause,		//ポーズマン
	MODEL_USE_ENEMY_Z,		//雑魚的
	MODEL_USE_ENEMY_ZE,		//雑魚的
	MODEL_USE_ENEMY_S,		//小ボス
	MODEL_USE_ENEMY_T,		//中ボス
	MODEL_USE_ENEMY_B,		//BOSS

}MODEL_USE;
typedef enum
{//モデルの種類
	MODEL_TYPE_RUN_bodyD,		//ランニングマン腹
	MODEL_TYPE_RUN_bodyU,		//ランニングマン胸
	MODEL_TYPE_RUN_head,		//ランニングマン頭
	MODEL_TYPE_RUN_armUR,		//ランニングマン右上腕
	MODEL_TYPE_RUN_armDR,		//ランニングマン右前腕
	MODEL_TYPE_RUN_handR,		//ランニングマン右手
	MODEL_TYPE_RUN_armUL,		//ランニングマン左上腕
	MODEL_TYPE_RUN_armDL,		//ランニングマン左前腕
	MODEL_TYPE_RUN_handL,		//ランニングマン左手
	MODEL_TYPE_RUN_legUR,		//ランニングマン右膝
	MODEL_TYPE_RUN_legDR,		//ランニングマン右脛
	MODEL_TYPE_RUN_footR,		//ランニングマン右足
	MODEL_TYPE_RUN_legUL,		//ランニングマン左膝
	MODEL_TYPE_RUN_legDL,		//ランニングマン左脛
	MODEL_TYPE_RUN_footL,		//ランニングマン左足
	MODEL_TYPE_RUN_Sword0,		//ランニングマン剣0
	MODEL_TYPE_RUN_Sword1,		//ランニングマン剣1
	MODEL_TYPE_RUN_Sword2,		//ランニングマン剣2

	MODEL_TYPE_PNOR_hip,		//プレイヤーノーマル型
	MODEL_TYPE_PNOR_chest,		//プレイヤーノーマル型
	MODEL_TYPE_PNOR_neck,		//プレイヤーノーマル型
	MODEL_TYPE_PNOR_head,		//プレイヤーノーマル型
	MODEL_TYPE_PNOR_chin,		//プレイヤーノーマル型
	MODEL_TYPE_PNOR_Rhand,		//プレイヤーノーマル型
	MODEL_TYPE_PNOR_Lhand,		//プレイヤーノーマル型
	MODEL_TYPE_PNOR_Rleg,		//プレイヤーノーマル型
	MODEL_TYPE_PNOR_Rknee,		//プレイヤーノーマル型
	MODEL_TYPE_PNOR_Rfoot,		//プレイヤーノーマル型
	MODEL_TYPE_PNOR_Lleg,		//プレイヤーノーマル型
	MODEL_TYPE_PNOR_Lknee,		//プレイヤーノーマル型
	MODEL_TYPE_PNOR_Lfoot,		//プレイヤーノーマル型
	MODEL_TYPE_PNOR_tail1,		//プレイヤーノーマル型
	MODEL_TYPE_PNOR_tail2,		//プレイヤーノーマル型

	MODEL_TYPE_PATK_hip,		//プレイヤー攻撃型
	MODEL_TYPE_PATK_body,		//プレイヤー攻撃型
	MODEL_TYPE_PATK_chest,		//プレイヤー攻撃型
	MODEL_TYPE_PATK_neck,		//プレイヤー攻撃型
	MODEL_TYPE_PATK_head,		//プレイヤー攻撃型
	MODEL_TYPE_PATK_chin,		//プレイヤー攻撃型
	MODEL_TYPE_PATK_legR,		//プレイヤー攻撃型
	MODEL_TYPE_PATK_kneeR,		//プレイヤー攻撃型
	MODEL_TYPE_PATK_footR,		//プレイヤー攻撃型
	MODEL_TYPE_PATK_legL,		//プレイヤー攻撃型
	MODEL_TYPE_PATK_kneeL,		//プレイヤー攻撃型
	MODEL_TYPE_PATK_footL,		//プレイヤー攻撃型
	MODEL_TYPE_PATK_shoulderR,	//プレイヤー攻撃型
	MODEL_TYPE_PATK_armR,		//プレイヤー攻撃型
	MODEL_TYPE_PATK_handR,		//プレイヤー攻撃型
	MODEL_TYPE_PATK_shoulderL,	//プレイヤー攻撃型
	MODEL_TYPE_PATK_armL,		//プレイヤー攻撃型
	MODEL_TYPE_PATK_handL,		//プレイヤー攻撃型
	MODEL_TYPE_PATK_tail1,		//プレイヤー攻撃型
	MODEL_TYPE_PATK_tail2,		//プレイヤー攻撃型

	MODEL_TYPE_PINT_body3,		//プレイヤー魔法型
	MODEL_TYPE_PINT_body2,		//プレイヤー魔法型
	MODEL_TYPE_PINT_body1,		//プレイヤー魔法型
	MODEL_TYPE_PINT_body0,		//プレイヤー魔法型
	MODEL_TYPE_PINT_neck,		//プレイヤー魔法型
	MODEL_TYPE_PINT_head,		//プレイヤー魔法型
	MODEL_TYPE_PINT_chin,		//プレイヤー魔法型
	MODEL_TYPE_PINT_tail0,		//プレイヤー魔法型
	MODEL_TYPE_PINT_tail1,		//プレイヤー魔法型
	MODEL_TYPE_PINT_wingS,		//プレイヤー魔法型
	MODEL_TYPE_PINT_wingL,		//プレイヤー魔法型
	MODEL_TYPE_PINT_wingR,		//プレイヤー魔法型
	MODEL_TYPE_PINT_armLU,		//プレイヤー魔法型
	MODEL_TYPE_PINT_armLD,		//プレイヤー魔法型
	MODEL_TYPE_PINT_handL,		//プレイヤー魔法型
	MODEL_TYPE_PINT_armRU,		//プレイヤー魔法型
	MODEL_TYPE_PINT_armRD,		//プレイヤー魔法型
	MODEL_TYPE_PINT_handR,		//プレイヤー魔法型
	MODEL_TYPE_PINT_legLU,		//プレイヤー魔法型
	MODEL_TYPE_PINT_legLD,		//プレイヤー魔法型
	MODEL_TYPE_PINT_footL,		//プレイヤー魔法型
	MODEL_TYPE_PINT_legRU,		//プレイヤー魔法型
	MODEL_TYPE_PINT_legRD,		//プレイヤー魔法型
	MODEL_TYPE_PINT_footR,		//プレイヤー魔法型

	MODEL_TYPE_PSPD_hip,		//プレイヤー速度型
	MODEL_TYPE_PSPD_third_body,	//プレイヤー速度型
	MODEL_TYPE_PSPD_second_body,//プレイヤー速度型
	MODEL_TYPE_PSPD_chest,		//プレイヤー速度型
	MODEL_TYPE_PSPD_head,		//プレイヤー速度型
	MODEL_TYPE_PSPD_chin,		//プレイヤー速度型
	MODEL_TYPE_PSPD_Rleg,		//プレイヤー速度型
	MODEL_TYPE_PSPD_Rknee,		//プレイヤー速度型
	MODEL_TYPE_PSPD_Rfoot,		//プレイヤー速度型
	MODEL_TYPE_PSPD_Lleg,		//プレイヤー速度型
	MODEL_TYPE_PSPD_Lknee,		//プレイヤー速度型
	MODEL_TYPE_PSPD_Lfoot,		//プレイヤー速度型
	MODEL_TYPE_PSPD_Rshoulder,	//プレイヤー速度型
	MODEL_TYPE_PSPD_Rarm,		//プレイヤー速度型
	MODEL_TYPE_PSPD_Rhand,		//プレイヤー速度型
	MODEL_TYPE_PSPD_Lshoulder,	//プレイヤー速度型
	MODEL_TYPE_PSPD_Larm,		//プレイヤー速度型
	MODEL_TYPE_PSPD_Lhand,		//プレイヤー速度型
	MODEL_TYPE_PSPD_tail1,		//プレイヤー速度型
	MODEL_TYPE_PSPD_tail2,		//プレイヤー速度型

	MODEL_TYPE_EKING_bodyD,		//敵　剣王
	MODEL_TYPE_EKING_bodyU,		//敵　剣王
	MODEL_TYPE_EKING_head,		//敵　剣王
	MODEL_TYPE_EKING_armUR,		//敵　剣王
	MODEL_TYPE_EKING_armDR,		//敵　剣王
	MODEL_TYPE_EKING_handR,		//敵　剣王
	MODEL_TYPE_EKING_armUL,		//敵　剣王
	MODEL_TYPE_EKING_armDL,		//敵　剣王
	MODEL_TYPE_EKING_handL,		//敵　剣王
	MODEL_TYPE_EKING_legUR,		//敵　剣王
	MODEL_TYPE_EKING_legDR,		//敵　剣王
	MODEL_TYPE_EKING_footR,		//敵　剣王
	MODEL_TYPE_EKING_legUL,		//敵　剣王
	MODEL_TYPE_EKING_legDL,		//敵　剣王
	MODEL_TYPE_EKING_footL,		//敵　剣王
	MODEL_TYPE_EKING_Sword,		//敵　剣王

	MODEL_TYPE_EWarrior_bodyD,	//敵　戦士
	MODEL_TYPE_EWarrior_bodyU,	//敵　戦士
	MODEL_TYPE_EWarrior_head,	//敵　戦士
	MODEL_TYPE_EWarrior_armUR,	//敵　戦士
	MODEL_TYPE_EWarrior_armDR,	//敵　戦士
	MODEL_TYPE_EWarrior_handR,	//敵　戦士
	MODEL_TYPE_EWarrior_armUL,	//敵　戦士
	MODEL_TYPE_EWarrior_armDL,	//敵　戦士
	MODEL_TYPE_EWarrior_handL,	//敵　戦士
	MODEL_TYPE_EWarrior_legUR,	//敵　戦士
	MODEL_TYPE_EWarrior_legDR,	//敵　戦士
	MODEL_TYPE_EWarrior_footR,	//敵　戦士
	MODEL_TYPE_EWarrior_legUL,	//敵　戦士
	MODEL_TYPE_EWarrior_legDL,	//敵　戦士
	MODEL_TYPE_EWarrior_footL,	//敵　戦士
	MODEL_TYPE_EWarrior_Sword,	//敵　戦士

	MODEL_TYPE_ENinja_bodyD,	//敵　忍者
	MODEL_TYPE_ENinja_bodyU,	//敵　忍者
	MODEL_TYPE_ENinja_head,		//敵　忍者
	MODEL_TYPE_ENinja_armUR,	//敵　忍者
	MODEL_TYPE_ENinja_armDR,	//敵　忍者
	MODEL_TYPE_ENinja_handR,	//敵　忍者
	MODEL_TYPE_ENinja_armUL,	//敵　忍者
	MODEL_TYPE_ENinja_armDL,	//敵　忍者
	MODEL_TYPE_ENinja_handL,	//敵　忍者
	MODEL_TYPE_ENinja_legUR,	//敵　忍者
	MODEL_TYPE_ENinja_legDR,	//敵　忍者
	MODEL_TYPE_ENinja_footR,	//敵　忍者
	MODEL_TYPE_ENinja_legUL,	//敵　忍者
	MODEL_TYPE_ENinja_legDL,	//敵　忍者
	MODEL_TYPE_ENinja_footL,	//敵　忍者
	MODEL_TYPE_ENinja_Sword1,	//敵　忍者
	MODEL_TYPE_ENinja_Sword2,	//敵　忍者

	MODEL_TYPE_EWizard_bodyD,	//敵　魔法使い
	MODEL_TYPE_EWizard_bodyU,	//敵　魔法使い
	MODEL_TYPE_EWizard_head,	//敵　魔法使い
	MODEL_TYPE_EWizard_armUR,	//敵　魔法使い
	MODEL_TYPE_EWizard_armDR,	//敵　魔法使い
	MODEL_TYPE_EWizard_handR,	//敵　魔法使い
	MODEL_TYPE_EWizard_armUL,	//敵　魔法使い
	MODEL_TYPE_EWizard_armDL,	//敵　魔法使い
	MODEL_TYPE_EWizard_handL,	//敵　魔法使い
	MODEL_TYPE_EWizard_legUR,	//敵　魔法使い
	MODEL_TYPE_EWizard_legDR,	//敵　魔法使い
	MODEL_TYPE_EWizard_footR,	//敵　魔法使い
	MODEL_TYPE_EWizard_legUL,	//敵　魔法使い
	MODEL_TYPE_EWizard_legDL,	//敵　魔法使い
	MODEL_TYPE_EWizard_footL,	//敵　魔法使い
	MODEL_TYPE_EWizard_Sword,	//敵　魔法使い

	MODEL_TYPE_ESoldiers_bodyD,	//敵　兵士
	MODEL_TYPE_ESoldiers_bodyU,	//敵　兵士
	MODEL_TYPE_ESoldiers_head,	//敵　兵士
	MODEL_TYPE_ESoldiers_armUR,	//敵　兵士
	MODEL_TYPE_ESoldiers_armDR,	//敵　兵士
	MODEL_TYPE_ESoldiers_handR,	//敵　兵士
	MODEL_TYPE_ESoldiers_armUL,	//敵　兵士
	MODEL_TYPE_ESoldiers_armDL,	//敵　兵士
	MODEL_TYPE_ESoldiers_handL,	//敵　兵士
	MODEL_TYPE_ESoldiers_legUR,	//敵　兵士
	MODEL_TYPE_ESoldiers_legDR,	//敵　兵士
	MODEL_TYPE_ESoldiers_footR,	//敵　兵士
	MODEL_TYPE_ESoldiers_legUL,	//敵　兵士
	MODEL_TYPE_ESoldiers_legDL,	//敵　兵士
	MODEL_TYPE_ESoldiers_footL,	//敵　兵士
	MODEL_TYPE_ESoldiers_Sword,	//敵　兵士

	MODEL_TYPE_EMPTY			//空っぽ

}MODEL_TYPE;

typedef enum
{
	MODEL_STATE_NONE = 0,	//何もなし(´・ω・`)
	MODEL_STATE_ATK,		//攻撃中
	MODEL_STATE_ATKHIT,		//攻撃が当たった
	MODEL_STATE_POISED,		//ガードの構え
	MODEL_STATE_GUARD,		//ガード成功
	MODEL_STATE_DAMAGE,		//被ダメ中　スキルキャンセルできない
	MODEL_STATE_DIE,		//死亡　スキルキャンセルできない
	MODEL_STATE_MAP,		//マップ切り替え中 動けない

}MODEL_STATE;
typedef struct
{//キャラクターのステータス
	float	fLifePoint;		//HP
	float	fSkillPoint;	//SP
	float	fAttack;		//攻撃力
	float	fDefense;		//防御力
	float	fSpeed;			//速さ, スタミナ
	float	fLucky;			//運, 会心率, 会心回避率

	float	fInt;			//魔攻	SP反映
	float	fRes;			//魔防	回復量反映

}CHAR_STATUS;
typedef struct
{//キャラクターのステータス
	char	aName[25];		//名前
	int		nLevel;			//レベル
	int		nCareer;		//職業
	int		nStrength;		//戦闘力 経験値
	int		nExperience;	//現在の経験値
	int		nNextLevel;		//次のレベルまでの経験値
	int		nLifePoint;		//現在のHP
	int		nSkillPoint;	//現在のSP
	int		nStamina;		//スタミナゲージ
	int		nMaxStamina;	//最大スタミナゲージ
	int		nTimeStamina;	//スタミナの回復待ち時間
	int		nSkill[2][5];	//持っているスキル2つと　0:スキル番号 1: 2: 3:追加 4:スキル発動判定
	CHAR_STATUS status;		//計算されたステータス
	CHAR_STATUS gifted;		//才能(名前からの贈り物) 撃破時才能+
	CHAR_STATUS plus[2];	//上昇ステータスとカウント用

}CHAR_DATA;

//*****************************************************************************
// マップ切り替えの構造体
//*****************************************************************************
typedef enum
{//モデルの種類
	MAP_TYPE_EMPTY = 0,		//切り替え無し
	MAP_TYPE_000_CHAR_MAKE,		//森
	MAP_TYPE_100_YEUNG,		//森
	MAP_TYPE_200_CITY,	//城下町
	MAP_TYPE_300_CASTLE,		//城1F
	MAP_TYPE_301_CASTLE,		//城2F
	MAP_TYPE_400_THRONE,	//玉座

}MAP_TYPE;
typedef struct
{
	MAP_TYPE	MapNumber;		//マップの番号
	D3DXVECTOR3 Entrance;		//入口
	D3DXVECTOR3 Export;			//出口
	int			nLengthX;		//範囲X
	int			nLengthZ;		//範囲Z
	bool		bUse;			//使用してるかどうか

}MAP_CHANGE;
typedef struct
{
	D3DXVECTOR3 Export;			//出口
	MAP_TYPE	nMapNumber;		//マップの番号

}LOAD_MAP;

typedef enum
{
	BUTTON_W = 0,	//W
	BUTTON_A,	//A
	BUTTON_S,	//S
	BUTTON_D,	//D
	BUTTON_WS,	//W
	BUTTON_AS,	//A
	BUTTON_SS,	//S
	BUTTON_DS,	//D
	BUTTON_UP,	//W
	BUTTON_LEFT,	//A
	BUTTON_DOWN,	//S
	BUTTON_RIGHT,	//D
	BUTTON_E,	//E
	BUTTON_Q,	//Q
	BUTTON_X,	//X
	BUTTON_C,	//C
	BUTTON_Z,	//Z
	BUTTON_LSHIFT,	//LSHIFT
	BUTTON_LCTRL,	//ENTER
	BUTTON_SPACE,	//SPACE
	BUTTON_ENTER,	//ENTER
	BUTTON_JENTER,	//ENTER
	BUTTON_BACKSPACE,	//SPACE
	BUTTON_U,	//U
	BUTTON_O,	//O
	BUTTON_Y,	//Y
	BUTTON_H,	//H
	BUTTON_US,	//U
	BUTTON_OS,	//O
	BUTTON_YS,	//Y
	BUTTON_HS,	//H
	BUTTON_J,	//J
	BUTTON_K,	//K
	BUTTON_I,	//I
	BUTTON_L,	//L
	BUTTON_P,	//P
	BUTTON_MAX,	//最大button数

}BUTTON;
typedef enum
{
	JAYPAD_STATE_SELECT = 0,	//SELECT画面の時
	JAYPAD_STATE_GAME,			//ゲームの時

}JAYPAD_STATE;
typedef enum
{
	SDROP_STATE_NONE = 0,	//何もなし
	SDROP_STATE_SELECT,		//ゲームの時
	SDROP_STATE_END,		//終了時
				
}SDROP_STATE;
typedef struct
{//キャラクターのステータス
	JAYPAD_STATE	state;			//ボタンの入力処理用
	int		nCntHPGage[3];			//HPのゲージ番号
	int		nCntMPGage[3];			//MPのゲージ番号
	int		nCntEXPGage[2];			//EXPのゲージ番号
	int		nCntSkillWindow[2];		//弱スキルと強スキルの番号
	int		nCntStamina[2];			//スタミナゲージ番号
	int		nCntGageWindow;			//HPゲージの装飾
	int		nTime[2];				//時間カウント
	int		nCntIcon[8];			//攻撃上昇等のアイコン
	int		nCntModel_P;			//ポーズ中のモデル
	int		nCntModel_M;			//MAP用モデル
	
	SDROP_STATE SDstate;			//状態管理管理　スキルドロップ
	int		nCntSDstate;				//カウント用
	int		nDSkill[3];				//ドロップしたスキル保存
	int		nCntDWindow[3];			//右, 左, 下の選択肢の番号保存

	MAP_TYPE	maptype;			//現在いるマップ
	D3DXVECTOR3	target;				//目指す場所
	int			nCntTarget;			//ポリゴン番号
	bool		bTarget;			//目指す場所の有無

	int			nScnSpeech;			//どのシーンか
	int			nNumSpeech;			//どこまで話したか
	int			nCntSpeech;			//ウィンドウ番号
	int			nCntSTime;			//会話の待ち時間カウント
	int			nCntSStop;			//会話の待ち時間合計

	bool	bButton[BUTTON_MAX];
	bool	bJoyUse;				//ジョイパッドの使用未使用
	bool	bGame;
}PLAYER;
typedef struct
{//キャラクターのステータス
	int		nCntHPGage[3];		//HPのゲージ番号
	int		nDistance;
	int		nAction[3];
	int		nCntTime;
	int		nMaxTime;
	int		nDying;				//0:死亡, 1:瀕死, 2:通常, 3:瀕死無視
	int		nDSkill[2];			//落とすスキル2つ
	int		nItem;				//落とすアイテム
	int		nDropS;				//スキルを落とす確率(倍率)
	int		nDropI;				//アイテムを落とす確率(倍率)
	bool	bUse;				//使用or未使用

}ENEMY;

//*****************************************************************************
// * 構造体宣言 モデルの親子関係について *
//*****************************************************************************
typedef struct
{//親子関係
	MODEL_TYPE		type;					//モデルの種類 自分
	int				nParent;				//親の番号
	D3DXMATRIX		mtxWorldModel;			//ワールドマトリックス
	D3DXVECTOR3		addrot;					//加算の向き モーション
	D3DXVECTOR3		addRotmove;				//モーションの移動

}MODEL_PARENT;
typedef struct
{//親子関係のセット
	int				nMaxModel;				//中に入っているモデル数
	MODEL_PARENT	parent[MAX_PARTS];		//モデルの種類 自分
	MODEL_STATE		state;					//キャラステート
	int				nCntState;				//状態管理用	怯みにしか使わない...
	D3DXVECTOR3		Initpos;				//初期位置...エラー対処用...
	D3DXVECTOR3		pos;					//プレイヤーの原点
	D3DXVECTOR3		rot;					//プレイヤーの向き
	D3DXVECTOR3		move;					//モデルの位置
	D3DXVECTOR3		addpos;					//加算の位置 モーション
	D3DXVECTOR3		addPosmove;				//モーションの移動
	D3DXVECTOR3		angle;					//移動方向
	D3DXVECTOR3		posold;					//前回のモデルの位置
	int				nSkillTime[2];			//スキルの待機時間
	int				nCntHeel;				//回復時間カウント
	int				nAirTime;				//無敵時間
	int				nPlayTime;				//P:指示の有効時間 E:指示の待ち時間
	int				nNumMotion[6];			//0:モーションデータ 1:現在のモーション 2:次のモーション 3:キー番号 4:キーのフレーム数 5:前のモーション
	int				nMotionFrame;			//MOTION全体のフレーム数をカウント
	int				nLeg;					//右足を出しているか左足を出しているか
	int				nLockChar;				//lockしているキャラ
	int				nCntChar;				//自分のキャラ番号
	float			fSpeed;					//移動速度
	float			fPos;					//Y軸ぶっ壊れの補助
	float			fSlength;				//影の長さ 当たり判定
	float			fHeight;				//当たり判定
	bool			bStep;					//ステップしたかどうか
	bool			bJump;					//地上にいるかどうか
	bool			bRun;					//走っているかどうか

}MODEL_SETTING;

//*****************************************************************************
// * 構造体宣言 モーションについて *
//*****************************************************************************
typedef struct
{//親子関係
	int				nMuse;				//moveの加算  0:単発, 1:永続
	int				nAtkFrame[2];		//始まりから終わりまでの攻撃有効フレーム
	int				nParts;				//攻撃する部位 始2桁部位 後ろ2桁つなげる範囲
	int				nInvincible;		//ヒット時の無敵になる時間
	int				nFloating;			//浮遊ありor無し
	int				nAtkEffect;			//攻撃による効果　弾が飛んだり
	int				nShowEffect;		//エフェクトタイプ
	float			fFrame;				//かけるフレーム数
	float			fRange;				//攻撃範囲
	float			fAtkplus;			//攻撃の倍率 怯み時間計算時活用
	float			fSlip;				//滑る倍率
	D3DXVECTOR3		blown;				//吹っ飛ぶ距離
	D3DXVECTOR3		move;				//移動値
	D3DXVECTOR3		pos;			//モーションの位置
	D3DXVECTOR3		rot[MAX_PARTS];			//モーションの向き

}MODEL_KEY;

typedef struct
{//親子関係
	int				nKey;					//キー数
	int				nCancel[2];				//始まりから終わりまでのキャンセル有効フレーム
	int				nBranch[4];				//派生保存  0:通常時  1:弱攻撃  2:狂攻撃 3:着地分岐
	float			fFastFrame;				//初期値までの加算フレーム数	常時時　ループ有りを考えて
	float			fEndFreame;			//そのまま次のモーションになるまでの硬直時間+
	float			fCancelFreame;			//キャンセル時次のモーションになるまでの硬直時間+
	MODEL_KEY		key[MAX_KEY];

}MODEL_MOTION;

//*****************************************************************************
// * 構造体宣言 モデルの表示について *
//*****************************************************************************
typedef struct
{//種類ごとのモデル情報保存
	LPD3DXMESH		MeshModel = NULL;		//メッシュ情報へのポインタ
	LPD3DXBUFFER	pBuffMatModel = NULL;	//マテリアル情報へのポインタ
	DWORD			nNumMatModel = NULL;	//マテリアルの情報数
	D3DXVECTOR3		addpos;					//モデルの位置
	D3DXVECTOR3		addrot;					//モデルの向き
	int				nTexture;				//テクスチャ
	char			aText[48];				//テキストの名前

}MODEL_DRAW;
typedef struct
{//モデルの情報
	CHAR_DATA		data;					//キャラデータ
	MODEL_SETTING	set;					//モデルの種類 自分
	MODEL_USE		use;					//モデルの使い方
	int				nCntShadow;				//自分の影の番号
	int				nCntObject;				//自分の影の番号
	bool			bUse;					//使用 or 未使用

}MODEL_INFO;

//*****************************************************************************
// * プロトタイプ宣言 *
//*****************************************************************************
void InitModel(void);
void UninitModel(void);
void LoadModel(int type);

void UpdateModel(void);
void UpdatePlayer(int nCount);
void UpdateModel_P(void);

void UpdateSpeech(void);
void Speech_Tutorial(void);

void UpdateEnemy(MODEL_SETTING *set, CHAR_DATA *data, MODEL_USE use, int nMode);
void UpdateSubEnemy(MODEL_SETTING *set, CHAR_DATA *data, float fRot, bool bMove);
void UpdateBEnemy(MODEL_SETTING *set, CHAR_DATA *data, MODEL_USE use, int nMode);
void UpdateTEnemy(MODEL_SETTING *set, CHAR_DATA *data, MODEL_USE use, int nMode);
void UpdateSEnemy(MODEL_SETTING *set, CHAR_DATA *data, MODEL_USE use, int nMode);
void UpdateZEnemy(MODEL_SETTING *set, CHAR_DATA *data, MODEL_USE use, int nMode);
void UpdateZEnemyE(MODEL_SETTING *set, CHAR_DATA *data, MODEL_USE use, int nMode);

void PointEnemy(MODEL_SETTING *set, int nRand);
void MotionEffect_A(int nCount, int nEnemy, float fAtk, float *fHeel, float *fPredator);
void SadnessError(int nCount);	//今後ちゃんと消そう...
void LockOn_MODEL(int nCount);
float ReturnAngle(MODEL_SETTING *set);
void CollisionModel(int nCount);
void UpdateMotion(int nCount);
void ResetMotion(MODEL_SETTING *set);
int MotionSkill(int nCntMotion);
void MotionEffect_S(int nMode, int nCount);
void Attackjudge(void);
void Hitjudge(int nCntModel, int nCntBullet, int nCntAtk, int nInvincible, float fRange, float fAtk, float fAtkplus, D3DXVECTOR3 blown, D3DXMATRIX	mtxParts, int nMode);

void DropSkill(int nSkill);
void SwapSkill(int nNumber, int nCount);
void PlusSkill(int *nSkill, int nCount);
void SkillEffect_S(int *nSkill, int nCount);
void SkillEffect_A(int *nSkill, int nCount, int nEnemy, float *fAtk, float *fHeel, float *fPredator);
void ReturnSkillName_P(char *aName, int *nSkill);
void ReturnSkillName(char *aName, int nSkill);

void DrawModel(void);
void DrawModel_P(void);
int SetModel(D3DXVECTOR3 pos, D3DXVECTOR3 rot, MODEL_USE use, MODEL_SETTING *pSet, CHAR_DATA *data, float fSlength, float fHeight);
int SetEnemy(CHAR_DATA *data, D3DXVECTOR3 pos, MODEL_TYPE type);

void SetEnemySkill(CHAR_DATA *data, MODEL_TYPE type, int nCntEnemy);
void SetEnemySkill_100(CHAR_DATA *data, MODEL_TYPE type, int nCntEnemy);
void SetEnemySkill_200(CHAR_DATA *data, MODEL_TYPE type, int nCntEnemy);
void SetEnemySkill_300(CHAR_DATA *data, MODEL_TYPE type, int nCntEnemy);
void SetEnemySkill_301(CHAR_DATA *data, MODEL_TYPE type, int nCntEnemy);

void BreakEnemy(void);
void BreakModel(int nUse);
void GetStatus(CHAR_DATA *data, bool bInit);
CHAR_DATA GetGifted(char *Name);
void ShowStatus0(CHAR_DATA data, char *aStr);
void ShowStatus(CHAR_DATA data, CHAR_DATA data0);
CHAR_STATUS GetCareer(int nCareer, char *aCareer);

void SetRunningman(MODEL_SETTING *set, MODEL_TYPE type);
void Set_P_NOR(MODEL_SETTING *set);
void Set_P_ATK(MODEL_SETTING *set);
void Set_P_INT(MODEL_SETTING *set);
void Set_P_SPD(MODEL_SETTING *set);
void Set_E_KING(MODEL_SETTING *set);
void Set_E_WARRIOR(MODEL_SETTING *set);
void Set_E_NINJA(MODEL_SETTING *set);
void Set_E_WIZARD(MODEL_SETTING *set);
void Set_E_SOLDIERS(MODEL_SETTING *set);

MODEL_INFO *GetModel_Info(void);
MODEL_MOTION *GetModel_Motion(void);
PLAYER *GetPlayer(void);
ENEMY *GetEnemy(void);
CHAR_DATA *GetChar_Data(void);
void InitChar_Data(int nLevel);
void LoadChar_Data(CHAR_DATA *data);
void SaveChar_Data(CHAR_DATA Pdata);
void CorrectionChar_Data(void);
#endif