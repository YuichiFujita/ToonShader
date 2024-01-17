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
texture	g_textureScreen;	// ��ʃe�N�X�`��

//************************************************************
//	�T���v���[�錾
//************************************************************
#if 0
sampler tex0 =	// ��ʃe�N�X�`��
sampler_state	// �T���v���[�X�e�[�g
{
	// �Ώۃe�N�X�`��
	Texture = <g_textureScreen>;	// �g�D�[���}�b�v

	// �e�N�X�`���A�h���b�V���O���[�h
	AddressU = Clamp;	// U�����̒[���������΂�
	AddressV = Clamp;	// V�����̒[���������΂�
};
#else
sampler tex0 : register(s0);	// �I�u�W�F�N�g�e�N�X�`��
#endif

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
	outCol  = saturate(outCol);

	if (outCol.r <= 0.0f)
	{
		outCol = float4(0.0f, 0.0f, 0.0f, 0.0f);

#if 0
		//int nNumEdge = 0;

		//// 3x3�͈̔͂Ńu���[��������
		//for (int i = -1; i <= 1; i++)
		//{
		//	for (int j = -1; j <= 1; j++)
		//	{
		//		// �T���v���̃I�t�Z�b�g���v�Z
		//		float2 offset = float2(i, j);

		//		// �G�b�W�e�N�X�`������d�݂��擾
		//		float weight = tex2D(tex1, inVertex.Tex + offset).r;

		//		if (weight > 0.0f)
		//		{
		//			nNumEdge++;
		//		}
		//	}
		//}

		//nNumEdge;
#endif
	}
	else
	{
		outCol = float4(1.0f, 0.0f, 0.0f, 1.0f);
	}

#if 0
	//float totalWeight = 0.0f;

	//// 3x3�͈̔͂Ńu���[��������
	//for (int i = -1; i <= 1; i++)
	//{
	//	for (int j = -1; j <= 1; j++)
	//	{
	//		// �T���v���̃I�t�Z�b�g���v�Z
	//		float2 offset = float2(i, j);

	//		// �G�b�W�e�N�X�`������d�݂��擾
	//		float weight = tex2D(tex1, inVertex.Tex + offset).r;

	//		// �G�b�W�̏d�݂��g���ē��̓e�N�X�`������F���T���v�����O���A�u���[�����ɉ��Z
	//		outCol += float4(weight * tex2D(tex0, inVertex.Tex + offset).rgb, 1.0f);

	//		// �d�݂����v
	//		totalWeight += weight;
	//	}
	//}

	//// ���v�d�݂Ŋ����Đ��K��
	//outCol /= totalWeight;
#endif
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
