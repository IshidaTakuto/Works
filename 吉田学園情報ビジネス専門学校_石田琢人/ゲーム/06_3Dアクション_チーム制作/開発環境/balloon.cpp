//=============================================================================
//
// 吹き出しの処理 [balloon.cpp]
// Author : Atsushi Yamashita
//
//=============================================================================
#include "balloon.h"
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "sceneX.h"
#include "fade.h"
#include "debuglog.h"
//============================================================================
//	静的メンバ変数宣言
//============================================================================
LPDIRECT3DTEXTURE9 CBalloon::m_pTexture[MAX_BALLOON_TEXTURE] = {};
//int CBalloon::m_nType = 0;
//============================================================================
//	マクロ定義
//============================================================================

#define BALLOON_TEXTURENAME_0 "data/TEXTURE/GAME/STAGE1.png"
#define BALLOON_TEXTURENAME_1 "data/TEXTURE/GAME/STAGE2.png"
#define BALLOON_TEXTURENAME_2 "data/TEXTURE/GAME/STAGE3.png"
#define BALLOON_TEXTURENAME_3 "data/TEXTURE/GAME/STAGE4.png"
#define BALLOON_TEXTURENAME_4 "data/TEXTURE/GAME/STAGE5.png"

//=============================================================================
//	コンストラクタ
//=============================================================================
CBalloon::CBalloon(int nPriority, OBJTYPE objType) : CScene2DSquare(nPriority, objType)
{
	m_pVtxBuff = NULL;						// 頂点バッファへのポインタ
	m_nCounter = 0;

	m_pos = D3DXVECTOR3(0, 0, 0);			// 吹き出しの位置
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f,0.0f);			// 色

}

//=============================================================================
//デストラクタ
//=============================================================================
CBalloon::~CBalloon()
{

}

//=============================================================================
// テクスチャの読み込み処理
//=============================================================================
HRESULT CBalloon::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	CManager Manager;
	pDevice = Manager.GetRenderer()->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, BALLOON_TEXTURENAME_0, &m_pTexture[0]);
	D3DXCreateTextureFromFile(pDevice, BALLOON_TEXTURENAME_1, &m_pTexture[1]);
	D3DXCreateTextureFromFile(pDevice, BALLOON_TEXTURENAME_2, &m_pTexture[2]);
	D3DXCreateTextureFromFile(pDevice, BALLOON_TEXTURENAME_3, &m_pTexture[3]);
	D3DXCreateTextureFromFile(pDevice, BALLOON_TEXTURENAME_4, &m_pTexture[4]);

	return S_OK;
}

//=============================================================================
// テクスチャの開放処理
//=============================================================================
void CBalloon::Unload(void)
{
	for (int nCntTex = 0; nCntTex < MAX_BALLOON_TEXTURE; nCntTex++)
	{
		//テクスチャの破棄
		if (m_pTexture[nCntTex] != NULL)
		{
			m_pTexture[nCntTex]->Release();
			m_pTexture[nCntTex] = NULL;
		}
	}
}

//=============================================================================
// 吹き出しの初期化処理
//=============================================================================
HRESULT CBalloon::Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスを取得
	CManager Manager;
	pDevice = Manager.GetRenderer()->GetDevice();

	//テクスチャの貼り付け
	BindTexture(m_pTexture[m_nTexNumber]);

	//吹き出しの初期化処理
	CScene2DSquare::Init(pos, col,fWidth,fHeight);

	return S_OK;
}

//=============================================================================
// 吹き出しの終了処理
//=============================================================================
void CBalloon::Uninit(void)
{
	//吹き出しの終了処理
	CScene2DSquare::Uninit();
}

//=============================================================================
// 吹き出しの更新処理
//=============================================================================
void CBalloon::Update(void)
{
	float fHeight = CScene2DSquare::GetHeight();
	float fWidth = CScene2DSquare::GetWidth();
	D3DXVECTOR3 BalloonPos = CScene2D::GetPos();

	//吹き出しの色を取得
	m_col = CScene2D::GetCol();

	if (m_nType == 0)
	{//吹き出しを拡大
		//fHeight += 1.0f;	//高さの加算
		//fWidth += 1.0f;		//幅の加
		m_col.a += 0.02f;	//透明度の加算

		//if (fHeight >= 70.0f && fWidth >= 70.0f)
		//{//大きさが70になったら止める
		//	fHeight = 70.0f;
		//	fWidth = 70.0f;
		//}
		if (m_col.a >= 1.0f)
		{//透明度が1.0f以上になったら1.0fで止める
			m_col.a = 1.0f;
		}

	}
	else if (m_nType == 1)
	{//吹き出しの縮小
		//fHeight -= 1.0f;	//高さの減算
		//fWidth -= 1.0f;		//幅の減算
		m_col.a -= 0.02f;	//透明度の減算

		//if (fHeight <= 0.0f && fWidth <= 0.0f)
		//{//大きさが0になったら止める
		//	fHeight = 0.0f;
		//	fWidth = 0.0f;
		//}
		if (m_col.a <= 0.0f)
		{//透明度が0.0f以下になったら0.0fで止める
			m_col.a = 0.0f;
		}
	}

	//高さの設定
	CScene2DSquare::SetHeight(fHeight);

	//幅の設定
	CScene2DSquare::SetWidth(fWidth);

	//位置の設定
	CScene2DSquare::SetVtxBuffPos(BalloonPos);

	//色の設定
	CScene2DSquare::SetVtxBuffCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_col.a));
}

//=============================================================================
//吹き出しの描画処理
//=============================================================================
void CBalloon::Draw(void)
{
	//吹き出しの描画処理
	CScene2DSquare::Draw();
}

//=============================================================================
//吹き出しの生成処理
//=============================================================================
CBalloon *CBalloon::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight ,int nTexNumber)
{
	CBalloon *pBalloon = {};

	if (pBalloon == NULL)
	{
		//メモリを動的確保
		pBalloon = new CBalloon;

		if (pBalloon != NULL)
		{//NULLチェック
			//テクスチャの番号を代入
			pBalloon->m_nTexNumber = nTexNumber;

			//初期化処理
			pBalloon->Init(pos,col,fWidth,fHeight);
		}
		else
		{
			MessageBox(0, "BalloonがNULLでした", "警告", MB_OK);
		}
	}
	else
	{
		MessageBox(0, "BalloonがNULLじゃないです", "警告", MB_OK);
	}
	return pBalloon;
}

//=============================================================================
//吹き出しの取得
//=============================================================================
void CBalloon::SetType(int nType)
{
	m_nType = nType;
}