//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                             //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#include "Common.h"
#include "Barrel.h"
#include "Dusty.h"

#define MAX_BARRELS 100

int NBarrels = 0;
SBarrel Barrels[MAX_BARRELS];

void CreateBarrel(int X, int Y, const char* Desc)
{
	SBarrel* Barrel = &Barrels[NBarrels++];

	Barrel->X = X + 32;
	Barrel->Y = Y + 32;

	sscanf(Desc, "barrel from=%d to=%d", &Barrel->FromDir, &Barrel->ToDir);//

	Barrel->State = BARRELSTATE_WAIT;
	Barrel->Dir = Barrel->FromDir;
}

void ClearBarrels()
{
	NBarrels = 0;
}

void DisplayBarrels_BeforeDusty()
{
	for (int i = 0; i < NBarrels; i++)
	{
		SBarrel* Barrel = &Barrels[i];
		
		gxDrawSpriteCenteredRotated(Barrel->X, Barrel->Y + ScrollY, Barrel->Dir * 3.14159f / 180.0f, &BarrelBackSprite);
	}
}

void DisplayBarrels_AfterDusty()
{
	for (int i = 0; i < NBarrels; i++)
	{
		SBarrel* Barrel = &Barrels[i];
		
		gxDrawSpriteCenteredRotated(Barrel->X, Barrel->Y + ScrollY, Barrel->Dir * 3.14159f / 180.0f, &BarrelFrontSprite);
	}
}

float GetDirDifference(float a, float b) 
{
	float mn = a - b;
	while (mn < -180) mn += 360;
	while (mn > 180) mn -= 360;
	return mn;
}

void UpdateBarrels()
{
	for (int i = 0; i < NBarrels; i++)
	{
		SBarrel* Barrel = &Barrels[i];

		if (Barrel->State == BARRELSTATE_WAIT)
		{
			float Dist = Distance(Dusty.FloatX, Dusty.FloatY-50, Barrel->X, Barrel->Y);

			if (Dist < 200)
			{
				float NewDir = AngleBetween(Barrel->X, Barrel->Y, Dusty.FloatX, Dusty.FloatY+50) * 180 / PI;
				Barrel->Dir += GetDirDifference(NewDir, Barrel->Dir) * 0.3f;
				
//				if (Dist < 100)
//				{
//					Dusty.FloatX = Dusty.FloatX * 0.9f + Barrel->X * 0.1f;
//					Dusty.FloatY = Dusty.FloatY * 0.9f + (Barrel->Y+50) * 0.1f;
//					SetDustyState_PrepareLaunch();
//				}
			}
			else 
			{
				Barrel->Dir += GetDirDifference(Barrel->FromDir, Barrel->Dir) * 0.2f;
			}
			
			if (Dist < 100)
			{
			    if (Tutorial.BarrelDisplayed == false)
			    {
			        SetGameState_Crumb(TUTORIALSTATE_BARREL);
			        return;
			    }
			    			    				
				Dusty.FloatX = (float)Barrel->X;
				Dusty.FloatY = (float)Barrel->Y + 50;
				SetDustyState_PrepareLaunch();
				
				Barrel->State = BARRELSTATE_TURN;
			}
		}
		else if (Barrel->State == BARRELSTATE_TURN)
		{
			Dusty.FloatX = (float)Barrel->X;
			Dusty.FloatY = (float)Barrel->Y + 50;

			int Diff = GetDirDifference(Barrel->Dir, Barrel->ToDir);
			if (Diff > 5 || Diff < -5)
			{
				if (Diff < 0)
					Barrel->Dir = fmodf(Barrel->Dir+5, 360);
				else
					Barrel->Dir = fmodf(Barrel->Dir+355, 360);
			}
			else
			{
				Barrel->Dir = Barrel->ToDir;
				float Angle = (float)((90 - Barrel->Dir)) * 3.14159f/180.0f;
				float Velocity = 20.0f;
				SetDustyState_Launch(Velocity*cosf(Angle), -Velocity*sinf(Angle));
				Barrel->Timer = 30;
				Barrel->State = BARRELSTATE_LAUNCH;
			}
		}
		else if (Barrel->State == BARRELSTATE_LAUNCH)
		{
			Barrel->Timer--;
			if (Barrel->Timer == 0)
			{
				Barrel->State = BARRELSTATE_RESET;
			}
		}
		else if (Barrel->State == BARRELSTATE_RESET)
		{
			int Diff = GetDirDifference(Barrel->Dir, Barrel->FromDir);
			if (Diff > 5 || Diff < -5)
			{
				if (Diff < 0)
					Barrel->Dir = fmodf(Barrel->Dir+5, 360);
				else
					Barrel->Dir = fmodf(Barrel->Dir+355, 360);
			}
			else
			{
				Barrel->Dir = Barrel->FromDir;
				Barrel->State = BARRELSTATE_WAIT;
			}
		}
	}
}
