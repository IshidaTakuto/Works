//=============================================================================
//
// フィールド処理 [field.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#include "field.h"
#include "manager.h"
#include "renderer.h"
#include "wall.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_NAME00	"data/TEXTURE/field00.jpg"
#define TEXTURE_NAME01	"data/TEXTURE/river000.jpg"
#define TEXTURE_NAME02	"data/TEXTURE/river000.png"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CField::m_pTexture[FIELD_TEXTURE] = {};
int CField::m_nCntPos = -1;

//==================================
// 生成処理
//==================================
CField* CField::Create(TYPE type)
{
	CField *pField;

	pField = new CField;	// メモリを確保
	pField->Init(type);
	pField->BindTexture(m_pTexture[type]);

	return pField;
}

//=========================================
// テクスチャの読み込み
//=========================================
HRESULT CField::Load(void)
{
	// 壁の読み込み
	CWall::Load();

	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME00, &m_pTexture[0]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME01, &m_pTexture[1]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME02, &m_pTexture[2]);

	return S_OK;
}

//=========================================
// 読み込んだテクスチャの解放
//=========================================
void CField::Unload(void)
{
	// 壁の情報を破棄
	CWall::Unload();

	// テクスチャの破棄
	for (int nCntTexture = 0; nCntTexture < FIELD_TEXTURE; nCntTexture++)
	{
		if (NULL != m_pTexture[nCntTexture])
		{
			m_pTexture[nCntTexture]->Release();
			m_pTexture[nCntTexture] = NULL;
		}
	}

	m_nCntPos = -1;
}

//=========================================
// コンストラクタ
//=========================================
CField::CField(int nPriority) : CScene3D(nPriority)
{
	m_type = TYPE_GROUND;
	m_pScene3D = NULL;
}

//=========================================
// デストラクタ
//=========================================
CField::~CField()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CField::Init(TYPE type)
{
	CScene3D::Init();														// 初期化処理
	CScene3D::SetPos(D3DXVECTOR3(m_nCntPos * FIELD_WIDTH, 0.0f, 0.0f));		// 位置の設定
	CScene3D::SetLength(D3DXVECTOR3(FIELD_WIDTH / 2, 0.0f, FIELD_DEPTH));	// 長さの設定
	CScene::SetObjType(CScene::OBJTYPE_FIELD);								// オブジェクトタイプの設定

	// 壁の生成
	m_pWall = CWall::Create(m_nCntPos * (int)FIELD_WIDTH);

	// 地面の種類の設定
	m_type = type;

	if (m_type == TYPE_SEA)
	{// 海の場合
		m_pScene3D = new CScene3D;
		m_pScene3D->Init();
		m_pScene3D->BindTexture(m_pTexture[2]);
		m_pScene3D->SetPos(D3DXVECTOR3(m_nCntPos * FIELD_WIDTH, 0.01f, 0.0f));
		m_pScene3D->SetLength(D3DXVECTOR3(FIELD_WIDTH / 2, 0.0f, FIELD_DEPTH));
		m_pScene3D->SetObjType(CScene::OBJTYPE_FIELD);
	}

	m_nCntPos++;	// 位置をずらすためのカウント
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CField::Uninit(void)
{
	if (m_pWall != NULL)
	{// 壁の破棄
		m_pWall->Uninit();
	}

	if (m_pScene3D != NULL)
	{// 3Dオブジェクトの開放
		m_pScene3D->Uninit();
		m_pScene3D = NULL;
	}

	// 終了処理
	CScene3D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CField::Update(void)
{
	if (m_type == TYPE_SEA)
	{// 海の場合
		if (m_pScene3D != NULL)
		{// テクスチャ座標を動かす
			m_pScene3D->MoveTexPos(D3DXVECTOR2(0.0004f, 0.0001f));
		}
		CScene3D::MoveTexPos(D3DXVECTOR2(-0.0001f, -0.0002f));
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CField::Draw(void)
{
	CScene3D::Draw();
}

//=============================================================================
// 当たり判定
//=============================================================================
bool CField::Collision(D3DXVECTOR3 *pos, float fVtxMinY)
{
	bool bField = 0;

	D3DXVECTOR3 thisPos = CScene3D::GetPos();		// 地面の位置を取得

	if (pos->y - fVtxMinY <= thisPos.y)
	{// 地面についている
		D3DXVECTOR3 length = CScene3D::GetLength();	// 地面の大きさを取得

		if (pos->z > thisPos.z - length.z && pos->z < thisPos.z + length.z &&
			pos->x > thisPos.x - length.x && pos->x < thisPos.x + length.x)
		{// 奥行きが地面の範囲内
			if (false == bField)
			{// 着地判定を有効にする
				bField = true;
				pos->y = thisPos.y + fVtxMinY;		// Y軸を地面の上に戻す
			}
		}
	}

	return bField;
}