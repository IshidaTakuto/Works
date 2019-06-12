//=============================================================================
//
// �I�u�W�F�N�g���� [scene.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _SCENE_H_
#define _SCENE_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_OBJECT		(128)		// �|���S���̍ő吔
#define NUM_PRIORITY	(8)			// �D�揇�ʂ̐�

//=====================================
// �N���X��`
//=====================================
class CScene
{
public:
	typedef enum
	{
		OBJTYPE_NONE = 0,		// �����l
		OBJTYPE_PLAYER,			// �v���C���[
		OBJTYPE_MODEL,			// ���f��
		OBJTYPE_FIELD,			// �t�B�[���h
		OBJTYPE_BULLET,			// �e
		OBJTYPE_SHADOW,			// �e
		OBJTYPE_CYLINDER,		// �V�����_�[
		OBJTYPE_MAX				// �I�u�W�F�N�g�̎�ނ̍ő吔
	}OBJTYPE;

	CScene(int nPriority = 3, OBJTYPE objType = OBJTYPE_NONE);
	virtual ~CScene();
	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	static void ReleaseAll(void);
	static void UpdateAll(void);
	static void DrawAll(void);
	static void ReleaseDeath(void);

	void SwapPriority(int nPriority);

	void SetObjType(OBJTYPE objType) { m_objType = objType; };
	OBJTYPE GetObjType(void) { return m_objType; };

	//CScene *GetScene(int nIdx) {};

protected:
	void Release(void);

private:
	static CScene *m_apTop[NUM_PRIORITY];		// �擪�̃I�u�W�F�N�g�ւ̃|�C���^
	static CScene *m_apCur[NUM_PRIORITY];		// ���݂̃I�u�W�F�N�g�ւ̃|�C���^�i�Ō���j
	CScene *m_pPrev;			// �O�̃I�u�W�F�N�g�ւ̃|�C���^
	CScene *m_pNext;			// ���̃I�u�W�F�N�g�ւ̃|�C���^
	int m_nPriority;			// �D�揇��
	static int m_nNumAll;		// ����
	OBJTYPE m_objType;			// �I�u�W�F�N�g�̎��
	bool m_bDeath;				// ���S�t���O
	int m_nID;					// �f�o�b�O�p�����̔ԍ�
};

#endif