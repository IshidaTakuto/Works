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
#define MAX_OBJECT		(2000)		// �|���S���̍ő吔
#define NUM_PRIORITY	(8)			// �D�揇�ʂ̐�

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CScene
{
public:
	typedef enum
	{// ���
		TYPE_2D = 0,
		TYPE_MAX
	}TYPE;

	typedef enum
	{
		OBJTYPE_NONE = 0,		// �����l
		OBJTYPE_PLAYER,			// �v���C���[
		OBJTYPE_ENEMY,			// �G
		OBJTYPE_BULLET,			// �e
		OBJTYPE_FADE,			// �t�F�[�h
		OBJTYPE_FIELD,			// �n��
		OBJTYPE_CYLINDER,		// �V�����_�[
		OBJTYPE_EFFECT,			// �G�t�F�N�g
		OBJTYPE_MODEL,			// ���f��
		OBJTYPE_PARTICLE,		// �p�[�e�B�N��
		OBJTYPE_UI,				// �t�h
		OBJTYPE_WALL,			// ��
		OBJTYPE_SHADOW,			// �e
		OBJTYPE_MAX				// �I�u�W�F�N�g�̎�ނ̍ő吔
	}OBJTYPE;

	CScene(int nPriority = 3);
	virtual ~CScene();
	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	static void ReleaseAll(void);
	static void UpdateAll(void);
	static void DrawAll(void);

	void SetObjType(OBJTYPE objType) { m_objType = objType; };
	OBJTYPE GetObjType(void) { return m_objType; };
	static CScene *GetScene(int nPriority, int nIndex) { return m_apScene[nPriority][nIndex]; };

protected:
	void Release(void);

private:
	static CScene *m_apScene[NUM_PRIORITY][MAX_OBJECT];	// ���
	static int m_nNumAll;								// ����
	int	m_nIndex;										// �������g�̔ԍ�
	int m_nPriority;									// �D�揇��
	OBJTYPE m_objType;									// �I�u�W�F�N�g�̎��
};

#endif