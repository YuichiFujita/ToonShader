//============================================================
//
//	�t�̏��� [liquid.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "liquid.h"
#include "manager.h"
#include "renderer.h"
#include "scrollMeshField.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char *TEXTURE_FILE[][CLiquid::LIQUID_MAX] =	// �e�N�X�`���t�@�C��
	{
		{ // �C�e�N�X�`��
			"data\\TEXTURE\\sea000.jpg",	// �C(��)
			"data\\TEXTURE\\sea000.png",	// �C(��)
		},
	};

	const int PRIORITY = 0;	// �t�̗̂D�揇��
}

//************************************************************
//	�X�^�e�B�b�N�A�T�[�g
//************************************************************
static_assert(NUM_ARRAY(TEXTURE_FILE) == CLiquid::TYPE_MAX, "ERROR : Texture Count Mismatch");

//************************************************************
//	�q�N���X [CLiquid] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CLiquid::CLiquid() : CObject(CObject::LABEL_LIQUID, CObject::DIM_3D, PRIORITY),
	m_type			(TYPE_SEA),	// ���
	m_fMaxUp		(0.0f),		// �g�̍ō��㏸��
	m_fSinRot		(0.0f),		// �g�ł�����
	m_fAddSinRot	(0.0f),		// �g�ł��������Z��
	m_fAddVtxRot	(0.0f)		// �הg�̌������Z��
{
	// �����o�ϐ����N���A
	memset(&m_apLiquid[0], 0, sizeof(m_apLiquid));	// �t�̂̏��
}

//============================================================
//	�f�X�g���N�^
//============================================================
CLiquid::~CLiquid()
{

}

//============================================================
//	����������
//============================================================
HRESULT CLiquid::Init(void)
{
	// �����o�ϐ���������
	memset(&m_apLiquid[0], 0, sizeof(m_apLiquid));	// �t�̂̏��
	m_type			= TYPE_SEA;	// ���
	m_fMaxUp		= 0.0f;		// �g�̍ō��㏸��
	m_fSinRot		= 0.0f;		// �g�ł�����
	m_fAddSinRot	= 0.0f;		// �g�ł��������Z��
	m_fAddVtxRot	= 0.0f;		// �הg�̌������Z��

	for (int nCntLiquid = 0; nCntLiquid < LIQUID_MAX; nCntLiquid++)
	{ // �t�̂̍ő吔���J��Ԃ�

		// �t�̂̐���
		m_apLiquid[nCntLiquid] = CScrollMeshField::Create
		( // ����
			0.0f,			// �����W�̈ړ���
			0.0f,			// �c���W�̈ړ���
			VEC3_ZERO,		// �ʒu
			VEC3_ZERO,		// ����
			VEC2_ZERO,		// �傫��
			XCOL_WHITE,		// �F
			GRID2_ONE		// ������
		);
		if (m_apLiquid[nCntLiquid] == nullptr)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �e�N�X�`����o�^�E����
		m_apLiquid[nCntLiquid]->BindTexture(TEXTURE_FILE[m_type][nCntLiquid]);
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CLiquid::Uninit(void)
{
	for (int nCntLiquid = 0; nCntLiquid < LIQUID_MAX; nCntLiquid++)
	{ // �t�̂̍ő吔���J��Ԃ�

		// �t�̂̏I��
		SAFE_UNINIT(m_apLiquid[nCntLiquid]);
	}

	// ���g�̃I�u�W�F�N�g��j��
	Release();
}

//============================================================
//	�X�V����
//============================================================
void CLiquid::Update(void)
{
	// �ϐ���錾
	POSGRID2 part = GetPattern();	// ������
	D3DXVECTOR3 pos;	// ���_���W�擾�p
	int nLine;			// ���_�̍s
	float fSinRot;		// �T�C���J�[�u�Ɏg�p����p�x

	// ���b�V���̒��_���W�̍X�V
	for (int nCntVtx = 0; nCntVtx < (part.x + 1) * (part.y + 1); nCntVtx++)
	{ // �g�p���_�����J��Ԃ�

		for (int nCntLiquid = 0; nCntLiquid < LIQUID_MAX; nCntLiquid++)
		{ // �t�̂̍ő吔���J��Ԃ�

			// ���_���W���擾
			pos = m_apLiquid[nCntLiquid]->GetMeshVertexPosition(nCntVtx);

			// �g�ł��p�̊p�x�����߂�
			nLine = nCntVtx / (part.x + 1);				// ���_�̍s
			fSinRot = m_fSinRot + nLine * m_fAddVtxRot;	// �T�C���J�[�u�Ɏg�p����p�x

			// �T�C���J�[�u�p�̌�����␳
			useful::NormalizeRot(fSinRot);

			// ���_���W��ݒ�
			m_apLiquid[nCntLiquid]->SetMeshVertexPosition(nCntVtx, D3DXVECTOR3(pos.x, (sinf(fSinRot) * m_fMaxUp) + m_fMaxUp, pos.z));
		}
	}

	// �g�ł����������Z
	m_fSinRot += m_fAddSinRot;

	// ������␳
	useful::NormalizeRot(m_fSinRot);

	for (int nCntLiquid = 0; nCntLiquid < LIQUID_MAX; nCntLiquid++)
	{ // �t�̂̍ő吔���J��Ԃ�

		// �t�̂̍X�V
		m_apLiquid[nCntLiquid]->Update();
	}
}

//============================================================
//	�`�揈��
//============================================================
void CLiquid::Draw(void)
{

}

//============================================================
//	�ʒu�̐ݒ菈��
//============================================================
void CLiquid::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// �t�̂̈ʒu��ݒ�
	for (int nCntLiquid = 0; nCntLiquid < LIQUID_MAX; nCntLiquid++)
	{ // �t�̂̍ő吔���J��Ԃ�

		// �����̈ʒu��ݒ�
		m_apLiquid[nCntLiquid]->SetVec3Position(rPos);
	}
}

//============================================================
//	�ʒu�擾����
//============================================================
D3DXVECTOR3 CLiquid::GetVec3Position(void) const
{
	// ���̉t�̂̈ʒu��Ԃ�
	return m_apLiquid[LIQUID_LOW]->GetVec3Position();
}

//============================================================
//	�����̐ݒ菈��
//============================================================
void CLiquid::SetVec3Rotation(const D3DXVECTOR3& rRot)
{
	// �t�̂̌�����ݒ�
	for (int nCntLiquid = 0; nCntLiquid < LIQUID_MAX; nCntLiquid++)
	{ // �t�̂̍ő吔���J��Ԃ�

		// �����̌�����ݒ�
		m_apLiquid[nCntLiquid]->SetVec3Rotation(rRot);
	}
}

//============================================================
//	�����擾����
//============================================================
D3DXVECTOR3 CLiquid::GetVec3Rotation(void) const
{
	// ���̉t�̂̌�����Ԃ�
	return m_apLiquid[LIQUID_LOW]->GetVec3Rotation();
}

//============================================================
//	�傫���̐ݒ菈��
//============================================================
void CLiquid::SetVec2Sizing(const D3DXVECTOR2& rSize)
{
	// �t�̂̑傫����ݒ�
	for (int nCntLiquid = 0; nCntLiquid < LIQUID_MAX; nCntLiquid++)
	{ // �t�̂̍ő吔���J��Ԃ�

		// �����̑傫����ݒ�
		m_apLiquid[nCntLiquid]->SetVec2Sizing(rSize);
	}
}

//============================================================
//	�傫���擾����
//============================================================
D3DXVECTOR2 CLiquid::GetVec2Sizing(void) const
{
	// ���̉t�̂̑傫����Ԃ�
	return m_apLiquid[LIQUID_LOW]->GetVec2Sizing();
}

//============================================================
//	�F�̐ݒ菈��
//============================================================
void CLiquid::SetColor(const D3DXCOLOR& rCol)
{
	// �t�̂̐F��ݒ�
	for (int nCntLiquid = 0; nCntLiquid < LIQUID_MAX; nCntLiquid++)
	{ // �t�̂̍ő吔���J��Ԃ�

		// �����̐F��ݒ�
		m_apLiquid[nCntLiquid]->SetColor(rCol);
	}
}

//============================================================
//	�F�擾����
//============================================================
D3DXCOLOR CLiquid::GetColor(void) const
{
	// ���̉t�̂̐F��Ԃ�
	return m_apLiquid[LIQUID_LOW]->GetColor();
}

//============================================================
//	��ނ̐ݒ菈��
//============================================================
void CLiquid::SetType(const int nType)
{
	if (nType > NONE_IDX && nType < TYPE_MAX)
	{ // ��ނ��͈͓��̏ꍇ

		// �����̎�ނ�ݒ�
		m_type = (EType)nType;

		for (int nCntLiquid = 0; nCntLiquid < LIQUID_MAX; nCntLiquid++)
		{ // �t�̂̍ő吔���J��Ԃ�

			// �e�N�X�`����o�^�E����
			m_apLiquid[nCntLiquid]->BindTexture(TEXTURE_FILE[m_type][nCntLiquid]);
		}
	}
	else { assert(false); }	// �͈͊O
}

//============================================================
//	��ގ擾����
//============================================================
int CLiquid::GetType(void) const
{
	// ��ނ�Ԃ�
	return m_type;
}

//============================================================
//	��������
//============================================================
CLiquid *CLiquid::Create
(
	const EType type,			// ���
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rRot,	// ����
	const D3DXVECTOR2& rSize,	// �傫��
	const D3DXCOLOR& rCol,		// �F
	const POSGRID2& rPart,		// ������
	const STexMove& rTexMove,	// �e�N�X�`���ړ���
	const float fMaxUp,			// �g�̍ō��㏸��
	const float fAddSinRot,		// �g�ł��������Z��
	const float fAddVtxRot		// �הg�̌������Z��
)
{
	// �t�̂̐���
	CLiquid *pLiquid = new CLiquid;
	if (pLiquid == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �t�̂̏�����
		if (FAILED(pLiquid->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �t�̂̔j��
			SAFE_DELETE(pLiquid);
			return nullptr;
		}

		// ��ނ�ݒ�
		pLiquid->SetType(type);

		// �ʒu��ݒ�
		pLiquid->SetVec3Position(rPos);

		// ������ݒ�
		pLiquid->SetVec3Rotation(rRot);

		// �傫����ݒ�
		pLiquid->SetVec2Sizing(rSize);

		// �F��ݒ�
		pLiquid->SetColor(rCol);

		// �e�N�X�`���ړ��ʂ�ݒ�
		pLiquid->SetTexMove(rTexMove);

		// �g�̍ō��㏸�ʂ�ݒ�
		pLiquid->SetMaxUp(fMaxUp);

		// �g�ł��������Z�ʂ�ݒ�
		pLiquid->SetAddSinRot(fAddSinRot);

		// �הg�̌������Z�ʂ�ݒ�
		pLiquid->SetAddVtxRot(fAddVtxRot);

		// ��������ݒ�
		if (FAILED(pLiquid->SetPattern(rPart)))
		{ // �������̐ݒ�Ɏ��s�����ꍇ

			// �t�̂̔j��
			SAFE_DELETE(pLiquid);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pLiquid;
	}
}

//============================================================
//	�������̐ݒ菈��
//============================================================
HRESULT CLiquid::SetPattern(const POSGRID2& rPart)
{
	// �t�̂̕�������ݒ�
	for (int nCntLiquid = 0; nCntLiquid < LIQUID_MAX; nCntLiquid++)
	{ // �t�̂̍ő吔���J��Ԃ�

		// �����̕�������ݒ�
		if (FAILED(m_apLiquid[nCntLiquid]->SetPattern(rPart)))
		{ // �ݒ�Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�������擾����
//============================================================
POSGRID2 CLiquid::GetPattern(void) const
{
	// ���̉t�̂̕�������Ԃ�
	return m_apLiquid[LIQUID_LOW]->GetPattern();
}

//============================================================
//	�e�N�X�`���ړ��ʂ̐ݒ菈��
//============================================================
void CLiquid::SetTexMove(const STexMove texMove)
{
	// ���t�̂̃e�N�X�`���ړ��ʂ�ݒ�
	m_apLiquid[LIQUID_LOW]->SetMoveU(texMove.texMoveLow.x);
	m_apLiquid[LIQUID_LOW]->SetMoveV(texMove.texMoveLow.y);

	// ��t�̂̃e�N�X�`���ړ��ʂ�ݒ�
	m_apLiquid[LIQUID_HIGH]->SetMoveU(texMove.texMoveHigh.x);
	m_apLiquid[LIQUID_HIGH]->SetMoveV(texMove.texMoveHigh.y);
}

//============================================================
//	�e�N�X�`���ړ��ʎ擾����
//============================================================
CLiquid::STexMove CLiquid::GetTexMove(void) const
{
	// �ϐ���錾
	STexMove texMove;	// �e�N�X�`���ړ���

	// ���t�̂̃e�N�X�`���ړ��ʂ�ݒ�
	texMove.texMoveLow.x = m_apLiquid[LIQUID_LOW]->GetMoveU();
	texMove.texMoveLow.y = m_apLiquid[LIQUID_LOW]->GetMoveV();

	// ��t�̂̃e�N�X�`���ړ��ʂ�ݒ�
	texMove.texMoveHigh.x = m_apLiquid[LIQUID_HIGH]->GetMoveU();
	texMove.texMoveHigh.y = m_apLiquid[LIQUID_HIGH]->GetMoveV();

	// �e�N�X�`���ړ��ʂ�Ԃ�
	return texMove;
}

//============================================================
//	�g�̍ō��㏸�ʂ̐ݒ菈��
//============================================================
void CLiquid::SetMaxUp(const float fMaxUp)
{
	// �����̔g�̍ō��㏸�ʂ�ݒ�
	m_fMaxUp = fMaxUp;
}

//============================================================
//	�g�̍ō��㏸�ʂ̎擾����
//============================================================
float CLiquid::GetMaxUp(void) const
{
	// �g�̍ō��㏸�ʂ�Ԃ�
	return m_fMaxUp;
}

//============================================================
//	�g�ł��������Z�ʂ̐ݒ菈��
//============================================================
void CLiquid::SetAddSinRot(const float fAddSinRot)
{
	// �����̔g�ł��������Z�ʂ�ݒ�
	m_fAddSinRot = fAddSinRot;
}

//============================================================
//	�g�ł��������Z�ʂ̎擾����
//============================================================
float CLiquid::GetAddSinRot(void) const
{
	// �g�ł��������Z�ʂ�Ԃ�
	return m_fAddSinRot;
}

//============================================================
//	�הg�̌������Z�ʂ̐ݒ菈��
//============================================================
void CLiquid::SetAddVtxRot(const float fAddVtxRot)
{
	// �����̗הg�̌������Z�ʂ�ݒ�
	m_fAddVtxRot = fAddVtxRot;
}

//============================================================
//	�הg�̌������Z�ʂ̎擾����
//============================================================
float CLiquid::GetAddVtxRot(void) const
{
	// �הg�̌������Z�ʂ�Ԃ�
	return m_fAddVtxRot;
}

//============================================================
//	�g�̃��b�V���t�B�[���h�擾����
//============================================================
CScrollMeshField *CLiquid::GetScrollMeshField(const int nID) const
{
	// �g�̃��b�V���t�B�[���h��Ԃ�
	return m_apLiquid[nID];
}

//============================================================
//	�j������
//============================================================
void CLiquid::Release(void)
{
	// �I�u�W�F�N�g�̔j��
	CObject::Release();
}
