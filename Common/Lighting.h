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
	LIGHTLIST_MIDGROUND,
	LIGHTLIST_DUST,
	LIGHTLIST_FOREGROUND_NO_SHADOW,
	LIGHTLIST_FOREGROUND,
	LIGHTLIST_FOREGROUND_GRAYSCALE,
	LIGHTLIST_SMOKE,
	LIGHTLIST_EFFECTS,
	LIGHTLIST_VACUUM,
	LIGHTLIST_LIGHTING,
	LIGHTLIST_WIPE,
	LIGHTLIST_COUNT
};

struct SLitQuad
{
	ELightList List;

	gxSprite* Sprite;

	int NVerts;
	SLitVertex* Verts;
    
#ifdef PLATFORM_IPHONE_OR_MAC
    GLushort BaseIndex;
    GLushort IndexCount;
#endif
};


struct SLightState
{
    bool LightsOff;
    bool ForegroundShadows;
	unsigned int AmbientColor;
};

extern SLightState LightState;

enum EScreenAspect
{
    ASPECT_IPAD,
    ASPECT_IPHONE,
    ASPECT_IPHONE_5,
};

extern EScreenAspect ScreenAspect;

extern float LitScreenHeight;
extern float LitSceneZoom;
extern float LitSceneOffsetX;
extern float LitSceneOffsetY;

void AddLitQuad(
	ELightList List, gxSprite* Sprite, unsigned int Color,
	float X0, float Y0, float U0, float V0, 
	float X1, float Y1, float U1, float V1, 
	float X2, float Y2, float U2, float V2, 
	float X3, float Y3, float U3, float V3);

SLitVertex* AddLitQuad(ELightList List, gxSprite* Sprite, int NVerts);

void AddLitSprite(ELightList List, gxSprite* Sprite, float X, float Y);
void AddLitSpriteScaled(ELightList List, gxSprite* Sprite, float X, float Y, float ScaleX, float ScaleY);
void AddLitSpriteAlpha(ELightList List, gxSprite* Sprite, float X, float Y, float Alpha);
void AddLitSpriteColor(ELightList List, gxSprite* Sprite, float X, float Y, unsigned int Color);
void AddLitSpriteScaledAlpha(ELightList List, gxSprite* Sprite, float X, float Y, float ScaleX, float ScaleY, float Alpha);
void AddLitSpriteScaledColor(ELightList List, gxSprite* Sprite, float X, float Y, float ScaleX, float ScaleY, unsigned int Color);

void AddLitSpriteSized(ELightList List, gxSprite* Sprite, float X, float Y, float SizeX, float SizeY);
void AddLitSpriteSizedAlpha(ELightList List, gxSprite* Sprite, float X, float Y, float SizeX, float SizeY, float Alpha);
void AddLitSpriteSizedColor(ELightList List, gxSprite* Sprite, float X, float Y, float SizeX, float SizeY, unsigned int Color);

void AddLitSpriteCenteredScaledAlpha(ELightList List, gxSprite* Sprite, float X, float Y, float Scale, float Alpha);
void AddLitSpriteCenteredScaledColor(ELightList List, gxSprite* Sprite, float X, float Y, float Scale, unsigned int Color);

void AddLitSubSprite(ELightList List, gxSprite* Sprite, float X, float Y, float SubX1, float SubY1, float SubX2, float SubY2);
void AddLitSubSpriteAlpha(ELightList List, gxSprite* Sprite, float X, float Y, float SubX1, float SubY1, float SubX2, float SubY2, float Alpha);
void AddLitSubSpriteSized(ELightList List, gxSprite* Sprite, float X, float Y, float SubX1, float SubY1, float SubX2, float SubY2, float SizeX, float SizeY);
void AddLitSubSpriteScaled(ELightList List, gxSprite* Sprite, float X, float Y, float SubX1, float SubY1, float SubX2, float SubY2, float ScaleX, float ScaleY);
void AddLitSubSpriteScaledAlpha(ELightList List, gxSprite* Sprite, float X, float Y, float SubX1, float SubY1, float SubX2, float SubY2, float ScaleX, float ScaleY, float Alpha);
void AddLitSubSpriteScaledColor(ELightList List, gxSprite* Sprite, float X, float Y, float SubX1, float SubY1, float SubX2, float SubY2, float ScaleX, float ScaleY, unsigned int Color);
void AddLitSubSpriteCenteredScaledRotated(ELightList List, gxSprite* Sprite, float X, float Y, float SubX1, float SubY1, float SubX2, float SubY2, float Scale, float Angle);
void AddLitSubSpriteOriginScaledRotatedAlpha(ELightList List, gxSprite* Sprite, float X, float Y, float XOrigin, float YOrigin, float SubX1, float SubY1, float SubX2, float SubY2, float Scale, float Angle, float Alpha);

void AddLitSpriteUV(ELightList List, gxSprite* Sprite, float X, float Y, float SizeX, float SizeY, float U0, float V0, float U1, float V1, float U2, float V2, float U3, float V3);

void AddLitSpriteCenteredScaledRotated(ELightList List, gxSprite* Sprite, float X, float Y, float Scale, float Angle);
void AddLitSpriteCenteredScaled2Rotated(ELightList List, gxSprite* Sprite, float X, float Y, float ScaleX, float ScaleY, float Angle);
void AddLitSpriteCenteredScaledRotatedAlpha(ELightList List, gxSprite* Sprite, float X, float Y, float Scale, float Angle, float Alpha);
void AddLitSpriteCenteredScaledRotatedColor(ELightList List, gxSprite* Sprite, float X, float Y, float Scale, float Angle, unsigned int Color);
void AddLitSpriteCenteredScaledRotatedAdditive(ELightList List, gxSprite* Sprite, float X, float Y, float Scale, float Angle, float Alpha);

void AddLitSpriteXCenteredScaledRotated(ELightList List, gxSprite* Sprite, float X, float Y, float Scale, float Angle);
void AddLitSpriteXCenteredScaledRotatedAlpha(ELightList List, gxSprite* Sprite, float X, float Y, float Scale, float Angle, float Alpha);

void AddLitSpriteOriginScaledRotatedAlpha(ELightList List, gxSprite* Sprite, float X, float Y, float XOrigin, float YOrigin, float Scale, float Angle, float Alpha);

void InitLighting();

void ResetLighting();
void RenderLighting();

void ResetLightState();

#endif
