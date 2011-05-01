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

#ifdef PLATFORM_IPHONE

int NLitVertIndices;

int BufferObjectIndex;

GLuint LitVertsVBO[2];
GLuint LitVertsIBO[2];

SLitVertex* LitVerts;
GLushort* LitVertIndices;

#else

SLitVertex LitVerts[MAX_LIT_VERTS];

#endif


float LitAspectOffset;
float LitAspectScale;

float LitScreenHeight;


// TODO: Move to SLighting.
float LitSceneZoom = 1.0f;
float LitSceneOffsetX = 0.0f;
float LitSceneOffsetY = 0.0f;


int LitRenderTargetWidth;
int LitRenderTargetHeight;


#ifdef PLATFORM_WINDOWS

D3DVERTEXELEMENT9 LitVertexElements[] =
{
	{ 0, offsetof(SLitVertex, X), D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
	{ 0, offsetof(SLitVertex, U), D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
	{ 0, offsetof(SLitVertex, Color), D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
	D3DDECL_END(),
};

IDirect3DVertexDeclaration9* LitVertexDecl;

#endif


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


gxSprite AmbientOcclusionDiv2RT;
gxSprite AmbientOcclusionDiv4RT;
gxSprite AmbientOcclusionPingRT;
gxSprite AmbientOcclusionPongRT;

gxSprite AmbientOcclusionForegroundRT;
gxSprite AmbientOcclusionVacuumRT;


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

#ifdef PLATFORM_WINDOWS

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
"	SVertexOutput VertexOutput;\n"
"	float2 Position = VertexInput.Position + ShadowOffset;\n"
"	VertexOutput.Position = float4(Position.x/768.0*2-1, (1-Position.y/1024)*2-1, 0, 1);\n"
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
"   return (Lighting*2.0) * Color/* * saturate(ColorBleed*1.5)*/;\n"
"}\n";

gxPixelShader CombineShader;

#endif


#ifdef PLATFORM_IPHONE

const char* LitVertexShaderSource =
"uniform vec2 Offset;\n"
"\n"
"attribute vec2 PositionAttr;\n"
"attribute vec2 TexCoordAttr;\n"
"attribute vec4 ColorAttr;\n"
"\n"
"varying vec2 TexCoordInterp;\n"
"varying vec4 ColorInterp;\n"
"\n"
"void main()\n"
"{\n"
"	gl_Position = vec4((PositionAttr.x+Offset.x)/768.0*2.0-1.0, (1.0-(PositionAttr.y+Offset.y)/1024.0)*2.0-1.0, 0, 1);\n"
"	TexCoordInterp = TexCoordAttr;\n"
"	ColorInterp = ColorAttr;\n"
"}\n";

const char* TexturedColoredShaderSource =
"uniform lowp sampler2D Sampler;\n"
"\n"
"varying lowp vec2 TexCoordInterp;\n"
"varying lowp vec4 ColorInterp;\n"
"\n"
"void main()\n"
"{\n"
"	gl_FragColor = texture2D(Sampler, TexCoordInterp) * ColorInterp;\n"
"}\n";

gxShader LitShader;


const char* ShadowShaderSource =
"uniform lowp float ShadowAlpha;\n"
"uniform lowp sampler2D Sampler;\n"
"\n"
"varying lowp vec2 TexCoordInterp;\n"
"\n"
"void main()\n"
"{\n"
"	gl_FragColor = vec4(0.0, 0.0, 0.0, ShadowAlpha * texture2D(Sampler, TexCoordInterp).a);\n"
"}\n";

gxShaderConstant ShadowShadowOffset;
gxShaderConstant ShadowShadowAlpha;
gxShaderConstant ShadowSampler;

gxShader ShadowShader;


const char* ApplyShadowShaderSource =
"uniform lowp sampler2D Sampler;\n"
"\n"
"varying lowp vec2 TexCoordInterp;\n"
"\n"
"void main()\n"
"{\n"
"	gl_FragColor = vec4(0.0, 0.0, 0.0, texture2D(Sampler, TexCoordInterp).a);\n"
"}\n";

gxShaderConstant ApplyShadowSampler;

gxShader ApplyShadowShader;


const char* EffectsShaderSource =
"uniform lowp sampler2D Sampler;\n"
"\n"
"varying lowp vec2 TexCoordInterp;\n"
"varying lowp vec4 ColorInterp;\n"
"\n"
"void main()\n"
"{\n"
"	gl_FragColor = texture2D(Sampler, TexCoordInterp) * ColorInterp * 2.0;\n"
"}\n";

gxShaderConstant EffectsSampler;

gxShader EffectsShader;


const char* Gaussian7ShaderSource =
"uniform lowp vec3 BlurOffsetScale0;\n"
"uniform lowp vec3 BlurOffsetScale1;\n"
"uniform lowp vec3 BlurOffsetScale2;\n"
"uniform lowp vec3 BlurOffsetScale3;\n"
"\n"
"uniform lowp sampler2D BlurSampler;\n"
"\n"
"varying lowp vec2 TexCoordInterp;\n"
"\n"
"void main()\n"
"{\n"
"	lowp vec4 color = vec4(0.0,0.0,0.0,0.0);\n"
"	color += texture2D(BlurSampler, TexCoordInterp+BlurOffsetScale0.xy) * BlurOffsetScale0.z;\n"
"	color += texture2D(BlurSampler, TexCoordInterp+BlurOffsetScale1.xy) * BlurOffsetScale1.z;\n"
"	color += texture2D(BlurSampler, TexCoordInterp+BlurOffsetScale2.xy) * BlurOffsetScale2.z;\n"
"	color += texture2D(BlurSampler, TexCoordInterp                    ) * BlurOffsetScale3.z;\n"
"	color += texture2D(BlurSampler, TexCoordInterp-BlurOffsetScale2.xy) * BlurOffsetScale2.z;\n"
"	color += texture2D(BlurSampler, TexCoordInterp-BlurOffsetScale1.xy) * BlurOffsetScale1.z;\n"
"	color += texture2D(BlurSampler, TexCoordInterp-BlurOffsetScale0.xy) * BlurOffsetScale0.z;\n"
"   gl_FragColor = color;\n"
"}\n";

gxShaderConstant Gaussian7BlurOffsetScale0;
gxShaderConstant Gaussian7BlurOffsetScale1;
gxShaderConstant Gaussian7BlurOffsetScale2;
gxShaderConstant Gaussian7BlurOffsetScale3;

gxShaderConstant Gaussian7BlurSampler;

gxShader Gaussian7Shader;


const char* Gaussian13ShaderSource =
"uniform lowp vec3 BlurOffsetScale0;\n"
"uniform lowp vec3 BlurOffsetScale1;\n"
"uniform lowp vec3 BlurOffsetScale2;\n"
"uniform lowp vec3 BlurOffsetScale3;\n"
"uniform lowp vec3 BlurOffsetScale4;\n"
"uniform lowp vec3 BlurOffsetScale5;\n"
"uniform lowp vec3 BlurOffsetScale6;\n"
"\n"
"uniform lowp sampler2D BlurSampler;\n"
"\n"
"varying lowp vec2 TexCoordInterp;\n"
"\n"
"void main()\n"
"{\n"
"	lowp vec4 color = vec4(0.0,0.0,0.0,0.0);\n"
"	color += texture2D(BlurSampler, TexCoordInterp+BlurOffsetScale0.xy) * BlurOffsetScale0.z;\n"
"	color += texture2D(BlurSampler, TexCoordInterp+BlurOffsetScale1.xy) * BlurOffsetScale1.z;\n"
"	color += texture2D(BlurSampler, TexCoordInterp+BlurOffsetScale2.xy) * BlurOffsetScale2.z;\n"
"	color += texture2D(BlurSampler, TexCoordInterp+BlurOffsetScale3.xy) * BlurOffsetScale3.z;\n"
"	color += texture2D(BlurSampler, TexCoordInterp+BlurOffsetScale4.xy) * BlurOffsetScale4.z;\n"
"	color += texture2D(BlurSampler, TexCoordInterp+BlurOffsetScale5.xy) * BlurOffsetScale5.z;\n"
"	color += texture2D(BlurSampler, TexCoordInterp                    ) * BlurOffsetScale6.z;\n"
"	color += texture2D(BlurSampler, TexCoordInterp-BlurOffsetScale5.xy) * BlurOffsetScale5.z;\n"
"	color += texture2D(BlurSampler, TexCoordInterp-BlurOffsetScale4.xy) * BlurOffsetScale4.z;\n"
"	color += texture2D(BlurSampler, TexCoordInterp-BlurOffsetScale3.xy) * BlurOffsetScale3.z;\n"
"	color += texture2D(BlurSampler, TexCoordInterp-BlurOffsetScale2.xy) * BlurOffsetScale2.z;\n"
"	color += texture2D(BlurSampler, TexCoordInterp-BlurOffsetScale1.xy) * BlurOffsetScale1.z;\n"
"	color += texture2D(BlurSampler, TexCoordInterp-BlurOffsetScale0.xy) * BlurOffsetScale0.z;\n"
"   gl_FragColor = color;\n"
"}\n";

gxShaderConstant Gaussian13BlurOffsetScale0;
gxShaderConstant Gaussian13BlurOffsetScale1;
gxShaderConstant Gaussian13BlurOffsetScale2;
gxShaderConstant Gaussian13BlurOffsetScale3;
gxShaderConstant Gaussian13BlurOffsetScale4;
gxShaderConstant Gaussian13BlurOffsetScale5;
gxShaderConstant Gaussian13BlurOffsetScale6;

gxShaderConstant Gaussian13BlurSampler;

gxShader Gaussian13Shader;


const char* CombineShaderSource =
"uniform lowp sampler2D ColorSampler;\n"
"uniform lowp sampler2D ColorBleedSampler;\n"
"uniform lowp sampler2D LightingSampler;\n"
"\n"
"varying lowp vec2 TexCoordInterp;\n"
"\n"
"void main()\n"
"{\n"
"	lowp vec4 Color = texture2D(ColorSampler, TexCoordInterp);\n"
"	lowp vec4 ColorBleed = texture2D(ColorBleedSampler, TexCoordInterp);\n"
"	lowp vec4 Lighting = texture2D(LightingSampler, TexCoordInterp);\n"
"   gl_FragColor = (Lighting*2.0) * Color/* * saturate(ColorBleed*1.5)*/;\n"
"}\n";

gxShaderConstant CombineColorSampler;
gxShaderConstant CombineColorBleedSampler;
gxShaderConstant CombineLightingSampler;

gxShader CombineShader;

#endif


// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                      Platform specific drawing                                                          //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void DrawLightList(int List, gxAlphaMode Alpha)
{
    _gxSetAlpha(Alpha);
    
#ifdef PLATFORM_WINDOWS
    for (int i = 0; i < LightLists[List].NQuads; i++)
    {
        SLitQuad* Quad = &LightLists[List].Quads[i];
        
        _gxSetTexture(Quad->Sprite);
        
        gxDev->SetVertexDeclaration( LitVertexDecl );
        gxDev->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, Quad->NVerts-2, Quad->Verts, sizeof(SLitVertex) );
    }
#endif
    
#ifdef PLATFORM_IPHONE
    glBindBuffer(GL_ARRAY_BUFFER, LitVertsVBO[BufferObjectIndex]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, LitVertsIBO[BufferObjectIndex]);
    
    if (gxOpenGLESVersion == 2)
    {
        glEnableVertexAttribArray(GX_ATTRIB_VERTEX);
        glEnableVertexAttribArray(GX_ATTRIB_TEXCOORD);
        glEnableVertexAttribArray(GX_ATTRIB_COLOR);
        
        glVertexAttribPointer(GX_ATTRIB_VERTEX, 2, GL_FLOAT, 0, sizeof(SLitVertex), (void*)offsetof(SLitVertex, X));
        glVertexAttribPointer(GX_ATTRIB_TEXCOORD, 2, GL_FLOAT, 0, sizeof(SLitVertex), (void*)offsetof(SLitVertex, U));
        glVertexAttribPointer(GX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, 1, sizeof(SLitVertex), (void*)offsetof(SLitVertex, Color));
    }
    else
    {
        glVertexPointer(3, GL_FLOAT, sizeof(SLitVertex), (void*)offsetof(SLitVertex, X));
        glTexCoordPointer(2, GL_FLOAT, sizeof(SLitVertex), (void*)offsetof(SLitVertex, U));
        glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(SLitVertex), (void*)offsetof(SLitVertex, Color));
    }
    
    int NQuads = LightLists[List].NQuads;
    int QuadIndex = 0;
    
    while (QuadIndex < NQuads)
    {
        SLitQuad* BaseQuad = &LightLists[List].Quads[QuadIndex];
        int BaseIndex = BaseQuad->BaseIndex;
        int IndexCount = 0;

        _gxSetTexture(BaseQuad->Sprite);

        SLitQuad* Quad = BaseQuad;
        for (;;)
        {
            if (QuadIndex >= NQuads)
                break;
            
            if (Quad->Sprite != BaseQuad->Sprite)
                break;
            
            if (Quad->BaseIndex != BaseIndex + IndexCount)
                break;
            
            IndexCount += Quad->IndexCount;
            Quad++;
            QuadIndex++;
        }
        
        glDrawElements(GL_TRIANGLE_STRIP, IndexCount, GL_UNSIGNED_SHORT, (void*)(BaseIndex * sizeof(GLushort)));
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
#endif
}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                      Ambient occlusion                                                                  //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void InitAmbientOcclusion()
{
	gxCreateRenderTarget(LitRenderTargetWidth/2, LitRenderTargetHeight/2, &AmbientOcclusionDiv2RT, true);
	gxCreateRenderTarget(LitRenderTargetWidth/4, LitRenderTargetHeight/4, &AmbientOcclusionDiv4RT, true);
	gxCreateRenderTarget(LitRenderTargetWidth/8, LitRenderTargetHeight/8, &AmbientOcclusionPingRT, true);
	gxCreateRenderTarget(LitRenderTargetWidth/8, LitRenderTargetHeight/8, &AmbientOcclusionPongRT, true);

	gxCreateRenderTarget(LitRenderTargetWidth/8, LitRenderTargetHeight/8, &AmbientOcclusionForegroundRT, true);
	gxCreateRenderTarget(LitRenderTargetWidth/8, LitRenderTargetHeight/8, &AmbientOcclusionVacuumRT, true);
}

void DrawAmbientOcclusion(ELightList List, gxSprite* FinalRT)
{
	gxSetRenderTarget(FinalRT);
	gxClearColor(gxRGBA32(0, 0, 0, 0));
    
#ifdef PLATFORM_WINDOWS
	gxSetPixelShader(&TexturedShadowShader);
	gxSetPixelShaderConst(0, 200.0f/255.0f);

	gxSetVertexShader(&ShadowVertexShader);
	gxSetVertexShaderConst(0, 0.0f, 0.0f);
#endif
    
#ifdef PLATFORM_IPHONE
    gxSetShader(&ShadowShader);
    gxSetShaderConstant(ShadowShadowOffset, 0.0f, 0.0f, 0.0f, 0.0f);
    gxSetShaderConstant(ShadowShadowAlpha, 192.0f/255.0f);
#endif
    
    DrawLightList(List, GXALPHA_BLEND);
}

void BlurAmbientOcclusion(gxSprite* FinalRT)
{
	gxCopyRenderTarget(FinalRT, &AmbientOcclusionDiv2RT);
	gxCopyRenderTarget(&AmbientOcclusionDiv2RT, &AmbientOcclusionDiv4RT);
	gxCopyRenderTarget(&AmbientOcclusionDiv4RT, &AmbientOcclusionPingRT);

#ifdef PLATFORM_WINDOWS
	gxSetPixelShader(&Gaussian13Shader);
#endif
    
#ifdef PLATFORM_IPHONE
    gxSetShader(&Gaussian13Shader);
#endif
    
	for (int i = 0; i < 4; i++)
	{
#ifdef PLATFORM_WINDOWS
		gxSetPixelShaderConst(0, -6.0f/AmbientOcclusionPingRT.width, 0.0f, 1.0f/4096.0f);
		gxSetPixelShaderConst(1, -5.0f/AmbientOcclusionPingRT.width, 0.0f, 12.0f/4096.0f);
		gxSetPixelShaderConst(2, -4.0f/AmbientOcclusionPingRT.width, 0.0f, 66.0f/4096.0f);
		gxSetPixelShaderConst(3, -3.0f/AmbientOcclusionPingRT.width, 0.0f, 220.0f/4096.0f);
		gxSetPixelShaderConst(4, -2.0f/AmbientOcclusionPingRT.width, 0.0f, 495.0f/4096.0f);
		gxSetPixelShaderConst(5, -1.0f/AmbientOcclusionPingRT.width, 0.0f, 792.0f/4096.0f);
		gxSetPixelShaderConst(6,  0.0f/AmbientOcclusionPingRT.width, 0.0f, 924.0f/4096.0f);
#endif
        
#ifdef PLATFORM_IPHONE
		gxSetShaderConstant(Gaussian13BlurOffsetScale0, -6.0f/AmbientOcclusionPingRT.width, 0.0f, 1.0f/4096.0f);
		gxSetShaderConstant(Gaussian13BlurOffsetScale1, -5.0f/AmbientOcclusionPingRT.width, 0.0f, 12.0f/4096.0f);
		gxSetShaderConstant(Gaussian13BlurOffsetScale2, -4.0f/AmbientOcclusionPingRT.width, 0.0f, 66.0f/4096.0f);
		gxSetShaderConstant(Gaussian13BlurOffsetScale3, -3.0f/AmbientOcclusionPingRT.width, 0.0f, 220.0f/4096.0f);
		gxSetShaderConstant(Gaussian13BlurOffsetScale4, -2.0f/AmbientOcclusionPingRT.width, 0.0f, 495.0f/4096.0f);
		gxSetShaderConstant(Gaussian13BlurOffsetScale5, -1.0f/AmbientOcclusionPingRT.width, 0.0f, 792.0f/4096.0f);
		gxSetShaderConstant(Gaussian13BlurOffsetScale6,  0.0f/AmbientOcclusionPingRT.width, 0.0f, 924.0f/4096.0f);
#endif
        
		gxCopyRenderTarget(&AmbientOcclusionPingRT, &AmbientOcclusionPongRT);

#ifdef PLATFORM_WINDOWS
		gxSetPixelShaderConst(0, 0.0f, -6.0f/AmbientOcclusionPingRT.height, 1.0f/4096.0f);
		gxSetPixelShaderConst(1, 0.0f, -5.0f/AmbientOcclusionPingRT.height, 12.0f/4096.0f);
		gxSetPixelShaderConst(2, 0.0f, -4.0f/AmbientOcclusionPingRT.height, 66.0f/4096.0f);
		gxSetPixelShaderConst(3, 0.0f, -3.0f/AmbientOcclusionPingRT.height, 220.0f/4096.0f);
		gxSetPixelShaderConst(4, 0.0f, -2.0f/AmbientOcclusionPingRT.height, 495.0f/4096.0f);
		gxSetPixelShaderConst(5, 0.0f, -1.0f/AmbientOcclusionPingRT.height, 792.0f/4096.0f);
		gxSetPixelShaderConst(6, 0.0f,  0.0f/AmbientOcclusionPingRT.height, 924.0f/4096.0f);
#endif

#ifdef PLATFORM_IPHONE
		gxSetShaderConstant(Gaussian13BlurOffsetScale0, 0.0f, -6.0f/AmbientOcclusionPingRT.height, 1.0f/4096.0f);
		gxSetShaderConstant(Gaussian13BlurOffsetScale1, 0.0f, -5.0f/AmbientOcclusionPingRT.height, 12.0f/4096.0f);
		gxSetShaderConstant(Gaussian13BlurOffsetScale2, 0.0f, -4.0f/AmbientOcclusionPingRT.height, 66.0f/4096.0f);
		gxSetShaderConstant(Gaussian13BlurOffsetScale3, 0.0f, -3.0f/AmbientOcclusionPingRT.height, 220.0f/4096.0f);
		gxSetShaderConstant(Gaussian13BlurOffsetScale4, 0.0f, -2.0f/AmbientOcclusionPingRT.height, 495.0f/4096.0f);
		gxSetShaderConstant(Gaussian13BlurOffsetScale5, 0.0f, -1.0f/AmbientOcclusionPingRT.height, 792.0f/4096.0f);
		gxSetShaderConstant(Gaussian13BlurOffsetScale6, 0.0f,  0.0f/AmbientOcclusionPingRT.height, 924.0f/4096.0f);
#endif
        
		gxCopyRenderTarget(&AmbientOcclusionPongRT, &AmbientOcclusionPingRT);
	}

#ifdef PLATFORM_WINDOWS
	gxSetPixelShader(&TexturedColoredShader);
#endif
    
#ifdef PLATFORM_IPHONE
    gxSetShader(&LitShader);
#endif
    
	gxCopyRenderTarget(&AmbientOcclusionPingRT, FinalRT);
}

void RenderAmbientOcclusion(gxSprite* FinalRT)
{
#ifdef PLATFORM_WINDOWS
	gxSetPixelShader(&TexturedColoredShader);
	gxSetVertexShader(&LitVertexShader);
#endif

#ifdef PLATFORM_IPHONE
    gxSetShader(&ApplyShadowShader);
#endif
    
	_gxSetAlpha(GXALPHA_BLEND);
	_gxSetTexture(FinalRT);

#ifdef PLATFORM_IPHONE
	_gxDrawQuad(0, 0, (float)gxScreenWidth, (float)gxScreenHeight, gxRGBA32(255,255,255,255), 0, 1, 1, 0);
#endif
#ifdef PLATFORM_WINDOWS
	_gxDrawQuad(0, 0, (float)gxScreenWidth, (float)gxScreenHeight, gxRGBA32(255,255,255,255), 0, 0, 1, 1);
#endif
}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                      Shadows                                                                            //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void InitShadows()
{
	gxCreateRenderTarget(LitRenderTargetWidth/4, LitRenderTargetHeight/4, &ShadowPingRT, true);
	gxCreateRenderTarget(LitRenderTargetWidth/4, LitRenderTargetHeight/4, &ShadowPongRT, true);

#ifdef PLATFORM_WINDOWS
	gxCreateRenderTarget(LitRenderTargetWidth, LitRenderTargetHeight, &ShadowForegroundRT, true);
	gxCreateRenderTarget(LitRenderTargetWidth, LitRenderTargetHeight, &ShadowVacuumRT, true);
#endif

#ifdef PLATFORM_IPHONE
	gxCreateRenderTarget(LitRenderTargetWidth/2, LitRenderTargetHeight/2, &ShadowForegroundRT, true);
	gxCreateRenderTarget(LitRenderTargetWidth/2, LitRenderTargetHeight/2, &ShadowVacuumRT, true);
#endif
}

void DrawShadows(ELightList List, gxSprite* FinalRT, float ShadowOffsetX, float ShadowOffsetY)
{
	gxSetRenderTarget(FinalRT);
	gxClearColor(gxRGBA32(0, 0, 0, 0));

#ifdef PLATFORM_WINDOWS
	gxSetPixelShader(&TexturedShadowShader);
	gxSetPixelShaderConst(0, 192.0f/255.0f);

	gxSetVertexShader(&ShadowVertexShader);
	gxSetVertexShaderConst(0, ShadowOffsetX, ShadowOffsetY);
#endif

#ifdef PLATFORM_IPHONE
    if (gxOpenGLESVersion == 2)
    {
        gxSetShader(&ShadowShader);
        gxSetShaderConstant(ShadowShadowOffset, ShadowOffsetX, ShadowOffsetY);
        gxSetShaderConstant(ShadowShadowAlpha, 192.0f/255.0f);
    }
    else
    {
        // ES1 TODO
    }
#endif
    
    DrawLightList(List, GXALPHA_BLEND);
}

void BlurShadows(gxSprite* FinalRT)
{
	gxCopyRenderTarget(FinalRT, &ShadowPingRT);

#ifdef PLATFORM_WINDOWS
	gxSetPixelShader(&Gaussian7Shader);
#endif
    
#ifdef PLATFORM_IPHONE
    gxSetShader(&Gaussian7Shader);
#endif
    
	for (int i = 0; i < 1; i++)
	{
#ifdef PLATFORM_WINDOWS
		gxSetPixelShaderConst(0, -3.0f/ShadowPingRT.width, 0.0f, 1.0f/64.0f);
		gxSetPixelShaderConst(1, -2.0f/ShadowPingRT.width, 0.0f, 6.0f/64.0f);
		gxSetPixelShaderConst(2, -1.0f/ShadowPingRT.width, 0.0f, 15.0f/64.0f);
		gxSetPixelShaderConst(3,  0.0f/ShadowPingRT.width, 0.0f, 20.0f/64.0f);
#endif
        
#ifdef PLATFORM_IPHONE
		gxSetShaderConstant(Gaussian7BlurOffsetScale0, -3.0f/ShadowPingRT.width, 0.0f, 1.0f/64.0f);
		gxSetShaderConstant(Gaussian7BlurOffsetScale1, -2.0f/ShadowPingRT.width, 0.0f, 6.0f/64.0f);
		gxSetShaderConstant(Gaussian7BlurOffsetScale2, -1.0f/ShadowPingRT.width, 0.0f, 15.0f/64.0f);
		gxSetShaderConstant(Gaussian7BlurOffsetScale3,  0.0f/ShadowPingRT.width, 0.0f, 20.0f/64.0f);
#endif
        
		gxCopyRenderTarget(&ShadowPingRT, &ShadowPongRT);

#ifdef PLATFORM_WINDOWS
		gxSetPixelShaderConst(0, 0.0f, -3.0f/ShadowPingRT.height, 1.0f/64.0f);
		gxSetPixelShaderConst(1, 0.0f, -2.0f/ShadowPingRT.height, 6.0f/64.0f);
		gxSetPixelShaderConst(2, 0.0f, -1.0f/ShadowPingRT.height, 15.0f/64.0f);
		gxSetPixelShaderConst(3, 0.0f,  0.0f/ShadowPingRT.height, 20.0f/64.0f);
#endif
        
#ifdef PLATFORM_IPHONE
		gxSetShaderConstant(Gaussian7BlurOffsetScale0, 0.0f, -3.0f/ShadowPingRT.height, 1.0f/64.0f);
		gxSetShaderConstant(Gaussian7BlurOffsetScale1, 0.0f, -2.0f/ShadowPingRT.height, 6.0f/64.0f);
		gxSetShaderConstant(Gaussian7BlurOffsetScale2, 0.0f, -1.0f/ShadowPingRT.height, 15.0f/64.0f);
		gxSetShaderConstant(Gaussian7BlurOffsetScale3, 0.0f,  0.0f/ShadowPingRT.height, 20.0f/64.0f);
#endif
        
		gxCopyRenderTarget(&ShadowPongRT, &ShadowPingRT);
	}

#ifdef PLATFORM_WINDOWS
	gxSetPixelShader(&TexturedColoredShader);
#endif
	
#ifdef PLATFORM_IPHONE
    gxSetShader(&LitShader);
#endif
    
    gxCopyRenderTarget(&ShadowPingRT, FinalRT);
}

void RenderShadows(gxSprite* FinalRT)
{
#ifdef PLATFORM_WINDOWS
	gxSetPixelShader(&TexturedColoredShader);
	gxSetVertexShader(&LitVertexShader);
#endif

#ifdef PLATFORM_IPHONE
    if (gxOpenGLESVersion == 2)
    {
        gxSetShader(&ApplyShadowShader);
    }
    else
    {
        // ES1 TODO
    }
#endif
    
	_gxSetAlpha(GXALPHA_BLEND);
	_gxSetTexture(FinalRT);

#ifdef PLATFORM_IPHONE
	_gxDrawQuad(0, 0, (float)gxScreenWidth, (float)gxScreenHeight, gxRGBA32(255,255,255,255), 0, 1, 1, 0);
#endif
#ifdef PLATFORM_WINDOWS
	_gxDrawQuad(0, 0, (float)gxScreenWidth/2, (float)gxScreenHeight/2, gxRGBA32(255,255,255,255), 0, 0, 1, 1);
#endif
}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                      Ambient occlusion                                                                  //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void InitColorBleed()
{
	gxCreateRenderTarget(LitRenderTargetWidth/2,    LitRenderTargetHeight/2,    &ColorBleedDiv2RT, false);
	gxCreateRenderTarget(LitRenderTargetWidth/4,    LitRenderTargetHeight/4,    &ColorBleedDiv4RT, false);
	gxCreateRenderTarget(LitRenderTargetWidth/8,    LitRenderTargetHeight/8,    &ColorBleedDiv8RT, false);
	gxCreateRenderTarget(LitRenderTargetWidth/16,   LitRenderTargetHeight/16,   &ColorBleedPingRT, false);
	gxCreateRenderTarget(LitRenderTargetWidth/16,   LitRenderTargetHeight/16,   &ColorBleedPongRT, false);
	gxCreateRenderTarget(LitRenderTargetWidth/16,   LitRenderTargetHeight/16,   &ColorBleedFinalRT, false);
}

void BuildColorBleed()
{
#ifdef PLATFORM_WINDOWS
	gxSetPixelShader(&Gaussian13Shader);
#endif
    
#ifdef PLATFORM_IPHONE
    gxSetShader(&Gaussian13Shader);
#endif

	// TODO: These samples are nowhere near correct but it looks tolerable so whatever.
	// Probably should make a Gaussian9 and just type in a 2D 131.
#ifdef PLATFORM_WINDOWS
	gxSetPixelShaderConst(0, -2.0f/ColorBleedPingRT.width, -2.0f/ColorBleedPingRT.width, 1.0f/4096.0f);
	gxSetPixelShaderConst(1, -2.0f/ColorBleedPingRT.width, -1.0f/ColorBleedPingRT.width, 12.0f/4096.0f);
	gxSetPixelShaderConst(2, -2.0f/ColorBleedPingRT.width,  0.0f/ColorBleedPingRT.width, 66.0f/4096.0f);
	gxSetPixelShaderConst(3, -1.0f/ColorBleedPingRT.width, -2.0f/ColorBleedPingRT.width, 220.0f/4096.0f);
	gxSetPixelShaderConst(4, -1.0f/ColorBleedPingRT.width, -1.0f/ColorBleedPingRT.width, 495.0f/4096.0f);
	gxSetPixelShaderConst(5, -1.0f/ColorBleedPingRT.width,  0.0f/ColorBleedPingRT.width, 792.0f/4096.0f);
	gxSetPixelShaderConst(6,  0.0f/ColorBleedPingRT.width,  0.0f/ColorBleedPingRT.width, 924.0f/4096.0f);
#endif

#ifdef PLATFORM_IPHONE
	gxSetShaderConstant(Gaussian13BlurOffsetScale0, -2.0f/ColorBleedPingRT.width, -2.0f/ColorBleedPingRT.width, 1.0f/4096.0f);
	gxSetShaderConstant(Gaussian13BlurOffsetScale1, -2.0f/ColorBleedPingRT.width, -1.0f/ColorBleedPingRT.width, 12.0f/4096.0f);
	gxSetShaderConstant(Gaussian13BlurOffsetScale2, -2.0f/ColorBleedPingRT.width,  0.0f/ColorBleedPingRT.width, 66.0f/4096.0f);
	gxSetShaderConstant(Gaussian13BlurOffsetScale3, -1.0f/ColorBleedPingRT.width, -2.0f/ColorBleedPingRT.width, 220.0f/4096.0f);
	gxSetShaderConstant(Gaussian13BlurOffsetScale4, -1.0f/ColorBleedPingRT.width, -1.0f/ColorBleedPingRT.width, 495.0f/4096.0f);
	gxSetShaderConstant(Gaussian13BlurOffsetScale5, -1.0f/ColorBleedPingRT.width,  0.0f/ColorBleedPingRT.width, 792.0f/4096.0f);
	gxSetShaderConstant(Gaussian13BlurOffsetScale6,  0.0f/ColorBleedPingRT.width,  0.0f/ColorBleedPingRT.width, 924.0f/4096.0f);
#endif

	gxCopyRenderTarget(&ColorRT, &ColorBleedDiv2RT);
	gxCopyRenderTarget(&ColorBleedDiv2RT, &ColorBleedDiv4RT);
	gxCopyRenderTarget(&ColorBleedDiv4RT, &ColorBleedDiv8RT);
	gxCopyRenderTarget(&ColorBleedDiv8RT, &ColorBleedPingRT);

	for (int i = 0; i < 2; i++)
	{
#ifdef PLATFORM_WINDOWS
		gxSetPixelShaderConst(0, -6.0f/ColorBleedPingRT.width, 0.0f, 1.0f/4096.0f);
		gxSetPixelShaderConst(1, -5.0f/ColorBleedPingRT.width, 0.0f, 12.0f/4096.0f);
		gxSetPixelShaderConst(2, -4.0f/ColorBleedPingRT.width, 0.0f, 66.0f/4096.0f);
		gxSetPixelShaderConst(3, -3.0f/ColorBleedPingRT.width, 0.0f, 220.0f/4096.0f);
		gxSetPixelShaderConst(4, -2.0f/ColorBleedPingRT.width, 0.0f, 495.0f/4096.0f);
		gxSetPixelShaderConst(5, -1.0f/ColorBleedPingRT.width, 0.0f, 792.0f/4096.0f);
		gxSetPixelShaderConst(6,  0.0f/ColorBleedPingRT.width, 0.0f, 924.0f/4096.0f);
#endif

#ifdef PLATFORM_IPHONE
		gxSetShaderConstant(Gaussian13BlurOffsetScale0, -6.0f/ColorBleedPingRT.width, 0.0f, 1.0f/4096.0f);
		gxSetShaderConstant(Gaussian13BlurOffsetScale1, -5.0f/ColorBleedPingRT.width, 0.0f, 12.0f/4096.0f);
		gxSetShaderConstant(Gaussian13BlurOffsetScale2, -4.0f/ColorBleedPingRT.width, 0.0f, 66.0f/4096.0f);
		gxSetShaderConstant(Gaussian13BlurOffsetScale3, -3.0f/ColorBleedPingRT.width, 0.0f, 220.0f/4096.0f);
		gxSetShaderConstant(Gaussian13BlurOffsetScale4, -2.0f/ColorBleedPingRT.width, 0.0f, 495.0f/4096.0f);
		gxSetShaderConstant(Gaussian13BlurOffsetScale5, -1.0f/ColorBleedPingRT.width, 0.0f, 792.0f/4096.0f);
		gxSetShaderConstant(Gaussian13BlurOffsetScale6,  0.0f/ColorBleedPingRT.width, 0.0f, 924.0f/4096.0f);
#endif

		gxCopyRenderTarget(&ColorBleedPingRT, &ColorBleedPongRT);

#ifdef PLATFORM_WINDOWS
		gxSetPixelShaderConst(0, 0.0f, -6.0f/ColorBleedPingRT.height, 1.0f/4096.0f);
		gxSetPixelShaderConst(1, 0.0f, -5.0f/ColorBleedPingRT.height, 12.0f/4096.0f);
		gxSetPixelShaderConst(2, 0.0f, -4.0f/ColorBleedPingRT.height, 66.0f/4096.0f);
		gxSetPixelShaderConst(3, 0.0f, -3.0f/ColorBleedPingRT.height, 220.0f/4096.0f);
		gxSetPixelShaderConst(4, 0.0f, -2.0f/ColorBleedPingRT.height, 495.0f/4096.0f);
		gxSetPixelShaderConst(5, 0.0f, -1.0f/ColorBleedPingRT.height, 792.0f/4096.0f);
		gxSetPixelShaderConst(6, 0.0f,  0.0f/ColorBleedPingRT.height, 924.0f/4096.0f);
#endif
        
#ifdef PLATFORM_IPHONE
		gxSetShaderConstant(Gaussian13BlurOffsetScale0, 0.0f, -6.0f/ColorBleedPingRT.height, 1.0f/4096.0f);
		gxSetShaderConstant(Gaussian13BlurOffsetScale1, 0.0f, -5.0f/ColorBleedPingRT.height, 12.0f/4096.0f);
		gxSetShaderConstant(Gaussian13BlurOffsetScale2, 0.0f, -4.0f/ColorBleedPingRT.height, 66.0f/4096.0f);
		gxSetShaderConstant(Gaussian13BlurOffsetScale3, 0.0f, -3.0f/ColorBleedPingRT.height, 220.0f/4096.0f);
		gxSetShaderConstant(Gaussian13BlurOffsetScale4, 0.0f, -2.0f/ColorBleedPingRT.height, 495.0f/4096.0f);
		gxSetShaderConstant(Gaussian13BlurOffsetScale5, 0.0f, -1.0f/ColorBleedPingRT.height, 792.0f/4096.0f);
		gxSetShaderConstant(Gaussian13BlurOffsetScale6, 0.0f,  0.0f/ColorBleedPingRT.height, 924.0f/4096.0f);
#endif
        
		gxCopyRenderTarget(&ColorBleedPongRT, &ColorBleedPingRT);
	}

#ifdef PLATFORM_WINDOWS
	gxSetPixelShader(&TexturedColoredShader);
#endif
    
#ifdef PLATFORM_IPHONE
    gxSetShader(&LitShader);
#endif
    
	gxCopyRenderTarget(&ColorBleedPingRT, &ColorBleedFinalRT);
}

void RenderCombinedColor()
{
#ifdef PLATFORM_WINDOWS
	gxSetPixelShader(&CombineShader);
#endif

#ifdef PLATFORM_IPHONE
    if (gxOpenGLESVersion == 2)
    {
        gxSetShader(&CombineShader);
    }
    else
    {
        // ES1 TODO
    }
#endif
    
	_gxSetAlpha(GXALPHA_NONE);
    
#ifdef PLATFORM_WINDOWS
	gxDev->SetTexture( 0, ColorRT.tex );
	gxDev->SetTexture( 1, ColorBleedFinalRT.tex );
	gxDev->SetTexture( 2, LightingRT.tex );
#endif
    
#ifdef PLATFORM_IPHONE
    if (gxOpenGLESVersion == 2)
    {
        gxSetShaderSampler(CombineColorSampler, &ColorRT);
    //    gxSetShaderSampler(CombineColorBleedSampler, &ColorBleedFinalRT);
        gxSetShaderSampler(CombineLightingSampler, &LightingRT);
    }
#endif
	
#ifdef PLATFORM_IPHONE
    _gxDrawQuad(0, 0, (float)gxScreenWidth, (float)gxScreenHeight, gxRGBA32(255,255,255,255), 0, 1, 1, 0);
#endif
#ifdef PLATFORM_WINDOWS
	_gxDrawQuad(0, 0, (float)gxScreenWidth, (float)gxScreenHeight, gxRGBA32(255,255,255,255), 0, 0, 1, 1);
#endif
    
#ifdef PLATFORM_WINDOWS
	gxSetPixelShader(&TexturedColoredShader);
#endif
    
#ifdef PLATFORM_IPHONE
    if (gxOpenGLESVersion == 2)
    {
        gxSetShader(&LitShader);
    }
#endif
}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                          Main Lighting Interface                                                                        //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void InitLighting()
{
    double StartTime = GetCurrentTime();
    
#ifdef PLATFORM_WINDOWS
    LitRenderTargetWidth = 320;
    LitRenderTargetHeight = 480;
	LitAspectScale = 1.0f;
	LitAspectOffset = 0.0f;
	LitScreenHeight = 1024;
    
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
#endif

#ifdef PLATFORM_IPHONE
    switch (gxGetDisplayType()) 
    {
    case GXDISPLAY_IPAD_PORTRAIT:
        LitRenderTargetWidth = 768;
        LitRenderTargetHeight = 1024;
        LitAspectScale = 1.0f;
        LitAspectOffset = 0.0f;
        LitScreenHeight = 1024.0f;
        break;
    case GXDISPLAY_IPHONE_RETINA_PORTRAIT:
        LitRenderTargetWidth = 640;
        LitRenderTargetHeight = 960;
        LitAspectScale = (640.0f/960.0f) / (768.0f/1024.0f);
        LitAspectOffset = 0.0f; //-(1024 - (1024 * LitAspectScale)) * 0.5f;
        LitScreenHeight = 1024 / LitAspectScale;
        break;
    default:
    case GXDISPLAY_IPHONE_PORTRAIT:
        LitRenderTargetWidth = 320;
        LitRenderTargetHeight = 480;
        LitAspectScale = (320.0f/480.0f) / (768.0f/1024.0f);
        LitAspectOffset = 0.0f; //-(1024 - (1024 * LitAspectScale)) * 0.5f;
        LitScreenHeight = 1024 / LitAspectScale;
        break;
    };
    
    // Create the vertex and index buffers.
    glGenBuffers(2, LitVertsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, LitVertsVBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(SLitVertex) * MAX_LIT_VERTS, NULL, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, LitVertsVBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(SLitVertex) * MAX_LIT_VERTS, NULL, GL_DYNAMIC_DRAW);
    
    glGenBuffers(2, LitVertsIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, LitVertsIBO[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * MAX_LIT_VERTS * 2, NULL, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, LitVertsIBO[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * MAX_LIT_VERTS * 2, NULL, GL_DYNAMIC_DRAW);
    
    BufferObjectIndex = 0;
    
    // Create shaders.
    if (gxOpenGLESVersion == 2)
    {
        gxCreateShader(LitVertexShaderSource, TexturedColoredShaderSource, &LitShader);

        gxCreateShader(LitVertexShaderSource, ShadowShaderSource, &ShadowShader);
        ShadowShadowOffset = gxGetShaderConstantByName(&ShadowShader, "Offset");
        ShadowShadowAlpha = gxGetShaderConstantByName(&ShadowShader, "ShadowAlpha");
        ShadowSampler = gxGetShaderConstantByName(&ShadowShader, "Sampler");
        
        gxCreateShader(LitVertexShaderSource, ApplyShadowShaderSource, &ApplyShadowShader);
        ApplyShadowSampler = gxGetShaderConstantByName(&ApplyShadowShader, "Sampler");
        
        gxCreateShader(LitVertexShaderSource, EffectsShaderSource, &EffectsShader);
        EffectsSampler = gxGetShaderConstantByName(&EffectsShader, "Sampler");
        
        gxCreateShader(LitVertexShaderSource, Gaussian7ShaderSource, &Gaussian7Shader);
        Gaussian7BlurOffsetScale0 = gxGetShaderConstantByName(&Gaussian7Shader, "BlurOffsetScale0");
        Gaussian7BlurOffsetScale1 = gxGetShaderConstantByName(&Gaussian7Shader, "BlurOffsetScale1");
        Gaussian7BlurOffsetScale2 = gxGetShaderConstantByName(&Gaussian7Shader, "BlurOffsetScale2");
        Gaussian7BlurOffsetScale3 = gxGetShaderConstantByName(&Gaussian7Shader, "BlurOffsetScale3");
        Gaussian7BlurSampler = gxGetShaderConstantByName(&Gaussian7Shader, "BlurSampler");
        
        gxCreateShader(LitVertexShaderSource, Gaussian13ShaderSource, &Gaussian13Shader);
        Gaussian13BlurOffsetScale0 = gxGetShaderConstantByName(&Gaussian13Shader, "BlurOffsetScale0");
        Gaussian13BlurOffsetScale1 = gxGetShaderConstantByName(&Gaussian13Shader, "BlurOffsetScale1");
        Gaussian13BlurOffsetScale2 = gxGetShaderConstantByName(&Gaussian13Shader, "BlurOffsetScale2");
        Gaussian13BlurOffsetScale3 = gxGetShaderConstantByName(&Gaussian13Shader, "BlurOffsetScale3");
        Gaussian13BlurOffsetScale4 = gxGetShaderConstantByName(&Gaussian13Shader, "BlurOffsetScale4");
        Gaussian13BlurOffsetScale5 = gxGetShaderConstantByName(&Gaussian13Shader, "BlurOffsetScale5");
        Gaussian13BlurOffsetScale6 = gxGetShaderConstantByName(&Gaussian13Shader, "BlurOffsetScale6");
        Gaussian13BlurSampler = gxGetShaderConstantByName(&Gaussian13Shader, "BlurSampler");

        gxCreateShader(LitVertexShaderSource, CombineShaderSource, &CombineShader);
        CombineColorSampler = gxGetShaderConstantByName(&CombineShader, "ColorSampler");
        CombineColorBleedSampler = gxGetShaderConstantByName(&CombineShader, "ColorBleedSampler");
        CombineLightingSampler = gxGetShaderConstantByName(&CombineShader, "LightingSampler");
    }
#endif
    
	// Create render targets.
#ifdef PLATFORM_IPHONE
    if (gxOpenGLESVersion == 2)
#endif
    {
        gxCreateRenderTarget(LitRenderTargetWidth, LitRenderTargetHeight, &ColorRT, false);
        gxCreateRenderTarget(LitRenderTargetHeight, LitRenderTargetHeight, &LightingRT, false);

        InitAmbientOcclusion();
        InitShadows();
        //InitColorBleed();
    }
    
    double EndTime = GetCurrentTime();
    LogMessage("Lighting setup took %.1f seconds.\n", EndTime-StartTime);
}

void ResetLighting()
{
	NLitVerts = 0;

    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
    //                                                   Begin writing buffers                                                                 //
    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
#ifdef PLATFORM_IPHONE
    NLitVertIndices = 0;
    
    BufferObjectIndex ^= 1;
    
    glBindBuffer(GL_ARRAY_BUFFER, LitVertsVBO[BufferObjectIndex]);
    LitVerts = (SLitVertex*)glMapBufferOES(GL_ARRAY_BUFFER, GL_WRITE_ONLY_OES);
    assert(LitVerts);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, LitVertsIBO[BufferObjectIndex]);
    LitVertIndices = (GLushort*)glMapBufferOES(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY_OES);
    assert(LitVertIndices);
#endif
    
	for (int i = 0; i < LIGHTLIST_COUNT; i++)
	{
		LightLists[i].NQuads = 0;
	}
}

void RenderLighting()
{
    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
    //                                                   Finish writing buffers                                                                //
    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
#ifdef PLATFORM_IPHONE
    glBindBuffer(GL_ARRAY_BUFFER, LitVertsVBO[BufferObjectIndex]);
    glUnmapBufferOES(GL_ARRAY_BUFFER);
    LitVerts = NULL;
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, LitVertsIBO[BufferObjectIndex]);
    glUnmapBufferOES(GL_ELEMENT_ARRAY_BUFFER);
    LitVertIndices = NULL;
#endif
    
    
    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
    //                                                   Set up lighting globals                                                               //
    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
	if (Chapter.PageProps.LightsOff)
		LightState.AmbientColor = gxRGBA32(16, 16, 16, 255);
	else
		LightState.AmbientColor = gxRGBA32(128, 128, 128, 255);

    
#ifdef PLATFORM_IPHONE
    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
    //                                                         OpenGL ES 1.1 Renderer                                                          //
    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
    if (gxOpenGLESVersion == 1)
    {
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
        //                                                   Build lighting                                                                        //
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
        //gxSetRenderTarget(&LightingRT);
        //gxClearColor(LightState.AmbientColor);
        
        //DrawLightList(LIGHTLIST_LIGHTING, GXALPHA_ADD);
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
        //                                                   Draw layers to screen                                                                 //
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
        gxSetRenderTarget(NULL);
        
        // White background - useful for testing AO and stuff.
        gxClearColor(gxRGBA32(255, 255, 255, 255));
        // Black background - useful for testing fireworks.
        //gxClearColor(gxRGBA32(0, 0, 0, 255));
        
        DrawLightList(LIGHTLIST_BACKGROUND, GXALPHA_NONE);
        //DrawLightListShadow(LIGHTLIST_FOREGROUND, GXALPHA_BLEND);        
        DrawLightList(LIGHTLIST_DUST, GXALPHA_BLEND);
        DrawLightList(LIGHTLIST_FOREGROUND, GXALPHA_BLEND);
        DrawLightList(LIGHTLIST_FOREGROUND_NO_SHADOW, GXALPHA_BLEND);        
        //DrawLightListShadow(LIGHTLIST_VACUUM, GXALPHA_BLEND);        
        DrawLightList(LIGHTLIST_VACUUM, GXALPHA_BLEND);
        DrawLightList(LIGHTLIST_EFFECTS, GXALPHA_ADD);
        DrawLightList(LIGHTLIST_WIPE, GXALPHA_BLEND);
    }
    else
#endif
    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
    //                                                         OpenGL ES 2.0 Renderer                                                          //
    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
    {
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
        //                                                   Build shadows and ambient occlusion                                                   //
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
        _gxSetAlpha(GXALPHA_BLEND);
        
        //DrawAmbientOcclusion(LIGHTLIST_FOREGROUND, &AmbientOcclusionForegroundRT);
        //DrawAmbientOcclusion(LIGHTLIST_VACUUM, &AmbientOcclusionVacuumRT);
        
        DrawShadows(LIGHTLIST_FOREGROUND, &ShadowForegroundRT, 30, 20);
        DrawShadows(LIGHTLIST_VACUUM, &ShadowVacuumRT, 30, 20);
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
        //                                                   Build lighting                                                                        //
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
#ifdef PLATFORM_WINDOWS
        gxSetPixelShader(&TexturedColoredShader);
        gxSetVertexShader(&LitVertexShader);
#endif
        
#ifdef PLATFORM_IPHONE
        if (gxOpenGLESVersion == 2)
        {
            gxSetShader(&LitShader);
        }
        else
        {
            // ES1 TODO (consider passing shader to DrawLightList?)
        }
#endif
        
        gxSetRenderTarget(&LightingRT);
        gxClearColor(LightState.AmbientColor);
        
        DrawLightList(LIGHTLIST_LIGHTING, GXALPHA_ADD);
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
        //                                                   Blur shadows and ambient occlusion                                                    //
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
        //BlurAmbientOcclusion(&AmbientOcclusionForegroundRT);
        //BlurAmbientOcclusion(&AmbientOcclusionVacuumRT);
        
        //BlurShadows(&ShadowForegroundRT);
        //BlurShadows(&ShadowVacuumRT);
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
        //                                                   Draw layers                                                                           //
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
        // Main rendering.
        gxSetRenderTarget(&ColorRT);
        
        // White background - useful for testing AO and stuff.
        gxClearColor(gxRGBA32(255, 255, 255, 255));
        // Black background - useful for testing fireworks.
        //gxClearColor(gxRGBA32(0, 0, 0, 255));
        
        // Real background.
#ifdef PLATFORM_WINDOWS
        gxSetPixelShader(&TexturedColoredShader);
        gxSetVertexShader(&LitVertexShader);
#endif
        
#ifdef PLATFORM_IPHONE
        if (gxOpenGLESVersion == 2)
        {
            gxSetShader(&LitShader);
        }
        else
        {
            // ES1 TODO
        }
#endif
        
        DrawLightList(LIGHTLIST_BACKGROUND, GXALPHA_NONE);
        
        // Foreground ambient occlusion & shadows.
        //RenderAmbientOcclusion(&AmbientOcclusionForegroundRT);
        RenderShadows(&ShadowForegroundRT);
        
        // Dust layer.
#ifdef PLATFORM_WINDOWS
        gxSetPixelShader(&TexturedColoredShader);
        gxSetVertexShader(&LitVertexShader);
#endif
        
#ifdef PLATFORM_IPHONE
        if (gxOpenGLESVersion == 2)
        {
            gxSetShader(&LitShader);
        }
        else
        {
            // ES1 TODO
        }
#endif
        
        DrawLightList(LIGHTLIST_DUST, GXALPHA_BLEND);
        DrawLightList(LIGHTLIST_FOREGROUND, GXALPHA_BLEND);
        DrawLightList(LIGHTLIST_FOREGROUND_NO_SHADOW, GXALPHA_BLEND);
        
        // Vacuum ambient occlusion & shadows.
        //RenderAmbientOcclusion(&AmbientOcclusionVacuumRT);
        RenderShadows(&ShadowVacuumRT);
        
#ifdef PLATFORM_WINDOWS
        gxSetPixelShader(&TexturedColoredShader);
        gxSetVertexShader(&LitVertexShader);
#endif
        
#ifdef PLATFORM_IPHONE
        if (gxOpenGLESVersion == 2)
        {
            gxSetShader(&LitShader);        
        }
        else
        {
            // ES1 TODO
        }
#endif
        
        // Vacuum.
        DrawLightList(LIGHTLIST_VACUUM, GXALPHA_BLEND);
        
        // Color bleeding - Adds color and bleed color into framebuffer.
        //BuildColorBleed();
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
        //                                                   Combine layers into frame                                                             //
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
        
        // Combine everything into the final output.
#ifdef PLATFORM_IPHONE
        if (gxOpenGLESVersion == 2)
#endif
        {
            gxSetRenderTarget(NULL);
            RenderCombinedColor();
        }
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
        //                                                   Draw effects on top                                                                   //
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
#ifdef PLATFORM_WINDOWS
        gxSetPixelShader(&EffectsShader);
        gxSetVertexShader(&LitVertexShader);
#endif
        
#ifdef PLATFORM_IPHONE
        if (gxOpenGLESVersion == 2)
        {
            gxSetShader(&EffectsShader);
        }
        else
        {
            // ES1 TODO
        }
#endif
        
        DrawLightList(LIGHTLIST_EFFECTS, GXALPHA_ADD);
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
        //                                                   Draw screen wipes                                                                     //
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
        // Screen wipe.
#ifdef PLATFORM_WINDOWS
        gxSetPixelShader(&TexturedColoredShader);
        gxSetVertexShader(&LitVertexShader);
#endif
        
#ifdef PLATFORM_IPHONE
        if (gxOpenGLESVersion == 2)
        {
            gxSetShader(&LitShader);
        }
        else
        {
            // ES1 TODO
        }
#endif
        
        DrawLightList(LIGHTLIST_WIPE, GXALPHA_BLEND);
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

#ifdef PLATFORM_IPHONE
void AddLitQuadStripIndices(SLitQuad* Quad, int NVerts)
{
    Quad->BaseIndex = NLitVertIndices;
    
    int Index = NLitVertIndices;
    
    if (NLitVerts > 0)
        LitVertIndices[Index++] = NLitVerts;

    for (int i = 0; i < NVerts; i++)
        LitVertIndices[Index++] = NLitVerts+i;
    
    LitVertIndices[Index++] = NLitVerts+NVerts-1;
    
    Quad->IndexCount = Index - Quad->BaseIndex;
    NLitVertIndices = Index;
}
#endif

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
	
    float tw = (float)Sprite->width / (float)Sprite->texWidth;
    float th = (float)Sprite->height / (float)Sprite->texHeight;
    
	Quad->List = List;
	Quad->Sprite = Sprite;

#ifdef PLATFORM_IPHONE
    AddLitQuadStripIndices(Quad, 4);
#endif    

	Quad->NVerts = 4;
	Quad->Verts = AllocateLitVerts(4);

    // TODO: Move math to GPU, as an optimization.
    if (List != LIGHTLIST_WIPE)
    {
        X0 = LitSceneOffsetX + (X0 - (gxScreenWidth/2)) * LitSceneZoom + (gxScreenWidth/2);
        X1 = LitSceneOffsetX + (X1 - (gxScreenWidth/2)) * LitSceneZoom + (gxScreenWidth/2);
        X2 = LitSceneOffsetX + (X2 - (gxScreenWidth/2)) * LitSceneZoom + (gxScreenWidth/2);
        X3 = LitSceneOffsetX + (X3 - (gxScreenWidth/2)) * LitSceneZoom + (gxScreenWidth/2);
        Y0 = LitSceneOffsetY + (Y0 - (LitScreenHeight/2)) * LitSceneZoom + (LitScreenHeight/2);
        Y1 = LitSceneOffsetY + (Y1 - (LitScreenHeight/2)) * LitSceneZoom + (LitScreenHeight/2);
        Y2 = LitSceneOffsetY + (Y2 - (LitScreenHeight/2)) * LitSceneZoom + (LitScreenHeight/2);
        Y3 = LitSceneOffsetY + (Y3 - (LitScreenHeight/2)) * LitSceneZoom + (LitScreenHeight/2);
    }
    
	Quad->Verts[0].X = X0;
	Quad->Verts[0].Y = LitAspectOffset + Y0 * LitAspectScale;
	Quad->Verts[0].U = U0 * tw;
	Quad->Verts[0].V = V0 * th;
	Quad->Verts[0].Color = Color;

	Quad->Verts[1].X = X1;
	Quad->Verts[1].Y = LitAspectOffset + Y1 * LitAspectScale;
	Quad->Verts[1].U = U1 * tw;
	Quad->Verts[1].V = V1 * th;
	Quad->Verts[1].Color = Color;

	Quad->Verts[3].X = X2;
	Quad->Verts[3].Y = LitAspectOffset + Y2 * LitAspectScale;
	Quad->Verts[3].U = U2 * tw;
	Quad->Verts[3].V = V2 * th;
	Quad->Verts[3].Color = Color;

	Quad->Verts[2].X = X3;
	Quad->Verts[2].Y = LitAspectOffset + Y3 * LitAspectScale;
	Quad->Verts[2].U = U3 * tw;
	Quad->Verts[2].V = V3 * th;
	Quad->Verts[2].Color = Color;
}

SLitVertex* AddLitQuad(ELightList List, gxSprite* Sprite, int NVerts)
{
	if (LightLists[List].NQuads >= MAX_LIT_QUADS_PER_LIST)
		ReportError("Exceeded the maximum of %d lit quads per light list.", MAX_LIT_QUADS_PER_LIST);

	SLitQuad* Quad = &LightLists[List].Quads[LightLists[List].NQuads++];

	Quad->List = List;
	Quad->Sprite = Sprite;

#ifdef PLATFORM_IPHONE
    AddLitQuadStripIndices(Quad, NVerts);
#endif    

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

void AddLitSpriteSized(ELightList List, gxSprite* Sprite, float X, float Y, float SizeX, float SizeY)
{
	AddLitQuad(List, Sprite, gxRGBA32(255,255,255,255),
               X,                      Y,                       0.0f, 0.0f, 
               X+SizeX,                Y,                       1.0f, 0.0f,
               X+SizeX,                Y+SizeY,                 1.0f, 1.0f, 
               X,                      Y+SizeY,                 0.0f, 1.0f);
}

void AddLitSpriteSizedColor(ELightList List, gxSprite* Sprite, float X, float Y, float SizeX, float SizeY, unsigned int Color)
{
	AddLitQuad(List, Sprite, Color,
               X,                      Y,                       0.0f, 0.0f, 
               X+SizeX,                Y,                       1.0f, 0.0f,
               X+SizeX,                Y+SizeY,                 1.0f, 1.0f, 
               X,                      Y+SizeY,                 0.0f, 1.0f);
}

void AddLitSpriteAlpha(ELightList List, gxSprite* Sprite, float X, float Y, float Alpha)
{
	AddLitQuad(List, Sprite, gxRGBA32(255,255,255,(int)(255*Alpha)),
               X,               Y,                0.0f, 0.0f, 
               X+Sprite->width, Y,                1.0f, 0.0f,
               X+Sprite->width, Y+Sprite->height, 1.0f, 1.0f, 
               X,               Y+Sprite->height, 0.0f, 1.0f);
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
		X,               Y,               SubX1/Sprite->width, SubY1/Sprite->height, 
		X+(SubX2-SubX1), Y,               SubX2/Sprite->width, SubY1/Sprite->height,
		X+(SubX2-SubX1), Y+(SubY2-SubY1), SubX2/Sprite->width, SubY2/Sprite->height, 
		X,               Y+(SubY2-SubY1), SubX1/Sprite->width, SubY2/Sprite->height);
}

void AddLitSubSpriteScaled(ELightList List, gxSprite* Sprite, float X, float Y, float SubX1, float SubY1, float SubX2, float SubY2, float ScaleX, float ScaleY)
{
	AddLitQuad(List, Sprite, gxRGBA32(255,255,255,255),
		X,                      Y,                      SubX1/Sprite->width, SubY1/Sprite->height, 
		X+(SubX2-SubX1)*ScaleX, Y,                      SubX2/Sprite->width, SubY1/Sprite->height,
		X+(SubX2-SubX1)*ScaleX, Y+(SubY2-SubY1)*ScaleY, SubX2/Sprite->width, SubY2/Sprite->height, 
		X,                      Y+(SubY2-SubY1)*ScaleY, SubX1/Sprite->width, SubY2/Sprite->height);
}

void AddLitSubSpriteCenteredScaledRotated(ELightList List, gxSprite* Sprite, float X, float Y, float SubX1, float SubY1, float SubX2, float SubY2, float Scale, float Angle)
{
	float ca = cosf(Angle);
	float sa = sinf(Angle);

	float w = Scale * (SubX2-SubX1) * 0.5f;
	float h = Scale * (SubY2-SubY1) * 0.5f;

	AddLitQuad(List, Sprite, gxRGBA32(255,255,255,255),
		X + (-w * ca) - (-h * sa), Y + (-w * sa) + (-h * ca), SubX1/Sprite->width, SubY1/Sprite->height, 
		X + (+w * ca) - (-h * sa), Y + (+w * sa) + (-h * ca), SubX2/Sprite->width, SubY1/Sprite->height,
		X + (+w * ca) - (+h * sa), Y + (+w * sa) + (+h * ca), SubX2/Sprite->width, SubY2/Sprite->height, 
		X + (-w * ca) - (+h * sa), Y + (-w * sa) + (+h * ca), SubX1/Sprite->width, SubY2/Sprite->height);
}

void AddLitSpriteCenteredScaledRotated(ELightList List, gxSprite* Sprite, float X, float Y, float Scale, float Angle)
{
	float ca = cosf(Angle);
	float sa = sinf(Angle);

	float w = Scale * (float)Sprite->width * 0.5f;
	float h = Scale * (float)Sprite->height * 0.5f;

	AddLitQuad(List, Sprite, gxRGBA32(255,255,255,255),
		X + (-w * ca) - (-h * sa),    Y + (-w * sa) + (-h * ca),    0.0f, 0.0f, 
		X + (+w * ca) - (-h * sa),    Y + (+w * sa) + (-h * ca),    1.0f, 0.0f, 
		X + (+w * ca) - (+h * sa),    Y + (+w * sa) + (+h * ca),    1.0f, 1.0f, 
		X + (-w * ca) - (+h * sa),    Y + (-w * sa) + (+h * ca),    0.0f, 1.0f);
}


void AddLitSpriteCenteredScaledRotatedAlpha(ELightList List, gxSprite* Sprite, float X, float Y, float Scale, float Angle, float Alpha)
{
	float ca = cosf(Angle);
	float sa = sinf(Angle);

	float w = (float)Sprite->width * 0.5f;
	float h = (float)Sprite->height * 0.5f;

	AddLitQuad(List, Sprite, gxRGBA32(255,255,255,(int)(255*Alpha)),
		X + (-w * ca) - (-h * sa),    Y + (-w * sa) + (-h * ca),    0.0f, 0.0f, 
		X + (+w * ca) - (-h * sa),    Y + (+w * sa) + (-h * ca),    1.0f, 0.0f, 
		X + (+w * ca) - (+h * sa),    Y + (+w * sa) + (+h * ca),    1.0f, 1.0f, 
		X + (-w * ca) - (+h * sa),    Y + (-w * sa) + (+h * ca),    0.0f, 1.0f);
}
