//=============================================================================
//
// パーティクル処理 [particle.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "particle.h"
#include "renderer.h"
#include "manager.h"

#include <stdio.h>

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PARTICLE_FILENAME	"data/TEXT/particle.txt"				// パーティクルデータ
#define TEXTURE_FILENAME	"data/TEXT/particle_texture.txt"		// テクスチャ名

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9 CParticlePolygon::m_pTexture[MAX_TEXTURE] = {};
CParData CParData::m_apParticleData[MAX_EMITTER_TYPE] = {};

//=========================================
// コンストラクタ
//=========================================
CParticle::CParticle(int nPriority) : CScene(nPriority)
{
	m_nType = 0;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=========================================
// デストラクタ
//=========================================
CParticle::~CParticle()
{
}

//==================================
// 生成処理
//==================================
CParticle *CParticle::Create(D3DXVECTOR3 pos, int nType)
{
	CParticle *pParticle;

	pParticle = new CParEmitter;	// メモリを確保
	pParticle->m_pos = pos;
	pParticle->m_nType = nType;
	pParticle->Init();

	return pParticle;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CParticle::Init(void)
{
	// オブジェクトタイプの設定
	CScene::SetObjType(CScene::OBJTYPE_PARTICLE);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CParticle::Uninit(void)
{
	Release();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//		パーティクルのエミッタ
//
//=========================================
// コンストラクタ
//=========================================
CParEmitter::CParEmitter()
{
	m_nCntParticle = 0;
	m_nAppear = 0;
	m_nCntAngle = 0;

	m_nLife = 0;
	m_nType = 0;
	m_nAppear = 0;
	m_nLap = 0;
	m_bLoop = false;
}

//=========================================
// デストラクタ
//=========================================
CParEmitter::~CParEmitter()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CParEmitter::Init(void)
{
	CParticle::Init();
	Set(m_pos, m_nType);	// エミッタの設定

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CParEmitter::Uninit(void)
{
	CParticle::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CParEmitter::Update(void)
{
	bool bDelete = false;
	m_nLife--;	// エミッタの表示時間をカウント

	if (m_nLife <= 0 && m_bLoop == false)
	{// 一定時間経過でエミッタを消す
		bDelete = true;
	}

	// 出現させる間隔のカウント
	m_nCntParticle = (m_nCntParticle + 1) % m_nAppear;

	if (m_nCntParticle == 0)
	{// 出現させる
		CParticlePolygon::Create(m_pos, m_nType, m_nTexture);

		m_nCntAngle = (m_nCntAngle + 1) % m_nLap;
	}

	if (true == bDelete)
	{
		Uninit();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CParEmitter::Draw(void)
{
}

//=============================================================================
// エミッタの設定
//=============================================================================
void CParEmitter::Set(D3DXVECTOR3 pos, int nType)
{
	m_pos = pos;
	m_nType = nType;
	m_nAppear = CParData::GetAppear(m_nType);
	m_nLife = CParData::GetLife(m_nType);
	m_nTexture = CParData::GetTextureIdx(m_nType);
	m_nLap = CParData::GetLap(m_nType);
}

//=============================================================================
// エミッタの位置の設定
//=============================================================================
void CParEmitter::SetPosParticleEmitter(int nIdx, D3DXVECTOR3 pos)
{
	// 位置を更新
	m_pos = pos;
}

//=============================================================================
// エミッタの削除
//=============================================================================
void CParEmitter::DestroyParticle(int nIdx)
{
	// 消す
	CParEmitter::Uninit();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//		パーティクルのポリゴン
//
//==================================
// 生成処理
//==================================
CParticlePolygon *CParticlePolygon::Create(D3DXVECTOR3 pos, int nType, int nTexture)
{
	CParticlePolygon *pParticlePlygon;

	pParticlePlygon = new CParticlePolygon;	// メモリを確保
	pParticlePlygon->m_nType = nType;
	pParticlePlygon->Init(pos);
	pParticlePlygon->BindTexture(m_pTexture[nTexture]);
	return pParticlePlygon;
}

//=========================================
// テクスチャの読み込み
//=========================================
HRESULT CParticlePolygon::Load(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	FILE *pFile;
	char aFileName[MAX_TEXTURE][64];		// テクスチャの名前
	int nCntTexture;
	int nNumTexture;

	pFile = fopen(TEXTURE_FILENAME, "r");

	if (NULL != pFile)
	{// 開けた
		fscanf(pFile, "%d", &nNumTexture);		// テクスチャ数

		for (nCntTexture = 0; nCntTexture < nNumTexture; nCntTexture++)
		{
			fscanf(pFile, "%s\n", &aFileName[nCntTexture][0]);		// テクスチャ数
		}
		fclose(pFile);
	}
	else
	{// 失敗
		MessageBox(0, "ファイルが開けませんでした", "texture.txt", MB_OK);
	}

	// テクスチャの読み込み
	for (nCntTexture = 0; nCntTexture < nNumTexture; nCntTexture++)
	{
		D3DXCreateTextureFromFile(pDevice, &aFileName[nCntTexture][0], &m_pTexture[nCntTexture]);
	}

	return S_OK;
}

//=========================================
// 読み込んだテクスチャの解放
//=========================================
void CParticlePolygon::Unload(void)
{
	// テクスチャの破棄
	for (int nCntPolygon = 0; nCntPolygon < TYPE_MAX; nCntPolygon++)
	{
		if (NULL != m_pTexture[nCntPolygon])
		{
			m_pTexture[nCntPolygon]->Release();
			m_pTexture[nCntPolygon] = NULL;
		}
	}
}

//=========================================
// コンストラクタ
//=========================================
CParticlePolygon::CParticlePolygon(int nPriority) : CScene3DBill(nPriority)
{
}

//=========================================
// デストラクタ
//=========================================
CParticlePolygon::~CParticlePolygon()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CParticlePolygon::Init(D3DXVECTOR3 pos)
{
	// オブジェクトタイプの設定
	CScene::SetObjType(CScene::OBJTYPE_PARTICLE);

	// 初期化処理
	CScene3DBill::Init();
	CScene3DBill::SetPos(pos);

	int nLap = CParData::GetLap(m_nType);				// 一周の数
	int *nLife = CParData::GetParLife(m_nType);
	int *nRadius = CParData::GetRadius(m_nType);
	int *nWidth = CParData::GetWidth(m_nType);
	int *nHeight = CParData::GetHeight(m_nType);
	int *nDepth = CParData::GetDepth(m_nType);
	float fSpread = CParData::GetSpread(m_nType);
	D3DXCOLOR col = CParData::GetColor(m_nType);
	bool bDraw = CParData::GetDraw(m_nType);
	float fGravity = CParData::GetGravity(m_nType);

	m_fAngle = 1.0f / (nLap / 2);
	if (0 != nHeight[1] && 0 != nHeight[0])
	{// Y軸移動量が両方０以外
		m_move = D3DXVECTOR3(sinf(D3DX_PI * (0.0f + (m_fAngle * m_nCntAngle))) * ((nWidth[0] + (rand() % (1 + nWidth[1]))) - ((rand() % 10) * 0.1f)) * fSpread,
			((rand() % nHeight[1]) + (rand() % nHeight[0])) * fSpread,
			cosf(D3DX_PI * (0.0f + (m_fAngle * m_nCntAngle))) * ((nDepth[0] + (rand() % (1 + nDepth[1]))) - ((rand() % 10) * 0.1f)) * fSpread);
	}
	else
	{// Y軸の移動量が０がある場合
		m_move = D3DXVECTOR3(sinf(D3DX_PI * (0.0f + (m_fAngle * m_nCntAngle))) * ((nWidth[0] + (rand() % (1 + nWidth[1]))) - ((rand() % 10) * 0.1f)) * fSpread,
			(nHeight[1] + nHeight[0]) * fSpread,
			cosf(D3DX_PI * (0.0f + (m_fAngle * m_nCntAngle))) * ((nDepth[0] + (rand() % (1 + nDepth[1]))) - ((rand() % 10) * 0.1f)) * fSpread);
	}
	m_col = col;
	CScene3DBill::SetColor(m_col);
	m_fRadius = nRadius[0] + (rand() % (1 + (nRadius[1] - nRadius[0])) - 1) + ((rand() % 10) * 0.1f);
	m_nLife = nLife[0] + (rand() % (1 + (nLife[1] - nLife[0])) - 1);
	m_fGravity = fGravity;
	m_bDraw = bDraw;

	// 頂点の角度
	m_fRotAngle = atan2f(pos.x - (pos.x + m_fRadius), pos.y - (pos.y + m_fRadius));

	// 中心から頂点までの距離
	m_fLength = sqrtf((m_fRadius * m_fRadius) + (m_fRadius * m_fRadius));

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CParticlePolygon::Uninit(void)
{
	CScene3DBill::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CParticlePolygon::Update(void)
{
	m_nLife--;		// 表示時間をカウント

	if (m_nLife <= 0)
	{// 表示時間が0以下になった
		Uninit();	// 終了処理
	}
	else
	{
		D3DXVECTOR3 pos = CScene3DBill::GetPos();
		D3DXCOLOR colFlame = CParData::GetFlameColor(m_nType);
		float fFlameRadius = CParData::GetFlameRadius(m_nType);
		bool bBouncing = CParData::GetBouncing(m_nType);
		int nRotType = CParData::GetRotType(m_nType);
		float fRotSpeed = CParData::GetRotSpeed(m_nType);
		m_move.y -= m_fGravity;		// 重力を加える

		m_fBouncing = pos.y;		// 値を記憶

		pos += m_move;				// 位置の更新
		m_col += colFlame;			// 色の更新
		m_fRadius += fFlameRadius;	// 大きさの更新

		if (pos.y <= 0 && bBouncing == true)
		{// バウンド
			m_move.y += (m_fBouncing - pos.y) * 2.0f;
		}

		if (nRotType == 1)
		{// 時計回り
			m_fCntRot -= fRotSpeed;			// 回転のカウンタ
		}
		else if (nRotType == 2)
		{// 反時計回り
			m_fCntRot += fRotSpeed;			// 回転のカウンタ
		}
		else if (nRotType == 3)
		{// ランダム
			if (rand() % 2 == 0)
			{// 時計回り
				m_fCntRot += fRotSpeed;		// 回転のカウンタ
			}
			else
			{// 反時計回り
				m_fCntRot -= fRotSpeed;		// 回転のカウンタ
			}
		}

		D3DXVECTOR3 vtxPos[4];

		//頂点座標の設定
		vtxPos[0].x = sinf(m_fRotAngle + m_fCntRot) * -(m_fLength + m_fRadius);
		vtxPos[0].y = cosf(m_fRotAngle + m_fCntRot) * (m_fLength + m_fRadius);
		vtxPos[0].z = 0.0f;
		vtxPos[1].x = sinf(m_fRotAngle - m_fCntRot) * (m_fLength + m_fRadius);
		vtxPos[1].y = cosf(m_fRotAngle - m_fCntRot) * (m_fLength + m_fRadius);
		vtxPos[1].z = 0.0f;
		vtxPos[2].x = sinf(m_fRotAngle - m_fCntRot) * -(m_fLength + m_fRadius);
		vtxPos[2].y = cosf(m_fRotAngle - m_fCntRot) * -(m_fLength + m_fRadius);
		vtxPos[2].z = 0.0f;
		vtxPos[3].x = sinf(m_fRotAngle + m_fCntRot) * (m_fLength + m_fRadius);
		vtxPos[3].y = cosf(m_fRotAngle + m_fCntRot) * -(m_fLength + m_fRadius);
		vtxPos[3].z = 0.0f;

		//頂点カラー
		CScene3DBill::SetColor(m_col);

		CScene3DBill::SetPos(pos);

		CScene3DBill::SetVtxPos(&vtxPos[0]);
	}
}

//=============================================================================
// ポリゴン描画処理
//=============================================================================
void CParticlePolygon::Draw(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// 被らないようにする
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	if (m_bDraw == true)
	{ // αブレンディングを加算合成に設定
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}
	else
	{// αブレンディングを元に戻す
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}

	// ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	CScene3DBill::Draw();

	// ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// Zバッファを元に戻す
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//		パーティクルのデータ
//
//=========================================
// コンストラクタ
//=========================================
CParData::CParData()
{
	for (int nCntParticle = 0; nCntParticle < 2; nCntParticle++)
	{
		m_nWidth[nCntParticle] = 0;
		m_nHeight[nCntParticle] = 0;
		m_nDepth[nCntParticle] = 0;
		m_nRadius[nCntParticle] = 0;
		m_nParLife[nCntParticle] = 0;
	}

	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_flameCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_fFlameRadius = 0.0f;
	m_nLife = 0;
	m_nTexture = 0;
	m_fSpread = 0.0f;
	m_nAppear = 0;
	m_fGravity = 0.0f;
	m_nLap = 0;
	m_nRot = 0;
	m_fRotSpeed = 0.0f;
	m_bLoop = false;
	m_bBouncing = false;
	m_bDraw = false;
}

//=========================================
// デストラクタ
//=========================================
CParData::~CParData()
{
}

//=============================================================================
// ファイルからデータを取得
//=============================================================================
HRESULT CParData::Load(void)
{
	FILE *pFile;
	int nCntEffect;

	pFile = fopen(PARTICLE_FILENAME, "r");

	if (NULL != pFile)
	{
		for (nCntEffect = 0; nCntEffect < MAX_EMITTER_TYPE; nCntEffect++)
		{// データ分まわす
			fscanf(pFile, "%d %d %d", &m_apParticleData[nCntEffect].m_nWidth[1], &m_apParticleData[nCntEffect].m_nHeight[1], &m_apParticleData[nCntEffect].m_nDepth[1]);	// 移動量の最大
			fscanf(pFile, "%d %d %d", &m_apParticleData[nCntEffect].m_nWidth[0], &m_apParticleData[nCntEffect].m_nHeight[0], &m_apParticleData[nCntEffect].m_nDepth[0]);	// 移動量の最小
			fscanf(pFile, "%f %f %f %f", &m_apParticleData[nCntEffect].m_col.r, &m_apParticleData[nCntEffect].m_col.g, &m_apParticleData[nCntEffect].m_col.b, &m_apParticleData[nCntEffect].m_col.a);	// 色
			fscanf(pFile, "%f %f %f %f", &m_apParticleData[nCntEffect].m_flameCol.r, &m_apParticleData[nCntEffect].m_flameCol.g, &m_apParticleData[nCntEffect].m_flameCol.b, &m_apParticleData[nCntEffect].m_flameCol.a);	// 色
			fscanf(pFile, "%d %d", &m_apParticleData[nCntEffect].m_nRadius[0], &m_apParticleData[nCntEffect].m_nRadius[1]);			// サイズ
			fscanf(pFile, "%f", &m_apParticleData[nCntEffect].m_fFlameRadius);	// サイズの加算
			fscanf(pFile, "%d", &m_apParticleData[nCntEffect].m_nLife);		// 表示時間(寿命)
			fscanf(pFile, "%d %d", &m_apParticleData[nCntEffect].m_nParLife[0], &m_apParticleData[nCntEffect].m_nParLife[1]);		// パーティクルの表示時間
			fscanf(pFile, "%d", &m_apParticleData[nCntEffect].m_nAppear);		// 出る間隔
			fscanf(pFile, "%f", &m_apParticleData[nCntEffect].m_fSpread);		// 広がり
			fscanf(pFile, "%d", &m_apParticleData[nCntEffect].m_nLap);			// 1周に出す個数
			fscanf(pFile, "%f", &m_apParticleData[nCntEffect].m_fGravity);		// 重力
			fscanf(pFile, "%d", &m_apParticleData[nCntEffect].m_nRot);			// 回転のパターン
			fscanf(pFile, "%f", &m_apParticleData[nCntEffect].m_fRotSpeed);	// 回転の速度
			fscanf(pFile, "%d", (int*)&m_apParticleData[nCntEffect].m_bLoop);		// ループするかどうか
			fscanf(pFile, "%d", (int*)&m_apParticleData[nCntEffect].m_bBouncing);	// バウンドするかどうか
			fscanf(pFile, "%d", &m_apParticleData[nCntEffect].m_nTexture);		// テクスチャの種類
			fscanf(pFile, "%d", (int*)&m_apParticleData[nCntEffect].m_bDraw);		// 加算合成の有無
		}

		fclose(pFile);		// ファイルを閉じる
	}
	else
	{// 失敗
		MessageBox(0, "ファイルが開けませんでした", "particle.txt", MB_OK);
	}

	return S_OK;
}

//=============================================================================
// データの開放
//=============================================================================
void CParData::Unload(void)
{
	for (int nCntParticle = 0; nCntParticle < MAX_EMITTER_TYPE; nCntParticle++)
	{// データ分まわす
		for (int nCntParticle = 0; nCntParticle < 2; nCntParticle++)
		{
			m_apParticleData[nCntParticle].m_nWidth[nCntParticle] = 0;
			m_apParticleData[nCntParticle].m_nHeight[nCntParticle] = 0;
			m_apParticleData[nCntParticle].m_nDepth[nCntParticle] = 0;
			m_apParticleData[nCntParticle].m_nRadius[nCntParticle] = 0;
			m_apParticleData[nCntParticle].m_nParLife[nCntParticle] = 0;
		}

		m_apParticleData[nCntParticle].m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		m_apParticleData[nCntParticle].m_flameCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		m_apParticleData[nCntParticle].m_fFlameRadius = 0.0f;
		m_apParticleData[nCntParticle].m_nLife = 0;
		m_apParticleData[nCntParticle].m_nTexture = 0;
		m_apParticleData[nCntParticle].m_fSpread = 0.0f;
		m_apParticleData[nCntParticle].m_nAppear = 0;
		m_apParticleData[nCntParticle].m_fGravity = 0.0f;
		m_apParticleData[nCntParticle].m_nLap = 0;
		m_apParticleData[nCntParticle].m_nRot = 0;
		m_apParticleData[nCntParticle].m_fRotSpeed = 0.0f;
		m_apParticleData[nCntParticle].m_bLoop = false;
		m_apParticleData[nCntParticle].m_bBouncing = false;
		m_apParticleData[nCntParticle].m_bDraw = false;
	}
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CParData::Init(void)
{
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CParData::Uninit(void)
{
}
