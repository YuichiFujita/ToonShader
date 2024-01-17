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
texture	g_textureScreen;	// 画面テクスチャ

//************************************************************
//	サンプラー宣言
//************************************************************
#if 0
sampler tex0 =	// 画面テクスチャ
sampler_state	// サンプラーステート
{
	// 対象テクスチャ
	Texture = <g_textureScreen>;	// トゥーンマップ

	// テクスチャアドレッシングモード
	AddressU = Clamp;	// U成分の端を引き延ばす
	AddressV = Clamp;	// V成分の端を引き延ばす
};
#else
sampler tex0 : register(s0);	// オブジェクトテクスチャ
#endif

sampler tex1 =	// シーンのZ値を格納したテクスチャ
sampler_state	// サンプラーステート
{
	// テクスチャアドレッシングモード
	AddressU = Clamp;	// U成分の端を引き延ばす
	AddressV = Clamp;	// V成分の端を引き延ばす
};

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
//	
//============================================================
float4 CalcGapTextureColor
(
	in float2 inTex : TEXCOORD0,	// テクセル座標
	in float2 inGap					// テクセルずれ量
)
{
	// 変数を宣言
	float2 texGap = inTex + (m_Texel * inGap);	// ずらしたテクセル座標

	// 
	float4 outCol = tex2D(tex1, inTex) - tex2D(tex1, texGap);
	outCol.a = 1.0f;	// α値は変更しない

	return outCol;
}

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

		//// 3x3の範囲でブラーをかける
		//for (int i = -1; i <= 1; i++)
		//{
		//	for (int j = -1; j <= 1; j++)
		//	{
		//		// サンプルのオフセットを計算
		//		float2 offset = float2(i, j);

		//		// エッジテクスチャから重みを取得
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

	//// 3x3の範囲でブラーをかける
	//for (int i = -1; i <= 1; i++)
	//{
	//	for (int j = -1; j <= 1; j++)
	//	{
	//		// サンプルのオフセットを計算
	//		float2 offset = float2(i, j);

	//		// エッジテクスチャから重みを取得
	//		float weight = tex2D(tex1, inVertex.Tex + offset).r;

	//		// エッジの重みを使って入力テクスチャから色をサンプリングし、ブラー処理に加算
	//		outCol += float4(weight * tex2D(tex0, inVertex.Tex + offset).rgb, 1.0f);

	//		// 重みを合計
	//		totalWeight += weight;
	//	}
	//}

	//// 合計重みで割って正規化
	//outCol /= totalWeight;
#endif
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
