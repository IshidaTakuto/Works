//=============================================================================
//
// エフェクト処理 [effect.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#include "effect.h"
#include "manager.h"
#include "renderer.h"
#include "enemy.h"

//==================================
// マクロ定義
//==================================
#define EFFECT_TEXTURE_NAME_00	"data/TEXTURE/shadow000.jpg"
#define EFFECT_TEXTURE_NAME_01	"data/TEXTURE/ripples.jpg"
#define EFFECT_TEXTURE_NAME_02	"data/TEXTURE/biim.jpg"

//==================================
// 静的メンバ変数宣言
//==================================
LPDIRECT3DTEXTURE9 CEffect::m_pTexture[EFFECT_NUMTEXTURE] = {};

//==================================
// 生成処理
//==================================
CEffect* CEffect::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fRange, int nLife, TYPE type)
{
	CEffect *pEffect = NULL;

	pEffect = new CEffect;		// メモリを確保

	if (NULL != pEffect)
	{// メモリが確保されているか確認
		pEffect->m_type = type;
		pEffect->Init(pos);						// 初期化
		pEffect->BindTexture(m_pTexture[type]);	// テクスチャの適用
		pEffect->SetColor(col);					// 色の設定
		pEffect->SetLength(fRange, fRange);		// サイズの設定
		pEffect->SetLife(nLife);				// 寿命の設定
	}
	return pEffect;
}

//=========================================
// テクスチャの読み込み
//=========================================
HRESULT CEffect::Load(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = pRenderer->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, EFFECT_TEXTURE_NAME_00, &m_pTexture[0]);
	D3DXCreateTextureFromFile(pDevice, EFFECT_TEXTURE_NAME_01, &m_pTexture[1]);
	D3DXCreateTextureFromFile(pDevice, EFFECT_TEXTURE_NAME_02, &m_pTexture[2]);

	return S_OK;
}

//=========================================
// 読み込んだテクスチャの解放
//=========================================
void CEffect::Unload(void)
{
	// テクスチャの破棄
	for (int nCntTexture = 0; nCntTexture < EFFECT_NUMTEXTURE; nCntTexture++)
	{
		if (NULL != m_pTexture[nCntTexture])
		{
			m_pTexture[nCntTexture]->Release();
			m_pTexture[nCntTexture] = NULL;
		}
	}
}

//=========================================
// コンストラクタ
//=========================================
CEffect::CEffect(int nPriority) : CScene3DBill(nPriority)
{
}

//=========================================
// デストラクタ
//=========================================
CEffect::~CEffect()
{
}

//=========================================
// 初期化処理
//=========================================
HRESULT CEffect::Init(D3DXVECTOR3 pos)
{
	CScene3DBill::Init();

	// ポリゴンの位置を設定
	CScene3DBill::SetPos(pos);

	CScene::SetObjType(CScene::OBJTYPE_EFFECT);

	return S_OK;
}

//=========================================
// 終了処理
//=========================================
void CEffect::Uninit(void)
{
	CScene3DBill::Uninit();
}

//=========================================
// 更新処理
//=========================================
void CEffect::Update(void)
{
	int nLife;								// ライフを格納
	float fLengthX, fLengthY;				// 大きさの更新用

	fLengthX = CScene3DBill::GetLengthX();	// 大きさを取得
	fLengthY = CScene3DBill::GetLengthY();	// 大きさを取得

	if (m_type == TYPE_NORMAL)
	{// 通常
		// 小さくする
		fLengthX -= 0.5f;
		fLengthY -= 0.5f;
	}
	else if (m_type == TYPE_RIPPLES)
	{// 波紋
		// 大きくする
		fLengthX += 1.0f;
		fLengthY += 1.0f;

		// 位置をずらす
		D3DXVECTOR3 pos = CScene3DBill::GetPos();
		pos.x -= 1.5f;
		CScene3DBill::SetPos(pos);
	}
	else if (m_type == TYPE_BIIM)
	{// ビーム
		if (fLengthY < 30)
		{// 一定まで大きくする
			fLengthY += 5.0f;
		}

		for (int nCntScene = 0; nCntScene < MAX_OBJECT; nCntScene++)
		{// 敵の情報の探索
			CScene3DModel *pScene3DModel;											// 3Dオブジェクトのポインタ変数
			pScene3DModel = (CScene3DModel*)GetScene(ENEMY_PRIORITY, nCntScene);	// インデックス順に取得

			if (pScene3DModel != NULL)
			{// nullチェック
				CScene::OBJTYPE objType;					// オブジェクトの種類判別用の変数
				objType = pScene3DModel->GetObjType();		// オブジェクトの種類を取得

				if (objType == CScene::OBJTYPE_ENEMY)
				{// オブジェクトの種類が敵のとき
					CEnemy *pEnemy = (CEnemy*)pScene3DModel->GetScene(ENEMY_PRIORITY, nCntScene);	// 敵の情報を取得;															// 敵の情報を格納
					D3DXVECTOR3 pos = pEnemy->GetPos();
					float fRange = pEnemy->GetLength();

					if (CScene3DBill::CollisionXY(pos, (fRange * 2.0f)))
					{// Y軸の判定
						pEnemy->Uninit();
					}
				}
			}
		}

		// 位置をずらす
		D3DXVECTOR3 pos = CScene3DBill::GetPos();
		pos.x += 10.0f;
		CScene3DBill::SetPos(pos);
	}

	nLife = CScene3DBill::SubtractLife(1);			// ライフを減らす
	CScene3DBill::SetLength(fLengthX, fLengthY);	// 大きさの更新

	if (nLife <= 0 || fLengthX <= 0)
	{// ライフがなくなったとき || 大きさが0以下になったとき
		Uninit();		// 終了処理
	}
}

//=========================================
// 描画処理
//=========================================
void CEffect::Draw(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// 被らないようにする
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	// 描画処理
	CScene3DBill::Draw();

	// Zバッファを元に戻す
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}
