//=============================================================================
//
// �e���� [shadow.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#include "shadow.h"
#include "manager.h"
#include "renderer.h"
#include "light.h"
#include "meshField.h"
#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//==================================
// ��������
//==================================
CShadow *CShadow::Create(void)
{
	CShadow *pShadow;

	pShadow = new CShadow;	// ���������m��

	if (pShadow != NULL)
	{// �������m�ې���
		pShadow->Init();
	}

	return pShadow;
}

//=========================================
// �R���X�g���N�^
//=========================================
CShadow::CShadow(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
}

//=========================================
// �f�X�g���N�^
//=========================================
CShadow::~CShadow()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CShadow::Init(void)
{
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CShadow::Uninit(void)
{
	// �I�u�W�F�N�g�̔j��
	CScene::Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CShadow::Update(void)
{
}

//=============================================================================
// �`�揈��
//=============================================================================
void CShadow::Draw(void)
{
}

//=============================================================================
// ���f���ɉe������
//=============================================================================
void CShadow::Set(D3DXMATRIX mtxWorld, LPD3DXMESH mesh, D3DXVECTOR3 posObj)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxShadow;		// �V���h�E�}�g���b�N�X
	D3DXPLANE planeField;		// �n�ʂ̕���
	D3DXVECTOR4 vecLight;		// ���C�g�̃x�N�g��
	D3DXVECTOR3 pos, normal;	// �ʒu�Ɩ@��

	// ���Ȃ��悤�ɂ���
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	// ���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// �V���h�E�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxShadow);

	// ���C�g�̃x�N�g���̐ݒ�
	D3DXVECTOR3 vecDir = CLight::GetDirection();
	vecLight = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 0.0f);

	// �n�ʃI�u�W�F�N�g��T��
	CScene *pScene = CScene::GetSceneTop(MESHFIELD_PRIORITY);
	float fHeight = 0.0f;

	while (pScene != NULL)
	{// NULL�ɂȂ�܂Ń��[�v
		CScene *pSceneNext = pScene->GetSceneNext();
		CScene::OBJTYPE objType = pScene->GetObjType();

		if (objType == CScene::OBJTYPE_FIELD)
		{// �n�ʂ������Ƃ�
			CMeshField *pMeshField = (CMeshField*)pScene;

			fHeight = pMeshField->GetHeight(posObj);	// �n�ʂ̍����̎擾
		}

		// ���̃I�u�W�F�N�g������
		pScene = pSceneNext;
	}

	// �e�̈ʒu
	pos = D3DXVECTOR3(0.0f, fHeight + 1.0f, 0.0f);

	// �e�̖@��
	normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// �_�Ɩ@�����畽�ʂ��쐬����
	D3DXPlaneFromPointNormal(&planeField, &pos, &normal);

	// ���ʂɎˉe����}�g���b�N�X���쐬
	D3DXMatrixShadow(&mtxShadow, &vecLight, &planeField);

	// ���[���h�}�g���b�N�X�Ɗ|�����킹��
	D3DXMatrixMultiply(&mtxShadow, &mtxWorld, &mtxShadow);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

	// �e�N�X�`����NULL�ɂ���
	pDevice->SetTexture(0, NULL);

	// �e�̕`��
	mesh->DrawSubset(0);

	// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// ���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// ���Ȃ��悤�ɂ���
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
}