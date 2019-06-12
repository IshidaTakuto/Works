//=============================================================================
//
// �t�B�[���h���� [field.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#include "field.h"
#include "manager.h"
#include "renderer.h"
#include "wall.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_NAME00	"data/TEXTURE/field00.jpg"
#define TEXTURE_NAME01	"data/TEXTURE/river000.jpg"
#define TEXTURE_NAME02	"data/TEXTURE/river000.png"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 CField::m_pTexture[FIELD_TEXTURE] = {};
int CField::m_nCntPos = -1;

//==================================
// ��������
//==================================
CField* CField::Create(TYPE type)
{
	CField *pField;

	pField = new CField;	// ���������m��
	pField->Init(type);
	pField->BindTexture(m_pTexture[type]);

	return pField;
}

//=========================================
// �e�N�X�`���̓ǂݍ���
//=========================================
HRESULT CField::Load(void)
{
	// �ǂ̓ǂݍ���
	CWall::Load();

	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME00, &m_pTexture[0]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME01, &m_pTexture[1]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME02, &m_pTexture[2]);

	return S_OK;
}

//=========================================
// �ǂݍ��񂾃e�N�X�`���̉��
//=========================================
void CField::Unload(void)
{
	// �ǂ̏���j��
	CWall::Unload();

	// �e�N�X�`���̔j��
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
// �R���X�g���N�^
//=========================================
CField::CField(int nPriority) : CScene3D(nPriority)
{
	m_type = TYPE_GROUND;
	m_pScene3D = NULL;
}

//=========================================
// �f�X�g���N�^
//=========================================
CField::~CField()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CField::Init(TYPE type)
{
	CScene3D::Init();														// ����������
	CScene3D::SetPos(D3DXVECTOR3(m_nCntPos * FIELD_WIDTH, 0.0f, 0.0f));		// �ʒu�̐ݒ�
	CScene3D::SetLength(D3DXVECTOR3(FIELD_WIDTH / 2, 0.0f, FIELD_DEPTH));	// �����̐ݒ�
	CScene::SetObjType(CScene::OBJTYPE_FIELD);								// �I�u�W�F�N�g�^�C�v�̐ݒ�

	// �ǂ̐���
	m_pWall = CWall::Create(m_nCntPos * (int)FIELD_WIDTH);

	// �n�ʂ̎�ނ̐ݒ�
	m_type = type;

	if (m_type == TYPE_SEA)
	{// �C�̏ꍇ
		m_pScene3D = new CScene3D;
		m_pScene3D->Init();
		m_pScene3D->BindTexture(m_pTexture[2]);
		m_pScene3D->SetPos(D3DXVECTOR3(m_nCntPos * FIELD_WIDTH, 0.01f, 0.0f));
		m_pScene3D->SetLength(D3DXVECTOR3(FIELD_WIDTH / 2, 0.0f, FIELD_DEPTH));
		m_pScene3D->SetObjType(CScene::OBJTYPE_FIELD);
	}

	m_nCntPos++;	// �ʒu�����炷���߂̃J�E���g
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CField::Uninit(void)
{
	if (m_pWall != NULL)
	{// �ǂ̔j��
		m_pWall->Uninit();
	}

	if (m_pScene3D != NULL)
	{// 3D�I�u�W�F�N�g�̊J��
		m_pScene3D->Uninit();
		m_pScene3D = NULL;
	}

	// �I������
	CScene3D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CField::Update(void)
{
	if (m_type == TYPE_SEA)
	{// �C�̏ꍇ
		if (m_pScene3D != NULL)
		{// �e�N�X�`�����W�𓮂���
			m_pScene3D->MoveTexPos(D3DXVECTOR2(0.0004f, 0.0001f));
		}
		CScene3D::MoveTexPos(D3DXVECTOR2(-0.0001f, -0.0002f));
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CField::Draw(void)
{
	CScene3D::Draw();
}

//=============================================================================
// �����蔻��
//=============================================================================
bool CField::Collision(D3DXVECTOR3 *pos, float fVtxMinY)
{
	bool bField = 0;

	D3DXVECTOR3 thisPos = CScene3D::GetPos();		// �n�ʂ̈ʒu���擾

	if (pos->y - fVtxMinY <= thisPos.y)
	{// �n�ʂɂ��Ă���
		D3DXVECTOR3 length = CScene3D::GetLength();	// �n�ʂ̑傫�����擾

		if (pos->z > thisPos.z - length.z && pos->z < thisPos.z + length.z &&
			pos->x > thisPos.x - length.x && pos->x < thisPos.x + length.x)
		{// ���s�����n�ʂ͈͓̔�
			if (false == bField)
			{// ���n�����L���ɂ���
				bField = true;
				pos->y = thisPos.y + fVtxMinY;		// Y����n�ʂ̏�ɖ߂�
			}
		}
	}

	return bField;
}