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


void ResetLighting()
{
	for (int i = 0; i < LIGHTLIST_COUNT; i++)
	{
		LightLists[i].NQuads = 0;
	}
}

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

void RenderLighting()
{
	_gxSetAlpha( GXALPHA_BLEND );
	for (int i = 0; i < LightLists[LIGHTLIST_BACKGROUND].NQuads; i++)
		DrawLitQuad( &LightLists[LIGHTLIST_BACKGROUND].Quads[i] );

	LightState.ShadowAlpha = 128;
	LightState.ShadowOffsetX = 30;
	LightState.ShadowOffsetY = 20;

	_gxSetAlpha( GXALPHA_BLEND );
	for (int i = 0; i < LightLists[LIGHTLIST_FOREGROUND].NQuads; i++)
		DrawLitQuad_Shadow( &LightLists[LIGHTLIST_FOREGROUND].Quads[i] );

	_gxSetAlpha( GXALPHA_ADD );
	for (int i = 0; i < LightLists[LIGHTLIST_DUST].NQuads; i++)
		DrawLitQuad( &LightLists[LIGHTLIST_DUST].Quads[i] );

	_gxSetAlpha( GXALPHA_BLEND );
	for (int i = 0; i < LightLists[LIGHTLIST_FOREGROUND].NQuads; i++)
		DrawLitQuad( &LightLists[LIGHTLIST_FOREGROUND].Quads[i] );
	for (int i = 0; i < LightLists[LIGHTLIST_FOREGROUND_NO_SHADOW].NQuads; i++)
		DrawLitQuad( &LightLists[LIGHTLIST_FOREGROUND_NO_SHADOW].Quads[i] );

	_gxSetAlpha( GXALPHA_ADD );
	for (int i = 0; i < LightLists[LIGHTLIST_EFFECTS].NQuads; i++)
		DrawLitQuad( &LightLists[LIGHTLIST_EFFECTS].Quads[i] );

	_gxSetAlpha( GXALPHA_BLEND );
	for (int i = 0; i < LightLists[LIGHTLIST_VACUUM].NQuads; i++)
		DrawLitQuad_Shadow( &LightLists[LIGHTLIST_VACUUM].Quads[i] );

	_gxSetAlpha( GXALPHA_BLEND );
	for (int i = 0; i < LightLists[LIGHTLIST_VACUUM].NQuads; i++)
		DrawLitQuad( &LightLists[LIGHTLIST_VACUUM].Quads[i] );
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
