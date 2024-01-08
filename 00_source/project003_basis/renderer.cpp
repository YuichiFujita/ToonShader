//============================================================
//
//	�����_���[���� [renderer.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "renderer.h"
#include "object.h"
#include "manager.h"
#include "renderState.h"
#include "camera.h"
#include "texture.h"
#include "object2D.h"
#include "shaderEdge.h"
#include "debug.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const D3DFORMAT	FORMAT_DEPTH_STENCIL = D3DFMT_D24S8;		// �[�x�X�e���V���̃t�H�[�}�b�g (�[�x�o�b�t�@�F24bit, �X�e���V���o�b�t�@�F8bit�g�p)
	const D3DCOLOR	COL_CLEAR = D3DCOLOR_RGBA(0, 0, 0, 255);	// ��ʃN���A���̐F

	const DWORD FLAG_CLEAR = (D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER);	// �N���A����o�b�t�@�[�̃r�b�g�t���O
}

//************************************************************
//	�e�N���X [CRenderer] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CRenderer::CRenderer() :
	m_pD3D				(nullptr),	// Direct3D�I�u�W�F�N�g
	m_pD3DDevice		(nullptr),	// Direct3D�f�o�C�X
	m_pDrawScreen		(nullptr),	// ��ʕ`��p��2D�|���S��
	m_nRenderTextureID	(0),		// �����_�[�e�N�X�`���̃C���f�b�N�X
	m_nZTextureID		(0),		// Z�l�e�N�X�`���̃C���f�b�N�X
	m_pRenderTextureSurface		(nullptr),	// �`��T�[�t�F�C�X�ւ̃|�C���^
	m_pDepthStencilSurface		(nullptr),	// Z�o�b�t�@�E�X�e���V���o�b�t�@�̃T�[�t�F�C�X�ւ̃|�C���^
	m_pZTexSurface				(nullptr),	// Z�l�e�N�X�`���̕`��T�[�t�F�C�X�ւ̃|�C���^
	m_pDefRenderTextureSurface	(nullptr),	// ���̕`��T�[�t�F�C�X�ۑ��p
	m_pDefDepthStencilSurface	(nullptr)	// ����Z�o�b�t�@�E�X�e���V���o�b�t�@�̃T�[�t�F�C�X�ۑ��p
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CRenderer::~CRenderer()
{

}

//============================================================
//	����������
//============================================================
HRESULT CRenderer::Init(HWND hWnd, BOOL bWindow)
{
	// �ϐ���錾
	D3DDISPLAYMODE			d3ddm;	// �f�B�X�v���C���[�h
	D3DPRESENT_PARAMETERS	d3dpp;	// �v���[���e�[�V�����p�����[�^

	// �����o�ϐ���������
	m_pD3D				= nullptr;	// Direct3D�I�u�W�F�N�g
	m_pD3DDevice		= nullptr;	// Direct3D�f�o�C�X
	m_pDrawScreen		= nullptr;	// ��ʕ`��p��2D�|���S��
	m_nRenderTextureID	= NONE_IDX;	// �����_�[�e�N�X�`���̃C���f�b�N�X

	m_pRenderTextureSurface		= nullptr;	// �`��T�[�t�F�C�X�ւ̃|�C���^
	m_pDepthStencilSurface		= nullptr;	// Z�o�b�t�@�E�X�e���V���o�b�t�@�̃T�[�t�F�C�X�ւ̃|�C���^
	m_pZTexSurface				= nullptr;	// Z�l�e�N�X�`���̕`��T�[�t�F�C�X�ւ̃|�C���^
	m_pDefRenderTextureSurface	= nullptr;	// ���̕`��T�[�t�F�C�X�ۑ��p
	m_pDefDepthStencilSurface	= nullptr;	// ����Z�o�b�t�@�E�X�e���V���o�b�t�@�̃T�[�t�F�C�X�ۑ��p

	// Direct3D�I�u�W�F�N�g�̐���
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == nullptr)
	{ // �I�u�W�F�N�g�̐����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{ // �f�B�X�v���C���[�h�̎擾�Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));			// �p�����[�^�̃[���N���A
	d3dpp.BackBufferWidth	= SCREEN_WIDTH;		// �Q�[����ʃT�C�Y (��)
	d3dpp.BackBufferHeight	= SCREEN_HEIGHT;	// �Q�[����ʃT�C�Y (����)
	d3dpp.BackBufferFormat	= d3ddm.Format;		// �o�b�N�o�b�t�@�̌`��
	d3dpp.BackBufferCount	= 1;				// �o�b�N�o�b�t�@�̐�

	d3dpp.SwapEffect		= D3DSWAPEFFECT_DISCARD;		// �_�u���o�b�t�@�̐؂�ւ� (�f���M���ɓ���)
	d3dpp.EnableAutoDepthStencil	= TRUE;					// �f�v�X�o�b�t�@�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat	= FORMAT_DEPTH_STENCIL;	// �[�x�o�b�t�@�F24bit, �X�e���V���o�b�t�@�F8bit���g�p
	d3dpp.Windowed					= bWindow;				// �E�C���h�E���[�h

	d3dpp.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;		// ���t���b�V�����[�g
	d3dpp.PresentationInterval			= D3DPRESENT_INTERVAL_DEFAULT;	// �C���^�[�o��

	// Direct3D�f�o�C�X�̐���
	if (FAILED(CreateDevice(hWnd, d3dpp)))
	{ // �f�o�C�X�̐����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �����_�[�X�e�[�g���̏�����
	CRenderState::BindDevice(&m_pD3DDevice);	// �f�o�C�X������
	CRenderState::InitRenderState();			// ���̏�����

	// �T���v���[�X�e�[�g�̐ݒ� (�e�N�X�`���̊g�k��Ԃ̐ݒ�)
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ� (�e�N�X�`���̃A���t�@�u�����h�̐ݒ�)
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP,	D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1,	D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2,	D3DTA_CURRENT);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP,	D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1,	D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2,	D3DTA_CURRENT);

	m_pD3DDevice->SetTextureStageState(1, D3DTSS_COLOROP,	D3DTOP_DISABLE);
	m_pD3DDevice->SetTextureStageState(1, D3DTSS_COLORARG1,	D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(1, D3DTSS_COLORARG2,	D3DTA_CURRENT);
	m_pD3DDevice->SetTextureStageState(1, D3DTSS_ALPHAOP,	D3DTOP_DISABLE);
	m_pD3DDevice->SetTextureStageState(1, D3DTSS_ALPHAARG1,	D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(1, D3DTSS_ALPHAARG2,	D3DTA_CURRENT);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CRenderer::Uninit(void)
{
	// ��ʕ`��p��2D�|���S���̏I��
	SAFE_UNINIT(m_pDrawScreen);

	// �`��T�[�t�F�C�X�̔j��
	SAFE_RELEASE(m_pRenderTextureSurface);

	// Z�o�b�t�@�E�X�e���V���o�b�t�@�̃T�[�t�F�C�X�̔j��
	SAFE_RELEASE(m_pDepthStencilSurface);

	// Z�l�e�N�X�`���̕`��T�[�t�F�C�X�̔j��
	SAFE_RELEASE(m_pZTexSurface);

	// ���̕`��T�[�t�F�C�X�̔j��
	SAFE_RELEASE(m_pDefRenderTextureSurface);

	// ����Z�o�b�t�@�E�X�e���V���o�b�t�@�̃T�[�t�F�C�X�j��
	SAFE_RELEASE(m_pDefDepthStencilSurface);

	// Direct3D�f�o�C�X�̔j��
	SAFE_RELEASE(m_pD3DDevice);

	// Direct3D�I�u�W�F�N�g�̔j��
	SAFE_RELEASE(m_pD3D);
}

//============================================================
//	�X�V����
//============================================================
void CRenderer::Update(void)
{
	// �I�u�W�F�N�g�̑S�X�V
	CObject::UpdateAll();
}

//============================================================
//	�`�揈��
//============================================================
void CRenderer::Draw(void)
{
	// �ϐ���錾
	HRESULT			hr;				// �ُ�I���̊m�F�p
	D3DVIEWPORT9	viewportDef;	// �J�����̃r���[�|�[�g�ۑ��p

	//--------------------------------------------------------
	//	�e�N�X�`���쐬�p�̕`��
	//--------------------------------------------------------
	// �h��Ԃ����[�h�̐ݒ�
	GET_MANAGER->GetDebug()->SetFillMode();

	// �`��T�[�t�F�C�X���쐬�������̂ɕύX
	hr = m_pD3DDevice->SetRenderTarget(0, m_pRenderTextureSurface);
	hr = m_pD3DDevice->SetRenderTarget(1, m_pZTexSurface);
	assert(SUCCEEDED(hr));

	// Z�o�b�t�@�E�X�e���V���o�b�t�@�̃T�[�t�F�C�X���쐬�������̂ɕύX
	hr = m_pD3DDevice->SetDepthStencilSurface(m_pDepthStencilSurface);
	assert(SUCCEEDED(hr));

	// �o�b�N�o�b�t�@��Z�o�b�t�@�̃N���A
	hr = m_pD3DDevice->Clear(0, nullptr, FLAG_CLEAR, COL_CLEAR, 1.0f, 0);
	assert(SUCCEEDED(hr));

	// �e�N�X�`���쐬�p�̕`��
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{ // �`��J�n�����������ꍇ

		// ���݂̃r���[�|�[�g���擾
		m_pD3DDevice->GetViewport(&viewportDef);

		// �J�����̐ݒ�
		GET_MANAGER->GetCamera()->SetCamera(CCamera::TYPE_MAIN);

		// �I�u�W�F�N�g�̑S�`��
		CObject::DrawAll();

		// �r���[�|�[�g�����ɖ߂�
		m_pD3DDevice->SetViewport(&viewportDef);

		// �`��I��
		hr = m_pD3DDevice->EndScene();
		assert(SUCCEEDED(hr));
	}

	//--------------------------------------------------------
	//	��ʗp�̕`��
	//--------------------------------------------------------
	// �h��Ԃ����[�h��ݒ�
	GET_DEVICE->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);	// �|���S����h��Ԃ�

	// �`��T�[�t�F�C�X�����ɖ߂�
	hr = m_pD3DDevice->SetRenderTarget(0, m_pDefRenderTextureSurface);
	hr = m_pD3DDevice->SetRenderTarget(1, nullptr);
	assert(SUCCEEDED(hr));

	// Z�o�b�t�@�E�X�e���V���o�b�t�@�̃T�[�t�F�C�X�����ɖ߂�
	hr = m_pD3DDevice->SetDepthStencilSurface(m_pDefDepthStencilSurface);
	assert(SUCCEEDED(hr));

	// �o�b�N�o�b�t�@��Z�o�b�t�@�̃N���A
	hr = m_pD3DDevice->Clear(0, nullptr, FLAG_CLEAR, COL_CLEAR, 1.0f, 0);
	assert(SUCCEEDED(hr));

	// ��ʂ̕`��
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{ // �`��J�n�����������ꍇ

		// ���݂̃r���[�|�[�g���擾
		m_pD3DDevice->GetViewport(&viewportDef);

		// �J�����̐ݒ�
		GET_MANAGER->GetCamera()->SetCamera(CCamera::TYPE_MAIN);

		// �T���v���[�X�e�[�g��ݒ�
		m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP); // U�����̃��b�s���O�𖳌���
		m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP); // V�����̃��b�s���O�𖳌���
		
		// ��ʕ`��p��2D�|���S���̕`��
		m_pDrawScreen->Draw();

		CTexture *p = GET_MANAGER->GetTexture();
		CEdgeShader::GetInstance()->SetRefValue(0.05f);
		CEdgeShader::GetInstance()->Render(p->GetTexture(m_nRenderTextureID), p->GetTexture(m_nZTextureID));

		// �f�o�b�O�\���̕`��
		GET_MANAGER->GetDebugProc()->Draw();

		// �T���v���[�X�e�[�g��ݒ�
		m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// U�����̃��b�s���O��L����
		m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// V�����̃��b�s���O��L����

		// �r���[�|�[�g�����ɖ߂�
		m_pD3DDevice->SetViewport(&viewportDef);

		// �`��I��
		hr = m_pD3DDevice->EndScene();
		assert(SUCCEEDED(hr));
	}

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	hr = m_pD3DDevice->Present(nullptr, nullptr, nullptr, nullptr);
	assert(SUCCEEDED(hr));
}

//============================================================
//	�����_�[�e�N�X�`���[��������
//============================================================
HRESULT CRenderer::CreateRenderTexture(void)
{
	// �ϐ���錾
	HRESULT hr;	// �ُ�I���̊m�F�p

	// �|�C���^��錾
	CTexture *pTexture = GET_MANAGER->GetTexture();	// �e�N�X�`���ւ̃|�C���^
	assert(pTexture != nullptr);

	// �����_�[�e�N�X�`���𐶐�
	m_nRenderTextureID = pTexture->Regist(CTexture::SInfo
	( // ����
		SCREEN_WIDTH,			// �e�N�X�`������
		SCREEN_HEIGHT,			// �e�N�X�`���c��
		1,						// �~�b�v�}�b�v���x��
		D3DUSAGE_RENDERTARGET,	// �����E�m�ۃI�v�V����
		D3DFMT_X8R8G8B8,		// �s�N�Z���t�H�[�}�b�g
		D3DPOOL_DEFAULT			// �i�[������
	));

	// Z�l�e�N�X�`���𐶐�
	m_nZTextureID = pTexture->Regist(CTexture::SInfo
	( // ����
		SCREEN_WIDTH,			// �e�N�X�`������
		SCREEN_HEIGHT,			// �e�N�X�`���c��
		1,						// �~�b�v�}�b�v���x��
		D3DUSAGE_RENDERTARGET,	// �����E�m�ۃI�v�V����
		D3DFMT_A8R8G8B8,		// �s�N�Z���t�H�[�}�b�g
		D3DPOOL_DEFAULT			// �i�[������
	));

	// ��ʕ`��p��2D�|���S���̐���
	if (m_pDrawScreen == nullptr)
	{ // ��ʕ`��p��2D�|���S������g�p���̏ꍇ

		// ��ʕ`��p��2D�|���S���̐���
		m_pDrawScreen = CObject2D::Create(SCREEN_CENT, SCREEN_SIZE);
		if (m_pDrawScreen == nullptr)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �e�N�X�`��������
		m_pDrawScreen->BindTexture(m_nRenderTextureID);

		// ���x�����X�N���[���ɐݒ�
		m_pDrawScreen->SetLabel(CObject::LABEL_SCREEN);

		// �����`���OFF�ɂ���
		m_pDrawScreen->SetEnableDraw(false);
	}
	else { assert(false); return E_FAIL; }

	// �e�N�X�`���p�T�[�t�F�C�X�̐���
	hr = m_pD3DDevice->CreateDepthStencilSurface
	( // ����
		SCREEN_WIDTH,				// �[�x�X�e���V���̃T�[�t�F�X����
		SCREEN_HEIGHT,				// �[�x�X�e���V���̃T�[�t�F�X�c��
		FORMAT_DEPTH_STENCIL,		// �[�x�X�e���V���̃T�[�t�F�X�`��
		D3DMULTISAMPLE_NONE,		// �}���`�T���v�����O�̃o�b�t�@�[�^
		0,							// �i�����x��
		FALSE,						// Z�o�b�t�@�j���̗L��/����
		&m_pDepthStencilSurface,	// Z�o�b�t�@�E�X�e���V���o�b�t�@�̃T�[�t�F�C�X�ւ̃|�C���^
		nullptr						// nullptr
	);
	if (FAILED(hr))
	{ // �`���̐����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �����_�[�e�N�X�`���̕`��T�[�t�F�C�X�擾
	hr = pTexture->GetTexture(m_nRenderTextureID)->GetSurfaceLevel
	( // ����
		0,							// �~�b�v�}�b�v���x��
		&m_pRenderTextureSurface	// �`��T�[�t�F�C�X�ւ̃|�C���^
	);
	if (FAILED(hr))
	{ // �T�[�t�F�C�X�̎擾�Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// Z�l�e�N�X�`���̕`��T�[�t�F�C�X�擾
	hr = pTexture->GetTexture(m_nZTextureID)->GetSurfaceLevel
	( // ����
		0,				// �~�b�v�}�b�v���x��
		&m_pZTexSurface	// �`��T�[�t�F�C�X�ւ̃|�C���^
	);
	if (FAILED(hr))
	{ // �T�[�t�F�C�X�̎擾�Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ���̕`��T�[�t�F�C�X��ۑ�
	hr = m_pD3DDevice->GetRenderTarget(0, &m_pDefRenderTextureSurface);
	if (FAILED(hr))
	{ // �`��T�[�t�F�C�X�̎擾�Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ����Z�o�b�t�@�E�X�e���V���o�b�t�@�̃T�[�t�F�C�X��ۑ�
	hr = m_pD3DDevice->GetDepthStencilSurface(&m_pDefDepthStencilSurface);
	if (FAILED(hr))
	{ // Z�o�b�t�@�E�X�e���V���o�b�t�@�̎擾�Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�f�o�C�X�擾����
//============================================================
LPDIRECT3DDEVICE9 CRenderer::GetDevice(void) const
{
	// �f�o�C�X�̃|�C���^��Ԃ�
	return m_pD3DDevice;
}

//============================================================
//	��������
//============================================================
CRenderer *CRenderer::Create(HWND hWnd, BOOL bWindow)
{
	// �����_���[�̐���
	CRenderer *pRenderer = new CRenderer;
	if (pRenderer == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �����_���[�̏�����
		if (FAILED(pRenderer->Init(hWnd, bWindow)))
		{ // �������Ɏ��s�����ꍇ

			// �����_���[�̔j��
			SAFE_DELETE(pRenderer);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pRenderer;
	}
}

//============================================================
//	�j������
//============================================================
void CRenderer::Release(CRenderer *&prRenderer)
{
	// �����_���[�̏I��
	assert(prRenderer != nullptr);
	prRenderer->Uninit();

	// �������J��
	SAFE_DELETE(prRenderer);
}

//============================================================
//	�f�o�C�X��������
//============================================================
HRESULT CRenderer::CreateDevice(HWND hWnd, D3DPRESENT_PARAMETERS d3dpp)
{
	// �ϐ���錾
	HRESULT hr;	// �ُ�I���̊m�F�p

	// �`�揈���ƒ��_�������n�[�h�E�F�A���s���ݒ�Ńf�o�C�X�𐶐�
	hr = m_pD3D->CreateDevice
	( // ����
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&m_pD3DDevice
	);
	if (SUCCEEDED(hr))
	{ // �����ɐ��������ꍇ

		return S_OK;
	}

	// CPU�����\�s���Ńf�o�C�X�����Ɏ��s�����ꍇ
	// �`�揈���̓n�[�h�E�F�A�A���_������CPU���s���ݒ�Ńf�o�C�X�𐶐�
	hr = m_pD3D->CreateDevice
	( // ����
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&m_pD3DDevice
	);
	if (SUCCEEDED(hr))
	{ // �����ɐ��������ꍇ

		return S_OK;
	}

	// �n�[�h�E�F�A�����\�s���Ńf�o�C�X�����Ɏ��s�����ꍇ
	// �`�揈���ƒ��_������CPU���s���ݒ�Ńf�o�C�X�𐶐�
	hr = m_pD3D->CreateDevice
	( // ����
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_REF,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&m_pD3DDevice
	);
	if (SUCCEEDED(hr))
	{ // �����ɐ��������ꍇ

		return S_OK;
	}

	// CPU�ƃn�[�h�E�F�A�̐��\���g�����ɂȂ�Ȃ������ꍇ
	// �f�o�C�X�����̎��s��Ԃ�
	return E_FAIL;
}
