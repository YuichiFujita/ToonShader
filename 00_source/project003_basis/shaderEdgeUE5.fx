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
sampler tex1 =	// �V�[����Z�l���i�[�����e�N�X�`��
sampler_state	// �T���v���[�X�e�[�g
{
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
	float4 Pos	: POSITION;		// ���_���W
	float2 Tex	: TEXCOORD0;	// �e�N�Z�����W
};

//************************************************************
//	�O���[�o���֐�
//************************************************************
//============================================================
//	
//============================================================
float4 CalcGapTextureColor
(
	in float2 inTex : TEXCOORD0,	// �e�N�Z�����W
	in float2 inGap					// �e�N�Z�������
)
{
	// �ϐ���錾
	float2 texGap = inTex + (m_Texel * inGap);	// ���炵���e�N�Z�����W

	// 
	float4 outCol = tex2D(tex1, inTex) - tex2D(tex1, texGap);
	outCol.a = 1.0f;	// ���l�͕ύX���Ȃ�

	return outCol;
}

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
	outCol  = CalcGapTextureColor(inVertex.Tex, float2( 2.0f,  0.0f));
	outCol += CalcGapTextureColor(inVertex.Tex, float2(-2.0f,  0.0f));
	outCol += CalcGapTextureColor(inVertex.Tex, float2( 0.0f,  2.0f));
	outCol += CalcGapTextureColor(inVertex.Tex, float2( 0.0f, -2.0f));

	outCol = saturate(outCol);
	outCol = lerp(float4(0.0f, 0.0f, 0.0f, 0.0f), float4(0.0f, 0.0f, 0.0f, 1.0f), outCol.r);
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
