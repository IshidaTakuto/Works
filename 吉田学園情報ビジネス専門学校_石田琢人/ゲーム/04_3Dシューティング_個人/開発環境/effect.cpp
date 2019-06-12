//=============================================================================
//
// �G�t�F�N�g���� [effect.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#include "effect.h"
#include "manager.h"
#include "renderer.h"
#include "enemy.h"

//==================================
// �}�N����`
//==================================
#define EFFECT_TEXTURE_NAME_00	"data/TEXTURE/shadow000.jpg"
#define EFFECT_TEXTURE_NAME_01	"data/TEXTURE/ripples.jpg"
#define EFFECT_TEXTURE_NAME_02	"data/TEXTURE/biim.jpg"

//==================================
// �ÓI�����o�ϐ��錾
//==================================
LPDIRECT3DTEXTURE9 CEffect::m_pTexture[EFFECT_NUMTEXTURE] = {};

//==================================
// ��������
//==================================
CEffect* CEffect::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fRange, int nLife, TYPE type)
{
	CEffect *pEffect = NULL;

	pEffect = new CEffect;		// ���������m��

	if (NULL != pEffect)
	{// ���������m�ۂ���Ă��邩�m�F
		pEffect->m_type = type;
		pEffect->Init(pos);						// ������
		pEffect->BindTexture(m_pTexture[type]);	// �e�N�X�`���̓K�p
		pEffect->SetColor(col);					// �F�̐ݒ�
		pEffect->SetLength(fRange, fRange);		// �T�C�Y�̐ݒ�
		pEffect->SetLife(nLife);				// �����̐ݒ�
	}
	return pEffect;
}

//=========================================
// �e�N�X�`���̓ǂݍ���
//=========================================
HRESULT CEffect::Load(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = pRenderer->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, EFFECT_TEXTURE_NAME_00, &m_pTexture[0]);
	D3DXCreateTextureFromFile(pDevice, EFFECT_TEXTURE_NAME_01, &m_pTexture[1]);
	D3DXCreateTextureFromFile(pDevice, EFFECT_TEXTURE_NAME_02, &m_pTexture[2]);

	return S_OK;
}

//=========================================
// �ǂݍ��񂾃e�N�X�`���̉��
//=========================================
void CEffect::Unload(void)
{
	// �e�N�X�`���̔j��
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
// �R���X�g���N�^
//=========================================
CEffect::CEffect(int nPriority) : CScene3DBill(nPriority)
{
}

//=========================================
// �f�X�g���N�^
//=========================================
CEffect::~CEffect()
{
}

//=========================================
// ����������
//=========================================
HRESULT CEffect::Init(D3DXVECTOR3 pos)
{
	CScene3DBill::Init();

	// �|���S���̈ʒu��ݒ�
	CScene3DBill::SetPos(pos);

	CScene::SetObjType(CScene::OBJTYPE_EFFECT);

	return S_OK;
}

//=========================================
// �I������
//=========================================
void CEffect::Uninit(void)
{
	CScene3DBill::Uninit();
}

//=========================================
// �X�V����
//=========================================
void CEffect::Update(void)
{
	int nLife;								// ���C�t���i�[
	float fLengthX, fLengthY;				// �傫���̍X�V�p

	fLengthX = CScene3DBill::GetLengthX();	// �傫�����擾
	fLengthY = CScene3DBill::GetLengthY();	// �傫�����擾

	if (m_type == TYPE_NORMAL)
	{// �ʏ�
		// ����������
		fLengthX -= 0.5f;
		fLengthY -= 0.5f;
	}
	else if (m_type == TYPE_RIPPLES)
	{// �g��
		// �傫������
		fLengthX += 1.0f;
		fLengthY += 1.0f;

		// �ʒu�����炷
		D3DXVECTOR3 pos = CScene3DBill::GetPos();
		pos.x -= 1.5f;
		CScene3DBill::SetPos(pos);
	}
	else if (m_type == TYPE_BIIM)
	{// �r�[��
		if (fLengthY < 30)
		{// ���܂ő傫������
			fLengthY += 5.0f;
		}

		for (int nCntScene = 0; nCntScene < MAX_OBJECT; nCntScene++)
		{// �G�̏��̒T��
			CScene3DModel *pScene3DModel;											// 3D�I�u�W�F�N�g�̃|�C���^�ϐ�
			pScene3DModel = (CScene3DModel*)GetScene(ENEMY_PRIORITY, nCntScene);	// �C���f�b�N�X���Ɏ擾

			if (pScene3DModel != NULL)
			{// null�`�F�b�N
				CScene::OBJTYPE objType;					// �I�u�W�F�N�g�̎�ޔ��ʗp�̕ϐ�
				objType = pScene3DModel->GetObjType();		// �I�u�W�F�N�g�̎�ނ��擾

				if (objType == CScene::OBJTYPE_ENEMY)
				{// �I�u�W�F�N�g�̎�ނ��G�̂Ƃ�
					CEnemy *pEnemy = (CEnemy*)pScene3DModel->GetScene(ENEMY_PRIORITY, nCntScene);	// �G�̏����擾;															// �G�̏����i�[
					D3DXVECTOR3 pos = pEnemy->GetPos();
					float fRange = pEnemy->GetLength();

					if (CScene3DBill::CollisionXY(pos, (fRange * 2.0f)))
					{// Y���̔���
						pEnemy->Uninit();
					}
				}
			}
		}

		// �ʒu�����炷
		D3DXVECTOR3 pos = CScene3DBill::GetPos();
		pos.x += 10.0f;
		CScene3DBill::SetPos(pos);
	}

	nLife = CScene3DBill::SubtractLife(1);			// ���C�t�����炷
	CScene3DBill::SetLength(fLengthX, fLengthY);	// �傫���̍X�V

	if (nLife <= 0 || fLengthX <= 0)
	{// ���C�t���Ȃ��Ȃ����Ƃ� || �傫����0�ȉ��ɂȂ����Ƃ�
		Uninit();		// �I������
	}
}

//=========================================
// �`�揈��
//=========================================
void CEffect::Draw(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// ���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// ���Ȃ��悤�ɂ���
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	// �`�揈��
	CScene3DBill::Draw();

	// Z�o�b�t�@�����ɖ߂�
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	// ���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}
