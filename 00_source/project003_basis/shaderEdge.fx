//============================================================
//
//	トゥーンシェーダーのエフェクトファイル [shaderToon.fx]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	グローバル変数宣言
//************************************************************
float2	m_Texel;	// 1テクセルのサイズ
float	m_Limit;	// エッジとなるかを判定するための基準値。1.0fでエッジフィルターが無効になる。

//************************************************************
//	サンプラー宣言
//************************************************************
sampler tex0 : register(s0);	// シーンのカラー情報を格納したテクスチャ
sampler tex1 : register(s1);	// シーンのZ値を格納したテクスチャ

//************************************************************
//	構造体定義
//************************************************************
// 頂点シェーダー出力情報
struct VS_OUTPUT
{
	float4 Pos	: POSITION;		// 頂点座標
	float2 Tex	: TEXCOORD0;	// テクセル座標
};

//************************************************************
//	グローバル関数
//************************************************************
//============================================================
//	頂点シェーダー
//============================================================
void VS
(
	in	float4		inPos	: POSITION,		// 頂点座標
	in	float4		inCol	: COLOR0,		// 頂点色
	in	float2		inTex	: TEXCOORD0,	// テクセル座標
	out	VS_OUTPUT	outVertex				// 頂点情報
)
{
	// 頂点座標を設定
	outVertex.Pos = inPos;

	// テクセル座標を設定
	outVertex.Tex = inTex;
}

//============================================================
//	ピクセルシェーダー
//============================================================
void PS
(
	in	VS_OUTPUT	inVertex,		// 頂点情報
	out	float4		outCol : COLOR0	// ピクセル色
)
{
	// Z値情報で書き込んであるテクスチャーを参照し、現在のピクセル位置のＺ値を取得
	float Z1 = tex2D(tex1, inVertex.Tex).r;

	// 周辺のピクセル位置のＺ値を取得
	float Z2 = tex2D(tex1, clamp(inVertex.Tex + float2( m_Texel.x,  m_Texel.y), 0.0f, 1.0f)).r;
	float Z3 = tex2D(tex1, clamp(inVertex.Tex + float2( m_Texel.x, -m_Texel.y), 0.0f, 1.0f)).r;
	float Z4 = tex2D(tex1, clamp(inVertex.Tex + float2(-m_Texel.x,  m_Texel.y), 0.0f, 1.0f)).r;
	float Z5 = tex2D(tex1, clamp(inVertex.Tex + float2(-m_Texel.x, -m_Texel.y), 0.0f, 1.0f)).r;

	// 周辺のピクセル位置のZ値と比較し、m_Limitより大きいところがあったらエッジ部分と判定する
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
//	テクニック関数
//============================================================
technique TShader
{
	pass P0
	{
		VertexShader = compile vs_1_1 VS();
		PixelShader  = compile ps_2_0 PS();
	}
}
