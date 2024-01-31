//==============================================
//
// �n�[�t�X�t�B�A.h (halfsphere.h)
// Author : Saito Shian
//
//==============================================
#ifndef  _HALFSPHERE_H_				//���̃}�N����`������ĂȂ�������
#define  _HALFSPHERE_H_				//��d�C���N���[�h�h�~�̃}�N����`

//==============================================
// �C���N���[�h
//==============================================
#include "object.h"
#include <vector>

//==============================================
//�}�N����`
//==============================================
#define HALFSPHEAR_X_BLOCK	(25)	//X�����̃u���b�N��
#define HALFSPHEAR_Z_BLOCK	(25)	//Z�����̃u���b�N��

#define MAX_SPHERE	(2)

#define HALFSPHEAR_VERTEX_NUM	((HALFSPHEAR_X_BLOCK + 1) * (HALFSPHEAR_Z_BLOCK + 1))
#define HALFSPHEAR_INDEXNUM		((HALFSPHEAR_X_BLOCK + 1) * 2 * HALFSPHEAR_Z_BLOCK + (HALFSPHEAR_Z_BLOCK - 1) * 2)
#define HALFSPHEAR_PRIMITIVE_NUM	(HALFSPHEAR_X_BLOCK * (HALFSPHEAR_Z_BLOCK * 2) + (HALFSPHEAR_Z_BLOCK - 1) * 4)

//==============================================
// �n�[�t�X�t�B�A�N���X
//==============================================
class CHalfSphere : public CObject
{
public:

	// 3D���_�t�H�[�}�b�g
	const DWORD FVF_VERTEX_3D = (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1);

	//****************
	// �񋓌^
	//****************
	enum SPHERE_TEX
	{
		SPHERE_UP = 0,
		SPHERE_DOWN,
		SPHERE_MAX
	};

	//****************
	// public�Ȋ֐�
	//****************
	CHalfSphere();				// �R���X�g���N�^
	~CHalfSphere() override;							// �f�X�g���N�^

	HRESULT Init(void) override;						// ����������
	void Uninit(void) override;							// �I������
	void Update(void) override;							// �X�V����
	void Draw(void) override;							// �`�揈��

	void SetPos(const D3DXVECTOR3 pos);					// �ʒu�̐ݒ菈��
	void SetSize(const D3DXVECTOR3 size);				// �T�C�Y�̐ݒ�
	void SetRot(const D3DXVECTOR3 rot);					// �����̐ݒ�
	void SetTex(const SPHERE_TEX tex);					// 

	const D3DXVECTOR2 GetSize(void);					// �T�C�Y�̎擾����
	const D3DXVECTOR3 GetPos(void);						// �ʒu�̎擾����
	const D3DXVECTOR3 GetRot(void);

	void AddPos(D3DXVECTOR3 pos) { m_pos += pos; }
	void AddRot(D3DXVECTOR3 rot) { m_rot += rot; }

	static CHalfSphere* Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot, SPHERE_TEX tex);		// ��������

	void SetAnimation(D3DXVECTOR2 animationMove) { m_bAnimation = true; m_AnimationMove = animationMove; }
	void SetRotMove(float fRotMove) { m_bRot = true; m_fRotMove = fRotMove; }

	void BindTexture(LPDIRECT3DTEXTURE9	pTexture);		// �e�N�X�`������
	void LoadTexture(const char *aFileName);			// �e�N�X�`���̓ǂݍ���

private:
	//****************
	// �����o�[�ϐ��Q
	//****************
	struct VERTEX_3D
	{
		D3DXVECTOR3 pos;		//���_���W
		D3DXVECTOR3 nor;		//�@���x�N�g��
		D3DCOLOR col;			//���_�J���[
		D3DXVECTOR2 tex;		//�e�N�X�`�����W
	};

	LPDIRECT3DTEXTURE9 m_pTexture;			// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;		// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3 m_pos;						// �ʒu
	D3DXVECTOR3 m_size;						// �T�C�Y
	D3DXVECTOR3 m_rot;						// ����
	D3DXMATRIX m_mtxWorld;					// ���[���h�}�g���b�N�X
	D3DXVECTOR2 m_AnimationMove;
	D3DXVECTOR2 m_Animation;
	bool m_bAnimation;
	bool m_bRot;
	float m_fRotMove;
	float m_fRot;

	SPHERE_TEX m_tex;						// �e�N�X�`��
};

#endif
