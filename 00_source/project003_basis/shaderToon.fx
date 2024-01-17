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
float4	g_ambient	= float4(0.0f, 0.0f, 0.0f, 0.0f);	// ����
float4	g_emissive	= float4(0.0f, 0.0f, 0.0f, 0.0f);	// ���ˌ�
float	g_fRefEdge	= 1.0f;		// �G�b�W�����Q�ƒl
bool	g_bUseTex	= false;	// �e�N�X�`���w���

//************************************************************
//	�T���v���[�錾
//************************************************************
sampler texObject : register(s0);	// �I�u�W�F�N�g�e�N�X�`��
sampler texToon =	// �g�D�[���}�b�v�e�N�X�`��
sampler_state		// �T���v���[�X�e�[�g
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
	float4 pos	: POSITION;		// ���_���W
	float2 tex	: TEXCOORD0;	// �e�N�Z�����W
	float3 nor	: TEXCOORD1;	// �@��
};

//************************************************************
//	�O���[�o���֐�
//************************************************************
//============================================================
//	���_���W�̎ˉe�ϊ�
//============================================================
float4 TransVertex
(
	in float4	inPos		: POSITION,		// ���_���W
	in float4x4	inMtxWorld	: WORLD,		// ���[���h�}�g���b�N�X
	in float4x4	inMtxView	: VIEW,			// �r���[�}�g���b�N�X
	in float4x4	inMtxProj	: PROJECTION	// �v���W�F�N�V�����}�g���b�N�X
)
{
	// ���_���W���ˉe�ϊ�
	inPos = mul(inPos, inMtxWorld);
	inPos = mul(inPos, inMtxView);
	inPos = mul(inPos, inMtxProj);

	// �ϊ���̒��_���W��Ԃ�
	return inPos;
}

//============================================================
//	���_�V�F�[�_�[
//============================================================
void VS
(
	in	float4		inPos	: POSITION,		// ���_���W
	in	float4		inNor	: NORMAL,		// �@���x�N�g��
	in	float2		inTex	: TEXCOORD0,	// �e�N�Z�����W
	out	VS_OUTPUT	outVertex				// ���_���
)
{
	// ���[�J�����_���W���ˉe�ϊ����ݒ�
	outVertex.pos = TransVertex(inPos, g_mtxWorld, g_mtxView, g_mtxProj);

	// �@���x�N�g�����e�N�Z�������ݒ�
	outVertex.nor = normalize(inNor.xyz);

	// �e�N�Z�����W��ݒ�
	outVertex.tex = inTex;
}

//============================================================
//	�s�N�Z���V�F�[�_�[
//============================================================
void PS
(
	in	VS_OUTPUT	inVertex,			// ���_���
	out	float4		outCol	: COLOR0,	// �s�N�Z���F
	out	float4		outRef	: COLOR1	// �s�N�Z�������Q�ƒl
)
{
	// �ϐ���錾
	float  fLight  = 0.0f;	// ���C�e�B���O����
	float4 toonCol = float4(0.0f, 0.0f, 0.0f, 0.0f);	// �g�D�[���}�b�v�e�N�Z���F

	// �ʏ탉�C�e�B���O�̌��ʂ����߂�
	fLight = dot(normalize(inVertex.nor), normalize(-g_dirLight));
	fLight = fLight * 0.5f + 0.5f;
	fLight = fLight * fLight;

	// ���C�e�B���O�̌��ʂ��g�D�[���}�b�v�̃e�N�Z��U�����ɕϊ�
	toonCol = tex2D(texToon, float2(fLight, 0.5f));

	// �s�N�Z���̐F����ݒ�
	outCol =  g_diffuse;	// �g�U����ݒ�
	outCol += g_ambient;	// ���������Z
	outCol += g_emissive;	// ���ˌ������Z
	outCol *= toonCol;		// �g�D�[���}�b�v�e�N�Z���F����Z

	if (g_bUseTex)
	{ // �e�N�X�`�����w�肳��Ă���ꍇ

		// �e�N�Z���̐F����Z
		outCol *= tex2D(texObject, inVertex.tex);
	}

	// �s�N�Z���̉����Q�ƒl��ݒ�
	outRef = float4(g_fRefEdge, g_fRefEdge, g_fRefEdge, 1.0f);
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
