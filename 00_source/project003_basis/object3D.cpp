//============================================================
//
//	�I�u�W�F�N�g3D���� [object3D.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object3D.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "collision.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int MAX_VERTEX = 4;	// ���_��
}

//************************************************************
//	�q�N���X [CObject3D] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CObject3D::CObject3D(const CObject::ELabel label, const CObject::EDim dimension, const int nPriority) : CObject(label, dimension, nPriority),
	m_pVtxBuff		(nullptr),			// ���_�o�b�t�@�ւ̃|�C���^
	m_pRenderState	(nullptr),			// �����_�[�X�e�[�g�̏��
	m_pPosGapBuff	(nullptr),			// ���W�̂���o�b�t�@
	m_pos			(VEC3_ZERO),		// �ʒu
	m_rot			(VEC3_ZERO),		// ����
	m_size			(VEC3_ZERO),		// �傫��
	m_col			(XCOL_WHITE),		// �F
	m_origin		(ORIGIN_CENTER),	// ���_
	m_nTextureID	(0)					// �e�N�X�`���C���f�b�N�X
{
	// �����o�ϐ����N���A
	memset(&m_mtxWorld, 0, sizeof(m_mtxWorld));	// ���[���h�}�g���b�N�X
}

//============================================================
//	�f�X�g���N�^
//============================================================
CObject3D::~CObject3D()
{

}

//============================================================
//	����������
//============================================================
HRESULT CObject3D::Init(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// �f�o�C�X�̃|�C���^

	// �����o�ϐ���������
	memset(&m_mtxWorld, 0, sizeof(m_mtxWorld));	// ���[���h�}�g���b�N�X
	m_pVtxBuff		= nullptr;			// ���_�o�b�t�@�ւ̃|�C���^
	m_pRenderState	= nullptr;			// �����_�[�X�e�[�g�̏��
	m_pPosGapBuff	= nullptr;			// ���W�̂���o�b�t�@
	m_pos			= VEC3_ZERO;		// �ʒu
	m_rot			= VEC3_ZERO;		// ����
	m_size			= VEC3_ZERO;		// �傫��
	m_col			= XCOL_WHITE;		// �F
	m_origin		= ORIGIN_CENTER;	// ���_
	m_nTextureID	= NONE_IDX;			// �e�N�X�`���C���f�b�N�X

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer
	( // ����
		sizeof(VERTEX_3D) * MAX_VERTEX,	// �K�v���_��
		D3DUSAGE_WRITEONLY,				// �g�p���@
		object::FVF_VERTEX_3D,			// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,				// �������̎w��
		&m_pVtxBuff,					// ���_�o�b�t�@�ւ̃|�C���^
		nullptr
	)))
	{ // ���_�o�b�t�@�̐����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �����_�[�X�e�[�g�̐���
	m_pRenderState = CRenderState::Create();
	if (m_pRenderState == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ���W�̂���o�b�t�@�̃������m��
	m_pPosGapBuff = new D3DXVECTOR3[MAX_VERTEX];
	if (m_pPosGapBuff != nullptr)
	{ // �m�ۂɐ��������ꍇ

		// �������N���A
		memset(m_pPosGapBuff, 0, sizeof(D3DXVECTOR3) * MAX_VERTEX);
	}
	else
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ���_���̐ݒ�
	SetVtx();

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CObject3D::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	SAFE_RELEASE(m_pVtxBuff);

	// �����_�[�X�e�[�g�̔j��
	SAFE_REF_RELEASE(m_pRenderState);

	// ���W�̂���o�b�t�@�̔j��
	SAFE_DEL_ARRAY(m_pPosGapBuff);

	// �I�u�W�F�N�g3D��j��
	Release();
}

//============================================================
//	�X�V����
//============================================================
void CObject3D::Update(void)
{

}

//============================================================
//	�`�揈��
//============================================================
void CObject3D::Draw(void)
{
	// �ϐ���錾
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// �f�o�C�X�̃|�C���^

	// �����_�[�X�e�[�g��ݒ�
	m_pRenderState->Set();

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(object::FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, GET_MANAGER->GetTexture()->GetTexture(m_nTextureID));

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// �����_�[�X�e�[�g���Đݒ�
	m_pRenderState->Reset();
}

//============================================================
//	��������
//============================================================
CObject3D *CObject3D::Create
(
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rSize,	// �傫��
	const D3DXVECTOR3& rRot,	// ����
	const D3DXCOLOR& rCol,		// �F
	const EOrigin origin		// ���_
)
{
	// �I�u�W�F�N�g3D�̐���
	CObject3D *pObject3D = new CObject3D;
	if (pObject3D == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �I�u�W�F�N�g3D�̏�����
		if (FAILED(pObject3D->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �I�u�W�F�N�g3D�̔j��
			SAFE_DELETE(pObject3D);
			return nullptr;
		}

		// ���_��ݒ�
		pObject3D->SetOrigin(origin);

		// �ʒu��ݒ�
		pObject3D->SetVec3Position(rPos);

		// ������ݒ�
		pObject3D->SetVec3Rotation(rRot);

		// �傫����ݒ�
		pObject3D->SetVec3Sizing(rSize);

		// �F��ݒ�
		pObject3D->SetColor(rCol);

		// �m�ۂ����A�h���X��Ԃ�
		return pObject3D;
	}
}

//============================================================
//	�e�N�X�`���������� (�C���f�b�N�X)
//============================================================
void CObject3D::BindTexture(const int nTextureID)
{
	if (nTextureID >= NONE_IDX)
	{ // �e�N�X�`���C���f�b�N�X���g�p�\�ȏꍇ

		// �e�N�X�`���C���f�b�N�X����
		m_nTextureID = nTextureID;
	}
	else { assert(false); }	// �͈͊O
}

//============================================================
//	�e�N�X�`���������� (�p�X)
//============================================================
void CObject3D::BindTexture(const char *pTexturePass)
{
	// �|�C���^��錾
	CTexture *pTexture = GET_MANAGER->GetTexture();	// �e�N�X�`���ւ̃|�C���^

	if (pTexturePass != nullptr)
	{ // ���蓖�Ă�e�N�X�`���p�X������ꍇ

		// �e�N�X�`���C���f�b�N�X��ݒ�
		m_nTextureID = pTexture->Regist(pTexturePass);
	}
	else
	{ // ���蓖�Ă�e�N�X�`���p�X���Ȃ��ꍇ

		// �e�N�X�`���Ȃ��C���f�b�N�X��ݒ�
		m_nTextureID = NONE_IDX;
	}
}

//============================================================
//	�ʒu�̐ݒ菈��
//============================================================
void CObject3D::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// �����̈ʒu��ݒ�
	m_pos = rPos;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�ʒu�擾����
//============================================================
D3DXVECTOR3 CObject3D::GetVec3Position(void) const
{
	// �ʒu��Ԃ�
	return m_pos;
}

//============================================================
//	�����̐ݒ菈��
//============================================================
void CObject3D::SetVec3Rotation(const D3DXVECTOR3& rRot)
{
	// �����̌�����ݒ�
	m_rot = rRot;

	// �����̐��K��
	useful::Vec3NormalizeRot(m_rot);

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�����擾����
//============================================================
D3DXVECTOR3 CObject3D::GetVec3Rotation(void) const
{
	// ������Ԃ�
	return m_rot;
}

//============================================================
//	�傫���̐ݒ菈��
//============================================================
void CObject3D::SetVec3Sizing(const D3DXVECTOR3& rSize)
{
	// �����̑傫������
	m_size = rSize;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�傫���擾����
//============================================================
D3DXVECTOR3 CObject3D::GetVec3Sizing(void) const
{
	// �傫����Ԃ�
	return m_size;
}

//============================================================
//	�F�̐ݒ菈��
//============================================================
void CObject3D::SetColor(const D3DXCOLOR& rCol)
{
	// �����̐F����
	m_col = rCol;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�F�擾����
//============================================================
D3DXCOLOR CObject3D::GetColor(void) const
{
	// �F��Ԃ�
	return m_col;
}

//============================================================
//	���_�̐ݒ菈��
//============================================================
void CObject3D::SetOrigin(const EOrigin origin)
{
	// �����̌��_��ݒ�
	m_origin = origin;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	���_�擾����
//============================================================
CObject3D::EOrigin CObject3D::GetOrigin(void) const
{
	// ���_��Ԃ�
	return m_origin;
}

//============================================================
//	�����_�[�X�e�[�g���̎擾����
//============================================================
CRenderState *CObject3D::GetRenderState(void)
{
	// �C���X�^���X���g�p
	assert(m_pRenderState != nullptr);

	// �����_�[�X�e�[�g�̏���Ԃ�
	return m_pRenderState;
}

//============================================================
//	���_�ʒu�̐ݒ菈��
//============================================================
void CObject3D::SetVertexPosition(const int nID, const D3DXVECTOR3& rPos)
{
	// �|�C���^��錾
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	if (m_pVtxBuff != nullptr)
	{ // �g�p���̏ꍇ

		if (nID < MAX_VERTEX)
		{ // �C���f�b�N�X���g�p�\�ȏꍇ

			// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
			m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			// ���_�f�[�^�̃|�C���^���������i�߂�
			pVtx += nID;

			// ���_���W�̐ݒ�
			pVtx[0].pos = rPos;

			// ���_�o�b�t�@���A�����b�N����
			m_pVtxBuff->Unlock();
		}
	}

	// �@���̐ݒ�E���K��
	NormalizeNormal();
}

//============================================================
//	���_�ʒu�擾����
//============================================================
D3DXVECTOR3 CObject3D::GetVertexPosition(const int nID)
{
	// �ϐ���錾
	D3DXVECTOR3 pos;	// ���_���W�̑���p

	// �|�C���^��錾
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	if (m_pVtxBuff != nullptr)
	{ // �g�p���̏ꍇ

		if (nID < MAX_VERTEX)
		{ // �C���f�b�N�X���g�p�\�ȏꍇ

			// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
			m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			// ���_�f�[�^�̃|�C���^���������i�߂�
			pVtx += nID;

			// ���_���W����
			pos = pVtx[0].pos;

			// ���_�o�b�t�@���A�����b�N����
			m_pVtxBuff->Unlock();
		}
	}

	// �����̃C���f�b�N�X�̒��_���W��Ԃ�
	return pos;
}

//============================================================
//	���W�̂���̐ݒ菈��
//============================================================
void CObject3D::SetGapPosition(const int nID, const D3DXVECTOR3& rPos)
{
	if (m_pPosGapBuff != nullptr)
	{ // �g�p���̏ꍇ

		if (nID < MAX_VERTEX)
		{ // �C���f�b�N�X���g�p�\�ȏꍇ

			// ���_�̂����ݒ�
			m_pPosGapBuff[nID] = rPos;
		}
		else { assert(false); }	// �g�p�s��
	}
}

//============================================================
//	���W�̂���擾����
//============================================================
D3DXVECTOR3 CObject3D::GetGapPosition(const int nID)
{
	// �ϐ���錾
	D3DXVECTOR3 pos = VEC3_ZERO;	// ���_�̂���̑���p

	if (m_pPosGapBuff != nullptr)
	{ // �g�p���̏ꍇ

		if (nID < MAX_VERTEX)
		{ // �C���f�b�N�X���g�p�\�ȏꍇ

			// ���_�̂����ݒ�
			pos = m_pPosGapBuff[nID];
		}
		else { assert(false); }	// �g�p�s��
	}

	// �����̃C���f�b�N�X�̒��_�̂����Ԃ�
	return pos;
}

//============================================================
//	�|���S���̒��n�擾����
//============================================================
float CObject3D::GetPositionHeight(const D3DXVECTOR3&rPos)
{
	// �ϐ���錾
	D3DXVECTOR3 nor;	// �@���x�N�g��

	// �ϐ��z���錾
	D3DXVECTOR3 aVtxPos[MAX_VERTEX];	// �|���S���̒��_���W [��] 0�F���� 1�F�E�� 2�F���� 3�F�E��

	for (int nCnt = 0; nCnt < MAX_VERTEX; nCnt++)
	{ // ���_�����J��Ԃ�

		// �ϐ���錾
		D3DXMATRIX mtxWorld, mtxRot, mtxTrans;		// �v�Z�p�}�g���b�N�X
		D3DXVECTOR3 pos = GetVertexPosition(nCnt);	// ���_���W
		
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&mtxWorld);

		// ���_�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

		// �|���S�������𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

		// �|���S���ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

		// �v�Z�����}�g���b�N�X������W��ݒ�
		aVtxPos[nCnt] = D3DXVECTOR3(mtxWorld._41, mtxWorld._42, mtxWorld._43);
	}

	if (collision::TriangleOuterPillar(aVtxPos[0], aVtxPos[1], aVtxPos[2], rPos))
	{ // �|���S���͈͓̔��ɂ���ꍇ

		// �@�������߂�
		useful::NormalizeNormal(aVtxPos[1], aVtxPos[0], aVtxPos[2], nor);

		if (nor.y != 0.0f)
		{ // �@�����ݒ肳��Ă���ꍇ

			// �v���C���[�̒��n�_��Ԃ�
			return (((rPos.x - aVtxPos[0].x) * nor.x + (-aVtxPos[0].y) * nor.y + (rPos.z - aVtxPos[0].z) * nor.z) * -1.0f) / nor.y;
		}
	}

	if (collision::TriangleOuterPillar(aVtxPos[3], aVtxPos[1], aVtxPos[2], rPos))
	{ // �|���S���͈͓̔��ɂ���ꍇ

		// �@�������߂�
		useful::NormalizeNormal(aVtxPos[2], aVtxPos[3], aVtxPos[1], nor);

		if (nor.y != 0.0f)
		{ // �@�����ݒ肳��Ă���ꍇ

			// �v���C���[�̒��n�_��Ԃ�
			return (((rPos.x - aVtxPos[3].x) * nor.x + (-aVtxPos[3].y) * nor.y + (rPos.z - aVtxPos[3].z) * nor.z) * -1.0f) / nor.y;
		}
	}

	// ���n�͈͊O�̏ꍇ���݂�y���W��Ԃ�
	return rPos.y;
}

//============================================================
//	���_���̐ݒ菈��
//============================================================
void CObject3D::SetVtx(void)
{
	// �|�C���^��錾
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	switch (m_origin)
	{ // ���_���Ƃ̏���
	case ORIGIN_CENTER:	// ����

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-m_size.x * 0.5f,  m_size.y * 0.5f,  m_size.z * 0.5f);
		pVtx[1].pos = D3DXVECTOR3( m_size.x * 0.5f,  m_size.y * 0.5f,  m_size.z * 0.5f);
		pVtx[2].pos = D3DXVECTOR3(-m_size.x * 0.5f, -m_size.y * 0.5f, -m_size.z * 0.5f);
		pVtx[3].pos = D3DXVECTOR3( m_size.x * 0.5f, -m_size.y * 0.5f, -m_size.z * 0.5f);

		break;

	case ORIGIN_DOWN:	// ��

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-m_size.x * 0.5f, m_size.y,  m_size.z * 0.5f);
		pVtx[1].pos = D3DXVECTOR3( m_size.x * 0.5f, m_size.y,  m_size.z * 0.5f);
		pVtx[2].pos = D3DXVECTOR3(-m_size.x * 0.5f,     0.0f, -m_size.z * 0.5f);
		pVtx[3].pos = D3DXVECTOR3( m_size.x * 0.5f,     0.0f, -m_size.z * 0.5f);

		break;

	default:	// ��O����
		assert(false);
		break;
	}

	// ���_����̂���ʂ����Z
	for (int nCntVtx = 0; nCntVtx < MAX_VERTEX; nCntVtx++)
	{ // ���_�����J��Ԃ�

		pVtx[nCntVtx].pos += m_pPosGapBuff[nCntVtx];
	}

	// �@���̐ݒ�E���K��
	NormalizeNormal();

	// ���_�J���[�̐ݒ�
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//============================================================
//	�j������
//============================================================
void CObject3D::Release(void)
{
	// �I�u�W�F�N�g�̔j��
	CObject::Release();
}

//============================================================
//	�@���̐��K������
//============================================================
void CObject3D::NormalizeNormal(void)
{
	// �|�C���^��錾
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �@���̐��K��
	useful::NormalizeNormal
	( // ����
		pVtx[1].pos,	// ���ʒu
		pVtx[0].pos,	// ���S�ʒu
		pVtx[2].pos,	// �E�ʒu
		pVtx[0].nor		// �@��
	);

	// �@���̐��K��
	useful::NormalizeNormal
	( // ����
		pVtx[2].pos,	// ���ʒu
		pVtx[3].pos,	// ���S�ʒu
		pVtx[1].pos,	// �E�ʒu
		pVtx[3].nor		// �@��
	);

	// �@���x�N�g���̐ݒ�
	pVtx[1].nor = (pVtx[0].nor + pVtx[3].nor) / 2;
	pVtx[2].nor = (pVtx[0].nor + pVtx[3].nor) / 2;

	// �@���𐳋K��
	D3DXVec3Normalize(&pVtx[1].nor, &pVtx[1].nor);
	D3DXVec3Normalize(&pVtx[2].nor, &pVtx[2].nor);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}
