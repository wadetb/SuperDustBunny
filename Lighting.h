//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//
#ifndef __LIGHTING_H__
#define __LIGHTING_H__

struct SLitVertex
{
	float X, Y;
	float U, V;
	unsigned int Color;
};

enum ELightList
{
	LIGHTLIST_BACKGROUND,
	LIGHTLIST_DUST,
	LIGHTLIST_FOREGROUND_NO_SHADOW,
	LIGHTLIST_FOREGROUND,
	LIGHTLIST_EFFECTS,
	LIGHTLIST_VACUUM,
	LIGHTLIST_LIGHTING,
	LIGHTLIST_COUNT
};

struct SLitQuad
{
	ELightList List;

	gxSprite* Sprite;

	int NVerts;
	SLitVertex* Verts;
};

void AddLitQuad(
	ELightList List, gxSprite* Sprite, unsigned int Color,
	float X0, float Y0, float U0, float V0, 
	float X1, float Y1, float U1, float V1, 
	float X2, float Y2, float U2, float V2, 
	float X3, float Y3, float U3, float V3);

SLitVertex* AllocateLitVerts(int NVerts);

void AddLitSprite(ELightList List, gxSprite* Sprite, float X, float Y);
void AddLitSpriteScaled(ELightList List, gxSprite* Sprite, float X, float Y, float ScaleX, float ScaleY);

void AddLitSpriteCenteredScaledAlpha(ELightList List, gxSprite* Sprite, float X, float Y, float Scale, float Alpha);
void AddLitSpriteCenteredScaledColor(ELightList List, gxSprite* Sprite, float X, float Y, float Scale, unsigned int Color);

void AddLitSubSprite(ELightList List, gxSprite* Sprite, float X, float Y, float SubX1, float SubY1, float SubX2, float SubY2);

void AddLitSpriteCenteredScaledRotated(ELightList List, gxSprite* Sprite, float X, float Y, float Scale, float Angle);
void AddLitSpriteCenteredScaledRotatedAdditive(ELightList List, gxSprite* Sprite, float X, float Y, float Scale, float Angle, float Alpha);

void InitLighting();

void ResetLighting();
void RenderLighting();

#endif
