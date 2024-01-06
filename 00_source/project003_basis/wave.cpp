//============================================================
//
//	�g������ [wave.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "wave.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char *TEXTURE_FILE[] =	// �e�N�X�`���t�@�C��
	{
		nullptr,	// �e�N�X�`���Ȃ�
	};

	const int PRIORITY = 4;	// �g���̗D�揇��
}

//************************************************************
//	�X�^�e�B�b�N�A�T�[�g
//************************************************************
static_assert(NUM_ARRAY(TEXTURE_FILE) == CWave::TEXTURE_MAX, "ERROR : Texture Count Mismatch");

//************************************************************
//	�q�N���X [CWave] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CWave::CWave() : CScrollMeshRing(LABEL_WAVE, DIM_3D, PRIORITY)
{
	// �����o�ϐ����N���A
	memset(&m_grow, 0, sizeof(m_grow));	// �����̏��
}

//============================================================
//	�f�X�g���N�^
//============================================================
CWave::~CWave()
{

}

//============================================================
//	����������
//============================================================
HRESULT CWave::Init(void)
{
	// �����o�ϐ���������
	m_grow = SGrow(0.0f, 0.0f, 0.0f);	// �����̏��

	// �X�N���[�����b�V�������O�̏�����
	if (FAILED(CScrollMeshRing::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �����_�[�X�e�[�g�̏����擾
	CRenderState *pRenderState = GetRenderState();

	// ���e�X�g�̐ݒ�
	pRenderState->SetAlphaTest(true);			// ���e�X�g�̗L�� / �����̐ݒ�
	pRenderState->SetAlphaFunc(D3DCMP_GREATER);	// ���e�X�g�̐ݒ�

	// �|���S���̗��ʂ�\����Ԃɂ���
	pRenderState->SetCulling(D3DCULL_NONE);

	// ���C�e�B���O��OFF�ɂ���
	pRenderState->SetLighting(false);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CWave::Uninit(void)
{
	// �X�N���[�����b�V�������O�̏I��
	CScrollMeshRing::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CWave::Update(void)
{
	// �ϐ���錾
	D3DXCOLOR colRing = GetColor();			// ���b�V�������O�̐F
	float fHoleRing  = GetHoleRadius();		// ���b�V�������O�̌��̔��a
	float fThickRing = GetThickness();		// ���b�V�������O�̑���

	// �g���𐬒�������
	colRing.a  -= m_grow.fSubAlpha;			// �g���̓����x�����Z (�����ɂ��Ă���)
	fHoleRing  += m_grow.fAddHoleRadius;	// �g���̌��̍L��������Z
	fThickRing += m_grow.fAddThickness;		// �g���𑾂��̍L��������Z

	if (fHoleRing <= 0.0f)
	{ // �g���̌��������Ȃ����ꍇ

		// �g���̌��̍L����ʂ̗]�蕪��g���̑����ɗ^����
		fThickRing += fHoleRing;

		// �g���̌���␳
		fHoleRing = 0.0f;
	}

	if (colRing.a  <= 0.0f
	||  fThickRing <= 0.0f)
	{ // �g���������A�܂��ׂ͍��Ȃ�؂����ꍇ

		// �g���I�u�W�F�N�g�̏I��
		Uninit();

		// �����𔲂���
		return;
	}

	// ���b�V�������O�̐F�𔽉f
	SetColor(colRing);

	// ���b�V�������O�̌��̔��a�𔽉f
	SetHoleRadius(fHoleRing);

	// ���b�V�������O�̑����𔽉f
	SetThickness(fThickRing);

	// �X�N���[�����b�V�������O�̍X�V
	CScrollMeshRing::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CWave::Draw(void)
{
	// �X�N���[�����b�V�������O�̕`��
	CScrollMeshRing::Draw();
}

//============================================================
//	��������
//============================================================
CWave *CWave::Create
(
	const ETexture texture,		// ���
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rRot,	// ����
	const D3DXCOLOR& rCol,		// �F
	const SGrow& rGrow,			// ������
	const POSGRID2& rPart,		// ������
	const POSGRID2& rTexPart,	// �e�N�X�`��������
	const float fHoleRadius,	// ���̔��a
	const float fThickness,		// ����
	const float fOuterPlusY		// �O����Y���W���Z��
)
{
	// �g���̐���
	CWave *pWave = new CWave;
	if (pWave == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �g���̏�����
		if (FAILED(pWave->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �g���̔j��
			SAFE_DELETE(pWave);
			return nullptr;
		}

		// �e�N�X�`����o�^�E����
		pWave->BindTexture(GET_MANAGER->GetTexture()->Regist(TEXTURE_FILE[texture]));

		// �ʒu��ݒ�
		pWave->SetVec3Position(rPos);

		// ������ݒ�
		pWave->SetVec3Rotation(rRot);

		// �F��ݒ�
		pWave->SetColor(rCol);

		// ��������ݒ�
		pWave->SetGrow(rGrow);

		// ���̔��a��ݒ�
		pWave->SetHoleRadius(fHoleRadius);

		// ������ݒ�
		pWave->SetThickness(fThickness);

		// �O����Y���W���Z�ʂ�ݒ�
		pWave->SetOuterPlusY(fOuterPlusY);

		// ��������ݒ�
		if (FAILED(pWave->SetPattern(rPart)))
		{ // �������̐ݒ�Ɏ��s�����ꍇ

			// �I�u�W�F�N�g���b�V�������O�̔j��
			SAFE_DELETE(pWave);
			return nullptr;
		}

		// �e�N�X�`����������ݒ�
		pWave->SetTexPattern(rTexPart);

		// �m�ۂ����A�h���X��Ԃ�
		return pWave;
	}
}

//============================================================
//	�������̐ݒ菈��
//============================================================
void CWave::SetGrow(const SGrow & rGrow)
{
	// �����̐�������ݒ�
	m_grow = rGrow;
}

//============================================================
//	�������̎擾����
//============================================================
CWave::SGrow CWave::GetGrow(void) const
{
	// ��������Ԃ�
	return m_grow;
}
