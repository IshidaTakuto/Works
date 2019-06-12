//=============================================================================
//
// �p�[�e�B�N������ [particle->h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "scene.h"
#include "scene3DBill.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_EMITTER			(16)			// �G�~�b�^�̍ő吔
#define MAX_EMITTER_TYPE	(32)			// �p�[�e�B�N���̎�ނ̍ő吔
#define MAX_TEXTURE			(32)			// �e�N�X�`���̍ő吔
#define PARTICLE_PRIORITY	(5)

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CParticle : public CScene
{
public:
	CParticle(int nPriority = 7);
	virtual ~CParticle();
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	static CParticle *Create(D3DXVECTOR3 pos, int nType);

private:

protected:
	D3DXVECTOR3 m_pos;				// �ʒu
	int m_nType;					// ���
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �p�[�e�B�N���̃G�~�b�^
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CParEmitter : public CParticle
{
public:
	CParEmitter();
	~CParEmitter();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Set(D3DXVECTOR3 pos, int nType);
	void SetPosParticleEmitter(int nIdx, D3DXVECTOR3 pos);
	void DestroyParticle(int nIdx);

private:
	int m_nCntAngle;		// �p�[�e�B�N���̊p�x�v�Z�p�J�E���^
	int m_nCntParticle;		// �o��������Ԋu�̃J�E���^

	int			m_nLife;			// �G�~�b�^�̎���
	int			m_nTexture;			// �e�N�X�`���̎��
	int			m_nAppear;			// �o��Ԋu
	int			m_nLap;				// 1���ɏo����
	bool		m_bLoop;			// ���[�v�̗L��
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �p�[�e�B�N���̈ꖇ
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CParticlePolygon : public CScene3DBill
{
public:
	CParticlePolygon(int nPriority = PARTICLE_PRIORITY);
	~CParticlePolygon();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CParticlePolygon *Create(D3DXVECTOR3 pos, int nType, int nTexture);

	static HRESULT Load(void);		// �e�N�X�`����ǂݍ���
	static void Unload(void);		// �ǂݍ��񂾃e�N�X�`�������

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[MAX_TEXTURE];	// ���L�e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffParticle;	// ���_�o�b�t�@�ւ̃|�C���^

	int m_nNumParticleTexture;		// �e�N�X�`���̐�

	D3DXVECTOR3 m_move;		// �ړ���
	D3DXCOLOR m_col;		// �G�~�b�^�̈ʒu
	float m_fLength;		// ���_�̋���
	float m_fCntRot;		// �p�x�̍X�V
	float m_fRotAngle;		// ��]����Ƃ��̊p�x
	float m_fAngle;			// �p�x
	float m_fRadius;		// �傫��
	int m_nLife;			// �\������
	int m_nType;			// �G�~�b�^�̎��
	float m_fGravity;		// �d��
	float m_fBouncing;		// �o�E���h����l
	bool m_bDraw;			// ���Z�������ǂ���
	int m_nCntAngle;		// �p�x�v�Z�p�ɕK�v
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �p�[�e�B�N���̃f�[�^
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CParData
{
public:
	CParData();
	~CParData();
	HRESULT Init(void);
	void Uninit(void);

	static HRESULT Load(void);		// �f�[�^��ǂݍ���
	static void Unload(void);		// �ǂݍ��񂾃f�[�^�����

	static D3DXCOLOR GetFlameColor(int nIdx) { return m_apParticleData[nIdx].m_flameCol; };
	static D3DXCOLOR GetColor(int nIdx) { return m_apParticleData[nIdx].m_col; };
	static float GetFlameRadius(int nIdx) { return m_apParticleData[nIdx].m_fFlameRadius; };
	static bool GetBouncing(int nIdx) { return m_apParticleData[nIdx].m_bBouncing; };
	static int GetRotType(int nIdx) { return m_apParticleData[nIdx].m_nRot; };
	static float GetRotSpeed(int nIdx) { return m_apParticleData[nIdx].m_fRotSpeed; };
	static int GetLap(int nIdx) { return m_apParticleData[nIdx].m_nLap; };
	static int *GetParLife(int nIdx) { return &m_apParticleData[nIdx].m_nParLife[0]; };
	static int *GetRadius(int nIdx) { return &m_apParticleData[nIdx].m_nRadius[0]; };
	static int *GetWidth(int nIdx) { return &m_apParticleData[nIdx].m_nWidth[0]; };
	static int *GetHeight(int nIdx) { return &m_apParticleData[nIdx].m_nHeight[0]; };
	static int *GetDepth(int nIdx) { return &m_apParticleData[nIdx].m_nDepth[0]; };
	static float GetSpread(int nIdx) { return m_apParticleData[nIdx].m_fSpread; };
	static bool GetDraw(int nIdx) { return m_apParticleData[nIdx].m_bDraw; };
	static float GetGravity(int nIdx) { return m_apParticleData[nIdx].m_fGravity; };
	static int GetAppear(int nIdx) { return m_apParticleData[nIdx].m_nAppear; };
	static int GetLife(int nIdx) { return m_apParticleData[nIdx].m_nLife; };
	static int GetTextureIdx(int nIdx) { return m_apParticleData[nIdx].m_nTexture; };

private:
	static CParData m_apParticleData[MAX_EMITTER_TYPE];

	int m_nWidth[2];		// ��(�ŏ��l�ƍő�l)
	int m_nHeight[2];		// ����(�ŏ��l�ƍő�l)
	int m_nDepth[2];		// ���s(�ŏ��l�ƍő�l)
	D3DXCOLOR m_col;		// �F
	D3DXCOLOR m_flameCol;	// ���t���[���̐F�̕ω�
	int m_nRadius[2];		// �傫��(�ŏ��l�ƍő�l)
	float m_fFlameRadius;	// ���t���[���̔��a�̊g�k
	int m_nLife;			// �G�~�b�^�̎���
	int m_nParLife[2];		// �\������(�ŏ��l�ƍő�l)
	int m_nTexture;			// �e�N�X�`���̎��
	float m_fSpread;		// �L����
	int m_nAppear;			// �o��Ԋu
	float m_fGravity;		// �d��
	int m_nLap;				// 1���ɏo����
	int m_nRot;				// ��]���邩�ǂ���(0:�Ȃ��@1:���v���@2:�����v��� 3:�����_��)
	float m_fRotSpeed;		// ��]���鑬�x
	bool m_bLoop;			// ���[�v�����邩 0:�Ȃ� 1:����
	bool m_bBouncing;		// �o�E���h�����邩 0:�Ȃ� 1:����
	bool m_bDraw;			// ���Z�������ǂ���
};

#endif
