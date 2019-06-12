//*****************************************************************************
//
//     パーティクルの処理[particle.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "particle.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
// 値読み込みの目印となるパス名
#define NUM_EMITTER     "NUM_EMITTER"       // エミッターの種類
#define PARTICLESET     "PARTICLESET"       // パーティクルセット読み込み開始の合図
#define END_PARTICLESET "END_PARTICLESET"   // パーティクルセット読み込み終了の合図

#define MAXMOVE         "MAXMOVE"           // 移動量の最大値
#define MINMOVE         "MINMOVE"           // 移動量の最小値
#define INITCOL         "INITCOL"           // 生成時の色
#define FLAMECOL        "FLAMECOL"          // 1フレームに加算する色の変化量
#define INITSIZE        "INITSIZE"          // 生成時のサイズ(左が最小値、右が最大値)
#define FLAMESIZE       "FLAMESIZE"         // 1フレームに加算するサイズ
#define EMITTERLIFE     "EMITTERLIFE"       // エミッタの寿命
#define PARLIFE         "PARLIFE"           // パーティクルの寿命(左が最小値、右が最大値)
#define APPEAR          "APPEAR"            // パーティクルを生成する間隔
#define SPREAD          "SPREAD"            // エミッタの拡散値
#define LAP             "LAP"               // 1回の生成タイミングでパーティクルを生成する個数
#define GRAVITY         "GRAVITY"           // パーティクルにかける重力
#define ROTPATTERN      "ROTPATTERN"        // 回転の種類( 0:なし　1:時計回り　2:反時計回り 3:ランダム )
#define ROTSPEED        "ROTSPEED"          // パーティクルにかける重力
#define LOOP            "LOOP"              // ループするかしないか(0ならループしない / 1ならループする)
#define BOUND           "BOUND"             // バウンドするかしないか(0ならバウンドしない / 1ならバウンドする)
#define TEXTURE         "TEXTURE"           // 使用するテクスチャの番号
#define DRAW            "DRAW"              // 加算合成で描画するかしないか(0なら加算合成なし / 1なら加算合成あり)

//*****************************************************************************
//    静的メンバ変数
//*****************************************************************************

//*****************************************************************************
//    CParticleDataの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CParticleData::CParticleData()
{
	// 各種値のクリア
	m_MaxMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);         // 移動量の最大値
	m_MinMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);         // 移動量の最小値
	m_InitCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);     // 初期値の色
	m_ChangeCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);   // 毎フレーム加算する色の変化量
	m_fMinRadius = 0.0f;                               // 生成時の半径最小値
	m_fMaxRadius = 0.0f;                               // 生成時の半径最大値
	m_fChangeRadius = 0.0f;                            // 毎フレーム加算するポリゴンの半径
	m_fRotSpeed = 0.0f;                                // 毎フレーム加算するポリゴンの回転量
	m_fSpread = 0.0f;                                  // 毎フレーム加算する広がり
	m_fGravity = 0.0f;                                 // 毎フレーム加算する重力
    m_nLap = 0;                                        // １フレームに生成するポリゴン数
	m_nLife = 0;			                           // エミッタの寿命
	m_nAppear = 0;                                     // パーティクルを生成する間隔
	m_nMinLife = 0;		                               // 表示時間の最小値
	m_nMaxLife = 0;		                               // 表示時間の最大値
	m_nTexIdx = 0;                                     // 使用するテクスチャの番号
	m_nRotPattern = 0;                                 // 回転の種類分け用[ 0:なし　1:時計回り　2:反時計回り 3:ランダム ]
	m_bLoop = false;                                   // ループするかしないか[ 0ならループしない / 1ならループする ]
    m_bBouncing = false;		                       // バウンドさせるか[ 0:なし 1:あり ]
	m_bDrawAddtive = false;	                           // 加算合成かどうか[ 0:なし 1:あり ]
}

//=============================================================================
//    デストラクタ
//=============================================================================
CParticleData::~CParticleData()
{

}

//*****************************************************************************
//    CParticleManagerの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CParticleManager::CParticleManager(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// 各種値のクリア
	m_pParticleData = NULL;  // パーティクルデータへのポインタ
	m_pTexture = NULL;       // テクスチャへのポインタ
	m_nNumEmitter = 0;       // パーティクルの種類
	m_nNumTexture = 0;       // 読み込んだテクスチャの数
}

//=============================================================================
//    デストラクタ
//=============================================================================
CParticleManager::~CParticleManager()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CParticleManager *CParticleManager::Create(char *pParticleFileName, char *pTextureFileName)
{
	CParticleManager *pParticleManager = NULL;  // パーティクル管理クラス型のポインタ
	if (pParticleManager == NULL)
	{// メモリが空になっている
		pParticleManager = new CParticleManager;
		if (pParticleManager != NULL)
		{// インスタンスを生成できた
			// 各種値の設定
			strcpy(pParticleManager->m_aParticleFileName, pParticleFileName);
			strcpy(pParticleManager->m_aTextureFileName, pTextureFileName);

			if (FAILED(pParticleManager->Init()))
			{// 初期化に失敗した
				return NULL;
			}
		}
		else
		{// インスタンスを生成できなかった
			return NULL;
		}
	}
	else
	{// インスタンスを生成できなかった
		return NULL;
	}

	return pParticleManager;   // インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CParticleManager::Init(void)
{
	// パーティクルデータの読み込み
	if (FAILED(ParticleDataLoad()))
	{// 読み込みに失敗した
		return E_FAIL;
	}

	// テクスチャの読み込み
	if (FAILED(TextureLoad()))
	{// 読み込みに失敗した
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CParticleManager::Uninit(void)
{
	// パーティクルデータの開放
	if (m_pParticleData != NULL)
	{
		ReleaseParticleData();
	}

	// テクスチャの開放
	if (m_pTexture != NULL)
	{
		ReleaseTexture();
	}

	// 自身のポインタを開放
	Release();
}

//=============================================================================
//    更新処理
//=============================================================================
void CParticleManager::Update(void)
{

}

//=============================================================================
//    描画処理
//=============================================================================
void CParticleManager::Draw(void)
{

}

//=============================================================================
//    パーティクルデータを読み込む処理
//=============================================================================
HRESULT CParticleManager::ParticleDataLoad(void)
{
	FILE *pFile = NULL;               // ファイルポインタ
	int nCntParticleData = 0;  // 読み込んだパーティクルデータの数
	char aStr[128];            // テキストの文字列格納用

	if (pFile == NULL)
	{// ファイルポインタを確保できる状態である
		pFile = fopen(m_aParticleFileName, "r");
		if (pFile != NULL)
		{// ファイルポインタが確保できた
			while (fscanf(pFile, "%s", aStr) != EOF)
			{// 終了メッセージまでループ
				if (strcmp(aStr, NUM_EMITTER) == 0)
				{// エミッタの種類の数
					fscanf(pFile, "%s %d", aStr, &m_nNumEmitter);
					if (m_nNumEmitter >= 1)
					{// 読み込むエミッターの数が１個以上ある
						if (m_pParticleData == NULL)
						{// ポインタを確保できる状態である
							m_pParticleData = new CParticleData*[m_nNumEmitter];
							if (m_pParticleData != NULL)
							{// ポインタが確保できた
								for (int nCntParticle = 0; nCntParticle < m_nNumEmitter; nCntParticle++)
								{// 読み込むエミッターの数だけ繰り返し
									m_pParticleData[nCntParticle] = new CParticleData;
								}
							}
						}
					}
				}
				else if (strcmp(aStr, PARTICLESET) == 0)
				{// エミッタの情報
					if (m_pParticleData != NULL)
					{// ポインタが確保できている
						while (strcmp(aStr, END_PARTICLESET) != 0)
						{// 終了メッセージまでループ
							fscanf(pFile, "%s", &aStr);

							if (strcmp(aStr, MAXMOVE) == 0)
							{// 移動量の最大
								D3DXVECTOR3 MaxMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
								if (fscanf(pFile, "%s %f %f %f", aStr, &MaxMove.x, &MaxMove.y, &MaxMove.z) == 4)
								{// 移動量の最大値を読み込めた
									m_pParticleData[nCntParticleData]->SetMaxMove(MaxMove);
								}
							}
							else if (strcmp(aStr, MINMOVE) == 0)
							{// 移動量の最小
								D3DXVECTOR3 MinMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
								if (fscanf(pFile, "%s %f %f %f", aStr, &MinMove.x, &MinMove.y, &MinMove.z) == 4)
								{// 移動量の最小値を読み込めた
									m_pParticleData[nCntParticleData]->SetMinMove(MinMove);
								}
							}
							else if (strcmp(aStr, INITCOL) == 0)
							{// 初期カラー
								D3DXCOLOR InitCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
								if (fscanf(pFile, "%s %f %f %f %f", aStr, &InitCol.r, &InitCol.g, &InitCol.b, &InitCol.a) == 5)
								{// 初期カラーを読み込めた
									m_pParticleData[nCntParticleData]->SetInitCol(InitCol);
								}
							}
							else if (strcmp(aStr, FLAMECOL) == 0)
							{// フレームによるカラーの変化
								D3DXCOLOR ChangeCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
								if (fscanf(pFile, "%s %f %f %f %f", aStr, &ChangeCol.r, &ChangeCol.g, &ChangeCol.b, &ChangeCol.a) == 5)
								{// // フレームによるカラーの変化を読み込めた
									m_pParticleData[nCntParticleData]->SetChangeCol(ChangeCol);
								}
							}
							else if (strcmp(aStr, INITSIZE) == 0)
							{// 初期サイズ
								float fMinSize = 0.0f;
								float fMaxSize = 0.0f;
								if (fscanf(pFile, "%s %f %f", aStr, &fMinSize, &fMaxSize) == 3)
								{// 初期サイズを読み込めた
									m_pParticleData[nCntParticleData]->SetMinRadius(fMinSize);
									m_pParticleData[nCntParticleData]->SetMaxRadius(fMaxSize);
								}
							}
							else if (strcmp(aStr, FLAMESIZE) == 0)
							{// フレームによるサイズの変化
								float fChangeRadius = 0.0f;
								if (fscanf(pFile, "%s %f", aStr, &fChangeRadius) == 2)
								{// フレームによるサイズの変化を読み込めた
									m_pParticleData[nCntParticleData]->SetChangeRadius(fChangeRadius);
								}
							}
							else if (strcmp(aStr, EMITTERLIFE) == 0)
							{// 表示時間(寿命)
								int nLife = 0;
								if (fscanf(pFile, "%s %d", aStr, &nLife) == 2)
								{// 表示時間(寿命)を読み込めた
									m_pParticleData[nCntParticleData]->SetLife(nLife);
								}
							}
							else if (strcmp(aStr, PARLIFE) == 0)
							{// パーティクルの表示時間
								int nMinParLife = 0;
								int nMaxParLife = 0;
								if (fscanf(pFile, "%s %d %d", aStr, &nMinParLife, &nMaxParLife) == 3)
								{// パーティクルの表示時間を読み込めた
									m_pParticleData[nCntParticleData]->SetMinLife(nMinParLife);
									m_pParticleData[nCntParticleData]->SetMaxLife(nMaxParLife);
								}
							}
							else if (strcmp(aStr, APPEAR) == 0)
							{// 出る間隔
								int nAppear = 0;
								if (fscanf(pFile, "%s %d", aStr, &nAppear) == 2)
								{// 出る間隔を読み込めた
									m_pParticleData[nCntParticleData]->SetAppear(nAppear);
								}
							}
							else if (strcmp(aStr, SPREAD) == 0)
							{// 拡散値
								float fSpread = 0.0f;
								if (fscanf(pFile, "%s %f", aStr, &fSpread) == 2)
								{// 拡散値を読み込めた
									m_pParticleData[nCntParticleData]->SetSpread(fSpread);
								}
							}
							else if (strcmp(aStr, LAP) == 0)
							{// 1周に出す個数
								int nLap = 0;
								if (fscanf(pFile, "%s %d", aStr, &nLap) == 2)
								{// 1周に出す個数を読み込めた
									m_pParticleData[nCntParticleData]->SetLap(nLap);
								}
							}
							else if (strcmp(aStr, GRAVITY) == 0)
							{// 重力
								float fGravity = 0.0f;
								if (fscanf(pFile, "%s %f", aStr, &fGravity) == 2)
								{// 重力を読み込めた
									m_pParticleData[nCntParticleData]->SetGravity(fGravity);
								}
							}
							else if (strcmp(aStr, ROTPATTERN) == 0)
							{// 回転のパターン
								int nRotPattern = 0;
								if (fscanf(pFile, "%s %d", aStr, &nRotPattern) == 2)
								{// 1周に出す個数を読み込めた
									m_pParticleData[nCntParticleData]->SetRotPattern(nRotPattern);
								}
							}
							else if (strcmp(aStr, ROTSPEED) == 0)
							{// 回転の速度
								float fRotSpeed = 0.0f;
								if (fscanf(pFile, "%s %f", aStr, &fRotSpeed) == 2)
								{// 回転の速度を読み込めた
									m_pParticleData[nCntParticleData]->SetRotSpeed(fRotSpeed);
								}
							}
							else if (strcmp(aStr, LOOP) == 0)
							{// ループするかどうか
								int nLoop = 0;
								if (fscanf(pFile, "%s %d", aStr, &nLoop) == 2)
								{// ループするかどうかを読み込めた
									if (nLoop == 0)
									{
										m_pParticleData[nCntParticleData]->SetLoop(false);
									}
									else if(nLoop == 1)
									{
										m_pParticleData[nCntParticleData]->SetRotSpeed(true);
									}
								}
							}
							else if (strcmp(aStr, BOUND) == 0)
							{// バウンドするかどうか
								int nBound = 0;
								if (fscanf(pFile, "%s %d", aStr, &nBound) == 2)
								{// バウンドするかどうかを読み込めた
									if (nBound == 0)
									{
										m_pParticleData[nCntParticleData]->SetBouncing(false);
									}
									else if (nBound == 1)
									{
										m_pParticleData[nCntParticleData]->SetBouncing(true);
									}
								}
							}
							else if (strcmp(aStr, TEXTURE) == 0)
							{// テクスチャの種類
								int nTexIdx = 0;
								if (fscanf(pFile, "%s %d", aStr, &nTexIdx) == 2)
								{// テクスチャの種類を読み込めた
									m_pParticleData[nCntParticleData]->SetTexIdx(nTexIdx);
								}
							}
							else if (strcmp(aStr, DRAW) == 0)
							{// 加算合成の有無
								int nDraw = 0;
								if (fscanf(pFile, "%s %d", aStr, &nDraw) == 2)
								{// 加算合成の有無を読み込めた
									if (nDraw == 0)
									{
										m_pParticleData[nCntParticleData]->SetDrawAddtive(false);
									}
									else if (nDraw == 1)
									{
										m_pParticleData[nCntParticleData]->SetDrawAddtive(true);
									}
								}
							}
						}
					}
					nCntParticleData++;	// エミッタ番号のカウント

					if (nCntParticleData >= m_nNumEmitter)
					{// これ以上読み込めない
						break;
					}
				}
			}

			fclose(pFile);		// ファイルを閉じる
		}
		else
		{// ファイルが開けなかった
			return E_FAIL;
		}
	}
	else
	{// 失敗
		MessageBox(0, "ファイルが開けませんでした", m_aParticleFileName, MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    テクスチャを読み込む処理
//=============================================================================
HRESULT CParticleManager::TextureLoad(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	if (pRenderer != NULL)
	{// レンダリングクラスが取得できた
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
			FILE *pFile = NULL;  // ファイルポインタ
			char aFileName[64];		// テクスチャの名前

			pFile = fopen(m_aTextureFileName, "r");

			if (NULL != pFile)
			{// 開けた
				fscanf(pFile, "%d", &m_nNumTexture);		// テクスチャ数

				if (m_pTexture == NULL)
				{// メモリが確保できる状態にある
					m_pTexture = new LPDIRECT3DTEXTURE9[m_nNumTexture];
					if (m_pTexture != NULL)
					{// メモリが確保できた
						for (int nCntTexture = 0; nCntTexture < m_nNumTexture; nCntTexture++)
						{
							fscanf(pFile, "%s\n", &aFileName[0]);
							D3DXCreateTextureFromFile(pDevice, &aFileName[0], &m_pTexture[nCntTexture]);
						}
					}
				}
				fclose(pFile);
			}
			else
			{// 失敗
				MessageBox(0, "ファイルが開けませんでした", m_aTextureFileName, MB_OK);
				return E_FAIL;
			}
		}
		else
		{// デバイスが取得できなかった
			return E_FAIL;
		}
	}
	else
	{// レンダリングクラスが取得できなかった
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    パーティクルデータを開放する処理
//=============================================================================
void CParticleManager::ReleaseParticleData(void)
{
	for (int nCntParticle = 0; nCntParticle < m_nNumEmitter; nCntParticle++)
	{// 生成したパーティクルデータの数だけ繰り返し
		if (m_pParticleData[nCntParticle] != NULL)
		{// メモリが確保されている
			delete m_pParticleData[nCntParticle];
			m_pParticleData[nCntParticle] = NULL;
		}
	}
	delete[] m_pParticleData;
	m_pParticleData = NULL;
}

//=============================================================================
//    テクスチャを開放する処理
//=============================================================================
void CParticleManager::ReleaseTexture(void)
{
	for (int nCntTex = 0; nCntTex < m_nNumTexture; nCntTex++)
	{// 生成したテクスチャの数だけ繰り返し
		if (m_pTexture[nCntTex] != NULL)
		{// メモリが確保されている
			m_pTexture[nCntTex]->Release();
			m_pTexture[nCntTex] = NULL;
		}
	}
	delete[] m_pTexture;
	m_pTexture = NULL;
}

//=============================================================================
//    パーティクルデータを取得する処理
//=============================================================================
CParticleData *CParticleManager::GetParticleDate(int nIdx)
{
	if (m_pParticleData == NULL)
	{
		return NULL;
	}

	return m_pParticleData[nIdx];
}

//=============================================================================
//    テクスチャを取得する処理
//=============================================================================
LPDIRECT3DTEXTURE9 CParticleManager::GetTexture(int nIdx)
{
	if (m_pTexture == NULL)
	{
		return NULL;
	}

	return m_pTexture[nIdx];
}

//*****************************************************************************
//    CParEmitterの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CParEmitter::CParEmitter(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// 各種値のクリア
	m_pTexture = NULL;                      // パーティクルに設定するテクスチャ
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // エミッタの座標
	m_pParticleData = NULL;                 // パーティクルデータクラスへのポインタ
	m_nLife = 0;			                // エミッタの寿命
	m_nTexture = 0;	                        // テクスチャの種類
	m_nAppear = 0;                          // 放出する間隔
	m_nLap = 0;                             // 1回の放出で出すパーティクルの個数
	m_nType = 0;                            // 放出するパーティクルの種類
	m_nCntEmitter = 0;                      // 放出するタイミングを管理するカウンター
	m_bLoop = 0;                            // ループの有無
}

//=============================================================================
//    デストラクタ
//=============================================================================
CParEmitter::~CParEmitter()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CParEmitter *CParEmitter::Create(D3DXVECTOR3 pos, int nType, int nPriority)
{
	CParEmitter *pParEmitter = NULL;  // パーティクルエミッタクラス型のポインタ
	if (pParEmitter == NULL)
	{// メモリが空になっている
		pParEmitter = new CParEmitter(nPriority);
		if (pParEmitter != NULL)
		{// インスタンスを生成できた
		    // 各種値の設定
			pParEmitter->SetPos(pos);
			pParEmitter->SetType(nType);

			if (FAILED(pParEmitter->Init()))
			{// 初期化に失敗した
				return NULL;
			}
		}
		else
		{// インスタンスを生成できなかった
			return NULL;
		}
	}
	else
	{// インスタンスを生成できなかった
		return NULL;
	}

	return pParEmitter;   // インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CParEmitter::Init(void)
{
	// 管理クラスを取得する
	CParticleManager *pParticleManager = NULL;
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntScene = 0; nCntScene < MAX_PRIORITY_NUM; nCntScene++)
	{// 描画優先順位の数だけ繰り返し
		if (pParticleManager == NULL)
		{// パーティクルの管理クラスを取得できていない
			pScene = CScene::GetScene(nCntScene);
			while (pScene != NULL)
			{// ポインタが空になるまで繰り返し
				pSceneNext = pScene->GetNext();
				if (pScene->GetObjType() == OBJTYPE_PARTICLEMANAGER)
				{// パーティクルの管理クラスだったら
					pParticleManager = (CParticleManager*)pScene;
					break;
				}
				pScene = pSceneNext;
			}
		}
	}


	// 各種値を管理クラスが保有するデータから抜き取る
	if (pParticleManager != NULL)
	{// 管理クラスが取得できた
		// パーティクルのデータ
		if (m_pParticleData == NULL)
		{// ポインタを確保できる状態である
			m_pParticleData = pParticleManager->GetParticleDate(m_nType);
			if (m_pParticleData != NULL)
			{// ポインタを確保できた
				m_nAppear = m_pParticleData->GetAppear();
				m_nLife = m_pParticleData->GetLife();
				m_bLoop = m_pParticleData->GetLoop();
				m_nLap = m_pParticleData->GetLap();
			}
		}

		// テクスチャ
		if (m_pTexture == NULL)
		{// ポインタを確保できる状態である
			m_pTexture = pParticleManager->GetTexture(m_pParticleData->GetTexIdx());
		}
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CParEmitter::Uninit(void)
{
	// エミッタの削除
	Release();
}

//=============================================================================
//    更新処理
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
	m_nCntEmitter = (m_nCntEmitter + 1) % m_nAppear;

	if (m_nCntEmitter == 0)
	{// 出現させるタイミングになった
		for (int nCntParticle = 0; nCntParticle < m_nLap; nCntParticle++)
		{// 一回の放出で出す個数分繰り返し
			SetParticle();
		}
	}

	if (true == bDelete)
	{// 消す
		Uninit();
	}
}

//=============================================================================
//    描画処理
//=============================================================================
void CParEmitter::Draw(void)
{

}

//=============================================================================
//    パーティクルをセットする処理
//=============================================================================
void CParEmitter::SetParticle(void)
{
	// 生成に必要なデータを宣言
	D3DXVECTOR3 Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);         // 移動量
	D3DXCOLOR InitCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);    // 初期化時の色
	float fRadius = 0.0f;                                     // 半径
	float fAngle = 0.0f;                                      // 向き
	D3DXCOLOR ChangeCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);  // 色の変化量
	float fGravity = 0.0f;                                    // 重力
	float fChangeRadisu = 0.0f;                               // 半径の変化量
	float fChangeRot = 0.0f;                                  // 向きの変化量
	float fBouncing = 0.0f;                                   // バウンド量
	int nLife = 0;                                            // 体力
	int nRotPattern = 0;                                      // 回転のパターン
	bool bBound = false;                                      // バウンドするかしないか
	bool bDrawAddtive = false;                                // 加算合成で描画するかしないか

	// データを作成する
	if (m_pParticleData != NULL)
	{// メモリが確保されている
	     // データから代入する部分はデータから引っ張ってくる
		InitCol = m_pParticleData->GetInitCol();
		ChangeCol = m_pParticleData->GetChangeCol();
		fGravity = m_pParticleData->GetGravity();
		fChangeRadisu = m_pParticleData->GetChangeRadius();
		fChangeRot = m_pParticleData->GetRotSpeed();
		nRotPattern = m_pParticleData->GetRotPattern();
		bBound = m_pParticleData->GetBouncing();
		bDrawAddtive = m_pParticleData->GetDrawAddtive();

		// 必要なところだけ計算する
		// 移動量
		int nLap = m_pParticleData->GetLap();
		float fSpread = m_pParticleData->GetSpread();
		D3DXVECTOR3 MinMove = m_pParticleData->GetMinMove();
		D3DXVECTOR3 MaxMove = m_pParticleData->GetMaxMove();
		fAngle = 1.0f / (nLap / 2);
		if (0 != MaxMove.y && 0 != MinMove.y)
		{// Y軸移動量が両方０以外
			Move = D3DXVECTOR3(sinf(D3DX_PI * (0.0f + (fAngle))) * ((MinMove.x + (rand() % (1 + (int)MaxMove.x))) - ((rand() % 10) * 0.1f)) * fSpread,
				((rand() % (int)MaxMove.y) + (rand() % (int)MinMove.y)) * fSpread,
				cosf(D3DX_PI * (0.0f + (fAngle ))) * ((MinMove.z + (rand() % (1 + (int)MaxMove.z))) - ((rand() % 10) * 0.1f)) * fSpread);
		}
		else
		{// Y軸の移動量が０がある場合
			Move = D3DXVECTOR3(sinf(D3DX_PI * (0.0f + (fAngle))) * ((MinMove.x + (rand() % (1 + (int)MaxMove.x))) - ((rand() % 10) * 0.1f)) * fSpread,
				((int)MaxMove.y + (int)MinMove.y) * fSpread,
				cosf(D3DX_PI * (0.0f + (fAngle))) * ((MinMove.z + (rand() % (1 + (int)MaxMove.z))) - ((rand() % 10) * 0.1f)) * fSpread);
		}

		// 寿命
		int nMinLife = m_pParticleData->GetMinLife();
		int nMaxLife = m_pParticleData->GetMaxLife();
		nLife = nMinLife + (rand() % (1 + (nMaxLife - nMinLife)) - 1);

		// 半径
		int fMinRadius = (int)m_pParticleData->GetMinRadius();
		int fMaxRadius = (int)m_pParticleData->GetMaxRadius();
		fRadius = fMinRadius + (rand() % (1 + (fMaxRadius - fMinRadius)) - 1) + ((rand() % 10) * 0.1f);

		// パーティクルを生成する
		CParticle *pParticle = CParticle::Create(m_Pos, Move, InitCol, fRadius, fAngle,
					 							 ChangeCol, fGravity, fChangeRadisu, fChangeRot,
												 fBouncing, nLife, nRotPattern, m_nType,
												 bBound, bDrawAddtive,3);
		if (pParticle != NULL)
		{// メモリが確保できた
			pParticle->BindTexture(m_pTexture);
		}
	}
}

//=============================================================================
//    座標を設定する処理
//=============================================================================
void CParEmitter::SetPos(D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//=============================================================================
//    種類を設定する処理
//=============================================================================
void CParEmitter::SetType(int nType)
{
	m_nType = nType;
}

//*****************************************************************************
//    CParticleの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CParticle::CParticle(int nPriority, OBJTYPE objType) : CSceneBillboard(nPriority, objType)
{
	// 各種値のクリア
	m_ChangeCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f); // どれくらい色が変化するか
	m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);          // 移動量
	m_fGravity = 0.0f;                               // 重力
	m_fChangeRadius = 0.0f;                          // どれくらい変形するか
	m_fChangeRot = 0.0f;                             // どれくらい回転するか
	m_fBouncing = 0.0f;                              // 地面についた際にバウンドする値
	m_nLife = 0;                                     // 寿命
	m_nRotPattern = 0;                               // 回転の種類
	m_nType = 0;                                     // 種類
	m_bBound = false;                                // バウンドするかどうか
	m_bDrawAddtive = false;                          // 加算合成で描画するかどうか
}

//=============================================================================
//    デストラクタ
//=============================================================================
CParticle::~CParticle()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CParticle *CParticle::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, float fAngle,
							 D3DXCOLOR ChangeCol, float fGravity, float fChangeRadius,
							 float fChangeRot, float fBouncing, int nLife, int nRotPattern,
							 int nType, bool bBound, bool bDrawAddtive, int nPriority)
{
	CParticle *pParticle = NULL;  // パーティクルクラス型のポインタ
	if (pParticle == NULL)
	{// メモリが空になっている
		pParticle = new CParticle(nPriority);
		if (pParticle != NULL)
		{// インスタンスを生成できた
			if (FAILED(pParticle->Init(pos, move, col, fRadius, fAngle, ChangeCol, fGravity, fChangeRadius, fChangeRot, fBouncing, nLife, nRotPattern, nType, bBound, bDrawAddtive)))
			{// 初期化に失敗した
				return NULL;
			}
		}
		else
		{// インスタンスを生成できなかった
			return NULL;
		}
	}
	else
	{// インスタンスを生成できなかった
		return NULL;
	}

	return pParticle;   // インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CParticle::Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, float fAngle,
						D3DXCOLOR ChangeCol, float fGravity, float fChangeRadius,
						float fChangeRot, float fBouncing, int nLife, int nRotPattern,
						int nType, bool bBound, bool bDrawAddtive)
{
	// 各種値の設定
	SetPos(pos);                       // 座標
	SetCol(col);                       // 色
	SetLength(fRadius);                // 半径
	SetAngle(fAngle);                  // 向き
	m_Move = move;                     // 移動量
	m_ChangeCol = ChangeCol;           // 色
	m_fGravity = fGravity;             // 半径
	m_fChangeRadius = fChangeRadius;   // 半径の変化量
	m_fChangeRot = fChangeRot;         // 向きの変化量
	m_fBouncing = fBouncing;           // バウンド量
	m_nLife = nLife;                   // 寿命
	m_nRotPattern = nRotPattern;       // 回転のパターン
	m_nType = nType;                   // 種類
	m_bBound = bBound;                 // バウンドするかしないか
	m_bDrawAddtive = bDrawAddtive;     // 加算合成で描画するかしないか

	// 共通の初期化処理
	SetWidth(cosf(fAngle) * fRadius);
	SetHeight(-sinf(fAngle) * fRadius);
	if (FAILED(CSceneBillboard::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CParticle::Uninit(void)
{
	// 共通の終了処理
	CSceneBillboard::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CParticle::Update(void)
{
	m_nLife--;		// 表示時間をカウント

	if (m_nLife <= 0)
	{// 表示時間が0以下になった
		Uninit();	// 終了処理
	}
	else
	{// まだ表示時間がある
		// 各種値の取得
		D3DXVECTOR3 pos = GetPos();  // 座標
		D3DXCOLOR col = GetCol();    // 色
		float fAngle = GetAngle();   // 向き
		float fRadius = GetLength(); // 半径
		m_Move.y -= m_fGravity;		 // 重力を加える

		m_fBouncing = pos.y;		// 値を記憶

		pos += m_Move;				// 位置の更新
		col += m_ChangeCol;         // 色の更新
		fRadius += m_fChangeRadius;	// 大きさの更新

		if (pos.y <= 0 && m_bBound == true)
		{// バウンド
			m_Move.y += (m_fBouncing - pos.y) * 2.0f;
		}

		if (m_nRotPattern == 1)
		{// 時計回り
			fAngle -= m_fChangeRot;			// 回転のカウンタ
		}
		else if (m_nRotPattern == 2)
		{// 反時計回り
			fAngle += m_fChangeRot;			// 回転のカウンタ
		}
		else if (m_nRotPattern == 3)
		{// ランダム
			if (rand() % 2 == 0)
			{// 時計回り
				fAngle += m_fChangeRot;		// 回転のカウンタ
			}
			else
			{// 反時計回り
				fAngle -= m_fChangeRot;		// 回転のカウンタ
			}
		}

		if (fAngle > D3DX_PI)
		{
			fAngle -= D3DX_PI * 2.0f;
		}
		if (fAngle > D3DX_PI)
		{
			fAngle += D3DX_PI * 2.0f;
		}

		// 各種値の設定
		SetPos(pos);          // 向き
		SetCol(col);          // 色
		SetAngle(fAngle);     // 向き
		SetLength(fRadius);   // 大きさ
	}
}

//=============================================================================
//    描画処理
//=============================================================================
void CParticle::Draw(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	if (pRenderer != NULL)
	{// レンダリングクラスが取得できた
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
			// 被らないようにする
			pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

			if (m_bDrawAddtive == true)
			{ // αブレンディングを加算合成に設定
				pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
				pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			}

			CSceneBillboard::Draw();

			// αブレンディングを元に戻す
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

			// Zバッファを元に戻す
			pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		}
	}
}

//=============================================================================
//    頂点バッファ生成処理
//=============================================================================
void CParticle::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファの生成
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = NULL;
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&pVtxBuff,
		NULL);

	// 頂点情報の設定
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標
	pVtx[0].pos = D3DXVECTOR3(-sinf(GetAngle()) * GetLength(), cosf(GetAngle()) * GetLength(), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(sinf(GetAngle()) * GetLength(), cosf(GetAngle()) * GetLength(), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-sinf(GetAngle()) * GetLength(), -cosf(GetAngle()) * GetLength(), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(sinf(GetAngle()) * GetLength(), -cosf(GetAngle()) * GetLength(), 0.0f);

	// 法線
	pVtx[0].nor = D3DXVECTOR3(-0.22f, 0.87f, -0.44f);
	pVtx[1].nor = D3DXVECTOR3(-0.22f, 0.87f, -0.44f);
	pVtx[2].nor = D3DXVECTOR3(-0.22f, 0.87f, -0.44f);
	pVtx[3].nor = D3DXVECTOR3(-0.22f, 0.87f, -0.44f);

	// 頂点カラー
	pVtx[0].col = GetCol();
	pVtx[1].col = GetCol();
	pVtx[2].col = GetCol();
	pVtx[3].col = GetCol();

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();

	// 頂点バッファの設定
	SetVtxBuff(pVtxBuff);
}