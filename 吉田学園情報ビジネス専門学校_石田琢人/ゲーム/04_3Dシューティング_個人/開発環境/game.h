//=============================================================================
//
// �Q�[������ [game.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"
#include "enemy.h"
#include "field.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SCREEN_HALF			(SCREEN_WIDTH / 2)		// ��ʂ̉�����
#define MAX_ENEMY_APPEAR	(32)					// �G�̏o����

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CPlayer;
class CPause;
class CPolygon;
class CString;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CGame
{
public:
	typedef enum
	{
		STATE_NONE = 0,	// �������Ă��Ȃ����
		STATE_NORMAL,	// �ʏ���
		STATE_END,		// �Q�[���I�[�o�[
		STATE_CLEAR,	// �Q�[���N���A
		STATE_MAX
	}State;

	CGame();
	~CGame();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static void NullPlayer(void);	// �v���C���[�̍폜

	static const void SetGameState(State state);
	State GetGameState(void) { return m_state; };

	const static void SetPause(bool bPause) { m_bPause = bPause; };
	static bool GetPause(void) { return m_bPause; };

	const static void SetPlayer(CPlayer *pPlayer);
	static CPlayer *GetPlayer(void) { return m_pPlayer; };				// �v���C���[�̎擾
	static CField *GetField(int nIndex) { return m_pField[nIndex]; };	// �t�B�[���h�̎擾

private:
	static CPlayer	*m_pPlayer;				// �v���C���[�N���X�̃|�C���^�ϐ�
	static CField	*m_pField[MAX_FIELD];	// �t�B�[���h�N���X�̃|�C���^�ϐ�
	CPolygon *m_apPolygon;					// �|���S���N���X�̃|�C���^�ϐ�
	CString *m_pString;						// �����\���N���X�̃|�C���^�ϐ�
	CPause *m_pause;						// �|�[�Y�N���X�̃|�C���^�ϐ�
	static bool m_bPause;					// �|�[�YON/OFF�̐ؑ�
	static State m_state;					// �Q�[���̏��
	static int m_stateCounter;				// �J�ڂ���܂ł̃J�E���^
	int	m_nCntEnemyAppear;					// �G���o��������J�E���^
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �G�̏��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CEnemyInfo
{
public:
	CEnemyInfo();
	~CEnemyInfo();

	static HRESULT LoadEnemy(void);		// �G�̏���ǂݍ���

	static CEnemyInfo *GetInfo(int nInfo) { return &m_aEnemyInfo[nInfo]; };

	D3DXVECTOR3 GetPos(void) { return m_pos; };			// �ʒu
	int GetAppearTime(void) { return m_nAppearTime; };	// �o�����鎞��
	CEnemy::TYPE GeType(void) { return m_type; };		// ���
	bool GetUse(void) { return m_bUse; };
	void SetUse(bool bUse) { m_bUse = bUse; };

private:
	static CEnemyInfo m_aEnemyInfo[MAX_ENEMY];	// �G�̏��

	D3DXVECTOR3 m_pos;		// �ʒu
	int m_nAppearTime;		// �o�����鎞��
	CEnemy::TYPE m_type;	// ���
	bool m_bUse;

};

#endif