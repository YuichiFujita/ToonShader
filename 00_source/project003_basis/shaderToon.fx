//============================================================
//
//	トゥーンシェーダーのエフェクトファイル [shaderToon.fx]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	グローバル変数宣言
//************************************************************
float4x4 g_mtxWorld	: WORLD;		// ワールドマトリックス
float4x4 g_mtxView	: VIEW;			// ビューマトリックス
float4x4 g_mtxProj	: PROJECTION;	// プロジェクションマトリックス
texture  g_textureToon;				// トゥーンマップテクスチャ

float3	g_dirLight	= float3(0.0f, 0.0f, 0.0f);			// 平行光源の方向ベクトル
float4	g_diffuse	= float4(1.0f, 1.0f, 1.0f, 1.0f);	// 拡散光
bool	g_bUseTex	= false;							// テクスチャ指定状況

//************************************************************
//	サンプラー宣言
//************************************************************
sampler texObject : register(s0);	// オブジェクトテクスチャ
sampler texToon =	// トゥーンマップテクスチャ
sampler_state		// サンプラーステート
{
	// 対象テクスチャ
	Texture = <g_textureToon>;	// トゥーンマップ

	// 拡縮・ミップマップ
	Filter = MIN_MAG_MIP_LINEAR;	// 線形補完

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
	float4 pos		: POSITION;		// 頂点座標
	float2 tex		: TEXCOORD0;	// テクセル座標
	float3 nor		: TEXCOORD1;	// 法線
	float4 depth	: TEXCOORD2;	// Z値
};

//************************************************************
//	グローバル関数
//************************************************************
//============================================================
//	頂点座標の射影変換
//============================================================
float4 TransVertex
(
	in float4	inPos		: POSITION,		// 頂点座標
	in float4x4	inMtxWorld	: WORLD,		// ワールドマトリックス
	in float4x4	inMtxView	: VIEW,			// ビューマトリックス
	in float4x4	inMtxProj	: PROJECTION	// プロジェクションマトリックス
)
{
	// 頂点座標を射影変換
	inPos = mul(inPos, inMtxWorld);
	inPos = mul(inPos, inMtxView);
	inPos = mul(inPos, inMtxProj);

	// 変換後の頂点座標を返す
	return inPos;
}

//============================================================
//	頂点シェーダー
//============================================================
void VS
(
	in	float4		inPos	: POSITION,		// 頂点座標
	in	float4		inNor	: NORMAL,		// 法線ベクトル
	in	float2		inTex	: TEXCOORD0,	// テクセル座標
	out	VS_OUTPUT	outVertex				// 頂点情報
)
{
	// ローカル頂点座標を射影変換し設定
	outVertex.pos = TransVertex(inPos, g_mtxWorld, g_mtxView, g_mtxProj);

	// 法線ベクトルをテクセル化し設定
	outVertex.nor = normalize(inNor.xyz);

	// テクセル座標を設定
	outVertex.tex = inTex;

	// テクスチャ座標を頂点に合わせる
	outVertex.depth = outVertex.pos;
}

//============================================================
//	ピクセルシェーダー
//============================================================
void PS
(
	in	VS_OUTPUT	inVertex,			// 頂点情報
	out	float4		outCol	: COLOR0,	// ピクセル色
	out	float4		outZ	: COLOR1	// ピクセルZ値
)
{
	// 変数を宣言
	float  fLight  = 0.0f;	// ライティング光量
	float4 toonCol = float4(0.0f, 0.0f, 0.0f, 0.0f);	// トゥーンマップテクセル色

	// 通常ライティングの光量を求める
	fLight = dot(normalize(inVertex.nor), normalize(-g_dirLight));
	fLight = fLight * 0.5f + 0.5f;
	fLight = fLight * fLight;

	// ライティングの光量をトゥーンマップのテクセルU成分に変換
	toonCol = tex2D(texToon, float2(fLight, 0.5f));

	// ピクセルの色情報を設定
	outCol = g_diffuse;	// 拡散光を設定
	outCol *= toonCol;	// トゥーンマップテクセル色を乗算

	if (g_bUseTex)
	{ // テクスチャが指定されている場合

		// テクセルの色を乗算
		outCol *= tex2D(texObject, inVertex.tex);
	}

	// 深度情報を格納する。
	// Z / W により 0 から 1 の間に正規化されたZ値情報に変換
	outZ = inVertex.depth.z / inVertex.depth.w;
	outZ.a = 1.0f;
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
