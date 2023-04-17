//============================================================================
//
//���b�V���|���S���̏���[meshfield.cpp] (�t�@�C���w�b�_�R�����g)
//Author:�J�{�N��
//============================================================================
#include "meshfield.h"	//�쐬����meshfield.h���C���N���[�h����
#include "input.h"		//�쐬����input.h���C���N���[�h����
#include "player.h"		//�쐬����player.h���C���N���[�h����

//****************************************************************************
//�O���[�o���ϐ�
//****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureMeshField = NULL;	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshField = NULL;	//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshField = NULL;	//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
MESHFIELD g_aMeshField;								//���b�V���̏��
int g_nMeshPlimitive = 1024;
int g_nMeshVertex = 1024;
int g_nMeshIndex;

//============================================================================
//���b�V���|���S���̏���������
//============================================================================
void InitMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�ւ̃|�C���^
							
	int g_nMeshIndex = (((MAX_MESHFIELD + 1) * 2) * MAX_MESHFIELD) + ((MAX_MESHFIELD - 1) * 2);

	pDevice->CreateVertexBuffer(				//���_�o�b�t�@�̐���
		sizeof(VERTEX_3D) * g_nMeshVertex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshField,
		NULL);

	D3DXCreateTextureFromFile(					//�e�N�X�`���[�̓ǂݍ���
		pDevice,
		"data\\TEXTURE\\field000.jpg",			//�e�N�X�`���̃t�@�C����
		&g_pTextureMeshField
	);

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * g_nMeshIndex,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshField,
		NULL);

	//�ʒu�̏���������
	g_aMeshField.nMeshX = MAX_MESHFIELD;
	g_aMeshField.nMeshZ = MAX_MESHFIELD;
	g_aMeshField.pos = D3DXVECTOR3(0.0f, -2.0f, 0.0f);
	g_aMeshField.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aMeshField.nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//����������������
	g_aMeshField.col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	g_aMeshField.fAngle = atan2f(MESHFIELD_WIDTH, MESHFIELD_HEIGHT);
	g_aMeshField.fLength = sqrtf(MESHFIELD_WIDTH * MESHFIELD_WIDTH + MESHFIELD_HEIGHT * MESHFIELD_HEIGHT) / 2.0f;
	g_aMeshField.Worldmtx = D3DXMATRIX();

	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx = NULL;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffMeshField->Lock(0, 0, (void**)&pVtx, 0);

	//�e���_�̖@���̐ݒ�(���x�N�g���̑傫����1�ɂ���K�v������)
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`�����W�̐ݒ�(0.0f ~ (1 / x�p�^�[����)f)
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffMeshField->Unlock();
}

//============================================================================
//���b�V���|���S���̏I������
//============================================================================
void UninitMeshField(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureMeshField != NULL)
	{
		g_pTextureMeshField->Release();
		g_pTextureMeshField = NULL;

	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffMeshField != NULL)
	{
		g_pVtxBuffMeshField->Release();
		g_pVtxBuffMeshField = NULL;
	}
	//�C���f�b�N�X�o�b�t�@�̉��
	if (g_pIdxBuffMeshField != NULL)
	{
		g_pIdxBuffMeshField->Release();
		g_pIdxBuffMeshField = NULL;
	}	
}

//============================================================================
//���b�V���|���S���̍X�V����
//============================================================================
void UpdateMeshField(void)
{
	int nCntMeshX;
	int nCntMeshZ;
	int nDeg = 0;		//�k�ރ|���S��

	//if (GetKeyboardTrigger(DIK_O) == true)
	//{
	//	if (g_aMeshField.nMeshX < MAX_MESHFIELD)
	//	{
	//		g_aMeshField.nMeshX++;
	//		g_aMeshField.nMeshZ++;
	//	}
	//}
	//else if(GetKeyboardTrigger(DIK_P) == true)
	//{
	//	if (g_aMeshField.nMeshX > 1)
	//	{
	//		g_aMeshField.nMeshX--;
	//		g_aMeshField.nMeshZ--;
	//	}
	//}

	//�|���S�����v�Z�� = ((�|���S����.x * �|���S����.z) * 2) + ((�|���S����.z - 1) * 4)
	g_nMeshPlimitive = ((g_aMeshField.nMeshX * g_aMeshField.nMeshZ) * 2) + ((g_aMeshField.nMeshZ - 1) * 4);

	//�C���f�b�N�X���v�Z�� = (((�|���S����.x + 1) * 2) * �|���S����.z) + ((�|���S����.y - 1) * 2)
	g_nMeshIndex = (((g_aMeshField.nMeshX + 1) * 2) * g_aMeshField.nMeshZ) + ((g_aMeshField.nMeshZ - 1) * 2);	

	//���_���v�Z�� = (�|���S����.x + 1) * (�|���S����.z + 1)
	g_nMeshVertex = (g_aMeshField.nMeshX + 1) * (g_aMeshField.nMeshZ + 1);								
																					
	VERTEX_3D *pVtx = NULL;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffMeshField->Lock(0, 0, (void**)&pVtx, 0);

	//�w�肳�ꂽ�|���S�������A���_��z�u���鏈��
	for (nCntMeshZ = 0; nCntMeshZ < g_aMeshField.nMeshZ + 1; nCntMeshZ++)
	{
		for (nCntMeshX = 0; nCntMeshX < g_aMeshField.nMeshX + 1; nCntMeshX++)
		{
			pVtx[nCntMeshX + (nCntMeshZ * (g_aMeshField.nMeshX + 1))].pos = 
				D3DXVECTOR3((nCntMeshX * MESHFIELD_WIDTH) - (g_aMeshField.nMeshX * MESHFIELD_WIDTH / 2),	//(���J�E���g����) - (�|���S��������)
				0.0f,
				-(nCntMeshZ * MESHFIELD_HEIGHT) + (g_aMeshField.nMeshZ * MESHFIELD_HEIGHT / 2)				//(���J�E���g����) + (�|���S��������)
				);
			
			//�e���_�̖@���̐ݒ�(���x�N�g���̑傫����1�ɂ���K�v������)
			pVtx[nCntMeshX + (nCntMeshZ * (g_aMeshField.nMeshX + 1))].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//���_�J���[�̐ݒ�
			pVtx[nCntMeshX + (nCntMeshZ * (g_aMeshField.nMeshX + 1))].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//�e�N�X�`�����W�̐ݒ�(0.0f ~ (1 / x�p�^�[����)f)
			pVtx[nCntMeshX + (nCntMeshZ * (g_aMeshField.nMeshX + 1))].tex = D3DXVECTOR2(nCntMeshX , nCntMeshZ );
		}
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffMeshField->Unlock();

	//�C���f�b�N�X�o�b�t�@�����b�N
	WORD* pIdx;
	g_pIdxBuffMeshField->Lock(0, 0, (void**)&pIdx, 0);

	//�C���f�b�N�X�̐ݒ�
	for (nCntMeshZ = 0; nCntMeshZ < g_aMeshField.nMeshZ; nCntMeshZ++)
	{//z���J�E���g
		if(nCntMeshZ != 0)
		{//�k�ރ|���S�����K�v�ȏꍇ
			pIdx[(((g_aMeshField.nMeshX + 1) * 2) * nCntMeshZ) + nDeg] = ((g_aMeshField.nMeshX + 1) * nCntMeshZ) - 1;		//pIdx[���Ō�̃C���f�b�N�X] = x,z���v�Z
			pIdx[(((g_aMeshField.nMeshX + 1) * 2) * nCntMeshZ) + nDeg + 1] = (g_aMeshField.nMeshX + 1) * (nCntMeshZ + 1);	//pIdx[���Ō�̃C���f�b�N�X + 1] = x,z���v�Z
			nDeg += 2;
		}
		//x���J�E���g
		for (nCntMeshX = 0; nCntMeshX < (g_aMeshField.nMeshX + 1); nCntMeshX++)
		{
			//pIdx[(���J�E���gx�� * 2) + ((�����镪��x�� * 2) * ���J�E���gz��) + �k�ރ|���S��]
			pIdx[(nCntMeshX * 2) + (((g_aMeshField.nMeshX + 1) * 2) * nCntMeshZ) + nDeg]	
				// = (�����镪��x��) + ���J�E���gx�� + ((�����镪��x��) * ���J�E���gz��)
				= (g_aMeshField.nMeshX + 1) + nCntMeshX + ((g_aMeshField.nMeshX + 1) * nCntMeshZ);

			//pIdx[���+1]
			pIdx[(nCntMeshX * 2) + (((g_aMeshField.nMeshX + 1) * 2) * nCntMeshZ) + nDeg + 1]
				// = ���J�E���gx�� + ((�����镪��x��) * ���J�E���gz��)
				= nCntMeshX + ((g_aMeshField.nMeshX + 1) * nCntMeshZ);
		}
	}
	//�C���f�b�N�X�̃A�����b�N
	g_pIdxBuffMeshField->Unlock();
}

//============================================================================
//���b�V���|���S���̕`�揈��
//============================================================================
void DrawMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;			//�v�Z�p�}�g���b�N�X

											//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_aMeshField.Worldmtx);
	//�����𔽉f(YaW : y,Pitch : x,Roll : z)
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMeshField.rot.y, g_aMeshField.rot.x, g_aMeshField.rot.z);
	D3DXMatrixMultiply(&g_aMeshField.Worldmtx, &g_aMeshField.Worldmtx, &mtxRot);
	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_aMeshField.pos.x, g_aMeshField.pos.y, g_aMeshField.pos.z);
	D3DXMatrixMultiply(&g_aMeshField.Worldmtx, &g_aMeshField.Worldmtx, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_aMeshField.Worldmtx);

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffMeshField, 0, sizeof(VERTEX_3D));

	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(g_pIdxBuffMeshField);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureMeshField);

	//�|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0,0, g_nMeshVertex,0, g_nMeshPlimitive);

	//�e�N�X�`���̏I��
	pDevice->SetTexture(0, NULL);
}

//============================================================================
//���b�V���|���S���̓����蔻�菈��
//============================================================================
bool FieldCollision(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move)
{
	bool bIsGround = false;
	PLAYER *pPlayer = GetPlayer();

	if (pos->x < g_aMeshField.pos.x + (g_aMeshField.nMeshX * (MESHFIELD_WIDTH / 2))
		&& pos->x > g_aMeshField.pos.x - (g_aMeshField.nMeshX * (MESHFIELD_WIDTH / 2))
		&& pos->z < g_aMeshField.pos.z + (g_aMeshField.nMeshZ * (MESHFIELD_HEIGHT / 2))
		&& pos->z > g_aMeshField.pos.z - (g_aMeshField.nMeshZ * (MESHFIELD_HEIGHT / 2)))
	{//���ɏ���Ă���ꍇ
		if (pos->y <= g_aMeshField.pos.y && posOld->y >= g_aMeshField.pos.y)
		{//���������b�V����荂���ꍇ
			pos->y = g_aMeshField.pos.y;
			move->y = 0;
			bIsGround = true;
		}
	}
	return bIsGround;
}
