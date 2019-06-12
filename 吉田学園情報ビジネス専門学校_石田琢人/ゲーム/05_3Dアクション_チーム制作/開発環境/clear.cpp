//=============================================================================
//
// クリアメッセージ処理 [clear.cpp]
// Author : shotaro kanamaru
//
//=============================================================================
#include "main.h"		// メイン
#include "clear.h"		// クリアメッセージ
#include "input.h"		// 入力
#include "sound.h"		// 音
#include "fade.h"		// フェード
#include "load.h"

//*****************************************************************************
//マクロ定義
//*****************************************************************************
#define MAX_CLEAR				(10)							// 最大数
// ノーマルクリア
#define CLEAR_NOR_TEX			"data/TEXTURE/戦闘開始.png"// テクスチャ
#define CLEAR_NOR_START			(30)							// 処理開始の時間
#define CLEAR_NORBAND_HEIGHT	(200.0f)						// 帯の太さ
#define CLEAR_NOR_BAND_COL		(D3DXCOLOR(0.3f,0.3f,0.3f,0.8f))// 帯の色
#define CLEAR_NOR_SPEED			(37.0f)							// 速度
#define CLEAR_NOR_CONNECT		(60)							// メッセージ表示の時間
#define CLEAR_NOR_WIDTH			(600.0f)						// メッセージの幅
#define CLEAR_NOR_HEIGHT		(200.0f)						// メッセージの高さ
#define CLEAR_NOR_DESTPOS_Y		(SCREEN_HEIGHT / 2)				// y位置
// コンプリートクリア
#define CLEAR_COMP_TEX_0		"data/TEXTURE/制.png"// 1文字目
#define CLEAR_COMP_TEX_1		"data/TEXTURE/圧.png"// 2文字目
#define CLEAR_COMP_TEX_2		"data/TEXTURE/完.png"// 3文字目
#define CLEAR_COMP_TEX_3		"data/TEXTURE/了.png"// 4文字目

#define CLEAR_COMP_START		(50)				// 処理開始の時間
#define CLEAR_COMP_NUM			(4)					// 文字数
#define CLEAR_COMP_SPACE		(30.0f)				// 文字の間隔
#define CLEAR_COMP_CONNECT		(220)				// メッセージ表示の時間
#define CLEAR_COMP_INITSIZE		(700.0f)			// 出現時の大きさ
#define CLEAR_COMP_DESTSIZE		(270.0f)			// 目的の大きさ
#define CLEAR_COMP_INTERVAL		(40)				// 表示間隔
#define CLEAR_COMP_REDUCT		(7.0f)				// 縮小速度

//*****************************************************************************
//プロトタイプ宣言
//*****************************************************************************
void ClearMakeVtx(void);						// 頂点情報の作成
void ClearBand(void);							// 帯の処理
void NormalMessage(void);						// ノーマルメッセージの処理
void CompleteMessage(void);						// コンプリートメッセージの処理
void ClearReset(void);							// 情報のリセット

//*****************************************************************************
// グローバル変数
//*****************************************************************************
Clear			g_aClear[MAX_CLEAR];			// クリアメッセージ情報
ClearOther		g_titleOther;					// その他設定情報

//=============================================================================
//クリアメッセージの初期化処理
//=============================================================================
void InitClear(void)
{
	// クリアメッセージの設定情報の初期化
	g_titleOther.nCntAll = 0;			// 共通カウンタ
	g_titleOther.nCntDraw = 0;			// 表示カウンタ
	g_titleOther.bSwitch = false;		// 移動の切替
	g_titleOther.type = TYPE_NORMAL;	// メッセージタイプ
	g_titleOther.state = STATE_NONE;	// 状態

	for (int nCntClear = 0; nCntClear < MAX_CLEAR; nCntClear++)
	{// 情報の初期化
		// 位置
		g_aClear[nCntClear].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		// 移動量
		g_aClear[nCntClear].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		// 大きさ
		g_aClear[nCntClear].size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		// 色
		g_aClear[nCntClear].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		// 使用
		g_aClear[nCntClear].bUse = false;
		// タイプ
		g_aClear[nCntClear].type = TYPE_EMPTY;
	}

	// 帯の設定(画面中央)
	// 位置
	g_aClear[MESSAGE_BAND].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, CLEAR_NOR_DESTPOS_Y, 0.0f);
	// 大きさ
	g_aClear[MESSAGE_BAND].size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	// 色
	g_aClear[MESSAGE_BAND].col = CLEAR_NOR_BAND_COL;
	// 使用
	g_aClear[MESSAGE_BAND].bUse = true;

	// メッセージ上部の設定
	// 位置(左)
	g_aClear[MESSAGE_UP].pos = D3DXVECTOR3(-CLEAR_NOR_WIDTH, CLEAR_NOR_DESTPOS_Y - CLEAR_NOR_HEIGHT / 4,0.0f);
	// 大きさ
	g_aClear[MESSAGE_UP].size = D3DXVECTOR3(CLEAR_NOR_WIDTH, CLEAR_NOR_HEIGHT / 4, 0.0f);
	// タイプ
	g_aClear[MESSAGE_UP].type = TYPE_NORMAL;
	// メッセージ下部の設定
	// 位置(右)
	g_aClear[MESSAGE_DOWN].pos = D3DXVECTOR3(SCREEN_WIDTH + CLEAR_NOR_WIDTH, CLEAR_NOR_DESTPOS_Y + CLEAR_NOR_HEIGHT / 4, 0.0f);
	// 大きさ
	g_aClear[MESSAGE_DOWN].size = D3DXVECTOR3(CLEAR_NOR_WIDTH, CLEAR_NOR_HEIGHT / 4, 0.0f);
	// タイプ
	g_aClear[MESSAGE_DOWN].type = TYPE_NORMAL;

	for (int nCntComplete = MESSAGE_ONE,nCntEquality = 0; nCntComplete < MESSAGE_ONE + CLEAR_COMP_NUM; nCntComplete++, nCntEquality++)
	{// コンプリートメッセージの設定
		// 位置の設定
		g_aClear[nCntComplete].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
		// 横に文字を等間隔で配置
		g_aClear[nCntComplete].pos.x += -((CLEAR_COMP_SPACE / 2) * (CLEAR_COMP_NUM - 1) + (CLEAR_COMP_DESTSIZE / 2) * (CLEAR_COMP_NUM - 1)) + nCntEquality * (CLEAR_COMP_DESTSIZE + CLEAR_COMP_SPACE);
		// 大きさの設定
		g_aClear[nCntComplete].size = D3DXVECTOR3(CLEAR_COMP_INITSIZE / 2, CLEAR_COMP_INITSIZE / 2, 0.0f);
		// タイプ
		g_aClear[nCntComplete].type = TYPE_COMPLETE;
	}

	ClearMakeVtx();		// 頂点情報の作成
}

//=============================================================================
//クリアメッセージの終了処理
//=============================================================================
void UninitClear(void)
{
	// 頂点バッファの破棄
	if (g_titleOther.pVtxBuff != NULL)
	{
		g_titleOther.pVtxBuff->Release();
		g_titleOther.pVtxBuff = NULL;
	}
	// テクスチャの破棄
	for (int nCntTex = 0; nCntTex < MESSAGE_MAX; nCntTex++)
	{
		if (g_titleOther.pTexture[nCntTex] != NULL)
		{
			g_titleOther.pTexture[nCntTex]->Release();
			g_titleOther.pTexture[nCntTex] = NULL;
		}
	}
}

//=============================================================================
//クリアメッセージの更新処理
//=============================================================================
void UpdateClear(void)
{
	VERTEX_2D*pVtx;  //頂点情報へのポインタ

	if (g_titleOther.pVtxBuff != NULL)
	{
		if (g_titleOther.state == STATE_START)
		{
			ClearBand();				// 帯の処理

			if (g_titleOther.type == TYPE_NORMAL)
			{// ノーマルメッセージの処理
				NormalMessage();
			}
			else if (g_titleOther.type == TYPE_COMPLETE)
			{// コンプリートメッセージの処理
				CompleteMessage();
			}

			// 情報のリセット(複数回使用時)
			ClearReset();

			g_titleOther.nCntAll++;		// 共通カウンタの加算

			//頂点バッファをロックし、頂点データへのポインタを取得
			g_titleOther.pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			for (int nCntClear = 0; nCntClear < MAX_CLEAR; nCntClear++, pVtx += 4)
			{
				// 移動量の更新
				g_aClear[nCntClear].pos += g_aClear[nCntClear].move;

				// 頂点座標
				pVtx[0].pos = D3DXVECTOR3(-g_aClear[nCntClear].size.x, -g_aClear[nCntClear].size.y, 0.0f) + g_aClear[nCntClear].pos;
				pVtx[1].pos = D3DXVECTOR3(g_aClear[nCntClear].size.x, -g_aClear[nCntClear].size.y, 0.0f) + g_aClear[nCntClear].pos;
				pVtx[2].pos = D3DXVECTOR3(-g_aClear[nCntClear].size.x, g_aClear[nCntClear].size.y, 0.0f) + g_aClear[nCntClear].pos;
				pVtx[3].pos = D3DXVECTOR3(g_aClear[nCntClear].size.x, g_aClear[nCntClear].size.y, 0.0f) + g_aClear[nCntClear].pos;

				// 頂点カラー
				pVtx[0].col = g_aClear[nCntClear].col;
				pVtx[1].col = g_aClear[nCntClear].col;
				pVtx[2].col = g_aClear[nCntClear].col;
				pVtx[3].col = g_aClear[nCntClear].col;
			}

			//頂点バッファをアンロック
			g_titleOther.pVtxBuff->Unlock();
		}
	}
}

//=============================================================================
// 帯の処理
//=============================================================================
void ClearBand(void)
{
	if (g_titleOther.type == TYPE_NORMAL)
	{// ノーマルクリアの時
		// 出現
		if (g_titleOther.nCntAll > CLEAR_NOR_START &&
			g_titleOther.nCntAll < CLEAR_NOR_START + CLEAR_NOR_CONNECT + 60)
		{// 指定の時間になったら
			if (g_aClear[MESSAGE_BAND].size.y < CLEAR_NORBAND_HEIGHT / 2)
			{// 指定の高さまで太くする
				g_aClear[MESSAGE_BAND].size.y += 5.0f;
				// 幅を画面幅に
				g_aClear[MESSAGE_BAND].size.x = SCREEN_WIDTH;
			}
		}

		// 縮小
		if (g_titleOther.nCntAll > CLEAR_NOR_START + CLEAR_NOR_CONNECT + 60)
		{// 指定の時間になったら
			// 縮小させる
			g_aClear[MESSAGE_BAND].size.y -= 5.0f;
		}
	}
	else if (g_titleOther.type == TYPE_COMPLETE)
	{// コンプリートクリアの時
		if (g_titleOther.nCntDraw > CLEAR_COMP_CONNECT)
		{// 指定の時間になったら
			g_aClear[MESSAGE_BAND].size.y -= 7.0f;
		}
		else if (g_aClear[MESSAGE_BAND].size.x < SCREEN_WIDTH)
		{// 画面幅まで伸ばす
			g_aClear[MESSAGE_BAND].size.x += 35.0f;
			// 高さを細く設定
			g_aClear[MESSAGE_BAND].size.y = 3.0f;
		}
		else
		{// 画面幅になったら
			if (g_aClear[MESSAGE_BAND].size.y < 240.0f)
			{// 指定の高さまで高くする
				g_aClear[MESSAGE_BAND].size.y += 10.0f;
			}
		}
	}
}

//=============================================================================
// ノーマルメッセージの処理
//=============================================================================
void NormalMessage(void)
{
	if (!g_titleOther.bSwitch)
	{// 移動切替がfalseの時
		if (g_aClear[MESSAGE_UP].pos.x > SCREEN_WIDTH / 2)
		{// 上部が中央に来たら
			// それぞれを中央で停止
			g_aClear[MESSAGE_UP].pos.x = SCREEN_WIDTH / 2;
			g_aClear[MESSAGE_DOWN].pos.x = SCREEN_WIDTH / 2;
			// それぞれの移動量を0に
			g_aClear[MESSAGE_UP].move.x = 0.0f;
			g_aClear[MESSAGE_DOWN].move.x = 0.0f;
			// 移動切替をtrueに
			g_titleOther.bSwitch = true;

			// 音を出す
			PlaySound(SOUND_LABEL_SENTOU);
		}
		else
		{
			if (g_titleOther.nCntAll > CLEAR_NOR_START + 6)
			{// 開始時間になったら
				// 移動量を代入
				g_aClear[MESSAGE_UP].move.x = CLEAR_NOR_SPEED;
				g_aClear[MESSAGE_DOWN].move.x = -CLEAR_NOR_SPEED;
			}
		}
	}
	else
	{
		// 表示カウンタを加算
		g_titleOther.nCntDraw++;

		if (g_titleOther.nCntDraw > CLEAR_NOR_CONNECT)
		{// 指定時間表示したら
		 // 移動量を代入
			g_aClear[MESSAGE_UP].move.x = CLEAR_NOR_SPEED;
			g_aClear[MESSAGE_DOWN].move.x = -CLEAR_NOR_SPEED;

			if (g_aClear[MESSAGE_UP].pos.x > SCREEN_WIDTH + g_aClear[MESSAGE_UP].size.x / 2 &&
				g_aClear[MESSAGE_BAND].size.y <= 0.0f)
			{
				g_titleOther.state = STATE_RESET;
				StageStateSwat(false);
			}
		}
	}
}

//=============================================================================
// コンプリートメッセージの処理
//=============================================================================
void CompleteMessage(void)
{
	if (g_titleOther.nCntAll > CLEAR_COMP_START)
	{
		for (int nCntComp = MESSAGE_ONE; nCntComp < MESSAGE_ONE + CLEAR_COMP_NUM; nCntComp++)
		{
			if (g_titleOther.nCntAll % CLEAR_COMP_INTERVAL == 0)
			{// 指定の間隔になったら
				if (!g_aClear[nCntComp].bUse)
				{// 表示されていない文字を順に表示
					g_aClear[nCntComp].bUse = true;
					PlaySound(SOUND_LABEL_SEIATU);
					break;
				}
			}

			if (g_aClear[nCntComp].bUse)
			{// 使用されている
				if (g_aClear[nCntComp].size.x > CLEAR_COMP_DESTSIZE / 2)
				{// 目的の大きさより大きければ
					// 縮小させる
					g_aClear[nCntComp].size -= D3DXVECTOR3(CLEAR_COMP_REDUCT, CLEAR_COMP_REDUCT, 0.0f);
				}
			}

			if (g_titleOther.nCntDraw > CLEAR_COMP_CONNECT)
			{// 表示時間を過ぎたら
				// 縮小させていく
				g_aClear[nCntComp].size.y -= 5.0f;

				if (g_aClear[nCntComp].size.y <= 0.0f &&
					g_aClear[MESSAGE_BAND].size.y <= 0.0f)
				{// 文字、帯ともにみえなくなったら
					// 待機状態に
					g_titleOther.state = STATE_RESET;

					STAGENUM stage = GetNumState();
					if (stage <= STAGENUM_2)
					{
						SetLoad();
					}
					else if (stage == STAGENUM_3)
					{// クリア
						SetGameState(GAMESTATE_CLEAR);
					}
				}
			}
		}

		// 表示カウンタを加算
		g_titleOther.nCntDraw++;
	}
}

//=============================================================================
// メッセージの起動
//=============================================================================
void SetClearMessage(CLEARTYPE type)
{
	if (g_titleOther.state == STATE_NONE)
	{
		StageStateSwat(true);

		g_titleOther.type = type;			// メッセージタイプの設定
		g_titleOther.state = STATE_START;	// 処理開始

		if (type == TYPE_NORMAL)
		{
			for (int nCntClear = 0; nCntClear < MAX_CLEAR; nCntClear++)
			{
				if (g_aClear[nCntClear].type == type)
				{
					g_aClear[nCntClear].bUse = true;
				}
			}
		}
	}
}

//=============================================================================
// 情報のリセット
//=============================================================================
void ClearReset(void)
{
	if (g_titleOther.state == STATE_RESET)
	{
		// クリアメッセージの設定情報の初期化
		g_titleOther.nCntAll = 0;			// 共通カウンタ
		g_titleOther.nCntDraw = 0;			// 表示カウンタ
		g_titleOther.bSwitch = false;		// 移動の切替
		g_titleOther.type = TYPE_NORMAL;	// メッセージタイプ
		g_titleOther.state = STATE_NONE;	// 状態

		for (int nCntClear = 0; nCntClear < MAX_CLEAR; nCntClear++)
		{// 情報の初期化
		 // 位置
			g_aClear[nCntClear].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			// 移動量
			g_aClear[nCntClear].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			// 大きさ
			g_aClear[nCntClear].size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			// 色
			g_aClear[nCntClear].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			// 使用
			g_aClear[nCntClear].bUse = false;
			// タイプ
			g_aClear[nCntClear].type = TYPE_EMPTY;
		}

		// 帯の設定(画面中央)
		// 位置
		g_aClear[MESSAGE_BAND].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, CLEAR_NOR_DESTPOS_Y, 0.0f);
		// 大きさ
		g_aClear[MESSAGE_BAND].size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		// 色
		g_aClear[MESSAGE_BAND].col = CLEAR_NOR_BAND_COL;
		// 使用
		g_aClear[MESSAGE_BAND].bUse = true;

		// メッセージ上部の設定
		// 位置(左)
		g_aClear[MESSAGE_UP].pos = D3DXVECTOR3(-CLEAR_NOR_WIDTH, CLEAR_NOR_DESTPOS_Y - CLEAR_NOR_HEIGHT / 4, 0.0f);
		// 大きさ
		g_aClear[MESSAGE_UP].size = D3DXVECTOR3(CLEAR_NOR_WIDTH, CLEAR_NOR_HEIGHT / 4, 0.0f);
		// タイプ
		g_aClear[MESSAGE_UP].type = TYPE_NORMAL;
		// メッセージ下部の設定
		// 位置(右)
		g_aClear[MESSAGE_DOWN].pos = D3DXVECTOR3(SCREEN_WIDTH + CLEAR_NOR_WIDTH, CLEAR_NOR_DESTPOS_Y + CLEAR_NOR_HEIGHT / 4, 0.0f);
		// 大きさ
		g_aClear[MESSAGE_DOWN].size = D3DXVECTOR3(CLEAR_NOR_WIDTH, CLEAR_NOR_HEIGHT / 4, 0.0f);
		// タイプ
		g_aClear[MESSAGE_DOWN].type = TYPE_NORMAL;

		for (int nCntComplete = MESSAGE_ONE, nCntEquality = 0; nCntComplete < MESSAGE_ONE + CLEAR_COMP_NUM; nCntComplete++, nCntEquality++)
		{// コンプリートメッセージの設定
		 // 位置の設定
			g_aClear[nCntComplete].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
			// 横に文字を等間隔で配置
			g_aClear[nCntComplete].pos.x += -((CLEAR_COMP_SPACE / 2) * (CLEAR_COMP_NUM - 1) + (CLEAR_COMP_DESTSIZE / 2) * (CLEAR_COMP_NUM - 1)) + nCntEquality * (CLEAR_COMP_DESTSIZE + CLEAR_COMP_SPACE);
			// 大きさの設定
			g_aClear[nCntComplete].size = D3DXVECTOR3(CLEAR_COMP_INITSIZE / 2, CLEAR_COMP_INITSIZE / 2, 0.0f);
			// タイプ
			g_aClear[nCntComplete].type = TYPE_COMPLETE;
		}
	}
}

//=============================================================================
//クリアメッセージの描画処理
//=============================================================================
void DrawClear(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_titleOther.pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntClear = 0; nCntClear < MAX_CLEAR; nCntClear++)
	{
		if (g_aClear[nCntClear].bUse)
		{// 使用されている時
			// テクスチャの設定
			if (nCntClear != MESSAGE_BAND)
			{// 帯以外のみ
				pDevice->SetTexture(0, g_titleOther.pTexture[nCntClear]);
			}
			else
			{// 帯
				pDevice->SetTexture(0, NULL);
			}

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntClear, 2);
		}
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void ClearMakeVtx(void)
{
	VERTEX_2D*pVtx;								// 頂点情報へのポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	if (g_titleOther.pVtxBuff == NULL)
	{
		// テクスチャの読み込み
		// ノーマルクリア(どっちも同じテクスチャです。ちょっとだるくてこうなりました)
		// 上
		D3DXCreateTextureFromFile(pDevice,
			CLEAR_NOR_TEX,
			&g_titleOther.pTexture[MESSAGE_UP]);
		// 下
		D3DXCreateTextureFromFile(pDevice,
			CLEAR_NOR_TEX,
			&g_titleOther.pTexture[MESSAGE_DOWN]);
		// コンプリートクリア
		// 1文字目
		D3DXCreateTextureFromFile(pDevice,
			CLEAR_COMP_TEX_0,
			&g_titleOther.pTexture[MESSAGE_ONE]);
		// 2文字目
		D3DXCreateTextureFromFile(pDevice,
			CLEAR_COMP_TEX_1,
			&g_titleOther.pTexture[MESSAGE_TWO]);
		// 3文字目
		D3DXCreateTextureFromFile(pDevice,
			CLEAR_COMP_TEX_2,
			&g_titleOther.pTexture[MESSAGE_THREE]);
		// 4文字目
		D3DXCreateTextureFromFile(pDevice,
			CLEAR_COMP_TEX_3,
			&g_titleOther.pTexture[MESSAGE_FOUR]);

		// 頂点バッファの生成
		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_CLEAR,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&g_titleOther.pVtxBuff,
			NULL);

		//頂点バッファをロックして頂点データへのポインタを取得
		g_titleOther.pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点情報の設定
		for (int nCntClear = 0; nCntClear < MAX_CLEAR; nCntClear++, pVtx += 4)
		{
			// 頂点座標
			pVtx[0].pos = D3DXVECTOR3(-g_aClear[nCntClear].size.x, -g_aClear[nCntClear].size.y, 0.0f) + g_aClear[nCntClear].pos;
			pVtx[1].pos = D3DXVECTOR3(g_aClear[nCntClear].size.x, -g_aClear[nCntClear].size.y, 0.0f) + g_aClear[nCntClear].pos;
			pVtx[2].pos = D3DXVECTOR3(-g_aClear[nCntClear].size.x, g_aClear[nCntClear].size.y, 0.0f) + g_aClear[nCntClear].pos;
			pVtx[3].pos = D3DXVECTOR3(g_aClear[nCntClear].size.x, g_aClear[nCntClear].size.y, 0.0f) + g_aClear[nCntClear].pos;

			// 頂点カラー
			pVtx[0].col = g_aClear[nCntClear].col;
			pVtx[1].col = g_aClear[nCntClear].col;
			pVtx[2].col = g_aClear[nCntClear].col;
			pVtx[3].col = g_aClear[nCntClear].col;

			// rhw
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			// テクスチャ座標
			if (nCntClear == MESSAGE_UP ||
				nCntClear == MESSAGE_DOWN)
			{// ノーマルメッセージの時
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.5f * nCntClear - 0.5f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.5f * nCntClear - 0.5f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, (float)nCntClear / 2);
				pVtx[3].tex = D3DXVECTOR2(1.0f, (float)nCntClear / 2);
			}
			else
			{// その他
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			}
		}

		//頂点バッファをアンロック
		g_titleOther.pVtxBuff->Unlock();
	}
}

