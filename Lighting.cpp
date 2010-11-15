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


#define MAX_LIT_QUADS_PER_LIST 512


struct SLightState
{
	unsigned int AmbientColor;

	float ShadowOffsetX;
	float ShadowOffsetY;
	unsigned int ShadowAlpha;
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

gxShader TexturedColoredShader;

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

gxShader Gaussian7Shader;


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

gxShader Gaussian13Shader;


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

gxShader CombineShader;

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                      Platform specific drawing                                                          //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void DrawLitQuad(SLitQuad* Quad)
{
	_gxSetTexture(Quad->Sprite);

	gxSpriteVertex v[4];
	v[0].x = Quad->X0; 
	v[0].y = Quad->Y0; 
	v[0].z = 0.0f; 
	v[0].w = 1.0f;
	v[0].color = Quad->Color;
	v[0].u = Quad->U0; 
	v[0].v = Quad->V0;

	v[1].x = Quad->X1; 
	v[1].y = Quad->Y1; 
	v[1].z = 0.0f; 
	v[1].w = 1.0f;
	v[1].color = Quad->Color;
	v[1].u = Quad->U1; 
	v[1].v = Quad->V1;

	v[3].x = Quad->X2; 
	v[3].y = Quad->Y2; 
	v[3].z = 0.0f; 
	v[3].w = 1.0f;
	v[3].color = Quad->Color;
	v[3].u = Quad->U2; 
	v[3].v = Quad->V2;

	v[2].x = Quad->X3; 
	v[2].y = Quad->Y3; 
	v[2].z = 0.0f; 
	v[2].w = 1.0f;
	v[2].color = Quad->Color;
	v[2].u = Quad->U3; 
	v[2].v = Quad->V3;

	gxDev->SetFVF( gxSpriteVertexFVF );
	gxDev->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, v, sizeof(gxSpriteVertex) );
}

void DrawLitQuad_Shadow(SLitQuad* Quad)
{
	_gxSetTexture(Quad->Sprite);

	gxSpriteVertex v[4];
	v[0].x = Quad->X0 + LightState.ShadowOffsetX; 
	v[0].y = Quad->Y0 + LightState.ShadowOffsetY; 
	v[0].z = 0.0f; 
	v[0].w = 1.0f;
	v[0].color = gxRGBA32(0, 0, 0, LightState.ShadowAlpha);
	v[0].u = Quad->U0; 
	v[0].v = Quad->V0;

	v[1].x = Quad->X1 + LightState.ShadowOffsetX; 
	v[1].y = Quad->Y1 + LightState.ShadowOffsetY; 
	v[1].z = 0.0f; 
	v[1].w = 1.0f;
	v[1].color = gxRGBA32(0, 0, 0, LightState.ShadowAlpha);
	v[1].u = Quad->U1; 
	v[1].v = Quad->V1;

	v[3].x = Quad->X2 + LightState.ShadowOffsetX; 
	v[3].y = Quad->Y2 + LightState.ShadowOffsetY; 
	v[3].z = 0.0f; 
	v[3].w = 1.0f;
	v[2].color = gxRGBA32(0, 0, 0, LightState.ShadowAlpha);
	v[3].u = Quad->U2; 
	v[3].v = Quad->V2;

	v[2].x = Quad->X3 + LightState.ShadowOffsetX; 
	v[2].y = Quad->Y3 + LightState.ShadowOffsetY; 
	v[2].z = 0.0f; 
	v[2].w = 1.0f;
	v[3].color = gxRGBA32(0, 0, 0, LightState.ShadowAlpha);
	v[2].u = Quad->U3; 
	v[2].v = Quad->V3;

	gxDev->SetFVF( gxSpriteVertexFVF );
	gxDev->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, v, sizeof(gxSpriteVertex) );
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

	gxSetPixelShader(&TexturedColoredShader);

	LightState.ShadowAlpha = 192;
	LightState.ShadowOffsetX = 0;
	LightState.ShadowOffsetY = 0;

	_gxSetAlpha( GXALPHA_BLEND );
	for (int i = 0; i < LightLists[List].NQuads; i++)
		DrawLitQuad_Shadow( &LightLists[List].Quads[i] );

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

	gxSetPixelShader(&TexturedColoredShader);

	LightState.ShadowAlpha = 128;
	LightState.ShadowOffsetX = ShadowOffsetX;
	LightState.ShadowOffsetY = ShadowOffsetY;

	_gxSetAlpha( GXALPHA_BLEND );
	for (int i = 0; i < LightLists[List].NQuads; i++)
		DrawLitQuad_Shadow( &LightLists[List].Quads[i] );

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
	// Compile shaders.
	gxCreateShader(TexturedColoredShaderSource, &TexturedColoredShader);

	gxCreateShader(Gaussian7ShaderSource, &Gaussian7Shader);
	gxCreateShader(Gaussian13ShaderSource, &Gaussian13Shader);

	gxCreateShader(CombineShaderSource, &CombineShader);

	// Create render targets.
	gxCreateRenderTarget(gxScreenWidth, gxScreenHeight, &ColorRT);
	gxCreateRenderTarget(gxScreenWidth, gxScreenHeight, &LightingRT);

	InitAmbientOcclusion();
	InitShadows();
	InitColorBleed();
}

void ResetLighting()
{
	for (int i = 0; i < LIGHTLIST_COUNT; i++)
	{
		LightLists[i].NQuads = 0;
	}
}

void RenderLighting()
{
	// Set up lighting.
	if (Chapter.LightsOff)
		LightState.AmbientColor = gxRGBA32(16, 16, 16, 255);
	else
		LightState.AmbientColor = gxRGBA32(128, 128, 128, 255);


	// Build render targerts.

	// Build ambient occlusion buffers.
	BuildAmbientOcclusion(LIGHTLIST_FOREGROUND, &AmbientOcclusionForegroundRT);
	BuildAmbientOcclusion(LIGHTLIST_VACUUM, &AmbientOcclusionVacuumRT);

	// Build shadow buffers.
	BuildShadows(LIGHTLIST_FOREGROUND, &ShadowForegroundRT, 30, 20);
	BuildShadows(LIGHTLIST_VACUUM, &ShadowVacuumRT, 30, 20);

	// Build lighting buffer.
	gxSetRenderTarget(&LightingRT);
	gxClearColor(LightState.AmbientColor);

	_gxSetAlpha( GXALPHA_ADD );
	for (int i = 0; i < LightLists[LIGHTLIST_LIGHTING].NQuads; i++)
		DrawLitQuad( &LightLists[LIGHTLIST_LIGHTING].Quads[i] );

	// Main rendering.
	gxSetRenderTarget(&ColorRT);

#if 0
	// Real background.
	_gxSetAlpha( GXALPHA_BLEND );
	for (int i = 0; i < LightLists[LIGHTLIST_BACKGROUND].NQuads; i++)
		DrawLitQuad( &LightLists[LIGHTLIST_BACKGROUND].Quads[i] );
#else
	// White background - useful for testing AO and stuff.
	gxClearColor(gxRGBA32(255, 255, 255, 255));
#endif

	// Foreground ambient occlusion & shadows.
	RenderAmbientOcclusion(&AmbientOcclusionForegroundRT);
	RenderShadows(&ShadowForegroundRT);

	// Dust layer.
	_gxSetAlpha( GXALPHA_BLEND );
	for (int i = 0; i < LightLists[LIGHTLIST_DUST].NQuads; i++)
		DrawLitQuad( &LightLists[LIGHTLIST_DUST].Quads[i] );

	// Foreground.
	_gxSetAlpha( GXALPHA_BLEND );
	for (int i = 0; i < LightLists[LIGHTLIST_FOREGROUND].NQuads; i++)
		DrawLitQuad( &LightLists[LIGHTLIST_FOREGROUND].Quads[i] );
	for (int i = 0; i < LightLists[LIGHTLIST_FOREGROUND_NO_SHADOW].NQuads; i++)
		DrawLitQuad( &LightLists[LIGHTLIST_FOREGROUND_NO_SHADOW].Quads[i] );

	// Effects.
	_gxSetAlpha( GXALPHA_ADD );
	for (int i = 0; i < LightLists[LIGHTLIST_EFFECTS].NQuads; i++)
		DrawLitQuad( &LightLists[LIGHTLIST_EFFECTS].Quads[i] );

	// Vacuum ambient occlusion & shadows.
	RenderAmbientOcclusion(&AmbientOcclusionVacuumRT);
	RenderShadows(&ShadowVacuumRT);

	// Vacuum.
	_gxSetAlpha( GXALPHA_BLEND );
	for (int i = 0; i < LightLists[LIGHTLIST_VACUUM].NQuads; i++)
		DrawLitQuad( &LightLists[LIGHTLIST_VACUUM].Quads[i] );

	// Color bleeding - Adds color and bleed color into framebuffer.
	BuildColorBleed();

	// Combine everything into the final output.
	gxSetRenderTarget(NULL);
	RenderCombinedColor();

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

	Quad->X0 = X0;
	Quad->Y0 = Y0;
	Quad->X1 = X1;
	Quad->Y1 = Y1;
	Quad->X2 = X2;
	Quad->Y2 = Y2;
	Quad->X3 = X3;
	Quad->Y3 = Y3;

	Quad->U0 = U0;
	Quad->V0 = V0;
	Quad->U1 = U1;
	Quad->V1 = V1;
	Quad->U2 = U2;
	Quad->V2 = V2;
	Quad->U3 = U3;
	Quad->V3 = V3;

	Quad->Color = Color;
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
