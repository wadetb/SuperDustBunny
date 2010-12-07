//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#include "Common.h"
#include "Lighting.h"
#include "Chapter.h"


#define MAX_LIT_QUADS_PER_LIST	8192 // Holy crap
#define MAX_LIT_VERTS			65536


int NLitVerts;
SLitVertex LitVerts[MAX_LIT_VERTS];


D3DVERTEXELEMENT9 LitVertexElements[] =
{
	{ 0, offsetof(SLitVertex, X), D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
	{ 0, offsetof(SLitVertex, U), D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
	{ 0, offsetof(SLitVertex, Color), D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
	D3DDECL_END(),
};

IDirect3DVertexDeclaration9* LitVertexDecl;


struct SLightState
{
	unsigned int AmbientColor;
};

struct SLightList
{
	int NQuads;
	SLitQuad Quads[MAX_LIT_QUADS_PER_LIST];
};


SLightState LightState;

SLightList LightLists[LIGHTLIST_COUNT];


// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                      Render Targets                                                                     //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

gxSprite ColorRT;

gxSprite LightingRT;


gxSprite AmbientOcclusionAlphaRT;
gxSprite AmbientOcclusionDiv2RT;
gxSprite AmbientOcclusionDiv4RT;
gxSprite AmbientOcclusionPingRT;
gxSprite AmbientOcclusionPongRT;

gxSprite AmbientOcclusionForegroundRT;
gxSprite AmbientOcclusionVacuumRT;


gxSprite ShadowAlphaRT;
gxSprite ShadowPingRT;
gxSprite ShadowPongRT;

gxSprite ShadowForegroundRT;
gxSprite ShadowVacuumRT;


gxSprite ColorBleedDiv2RT;
gxSprite ColorBleedDiv4RT;
gxSprite ColorBleedDiv8RT;
gxSprite ColorBleedPingRT;
gxSprite ColorBleedPongRT;

gxSprite ColorBleedFinalRT;


// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                      Shaders                                                                            //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

const char* LitVertexShaderSource =
"struct SVertexInput\n"
"{\n"
"	float2 Position : POSITION;\n"
"	float2 TexCoord0 : TEXCOORD0;\n"
"	float4 Color0 : COLOR0;\n"
"};\n"
"\n"
"struct SVertexOutput\n"
"{\n"
"	float4 Position : POSITION;\n"
"	float2 TexCoord0 : TEXCOORD0;\n"
"	float4 Color0 : COLOR0;\n"
"};\n"
"\n"
"SVertexOutput main(SVertexInput VertexInput)\n"
"{\n"
"	SVertexOutput VertexOutput;\n"
"	VertexOutput.Position = float4(VertexInput.Position.x/768.0*2-1, (1-VertexInput.Position.y/1024)*2-1, 0, 1);\n"
"	VertexOutput.TexCoord0 = VertexInput.TexCoord0;\n"
"	VertexOutput.Color0 = VertexInput.Color0;\n"
"	return VertexOutput;\n"
"}\n";

gxVertexShader LitVertexShader;

const char* ShadowVertexShaderSource =
"float2 ShadowOffset : register(c0);\n"
"\n"
"struct SVertexInput\n"
"{\n"
"	float2 Position : POSITION;\n"
"	float2 TexCoord0 : TEXCOORD0;\n"
"	float4 Color0 : COLOR0;\n"
"};\n"
"\n"
"struct SVertexOutput\n"
"{\n"
"	float4 Position : POSITION;\n"
"	float2 TexCoord0 : TEXCOORD0;\n"
"};\n"
"\n"
"SVertexOutput main(SVertexInput VertexInput)\n"
"{\n"
"	float2 ScreenScale = float2(1.0/768.0, 1.0/1024.0);\n"
"\n"
"	SVertexOutput VertexOutput;\n"
"	float2 Position = VertexInput.Position + ShadowOffset;\n"
"	VertexOutput.Position = float4(Position.x*ScreenScale.x*2-1, (1-Position.y*ScreenScale.y)*2-1, 0, 1);\n"
"	VertexOutput.TexCoord0 = VertexInput.TexCoord0;\n"
"	return VertexOutput;\n"
"}\n";

gxVertexShader ShadowVertexShader;

const char* TexturedColoredShaderSource =
"sampler Sampler0 : register(s0);\n"
"\n"
"struct SVertexOutput\n"
"{\n"
"	float2 TexCoord0 : TEXCOORD0;\n"
"	float4 Color0 : COLOR0;\n"
"};\n"
"\n"
"float4 main(SVertexOutput VertexOutput) : COLOR\n"
"{\n"
"	return tex2D(Sampler0, VertexOutput.TexCoord0) * VertexOutput.Color0;\n"
"}\n";

gxPixelShader TexturedColoredShader;

const char* TexturedShadowShaderSource =
"float ShadowAlpha : register(c0);\n"
"sampler Sampler0 : register(s0);\n"
"\n"
"struct SVertexOutput\n"
"{\n"
"	float2 TexCoord0 : TEXCOORD0;\n"
"};\n"
"\n"
"float4 main(SVertexOutput VertexOutput) : COLOR\n"
"{\n"
"	return float4(0, 0, 0, ShadowAlpha * tex2D(Sampler0, VertexOutput.TexCoord0).a);\n"
"}\n";

gxPixelShader TexturedShadowShader;

const char* EffectsShaderSource =
"sampler Sampler0 : register(s0);\n"
"\n"
"struct SVertexOutput\n"
"{\n"
"	float2 TexCoord0 : TEXCOORD0;\n"
"	float4 Color0 : COLOR0;\n"
"};\n"
"\n"
"float4 main(SVertexOutput VertexOutput) : COLOR\n"
"{\n"
"	return tex2D(Sampler0, VertexOutput.TexCoord0) * VertexOutput.Color0 * 2;\n"
"}\n";

gxPixelShader EffectsShader;

const char* Gaussian7ShaderSource =
"float3 BlurOffsetScale0 : register(c0);\n"
"float3 BlurOffsetScale1 : register(c1);\n"
"float3 BlurOffsetScale2 : register(c2);\n"
"float3 BlurOffsetScale3 : register(c3);\n"
"\n"
"sampler BlurSampler : register(s0);\n"
"\n"
"struct SVertexOutput\n"
"{\n"
"	float2 TexCoord0 : TEXCOORD0;\n"
"};\n"
"\n"
"float4 main(SVertexOutput VertexOutput) : COLOR\n"
"{\n"
"	float4 color = float4(0,0,0,0);\n"
"	color += tex2D(BlurSampler, VertexOutput.TexCoord0+BlurOffsetScale0.xy) * BlurOffsetScale0.z;\n"
"	color += tex2D(BlurSampler, VertexOutput.TexCoord0+BlurOffsetScale1.xy) * BlurOffsetScale1.z;\n"
"	color += tex2D(BlurSampler, VertexOutput.TexCoord0+BlurOffsetScale2.xy) * BlurOffsetScale2.z;\n"
"	color += tex2D(BlurSampler, VertexOutput.TexCoord0                    ) * BlurOffsetScale3.z;\n"
"	color += tex2D(BlurSampler, VertexOutput.TexCoord0-BlurOffsetScale2.xy) * BlurOffsetScale2.z;\n"
"	color += tex2D(BlurSampler, VertexOutput.TexCoord0-BlurOffsetScale1.xy) * BlurOffsetScale1.z;\n"
"	color += tex2D(BlurSampler, VertexOutput.TexCoord0-BlurOffsetScale0.xy) * BlurOffsetScale0.z;\n"
"   return color;\n"
"}\n";

gxPixelShader Gaussian7Shader;


const char* Gaussian13ShaderSource =
"float3 BlurOffsetScale0 : register(c0);\n"
"float3 BlurOffsetScale1 : register(c1);\n"
"float3 BlurOffsetScale2 : register(c2);\n"
"float3 BlurOffsetScale3 : register(c3);\n"
"float3 BlurOffsetScale4 : register(c4);\n"
"float3 BlurOffsetScale5 : register(c5);\n"
"float3 BlurOffsetScale6 : register(c6);\n"
"\n"
"sampler BlurSampler : register(s0);\n"
"\n"
"struct SVertexOutput\n"
"{\n"
"	float2 TexCoord0 : TEXCOORD0;\n"
"};\n"
"\n"
"float4 main(SVertexOutput VertexOutput) : COLOR\n"
"{\n"
"	float4 color = float4(0,0,0,0);\n"
"	color += tex2D(BlurSampler, VertexOutput.TexCoord0+BlurOffsetScale0.xy) * BlurOffsetScale0.z;\n"
"	color += tex2D(BlurSampler, VertexOutput.TexCoord0+BlurOffsetScale1.xy) * BlurOffsetScale1.z;\n"
"	color += tex2D(BlurSampler, VertexOutput.TexCoord0+BlurOffsetScale2.xy) * BlurOffsetScale2.z;\n"
"	color += tex2D(BlurSampler, VertexOutput.TexCoord0+BlurOffsetScale3.xy) * BlurOffsetScale3.z;\n"
"	color += tex2D(BlurSampler, VertexOutput.TexCoord0+BlurOffsetScale4.xy) * BlurOffsetScale4.z;\n"
"	color += tex2D(BlurSampler, VertexOutput.TexCoord0+BlurOffsetScale5.xy) * BlurOffsetScale5.z;\n"
"	color += tex2D(BlurSampler, VertexOutput.TexCoord0                    ) * BlurOffsetScale6.z;\n"
"	color += tex2D(BlurSampler, VertexOutput.TexCoord0-BlurOffsetScale5.xy) * BlurOffsetScale5.z;\n"
"	color += tex2D(BlurSampler, VertexOutput.TexCoord0-BlurOffsetScale4.xy) * BlurOffsetScale4.z;\n"
"	color += tex2D(BlurSampler, VertexOutput.TexCoord0-BlurOffsetScale3.xy) * BlurOffsetScale3.z;\n"
"	color += tex2D(BlurSampler, VertexOutput.TexCoord0-BlurOffsetScale2.xy) * BlurOffsetScale2.z;\n"
"	color += tex2D(BlurSampler, VertexOutput.TexCoord0-BlurOffsetScale1.xy) * BlurOffsetScale1.z;\n"
"	color += tex2D(BlurSampler, VertexOutput.TexCoord0-BlurOffsetScale0.xy) * BlurOffsetScale0.z;\n"
"   return color;\n"
"}\n";

gxPixelShader Gaussian13Shader;


const char* CombineShaderSource =
"sampler ColorSampler : register(s0);\n"
"sampler ColorBleedSampler : register(s1);\n"
"sampler LightingSampler : register(s2);\n"
"\n"
"struct SVertexOutput\n"
"{\n"
"	float2 TexCoord0 : TEXCOORD0;\n"
"};\n"
"\n"
"float4 main(SVertexOutput VertexOutput) : COLOR\n"
"{\n"
"	float4 Color = tex2D(ColorSampler, VertexOutput.TexCoord0);\n"
"	float4 ColorBleed = tex2D(ColorBleedSampler, VertexOutput.TexCoord0);\n"
"	float4 Lighting = tex2D(LightingSampler, VertexOutput.TexCoord0);\n"
"   return (Lighting*2.0) * Color * saturate(ColorBleed*1.5);\n"
"}\n";

gxPixelShader CombineShader;

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                      Platform specific drawing                                                          //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void DrawLitQuad(SLitQuad* Quad)
{
	_gxSetTexture(Quad->Sprite);

	gxDev->SetVertexDeclaration( LitVertexDecl );
	gxDev->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, Quad->NVerts-2, Quad->Verts, sizeof(SLitVertex) );
}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                      Ambient occlusion                                                                  //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void InitAmbientOcclusion()
{
	gxCreateRenderTarget(gxScreenWidth,   gxScreenHeight,   &AmbientOcclusionAlphaRT);
	gxCreateRenderTarget(gxScreenWidth/2, gxScreenHeight/2, &AmbientOcclusionDiv2RT);
	gxCreateRenderTarget(gxScreenWidth/4, gxScreenHeight/4, &AmbientOcclusionDiv4RT);
	gxCreateRenderTarget(gxScreenWidth/8, gxScreenHeight/8, &AmbientOcclusionPingRT);
	gxCreateRenderTarget(gxScreenWidth/8, gxScreenHeight/8, &AmbientOcclusionPongRT);

	gxCreateRenderTarget(gxScreenWidth/8, gxScreenHeight/8, &AmbientOcclusionForegroundRT);
	gxCreateRenderTarget(gxScreenWidth/8, gxScreenHeight/8, &AmbientOcclusionVacuumRT);
}

void BuildAmbientOcclusion(ELightList List, gxSprite* FinalRT)
{
	gxSetRenderTarget(&AmbientOcclusionAlphaRT);
	gxClearColor(gxRGBA32(0, 0, 0, 0));

	gxSetPixelShader(&TexturedShadowShader);
	gxSetPixelShaderConst(0, 192.0f/255.0f);

	gxSetVertexShader(&ShadowVertexShader);
	gxSetVertexShaderConst(0, 0.0f, 0.0f);

	_gxSetAlpha( GXALPHA_BLEND );
	for (int i = 0; i < LightLists[List].NQuads; i++)
		DrawLitQuad( &LightLists[List].Quads[i] );

	gxCopyRenderTarget(&AmbientOcclusionAlphaRT, &AmbientOcclusionDiv2RT);
	gxCopyRenderTarget(&AmbientOcclusionDiv2RT, &AmbientOcclusionDiv4RT);
	gxCopyRenderTarget(&AmbientOcclusionDiv4RT, &AmbientOcclusionPingRT);

	gxSetPixelShader(&Gaussian13Shader);

	for (int i = 0; i < 4; i++)
	{
		gxSetPixelShaderConst(0, -6.0f/AmbientOcclusionPingRT.width, 0.0f, 1.0f/4096.0f);
		gxSetPixelShaderConst(1, -5.0f/AmbientOcclusionPingRT.width, 0.0f, 12.0f/4096.0f);
		gxSetPixelShaderConst(2, -4.0f/AmbientOcclusionPingRT.width, 0.0f, 66.0f/4096.0f);
		gxSetPixelShaderConst(3, -3.0f/AmbientOcclusionPingRT.width, 0.0f, 220.0f/4096.0f);
		gxSetPixelShaderConst(4, -2.0f/AmbientOcclusionPingRT.width, 0.0f, 495.0f/4096.0f);
		gxSetPixelShaderConst(5, -1.0f/AmbientOcclusionPingRT.width, 0.0f, 792.0f/4096.0f);
		gxSetPixelShaderConst(6,  0.0f/AmbientOcclusionPingRT.width, 0.0f, 924.0f/4096.0f);

		gxCopyRenderTarget(&AmbientOcclusionPingRT, &AmbientOcclusionPongRT);

		gxSetPixelShaderConst(0, 0.0f, -6.0f/AmbientOcclusionPingRT.height, 1.0f/4096.0f);
		gxSetPixelShaderConst(1, 0.0f, -5.0f/AmbientOcclusionPingRT.height, 12.0f/4096.0f);
		gxSetPixelShaderConst(2, 0.0f, -4.0f/AmbientOcclusionPingRT.height, 66.0f/4096.0f);
		gxSetPixelShaderConst(3, 0.0f, -3.0f/AmbientOcclusionPingRT.height, 220.0f/4096.0f);
		gxSetPixelShaderConst(4, 0.0f, -2.0f/AmbientOcclusionPingRT.height, 495.0f/4096.0f);
		gxSetPixelShaderConst(5, 0.0f, -1.0f/AmbientOcclusionPingRT.height, 792.0f/4096.0f);
		gxSetPixelShaderConst(6, 0.0f,  0.0f/AmbientOcclusionPingRT.height, 924.0f/4096.0f);

		gxCopyRenderTarget(&AmbientOcclusionPongRT, &AmbientOcclusionPingRT);
	}

	gxSetPixelShader(&TexturedColoredShader);
	gxCopyRenderTarget(&AmbientOcclusionPingRT, FinalRT);
}

void RenderAmbientOcclusion(gxSprite* FinalRT)
{
	gxSetPixelShader(&TexturedColoredShader);
	gxSetVertexShader(&LitVertexShader);

	_gxSetAlpha(GXALPHA_BLEND);
	_gxSetTexture(FinalRT);
	_gxDrawQuad(0, 0, (float)gxScreenWidth, (float)gxScreenHeight);
}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                      Shadows                                                                            //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void InitShadows()
{
	gxCreateRenderTarget(gxScreenWidth,   gxScreenHeight,   &ShadowAlphaRT);
	gxCreateRenderTarget(gxScreenWidth/2, gxScreenHeight/2, &ShadowPingRT);
	gxCreateRenderTarget(gxScreenWidth/2, gxScreenHeight/2, &ShadowPongRT);

	gxCreateRenderTarget(gxScreenWidth,   gxScreenHeight,   &ShadowForegroundRT);
	gxCreateRenderTarget(gxScreenWidth,   gxScreenHeight,   &ShadowVacuumRT);
}

void BuildShadows(ELightList List, gxSprite* FinalRT, float ShadowOffsetX, float ShadowOffsetY)
{
	gxSetRenderTarget(&ShadowAlphaRT);
	gxClearColor(gxRGBA32(0, 0, 0, 0));

	gxSetPixelShader(&TexturedShadowShader);
	gxSetPixelShaderConst(0, 192.0f/255.0f);

	gxSetVertexShader(&ShadowVertexShader);
	gxSetVertexShaderConst(0, ShadowOffsetX, ShadowOffsetY);

	_gxSetAlpha( GXALPHA_BLEND );
	for (int i = 0; i < LightLists[List].NQuads; i++)
		DrawLitQuad( &LightLists[List].Quads[i] );

	gxCopyRenderTarget(&ShadowAlphaRT, &ShadowPingRT);

	gxSetPixelShader(&Gaussian7Shader);

	for (int i = 0; i < 1; i++)
	{
		gxSetPixelShaderConst(0, -3.0f/ShadowPingRT.width, 0.0f, 1.0f/64.0f);
		gxSetPixelShaderConst(1, -2.0f/ShadowPingRT.width, 0.0f, 6.0f/64.0f);
		gxSetPixelShaderConst(2, -1.0f/ShadowPingRT.width, 0.0f, 15.0f/64.0f);
		gxSetPixelShaderConst(3,  0.0f/ShadowPingRT.width, 0.0f, 20.0f/64.0f);

		gxCopyRenderTarget(&ShadowPingRT, &ShadowPongRT);

		gxSetPixelShaderConst(0, 0.0f, -3.0f/ShadowPingRT.height, 1.0f/64.0f);
		gxSetPixelShaderConst(1, 0.0f, -2.0f/ShadowPingRT.height, 6.0f/64.0f);
		gxSetPixelShaderConst(2, 0.0f, -1.0f/ShadowPingRT.height, 15.0f/64.0f);
		gxSetPixelShaderConst(3, 0.0f,  0.0f/ShadowPingRT.height, 20.0f/64.0f);

		gxCopyRenderTarget(&ShadowPongRT, &ShadowPingRT);
	}

	gxSetPixelShader(&TexturedColoredShader);
	gxCopyRenderTarget(&ShadowPingRT, FinalRT);
}

void RenderShadows(gxSprite* FinalRT)
{
	gxSetPixelShader(&TexturedColoredShader);
	gxSetVertexShader(&LitVertexShader);

	_gxSetAlpha(GXALPHA_BLEND);
	_gxSetTexture(FinalRT);
	_gxDrawQuad(0, 0, (float)gxScreenWidth, (float)gxScreenHeight);
}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                      Ambient occlusion                                                                  //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void InitColorBleed()
{
	gxCreateRenderTarget(gxScreenWidth/2, gxScreenHeight/2, &ColorBleedDiv2RT);
	gxCreateRenderTarget(gxScreenWidth/4, gxScreenHeight/4, &ColorBleedDiv4RT);
	gxCreateRenderTarget(gxScreenWidth/8, gxScreenHeight/8, &ColorBleedDiv8RT);
	gxCreateRenderTarget(gxScreenWidth/16, gxScreenHeight/16, &ColorBleedPingRT);
	gxCreateRenderTarget(gxScreenWidth/16, gxScreenHeight/16, &ColorBleedPongRT);
	gxCreateRenderTarget(gxScreenWidth/16, gxScreenHeight/16, &ColorBleedFinalRT);
}

void BuildColorBleed()
{
	gxSetPixelShader(&Gaussian13Shader);

	// TODO: These samples are nowhere near correct but it looks tolerable so whatever.
	// Probably should make a Gaussian9 and just type in a 2D 131.
	gxSetPixelShaderConst(0, -2.0f/ColorBleedPingRT.width, -2.0f/ColorBleedPingRT.width, 1.0f/4096.0f);
	gxSetPixelShaderConst(1, -2.0f/ColorBleedPingRT.width, -1.0f/ColorBleedPingRT.width, 12.0f/4096.0f);
	gxSetPixelShaderConst(2, -2.0f/ColorBleedPingRT.width,  0.0f/ColorBleedPingRT.width, 66.0f/4096.0f);
	gxSetPixelShaderConst(3, -1.0f/ColorBleedPingRT.width, -2.0f/ColorBleedPingRT.width, 220.0f/4096.0f);
	gxSetPixelShaderConst(4, -1.0f/ColorBleedPingRT.width, -1.0f/ColorBleedPingRT.width, 495.0f/4096.0f);
	gxSetPixelShaderConst(5, -1.0f/ColorBleedPingRT.width,  0.0f/ColorBleedPingRT.width, 792.0f/4096.0f);
	gxSetPixelShaderConst(6,  0.0f/ColorBleedPingRT.width,  0.0f/ColorBleedPingRT.width, 924.0f/4096.0f);

	gxCopyRenderTarget(&ColorRT, &ColorBleedDiv2RT);
	gxCopyRenderTarget(&ColorBleedDiv2RT, &ColorBleedDiv4RT);
	gxCopyRenderTarget(&ColorBleedDiv4RT, &ColorBleedDiv8RT);
	gxCopyRenderTarget(&ColorBleedDiv8RT, &ColorBleedPingRT);

	for (int i = 0; i < 2; i++)
	{
		gxSetPixelShaderConst(0, -6.0f/ColorBleedPingRT.width, 0.0f, 1.0f/4096.0f);
		gxSetPixelShaderConst(1, -5.0f/ColorBleedPingRT.width, 0.0f, 12.0f/4096.0f);
		gxSetPixelShaderConst(2, -4.0f/ColorBleedPingRT.width, 0.0f, 66.0f/4096.0f);
		gxSetPixelShaderConst(3, -3.0f/ColorBleedPingRT.width, 0.0f, 220.0f/4096.0f);
		gxSetPixelShaderConst(4, -2.0f/ColorBleedPingRT.width, 0.0f, 495.0f/4096.0f);
		gxSetPixelShaderConst(5, -1.0f/ColorBleedPingRT.width, 0.0f, 792.0f/4096.0f);
		gxSetPixelShaderConst(6,  0.0f/ColorBleedPingRT.width, 0.0f, 924.0f/4096.0f);

		gxCopyRenderTarget(&ColorBleedPingRT, &ColorBleedPongRT);

		gxSetPixelShaderConst(0, 0.0f, -6.0f/ColorBleedPingRT.height, 1.0f/4096.0f);
		gxSetPixelShaderConst(1, 0.0f, -5.0f/ColorBleedPingRT.height, 12.0f/4096.0f);
		gxSetPixelShaderConst(2, 0.0f, -4.0f/ColorBleedPingRT.height, 66.0f/4096.0f);
		gxSetPixelShaderConst(3, 0.0f, -3.0f/ColorBleedPingRT.height, 220.0f/4096.0f);
		gxSetPixelShaderConst(4, 0.0f, -2.0f/ColorBleedPingRT.height, 495.0f/4096.0f);
		gxSetPixelShaderConst(5, 0.0f, -1.0f/ColorBleedPingRT.height, 792.0f/4096.0f);
		gxSetPixelShaderConst(6, 0.0f,  0.0f/ColorBleedPingRT.height, 924.0f/4096.0f);

		gxCopyRenderTarget(&ColorBleedPongRT, &ColorBleedPingRT);
	}

	gxSetPixelShader(&TexturedColoredShader);
	gxCopyRenderTarget(&ColorBleedPingRT, &ColorBleedFinalRT);
}

void RenderCombinedColor()
{
	gxSetPixelShader(&CombineShader);
	_gxSetAlpha(GXALPHA_NONE);
	gxDev->SetTexture( 0, ColorRT.tex );
	gxDev->SetTexture( 1, ColorBleedFinalRT.tex );
	gxDev->SetTexture( 2, LightingRT.tex );
	_gxDrawQuad(0, 0, (float)gxScreenWidth, (float)gxScreenHeight);
	gxSetPixelShader(&TexturedColoredShader);
}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                          Main Lighting Interface                                                                        //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void InitLighting()
{
	gxDev->CreateVertexDeclaration(LitVertexElements, &LitVertexDecl);

	// Compile shaders.
	gxCreateVertexShader(LitVertexShaderSource, &LitVertexShader);
	gxCreateVertexShader(ShadowVertexShaderSource, &ShadowVertexShader);

	gxCreatePixelShader(TexturedColoredShaderSource, &TexturedColoredShader);
	gxCreatePixelShader(TexturedShadowShaderSource, &TexturedShadowShader);

	gxCreatePixelShader(EffectsShaderSource, &EffectsShader);

	gxCreatePixelShader(Gaussian7ShaderSource, &Gaussian7Shader);
	gxCreatePixelShader(Gaussian13ShaderSource, &Gaussian13Shader);

	gxCreatePixelShader(CombineShaderSource, &CombineShader);

	// Create render targets.
	gxCreateRenderTarget(gxScreenWidth, gxScreenHeight, &ColorRT);
	gxCreateRenderTarget(gxScreenWidth, gxScreenHeight, &LightingRT);

	InitAmbientOcclusion();
	InitShadows();
	InitColorBleed();
}

void ResetLighting()
{
	NLitVerts = 0;

	for (int i = 0; i < LIGHTLIST_COUNT; i++)
	{
		LightLists[i].NQuads = 0;
	}
}

void RenderLighting()
{
	// Set up lighting.
	if (Chapter.PageProps.LightsOff)
		LightState.AmbientColor = gxRGBA32(16, 16, 16, 255);
	else
		LightState.AmbientColor = gxRGBA32(128, 128, 128, 255);

	// Build render targets.
	gxSetVertexShader(&LitVertexShader);

	// Build ambient occlusion buffers.
	BuildAmbientOcclusion(LIGHTLIST_FOREGROUND, &AmbientOcclusionForegroundRT);
	BuildAmbientOcclusion(LIGHTLIST_VACUUM, &AmbientOcclusionVacuumRT);

	// Build shadow buffers.
	BuildShadows(LIGHTLIST_FOREGROUND, &ShadowForegroundRT, 30, 20);
	BuildShadows(LIGHTLIST_VACUUM, &ShadowVacuumRT, 30, 20);

	// Build lighting buffer.
	gxSetPixelShader(&TexturedColoredShader);
	gxSetVertexShader(&LitVertexShader);

	gxSetRenderTarget(&LightingRT);
	gxClearColor(LightState.AmbientColor);

	_gxSetAlpha( GXALPHA_ADD );
	for (int i = 0; i < LightLists[LIGHTLIST_LIGHTING].NQuads; i++)
		DrawLitQuad( &LightLists[LIGHTLIST_LIGHTING].Quads[i] );

	// Main rendering.
	gxSetRenderTarget(&ColorRT);

	gxSetPixelShader(&TexturedColoredShader);
	gxSetVertexShader(&LitVertexShader);

#if 1
	// Real background.
	_gxSetAlpha( GXALPHA_BLEND );
	for (int i = 0; i < LightLists[LIGHTLIST_BACKGROUND].NQuads; i++)
		DrawLitQuad( &LightLists[LIGHTLIST_BACKGROUND].Quads[i] );
#else
	// White background - useful for testing AO and stuff.
	gxClearColor(gxRGBA32(255, 255, 255, 255));
	// Black background - useful for testing fireworks.
	//gxClearColor(gxRGBA32(0, 0, 0, 255));
#endif

	// Foreground ambient occlusion & shadows.
	RenderAmbientOcclusion(&AmbientOcclusionForegroundRT);
	RenderShadows(&ShadowForegroundRT);

	gxSetPixelShader(&TexturedColoredShader);
	gxSetVertexShader(&LitVertexShader);

	// Dust layer.
	_gxSetAlpha( GXALPHA_BLEND );
	for (int i = 0; i < LightLists[LIGHTLIST_DUST].NQuads; i++)
		DrawLitQuad( &LightLists[LIGHTLIST_DUST].Quads[i] );

	// Foreground.
	gxSetPixelShader(&TexturedColoredShader);
	gxSetVertexShader(&LitVertexShader);

	_gxSetAlpha( GXALPHA_BLEND );
	for (int i = 0; i < LightLists[LIGHTLIST_FOREGROUND].NQuads; i++)
		DrawLitQuad( &LightLists[LIGHTLIST_FOREGROUND].Quads[i] );
	for (int i = 0; i < LightLists[LIGHTLIST_FOREGROUND_NO_SHADOW].NQuads; i++)
		DrawLitQuad( &LightLists[LIGHTLIST_FOREGROUND_NO_SHADOW].Quads[i] );

	// Vacuum ambient occlusion & shadows.
	RenderAmbientOcclusion(&AmbientOcclusionVacuumRT);
	RenderShadows(&ShadowVacuumRT);

	gxSetPixelShader(&TexturedColoredShader);
	gxSetVertexShader(&LitVertexShader);

	// Vacuum.
	_gxSetAlpha( GXALPHA_BLEND );
	for (int i = 0; i < LightLists[LIGHTLIST_VACUUM].NQuads; i++)
		DrawLitQuad( &LightLists[LIGHTLIST_VACUUM].Quads[i] );

	// Color bleeding - Adds color and bleed color into framebuffer.
	BuildColorBleed();

	// Combine everything into the final output.
	gxSetRenderTarget(NULL);
	RenderCombinedColor();

	gxSetPixelShader(&TexturedColoredShader);
	gxSetVertexShader(&LitVertexShader);

	// Effects.
	gxSetPixelShader(&EffectsShader);
	gxSetVertexShader(&LitVertexShader);
	_gxSetAlpha( GXALPHA_ADD );
	for (int i = 0; i < LightLists[LIGHTLIST_EFFECTS].NQuads; i++)
		DrawLitQuad( &LightLists[LIGHTLIST_EFFECTS].Quads[i] );

	// Debugging of render targets.
	if (DevMode)
	{
		//_gxSetAlpha( GXALPHA_BLEND );
		//_gxSetTexture( &AmbientOcclusionFinalRT );
		//_gxDrawQuad( 0, 0, 768, 1024);
	}
}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                          Quad Addition Functions                                                                        //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

SLitVertex* AllocateLitVerts(int NVerts)
{
	if (NLitVerts + NVerts > MAX_LIT_VERTS)
		ReportError("Exceeded the maximum of %d lit vertices.", MAX_LIT_VERTS);

	SLitVertex* Verts = &LitVerts[NLitVerts];
	
	NLitVerts += NVerts;

	return Verts;
}

void AddLitQuad(
	ELightList List, gxSprite* Sprite, unsigned int Color,
	float X0, float Y0, float U0, float V0, 
	float X1, float Y1, float U1, float V1, 
	float X2, float Y2, float U2, float V2, 
	float X3, float Y3, float U3, float V3)
{
	if (LightLists[List].NQuads >= MAX_LIT_QUADS_PER_LIST)
		ReportError("Exceeded the maximum of %d lit quads per light list.", MAX_LIT_QUADS_PER_LIST);

	SLitQuad* Quad = &LightLists[List].Quads[LightLists[List].NQuads++];
	
	Quad->List = List;
	Quad->Sprite = Sprite;

	Quad->NVerts = 4;
	Quad->Verts = AllocateLitVerts(4);

	Quad->Verts[0].X = X0;
	Quad->Verts[0].Y = Y0;
	Quad->Verts[0].U = U0;
	Quad->Verts[0].V = V0;
	Quad->Verts[0].Color = Color;

	Quad->Verts[1].X = X1;
	Quad->Verts[1].Y = Y1;
	Quad->Verts[1].U = U1;
	Quad->Verts[1].V = V1;
	Quad->Verts[1].Color = Color;

	Quad->Verts[3].X = X2;
	Quad->Verts[3].Y = Y2;
	Quad->Verts[3].U = U2;
	Quad->Verts[3].V = V2;
	Quad->Verts[3].Color = Color;

	Quad->Verts[2].X = X3;
	Quad->Verts[2].Y = Y3;
	Quad->Verts[2].U = U3;
	Quad->Verts[2].V = V3;
	Quad->Verts[2].Color = Color;
}

SLitVertex* AddLitQuad(ELightList List, gxSprite* Sprite, int NVerts)
{
	if (LightLists[List].NQuads >= MAX_LIT_QUADS_PER_LIST)
		ReportError("Exceeded the maximum of %d lit quads per light list.", MAX_LIT_QUADS_PER_LIST);

	SLitQuad* Quad = &LightLists[List].Quads[LightLists[List].NQuads++];

	Quad->List = List;
	Quad->Sprite = Sprite;

	Quad->NVerts = NVerts;
	Quad->Verts = AllocateLitVerts(NVerts);

	return Quad->Verts;
}

void AddLitSprite(ELightList List, gxSprite* Sprite, float X, float Y)
{
	AddLitQuad(List, Sprite, gxRGBA32(255,255,255,255),
		X,               Y,                0.0f, 0.0f, 
		X+Sprite->width, Y,                1.0f, 0.0f,
		X+Sprite->width, Y+Sprite->height, 1.0f, 1.0f, 
		X,               Y+Sprite->height, 0.0f, 1.0f);
}

void AddLitSpriteScaled(ELightList List, gxSprite* Sprite, float X, float Y, float ScaleX, float ScaleY)
{
	AddLitQuad(List, Sprite, gxRGBA32(255,255,255,255),
		X,                      Y,                       0.0f, 0.0f, 
		X+Sprite->width*ScaleX, Y,                       1.0f, 0.0f,
		X+Sprite->width*ScaleX, Y+Sprite->height*ScaleY, 1.0f, 1.0f, 
		X,                      Y+Sprite->height*ScaleY, 0.0f, 1.0f);
}

void AddLitSpriteCenteredScaledAlpha(ELightList List, gxSprite* Sprite, float X, float Y, float Scale, float Alpha)
{
	float w = Sprite->width*Scale*0.5f;
	float h = Sprite->height*Scale*0.5f;

	AddLitQuad(List, Sprite, gxRGBA32(255,255,255,(int)(255*Alpha)),
		X-w, Y-h, 0.0f, 0.0f, 
		X+w, Y-h, 1.0f, 0.0f,
		X+w, Y+h, 1.0f, 1.0f, 
		X-w, Y+h, 0.0f, 1.0f);
}

void AddLitSpriteCenteredScaledColor(ELightList List, gxSprite* Sprite, float X, float Y, float Scale, unsigned int Color)
{
	float w = Sprite->width*Scale*0.5f;
	float h = Sprite->height*Scale*0.5f;

	AddLitQuad(List, Sprite, Color,
		X-w, Y-h, 0.0f, 0.0f, 
		X+w, Y-h, 1.0f, 0.0f,
		X+w, Y+h, 1.0f, 1.0f, 
		X-w, Y+h, 0.0f, 1.0f);
}

void AddLitSubSprite(ELightList List, gxSprite* Sprite, float X, float Y, float SubX1, float SubY1, float SubX2, float SubY2)
{
	AddLitQuad(List, Sprite, gxRGBA32(255,255,255,255),
		X,               Y,               SubX1/Sprite->texWidth, SubY1/Sprite->texHeight, 
		X+(SubX2-SubX1), Y,               SubX2/Sprite->texWidth, SubY1/Sprite->texHeight,
		X+(SubX2-SubX1), Y+(SubY2-SubY1), SubX2/Sprite->texWidth, SubY2/Sprite->texHeight, 
		X,               Y+(SubY2-SubY1), SubX1/Sprite->texWidth, SubY2/Sprite->texHeight);
}

void AddLitSpriteCenteredScaledRotated(ELightList List, gxSprite* Sprite, float X, float Y, float Scale, float Angle)
{
	float ca = cosf(Angle);
	float sa = sinf(Angle);

	float w = (float)Sprite->width * 0.5f;
	float h = (float)Sprite->height * 0.5f;

	AddLitQuad(List, Sprite, gxRGBA32(255,255,255,255),
		X + (-w * ca) - (-h * sa),    Y + (-w * sa) + (-h * ca),    0.0f, 0.0f, 
		X + (+w * ca) - (-h * sa),    Y + (+w * sa) + (-h * ca),    1.0f, 0.0f, 
		X + (+w * ca) - (+h * sa),    Y + (+w * sa) + (+h * ca),    1.0f, 1.0f, 
		X + (-w * ca) - (+h * sa),    Y + (-w * sa) + (+h * ca),    0.0f, 1.0f);
}
