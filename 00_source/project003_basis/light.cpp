//============================================================
//
//	���C�g���� [light.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "light.h"
#include "manager.h"
#include "renderer.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const D3DXCOLOR DIFFUSE[] =	// �ݒ�p�g�U���J���[
	{
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
		D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f),
		D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f),
		D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f),
	};

	const D3DXVECTOR3 DIRECTION[] =	// �ݒ�p�����x�N�g��
	{
		D3DXVECTOR3( 0.22f, -0.97f,  0.54f),
		D3DXVECTOR3(-0.38f,  0.78f, -0.54f),
		D3DXVECTOR3( 0.89f, -0.21f,  0.44f),
		D3DXVECTOR3(-0.96f,  0.15f, -0.44f),
	};
}

//************************************************************
//	�X�^�e�B�b�N�A�T�[�g
//************************************************************
static_assert(NUM_ARRAY(DIFFUSE)   == light::MAX_NUM, "ERROR : Light Count Mismatch");
static_assert(NUM_ARRAY(DIRECTION) == light::MAX_NUM, "ERROR : Light Count Mismatch");

//************************************************************
//	�e�N���X [CLight] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CLight::CLight()
{
	// �����o�ϐ����N���A
	memset(&m_aLight[0], 0, sizeof(m_aLight));	// ���C�g�̏��
}

//============================================================
//	�f�X�g���N�^
//============================================================
CLight::~CLight()
{

}

//============================================================
//	����������
//============================================================
HRESULT CLight::Init(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// �f�o�C�X�̃|�C���^

	for (int nCntLight = 0; nCntLight < light::MAX_NUM; nCntLight++)
	{ // �g�p���郉�C�g�����J��Ԃ�

		// �ϐ���錾
		D3DXCOLOR	diffuse		= DIFFUSE[nCntLight];	// �ݒ�p�g�U���J���[
		D3DXVECTOR3	direction	= DIRECTION[nCntLight];	// �ݒ�p�����x�N�g��

		// �\���̗̂v�f���N���A
		ZeroMemory(&m_aLight[nCntLight], sizeof(D3DLIGHT9));

		// ���C�g�̎�ނ�ݒ�
		m_aLight[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

		// ���C�g�̊g�U����ݒ�
		m_aLight[nCntLight].Diffuse = diffuse;

		// ���C�g�̕�����ݒ�
		D3DXVec3Normalize(&direction, &direction);	// �x�N�g���𐳋K��
		m_aLight[nCntLight].Direction = direction;

		// ���C�g��ݒ肷��
		pDevice->SetLight(nCntLight, &m_aLight[nCntLight]);

		// ���C�g��L���ɂ���
		pDevice->LightEnable(nCntLight, TRUE);
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CLight::Uninit(void)
{

}

//============================================================
//	�X�V����
//============================================================
void CLight::Update(void)
{

}

//============================================================
//	���C�g�擾����
//============================================================
D3DLIGHT9 CLight::GetLight(const int nID)
{
	if (nID > NONE_IDX && nID < light::MAX_NUM)
	{ // �v�f���͈͓̔��̏ꍇ

		// ���C�g�̏���Ԃ�
		return m_aLight[nID];
	}
	else { assert(false); return m_aLight[0]; }	// �͈͊O
}

//============================================================
//	��������
//============================================================
CLight *CLight::Create(void)
{
	// ���C�g�̐���
	CLight *pLight = new CLight;
	if (pLight == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// ���C�g�̏�����
		if (FAILED(pLight->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ���C�g�̔j��
			SAFE_DELETE(pLight);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pLight;
	}
}

//============================================================
//	�j������
//============================================================
void CLight::Release(CLight *&prLight)
{
	// ���C�g�̏I��
	assert(prLight != nullptr);
	prLight->Uninit();

	// �������J��
	SAFE_DELETE(prLight);
}
