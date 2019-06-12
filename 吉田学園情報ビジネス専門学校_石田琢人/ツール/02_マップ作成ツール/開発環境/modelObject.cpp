//=============================================================================
//
// 3D���f���I�u�W�F�N�g���� [modelObject.h]
// Author : Ishida Takuto
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "modelObject.h"
#include "input.h"
#include "manager.h"
#include "renderer.h"
#include "debugProc.h"
#include "meshField.h"
#include "enemy.h"
#include "setPlayer.h"

#include <stdio.h>

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MODEL_SAVE_FILENAME	"data/TEXT/MODEL/model_object_save.txt"	// �Z�[�u�p�e�L�X�g�t�@�C��

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9	*CModelObjectManager::m_pTexture[TYPE_MAX] = {};	// �e�N�X�`�����
LPD3DXMESH			*CModelObjectManager::m_pMesh[TYPE_MAX] = {};		// ���b�V�����
LPD3DXBUFFER		*CModelObjectManager::m_pBuffMat[TYPE_MAX] = {};	// �}�e���A�����
DWORD				*CModelObjectManager::m_pNumMat[TYPE_MAX] = {};		// �}�e���A����
int					CModelObjectManager::m_nNumType[TYPE_MAX] = {};		// ���f���̎��
int					CModelObjectManager::m_nNumAllModel = 0;			// ���f���̑���
char				**CModelObjectManager::m_apModelName[TYPE_MAX] = {};		// ���f���t�@�C����
bool				CModelObjectManager::m_bSave = false;

//=============================================================================
// ���f�����̓ǂݍ���
//=============================================================================
HRESULT CModelObjectManager::Load(void)
{
	// �z�u���̍폜
	CScene::LoadDelete();

	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	FILE *pFile;		// �t�@�C���̃|�C���^�ϐ�
	char aStr[128];		// �擾���镶����
	int nCntModel[TYPE_MAX] = {};	// ���f���̃J�E���g
	char *pFileName = CManager::GetSetFileName(0);

	// �t�@�C�����J��
	pFile = fopen(pFileName, "r");

	if (NULL != pFile)
	{// �t�@�C����������
		while (strcmp(aStr, "END_SCRIPT") != 0)
		{// �I�����b�Z�[�W�܂Ń��[�v
			fscanf(pFile, "%s", &aStr[0]);

			if (strcmp(aStr, "TYPE_MODEL") == 0)
			{// �I�u�W�F�N�g�̎�ނ����f���̂Ƃ�
				fscanf(pFile, " = %d", &m_nNumType[TYPE_MODEL]);

				// ���f���t�@�C�����ۑ��ϐ��̃������m��
				m_apModelName[TYPE_MODEL] = new char*[m_nNumType[TYPE_MODEL]];

				// ���f�����̃������m��
				NumObjectType(TYPE_MODEL);
			}
			if (strcmp(aStr, "TYPE_START") == 0)
			{// �I�u�W�F�N�g�̎�ނ��X�^�[�g�n�_�̂Ƃ�
				fscanf(pFile, " = %d", &m_nNumType[TYPE_START]);

				// ���f���t�@�C�����ۑ��ϐ��̃������m��
				m_apModelName[TYPE_START] = new char*[m_nNumType[TYPE_START]];

				// ���f�����̃������m��
				NumObjectType(TYPE_START);
			}
			if (strcmp(aStr, "TYPE_ENEMY") == 0)
			{// �I�u�W�F�N�g�̎�ނ��G�̂Ƃ�
				fscanf(pFile, " = %d", &m_nNumType[TYPE_ENEMY]);

				// ���f���t�@�C�����ۑ��ϐ��̃������m��
				m_apModelName[TYPE_ENEMY] = new char*[m_nNumType[TYPE_ENEMY]];

				// ���f�����̃������m��
				NumObjectType(TYPE_ENEMY);
			}
			if (strcmp(aStr, "TYPE_ITEM") == 0)
			{// �I�u�W�F�N�g�̎�ނ��A�C�e���̂Ƃ�
				fscanf(pFile, " = %d", &m_nNumType[TYPE_ITEM]);

				// ���f���t�@�C�����ۑ��ϐ��̃������m��
				m_apModelName[TYPE_ITEM] = new char*[m_nNumType[TYPE_ITEM]];

				// ���f�����̃������m��
				NumObjectType(TYPE_ITEM);
			}
			if (strcmp(aStr, "LOAD") == 0)
			{// �ǂݍ��ރ��f����
				while (strcmp(aStr, "END_LOAD") != 0)
				{// �I�����b�Z�[�W�܂Ń��[�v
					fscanf(pFile, "%s", &aStr[0]);

					if (strcmp(aStr, "OBJECT_TYPE") == 0)
					{// �ǂݍ��ރI�u�W�F�N�g�̎��
						fscanf(pFile, " = %s", &aStr[0]);
						TYPE type = TYPE_MODEL;

						// �e�I�u�W�F�N�g�̐����J�E���g
						if (strcmp(aStr, "TYPE_MODEL") == 0) { type = TYPE_MODEL; }
						if (strcmp(aStr, "TYPE_START") == 0) { type = TYPE_START; }
						if (strcmp(aStr, "TYPE_ENEMY") == 0) { type = TYPE_ENEMY; }
						if (strcmp(aStr, "TYPE_ITEM") == 0) { type = TYPE_ITEM; }

						while (1)
						{// ���[�v������
							fscanf(pFile, "%s", &aStr[0]);

							if (strcmp(aStr, "MODEL_FILENAME") == 0)
							{// �ǂݍ��ރ��f���t�@�C��
								fscanf(pFile, " = %s", &aStr[0]);

								// �K�v�ȕ��������m��
								m_apModelName[type][nCntModel[type]] = new char[strlen(&aStr[0])];
								strcpy(m_apModelName[type][nCntModel[type]], aStr);

								if (type != TYPE_ENEMY)
								{// ���f�����̍쐬
									CreateModelInfo(type, pDevice, &aStr[0], nCntModel[type]);
								}

								nCntModel[type]++;	// �I�u�W�F�N�g���̃J�E���g
								break;
							}
						}
					}
				}

				CEnemy::Load();		// �G�̃��f�����̍쐬
			}
			if (strcmp(aStr, "MODELSET") == 0)
			{// ���f���z�u���
				CModelObjectManager::TYPE type = CModelObjectManager::TYPE_MODEL;
				int nType = 0;	// ���f���ԍ�
				D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
				D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ����

				while (strcmp(aStr, "END_MODELSET") != 0)
				{// �I�����b�Z�[�W�܂Ń��[�v
					fscanf(pFile, "%s", &aStr[0]);

					if (strcmp(aStr, "OBJ") == 0)
					{// �I�u�W�F�N�g�̎��
						fscanf(pFile, " = %s", &aStr[0]);

						if (strcmp(aStr, "TYPE_MODEL") == 0)
						{// ��Q��
							type = TYPE_MODEL;
						}
						else if (strcmp(aStr, "TYPE_START") == 0)
						{// �X�^�[�g�n�_
							type = TYPE_START;
						}
						else if (strcmp(aStr, "TYPE_ENEMY") == 0)
						{// �G
							type = TYPE_ENEMY;
						}
						else if (strcmp(aStr, "TYPE_ITEM") == 0)
						{// �A�C�e��
							type = TYPE_ITEM;
						}
					}
					if (strcmp(aStr, "TYPE") == 0)
					{// ���f���ԍ�
						fscanf(pFile, " = %d", &nType);
					}
					if (strcmp(aStr, "POS") == 0)
					{// �ʒu
						fscanf(pFile, " = %f %f %f", &pos.x, &pos.y, &pos.z);
					}
					if (strcmp(aStr, "ROT") == 0)
					{// ����
						fscanf(pFile, " = %f %f %f", &rot.x, &rot.y, &rot.z);
					}
				}

				if (type != TYPE_ENEMY)
				{// �I�u�W�F�N�g�̐���
					CModelObject::Create(pos, rot, type, nType);
				}
				else if (type == TYPE_ENEMY)
				{// �G�̐���
					CEnemy::Create(pos, nType);
				}
			}
		}

		// �t�@�C�������
		fclose(pFile);
	}
	else
	{// �t�@�C�����J���Ȃ�
		MessageBox(0, "�t�@�C��������܂���I", "field.ini", MB_OK);
	}

	return S_OK;
}

//=============================================================================
// �I�u�W�F�N�g�̎�ނ��Ƃ̑������������m��
//=============================================================================
void CModelObjectManager::NumObjectType(TYPE type)
{
	m_pTexture[type] = new LPDIRECT3DTEXTURE9[m_nNumType[type]];
	for (int nCnt = 0; nCnt < m_nNumType[type]; nCnt++)
	{
		m_pTexture[type][nCnt] = NULL;
	}

	m_pMesh[type] = new LPD3DXMESH[m_nNumType[type]];
	for (int nCnt = 0; nCnt < m_nNumType[type]; nCnt++)
	{
		m_pMesh[type][nCnt] = NULL;
	}

	m_pBuffMat[type] = new LPD3DXBUFFER[m_nNumType[type]];
	for (int nCnt = 0; nCnt < m_nNumType[type]; nCnt++)
	{
		m_pBuffMat[type][nCnt] = NULL;
	}

	m_pNumMat[type] = new DWORD[m_nNumType[type]];
	for (int nCnt = 0; nCnt < m_nNumType[type]; nCnt++)
	{
		m_pNumMat[type][nCnt] = NULL;
	}

}

//=============================================================================
// ���f�����̍쐬
//=============================================================================
void CModelObjectManager::CreateModelInfo(TYPE type, LPDIRECT3DDEVICE9 pDevice, char *pModelFileName, int nCntModel)
{
	D3DXMATERIAL *pMat;				// �}�e���A���f�[�^�ւ̃|�C���^

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(pModelFileName,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat[type][nCntModel],
		NULL,
		&m_pNumMat[type][nCntModel],
		&m_pMesh[type][nCntModel]);

	// �e�N�X�`���̍쐬
	pMat = (D3DXMATERIAL*)m_pBuffMat[type][nCntModel]->GetBufferPointer();
	D3DXCreateTextureFromFile(pDevice, pMat->pTextureFilename, &m_pTexture[type][nCntModel]);
}

//=============================================================================
// �ǂݍ��񂾏��̉��
//=============================================================================
void CModelObjectManager::Unload(void)
{
	for (int nCntType = 0; nCntType < TYPE_MAX; nCntType++)
	{// �I�u�W�F�N�g�̎�ޕ����[�v
		for (int nCntNumType = 0; nCntNumType < m_nNumType[nCntType]; nCntNumType++)
		{
			if (NULL != m_pTexture[nCntType][nCntNumType])
			{// �e�N�X�`���̔j��
				m_pTexture[nCntType][nCntNumType]->Release();
				m_pTexture[nCntType][nCntNumType] = NULL;
			}

			if (NULL != m_pMesh[nCntType][nCntNumType])
			{// ���b�V�����̔j��
				m_pMesh[nCntType][nCntNumType]->Release();
				m_pMesh[nCntType][nCntNumType] = NULL;
			}

			if (NULL != m_pBuffMat[nCntType][nCntNumType])
			{// �}�e���A�����̔j��
				m_pBuffMat[nCntType][nCntNumType]->Release();
				m_pBuffMat[nCntType][nCntNumType] = NULL;
			}

			if (NULL != m_pNumMat[nCntType][nCntNumType])
			{// �}�e���A���̐���j��
				m_pNumMat[nCntType][nCntNumType] = NULL;
			}
		}
	}

	// ���f�����̏�����
	m_nNumAllModel = 0;
}

//=============================================================================
// ��������
//=============================================================================
CModelObjectManager *CModelObjectManager::Create(void)
{
	CModelObjectManager *pModelObjectManager = NULL;

	// �������m��
	pModelObjectManager = new CModelObjectManager;

	if (pModelObjectManager != NULL)
	{// �������m�ې���
		pModelObjectManager->Init();
	}

	return pModelObjectManager;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CModelObjectManager::CModelObjectManager(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CModelObjectManager::~CModelObjectManager()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CModelObjectManager::Init(void)
{
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CModelObjectManager::Uninit(void)
{
	// ������j��
	CScene::Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CModelObjectManager::Update(void)
{
	// �L�[�{�[�h�̓��͂��擾
	CInputKeyboard *pKeyboard = CManager::GetInput();

	if (m_bSave)
	{// �Z�[�u����
		FILE *pFile;

		// �t�@�C�����J��
		pFile = fopen(MODEL_SAVE_FILENAME, "w");

		if (NULL != pFile)
		{// �t�@�C����������
			fprintf(pFile, "#********************************************************************\n");
			fprintf(pFile, "#\n");
			fprintf(pFile, "#    ���f���z�u�X�N���v�g�t�@�C��[model_object_save.txt]\n");
			fprintf(pFile, "#    Auther : TAKUTO ISHIDA\n");
			fprintf(pFile, "#\n");
			fprintf(pFile, "#********************************************************************\n");

			fprintf(pFile, "SCRIPT\n\n");

			fprintf(pFile, "#------------------------------------------------------------------------------\n");
			fprintf(pFile, "# �\�����郂�f���̔z�u���\n");
			fprintf(pFile, "#------------------------------------------------------------------------------\n");

			// ���f���I�u�W�F�N�g��T��
			CScene *pScene = CScene::GetSceneTop(MODEL_PRIORITY);

			while (pScene != NULL)
			{// NULL�ɂȂ�܂Ń��[�v
				CScene *pSceneNext = pScene->GetSceneNext();
				CScene::OBJTYPE objType = pScene->GetObjType();

				if (CScene::OBJTYPE_SETOBJ == objType)
				{// ���f���̏ꍇ
					CModelObject *pModel = (CModelObject*)pScene;
					char *pType = pModel->GetTypeName();
					int nType = pModel->GetType();			// ��ނ̎擾
					D3DXVECTOR3 pos = pModel->GetPos();		// �ʒu�̎擾
					D3DXVECTOR3 rot = pModel->GetRot();		// �����̎擾

					fprintf(pFile, "MODELSET\n");
					fprintf(pFile, "	OBJ = %s		# �I�u�W�F�N�g�̎��\n", pType);
					fprintf(pFile, "	TYPE = %d				# ���\n", nType);
					fprintf(pFile, "	POS = %.1f %.1f %.1f		# �ʒu\n", pos.x, pos.y, pos.z);
					fprintf(pFile, "	ROT = %.1f %.1f %.1f		# ����\n", rot.x, rot.y, rot.z);
					fprintf(pFile, "END_MODELSET\n\n");
				}
				else if (CScene::OBJTYPE_ENEMY == objType)
				{// �G�̏ꍇ
					CEnemy *pEnemy = (CEnemy*)pScene;
					int nType = pEnemy->GetType();			// ��ނ̎擾
					D3DXVECTOR3 pos = pEnemy->GetPos();		// �ʒu�̎擾
					D3DXVECTOR3 rot = pEnemy->GetRot();		// �����̎擾

					fprintf(pFile, "MODELSET\n");
					fprintf(pFile, "	OBJ = TYPE_ENEMY		# �I�u�W�F�N�g�̎��\n");
					fprintf(pFile, "	TYPE = %d				# ���\n", nType);
					fprintf(pFile, "	POS = %.1f %.1f %.1f		# �ʒu\n", pos.x, pos.y, pos.z);
					fprintf(pFile, "	ROT = %.1f %.1f %.1f		# ����\n", rot.x, rot.y, rot.z);
					fprintf(pFile, "END_MODELSET\n\n");
				}

				// ���̃I�u�W�F�N�g������
				pScene = pSceneNext;
			}

			fprintf(pFile, "\nEND_SCRIPT		�� ��΂ɏ����Ȃ��I");

			// �t�@�C�������
			fclose(pFile);

			MessageBox(NULL, "�Z�[�u���܂����B", "data/TEXT/MODEL/model_object_save.txt", MB_OK);
		}
		else
		{// �t�@�C�����J���Ȃ�
			MessageBox(0, "�t�@�C��������܂���I\n�t�@�C�����쐬���܂��B", "field.txt", MB_OK);
		}

		m_bSave = false;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CModelObjectManager::Draw(void)
{
}

/*==================================================================================*/
/*																					*/
/*								���f���I�u�W�F�N�g�N���X							*/
/*																					*/
/*==================================================================================*/

//=============================================================================
// ��������
//=============================================================================
CModelObject* CModelObject::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModelObjectManager::TYPE type, int nType)
{
	CModelObject *pObject = NULL;

	pObject = new CModelObject;	// ���������m��

	if (pObject != NULL)
	{// �������m�ې���
		pObject->Init(pos, rot, type, nType);

		// ���f�����̐ݒ�
		CModelObjectManager::AddNumModel(1);
	}

	return pObject;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CModelObject::CModelObject(int nPriority, CScene::OBJTYPE objType) : CSceneX(nPriority, objType)
{
	m_nType = 0;
	m_pType = NULL;
	m_bRange = false;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CModelObject::~CModelObject()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CModelObject::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModelObjectManager::TYPE type, int nType)
{
	// ���f�����̎󂯓n��
	CSceneX::BindInfo(CModelObjectManager::GetTexture(type, nType), CModelObjectManager::GetMesh(type, nType), CModelObjectManager::GetBuffMat(type, nType), CModelObjectManager::GetNumMat(type, nType));

	// ����������
	CSceneX::Init();

	// �ʒu�̐ݒ�
	CSceneX::SetPos(pos);

	// �����̐ݒ�
	CSceneX::SetRot(rot);

	// ��ނ̕ۑ�
	m_nType = nType;

	if (type == CModelObjectManager::TYPE_MODEL)
	{// ���f��
		m_pType = new char[strlen("TYPE_MODEL")];
		strcpy(m_pType, "TYPE_MODEL");
	}
	else if (type == CModelObjectManager::TYPE_START)
	{// �X�^�[�g�n�_
		m_pType = new char[strlen("TYPE_START")];
		strcpy(m_pType, "TYPE_START");
	}
	else if (type == CModelObjectManager::TYPE_ENEMY)
	{// �G
		m_pType = new char[strlen("TYPE_ENEMY")];
		strcpy(m_pType, "TYPE_ENEMY");
	}
	else if (type == CModelObjectManager::TYPE_ITEM)
	{// �A�C�e��
		m_pType = new char[strlen("TYPE_ITEM")];
		strcpy(m_pType, "TYPE_ITEM");
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CModelObject::Uninit(void)
{
	// ������j��
	CSceneX::Uninit();

	// ���f�����̐ݒ�
	CModelObjectManager::AddNumModel(-1);
}

//=============================================================================
// �X�V����
//=============================================================================
void CModelObject::Update(void)
{
	D3DXVECTOR3 pos = CSceneX::GetPos();
	pos.y = CManager::GetMeshField()->CMeshField::GetHeight(pos);
	CSceneX::SetPos(pos);

	// �n�ʃI�u�W�F�N�g��T��
	CScene *pScene = CScene::GetSceneTop(SETPLAYER_PRIORITY);

	while (pScene != NULL)
	{// NULL�ɂȂ�܂Ń��[�v
		CScene *pSceneNext = pScene->GetSceneNext();
		CScene::OBJTYPE objType = pScene->GetObjType();

		if (objType == CScene::OBJTYPE_SETPLAYER)
		{// �z�u�p�v���C���[�������Ƃ�
			CSetPlayer *pSetPlayer = (CSetPlayer*)pScene;	// �n�ʂ̃|�C���^���擾

			m_bRange = pSetPlayer->Range(pos);			// �n�ʂ̍����̎擾
		}

		// ���̃I�u�W�F�N�g������
		pScene = pSceneNext;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CModelObject::Draw(void)
{
	if (m_bRange)
	{// �͈͓��̂Ƃ��F�����Z
		CSceneX::AddColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, -0.3f));
	}

	// �`�揈��
	CSceneX::Draw();

	if (m_bRange)
	{// �F��߂�
		CSceneX::AddColor(D3DXCOLOR(-1.0f, 0.0f, 0.0f, 0.3f));
	}
}

//=============================================================================
// �͈͓��̓G���폜
//=============================================================================
void CModelObject::Range(void)
{
	if (m_bRange)
	{// �͈͓��̂Ƃ��폜
		Uninit();
	}
}