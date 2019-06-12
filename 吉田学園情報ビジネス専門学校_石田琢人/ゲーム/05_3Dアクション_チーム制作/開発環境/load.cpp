//=============================================================================
//
// エリア移動処理 [load.cpp]
// Author : shotaro kanamaru
//
//=============================================================================
#include "main.h"		// メイン
#include "load.h"		// エリア移動
#include "input.h"		// 入力
#include "sound.h"		// 音
#include "fade.h"		// フェード
#include "clear.h"

//*****************************************************************************
//マクロ定義
//*****************************************************************************
#define LOAD_TEX_PLAYER			"data/TEXTURE/load_player.png"	// プレイヤーアイコン
#define LOAD_TEX_BG				"data/TEXTURE/load_bg.png"		// 背景
#define LOAD_TEX_LINE			"data/TEXTURE/load_line.png"	// ステージライン
#define LOAD_MAX_STAGE			(3)								// ステージ数
#define LOAD_LINE_WIDTH			(1000.0f)						// ステージラインの幅
#define LOAD_LINE_HEIGHT		(100.0f)						// ステージラインの高さ
#define LOAD_ICON_DEF			(LOAD_LINE_WIDTH / (LOAD_MAX_STAGE - 1))	// プレイヤーアイコンの目的位置
#define LOAD_START				(60)							// 全体的な処理の開始時間

//*****************************************************************************
//プロトタイプ宣言
//*****************************************************************************
void LoadMakeVtx(void);						// 頂点情報の作成
void LoadPlayerIcon(void);					// プレイヤーアイコンの処理
void LoadFade(void);						// フェードの処理
void LoadBg(void);							// 背景の処理
void LoadColAExtent(void);					// 透明度の範囲制限

//*****************************************************************************
// グローバル変数
//*****************************************************************************
Load			g_aLoad[LOADTYPE_MAX];		// エリア移動情報
LoadOther		g_loadOther;				// その他設定情報
LOADFADE		g_loadFade;					//フェードの状態
// パーツの設定情報
LoadInfo		g_aLoadInfo[LOADTYPE_MAX] =
{// [位置]:[大きさ]:[アニメーション数]
	/*塗りつぶし*/		D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),		D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f),1,
	/*背景*/			D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),		D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f),3,
	/*ステージライン*/	D3DXVECTOR3(SCREEN_WIDTH / 2, 500.0f, 0.0f),				D3DXVECTOR3(LOAD_LINE_WIDTH, LOAD_LINE_HEIGHT, 0.0f),1,
	/*プレイヤー*/		D3DXVECTOR3(-LOAD_LINE_WIDTH / 2, -LOAD_LINE_HEIGHT,0.0f) + g_aLoadInfo[LOADTYPE_LINE].pos	,D3DXVECTOR3(70.0f, 100.0f, 0.0f),6,
	/*フェード*/		D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),		D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f),1,
};

//=============================================================================
//エリア移動の初期化処理
//=============================================================================
void InitLoad(void)
{
	// 設定情報の初期化
	g_loadOther.nNextStage = 0;					// 次のステージ
	g_loadOther.nCntAll = 0;					// 共通カウンタ
	g_loadOther.state = LOADSTATE_NONE;			// 状態
	g_loadOther.bUse = false;					// 描画状態
	g_loadFade = LOADFADE_NONE;					// フェード

	for (int nCntLoad = 0; nCntLoad < LOADTYPE_MAX; nCntLoad++)
	{// 情報の初期化
		// 位置
		g_aLoad[nCntLoad].pos = g_aLoadInfo[nCntLoad].pos;
		// 大きさ
		g_aLoad[nCntLoad].size = g_aLoadInfo[nCntLoad].size / 2;
		// 移動量
		g_aLoad[nCntLoad].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		// 色
		g_aLoad[nCntLoad].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		// 使用
		g_aLoad[nCntLoad].bUse = true;
		// 現在のアニメーション
		g_aLoad[nCntLoad].nNumPattern = 0;
	}

	// 塗りつぶし色の設定
	g_aLoad[LOADTYPE_BACK].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	// フェードの設定
	g_aLoad[LOADTYPE_FADE].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

	LoadMakeVtx();		// 頂点情報の作成
}

//=============================================================================
//エリア移動の終了処理
//=============================================================================
void UninitLoad(void)
{
	// 頂点バッファの破棄
	if (g_loadOther.pVtxBuff != NULL)
	{
		g_loadOther.pVtxBuff->Release();
		g_loadOther.pVtxBuff = NULL;
	}
	// テクスチャの破棄
	for (int nCntTex = 0; nCntTex < LOADTYPE_MAX; nCntTex++)
	{
		if (g_loadOther.pTexture[nCntTex] != NULL)
		{
			g_loadOther.pTexture[nCntTex]->Release();
			g_loadOther.pTexture[nCntTex] = NULL;
		}
	}
}

//=============================================================================
//エリア移動の更新処理
//=============================================================================
void UpdateLoad(void)
{
	VERTEX_2D*pVtx;  //頂点情報へのポインタ

	// フェード
	//LoadFade();

	if (g_loadOther.pVtxBuff != NULL)
	{
		if (g_loadOther.state == LOADSTATE_START)
		{// 処理が開始されたら
			// プレイヤーアイコンの処理
			LoadPlayerIcon();
			// 背景の処理
			LoadBg();

			g_loadOther.nCntAll++;		// 共通カウンタの加算
		}
		else
		{
			g_loadOther.nCntAll = 0;		// 共通カウンタをリセット
		}

		// フェード処理
		LoadFade();
		// 透明度の範囲制限
		LoadColAExtent();

		//頂点バッファをロックし、頂点データへのポインタを取得
		g_loadOther.pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntLoad = 0; nCntLoad < LOADTYPE_MAX; nCntLoad++, pVtx += 4)
		{
			// 分割したテクスチャの幅の算出
			float fTexWidth = 1.0f / g_aLoadInfo[nCntLoad].nAnimPattern;

			// 移動量の更新
			g_aLoad[nCntLoad].pos += g_aLoad[nCntLoad].move;

			// 頂点座標
			pVtx[0].pos = D3DXVECTOR3(-g_aLoad[nCntLoad].size.x, -g_aLoad[nCntLoad].size.y, 0.0f) + g_aLoad[nCntLoad].pos;
			pVtx[1].pos = D3DXVECTOR3(g_aLoad[nCntLoad].size.x, -g_aLoad[nCntLoad].size.y, 0.0f) + g_aLoad[nCntLoad].pos;
			pVtx[2].pos = D3DXVECTOR3(-g_aLoad[nCntLoad].size.x, g_aLoad[nCntLoad].size.y, 0.0f) + g_aLoad[nCntLoad].pos;
			pVtx[3].pos = D3DXVECTOR3(g_aLoad[nCntLoad].size.x, g_aLoad[nCntLoad].size.y, 0.0f) + g_aLoad[nCntLoad].pos;

			// 頂点カラー
			pVtx[0].col = g_aLoad[nCntLoad].col;
			pVtx[1].col = g_aLoad[nCntLoad].col;
			pVtx[2].col = g_aLoad[nCntLoad].col;
			pVtx[3].col = g_aLoad[nCntLoad].col;

			// テクスチャ座標
			pVtx[0].tex = D3DXVECTOR2(fTexWidth * g_aLoad[nCntLoad].nNumPattern, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(fTexWidth * g_aLoad[nCntLoad].nNumPattern + fTexWidth, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(fTexWidth * g_aLoad[nCntLoad].nNumPattern, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(fTexWidth * g_aLoad[nCntLoad].nNumPattern + fTexWidth, 1.0f);
		}

		//頂点バッファをアンロック
		g_loadOther.pVtxBuff->Unlock();
	}
}

//=============================================================================
// プレイヤーアイコンの処理
//=============================================================================
void LoadPlayerIcon(void)
{
	if (g_aLoad[LOADTYPE_PLAYER].pos.x < LOAD_ICON_DEF * g_loadOther.nNextStage - LOAD_ICON_DEF + g_aLoad[LOADTYPE_LINE].pos.x)
	{// 指定位置に来ていなかったら
		if (g_loadOther.nCntAll > LOAD_START)
		{
		 // アイコンを次の場所へ移動
			g_aLoad[LOADTYPE_PLAYER].pos.x += 5.0f;
		}

		if (g_loadOther.nCntAll % 15 == 0)
		{// 歩行アニメーションの更新
			g_aLoad[LOADTYPE_PLAYER].nNumPattern = (g_aLoad[LOADTYPE_PLAYER].nNumPattern + 1) % 4;
		}
	}
	else
	{// 指定位置に来たら
		// 指定位置で停止
		g_aLoad[LOADTYPE_PLAYER].pos.x = LOAD_ICON_DEF * g_loadOther.nNextStage - LOAD_ICON_DEF + g_aLoad[LOADTYPE_LINE].pos.x;

		// ステージ移動音を止める
		StopSound(SOUND_LABEL_NEXTSTAGE);

		if (g_loadOther.nCntAll % 15 == 0)
		{// 挙手アニメーションの更新
			if (g_aLoad[LOADTYPE_PLAYER].nNumPattern != 5)
			{
				g_aLoad[LOADTYPE_PLAYER].nNumPattern = (g_aLoad[LOADTYPE_PLAYER].nNumPattern + 1) % 6;
			}
		}

		if (g_loadOther.nCntAll > 300)
		{// 挙手状態になったら
			// 終了状態に
			g_loadOther.state = LOADSTATE_OUT;
			// フェード開始
			g_loadFade = LOADFADE_OUT;
		}
		if (g_loadOther.nCntAll == 250)
		{// 音を出す
			PlaySound(SOUND_LABEL_HUNDUP);
		}
	}
}

//=============================================================================
// 背景の処理
//=============================================================================
void LoadBg(void)
{
	if (g_aLoad[LOADTYPE_BG].col.a == 0)
	{
		//static int nNowStage = 0;

		if (g_loadOther.nCntAll == LOAD_START + 30 - 1)
		{
			g_aLoad[LOADTYPE_BG].nNumPattern++;
		}
	}

	if (g_loadOther.nCntAll > LOAD_START + 30)
	{
		// 指定時間経過後
		g_aLoad[LOADTYPE_BG].col.a += 0.02f;
	}
	else if (g_loadOther.nCntAll > LOAD_START)
	{
		// 指定時間経過後
		g_aLoad[LOADTYPE_BG].col.a -= 0.06f;
	}
}

//=============================================================================
// フェード
//=============================================================================
void LoadFade(void)
{
	if (g_loadFade != LOADFADE_NONE)
	{//連打防止
		if (g_loadFade == LOADFADE_IN)
		{//フェードイン状態の時
			g_aLoad[LOADTYPE_FADE].col.a -= 0.02f;	//画面を透明に

			if (g_aLoad[LOADTYPE_FADE].col.a <= 0.0f)
			{
				g_aLoad[LOADTYPE_FADE].col.a = 0.0f;
				g_loadFade = LOADFADE_NONE;			//何もしていない状態へ

				if (g_loadOther.state == LOADSTATE_NONE)
				{// 終了状態だったら
					SetClearMessage(TYPE_NORMAL);
				}
				else
				{// ステージ移動音
					PlaySound(SOUND_LABEL_NEXTSTAGE);
				}
			}
		}
		else if (g_loadFade == LOADFADE_OUT)
		{//フェードアウト状態の時
			g_aLoad[LOADTYPE_FADE].col.a += 0.02f;	//画面を不透明に

			if (g_aLoad[LOADTYPE_FADE].col.a >= 1.0f)
			{
				g_aLoad[LOADTYPE_FADE].col.a = 1.0f;
				g_loadFade = LOADFADE_IN;			//フェードイン状態へ

				if (g_loadOther.state == LOADSTATE_NONE)
				{// 待機状態だったら
					// 描画開始
					g_loadOther.bUse = true;
					// 処理の開始
					g_loadOther.state = LOADSTATE_START;
				}
				else if (g_loadOther.state == LOADSTATE_OUT)
				{// 終了状態だったら
					// 描画終了
					g_loadOther.bUse = false;
					// 待機状態に
					g_loadOther.state = LOADSTATE_NONE;

					STAGENUM stage = GetNumState();
					if (stage == STAGENUM_1)
					{
						SetStageState(STAGENUM_2);
					}
					else if (stage == STAGENUM_2)
					{
						SetStageState(STAGENUM_3);
					}
				}
			}
		}
	}
}

//=============================================================================
// 透明度の範囲制限
//=============================================================================
void LoadColAExtent(void)
{
	for (int nCntLoad = 0; nCntLoad < LOADTYPE_MAX; nCntLoad++)
	{
		if (g_aLoad[nCntLoad].col.a < 0.0f)
		{// 0未満になったら
			g_aLoad[nCntLoad].col.a = 0.0f;
		}
		if (g_aLoad[nCntLoad].col.a > 1.0f)
		{// 1を超過したら
			g_aLoad[nCntLoad].col.a = 1.0f;
		}
	}
}

//=============================================================================
// エリア移動の起動
//=============================================================================
void SetLoad(void)
{
	if (g_loadOther.state == LOADSTATE_NONE &&
		g_loadFade == LOADFADE_NONE)
	{// 待機状態かつフェードがない状態
		// 移動地点の更新
		g_loadOther.nNextStage++;
		// フェード開始
		g_loadFade = LOADFADE_OUT;

		StageStateSwat(true);
	}
}

//=============================================================================
//エリア移動の描画処理
//=============================================================================
void DrawLoad(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_loadOther.pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntLoad = 0; nCntLoad < LOADTYPE_MAX; nCntLoad++)
	{
		if (g_loadOther.bUse ||
			nCntLoad == LOADTYPE_FADE)
		{// 描画状態もしくはフェード
			 // テクスチャの設定
			pDevice->SetTexture(0, g_loadOther.pTexture[nCntLoad]);
			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntLoad, 2);
		}
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void LoadMakeVtx(void)
{
	VERTEX_2D*pVtx;								// 頂点情報へのポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	//if (g_loadOther.pVtxBuff == NULL)
	{
		// テクスチャの読み込み
		// 背景
		D3DXCreateTextureFromFile(pDevice,
			LOAD_TEX_BG,
			&g_loadOther.pTexture[LOADTYPE_BG]);
		// プレイヤーアイコン
		D3DXCreateTextureFromFile(pDevice,
			LOAD_TEX_PLAYER,
			&g_loadOther.pTexture[LOADTYPE_PLAYER]);
		// ステージライン
		D3DXCreateTextureFromFile(pDevice,
			LOAD_TEX_LINE,
			&g_loadOther.pTexture[LOADTYPE_LINE]);

		// 頂点バッファの生成
		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * LOADTYPE_MAX,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&g_loadOther.pVtxBuff,
			NULL);

		//頂点バッファをロックして頂点データへのポインタを取得
		g_loadOther.pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点情報の設定
		for (int nCntLoad = 0; nCntLoad < LOADTYPE_MAX; nCntLoad++, pVtx += 4)
		{
			// 頂点座標
			pVtx[0].pos = D3DXVECTOR3(-g_aLoad[nCntLoad].size.x, -g_aLoad[nCntLoad].size.y, 0.0f) + g_aLoad[nCntLoad].pos;
			pVtx[1].pos = D3DXVECTOR3(g_aLoad[nCntLoad].size.x, -g_aLoad[nCntLoad].size.y, 0.0f) + g_aLoad[nCntLoad].pos;
			pVtx[2].pos = D3DXVECTOR3(-g_aLoad[nCntLoad].size.x, g_aLoad[nCntLoad].size.y, 0.0f) + g_aLoad[nCntLoad].pos;
			pVtx[3].pos = D3DXVECTOR3(g_aLoad[nCntLoad].size.x, g_aLoad[nCntLoad].size.y, 0.0f) + g_aLoad[nCntLoad].pos;

			// 頂点カラー
			pVtx[0].col = g_aLoad[nCntLoad].col;
			pVtx[1].col = g_aLoad[nCntLoad].col;
			pVtx[2].col = g_aLoad[nCntLoad].col;
			pVtx[3].col = g_aLoad[nCntLoad].col;

			// rhw
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			// テクスチャ座標
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		//頂点バッファをアンロック
		g_loadOther.pVtxBuff->Unlock();
	}
}

//=============================================================================
// ロードのフェード情報を取得
//=============================================================================
LOADFADE GetLoadFade(void)
{
	return g_loadFade;
}