//============================================================
//
//	�����_�[�X�e�[�g���� [renderState.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "renderState.h"
#include "manager.h"
#include "renderer.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const CRenderState::SBlendAlpha SET_BLEND[] =	// �񋓂��Ƃ̃��u�����h
	{
		RS_BL_NORMAL,	// �ʏ탿�u�����h
		RS_BL_ADD,		// ���Z���u�����h
		RS_BL_SUB,		// ���Z���u�����h
	};
}

//************************************************************
//	�X�^�e�B�b�N�A�T�[�g
//************************************************************
static_assert(NUM_ARRAY(SET_BLEND) == CRenderState::BLEND_MAX, "ERROR : Blend Count Mismatch");

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
LPDIRECT3DDEVICE9 *CRenderState::m_ppDevice = nullptr;	// Direct3D�f�o�C�X�ւ̃|�C���^
CRenderState::SInfo CRenderState::m_save = {};			// �ۑ������_�[�X�e�[�g���

//************************************************************
//	�e�N���X [CRenderState] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CRenderState::CRenderState()
{
	// �����o�ϐ����N���A
	memset(&m_info, 0, sizeof(m_info));	// �����_�[�X�e�[�g���
}

//============================================================
//	�f�X�g���N�^
//============================================================
CRenderState::~CRenderState()
{

}

//============================================================
//	����������
//============================================================
HRESULT CRenderState::Init(void)
{
	// �����o�ϐ���������
	m_info = m_save;	// �����_�[�X�e�[�g���

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CRenderState::Uninit(void)
{

}

//============================================================
//	�ݒ菈��
//============================================================
void CRenderState::Set(void)
{
	// �ݒ蒆�̃����_�[�X�e�[�g��ݒ�
	SetRenderState(m_info);
}

//============================================================
//	�Đݒ菈��
//============================================================
void CRenderState::Reset(void)
{
	// �ۑ��ς݂̃����_�[�X�e�[�g�ɖ߂�
	SetRenderState(m_save);
}

//============================================================
//	�����_�[�X�e�[�g���̐ݒ菈��
//============================================================
void CRenderState::SetInfoRenderState(const SInfo & rInfo)
{
	// �����̃����_�[�X�e�[�g����ݒ�
	m_info = rInfo;
}

//============================================================
//	�����_�[�X�e�[�g���̎擾����
//============================================================
CRenderState::SInfo CRenderState::GetInfoRenderState(void)
{
	// �����_�[�X�e�[�g����Ԃ�
	return m_info;
}

//============================================================
//	���u�����h���̐ݒ菈��
//============================================================
void CRenderState::SetAlphaBlend(const EBlend blend)
{
	if (blend > NONE_IDX && blend < BLEND_MAX)
	{ // �u�����h��ނ͈͓̔��̏ꍇ

		// �����̃��u�����h��ݒ�
		m_info.blendAlpha = SET_BLEND[blend];
	}
	else { assert(false); }	// �͈͊O
}

//============================================================
//	��������
//============================================================
CRenderState *CRenderState::Create(void)
{
	// �����_�[�X�e�[�g�̐���
	CRenderState *pRenderState = new CRenderState;
	if (pRenderState == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �����_�[�X�e�[�g�̏�����
		if (FAILED(pRenderState->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �����_�[�X�e�[�g�̔j��
			SAFE_DELETE(pRenderState);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pRenderState;
	}
}

//============================================================
//	�j������
//============================================================
void CRenderState::Release(CRenderState *&prRenderState)
{
	// �����_�[�X�e�[�g�̏I��
	assert(prRenderState != nullptr);
	prRenderState->Uninit();

	// �������J��
	SAFE_DELETE(prRenderState);
}

//============================================================
//	�f�o�C�X�̊�������
//============================================================
void CRenderState::BindDevice(LPDIRECT3DDEVICE9 *ppDevice)
{
	// �f�o�C�X���w��
	assert(*ppDevice != nullptr);

	// �f�o�C�X�̃A�h���X��ۑ�
	m_ppDevice = ppDevice;
}

//============================================================
//	�����_�[�X�e�[�g���̏���������
//============================================================
void CRenderState::InitRenderState(void)
{
	// ���u�����h����������
	m_save.blendAlpha.op		= D3DBLENDOP_ADD;
	m_save.blendAlpha.scr		= D3DBLEND_SRCALPHA;
	m_save.blendAlpha.dest		= D3DBLEND_INVSRCALPHA;
	m_save.blendAlpha.bBlend	= true;

	// ���e�X�g����������
	m_save.testAlpha.func	= D3DCMP_ALWAYS;
	m_save.testAlpha.nRef	= 0;
	m_save.testAlpha.bTest	= false;

	// Z�e�X�g����������
	m_save.testZ.func		= D3DCMP_LESSEQUAL;
	m_save.testZ.bUpdate	= true;

	// �J�����O����������
	m_save.cull = D3DCULL_CCW;

	// ���C�e�B���O�󋵂�������
	m_save.bLight = true;

	// �����_�[�X�e�[�g�̏�����
	SetRenderState(m_save);
}

//============================================================
//	�ۑ������_�[�X�e�[�g���̎擾����
//============================================================
CRenderState::SInfo CRenderState::GetSaveRenderState(void)
{
	// �ۑ������_�[�X�e�[�g����Ԃ�
	return m_save;
}

//============================================================
//	�����_�[�X�e�[�g�̐ݒ菈��
//============================================================
void CRenderState::SetRenderState(const SInfo& rInfo)
{
	// �f�o�C�X���ݒ�
	assert(*m_ppDevice != nullptr);

	// ���u�����f�B���O��ݒ�
	(*m_ppDevice)->SetRenderState(D3DRS_ALPHABLENDENABLE, rInfo.blendAlpha.bBlend);
	(*m_ppDevice)->SetRenderState(D3DRS_BLENDOP, rInfo.blendAlpha.op);
	(*m_ppDevice)->SetRenderState(D3DRS_SRCBLEND, rInfo.blendAlpha.scr);
	(*m_ppDevice)->SetRenderState(D3DRS_DESTBLEND, rInfo.blendAlpha.dest);

	// ���e�X�g��ݒ�
	(*m_ppDevice)->SetRenderState(D3DRS_ALPHAFUNC, rInfo.testAlpha.func);			// ���e�X�g�̐ݒ�
	(*m_ppDevice)->SetRenderState(D3DRS_ALPHAREF, rInfo.testAlpha.nRef);			// ���e�X�g�̎Q�ƒl�ݒ�
	(*m_ppDevice)->SetRenderState(D3DRS_ALPHATESTENABLE, rInfo.testAlpha.bTest);	// ���e�X�g�̗L�� / �����̐ݒ�

	// Z�e�X�g��ݒ�
	(*m_ppDevice)->SetRenderState(D3DRS_ZFUNC, rInfo.testZ.func);			// Z�e�X�g�̐ݒ�
	(*m_ppDevice)->SetRenderState(D3DRS_ZWRITEENABLE, rInfo.testZ.bUpdate);	// Z�o�b�t�@�X�V�̗L�� / �����̐ݒ�

	// �|���S���̕\����Ԃ�ݒ�
	(*m_ppDevice)->SetRenderState(D3DRS_CULLMODE, rInfo.cull);

	// ���C�e�B���O��ݒ�
	(*m_ppDevice)->SetRenderState(D3DRS_LIGHTING, rInfo.bLight);
}
