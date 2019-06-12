//=============================================================================
//
// 3D���f���I�u�W�F�N�g���� [modelObject.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _MODELOBJECT_H_
#define _MODELOBJECT_H_

#include "scene.h"
#include "sceneX.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MODEL_PRIORITY	(4)

//*****************************************************************************
// �O���錾
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CModelObjectManager : public CScene
{
public:
	typedef enum
	{
		TYPE_MODEL = 0,		// ���f��
		TYPE_START,			// �X�^�[�g�n�_
		TYPE_ENEMY,			// �G
		TYPE_ITEM,			// �A�C�e��
		TYPE_MAX
	}TYPE;

	CModelObjectManager(int nPriority = 0, CScene::OBJTYPE objType = CScene::OBJTYPE_OBJECTMANAGER);
	~CModelObjectManager();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static HRESULT Load(void);				// �e�N�X�`����ǂݍ���
	static void Unload(void);				// �ǂݍ��񂾃e�N�X�`�������

	static CModelObjectManager *Create(void);

	static LPDIRECT3DTEXTURE9	GetTexture(TYPE type, int nType) { return m_pTexture[type][nType]; };		// �e�N�X�`�����̎擾
	static LPD3DXMESH			GetMesh(TYPE type, int nType) { return m_pMesh[type][nType]; };				// ���b�V�����̎擾
	static LPD3DXBUFFER			GetBuffMat(TYPE type, int nType) { return m_pBuffMat[type][nType]; };		// �}�e���A�����̎擾
	static DWORD				GetNumMat(TYPE type, int nType) { return m_pNumMat[type][nType]; };			// �}�e���A�����̎擾

	static int					GetNumModel(void) { return m_nNumAllModel; };					// ���f�����̎擾
	static void					SetNumModel(int nNumModel) { m_nNumAllModel = nNumModel; };	// ���f�����̐ݒ�
	static void					AddNumModel(int nValue) { m_nNumAllModel += nValue; };			// ���f�����̉��Z

	static int					GetNumType(TYPE type) { return m_nNumType[type]; };					// ��ނ̐��̎擾

	static char					**&GetModelFileName(TYPE type) { return m_apModelName[type]; };

	static void Save(void) { m_bSave = m_bSave ? false : true; };	// ���̕ۑ�

protected:

private:
	static void NumObjectType(TYPE type);					// ���f�������쐬���邽�߂̃������m��
	static void CreateModelInfo(TYPE type, LPDIRECT3DDEVICE9 pDevice, char *pModelFileName, int nCntModel);	// ���f�����̍쐬

	static LPDIRECT3DTEXTURE9	*m_pTexture[TYPE_MAX];		// �e�N�X�`���ւ̃|�C���^
	static LPD3DXMESH			*m_pMesh[TYPE_MAX];			// ���b�V�����ւ̃|�C���^
	static LPD3DXBUFFER			*m_pBuffMat[TYPE_MAX];		// �}�e���A�����ւ̃|�C���^
	static DWORD				*m_pNumMat[TYPE_MAX];		// �}�e���A�����̐�

	static int					m_nNumType[TYPE_MAX];		// �ݒu���f���̎�ނ̐�
	static int					m_nNumAllModel;				// ���f���̍��v��
	static char					**m_apModelName[TYPE_MAX];	// ���f���̖��O
	static bool					m_bSave;
};

/*========================*/
/*  ��Q���N���X  */
/*========================*/
class CModelObject : public CSceneX
{
public:
	CModelObject(int nPriority = MODEL_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_SETOBJ);
	~CModelObject();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModelObjectManager::TYPE type, int nObject);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CModelObject *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModelObjectManager::TYPE type, int nObject);

	int GetType(void) { return m_nType; };
	char *GetTypeName(void) { return m_pType; };

	void Range(void);

protected:

private:
	int m_nType;
	char *m_pType;
	bool m_bRange;		// �͈͓����ǂ���
};

#endif