//============================================================
//
//	�Q�[�W2D���� [gauge2D.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "gauge2D.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int MAX_VERTEX	= 12;	// ���_��
	const int PRIORITY		= 6;	// �Q�[�W2D�̗D�揇��
}

//************************************************************
//	�q�N���X [CGauge2D] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CGauge2D::CGauge2D(const int nFrame) : CObject(LABEL_UI, DIM_2D, PRIORITY),
	m_nFrame	(nFrame),		// �\���l�̕ϓ��t���[���萔
	m_pVtxBuff	(nullptr),		// ���_�o�b�t�@�ւ̃|�C���^
	m_pos		(VEC3_ZERO),	// �ʒu
	m_sizeGauge	(VEC3_ZERO),	// �Q�[�W�傫��
	m_sizeFrame	(VEC3_ZERO),	// �g�傫��
	m_colFront	(XCOL_WHITE),	// �\�Q�[�W�F
	m_colBack	(XCOL_WHITE),	// ���Q�[�W�F
	m_state		(STATE_NONE),	// ���
	m_nNumGauge	(0),			// �\���l
	m_fChange	(0.0f),			// �Q�[�W�ϓ���
	m_fAddRight	(0.0f),			// �������Z��
	m_nCounterState		(0),	// ��ԊǗ��J�E���^�[
	m_nMaxNumGauge		(0),	// �\���l�̍ő�l
	m_fCurrentNumGauge	(0.0f),	// ���ݕ\���l
	m_bDrawFrame		(false)	// �g�\����
{
	// �����o�ϐ����N���A
	memset(&m_aTextureID[0], 0, sizeof(m_aTextureID));	// �e�N�X�`���C���f�b�N�X
}

//============================================================
//	�f�X�g���N�^
//============================================================
CGauge2D::~CGauge2D()
{

}

//============================================================
//	����������
//============================================================
HRESULT CGauge2D::Init(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// �f�o�C�X�̃|�C���^

	// �����o�ϐ���������
	m_pVtxBuff	= nullptr;			// ���_�o�b�t�@�ւ̃|�C���^
	m_pos		= VEC3_ZERO;		// �ʒu
	m_sizeGauge	= VEC3_ZERO;		// �Q�[�W�傫��
	m_sizeFrame	= VEC3_ZERO;		// �g�傫��
	m_colFront	= XCOL_WHITE;		// �\�Q�[�W�F
	m_colBack	= XCOL_WHITE;		// ���Q�[�W�F
	m_state		= STATE_NONE;		// ���
	m_nNumGauge	= 0;				// �\���l
	m_fChange	= 0.0f;				// �Q�[�W�ϓ���
	m_fAddRight	= 0.0f;				// �������Z��
	m_nCounterState		= 0;		// ��ԊǗ��J�E���^�[
	m_nMaxNumGauge		= 0;		// �\���l�̍ő�l
	m_fCurrentNumGauge	= 0.0f;		// ���ݕ\���l
	m_bDrawFrame		= false;	// �g�\����

	for (int nCntTexture = 0; nCntTexture < POLYGON_MAX; nCntTexture++)
	{ // �g�p����l�p�`�|���S�������J��Ԃ�

		// �e�N�X�`���C���f�b�N�X���N���A
		m_aTextureID[nCntTexture] = NONE_IDX;
	}

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer
	( // ����
		sizeof(VERTEX_2D) * MAX_VERTEX,	// �K�v���_��
		D3DUSAGE_WRITEONLY,				// �g�p���@
		object::FVF_VERTEX_2D,			// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,				// �������̎w��
		&m_pVtxBuff,					// ���_�o�b�t�@�ւ̃|�C���^
		nullptr
	)))
	{ // ���_�o�b�t�@�̐����Ɏ��s�����ꍇ

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
void CGauge2D::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	SAFE_RELEASE(m_pVtxBuff);

	// �I�u�W�F�N�g��j��
	Release();
}

//============================================================
//	�X�V����
//============================================================
void CGauge2D::Update(void)
{
	// �Q�[�W�̐ݒ�
	if (m_state == STATE_CHANGE)
	{ // �̗͂��ϓ����̏ꍇ

		// �J�E���^�[�����Z
		m_nCounterState--;

		// ���݂̃Q�[�W�ɕϓ��ʂ����Z
		m_fCurrentNumGauge += m_fChange;

		// ���݂̕\���l��␳
		useful::LimitNum(m_fCurrentNumGauge, 0.0f, (float)m_nMaxNumGauge);

		// �Q�[�W�̉������Z�ʂ�ݒ�
		m_fAddRight = (m_fCurrentNumGauge * ((m_sizeGauge.x * 2.0f) / (float)m_nMaxNumGauge)) - m_sizeGauge.x;

		// ���_���̐ݒ�
		SetVtx();

		if (m_nCounterState <= 0)
		{ // �J�E���^�[�� 0�ȉ��ɂȂ����ꍇ

			// �ʏ��Ԃɂ���
			m_state = STATE_NONE;
		}
	}
}

//============================================================
//	�`�揈��
//============================================================
void CGauge2D::Draw(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// �f�o�C�X�̃|�C���^

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(object::FVF_VERTEX_2D);

	for (int nCntGauge = 0; nCntGauge < POLYGON_MAX; nCntGauge++)
	{ // �g�p����l�p�`�|���S�������J��Ԃ�

		if (nCntGauge == POLYGON_FRAME)
		{ // �`�悷��l�p�`�|���S�����g�̏ꍇ

			if (m_bDrawFrame)
			{ // �g��\������ꍇ

				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, GET_MANAGER->GetTexture()->GetTexture(m_aTextureID[nCntGauge]));

				// �|���S���̕`��
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntGauge * 4, 2);
			}
		}
		else
		{ // �`�悷��l�p�`�|���S�����g�ȊO�̏ꍇ

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, GET_MANAGER->GetTexture()->GetTexture(m_aTextureID[nCntGauge]));

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntGauge * 4, 2);
		}
	}
}

//============================================================
//	�ʒu�̐ݒ菈��
//============================================================
void CGauge2D::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// �����̈ʒu��ݒ�
	m_pos = rPos;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�ʒu�擾����
//============================================================
D3DXVECTOR3 CGauge2D::GetVec3Position(void) const
{
	// �ʒu��Ԃ�
	return m_pos;
}

//============================================================
//	��������
//============================================================
CGauge2D *CGauge2D::Create
(
	const int nMax,					// �ő�\���l
	const int nFrame,				// �\���l�ϓ��t���[��
	const D3DXVECTOR3& rPos,		// �ʒu
	const D3DXVECTOR3& rSizeGauge,	// �Q�[�W�傫��
	const D3DXCOLOR& rColFront,		// �\�Q�[�W�F
	const D3DXCOLOR& rColBack,		// ���Q�[�W�F
	const bool bDrawFrame,			// �g�`���
	const char *pPassTex,			// �t���[���e�N�X�`���p�X
	const D3DXVECTOR3& rSizeFrame	// �g�傫��
)
{
	// �Q�[�W2D�̐���
	CGauge2D *pGauge2D = new CGauge2D(nFrame);
	if (pGauge2D == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �Q�[�W2D�̏�����
		if (FAILED(pGauge2D->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �Q�[�W2D�̔j��
			SAFE_DELETE(pGauge2D);
			return nullptr;
		}

		// �e�N�X�`����o�^�E����
		pGauge2D->BindTexture(POLYGON_FRAME, GET_MANAGER->GetTexture()->Regist(pPassTex));

		// �Q�[�W�ő�l��ݒ�
		pGauge2D->SetMaxNum(nMax);

		// �ʒu��ݒ�
		pGauge2D->SetVec3Position(rPos);

		// �傫����ݒ�
		pGauge2D->SetScalingGauge(rSizeGauge);	// �Q�[�W�傫��
		pGauge2D->SetScalingFrame(rSizeFrame);	// �g�傫��

		// �F��ݒ�
		pGauge2D->SetColorFront(rColFront);	// �\�Q�[�W�F
		pGauge2D->SetColorBack(rColBack);	// ���Q�[�W�F

		// �g�̕\���󋵂�ݒ�
		pGauge2D->SetEnableDrawFrame(bDrawFrame);

		// �m�ۂ����A�h���X��Ԃ�
		return pGauge2D;
	}
}

//============================================================
//	�Q�[�W�̉��Z����
//============================================================
void CGauge2D::AddNum(const int nAdd)
{
	// ���݂̕\���l��ݒ�
	m_fCurrentNumGauge = (float)m_nNumGauge;

	// �\���l�̕ϓ��ʂ����߂�
	m_fChange = ((float)nAdd / (float)m_nFrame);

	// ����ݒ�
	m_state = STATE_CHANGE;		// �Q�[�W�ϓ����
	m_nCounterState = m_nFrame;	// ��ԊǗ��J�E���^�[

	// �\���l�����������Z
	m_nNumGauge += nAdd;

	// �\���l�̐���
	useful::LimitNum(m_nNumGauge, 0, m_nMaxNumGauge);
}

//============================================================
//	�Q�[�W�̐ݒ菈��
//============================================================
void CGauge2D::SetNum(const int nNum)
{
	// �����̕\���l��ݒ�
	m_nNumGauge = nNum;
	useful::LimitNum(m_nNumGauge, 0, m_nMaxNumGauge);	// �\���l�̐���

	// ���݂̕\���l��ݒ�
	m_fCurrentNumGauge = (float)m_nNumGauge;
	useful::LimitNum(m_fCurrentNumGauge, 0.0f, (float)m_nMaxNumGauge);	// ���݂̕\���l�̐���

	// ����ݒ�
	m_state = STATE_NONE;	// �Q�[�W�ϓ����
	m_nCounterState = 0;	// ��ԊǗ��J�E���^�[

	// �Q�[�W�̉������Z�ʂ�ݒ�
	m_fAddRight = (m_fCurrentNumGauge * ((m_sizeGauge.x * 2.0f) / (float)m_nMaxNumGauge)) - m_sizeGauge.x;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�Q�[�W�擾����
//============================================================
int CGauge2D::GetNum(void) const
{
	// �\���l��Ԃ�
	return m_nNumGauge;
}

//============================================================
//	�Q�[�W�ő�l�̐ݒ菈��
//============================================================
void CGauge2D::SetMaxNum(const int nMax)
{
	// �����̕\���ő�l��ݒ�
	m_nMaxNumGauge = nMax;

	// �Q�[�W�̐ݒ�
	SetNum(m_nMaxNumGauge);
}

//============================================================
//	�Q�[�W�ő�l�擾����
//============================================================
int CGauge2D::GetMaxNum(void) const
{
	// �\���ő�l��Ԃ�
	return m_nMaxNumGauge;
}

//============================================================
//	�e�N�X�`���������� (�C���f�b�N�X)
//============================================================
void CGauge2D::BindTexture(const int nPolygonID, const int nTextureID)
{
	if (nPolygonID > NONE_IDX && nPolygonID < POLYGON_MAX)
	{ // ���K�C���f�b�N�X�̏ꍇ

		if (nTextureID >= NONE_IDX)
		{ // �e�N�X�`���C���f�b�N�X���g�p�\�ȏꍇ

			// �e�N�X�`���C���f�b�N�X����
			m_aTextureID[nPolygonID] = nTextureID;
		}
		else { assert(false); }	// �͈͊O
	}
}

//============================================================
//	�e�N�X�`���������� (�p�X)
//============================================================
void CGauge2D::BindTexture(const int nPolygonID, const char *pTexturePass)
{
	// �|�C���^��錾
	CTexture *pTexture = GET_MANAGER->GetTexture();	// �e�N�X�`���ւ̃|�C���^

	if (nPolygonID > NONE_IDX && nPolygonID < POLYGON_MAX)
	{ // ���K�C���f�b�N�X�̏ꍇ

		if (pTexturePass != nullptr)
		{ // ���蓖�Ă�e�N�X�`���p�X������ꍇ
	
			// �e�N�X�`���C���f�b�N�X��ݒ�
			m_aTextureID[nPolygonID] = pTexture->Regist(pTexturePass);
		}
		else
		{ // ���蓖�Ă�e�N�X�`���p�X���Ȃ��ꍇ

			// �e�N�X�`���Ȃ��C���f�b�N�X��ݒ�
			m_aTextureID[nPolygonID] = NONE_IDX;
		}
	}
}

//============================================================
//	�Q�[�W�傫���̐ݒ菈��
//============================================================
void CGauge2D::SetScalingGauge(const D3DXVECTOR3& rSize)
{
	// �����̃Q�[�W�傫������
	m_sizeGauge = rSize;

	// �Q�[�W�̐ݒ�
	SetNum(m_nNumGauge);

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�Q�[�W�傫���擾����
//============================================================
D3DXVECTOR3 CGauge2D::GetScalingGauge(void) const
{
	// �Q�[�W�傫����Ԃ�
	return m_sizeGauge;
}

//============================================================
//	�w�i�傫���̐ݒ菈��
//============================================================
void CGauge2D::SetScalingFrame(const D3DXVECTOR3& rSize)
{
	// �����̔w�i�傫������
	m_sizeFrame = rSize;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�w�i�傫���擾����
//============================================================
D3DXVECTOR3 CGauge2D::GetScalingFrame(void) const
{
	// �w�i�傫����Ԃ�
	return m_sizeFrame;
}

//============================================================
//	�\�Q�[�W�F�̐ݒ菈��
//============================================================
void CGauge2D::SetColorFront(const D3DXCOLOR& rCol)
{
	// �����̕\�Q�[�W�F����
	m_colFront = rCol;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�\�Q�[�W�F�擾����
//============================================================
D3DXCOLOR CGauge2D::GetColorFront(void) const
{
	// �\�Q�[�W�F��Ԃ�
	return m_colFront;
}

//============================================================
//	���Q�[�W�F�̐ݒ菈��
//============================================================
void CGauge2D::SetColorBack(const D3DXCOLOR& rCol)
{
	// �����̗��Q�[�W�F����
	m_colBack = rCol;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	���Q�[�W�F�擾����
//============================================================
D3DXCOLOR CGauge2D::GetColorBack(void) const
{
	// ���Q�[�W�F��Ԃ�
	return m_colBack;
}

//============================================================
//	�g�\���󋵐ݒ菈��
//============================================================
void CGauge2D::SetEnableDrawFrame(const bool bDraw)
{
	// �����̘g�̕\���󋵂�ݒ�
	m_bDrawFrame = bDraw;
}

//============================================================
//	�g�\���󋵎擾����
//============================================================
bool CGauge2D::IsEnableDrawFrame(void) const
{
	// �g�\���󋵂�Ԃ�
	return m_bDrawFrame;
}

//============================================================
//	�j������
//============================================================
void CGauge2D::Release(void)
{
	// �I�u�W�F�N�g�̔j��
	CObject::Release();
}

//============================================================
//	���_���̐ݒ菈��
//============================================================
void CGauge2D::SetVtx(void)
{
	// �|�C���^��錾
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	if (m_pVtxBuff != nullptr)
	{ // �g�p���̏ꍇ

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntGauge = 0; nCntGauge < POLYGON_MAX; nCntGauge++)
		{ // �g�p����l�p�`�|���S�������J��Ԃ�

			switch (nCntGauge)
			{ // �l�p�`�|���S�����Ƃ̏���
			case POLYGON_BACK:	// �w�i

				// ���_���W��ݒ�
				pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_sizeGauge.x, m_pos.y - m_sizeGauge.y, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_sizeGauge.x, m_pos.y - m_sizeGauge.y, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_sizeGauge.x, m_pos.y + m_sizeGauge.y, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_sizeGauge.x, m_pos.y + m_sizeGauge.y, 0.0f);

				// ���_�J���[�̐ݒ�
				pVtx[0].col = m_colBack;
				pVtx[1].col = m_colBack;
				pVtx[2].col = m_colBack;
				pVtx[3].col = m_colBack;

				break;

			case POLYGON_FRONT:	// �Q�[�W

				// ���_���W��ݒ�
				pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_sizeGauge.x, m_pos.y - m_sizeGauge.y, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fAddRight,   m_pos.y - m_sizeGauge.y, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_sizeGauge.x, m_pos.y + m_sizeGauge.y, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fAddRight,   m_pos.y + m_sizeGauge.y, 0.0f);

				// ���_�J���[�̐ݒ�
				pVtx[0].col = m_colFront;
				pVtx[1].col = m_colFront;
				pVtx[2].col = m_colFront;
				pVtx[3].col = m_colFront;

				break;

			case POLYGON_FRAME:	// �g

				// ���_���W��ݒ�
				pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_sizeFrame.x, m_pos.y - m_sizeFrame.y, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_sizeFrame.x, m_pos.y - m_sizeFrame.y, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_sizeFrame.x, m_pos.y + m_sizeFrame.y, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_sizeFrame.x, m_pos.y + m_sizeFrame.y, 0.0f);

				// ���_�J���[�̐ݒ�
				pVtx[0].col = XCOL_WHITE;
				pVtx[1].col = XCOL_WHITE;
				pVtx[2].col = XCOL_WHITE;
				pVtx[3].col = XCOL_WHITE;

				break;

			default:	// ��O����
				assert(false);
				break;
			}

			// rhw �̐ݒ�
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			// ���_�f�[�^�̃|�C���^�� 4���i�߂�
			pVtx += 4;
		}

		// ���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}
}
