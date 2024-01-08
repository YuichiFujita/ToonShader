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
bool	g_bUseTex	= false;	// テクスチャ指定状況

//************************************************************
//	サンプラー宣言
//************************************************************
sampler tex0 : register(s0);	// オブジェクトテクスチャ
sampler tex1 =	// トゥーンマップテクスチャ
sampler_state	// サンプラーステート
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
	float4 Pos		: POSITION;		// 頂点座標
	float2 Tex		: TEXCOORD0;	// テクセル座標
	float3 Normal	: TEXCOORD1;	// 法線
};

//************************************************************
//	グローバル関数
//************************************************************
//============================================================
//	頂点座標の射影変換
//============================================================
float4 TransVertex
(
	float4   Pos		: POSITION,		// 頂点座標
	float4x4 MtxWorld	: WORLD,		// ワールドマトリックス
	float4x4 MtxView	: VIEW,			// ビューマトリックス
	float4x4 MtxProj	: PROJECTION	// プロジェクションマトリックス
)
{
	// 頂点座標を射影変換
	Pos = mul(Pos, MtxWorld);
	Pos = mul(Pos, MtxView);
	Pos = mul(Pos, MtxProj);

	// 変換後の頂点座標を返す
	return Pos;
}

//============================================================
//	頂点シェーダー
//============================================================
VS_OUTPUT VS
(
	float4 Pos		: POSITION,	// 頂点座標
	float4 Normal	: NORMAL,	// 法線ベクトル
	float2 Tex		: TEXCOORD0	// テクセル
)
{
	// 変数を宣言
	VS_OUTPUT Out;	// 頂点情報

	// ローカル頂点座標を射影変換し設定
	Out.Pos = TransVertex(Pos, g_mtxWorld, g_mtxView, g_mtxProj);

	// 法線ベクトルをテクセル化し設定
	Out.Normal = normalize(Normal.xyz);

	// テクセル座標を設定
	Out.Tex = Tex;

	// 頂点情報を返す
	return Out;
}

//============================================================
//	ピクセルシェーダー
//============================================================
float4 PS(VS_OUTPUT In) : COLOR0
{
	// 変数を宣言
	float4 Out;	// ピクセル色情報

	// 通常ライティングの計算を行う
	float fLight = dot(normalize(In.Normal), normalize(-g_dirLight));
	fLight = clamp(fLight, 0.0f, 1.0f);
	fLight *= 0.5f + 0.5f;
	fLight *= fLight;

	// 通常ライティングの光量を、トゥーンマップテクスチャーのテクセルのU成分に変換する
	float4 ToonLight = tex2D(tex1, float2(fLight, 0.5f));

	// ピクセル色情報を設定する
	Out = g_diffuse;	// 拡散光を設定
	Out *= ToonLight;	// トゥーンマップを乗算

	if (g_bUseTex)
	{ // テクスチャが指定されている場合

		// テクセルの色を乗算
		Out *= tex2D(tex0, In.Tex);
	}

	// ピクセル色情報を返す
	return Out;
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
