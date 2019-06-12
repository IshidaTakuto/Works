//=============================================================================
//
// �c�@���� [stock.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#include "stock.h"
#include "polygon.h"
#include "number.h"
#include "scene3DModel.h"
#include "player.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define STOCK_SIZE		(40)	// �c�@�̑傫��

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
CPolygon *CStock::m_pPolygon[MAX_STOCK] = {};
CNumber *CStock::m_pNumber				= NULL;
int		  CStock::m_nStock				= 0;

//=============================================================================
// ��������
//=============================================================================
CStock* CStock::Create(void)
{
	CStock *pStock;

	pStock = new CStock;	// ���������m��

	// �c�@�̃|���S��
	pStock->m_pPolygon[0] = CPolygon::Create(0, D3DXVECTOR3(530.0f - (STOCK_SIZE * 2), 670.0f, 0.0f), STOCK_SIZE, STOCK_SIZE, CPolygon::TYPE_STOCK00);

	// �c�@��
	pStock->m_pNumber = CNumber::Create(D3DXVECTOR3(530.0f, 670.0f, 0.0f), STOCK_SIZE);

	// ����������
	pStock->Init();

	return pStock;
}

//=========================================
// �R���X�g���N�^
//=========================================
CStock::CStock(int nPriority) : CScene(nPriority)
{
}

//=========================================
// �f�X�g���N�^
//=========================================
CStock::~CStock()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CStock::Init(void)
{
	// �I�u�W�F�N�g�^�C�v�̐ݒ�
	CScene::SetObjType(CScene::OBJTYPE_UI);

	m_nStock = NUM_STOCK;
	m_pNumber->SetNumber(NUM_STOCK);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CStock::Uninit(void)
{
	// �|���S���N���X�̏I������
	for (int nCntPolygon = 0; nCntPolygon < MAX_STOCK; nCntPolygon++)
	{
		if (m_pPolygon[nCntPolygon] != NULL)
		{
			m_pPolygon[nCntPolygon]->Uninit();
			delete m_pPolygon[nCntPolygon];
			m_pPolygon[nCntPolygon] = NULL;
		}
	}

	// �i���o�[�N���X�̏I������
	if (m_pNumber != NULL)
	{
		m_pNumber->Uninit();
		delete m_pNumber;
		m_pNumber = NULL;
	}


	// ���C�t�̃|�C���^�ϐ����J��
	if (NULL != m_nStock)
	{
		m_nStock = NULL;
	}

	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CStock::Update(void)
{
	for (int nCntScene = 0; nCntScene < MAX_OBJECT; nCntScene++)
	{
		CScene3DModel *pScene3DModel;		// �I�u�W�F�N�g�̃|�C���^�ϐ�
		pScene3DModel = (CScene3DModel*)GetScene(PLAYER_PRIORITY, nCntScene);		// �C���f�b�N�X���Ɏ擾

		if (pScene3DModel != NULL)
		{// �g���Ă��邩�̃`�F�b�N
			CScene::OBJTYPE objType = pScene3DModel->GetObjType();;					// �I�u�W�F�N�g�̎�ނ��擾

			if (objType == CScene::OBJTYPE_PLAYER)
			{// �I�u�W�F�N�g�̎�ނ��v���C���[�̂Ƃ�
				CPlayer *pPlayer;	// �v���C���[�̏����i�[
				pPlayer = (CPlayer*)pScene3DModel->GetScene(PLAYER_PRIORITY, nCntScene);	// �v���C���[�̏����擾
				CPlayer::TYPE typePlayer = pPlayer->GetType();								// �v���C���[�̌`�Ԃ��擾

				if (typePlayer == CPlayer::TYPE_AIRCRAFT)
				{// �퓬�@
					m_pPolygon[0]->TextureChange(CPolygon::TYPE_STOCK00);
				}
				else if (typePlayer == CPlayer::TYPE_SHIP)
				{// ���
					m_pPolygon[0]->TextureChange(CPolygon::TYPE_STOCK01);
				}
				else if (typePlayer == CPlayer::TYPE_TANK)
				{// ���
					m_pPolygon[0]->TextureChange(CPolygon::TYPE_STOCK02);
				}
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CStock::Draw(void)
{
	for (int nCntPolygon = 0; nCntPolygon < MAX_STOCK; nCntPolygon++)
	{// �|���S���̕`��
		m_pPolygon[nCntPolygon]->Draw();
	}

	m_pNumber->Draw();	// �����̕`�揈��
}

//=============================================================================
// �c�@�̑���
//=============================================================================
const void CStock::AddSubtract(int nValue)
{
	if (m_nStock <= NUM_STOCK)
	{// �X�g�b�N���ő吔�𒴂��Ȃ��悤�ɂ���
		m_nStock += nValue;		// �X�g�b�N�̑���
	}

	m_pNumber->SetNumber(m_nStock);		// �c�@���̍X�V
}
