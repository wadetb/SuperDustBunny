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
#include "Flashlight.h"


#define MAX_LIT_QUADS_PER_LIST	8192 // Holy crap
#define MAX_LIT_VERTS			65536


int NLitVerts;

int CurrentBlendMode;
gxShader* CurrentShader;

#ifdef PLATFORM_IPHONE_OR_MAC

int NLitVertIndices;

int BufferObjectIndex;

#define LIT_VBO_COUNT 3

GLuint LitVertsVBO[LIT_VBO_COUNT];
GLuint LitVertsIBO[LIT_VBO_COUNT];
GLuint LitVertsVAO[LIT_VBO_COUNT];

GLuint ScreenVAO;
GLuint ScreenVBO;

SLitVertex* LitVerts;
GLushort* LitVertIndices;

SLitVertex ScreenVerts[4] =
{
    { 0.0f,    0.0f,    0.0f, 1.0f, 0xffffffff },
    { 768.0f,  0.0f,    1.0f, 1.0f, 0xffffffff },
    { 0.0f,    1024.0f, 0.0f, 0.0f, 0xffffffff },
    { 768.0f,  1024.0f, 1.0f, 0.0f, 0xffffffff },
};

GLuint CurrentVAO;

#endif

#ifdef PLATFORM_WINDOWS

SLitVertex LitVerts[MAX_LIT_VERTS];

SLitVertex ScreenVerts[4] =
{
    { 0.0f,    0.0f,    0.0f, 0.0f, 0xffffffff },
    { 768.0f,  0.0f,    1.0f, 0.0f, 0xffffffff },
    { 0.0f,    1024.0f, 0.0f, 1.0f, 0xffffffff },
    { 768.0f,  1024.0f, 1.0f, 1.0f, 0xffffffff },
};

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

gxSprite ShadowForegroundRT;
gxSprite ShadowVacuumRT;


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

const char* LitShaderSource =
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

const char* ShadowVertexShaderSource =
"float2 ShadowOffset : register(c0);\n"
"float2 ShadowScale : register(c1);\n"
"float2 LightOrigin : register(c2);\n"
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
"   float2 ShadowDir = VertexInput.Position - LightOrigin;\n"
"   float2 ShadowPos = LightOrigin + ShadowDir*ShadowScale + ShadowOffset;\n"
"	VertexOutput.Position = float4(ShadowPos.x/768.0*2.0-1.0, (1.0-ShadowPos.y/1024.0)*2.0-1.0, 0, 1);\n"
"	VertexOutput.TexCoord0 = VertexInput.TexCoord0;\n"
"	return VertexOutput;\n"
"}\n";

const char* ShadowShaderSource =
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

const char* StandardVertexShaderSource =
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

const char* CombineShaderSource =
"sampler ColorSampler : register(s0);\n"
"sampler LightingSampler : register(s1);\n"
"\n"
"struct SVertexOutput\n"
"{\n"
"	float2 TexCoord0 : TEXCOORD0;\n"
"};\n"
"\n"
"float4 main(SVertexOutput VertexOutput) : COLOR\n"
"{\n"
"	float4 Color = tex2D(ColorSampler, VertexOutput.TexCoord0);\n"
"	float4 Lighting = tex2D(LightingSampler, VertexOutput.TexCoord0);\n"
"   return (Lighting*2.0) * Color;\n"
"}\n";

#endif


#ifdef PLATFORM_MAC

const char* LitVertexShaderSource =
"#version 120\n"
"attribute vec2 PositionAttr;\n"
"attribute vec2 TexCoordAttr;\n"
"attribute vec4 ColorAttr;\n"
"\n"
"varying vec2 TexCoordInterp;\n"
"varying vec4 ColorInterp;\n"
"\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(PositionAttr.x/768.0*2.0-1.0, (1.0-PositionAttr.y/1024.0)*2.0-1.0, 0, 1);\n"
"	TexCoordInterp = TexCoordAttr;\n"
"	ColorInterp = ColorAttr;\n"
"}\n";

const char* LitShaderSource =
"#version 120\n"
"uniform sampler2D ColorSampler;\n"
"\n"
"varying vec2 TexCoordInterp;\n"
"varying vec4 ColorInterp;\n"
"\n"
"void main()\n"
"{\n"
"	gl_FragColor = texture2D(ColorSampler, TexCoordInterp) * ColorInterp;\n"
"}\n";

const char* ShadowVertexShaderSource =
"#version 120\n"
"uniform vec2 ShadowOffset;\n"
"uniform vec2 ShadowScale;\n"
"uniform vec2 LightOrigin;\n"
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
"   vec2 ShadowDir = PositionAttr - LightOrigin;\n"
"   vec2 ShadowPos = LightOrigin + ShadowDir*ShadowScale + ShadowOffset;\n"
"	gl_Position = vec4(ShadowPos.x/768.0*2.0-1.0, (1.0-ShadowPos.y/1024.0)*2.0-1.0, 0, 1);\n"
"	TexCoordInterp = TexCoordAttr;\n"
"	ColorInterp = ColorAttr;\n"
"}\n";

const char* ShadowShaderSource =
"#version 120\n"
"uniform float ShadowAlpha;\n"
"uniform sampler2D ColorSampler;\n"
"\n"
"varying vec2 TexCoordInterp;\n"
"varying vec4 ColorInterp;\n"
"\n"
"void main()\n"
"{\n"
"	gl_FragColor = vec4(0.0, 0.0, 0.0, ColorInterp.a * ShadowAlpha * texture2D(ColorSampler, TexCoordInterp).a);\n"
"}\n";

const char* StandardVertexShaderSource =
"#version 120\n"
"attribute vec2 PositionAttr;\n"
"attribute vec2 TexCoordAttr;\n"
"attribute vec4 ColorAttr;\n"
"\n"
"varying vec2 TexCoordInterp;\n"
"varying vec4 ColorInterp;\n"
"\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(PositionAttr.x/768.0*2.0-1.0, (1.0-PositionAttr.y/1024.0)*2.0-1.0, 0, 1);\n"
"	TexCoordInterp = TexCoordAttr;\n"
"	ColorInterp = ColorAttr;\n"
"}\n";

const char* EffectsShaderSource =
"#version 120\n"
"uniform sampler2D ColorSampler;\n"
"\n"
"varying vec2 TexCoordInterp;\n"
"varying vec4 ColorInterp;\n"
"\n"
"void main()\n"
"{\n"
"	gl_FragColor = texture2D(ColorSampler, TexCoordInterp) * ColorInterp * 2.0;\n"
"}\n";

const char* CombineShaderSource =
"#version 120\n"
"uniform sampler2D ColorSampler;\n"
"uniform sampler2D LightingSampler;\n"
"\n"
"varying vec2 TexCoordInterp;\n"
"\n"
"void main()\n"
"{\n"
"	vec4 SceneColor = texture2D(ColorSampler, TexCoordInterp);\n"
"	vec4 Lighting = texture2D(LightingSampler, TexCoordInterp);\n"
"   gl_FragColor = (Lighting*2.0) * SceneColor;\n"
"}\n";

#endif


#ifdef PLATFORM_IPHONE

const char* LitVertexShaderSource =
"attribute vec2 PositionAttr;\n"
"attribute vec2 TexCoordAttr;\n"
"attribute vec4 ColorAttr;\n"
"\n"
"varying lowp vec2 TexCoordInterp;\n"
"varying lowp vec4 ColorInterp;\n"
"\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(PositionAttr.x/768.0*2.0-1.0, (1.0-PositionAttr.y/1024.0)*2.0-1.0, 0, 1);\n"
"	TexCoordInterp = TexCoordAttr;\n"
"	ColorInterp = ColorAttr;\n"
"}\n";

const char* LitShaderSource =
"uniform lowp sampler2D Sampler;\n"
"\n"
"varying lowp vec2 TexCoordInterp;\n"
"varying lowp vec4 ColorInterp;\n"
"\n"
"void main()\n"
"{\n"
"	gl_FragColor = texture2D(Sampler, TexCoordInterp) * ColorInterp;\n"
"}\n";

const char* ShadowVertexShaderSource =
"uniform vec2 ShadowOffset;\n"
"uniform vec2 ShadowScale;\n"
"uniform vec2 LightOrigin;\n"
"\n"
"attribute vec2 PositionAttr;\n"
"attribute vec2 TexCoordAttr;\n"
"attribute vec4 ColorAttr;\n"
"\n"
"varying lowp vec2 TexCoordInterp;\n"
"varying lowp vec4 ColorInterp;\n"
"\n"
"void main()\n"
"{\n"
"   vec2 ShadowDir = PositionAttr - LightOrigin;\n"
"   vec2 ShadowPos = LightOrigin + ShadowDir*ShadowScale + ShadowOffset;\n"
"	gl_Position = vec4(ShadowPos.x/768.0*2.0-1.0, (1.0-ShadowPos.y/1024.0)*2.0-1.0, 0, 1);\n"
"	TexCoordInterp = TexCoordAttr;\n"
"	ColorInterp = ColorAttr;\n"
"}\n";

const char* ShadowShaderSource =
"uniform lowp float ShadowAlpha;\n"
"uniform lowp sampler2D Sampler;\n"
"\n"
"varying lowp vec2 TexCoordInterp;\n"
"varying lowp vec4 ColorInterp;\n"
"\n"
"void main()\n"
"{\n"
"	gl_FragColor = vec4(0.0, 0.0, 0.0, ColorInterp.a * ShadowAlpha * texture2D(Sampler, TexCoordInterp).a);\n"
"}\n";

const char* StandardVertexShaderSource =
"attribute vec2 PositionAttr;\n"
"attribute vec2 TexCoordAttr;\n"
"attribute vec4 ColorAttr;\n"
"\n"
"varying lowp vec2 TexCoordInterp;\n"
"varying lowp vec4 ColorInterp;\n"
"\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(PositionAttr.x/768.0*2.0-1.0, (1.0-PositionAttr.y/1024.0)*2.0-1.0, 0, 1);\n"
"	TexCoordInterp = TexCoordAttr;\n"
"	ColorInterp = ColorAttr;\n"
"}\n";

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

const char* CombineShaderSource =
"uniform lowp sampler2D ColorSampler;\n"
"uniform lowp sampler2D LightingSampler;\n"
"\n"
"varying lowp vec2 TexCoordInterp;\n"
"\n"
"void main()\n"
"{\n"
"	lowp vec4 Color = texture2D(ColorSampler, TexCoordInterp);\n"
"	lowp vec4 Lighting = texture2D(LightingSampler, TexCoordInterp);\n"
"   gl_FragColor = (Lighting*2.0) * Color;\n"
"}\n";

#endif


gxShader LitShader;

gxShaderConstant ShadowShadowOffset;
gxShaderConstant ShadowShadowScale;
gxShaderConstant ShadowLightOrigin;
gxShaderConstant ShadowShadowAlpha;
gxShaderConstant ShadowSampler;

gxShader ShadowShader;

gxShaderConstant EffectsSampler;

gxShader EffectsShader;

gxShaderConstant CombineColorSampler;
gxShaderConstant CombineLightingSampler;

gxShader CombineShader;


// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                      Platform specific drawing                                                          //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

static void DrawLightList(int List, gxShader* Shader, gxAlphaMode Alpha)
{
    if (Alpha != CurrentBlendMode)
    {
        CurrentBlendMode = Alpha;
        _gxSetAlpha(Alpha);
    }
    
    if (Shader != CurrentShader)
    {
        CurrentShader = Shader;
        if (Shader)
            gxSetShader(Shader);
    }

#ifdef PLATFORM_WINDOWS
    for (int i = 0; i < LightLists[List].NQuads; i++)
    {
        SLitQuad* Quad = &LightLists[List].Quads[i];
        
        _gxSetTexture(Quad->Sprite);
        
        gxDev->SetVertexDeclaration( LitVertexDecl );
        gxDev->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, Quad->NVerts-2, Quad->Verts, sizeof(SLitVertex) );
    }
#endif
    
#ifdef PLATFORM_IPHONE_OR_MAC
    if (gxOpenGLESVersion == 2)
    {
        if (CurrentVAO != LitVertsVAO[BufferObjectIndex])
        {
            CurrentVAO = LitVertsVAO[BufferObjectIndex];

            glBindVertexArray(LitVertsVAO[BufferObjectIndex]);
        }
    }
    else
    {
        glBindBuffer(GL_ARRAY_BUFFER, LitVertsVBO[BufferObjectIndex]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, LitVertsIBO[BufferObjectIndex]);
        
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
#endif
}

static void DrawScreen(gxShader* Shader, gxAlphaMode Alpha)
{
    _gxSetAlpha(Alpha);

    if (Shader)
        gxSetShader(Shader);

#ifdef PLATFORM_WINDOWS
    gxDev->SetVertexDeclaration( LitVertexDecl );
    gxDev->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 4, ScreenVerts, sizeof(SLitVertex) );
#endif

#ifdef PLATFORM_IPHONE_OR_MAC
    if (gxOpenGLESVersion == 2)
    {
        if (CurrentVAO != ScreenVAO)
        {
            CurrentVAO = ScreenVAO;
            
            glBindVertexArray(ScreenVAO);
        }
    }
    else
    {
        glBindBuffer(GL_ARRAY_BUFFER, ScreenVBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        
        glVertexPointer(3, GL_FLOAT, sizeof(SLitVertex), (void*)offsetof(SLitVertex, X));
        glTexCoordPointer(2, GL_FLOAT, sizeof(SLitVertex), (void*)offsetof(SLitVertex, U));
        glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(SLitVertex), (void*)offsetof(SLitVertex, Color));        
    }
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif
}


// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                      Shadows                                                                            //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

static void InitShadows()
{
#ifdef PLATFORM_WINDOWS
	gxCreateRenderTarget(LitRenderTargetWidth, LitRenderTargetHeight, &ShadowForegroundRT, true);
	gxCreateRenderTarget(LitRenderTargetWidth, LitRenderTargetHeight, &ShadowVacuumRT, true);
#endif

#ifdef PLATFORM_IPHONE_OR_MAC
	gxCreateRenderTarget(LitRenderTargetWidth/2, LitRenderTargetHeight/2, &ShadowForegroundRT, true);
	gxCreateRenderTarget(LitRenderTargetWidth/2, LitRenderTargetHeight/2, &ShadowVacuumRT, true);
#endif
}

static void DrawShadows(ELightList List, gxSprite* FinalRT)
{
    if (FinalRT != &ColorRT)
    {
        gxSetRenderTarget(FinalRT);
        gxClearColor(gxRGBA32(0, 0, 0, 0));
    }

    gxSetShader(&ShadowShader);
    
/*
    if (Chapter.Name)
    {
        gxSetShaderConstant2(ShadowShadowOffset, Chapter.PageProps.ShadowOffsetX, Chapter.PageProps.ShadowOffsetY);
        gxSetShaderConstant2(ShadowShadowScale, Chapter.PageProps.ShadowScaleX, Chapter.PageProps.ShadowScaleY);
        gxSetShaderConstant2(ShadowLightOrigin, Chapter.PageProps.LightOriginX, Chapter.PageProps.LightOriginY);
    }
    else
    {
        gxSetShaderConstant2(ShadowShadowOffset, 30, 20);
        gxSetShaderConstant2(ShadowShadowScale, 1.0f, 1.0f);
        gxSetShaderConstant2(ShadowLightOrigin, 384.0f, 512.0f);
    }
*/
    
    if (Chapter.PageProps.LightsOff)
    {
        gxSetShaderConstant2(ShadowShadowOffset, 0, 0);
        gxSetShaderConstant2(ShadowShadowScale, 1.25f, 1.25f);
        gxSetShaderConstant2(ShadowLightOrigin, Flashlight.X + ScrollX, Flashlight.Y + ScrollY);
    }
    else
    {
        gxSetShaderConstant2(ShadowShadowOffset, 0, 0);
        gxSetShaderConstant2(ShadowShadowScale, 1.05f, 1.05f);
        gxSetShaderConstant2(ShadowLightOrigin, 128, 128);
    }
    gxSetShaderConstant1(ShadowShadowAlpha, 128.0f/255.0f);
    
    DrawLightList(List, NULL, GXALPHA_BLEND);
}

static void RenderCombinedColor()
{
    gxSetShader(&CombineShader);
    gxSetShaderSampler(CombineColorSampler, &ColorRT);
    gxSetShaderSampler(CombineLightingSampler, &LightingRT);
	
    DrawScreen(NULL, GXALPHA_NONE);
}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                          Main Lighting Interface                                                                        //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void InitLighting()
{
    double StartTime = GetCurrentTime();
    
#ifdef PLATFORM_WINDOWS
    LitRenderTargetWidth = 640;
    LitRenderTargetHeight = 960;
	LitAspectScale = 1.0f;
	LitAspectOffset = 0.0f;
	LitScreenHeight = 1024;
    
	gxDev->CreateVertexDeclaration(LitVertexElements, &LitVertexDecl);
    
    gxCreateShader(LitVertexShaderSource, LitShaderSource, &LitShader);
        
    gxCreateShader(ShadowVertexShaderSource, ShadowShaderSource, &ShadowShader);
    ShadowShadowOffset = gxGetShaderConstantByName(&ShadowShader, "ShadowOffset");
    ShadowShadowScale = gxGetShaderConstantByName(&ShadowShader, "ShadowScale");
    ShadowLightOrigin = gxGetShaderConstantByName(&ShadowShader, "LightOrigin");
    ShadowShadowAlpha = gxGetShaderConstantByName(&ShadowShader, "ShadowAlpha");
    ShadowSampler = gxGetShaderConstantByName(&ShadowShader, "Sampler");
        
    gxCreateShader(StandardVertexShaderSource, EffectsShaderSource, &EffectsShader);
    EffectsSampler = gxGetShaderConstantByName(&EffectsShader, "Sampler");

    gxCreateShader(StandardVertexShaderSource, CombineShaderSource, &CombineShader);
    CombineColorSampler = gxGetShaderConstantByName(&CombineShader, "ColorSampler");
    CombineLightingSampler = gxGetShaderConstantByName(&CombineShader, "LightingSampler");
#endif

#ifdef PLATFORM_IPHONE_OR_MAC
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
    
    if (gxOpenGLESVersion == 1)
    {
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);        
    }
    
    // Create the vertex and index buffers.
    if (gxOpenGLESVersion == 2)
        glGenVertexArrays(LIT_VBO_COUNT, LitVertsVAO);
    glGenBuffers(LIT_VBO_COUNT, LitVertsVBO);
    glGenBuffers(LIT_VBO_COUNT, LitVertsIBO);
    
    for (int i = 0; i < LIT_VBO_COUNT; i++)
    {
        if (gxOpenGLESVersion == 2)
            glBindVertexArray(LitVertsVAO[i]);
        
        glBindBuffer(GL_ARRAY_BUFFER, LitVertsVBO[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(SLitVertex) * MAX_LIT_VERTS, NULL, GL_DYNAMIC_DRAW);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, LitVertsIBO[i]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * MAX_LIT_VERTS * 2, NULL, GL_DYNAMIC_DRAW);
        
        if (gxOpenGLESVersion == 2)
        {
            glEnableVertexAttribArray(GX_ATTRIB_VERTEX);
            glEnableVertexAttribArray(GX_ATTRIB_TEXCOORD);
            glEnableVertexAttribArray(GX_ATTRIB_COLOR);
            glVertexAttribPointer(GX_ATTRIB_VERTEX, 2, GL_FLOAT, 0, sizeof(SLitVertex), (void*)offsetof(SLitVertex, X));
            glVertexAttribPointer(GX_ATTRIB_TEXCOORD, 2, GL_FLOAT, 0, sizeof(SLitVertex), (void*)offsetof(SLitVertex, U));
            glVertexAttribPointer(GX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, 1, sizeof(SLitVertex), (void*)offsetof(SLitVertex, Color));
        }
    }
    
    BufferObjectIndex = 0;
   
    if (gxOpenGLESVersion == 2)
        glBindVertexArray(0);

#ifdef PLATFORM_IPHONE_OR_MAC
    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
        ReportError("Lit VAO setup caused an OpenGL error: 0x%x", error);
#endif

    if (gxOpenGLESVersion == 2)
    {
        glGenVertexArrays(1, &ScreenVAO);
        glBindVertexArray(ScreenVAO);
    }

    glGenBuffers(1, &ScreenVBO);
    glBindBuffer(GL_ARRAY_BUFFER, ScreenVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(SLitVertex) * 4, ScreenVerts, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    if (gxOpenGLESVersion == 2)
    {
        glEnableVertexAttribArray(GX_ATTRIB_VERTEX);
        glEnableVertexAttribArray(GX_ATTRIB_TEXCOORD);
        glEnableVertexAttribArray(GX_ATTRIB_COLOR);
        glVertexAttribPointer(GX_ATTRIB_VERTEX, 2, GL_FLOAT, 0, sizeof(SLitVertex), (void*)offsetof(SLitVertex, X));
        glVertexAttribPointer(GX_ATTRIB_TEXCOORD, 2, GL_FLOAT, 0, sizeof(SLitVertex), (void*)offsetof(SLitVertex, U));
        glVertexAttribPointer(GX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, 1, sizeof(SLitVertex), (void*)offsetof(SLitVertex, Color));
    }

    if (gxOpenGLESVersion == 2)
        glBindVertexArray(0);
    
#ifdef PLATFORM_IPHONE_OR_MAC
    error = glGetError();
    if (error != GL_NO_ERROR)
        ReportError("Screen VAO setup caused an OpenGL error: 0x%x", error);
#endif

    // Create shaders.
    if (gxOpenGLESVersion == 2)
    {
        gxCreateShader(LitVertexShaderSource, LitShaderSource, &LitShader);
        
        gxCreateShader(ShadowVertexShaderSource, ShadowShaderSource, &ShadowShader);
        ShadowShadowOffset = gxGetShaderConstantByName(&ShadowShader, "ShadowOffset");
        ShadowShadowScale = gxGetShaderConstantByName(&ShadowShader, "ShadowScale");
        ShadowLightOrigin = gxGetShaderConstantByName(&ShadowShader, "LightOrigin");
        ShadowShadowAlpha = gxGetShaderConstantByName(&ShadowShader, "ShadowAlpha");
        ShadowSampler = gxGetShaderConstantByName(&ShadowShader, "Sampler");
        
        gxCreateShader(StandardVertexShaderSource, EffectsShaderSource, &EffectsShader);
        EffectsSampler = gxGetShaderConstantByName(&EffectsShader, "Sampler");

        gxCreateShader(StandardVertexShaderSource, CombineShaderSource, &CombineShader);
        CombineColorSampler = gxGetShaderConstantByName(&CombineShader, "ColorSampler");
        CombineLightingSampler = gxGetShaderConstantByName(&CombineShader, "LightingSampler");
    }
#endif
    
#ifdef PLATFORM_IPHONE_OR_MAC
    error = glGetError();
    if (error != GL_NO_ERROR)
        ReportError("Shader setup caused an OpenGL error: 0x%x", error);
#endif
 
	// Create render targets.
#ifdef PLATFORM_IPHONE_OR_MAC
    if (gxOpenGLESVersion == 2)
#endif
    {
        gxCreateRenderTarget(LitRenderTargetWidth, LitRenderTargetHeight, &ColorRT, false);
        gxCreateRenderTarget(LitRenderTargetWidth, LitRenderTargetHeight, &LightingRT, false);

        InitShadows();
    }
    
#ifdef PLATFORM_IPHONE_OR_MAC
    error = glGetError();
    if (error != GL_NO_ERROR)
        ReportError("RT setup caused an OpenGL error: 0x%x", error);
#endif
    
    double EndTime = GetCurrentTime();
    LogMessage("Lighting setup took %.1f seconds.\n", EndTime-StartTime);
}

void ResetLighting()
{
	NLitVerts = 0;

    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
    //                                                   Begin writing buffers                                                                 //
    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
#ifdef PLATFORM_IPHONE_OR_MAC
    NLitVertIndices = 0;
    
    BufferObjectIndex = (BufferObjectIndex+1) % LIT_VBO_COUNT;
    
    if (gxOpenGLESVersion == 2)
        glBindVertexArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, LitVertsVBO[BufferObjectIndex]);
    LitVerts = (SLitVertex*)glMapBufferOES(GL_ARRAY_BUFFER, GL_WRITE_ONLY_OES);
    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
        ReportError("Mapping lit verts caused an OpenGL error: 0x%x", error);
    assert(LitVerts);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, LitVertsIBO[BufferObjectIndex]);
    LitVertIndices = (GLushort*)glMapBufferOES(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY_OES);
    error = glGetError();
    if (error != GL_NO_ERROR)
        ReportError("Mapping lit vert indices caused an OpenGL error: 0x%x", error);
    assert(LitVertIndices);
    
    CurrentVAO = -1;
#endif
    
    CurrentBlendMode = -1;
    CurrentShader = NULL;
    
	for (int i = 0; i < LIGHTLIST_COUNT; i++)
	{
		LightLists[i].NQuads = 0;
	}
    
	if (Chapter.PageProps.LightsOff)
		LightState.AmbientColor = gxRGBA32(16, 16, 16, 255);
	else
		LightState.AmbientColor = gxRGBA32(128, 128, 128, 255);

    //AddLitSpriteSizedColor(LIGHTLIST_LIGHTING, &WhiteSprite, 0, 0, 768, LitScreenHeight, LightState.AmbientColor);
    //AddLitSpriteSizedColor(LIGHTLIST_LIGHTING, &LightingSprite, 0, 0, 768, LitScreenHeight, LightState.AmbientColor);
}

void RenderLighting()
{
    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
    //                                                   Finish writing buffers                                                                //
    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
#ifdef PLATFORM_IPHONE_OR_MAC
    if (gxOpenGLESVersion == 2)
        glBindVertexArray(0);
    
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
#ifdef PLATFORM_IPHONE_OR_MAC
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
        
        DrawLightList(LIGHTLIST_BACKGROUND, NULL, GXALPHA_NONE);
        //DrawLightListShadow(LIGHTLIST_FOREGROUND, NULL, GXALPHA_BLEND);        
        DrawLightList(LIGHTLIST_DUST, NULL, GXALPHA_BLEND);
        DrawLightList(LIGHTLIST_FOREGROUND, NULL, GXALPHA_BLEND);
        DrawLightList(LIGHTLIST_FOREGROUND_NO_SHADOW, NULL, GXALPHA_BLEND);        
        //DrawLightListShadow(LIGHTLIST_VACUUM, NULL, GXALPHA_BLEND);        
        DrawLightList(LIGHTLIST_SMOKE, NULL, GXALPHA_BLEND);
        DrawLightList(LIGHTLIST_VACUUM, NULL, GXALPHA_BLEND);
        DrawLightList(LIGHTLIST_EFFECTS, NULL, GXALPHA_ADD);
        DrawLightList(LIGHTLIST_WIPE, NULL, GXALPHA_BLEND);
    }
    else
#endif
    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
    //                                                         OpenGL ES 2.0 Renderer                                                          //
    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
    {
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
        //                                                   Build lighting                                                                        //
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
        gxSetRenderTarget(&LightingRT);
        gxClearColor(LightState.AmbientColor);
        
        DrawLightList(LIGHTLIST_LIGHTING, &LitShader, GXALPHA_ADD);
        
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
        DrawLightList(LIGHTLIST_BACKGROUND, &LitShader, GXALPHA_NONE);
        
        // Foreground shadows.
        DrawShadows(LIGHTLIST_FOREGROUND, &ColorRT);
        
        // Dust layer.        
        DrawLightList(LIGHTLIST_DUST, &LitShader, GXALPHA_BLEND);
        DrawLightList(LIGHTLIST_FOREGROUND, &LitShader, GXALPHA_BLEND);
        DrawLightList(LIGHTLIST_FOREGROUND_NO_SHADOW, &LitShader, GXALPHA_BLEND);
        
        // Vacuum shadows.
        DrawShadows(LIGHTLIST_VACUUM, &ColorRT);
        
        // Smoke.
        DrawLightList(LIGHTLIST_SMOKE, &LitShader, GXALPHA_BLEND);

        // Vacuum.
        DrawLightList(LIGHTLIST_VACUUM, &LitShader, GXALPHA_BLEND);
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
        //                                                   Combine layers into frame                                                             //
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
        
        // Combine everything into the final output.
        gxSetRenderTarget(NULL);
#ifdef PLATFORM_MAC
        int displayWidth = 576;
        int displayHeight = 768;
        NSRect mainDisplayRect = [[NSScreen mainScreen] frame];
        glViewport(mainDisplayRect.size.width/2 - displayWidth/2, mainDisplayRect.size.height/2 - displayHeight/2, displayWidth, displayHeight);
#endif
        RenderCombinedColor();
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
        //                                                   Draw effects on top                                                                   //
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//       
        DrawLightList(LIGHTLIST_EFFECTS, &EffectsShader, GXALPHA_ADD);
        
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
        //                                                   Draw screen wipes                                                                     //
        // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
        // Screen wipe.
        DrawLightList(LIGHTLIST_WIPE, &LitShader, GXALPHA_BLEND);
    }
}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                          Quad Addition Functions                                                                        //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

static SLitVertex* AllocateLitVerts(int NVerts)
{
	if (NLitVerts + NVerts > MAX_LIT_VERTS)
		ReportError("Exceeded the maximum of %d lit vertices.", MAX_LIT_VERTS);

	SLitVertex* Verts = &LitVerts[NLitVerts];
	
	NLitVerts += NVerts;

	return Verts;
}

#ifdef PLATFORM_IPHONE_OR_MAC
static void AddLitQuadStripIndices(SLitQuad* Quad, int NVerts)
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

#ifdef PLATFORM_IPHONE_OR_MAC
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

#ifdef PLATFORM_IPHONE_OR_MAC
    AddLitQuadStripIndices(Quad, NVerts);
#endif    

	Quad->NVerts = NVerts;
	Quad->Verts = AllocateLitVerts(NVerts);

	return Quad->Verts;
}

void AddLitSprite(ELightList List, gxSprite* Sprite, float X, float Y)
{
    float LeftU = (float)Sprite->left / Sprite->width;
    float RightU = (float)Sprite->right / Sprite->width;
    float TopV = (float)Sprite->top / Sprite->height;
    float BottomV = (float)Sprite->bottom / Sprite->height;
    
	AddLitQuad(List, Sprite, gxRGBA32(255,255,255,255),
		X+Sprite->left,  Y+Sprite->top,    LeftU,  TopV, 
		X+Sprite->right, Y+Sprite->top,    RightU, TopV,
		X+Sprite->right, Y+Sprite->bottom, RightU, BottomV, 
		X+Sprite->left,  Y+Sprite->bottom, LeftU,  BottomV);
}

void AddLitSpriteScaled(ELightList List, gxSprite* Sprite, float X, float Y, float ScaleX, float ScaleY)
{
	AddLitQuad(List, Sprite, gxRGBA32(255,255,255,255),
		X,                      Y,                       0.0f, 0.0f, 
		X+Sprite->width*ScaleX, Y,                       1.0f, 0.0f,
		X+Sprite->width*ScaleX, Y+Sprite->height*ScaleY, 1.0f, 1.0f, 
		X,                      Y+Sprite->height*ScaleY, 0.0f, 1.0f);
}

void AddLitSpriteAlpha(ELightList List, gxSprite* Sprite, float X, float Y, float Alpha)
{
	AddLitQuad(List, Sprite, gxRGBA32(255,255,255,(int)(255*Alpha)),
               X,               Y,                0.0f, 0.0f, 
               X+Sprite->width, Y,                1.0f, 0.0f,
               X+Sprite->width, Y+Sprite->height, 1.0f, 1.0f, 
               X,               Y+Sprite->height, 0.0f, 1.0f);
}

void AddLitSpriteColor(ELightList List, gxSprite* Sprite, float X, float Y, unsigned int Color)
{
	AddLitQuad(List, Sprite, Color,
               X,               Y,                0.0f, 0.0f, 
               X+Sprite->width, Y,                1.0f, 0.0f,
               X+Sprite->width, Y+Sprite->height, 1.0f, 1.0f, 
               X,               Y+Sprite->height, 0.0f, 1.0f);
}

void AddLitSpriteScaledAlpha(ELightList List, gxSprite* Sprite, float X, float Y, float ScaleX, float ScaleY, float Alpha)
{
	AddLitQuad(List, Sprite, gxRGBA32(255,255,255,(int)(255*Alpha)),
               X,                      Y,                       0.0f, 0.0f, 
               X+Sprite->width*ScaleX, Y,                       1.0f, 0.0f,
               X+Sprite->width*ScaleX, Y+Sprite->height*ScaleY, 1.0f, 1.0f, 
               X,                      Y+Sprite->height*ScaleY, 0.0f, 1.0f);
}

void AddLitSpriteScaledColor(ELightList List, gxSprite* Sprite, float X, float Y, float ScaleX, float ScaleY, unsigned int Color)
{
	AddLitQuad(List, Sprite, Color,
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

void AddLitSpriteSizedAlpha(ELightList List, gxSprite* Sprite, float X, float Y, float SizeX, float SizeY, float Alpha)
{
	AddLitQuad(List, Sprite, gxRGBA32(255,255,255,(int)(255*Alpha)),
               X,                      Y,                       0.0f, 0.0f, 
               X+SizeX,                Y,                       1.0f, 0.0f,
               X+SizeX,                Y+SizeY,                 1.0f, 1.0f, 
               X,                      Y+SizeY,                 0.0f, 1.0f);
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

void AddLitSubSpriteAlpha(ELightList List, gxSprite* Sprite, float X, float Y, float SubX1, float SubY1, float SubX2, float SubY2, float Alpha)
{
	AddLitQuad(List, Sprite, gxRGBA32(255,255,255,(int)(255*Alpha)),
               X,               Y,               SubX1/Sprite->width, SubY1/Sprite->height, 
               X+(SubX2-SubX1), Y,               SubX2/Sprite->width, SubY1/Sprite->height,
               X+(SubX2-SubX1), Y+(SubY2-SubY1), SubX2/Sprite->width, SubY2/Sprite->height, 
               X,               Y+(SubY2-SubY1), SubX1/Sprite->width, SubY2/Sprite->height);
}

void AddLitSubSpriteSized(ELightList List, gxSprite* Sprite, float X, float Y, float SubX1, float SubY1, float SubX2, float SubY2, float SizeX, float SizeY)
{
	AddLitQuad(List, Sprite, gxRGBA32(255,255,255,255),
		X,       Y,       SubX1/Sprite->width, SubY1/Sprite->height, 
		X+SizeX, Y,       SubX2/Sprite->width, SubY1/Sprite->height,
		X+SizeX, Y+SizeY, SubX2/Sprite->width, SubY2/Sprite->height, 
		X,       Y+SizeY, SubX1/Sprite->width, SubY2/Sprite->height);
}

void AddLitSubSpriteScaled(ELightList List, gxSprite* Sprite, float X, float Y, float SubX1, float SubY1, float SubX2, float SubY2, float ScaleX, float ScaleY)
{
	AddLitQuad(List, Sprite, gxRGBA32(255,255,255,255),
               X,                      Y,                      SubX1/Sprite->width, SubY1/Sprite->height, 
               X+(SubX2-SubX1)*ScaleX, Y,                      SubX2/Sprite->width, SubY1/Sprite->height,
               X+(SubX2-SubX1)*ScaleX, Y+(SubY2-SubY1)*ScaleY, SubX2/Sprite->width, SubY2/Sprite->height, 
               X,                      Y+(SubY2-SubY1)*ScaleY, SubX1/Sprite->width, SubY2/Sprite->height);
}

void AddLitSubSpriteScaledAlpha(ELightList List, gxSprite* Sprite, float X, float Y, float SubX1, float SubY1, float SubX2, float SubY2, float ScaleX, float ScaleY, float Alpha)
{
	AddLitQuad(List, Sprite, gxRGBA32(255,255,255,(int)(255*Alpha)),
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

void AddLitSubSpriteOriginScaledRotatedAlpha(ELightList List, gxSprite* Sprite, float X, float Y, float XOrigin, float YOrigin, float SubX1, float SubY1, float SubX2, float SubY2, float Scale, float Angle, float Alpha)
{
	float ca = cosf(Angle);
	float sa = sinf(Angle);
    
	float w0 = Scale * XOrigin;
	float h0 = Scale * YOrigin;
	float w1 = Scale * ( (SubX2-SubX1) - XOrigin );
	float h1 = Scale * ( (SubY2-SubY1) - YOrigin );
    
	AddLitQuad(List, Sprite, gxRGBA32(255,255,255,(int)(255*Alpha)),
               X + (-w0 * ca) - (-h0 * sa),    Y + (-w0 * sa) + (-h0 * ca), SubX1/Sprite->width, SubY1/Sprite->height, 
               X + (+w1 * ca) - (-h0 * sa),    Y + (+w1 * sa) + (-h0 * ca), SubX2/Sprite->width, SubY1/Sprite->height, 
               X + (+w1 * ca) - (+h1 * sa),    Y + (+w1 * sa) + (+h1 * ca), SubX2/Sprite->width, SubY2/Sprite->height, 
               X + (-w0 * ca) - (+h1 * sa),    Y + (-w0 * sa) + (+h1 * ca), SubX1/Sprite->width, SubY2/Sprite->height);
}

void AddLitSpriteUV(ELightList List, gxSprite* Sprite, float X, float Y, float SizeX, float SizeY, float U0, float V0, float U1, float V1, float U2, float V2, float U3, float V3)
{
	AddLitQuad(List, Sprite, gxRGBA32(255,255,255,255),
               X,       Y,       U0, V0, 
               X+SizeX, Y,       U1, V1,
               X+SizeX, Y+SizeY, U2, V2, 
               X,       Y+SizeY, U3, V3);
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

	float w = Scale * (float)Sprite->width * 0.5f;
	float h = Scale * (float)Sprite->height * 0.5f;

	AddLitQuad(List, Sprite, gxRGBA32(255,255,255,(int)(255*Alpha)),
		X + (-w * ca) - (-h * sa),    Y + (-w * sa) + (-h * ca),    0.0f, 0.0f, 
		X + (+w * ca) - (-h * sa),    Y + (+w * sa) + (-h * ca),    1.0f, 0.0f, 
		X + (+w * ca) - (+h * sa),    Y + (+w * sa) + (+h * ca),    1.0f, 1.0f, 
		X + (-w * ca) - (+h * sa),    Y + (-w * sa) + (+h * ca),    0.0f, 1.0f);
}

void AddLitSpriteOriginScaledRotatedAlpha(ELightList List, gxSprite* Sprite, float X, float Y, float XOrigin, float YOrigin, float Scale, float Angle, float Alpha)
{
	float ca = cosf(Angle);
	float sa = sinf(Angle);
    
	float w0 = Scale * XOrigin;
	float h0 = Scale * YOrigin;
	float w1 = Scale * ( Sprite->width - XOrigin );
	float h1 = Scale * ( Sprite->height - YOrigin );
    
	AddLitQuad(List, Sprite, gxRGBA32(255,255,255,(int)(255*Alpha)),
               X + (-w0 * ca) - (-h0 * sa),    Y + (-w0 * sa) + (-h0 * ca),    0.0f, 0.0f, 
               X + (+w1 * ca) - (-h0 * sa),    Y + (+w1 * sa) + (-h0 * ca),    1.0f, 0.0f, 
               X + (+w1 * ca) - (+h1 * sa),    Y + (+w1 * sa) + (+h1 * ca),    1.0f, 1.0f, 
               X + (-w0 * ca) - (+h1 * sa),    Y + (-w0 * sa) + (+h1 * ca),    0.0f, 1.0f);
    
}

