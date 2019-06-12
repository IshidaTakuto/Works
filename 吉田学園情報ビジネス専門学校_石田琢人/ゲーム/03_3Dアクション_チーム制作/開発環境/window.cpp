//=========================================================================================================//
//
// window処理 [window.cpp]
// Author : Ryou Sugimoto
//
//=========================================================================================================//
#include "main.h"
#include "game.h"
#include "input.h"
#include "sound.h"

#include <stdlib.h>				//ランド用
#include <time.h>				//同じくランド用

//*********************************************************************************************************//
// マクロ定義
//*********************************************************************************************************//
#define WINDOW_VERTEX_NUM			(4)					// 頂点の数
#define WINDOW_PRIMITIVE_NUM		(2)					// プリミティブの数
#define WINDOW_MAX_TEXTURE			(27)
#define WINDOW_TextureName0		"data/TEXTURE/press_enter.png"		//タイトル エンターキー
#define WINDOW_TextureName1		"data/TEXTURE/GAGE000.png"				//プレイヤーのゲージ
#define WINDOW_TextureName2		"data/TEXTURE/GAGE000.jpg"		//敵のゲージ
#define WINDOW_TextureName3		"data/TEXTURE/EXP_Window.png"			//経験値の枠
#define WINDOW_TextureName4		"data/TEXTURE/pause100.png"			//ポーズ背景
#define WINDOW_TextureName5		"data/TEXTURE/pauseALL.png"			//ポーズ画面戻る
#define WINDOW_TextureName6		"data/TEXTURE/1207_stone_s.jpg"			//枠
#define WINDOW_TextureName7		"data/TEXTURE/goburinnbox.png"		//ゴブリン枠
#define WINDOW_TextureName8		"data/TEXTURE/3D_logo.png"			//タイトルロゴ3D
#define WINDOW_TextureName9		"data/TEXTURE/Name_logo.png"		//タイトルロゴName
#define WINDOW_TextureName10	"data/TEXTURE/Hunter_logo.png"		//タイトルロゴHunter
#define WINDOW_TextureName11	"data/TEXTURE/GAGEWindow.png"		//ゲージの枠
#define WINDOW_TextureName12	"data/TEXTURE/FontWindow.png"		//ゲージの枠
#define WINDOW_TextureName13	"data/TEXTURE/STATUS_WINDOW.png"	//ステータスの枠
#define WINDOW_TextureName14	"data/TEXTURE/RANKING_logo.png"	//ランキングロゴ
#define WINDOW_TextureName15	"data/TEXTURE/Ranking_Window.png"	//ランキング枠
#define WINDOW_TextureName16	"data/TEXTURE/GAMEOVER000.jpg"	//オーバー画面
#define WINDOW_TextureName17	"data/TEXTURE/GAMECLEAR000.jpg"	//クリア画面
#define WINDOW_TextureName18	"data/TEXTURE/Result000.png"	//リザルト戦闘力
#define WINDOW_TextureName19	"data/TEXTURE/Skill_Window000.png"	//リザルト戦闘力
#define WINDOW_TextureName20	"data/TEXTURE/Skill_Window001.png"	//リザルト戦闘力
#define WINDOW_TextureName21	"data/TEXTURE/BOSS_Window.png"	//リザルト戦闘力
#define WINDOW_TextureName22	"data/TEXTURE/TIMEUP000.png"	//タイムアップ
#define WINDOW_TextureName23	"data/TEXTURE/SkillSwap1.png"	//スキル入れ替え1
#define WINDOW_TextureName24	"data/TEXTURE/SkillSwap2.png"	//スキル入れ替え2
#define WINDOW_TextureName25	"data/TEXTURE/SkillSwap3.png"	//スキル入れ替え3
#define WINDOW_TextureName26	"data/TEXTURE/STATUS_ALL.png"	//ステータスicon 
#define	MAX_WINDOW			(48)		// windowの最大数
#define HALFEXPLOSION		(30)

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 initpos;	//初期位置
	D3DXCOLOR col;
	float fLengthX;
	float fLengthY;
	int nPtnUV;			//指定された座標で表示
	int nPtnHeight;		//画像の縦の分割数
	int nPtnWide;		//画像の横の分割数
	int nType;
	float fSteep;			//角度
	WINDOWSTATE state;
	WINDOWUSE use;
	WINDOWTYPE PosType;	//表示するときの軸の設定
	WINDOW_DRAW	draw;
	int nCounterState[2];	//エネミーの状態管理用
	WINDOW_UPDATE nUpdateType;		//更新を分けるため
	bool bUse;
}Window;

//*********************************************************************************************************//
// グローバル変数
//*********************************************************************************************************//
LPDIRECT3DTEXTURE9		g_pTextureWindow[WINDOW_MAX_TEXTURE] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWindow = NULL;	// 頂点バッファへのポインタ
Window g_aWindow[MAX_WINDOW];

//=========================================================================================================//
// * 背景の描画処理1	初期化処理
//=========================================================================================================//
void InitWindow(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();
	int nCntWindow;

	// 弾の情報の初期化
	for (nCntWindow = 0; nCntWindow < MAX_WINDOW; nCntWindow++)
	{
		//g_aWindow[nCntWindow].pos = { 0.0f, 0.0f, 0.0f };
		//g_aWindow[nCntWindow].nLengthX = 0;
		//g_aWindow[nCntWindow].nLengthY = 0;
		//g_aWindow[nCntWindow].nType = 0;
		//g_aWindow[nCntWindow].col = { 1.0f, 1.0f, 1.0f, };
		g_aWindow[nCntWindow].bUse = false;
		g_aWindow[nCntWindow].nUpdateType = WINDOW_UPDATE_SELECT;
	}

	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName0, &g_pTextureWindow[0]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName1, &g_pTextureWindow[1]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName2, &g_pTextureWindow[2]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName3, &g_pTextureWindow[3]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName4, &g_pTextureWindow[4]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName5, &g_pTextureWindow[5]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName6, &g_pTextureWindow[6]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName7, &g_pTextureWindow[7]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName8, &g_pTextureWindow[8]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName9, &g_pTextureWindow[9]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName10, &g_pTextureWindow[10]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName11, &g_pTextureWindow[11]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName12, &g_pTextureWindow[12]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName13, &g_pTextureWindow[13]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName14, &g_pTextureWindow[14]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName15, &g_pTextureWindow[15]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName16, &g_pTextureWindow[16]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName17, &g_pTextureWindow[17]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName18, &g_pTextureWindow[18]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName19, &g_pTextureWindow[19]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName20, &g_pTextureWindow[20]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName21, &g_pTextureWindow[21]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName22, &g_pTextureWindow[22]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName23, &g_pTextureWindow[23]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName24, &g_pTextureWindow[24]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName25, &g_pTextureWindow[25]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName26, &g_pTextureWindow[26]);

	//頂点BUFFERの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * WINDOW_VERTEX_NUM * MAX_WINDOW,		//確保するBUFFERのサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,				//頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffWindow,
		NULL);

	VERTEX_2D *pVtx;		//頂点情報へのポインタ
							//頂点BUFFERをロックし、頂点情報へのポインタを取得
	g_pVtxBuffWindow->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntWindow = 0; nCntWindow < MAX_WINDOW; nCntWindow++)
	{
		//頂点座標の設定
		pVtx[nCntWindow * 4].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[nCntWindow * 4 + 1].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[nCntWindow * 4 + 2].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[nCntWindow * 4 + 3].pos = D3DXVECTOR3(0, 0, 0.0f);

		pVtx[nCntWindow * 4].rhw = 1.0f;
		pVtx[nCntWindow * 4 + 1].rhw = 1.0f;
		pVtx[nCntWindow * 4 + 2].rhw = 1.0f;
		pVtx[nCntWindow * 4 + 3].rhw = 1.0f;

		//カラー設定
		pVtx[nCntWindow * 4].col = D3DCOLOR_RGBA(0, 0, 0, 255);	// A = 透明度
		pVtx[nCntWindow * 4 + 1].col = D3DCOLOR_RGBA(0, 0, 0, 255);
		pVtx[nCntWindow * 4 + 2].col = D3DCOLOR_RGBA(0, 0, 0, 255);
		pVtx[nCntWindow * 4 + 3].col = D3DCOLOR_RGBA(0, 0, 0, 255);

		//テクスチャ座標設定
		pVtx[nCntWindow * 4].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[nCntWindow * 4 + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[nCntWindow * 4 + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[nCntWindow * 4 + 3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	//頂点BUFFERをアンロックする
	g_pVtxBuffWindow->Unlock();
}
//=========================================================================================================//
// * 背景の描画処理2	終了処理
//=========================================================================================================//
void UninitWindow(void)
{//テクスチャの破棄
	int nCntTexture;

	for (nCntTexture = 0; nCntTexture < WINDOW_MAX_TEXTURE; nCntTexture++)
	{
		if (g_pTextureWindow[nCntTexture] != NULL)
		{
			g_pTextureWindow[nCntTexture]->Release();
			g_pTextureWindow[nCntTexture] = NULL;
		}
	}
	if (g_pVtxBuffWindow != NULL)
	{
		g_pVtxBuffWindow->Release();
		g_pVtxBuffWindow = NULL;
	}
}
//=========================================================================================================//
// * 背景の描画処理3	更新処理
//=========================================================================================================//
void UpdateWindow(WINDOW_UPDATE update)
{
	GAME_PLAY *game_play = GetGame_Play();
	int nCntWindow;
	int nRand;
	float fRand;	//0.01から6.28まで
	float fEmpty;	//計算用
	static float fStealth = 0.02f;
	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//pPlayer = GetPlayer();

							//頂点BUFFERをロックし、頂点情報へのポインタを取得
	g_pVtxBuffWindow->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntWindow = 0; nCntWindow < MAX_WINDOW; nCntWindow++)
	{
		if (g_aWindow[nCntWindow].bUse == true && g_aWindow[nCntWindow].nUpdateType == update)
		{
			switch (g_aWindow[nCntWindow].state)
			{
			case WINDOWSTATE_STEALTH://設定された透明度から明るくなり255で通常になる
				g_aWindow[nCntWindow].col.a += 0.012f;
				if (GetKeyboardTrigger(DIK_RETURN) == true || GetKeyboardTrigger(DIK_Z) == true || g_aWindow[nCntWindow].col.a >= 1.0f)
				{//キー入力でスキップ
					g_aWindow[nCntWindow].col.a = 1.0f;
					g_aWindow[nCntWindow].state = WINDOWSTATE_NORMAL;
				}
				break;
			case WINDOWSTATE_FADEIN:	//Stealthの早送りなし
				g_aWindow[nCntWindow].col.a += 0.012f;
				if (g_aWindow[nCntWindow].col.a >= 1.0f)
				{
					g_aWindow[nCntWindow].col.a = 1.0f;
					g_aWindow[nCntWindow].state = WINDOWSTATE_NORMAL;
				}
				break;
			case WINDOWSTATE_FADEOUT://設定された透明度から暗くなり0で消える
				g_aWindow[nCntWindow].col.a -= 0.012f;
				if (g_aWindow[nCntWindow].col.a <= 0.0f)
				{
					g_aWindow[nCntWindow].col.a = 0.0f;
					g_aWindow[nCntWindow].bUse = false;
				}
				break;

			case WINDOWSTATE_FADEOUT_B://設定された透明度から暗くなり0で消える
				g_aWindow[nCntWindow].col.a -= 0.012f;
				g_aWindow[nCntWindow].fLengthX *= 1.01f;
				g_aWindow[nCntWindow].fLengthY *= 1.01f;
				if (g_aWindow[nCntWindow].col.a <= 0.0f)
				{
					g_aWindow[nCntWindow].col.a = 0.0f;
					g_aWindow[nCntWindow].bUse = false;
				}
				break;

			case WINDOWSTATE_FADE_ALL://設定された透明度から暗くなり0で消える
				if (g_aWindow[nCntWindow].nCounterState[0] == 0)
				{
					g_aWindow[nCntWindow].col.a += 0.05f;
					if (g_aWindow[nCntWindow].col.a >= 1.0f)
					{
						g_aWindow[nCntWindow].col.a = 1.0f;
						g_aWindow[nCntWindow].nCounterState[0] = 1;
					}
				}
				else { g_aWindow[nCntWindow].nCounterState[0]++; }

				if (g_aWindow[nCntWindow].nCounterState[0] > 0)
				{
					g_aWindow[nCntWindow].col.a -= 0.05f;
					if (g_aWindow[nCntWindow].col.a <= 0.0f)
					{
						g_aWindow[nCntWindow].col.a = 0.0f;
						g_aWindow[nCntWindow].bUse = false;
					}
				}
				break;

			case WINDOWSTATE_FLASH://点滅で表示
				g_aWindow[nCntWindow].nCounterState[0]++;
				if (g_aWindow[nCntWindow].nCounterState[1] % 2 == 0 && g_aWindow[nCntWindow].nCounterState[0] % 45 == 0)
				{
					g_aWindow[nCntWindow].col.a = 1.0f;
					g_aWindow[nCntWindow].nCounterState[1]++;
				}
				else if (g_aWindow[nCntWindow].nCounterState[0] % 45 == 0)
				{
					g_aWindow[nCntWindow].col.a = 0.0f;
					g_aWindow[nCntWindow].nCounterState[1]++;
				}
				break;

			case WINDOWSTATE_SELECTON://SELECT処理で使用され、選択されているとき
				g_aWindow[nCntWindow].col.r += fStealth;
				g_aWindow[nCntWindow].col.g += fStealth;
				g_aWindow[nCntWindow].col.b += fStealth;
				g_aWindow[nCntWindow].col.a = 1.0f;
				if (g_aWindow[nCntWindow].col.r >= 1.0f || g_aWindow[nCntWindow].col.r <= 0.22f)
				{//明るさが最大, 最小になったら反転
					fStealth *= -1.0f;
				}
				break;

			case WINDOWSTATE_SELECTOFF://SELECT処理で使用され、選択されていないとき
				g_aWindow[nCntWindow].col.r = 0.23f;
				g_aWindow[nCntWindow].col.g = 0.23f;
				g_aWindow[nCntWindow].col.b = 0.23f;
				g_aWindow[nCntWindow].col.a = 1.0f;
				break;

			case WINDOWSTATE_LEVELUP:
				g_aWindow[nCntWindow].col.a -= 0.01f;
				g_aWindow[nCntWindow].pos.y -= 3;
				g_aWindow[nCntWindow].initpos.y -= 3;
				if (g_aWindow[nCntWindow].col.a <= 0.0f)
				{
					g_aWindow[nCntWindow].col.a = 0.0f;
					g_aWindow[nCntWindow].bUse = false;
				}
			case WINDOWSTATE_VIBRATION:	//振動
				fRand = (rand() % 628) * 0.01f;
				nRand = rand() % 30 + 5;
				g_aWindow[nCntWindow].nCounterState[0] = (g_aWindow[nCntWindow].nCounterState[0] + 1) % 2;
				if (g_aWindow[nCntWindow].nCounterState[0] == 0) { g_aWindow[nCntWindow].pos += D3DXVECTOR3(sinf(fRand) * nRand, cosf(fRand) * nRand, 0.0f); }
				if (g_aWindow[nCntWindow].nCounterState[0] == 1) { g_aWindow[nCntWindow].pos = g_aWindow[nCntWindow].initpos; }
				break;

			case WINDOWSTATE_FLOW:
				if (g_aWindow[nCntWindow].nCounterState[0] == 0)
				{//左から右に流れながら表示
					g_aWindow[nCntWindow].col.a += 0.05f;
					g_aWindow[nCntWindow].pos.x += 20.0f;
					if (g_aWindow[nCntWindow].col.a >= 1.0f)
					{
						g_aWindow[nCntWindow].col.a = 1.0f;
						g_aWindow[nCntWindow].nCounterState[0] = 1;
					}
				}
				else
				{//時間経過で
					g_aWindow[nCntWindow].nCounterState[0]++;
					if (g_aWindow[nCntWindow].nCounterState[0] > 60)
					{//さらに時間経過で
						g_aWindow[nCntWindow].col.a -= 0.029f;
						g_aWindow[nCntWindow].pos.x += 8.0f;
						if (g_aWindow[nCntWindow].col.a <= 0.0f)
						{//右に流れながら消える
							g_aWindow[nCntWindow].col.a = 0.0f;
							g_aWindow[nCntWindow].bUse = false;
						}
					}
				}
				break;

			case WINDOWSTATE_SHOT:	//ズームから等倍に
				if (g_aWindow[nCntWindow].nCounterState[0] > 0)
				{//nLengthを倍率で割る
					fEmpty = (10000 - g_aWindow[nCntWindow].nCounterState[0]) * 0.001f;
					g_aWindow[nCntWindow].fLengthX = g_aWindow[nCntWindow].fLengthX / fEmpty;
					g_aWindow[nCntWindow].fLengthY = g_aWindow[nCntWindow].fLengthY / fEmpty;
					g_aWindow[nCntWindow].nCounterState[1] += 4;
				}

				if (g_aWindow[nCntWindow].nCounterState[0] < 9000) { g_aWindow[nCntWindow].nCounterState[0] += g_aWindow[nCntWindow].nCounterState[1]; }
				if (g_aWindow[nCntWindow].nCounterState[0] > 9000) 
				{ 
					SetExplosion(g_aWindow[nCntWindow].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 300, 300, 5, 2, 5, 1, ANIMATION_NORMAL, EXPLOSIONSTATE_EFFECT, D3DX_PI, 1);
					SetExplosion(g_aWindow[nCntWindow].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 300, 300, 5, 2, 3, 1, ANIMATION_NORMAL, EXPLOSIONSTATE_EFFECT, D3DX_PI * 0.75f, 1);
					SetExplosion(g_aWindow[nCntWindow].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 300, 300, 5, 2, 2, 1, ANIMATION_NORMAL, EXPLOSIONSTATE_EFFECT, D3DX_PI * 0.5f, 1);
					SetExplosion(g_aWindow[nCntWindow].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 300, 300, 5, 2, 4, 1, ANIMATION_NORMAL, EXPLOSIONSTATE_EFFECT, D3DX_PI * 0.25f, 1);
					SetExplosion(g_aWindow[nCntWindow].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 300, 300, 5, 2, 6, 1, ANIMATION_NORMAL, EXPLOSIONSTATE_EFFECT, D3DX_PI * 0.0f, 1);
					SetExplosion(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.1f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 2, 5, 1, 2, ANIMATION_LOOP, EXPLOSIONSTATE_FADE, D3DX_PI * 0.0f, 1);
					SetWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.85f, 0.0f), D3DXCOLOR{ 1.0f,1.0f,1.0f, 1.0f }, 150, 50, 0, 0, 1, 1, 0, WINDOW_UPDATE_SELECT, WINDOWSTATE_FLASH, WINDOWUSE_NORMAL, WINDOWTYPE_CENTER, WINDOW_DRAW_2);
					g_aWindow[nCntWindow].nCounterState[0] = 9000;
					if (game_play[0].bUse == true)
					{
						game_play[0].bUse = false;
						PlaySound(SOUND_LABEL_TitleSE001);
						PlaySound(SOUND_LABEL_TITLE000);
					}

					MODEL_SETTING set;
					SetRunningman(&set, MODEL_TYPE_RUN_Sword0);
					
					CHAR_DATA data;
					data = GetGifted("石田");

					GetStatus(&data, false);
					data.nLifePoint = (int)data.status.fLifePoint;
					data.nSkillPoint = (int)data.status.fSkillPoint;
					for (int nCntModel = 0; nCntModel < 15; nCntModel++)
					{
						nRand = rand() % 3;
						if(nRand == 0){ set.parent[15].type = MODEL_TYPE_RUN_Sword0; }
						else if (nRand == 1) { set.parent[15].type = MODEL_TYPE_RUN_Sword1; }
						else { set.parent[15].type = MODEL_TYPE_RUN_Sword2; }
						
						SetModel(D3DXVECTOR3(200.0f + nCntModel * 30.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), MODEL_USE_Title, &set, &data, 0.0f, 0.0f);
					}
				}
				fEmpty = (10000 - g_aWindow[nCntWindow].nCounterState[0]) * 0.001f;
				g_aWindow[nCntWindow].fLengthX = g_aWindow[nCntWindow].fLengthX * fEmpty;
				g_aWindow[nCntWindow].fLengthY = g_aWindow[nCntWindow].fLengthY * fEmpty;
				g_aWindow[nCntWindow].col.a = 1.1f - fEmpty * 00.1f;
				break;

			case WINDOWSTATE_FLOW0:
				if (g_aWindow[nCntWindow].nCounterState[1] < 94)
				{
					if (g_aWindow[nCntWindow].nCounterState[1] > 10)
					{ SetExplosion(g_aWindow[nCntWindow].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 300, 300, 8, 1, 2, 0, ANIMATION_NORMAL, EXPLOSIONSTATE_DELETE, D3DX_PI, 1); }
					g_aWindow[nCntWindow].initpos += D3DXVECTOR3(sinf(D3DX_PI * -0.65f), cosf(D3DX_PI * -0.65f), 0.0f) * (g_aWindow[nCntWindow].nCounterState[0] * 0.015f);
					g_aWindow[nCntWindow].pos += g_aWindow[nCntWindow].initpos;
					g_aWindow[nCntWindow].initpos *= 0.8f;
					g_aWindow[nCntWindow].nCounterState[0] -= g_aWindow[nCntWindow].nCounterState[1];
					if (g_aWindow[nCntWindow].nCounterState[0] < 0) { g_aWindow[nCntWindow].nCounterState[1]++; }
				}
				if(g_aWindow[nCntWindow].nCounterState[1] == 30 && game_play[0].bUse == true){ PlaySound(SOUND_LABEL_TitleSE000); }
				g_aWindow[nCntWindow].nCounterState[1]++;
				break;

			case WINDOWSTATE_FLOW1:
				if (g_aWindow[nCntWindow].nCounterState[1] < 94)
				{
					if (g_aWindow[nCntWindow].nCounterState[1] > 10)
					{ SetExplosion(g_aWindow[nCntWindow].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 300, 300, 8, 1, 2, 0, ANIMATION_NORMAL, EXPLOSIONSTATE_DELETE, D3DX_PI, 1); }
					g_aWindow[nCntWindow].initpos += D3DXVECTOR3(sinf(D3DX_PI * -0.65f), cosf(D3DX_PI * -0.65f), 0.0f) * (g_aWindow[nCntWindow].nCounterState[0] * 0.015f);
					g_aWindow[nCntWindow].pos -= g_aWindow[nCntWindow].initpos;
					g_aWindow[nCntWindow].initpos *= 0.8f;
					g_aWindow[nCntWindow].nCounterState[0] -= g_aWindow[nCntWindow].nCounterState[1];
					if (g_aWindow[nCntWindow].nCounterState[0] < 0) { g_aWindow[nCntWindow].nCounterState[1]++; }
				}
				g_aWindow[nCntWindow].nCounterState[1]++;
				break;
			}

			switch (g_aWindow[nCntWindow].PosType)
			{

			case WINDOWTYPE_CENTER:
				pVtx[nCntWindow * 4].pos = D3DXVECTOR3(g_aWindow[nCntWindow].pos.x + (sinf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].fLengthY) + (cosf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].fLengthX), g_aWindow[nCntWindow].pos.y - (sinf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].fLengthX) + (cosf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].fLengthY), 0.0f);
				pVtx[nCntWindow * 4 + 1].pos = D3DXVECTOR3(g_aWindow[nCntWindow].pos.x + (sinf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].fLengthY) - (cosf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].fLengthX), g_aWindow[nCntWindow].pos.y + (sinf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].fLengthX) + (cosf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].fLengthY), 0.0f);
				pVtx[nCntWindow * 4 + 2].pos = D3DXVECTOR3(g_aWindow[nCntWindow].pos.x - (sinf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].fLengthY) + (cosf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].fLengthX), g_aWindow[nCntWindow].pos.y - (sinf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].fLengthX) - (cosf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].fLengthY), 0.0f);
				pVtx[nCntWindow * 4 + 3].pos = D3DXVECTOR3(g_aWindow[nCntWindow].pos.x - (sinf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].fLengthY) - (cosf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].fLengthX), g_aWindow[nCntWindow].pos.y + (sinf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].fLengthX) - (cosf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].fLengthY), 0.0f);
				break;
			case WINDOWTYPE_LEFT_DOWN:
				pVtx[nCntWindow * 4].pos = D3DXVECTOR3(g_aWindow[nCntWindow].pos.x + (sinf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].fLengthY), g_aWindow[nCntWindow].pos.y + (cosf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].fLengthY), 0.0f);
				pVtx[nCntWindow * 4 + 1].pos = D3DXVECTOR3(g_aWindow[nCntWindow].pos.x + (sinf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].fLengthY) - (cosf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].fLengthX), g_aWindow[nCntWindow].pos.y + (sinf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].fLengthX) + (cosf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].fLengthY), 0.0f);
				pVtx[nCntWindow * 4 + 2].pos = D3DXVECTOR3(g_aWindow[nCntWindow].pos.x, g_aWindow[nCntWindow].pos.y, 0.0f);
				pVtx[nCntWindow * 4 + 3].pos = D3DXVECTOR3(g_aWindow[nCntWindow].pos.x - (cosf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].fLengthX), g_aWindow[nCntWindow].pos.y + (sinf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].fLengthX), 0.0f);
				break;

			}
			pVtx[nCntWindow * 4].col = g_aWindow[nCntWindow].col;	// A = 透明度
			pVtx[nCntWindow * 4 + 1].col = g_aWindow[nCntWindow].col;
			pVtx[nCntWindow * 4 + 2].col = g_aWindow[nCntWindow].col;
			pVtx[nCntWindow * 4 + 3].col = g_aWindow[nCntWindow].col;
		}
	}
	//頂点BUFFERをアンロックする
	g_pVtxBuffWindow->Unlock();
}
//=========================================================================================================//
// * 背景の描画処理4	描画処理
//=========================================================================================================//
void DrawWindow(WINDOW_DRAW draw)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntWindow;

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffWindow, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, PRIMITIVE_NUM);
	// ポリゴンの描画
	for (nCntWindow = 0; nCntWindow < MAX_WINDOW; nCntWindow++)
	{
		if (g_aWindow[nCntWindow].bUse == true && g_aWindow[nCntWindow].draw == draw)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureWindow[g_aWindow[nCntWindow].nType]);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntWindow * 4, WINDOW_PRIMITIVE_NUM);
		}
	}

}
//=============================================================================
// 弾の設定処理
//=============================================================================
int SetWindow(D3DXVECTOR3 pos, D3DXCOLOR col, int nLengthX, int nLengthY, int nType, int nPtnUV, int nPtnWide, int nPtnHeight
	, int nCntUse, WINDOW_UPDATE nUpdateType, WINDOWSTATE state, WINDOWUSE use, WINDOWTYPE type, WINDOW_DRAW draw)
{
	int nCntWindow;
	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点BUFFERをロックし、頂点情報へのポインタを取得
	g_pVtxBuffWindow->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntWindow = 1; nCntWindow < MAX_WINDOW; nCntWindow++)
	{
		if (g_aWindow[nCntWindow].bUse == false)
		{
			g_aWindow[nCntWindow].pos = pos;
			g_aWindow[nCntWindow].initpos = pos;
			g_aWindow[nCntWindow].fLengthX = nLengthX * 1.0f;
			g_aWindow[nCntWindow].fLengthY = nLengthY * 1.0f;
			g_aWindow[nCntWindow].nType = nType;
			g_aWindow[nCntWindow].nUpdateType = nUpdateType;
			g_aWindow[nCntWindow].nPtnUV = nPtnUV;
			g_aWindow[nCntWindow].nPtnWide = nPtnWide;
			g_aWindow[nCntWindow].nPtnHeight = nPtnHeight;
			g_aWindow[nCntWindow].state = state;
			g_aWindow[nCntWindow].use = use;
			g_aWindow[nCntWindow].nCounterState[0] = nCntUse;
			g_aWindow[nCntWindow].nCounterState[1] = 1;
			g_aWindow[nCntWindow].PosType = type;
			g_aWindow[nCntWindow].draw = draw;
			g_aWindow[nCntWindow].bUse = true;

			g_aWindow[nCntWindow].fSteep = D3DX_PI;
			/*pVtx[nCntWindow * 4].pos = D3DXVECTOR3(g_aWindow[nCntWindow].pos.x - g_aWindow[nCntWindow].nLengthX, g_aWindow[nCntWindow].pos.y - g_aWindow[nCntWindow].nLengthY, 0.0f);
			pVtx[nCntWindow * 4 + 1].pos = D3DXVECTOR3(g_aWindow[nCntWindow].pos.x + g_aWindow[nCntWindow].nLengthX, g_aWindow[nCntWindow].pos.y - g_aWindow[nCntWindow].nLengthY, 0.0f);
			pVtx[nCntWindow * 4 + 2].pos = D3DXVECTOR3(g_aWindow[nCntWindow].pos.x - g_aWindow[nCntWindow].nLengthX, g_aWindow[nCntWindow].pos.y + g_aWindow[nCntWindow].nLengthY, 0.0f);
			pVtx[nCntWindow * 4 + 3].pos = D3DXVECTOR3(g_aWindow[nCntWindow].pos.x + g_aWindow[nCntWindow].nLengthX, g_aWindow[nCntWindow].pos.y + g_aWindow[nCntWindow].nLengthY, 0.0f);*/

			pVtx[nCntWindow * 4].pos = D3DXVECTOR3(g_aWindow[nCntWindow].pos.x + (sinf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].fLengthY) + (cosf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].fLengthX), g_aWindow[nCntWindow].pos.y - (sinf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].fLengthX) + (cosf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].fLengthY), 0.0f);
			pVtx[nCntWindow * 4 + 1].pos = D3DXVECTOR3(g_aWindow[nCntWindow].pos.x + (sinf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].fLengthY) - (cosf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].fLengthX), g_aWindow[nCntWindow].pos.y + (sinf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].fLengthX) + (cosf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].fLengthY), 0.0f);
			pVtx[nCntWindow * 4 + 2].pos = D3DXVECTOR3(g_aWindow[nCntWindow].pos.x - (sinf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].fLengthY) + (cosf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].fLengthX), g_aWindow[nCntWindow].pos.y - (sinf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].fLengthX) - (cosf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].fLengthY), 0.0f);
			pVtx[nCntWindow * 4 + 3].pos = D3DXVECTOR3(g_aWindow[nCntWindow].pos.x - (sinf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].fLengthY) - (cosf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].fLengthX), g_aWindow[nCntWindow].pos.y + (sinf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].fLengthX) - (cosf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].fLengthY), 0.0f);

			g_aWindow[nCntWindow].col = col;
			pVtx[nCntWindow * 4].col = col;	// A = 透明度
			pVtx[nCntWindow * 4 + 1].col = col;
			pVtx[nCntWindow * 4 + 2].col = col;
			pVtx[nCntWindow * 4 + 3].col = col;

			pVtx[nCntWindow * 4].tex = D3DXVECTOR2((g_aWindow[nCntWindow].nPtnUV % g_aWindow[nCntWindow].nPtnWide) * (1.0f / g_aWindow[nCntWindow].nPtnWide), (g_aWindow[nCntWindow].nPtnUV / g_aWindow[nCntWindow].nPtnWide) * (1.0f / g_aWindow[nCntWindow].nPtnHeight));
			pVtx[nCntWindow * 4 + 1].tex = D3DXVECTOR2((g_aWindow[nCntWindow].nPtnUV % g_aWindow[nCntWindow].nPtnWide) * (1.0f / g_aWindow[nCntWindow].nPtnWide) + (1.0f / g_aWindow[nCntWindow].nPtnWide), (g_aWindow[nCntWindow].nPtnUV / g_aWindow[nCntWindow].nPtnWide) * (1.0f / g_aWindow[nCntWindow].nPtnHeight));
			pVtx[nCntWindow * 4 + 2].tex = D3DXVECTOR2((g_aWindow[nCntWindow].nPtnUV % g_aWindow[nCntWindow].nPtnWide) * (1.0f / g_aWindow[nCntWindow].nPtnWide), (g_aWindow[nCntWindow].nPtnUV / g_aWindow[nCntWindow].nPtnWide) * (1.0f / g_aWindow[nCntWindow].nPtnHeight) + (1.0f / g_aWindow[nCntWindow].nPtnHeight));
			pVtx[nCntWindow * 4 + 3].tex = D3DXVECTOR2((g_aWindow[nCntWindow].nPtnUV % g_aWindow[nCntWindow].nPtnWide) * (1.0f / g_aWindow[nCntWindow].nPtnWide) + (1.0f / g_aWindow[nCntWindow].nPtnWide), (g_aWindow[nCntWindow].nPtnUV / g_aWindow[nCntWindow].nPtnWide) * (1.0f / g_aWindow[nCntWindow].nPtnHeight) + (1.0f / g_aWindow[nCntWindow].nPtnHeight));

			break;
		}
	}
	//頂点BUFFERをアンロックする
	g_pVtxBuffWindow->Unlock();

	return nCntWindow;
}
//=============================================================================
// windowの破壊
//=============================================================================
void BreakWindow(void)
{//全ウィンドウ破棄
	int nCntWindow;

	for (nCntWindow = 0; nCntWindow < MAX_WINDOW; nCntWindow++)
	{ g_aWindow[nCntWindow].bUse = false; }
}
//=============================================================================
// windowの指定変更
//=============================================================================
void ChangeWindow(int nCntWindow, D3DXCOLOR col, WINDOWSTATE state)
{
	if (g_aWindow[nCntWindow].bUse == true)
	{
		g_aWindow[nCntWindow].col = col;
		if(state != WINDOWSTATE_MAX) { g_aWindow[nCntWindow].state = state; }
	}
}
//=============================================================================
// windowの破壊
//=============================================================================
void CheckGage(float fGage, float fMaxGage, int nCntWindow)
{
	float fPercent;
	float fPlus;
	float fLength = 0.0f;
	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点BUFFERをロックし、頂点情報へのポインタを取得
	g_pVtxBuffWindow->Lock(0, 0, (void**)&pVtx, 0);

	//最大値から%を求める
	fPercent = fGage / fMaxGage;
	if (fPercent > 1.0f) { fPercent = 1.0f; }

	//Windowのタイプによって長さを変更
	switch (g_aWindow[nCntWindow].state)
	{//長さの倍率にかける
	case WINDOWSTATE_LIFEGAGE:
		fLength = 430.0f;
		if(g_aWindow[nCntWindow].fLengthX > fLength * fPercent){ g_aWindow[nCntWindow].fLengthX = fLength * fPercent; }
		else
		{//だんだん増える
			fPlus = (fLength * fPercent - g_aWindow[nCntWindow].fLengthX) * 0.03f;
			if (fPlus < fLength * 0.001f) { fPlus = fLength * 0.001f; }
			g_aWindow[nCntWindow].fLengthX += fPlus;
			if (g_aWindow[nCntWindow].fLengthX > fLength * fPercent) { g_aWindow[nCntWindow].fLengthX = fLength * fPercent; }
		}
		break;

	case WINDOWSTATE_MPGAGE:
		fLength = 400.0f;
		if (g_aWindow[nCntWindow].fLengthX > fLength * fPercent) { g_aWindow[nCntWindow].fLengthX = fLength * fPercent; }
		else
		{//だんだん増える
			fPlus = (fLength * fPercent - g_aWindow[nCntWindow].fLengthX) * 0.03f;
			if (fPlus < fLength * 0.001f) { fPlus = fLength * 0.001f; }
			g_aWindow[nCntWindow].fLengthX += fPlus;
			if (g_aWindow[nCntWindow].fLengthX > fLength * fPercent) { g_aWindow[nCntWindow].fLengthX = fLength * fPercent; }
		}
		break;

	case WINDOWSTATE_AUTOLIFEGAGE:
		fLength = 430.0f;

		if (g_aWindow[nCntWindow].fLengthX > fLength * fPercent)
		{//だんだん減る
			fPlus = (fLength * fPercent - g_aWindow[nCntWindow].fLengthX) * 0.03f;
			if (fPlus < fLength * 0.0015f) { fPlus = fLength * 0.0015f; }
			g_aWindow[nCntWindow].fLengthX -= fPlus;
			g_aWindow[nCntWindow].col = D3DXCOLOR(0.76f, 0.1f, 0.0f, 1.0f);
			if (g_aWindow[nCntWindow].fLengthX < fLength * fPercent) { g_aWindow[nCntWindow].fLengthX = fLength * fPercent; }
		}
		else if (g_aWindow[nCntWindow].fLengthX < fLength * fPercent)
		{//そのまま代入
			g_aWindow[nCntWindow].fLengthX = fLength * fPercent;
			g_aWindow[nCntWindow].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		else { g_aWindow[nCntWindow].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); }
		break;

	case WINDOWSTATE_AUTOMPGAGE:
		fLength = 400.0f;

		if (g_aWindow[nCntWindow].fLengthX > fLength * fPercent)
		{//だんだん減る
			fPlus = (fLength * fPercent - g_aWindow[nCntWindow].fLengthX) * 0.03f;
			if (fPlus < fLength * 0.0015f) { fPlus = fLength * 0.0015f; }
			g_aWindow[nCntWindow].fLengthX -= fPlus;
			g_aWindow[nCntWindow].col = D3DXCOLOR(0.45f, 0.0f, 3.0f, 1.0f);
			if (g_aWindow[nCntWindow].fLengthX < fLength * fPercent) { g_aWindow[nCntWindow].fLengthX = fLength * fPercent; }
		}
		else if (g_aWindow[nCntWindow].fLengthX < fLength * fPercent)
		{//そのまま代入
			g_aWindow[nCntWindow].fLengthX = fLength * fPercent;
			g_aWindow[nCntWindow].col = D3DXCOLOR(0.99f, 1.0f, 0.18f, 1.0f);
		}
		else { g_aWindow[nCntWindow].col = D3DXCOLOR(0.99f, 1.0f, 0.18f, 1.0f); }
		break;
	}

	pVtx[nCntWindow * 4].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[nCntWindow * 4 + 1].tex = D3DXVECTOR2((g_aWindow[nCntWindow].fLengthX / fLength) * 1.0f, 0.0f);
	pVtx[nCntWindow * 4 + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[nCntWindow * 4 + 3].tex = D3DXVECTOR2((g_aWindow[nCntWindow].fLengthX / fLength) * 1.0f, 1.0f);


	//頂点BUFFERをアンロックする
	g_pVtxBuffWindow->Unlock();
}
//=============================================================================
// 選択処理
//=============================================================================
void SelectWindow(int nNumber)
{
	int nCntWindow;

	for (nCntWindow = 0; nCntWindow < MAX_WINDOW; nCntWindow++)
	{
		if (g_aWindow[nCntWindow].use == WINDOWUSE_SELECT && g_aWindow[nCntWindow].bUse == true)
		{
			if (g_aWindow[nCntWindow].nCounterState[0] == nNumber)
			{
				g_aWindow[nCntWindow].state = WINDOWSTATE_SELECTON;
			}
			else
			{
				g_aWindow[nCntWindow].state = WINDOWSTATE_SELECTOFF;
			}
		}
	}
}
//=============================================================================
// 指定したwindow分破壊
//=============================================================================
void BackWindow(int nNumber)
{
	int nCntWindow;			//windowを数える
	int nBreak = nNumber;	//nNumberいじったらどうなるかわからんし...

	for (nCntWindow = MAX_WINDOW; nCntWindow > 0; nCntWindow--)
	{//最大でも全部終わったら終了
		if (g_aWindow[nCntWindow - 1].bUse == true)
		{//後ろから見てtrueになっとるやつを殺す
			g_aWindow[nCntWindow - 1].bUse = false;
			nBreak--;
			if (nBreak < 1)
			{//0以下になったら終わり
				break;
			}
		}
	}
}
//=============================================================================
// 50音表示の枠移動処理
//=============================================================================
void FontWindowMove(int nWide, int nHeight)
{
	int nCntWindow;

	for (nCntWindow = 0; nCntWindow < MAX_WINDOW; nCntWindow++)
	{
		if (g_aWindow[nCntWindow].use == WINDOWUSE_FONT)
		{
			if (nHeight == 5)
			{//ひら カナ 終わりの場合
				g_aWindow[nCntWindow].fLengthY = SCREEN_HEIGHT / 30;
				g_aWindow[nCntWindow].pos.y = SCREEN_HEIGHT * 0.805f;
				if (nWide == 0)
				{
					g_aWindow[nCntWindow].fLengthX = SCREEN_WIDTH / 20;
					g_aWindow[nCntWindow].pos.x = SCREEN_WIDTH * 0.405f;
				}
				else if (nWide == 1)
				{
					g_aWindow[nCntWindow].fLengthX = SCREEN_WIDTH / 20;
					g_aWindow[nCntWindow].pos.x = SCREEN_WIDTH * 0.505f;
				}
				else if (nWide == 2)
				{
					g_aWindow[nCntWindow].fLengthX = SCREEN_WIDTH / 17;
					g_aWindow[nCntWindow].pos.x = SCREEN_WIDTH * 0.608f;
				}
				else if (nWide == 3)
				{
					g_aWindow[nCntWindow].fLengthX = SCREEN_WIDTH / 17;
					g_aWindow[nCntWindow].pos.x = SCREEN_WIDTH * 0.7305f;
				}
			}
			else
			{//ひら カナ 終わり 以外
				g_aWindow[nCntWindow].fLengthX = SCREEN_WIDTH / 38;
				g_aWindow[nCntWindow].fLengthY = SCREEN_HEIGHT / 19;
				g_aWindow[nCntWindow].pos.x = nWide * SCREEN_WIDTH * 0.0525f + SCREEN_WIDTH * 0.2365f;
				g_aWindow[nCntWindow].pos.y = nHeight * SCREEN_HEIGHT * 0.101f + SCREEN_HEIGHT * 0.31f;
			}
		}
	}
}

//=============================================================================
// プレイヤー更新処理
//=============================================================================
void ColPlayerWindow(float fCola)
{
	PLAYER *pPlayer = GetPlayer();

	g_aWindow[pPlayer[0].nCntHPGage[0]].col.a = fCola;
	g_aWindow[pPlayer[0].nCntHPGage[1]].col.a = fCola;
	g_aWindow[pPlayer[0].nCntHPGage[2]].col.a = fCola;
	g_aWindow[pPlayer[0].nCntMPGage[0]].col.a = fCola;
	g_aWindow[pPlayer[0].nCntMPGage[1]].col.a = fCola;
	g_aWindow[pPlayer[0].nCntMPGage[2]].col.a = fCola;
	g_aWindow[pPlayer[0].nCntEXPGage[0]].col.a = fCola;
	g_aWindow[pPlayer[0].nCntEXPGage[1]].col.a = fCola;
	g_aWindow[pPlayer[0].nCntGageWindow].col.a = fCola;
	g_aWindow[pPlayer[0].nCntSkillWindow[0]].col.a = fCola;
	g_aWindow[pPlayer[0].nCntSkillWindow[1]].col.a = fCola;
	for (int nCount = 0; nCount < 8; nCount++)
	{
		g_aWindow[pPlayer[0].nCntIcon[nCount]].col.a = fCola;
	}
	
}