//============================================================
//
//	�g�D�[���V�F�[�_�[�̃G�t�F�N�g�t�@�C�� [shaderToon.fx]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�O���[�o���ϐ��錾
//************************************************************
float2	m_Texel;	// 1�e�N�Z���̃T�C�Y
float	m_Limit;	// �G�b�W�ƂȂ邩�𔻒肷�邽�߂̊�l�B1.0f�ŃG�b�W�t�B���^�[�������ɂȂ�B

//************************************************************
//	�T���v���[�錾
//************************************************************
sampler tex0 : register(s0);	// �V�[���̃J���[�����i�[�����e�N�X�`��
sampler tex1 : register(s1);	// �V�[����Z�l���i�[�����e�N�X�`��

//************************************************************
//	�\���̒�`
//************************************************************
// ���_�V�F�[�_�[�o�͏��
struct VS_OUTPUT
{
	float4 Pos	: POSITION;		// ���_���W
	float2 Tex	: TEXCOORD0;	// �e�N�Z�����W
};

//************************************************************
//	�O���[�o���֐�
//************************************************************
//============================================================
//	���_�V�F�[�_�[
//============================================================
void VS
(
	in	float4		inPos	: POSITION,		// ���_���W
	in	float4		inCol	: COLOR0,		// ���_�F
	in	float2		inTex	: TEXCOORD0,	// �e�N�Z�����W
	out	VS_OUTPUT	outVertex				// ���_���
)
{
	// ���_���W��ݒ�
	outVertex.Pos = inPos;

	// �e�N�Z�����W��ݒ�
	outVertex.Tex = inTex;
}

//============================================================
//	�s�N�Z���V�F�[�_�[
//============================================================
void PS
(
	in	VS_OUTPUT	inVertex,		// ���_���
	out	float4		outCol : COLOR0	// �s�N�Z���F
)
{
	// Z�l���ŏ�������ł���e�N�X�`���[���Q�Ƃ��A���݂̃s�N�Z���ʒu�̂y�l���擾
	float Z1 = tex2D(tex1, inVertex.Tex).r;

	// ���ӂ̃s�N�Z���ʒu�̂y�l���擾
	float Z2 = tex2D(tex1, clamp(inVertex.Tex + float2( m_Texel.x,  m_Texel.y), 0.0f, 1.0f)).r;
	float Z3 = tex2D(tex1, clamp(inVertex.Tex + float2( m_Texel.x, -m_Texel.y), 0.0f, 1.0f)).r;
	float Z4 = tex2D(tex1, clamp(inVertex.Tex + float2(-m_Texel.x,  m_Texel.y), 0.0f, 1.0f)).r;
	float Z5 = tex2D(tex1, clamp(inVertex.Tex + float2(-m_Texel.x, -m_Texel.y), 0.0f, 1.0f)).r;

	// ���ӂ̃s�N�Z���ʒu��Z�l�Ɣ�r���Am_Limit���傫���Ƃ��낪��������G�b�W�����Ɣ��肷��
	if (abs(Z1 - Z2) < m_Limit
	&&  abs(Z1 - Z3) < m_Limit
	&&  abs(Z1 - Z4) < m_Limit
	&&  abs(Z1 - Z5) < m_Limit)
	{
		outCol = float4(0.0f, 0.0f, 0.0f, 0.0f);
	}
	else
	{
		outCol = float4(0.0f, 0.0f, 0.0f, 1.0f);
	}
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
