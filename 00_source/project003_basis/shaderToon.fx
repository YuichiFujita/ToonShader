//============================================================
//
//	�g�D�[���V�F�[�_�[�̃G�t�F�N�g�t�@�C�� [shaderToon.fx]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�O���[�o���ϐ��錾
//************************************************************
float4x4 g_mtxWorld	: WORLD;		// ���[���h�}�g���b�N�X
float4x4 g_mtxView	: VIEW;			// �r���[�}�g���b�N�X
float4x4 g_mtxProj	: PROJECTION;	// �v���W�F�N�V�����}�g���b�N�X
texture  g_textureToon;				// �g�D�[���}�b�v�e�N�X�`��

float3	g_dirLight	= float3(0.0f, 0.0f, 0.0f);			// ���s�����̕����x�N�g��
float4	g_diffuse	= float4(1.0f, 1.0f, 1.0f, 1.0f);	// �g�U��
bool	g_bUseTex	= false;	// �e�N�X�`���w���

//************************************************************
//	�T���v���[�錾
//************************************************************
sampler tex0 : register(s0);	// �I�u�W�F�N�g�e�N�X�`��
sampler tex1 =	// �g�D�[���}�b�v�e�N�X�`��
sampler_state	// �T���v���[�X�e�[�g
{
	// �Ώۃe�N�X�`��
	Texture = <g_textureToon>;	// �g�D�[���}�b�v

	// �g�k�E�~�b�v�}�b�v
	Filter = MIN_MAG_MIP_LINEAR;	// ���`�⊮

	// �e�N�X�`���A�h���b�V���O���[�h
	AddressU = Clamp;	// U�����̒[���������΂�
	AddressV = Clamp;	// V�����̒[���������΂�
};

//************************************************************
//	�\���̒�`
//************************************************************
// ���_�V�F�[�_�[�o�͏��
struct VS_OUTPUT
{
	float4 Pos		: POSITION;		// ���_���W
	float2 Tex		: TEXCOORD0;	// �e�N�Z�����W
	float3 Normal	: TEXCOORD1;	// �@��
};

//************************************************************
//	�O���[�o���֐�
//************************************************************
//============================================================
//	���_���W�̎ˉe�ϊ�
//============================================================
float4 TransVertex
(
	float4   Pos		: POSITION,		// ���_���W
	float4x4 MtxWorld	: WORLD,		// ���[���h�}�g���b�N�X
	float4x4 MtxView	: VIEW,			// �r���[�}�g���b�N�X
	float4x4 MtxProj	: PROJECTION	// �v���W�F�N�V�����}�g���b�N�X
)
{
	// ���_���W���ˉe�ϊ�
	Pos = mul(Pos, MtxWorld);
	Pos = mul(Pos, MtxView);
	Pos = mul(Pos, MtxProj);

	// �ϊ���̒��_���W��Ԃ�
	return Pos;
}

//============================================================
//	���_�V�F�[�_�[
//============================================================
VS_OUTPUT VS
(
	float4 Pos		: POSITION,	// ���_���W
	float4 Normal	: NORMAL,	// �@���x�N�g��
	float2 Tex		: TEXCOORD0	// �e�N�Z��
)
{
	// �ϐ���錾
	VS_OUTPUT Out;	// ���_���

	// ���[�J�����_���W���ˉe�ϊ����ݒ�
	Out.Pos = TransVertex(Pos, g_mtxWorld, g_mtxView, g_mtxProj);

	// �@���x�N�g�����e�N�Z�������ݒ�
	Out.Normal = normalize(Normal.xyz);

	// �e�N�Z�����W��ݒ�
	Out.Tex = Tex;

	// ���_����Ԃ�
	return Out;
}

//============================================================
//	�s�N�Z���V�F�[�_�[
//============================================================
float4 PS(VS_OUTPUT In) : COLOR0
{
	// �ϐ���錾
	float4 Out;	// �s�N�Z���F���

	// �ʏ탉�C�e�B���O�̌v�Z���s��
	float fLight = dot(normalize(In.Normal), normalize(-g_dirLight));
	fLight = clamp(fLight, 0.0f, 1.0f);
	fLight *= 0.5f + 0.5f;
	fLight *= fLight;

	// �ʏ탉�C�e�B���O�̌��ʂ��A�g�D�[���}�b�v�e�N�X�`���[�̃e�N�Z����U�����ɕϊ�����
	float4 ToonLight = tex2D(tex1, float2(fLight, 0.5f));

	// �s�N�Z���F����ݒ肷��
	Out = g_diffuse;	// �g�U����ݒ�
	Out *= ToonLight;	// �g�D�[���}�b�v����Z

	if (g_bUseTex)
	{ // �e�N�X�`�����w�肳��Ă���ꍇ

		// �e�N�Z���̐F����Z
		Out *= tex2D(tex0, In.Tex);
	}

	// �s�N�Z���F����Ԃ�
	return Out;
}

//============================================================
//	�e�N�j�b�N�֐�
//============================================================
technique TShader
{
	pass P0
	{
		VertexShader = compile vs_1_1 VS();
		PixelShader  = compile ps_2_0 PS();
	}
}
