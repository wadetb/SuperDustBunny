//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                              //
//                                                                                                                                         //
//-----------------------------------------------------------------------------------------------------------------------------------------//

#include "Common.h"
#include "Dusty.h"
#include "Chapter.h"

SDusty Dusty;

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                  Dusty initialization function                                                          //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void InitDusty()
{
	Dusty.State = DUSTYSTATE_STAND;

	Dusty.FloatX = 350;
    Dusty.FloatY = -1024;

	Dusty.Direction = DIRECTION_RIGHT;

	Dusty.Left = -32;
	Dusty.Right = 32;
	Dusty.Top = -120;
	Dusty.Bottom = 0;

	Dusty.FloatVelocityX = 0.0f;
	Dusty.FloatVelocityY = 0.0f;
	
	Dusty.FloatGravity = 0.5f;

	Dusty.SpriteTransition = 0;

	Dusty.WallStickTimer = 0;
	Dusty.LastWall = DIRECTION_NONE;
	
	Dusty.HasGumExpired = false;
	Dusty.GumTimer = 30;
	
	Dusty.HasGumBlockExpired = false;
	Dusty.GumJumpAttempt = 0;
	
	Dusty.CrumbTimer = 400;
	
	Dusty.HasCrumbExpired = false;

	Dusty.CollideWithLeftSide = false;
	Dusty.CollideWithRightSide = false;
	Dusty.CollideWithTopSide = false;
	Dusty.CollideWithBottomSide = false;
	
};


// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                  State switching function declarations                                                  //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void SetDustyState_Stand();
void SetDustyState_Jump( bool OffWall );
void SetDustyState_Fall();
void SetDustyState_Hop( EDirection Direction );
void SetDustyState_WallJump();
void SetDustyState_PrepareLaunch();
void SetDustyState_Launch();

void UpdateDusty_JumpCommon();


// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                  DUSTYSTATE_STAND Implementation                                                        //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void SetDustyState_Stand()
{
	Dusty.LastWall = DIRECTION_NONE;

	Dusty.FloatVelocityX = 0;
	Dusty.FloatVelocityY = 0;

	Dusty.SpriteTransition = 0;

	Dusty.State = DUSTYSTATE_STAND;
}

void DisplayDusty_Stand()
{
	float ScaleX, OffsetX;
	if (Dusty.Direction == DIRECTION_RIGHT)
	{
		ScaleX = 1.0f;
		OffsetX = 0.0f;
	}
	else
	{
		ScaleX = -1.0f;
		OffsetX = 256.0f;
	}

	if (Dusty.SpriteTransition <= 4)
		gxDrawSpriteScaled( (int)(Dusty.FloatX + OffsetX - 119 + 18*ScaleX), (int)(Dusty.FloatY - 218 + ScrollY), ScaleX, 1.0f, &DustyHop5Sprite );
	else
		gxDrawSpriteScaled( (int)(Dusty.FloatX + OffsetX - 124 + 5*ScaleX), (int)(Dusty.FloatY - 221 + ScrollY), ScaleX, 1.0f, &DustyHop1Sprite );
}

void UpdateDusty_Stand()
{		
	if ( GetInput_Jump() )      
	{
	    if (Tutorial.JumpDisplayed == false)
	    {
	        SetGameState_Crumb(TUTORIALSTATE_JUMP);
	        return;
	    }
        
		SetDustyState_Jump( false );
		return;
	}

	if ( GetInput_MoveRight() )
	{
		SetDustyState_Hop(DIRECTION_RIGHT);
		return;
	}

	if ( GetInput_MoveLeft() )
	{
		SetDustyState_Hop(DIRECTION_LEFT);
		return;
	}

	if (Dusty.CollideWithBottomSide == false)
	{
		SetDustyState_Fall();
		return;
	}

	Dusty.SpriteTransition += 1;
}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                  DUSTYSTATE_JUMP Implementation                                                         //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void SetDustyState_Jump( bool OffWall )
{
	if ( OffWall )
	{
		sxPlaySound( &DustyWallJumpSound );
	}
	else
	{
		//sxPlaySound( &DustyJumps );
	}

	Dusty.FloatVelocityX = 6.0f;
	Dusty.FloatVelocityY = -16.0f;

	Dusty.FloatY -= 40.0f;

	if (Dusty.Direction == DIRECTION_LEFT)
		Dusty.FloatVelocityX = -Dusty.FloatVelocityX;

	Dusty.State = DUSTYSTATE_JUMP;
}

void DisplayDusty_Jump()
{
	float ScaleX, OffsetX;
	if (Dusty.Direction == DIRECTION_RIGHT)
	{
		ScaleX = 1.0f;
		OffsetX = 0.0f;
	}
	else
	{
		ScaleX = -1.0f;
		OffsetX = 256.0f;
	}

	if (Dusty.FloatVelocityY <= -5)
		gxDrawSpriteScaled( (int)(Dusty.FloatX + OffsetX - 124 - 18*ScaleX), (int)(Dusty.FloatY - 150 + ScrollY), ScaleX, 1.0f, &DustyHop2Sprite );
	else
		gxDrawSpriteScaled( (int)(Dusty.FloatX + OffsetX - 124 - 18*ScaleX), (int)(Dusty.FloatY - 140 + ScrollY), ScaleX, 1.0f, &DustyHop3Sprite );
}

void UpdateDusty_Jump()
{       
	Dusty.FloatX = Dusty.FloatX + Dusty.FloatVelocityX;                                                   
    Dusty.FloatY += Dusty.FloatVelocityY;
      
	Dusty.FloatVelocityY += Dusty.FloatGravity;
   
	if (Dusty.CollideWithTopSide == true)
	{
		SetDustyState_Fall();
		return;
	}

	if (Dusty.FloatVelocityY >= 0)
	{
		SetDustyState_Fall();
		return;
	}

	UpdateDusty_JumpCommon();
}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                  DUSTYSTATE_FALL Implementation                                                         //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void SetDustyState_Fall()
{
	Dusty.FloatVelocityY = 0.0f;

	Dusty.State = DUSTYSTATE_FALL;
}

void DisplayDusty_Fall()
{
	float ScaleX, OffsetX;
	if (Dusty.Direction == DIRECTION_RIGHT)
	{
		ScaleX = 1.0f;
		OffsetX = 0.0f;
	}
	else
	{
		ScaleX = -1.0f;
		OffsetX = 256.0f;
	}

	if (Dusty.FloatVelocityY >= 5)
		gxDrawSpriteScaled( (int)(Dusty.FloatX + OffsetX - 124 - 18*ScaleX), (int)(Dusty.FloatY - 150 + ScrollY), ScaleX, 1.0f, &DustyHop4Sprite );
	else
		gxDrawSpriteScaled( (int)(Dusty.FloatX + OffsetX - 124 - 18*ScaleX), (int)(Dusty.FloatY - 140 + ScrollY), ScaleX, 1.0f, &DustyHop3Sprite );
}

void UpdateDusty_Fall()
{
	Dusty.FloatX += Dusty.FloatVelocityX;
	Dusty.FloatY += Dusty.FloatVelocityY;

	Dusty.FloatVelocityY += Dusty.FloatGravity;

	if (Dusty.FloatVelocityY > 15.0f)
		Dusty.FloatVelocityY = 15.0f;

	if (Dusty.CollideWithBottomSide == true )
	{	
		SetDustyState_Stand();
		return;
	} 

	UpdateDusty_JumpCommon();
}


// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                  DUSTYSTATE_HOP Implementation                                                          //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void SetDustyState_Hop(EDirection Direction)
{
	Dusty.Direction = Direction;

	Dusty.SpriteTransition = 0;

	if (Direction == DIRECTION_LEFT)
		Dusty.FloatVelocityX = -8.25f;
	else
		Dusty.FloatVelocityX = 8.25f;

	Dusty.FloatVelocityY = 0;

	Dusty.State = DUSTYSTATE_HOP;
}

void DisplayDusty_Hop()
{
	float ScaleX, OffsetX;
	if (Dusty.Direction == DIRECTION_RIGHT)
	{
		ScaleX = 1.0f;
		OffsetX = 0.0f;
	}
	else
	{
		ScaleX = -1.0f;
		OffsetX = 256.0f;
	}

	switch (Dusty.SpriteTransition)
	{
	case 0:   gxDrawSpriteScaled( (int)(Dusty.FloatX + OffsetX - 122), (int)(Dusty.FloatY - 201 + ScrollY), ScaleX, 1.0f, &DustyHop2Sprite ); break;
	case 1:   gxDrawSpriteScaled( (int)(Dusty.FloatX + OffsetX - 122), (int)(Dusty.FloatY - 212 + ScrollY), ScaleX, 1.0f, &DustyHop2Sprite ); break;
	case 2:   gxDrawSpriteScaled( (int)(Dusty.FloatX + OffsetX - 122), (int)(Dusty.FloatY - 218 + ScrollY), ScaleX, 1.0f, &DustyHop2Sprite ); break;
	case 3:   gxDrawSpriteScaled( (int)(Dusty.FloatX + OffsetX - 122), (int)(Dusty.FloatY - 224 + ScrollY), ScaleX, 1.0f, &DustyHop2Sprite ); break;
	case 4:   gxDrawSpriteScaled( (int)(Dusty.FloatX + OffsetX - 122), (int)(Dusty.FloatY - 228 + ScrollY), ScaleX, 1.0f, &DustyHop2Sprite ); break;
	case 5:   gxDrawSpriteScaled( (int)(Dusty.FloatX + OffsetX - 122), (int)(Dusty.FloatY - 236 + ScrollY), ScaleX, 1.0f, &DustyHop2Sprite ); break;
	case 6:   gxDrawSpriteScaled( (int)(Dusty.FloatX + OffsetX - 122), (int)(Dusty.FloatY - 226 + ScrollY), ScaleX, 1.0f, &DustyHop3Sprite ); break;
	case 7:   gxDrawSpriteScaled( (int)(Dusty.FloatX + OffsetX - 122), (int)(Dusty.FloatY - 226 + ScrollY), ScaleX, 1.0f, &DustyHop3Sprite ); break;
	case 8:   gxDrawSpriteScaled( (int)(Dusty.FloatX + OffsetX - 122), (int)(Dusty.FloatY - 226 + ScrollY), ScaleX, 1.0f, &DustyHop3Sprite ); break;
	case 9:   gxDrawSpriteScaled( (int)(Dusty.FloatX + OffsetX - 122), (int)(Dusty.FloatY - 226 + ScrollY), ScaleX, 1.0f, &DustyHop3Sprite ); break;
	case 10:  gxDrawSpriteScaled( (int)(Dusty.FloatX + OffsetX - 122), (int)(Dusty.FloatY - 226 + ScrollY), ScaleX, 1.0f, &DustyHop3Sprite ); break;
	case 11:  gxDrawSpriteScaled( (int)(Dusty.FloatX + OffsetX - 122), (int)(Dusty.FloatY - 226 + ScrollY), ScaleX, 1.0f, &DustyHop3Sprite ); break;
	case 12:  gxDrawSpriteScaled( (int)(Dusty.FloatX + OffsetX - 119), (int)(Dusty.FloatY - 220 + ScrollY), ScaleX, 1.0f, &DustyHop4Sprite ); break;
	case 13:  gxDrawSpriteScaled( (int)(Dusty.FloatX + OffsetX - 119), (int)(Dusty.FloatY - 215 + ScrollY), ScaleX, 1.0f, &DustyHop4Sprite ); break;
	case 14:  gxDrawSpriteScaled( (int)(Dusty.FloatX + OffsetX - 119), (int)(Dusty.FloatY - 210 + ScrollY), ScaleX, 1.0f, &DustyHop4Sprite ); break;
	case 15:  gxDrawSpriteScaled( (int)(Dusty.FloatX + OffsetX - 119), (int)(Dusty.FloatY - 205 + ScrollY), ScaleX, 1.0f, &DustyHop4Sprite ); break;
	case 16:  gxDrawSpriteScaled( (int)(Dusty.FloatX + OffsetX - 119), (int)(Dusty.FloatY - 200 + ScrollY), ScaleX, 1.0f, &DustyHop4Sprite ); break;
	case 17:  gxDrawSpriteScaled( (int)(Dusty.FloatX + OffsetX - 119), (int)(Dusty.FloatY - 195 + ScrollY), ScaleX, 1.0f, &DustyHop4Sprite ); break;
	case 18:  gxDrawSpriteScaled( (int)(Dusty.FloatX + OffsetX - 119 + 18*ScaleX), (int)(Dusty.FloatY - 217 + ScrollY), ScaleX, 1.0f, &DustyHop5Sprite ); break;
	case 19:  gxDrawSpriteScaled( (int)(Dusty.FloatX + OffsetX - 119 + 18*ScaleX), (int)(Dusty.FloatY - 217 + ScrollY), ScaleX, 1.0f, &DustyHop5Sprite ); break;
	case 20:  gxDrawSpriteScaled( (int)(Dusty.FloatX + OffsetX - 119 + 18*ScaleX), (int)(Dusty.FloatY - 217 + ScrollY), ScaleX, 1.0f, &DustyHop5Sprite ); break;
	case 21:  gxDrawSpriteScaled( (int)(Dusty.FloatX + OffsetX - 119 + 18*ScaleX), (int)(Dusty.FloatY - 217 + ScrollY), ScaleX, 1.0f, &DustyHop5Sprite ); break;
	case 22:  gxDrawSpriteScaled( (int)(Dusty.FloatX + OffsetX - 119 + 18*ScaleX), (int)(Dusty.FloatY - 217 + ScrollY), ScaleX, 1.0f, &DustyHop5Sprite ); break;
	case 23:  gxDrawSpriteScaled( (int)(Dusty.FloatX + OffsetX - 119 + 18*ScaleX), (int)(Dusty.FloatY - 217 + ScrollY), ScaleX, 1.0f, &DustyHop5Sprite ); break;
	}
}

void UpdateDusty_Hop()
{
    if (Tutorial.InitialDisplayed == false)
    {    
	    SetGameState_Crumb(TUTORIALSTATE_INITIAL);
	    return;
	}    

	// Update animation
	Dusty.SpriteTransition += 1;

	// Check for end of hop animation.
	if (Dusty.SpriteTransition == 24)
	{
		// If still holding right, reset animation and continue hopping.
		if ( ( Dusty.Direction == DIRECTION_RIGHT && GetInput_MoveRight() ) || 
			 ( Dusty.Direction == DIRECTION_LEFT  && GetInput_MoveLeft() ) )
		{
			Dusty.FloatX += Dusty.FloatVelocityX*3;
			Dusty.SpriteTransition = 0;
		}
		else  // If not, revert to stand.
		{			                         
			SetDustyState_Stand();
			return;
		}
	}

	// Fall if he walks off the current platform.
	if (Dusty.CollideWithBottomSide == false)
	{
		SetDustyState_Fall();
		return;
	}
    
	// Jump immediately if requested.
	if (GetInput_Jump())
	{
		SetDustyState_Jump( false );
	}

	// Change direction mid hop.
	if (Dusty.Direction == DIRECTION_RIGHT && GetInput_MoveLeft())
	{
		Dusty.Direction = DIRECTION_LEFT;
		Dusty.FloatVelocityX = -Dusty.FloatVelocityX;
	}
	if (Dusty.Direction == DIRECTION_LEFT && GetInput_MoveRight())
	{
		Dusty.Direction = DIRECTION_RIGHT;
		Dusty.FloatVelocityX = -Dusty.FloatVelocityX;
	}

	// If we have not switched states, move Dusty.
	if (Dusty.SpriteTransition == 18)
	{
		Dusty.FloatX += Dusty.FloatVelocityX*6;
	}
	else if (Dusty.SpriteTransition >= 0 && Dusty.SpriteTransition <= 17)
	{
		Dusty.FloatX += Dusty.FloatVelocityX;
	}
}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                  UpdateDusty_JumpCommon Implementation										           //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void UpdateDusty_JumpCommon()
{  
	// Air control.
	if (GetInput_MoveLeft())
	{
		Dusty.Direction = DIRECTION_LEFT;
		if (Dusty.FloatVelocityX >= -6)
			Dusty.FloatVelocityX -= 1.0f;
	}

	if (GetInput_MoveRight())
	{
		Dusty.Direction = DIRECTION_RIGHT;
		if (Dusty.FloatVelocityX <= 6)
			Dusty.FloatVelocityX += 1.0f;
	}
	
    // Collision with either side of screen translates to a possible wall jump.
	// Dusty is not allowed to collide with the same side twice in a row, unless he stands once in between.
	if (Dusty.CollideWithLeftSide && Dusty.Direction == DIRECTION_LEFT && Dusty.LastWall != DIRECTION_LEFT)
	{
        SetDustyState_WallJump();
        return;
	}

	if (Dusty.CollideWithRightSide && Dusty.Direction == DIRECTION_RIGHT && Dusty.LastWall != DIRECTION_RIGHT)
    {
        SetDustyState_WallJump();
        return;
    }
}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                  DUSTYSTATE_WALLJUMP_RIGHT Implementation                                               //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void SetDustyState_WallJump()
{   	
	Dusty.WallStickTimer = 15;

	Dusty.FloatVelocityX = 0;
	Dusty.FloatVelocityY = 0;

	Dusty.LastWall = Dusty.Direction;

	// Switch directions when entering a wall jump.
	if (Dusty.Direction == DIRECTION_RIGHT)
		Dusty.Direction = DIRECTION_LEFT;
	else
		Dusty.Direction = DIRECTION_RIGHT;

	Dusty.State = DUSTYSTATE_WALLJUMP;
}

void DisplayDusty_WallJump()
{
	float ScaleX, OffsetX;
	if (Dusty.Direction == DIRECTION_RIGHT)
	{
		ScaleX = 1.0f;
		OffsetX = 0.0f;
	}
	else
	{
		ScaleX = -1.0f;
		OffsetX = 256.0f;
	}

	gxDrawSpriteScaled( (int)(Dusty.FloatX + OffsetX - 128 - 8*ScaleX), (int)(Dusty.FloatY - 200 + ScrollY), ScaleX, 1.0f, &DustyWallJumpSprite );
}

void UpdateDusty_WallJump()
{                                                   
	if (Tutorial.WallJumpDisplayed == false)
	{
	    SetGameState_Crumb(TUTORIALSTATE_WALLJUMP);
	    return;
	}
		
	if (Dusty.WallStickTimer != 0)
	{                        
		Dusty.WallStickTimer -= 1;
	}

	if (Dusty.WallStickTimer == 0)
	{
		// Sliding down the wall... If you hit the ground, switch to stand state.
		if (Dusty.CollideWithBottomSide == true)
		{			  
			SetDustyState_Stand();
			return;
		}
		// If contact with the wall is lost, switch to fall state.
		else if (Dusty.CollideWithRightSide == false && Dusty.CollideWithLeftSide == false)
		{
			SetDustyState_Fall();
			return;
		}
		else // Normal sliding.
		{
			Dusty.FloatY += Dusty.FloatVelocityY;

			Dusty.FloatVelocityY += Dusty.FloatGravity/2;

			if (Dusty.FloatVelocityY > 7.0f)
				Dusty.FloatVelocityY = 7.0f;
		}
	}

	// Jump off wall by pressing jump
	if (GetInput_Jump())
	{
		SetDustyState_Jump( true );
		return;
	}

	// Let go of wall by pressing direction.
	if ( ( Dusty.Direction == DIRECTION_LEFT  && GetInput_MoveLeft() ) ||
		 ( Dusty.Direction == DIRECTION_RIGHT && GetInput_MoveRight() ) )
	{
		SetDustyState_Fall();
		return;
	}
}	

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                  DUSTYSTATE_PREPARELAUNCH Implementation                                                //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void SetDustyState_PrepareLaunch()
{
    Dusty.LastWall = DIRECTION_NONE;

    Dusty.State = DUSTYSTATE_PREPARELAUNCH;
}

void DisplayDusty_PrepareLaunch()
{
	float ScaleX, OffsetX;
	if (Dusty.Direction == DIRECTION_RIGHT)
	{
		ScaleX = 1.0f;
		OffsetX = 0.0f;
	}
	else
	{
		ScaleX = -1.0f;
		OffsetX = 256.0f;
	}
	
//	gxDrawSpriteScaled( (int)(Dusty.FloatX + OffsetX - 124 - 18*ScaleX), (int)(Dusty.FloatY - 160 + ScrollY), ScaleX, 1.0f, &DustyHop3Sprite );
}

void UpdateDusty_PrepareLaunch()
{
}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                  DUSTYSTATE_LAUNCH Implementation                                                       //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void SetDustyState_Launch(float VelocityX, float VelocityY)
{
	Dusty.FloatVelocityX = VelocityX;
	Dusty.FloatVelocityY = VelocityY;

	if (Dusty.FloatVelocityX >= 0)
		Dusty.Direction = DIRECTION_RIGHT;
	else
		Dusty.Direction = DIRECTION_LEFT;

	sxPlaySound( &DustyLaunchSound );    

    Dusty.State = DUSTYSTATE_LAUNCH;
}

void DisplayDusty_Launch()
{
	float ScaleX, OffsetX;
	if (Dusty.Direction == DIRECTION_RIGHT)
	{
		ScaleX = 1.0f;
		OffsetX = 0.0f;
	}
	else
	{
		ScaleX = -1.0f;
		OffsetX = 256.0f;
	}
	
	if (Dusty.FloatVelocityY <= -5)
		gxDrawSpriteScaled( (int)(Dusty.FloatX + OffsetX - 124 - 18*ScaleX), (int)(Dusty.FloatY - 150 + ScrollY), ScaleX, 1.0f, &DustyHop2Sprite );
	else if (Dusty.FloatVelocityY >= 5)
		gxDrawSpriteScaled( (int)(Dusty.FloatX + OffsetX - 124 - 18*ScaleX), (int)(Dusty.FloatY - 150 + ScrollY), ScaleX, 1.0f, &DustyHop4Sprite );
	else
		gxDrawSpriteScaled( (int)(Dusty.FloatX + OffsetX - 124 - 18*ScaleX), (int)(Dusty.FloatY - 140 + ScrollY), ScaleX, 1.0f, &DustyHop3Sprite );
}

void UpdateDusty_Launch()
{
	Dusty.FloatX += Dusty.FloatVelocityX;
	Dusty.FloatY += Dusty.FloatVelocityY;
	
	Dusty.FloatVelocityY += Dusty.FloatGravity;

	if (Dusty.CollideWithBottomSide == true)
	{	
		SetDustyState_Stand();
		return;
	} 

	if (Dusty.CollideWithLeftSide == true || Dusty.CollideWithRightSide == true)
	{	    
		SetDustyState_WallJump();
		return;
	}

	UpdateDusty_JumpCommon();
}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                  DUSTYSTATE_DIE Implementation                                                          //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void SetDustyState_Die()
{
	Dusty.FloatVelocityX = 0;
	Dusty.FloatVelocityY = -20;

	//sxPlaySound( &DieSound );    

	Dusty.State = DUSTYSTATE_DIE;
}

void DisplayDusty_Die()
{
	gxDrawSprite( (int)Dusty.FloatX-125, (int)(Dusty.FloatY - 181) + ScrollY, &DustyDeathSprite );
}

void UpdateDusty_Die()
{
	Dusty.FloatX += Dusty.FloatVelocityX;
	Dusty.FloatY += Dusty.FloatVelocityY;

	Dusty.FloatVelocityY += Dusty.FloatGravity;

	if (Dusty.FloatY + ScrollY > gxScreenHeight)
	{
		SetGameState_DieScreen();
		return;
	} 
}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                  UpdateDusty_Stuck Implementation                                                       //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void SetDustyState_Stuck()
{
    Dusty.State = DUSTYSTATE_STUCK;
}

void DisplayDusty_Stuck()
{
    float ScaleX, OffsetX;
    if (Dusty.Direction == DIRECTION_RIGHT)
    {
        ScaleX = 1.0f;
        OffsetX = 0.0f;
    }
    else
    {
        ScaleX = -1.0f;
        OffsetX = 256.0f;
    }

    if (Dusty.SpriteTransition <= 4)
        gxDrawSpriteScaled( (int)(Dusty.FloatX + OffsetX - 119 + 35*ScaleX), (int)(Dusty.FloatY - 217 + ScrollY), ScaleX, 1.0f, &DustyHop2Sprite );
    else
        gxDrawSpriteScaled( (int)(Dusty.FloatX + OffsetX - 124 - 18*ScaleX), (int)(Dusty.FloatY - 221 + ScrollY), ScaleX, 1.0f, &DustyHop1Sprite );     
}

void UpdateDusty_Stuck()
{ 
    {        
        if (Dusty.GumTimer <= 0 && (Dusty.CollideWithTopSide))
        {   
            Dusty.HasGumExpired = true;
            Dusty.GumTimer = 30;      
            SetDustyState_Fall();
            return;
        }
        
        if (Dusty.GumTimer <= 0 && (Dusty.CollideWithLeftSide || Dusty.CollideWithRightSide))
        {
            Dusty.HasGumExpired = true;
            Dusty.GumTimer = 30;
            SetDustyState_Stand();
            return;
        }
                                            
        if (Dusty.GumJumpAttempt > 4 && (Dusty.CollideWithLeftSide || Dusty.CollideWithRightSide))
        {
            Dusty.HasGumBlockExpired = true;
            Dusty.GumJumpAttempt = 0;
            SetDustyState_Stand();
            return;
        }
    }    
    
    if(GetInput_Jump())
    {              
        Dusty.GumJumpAttempt += 1;
    }
    
    Dusty.GumTimer--;
}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                  UpdateDusty_Collision Implementation                                                   //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

extern int ScrollY;

void UpdateDusty_Collision()
{
	// Initialize all collision variables to false.  One or more of these will be set to true in this function.
	// This function also corrects Dusty's position to not intersect anything.
	Dusty.CollideWithRightSide = false;
	Dusty.CollideWithLeftSide = false;
	Dusty.CollideWithBottomSide = false;
	Dusty.CollideWithTopSide = false;

	// Collision with right side of the right screen and Dusty's right Side
	if (Dusty.FloatX + Dusty.Right >= gxScreenWidth)
	{
		Dusty.CollideWithRightSide = true;
		Dusty.FloatX = (float)gxScreenWidth - (float)Dusty.Right;	
	} 

	//Collision with the left side of the screen    
	if (Dusty.FloatX + Dusty.Left <= 0)
	{
		Dusty.CollideWithLeftSide = true;
		Dusty.FloatX = -(float)Dusty.Left;
	}
	//Collision with the bottom side of the screen
	if (Dusty.FloatY + Dusty.Bottom >= Chapter.StitchedHeight * 64 )
	{	
		Dusty.CollideWithBottomSide = true;
		Dusty.FloatY = (float)Chapter.StitchedHeight * 64 - (float)Dusty.Bottom;
	} 

	for (int y = 0; y < Chapter.StitchedHeight; y++)
	{
		for (int x = 0; x < Chapter.StitchedWidth; x++)
		{
			// Skip empty blocks.
			if (IsBlockEmpty(x, y))
			{
				continue;
			}

			// Determine the bounds of the block.
			float BlockLeft   = (float)x*64;
			float BlockRight  = (float)x*64 + 64;
			float BlockTop    = (float)y*64;
			float BlockBottom = (float)y*64 + 64;

			// Check to see if Dusty's rectangle overlaps with this block.
			// 
			// The general idea here is to compare the smaller of the bottom sides (Min of Maxes) against the larger of the top sides (Max of Mins).  
			// If the min is less than the max, there is overlap.  The same calculation is also done in X.
			//
			// +-----------------+    +--------+   MnMx = Minimum of Maximum values (e.g. smaller of the two right edges)
			// |                 |    |        |   MxMn = Maximum of Minimum values (e.g. larger of the two left edges)
			// +-----------------+    +--------+        
			//              MnMx-^    ^-MxMn            In this diagram MxMn is greater than MnMx, so there is no overlap.
			//
			// The Y axis is considered first because the level is larger on the Y axis, so this will reject most blocks earlier (code optimization).
			if (Max(Dusty.FloatY + Dusty.Top, BlockTop) <= Min(Dusty.FloatY + Dusty.Bottom, BlockBottom))
			{
				// Check to see if Dusty's rectangle overlaps with the block in X.
				if(Max(Dusty.FloatX + Dusty.Left, BlockLeft) <= Min(Dusty.FloatX + Dusty.Right, BlockRight))
				{    
					// {Left,Right,Top,Bottom}BlockIsEmpty are used to prevent collisions with internal edges (edges between blocks).  If Dusty jumps up into the bottom of a wide platform
					// and hits a lucky spot between two blocks, the collision code can determine that it would be easier to push him left or right instead of down.
					// This is bad and causes all kinds of potential collision problems.
					//
					// Helpful diagram:
					//
					// +---++---++---++---++---++---+ 
					// | A || B ||   ||   ||   ||   | 
					// +---++---++---++---++---++---+ 
					//      ^--Dusty hits right here.  
					//
					// In this example Dusty might collide with the right edge of block A OR the left edge of block B, which would be terrible since we really 
					// only want him to collide with the bottom sides of the blocks.
					//
					// The solution is to detect when blocks have other blocks next to them.  When they do, the internal edges between blocks are ignored for collision.
					//
					bool LeftBlockIsEmpty   = IsBlockEmpty(x-1, y);
					bool RightBlockIsEmpty  = IsBlockEmpty(x+1, y);
					bool TopBlockIsEmpty    = IsBlockEmpty(x, y-1);
					bool BottomBlockIsEmpty = IsBlockEmpty(x, y+1);

					// Calculate the distance Dusty would have to be pushed in each possible direction to get him out of intersecting the block.
					float LeftDistance	= (Dusty.FloatX + Dusty.Right)  - (BlockLeft);
					float RightDistance	= (BlockRight) - (Dusty.FloatX +  Dusty.Left);
					float DownDistance	= (BlockBottom)- (Dusty.FloatY +  Dusty.Top );
					float UpDistance	= (Dusty.FloatY + Dusty.Bottom) - (BlockTop );

					// Prefer to collide with the side of the block that would push Dusty out the least distance.
					// (Only consider sides that are not adjacent to another solid block).
					if (LeftBlockIsEmpty && LeftDistance < RightDistance && LeftDistance < DownDistance && LeftDistance < UpDistance)
					{
						Dusty.CollideWithRightSide = true;//Collision with Dusty's Right Side but the left side of the platform
						Dusty.FloatX -= LeftDistance;
					}

					if (RightBlockIsEmpty && RightDistance < LeftDistance && RightDistance < DownDistance && RightDistance < UpDistance)
					{
						Dusty.CollideWithLeftSide = true;//Collision with Dusty's Left Side but the right side of the platform
						Dusty.FloatX += RightDistance;
					}


					if (BottomBlockIsEmpty && DownDistance < RightDistance && DownDistance < LeftDistance && DownDistance < UpDistance)
					{
						Dusty.CollideWithTopSide = true;//Collision with Dusty's Top Side but the Bottom side of the platform
						Dusty.FloatY += DownDistance;
					}

					if (TopBlockIsEmpty && UpDistance < RightDistance && UpDistance < DownDistance && UpDistance < LeftDistance)
					{
						Dusty.CollideWithBottomSide = true;//Collision with Dusty's Bottom Side but the Top side of the platform
						Dusty.FloatY -= UpDistance;
					}
										
					int BlockID = GetBlockID(x, y);
					if (BlockID < SPECIALBLOCKID_FIRST)
					{
						SBlock* Block = &Chapter.Blocks[BlockID];
						
						if (Dusty.CollideWithTopSide && Block->Destructible)
						{
							Chapter.StitchedBlocks[y * Chapter.StitchedWidth + x] = SPECIALBLOCKID_BLANK;
						}
						
						if ((Dusty.CollideWithBottomSide || Dusty.CollideWithTopSide || Dusty.CollideWithLeftSide || Dusty.CollideWithRightSide)
						 && Block->DelayDestructible)
						{
							for (int i = 50; i < 1; i++)
							{	    
							    if (i == 0)
							    {
							        sxPlaySound( &BlockBreakSound );
							        Chapter.StitchedBlocks[y * Chapter.StitchedWidth + x] = SPECIALBLOCKID_BLANK;
							        
							    }    
							}							
						}
						
						if ((Dusty.CollideWithTopSide || Dusty.CollideWithLeftSide || Dusty.CollideWithRightSide || Dusty.CollideWithBottomSide)
							&& Block->EndOfLevel)
						{
						    sxPlaySound( &DustyWinSound );
							SetGameState_WinScreen();
							return;
						}
									
						if (Dusty.HasGumExpired && Block->Gum)
						{
						    sxPlaySound( &BlockBreakSound );
							Chapter.StitchedBlocks[y * Chapter.StitchedWidth + x] = SPECIALBLOCKID_BLANK;
						}
	                    
						if ((Dusty.CollideWithLeftSide || Dusty.CollideWithRightSide || Dusty.CollideWithTopSide)
							&& Block->Gum == true)
						{
						    sxPlaySound( &BlockBreakSound );
							SetDustyState_Stuck();
							return;                  
						}	
						
                        if (Dusty.HasGumBlockExpired && Block->GumJump)
                        {
                            sxPlaySound( &BlockBreakSound );
                            Chapter.StitchedBlocks[y * Chapter.StitchedWidth + x] = SPECIALBLOCKID_BLANK;
                        }

                        if ((Dusty.CollideWithLeftSide || Dusty.CollideWithRightSide || Dusty.CollideWithTopSide)
                            && Block->GumJump == true)
                        {
                            sxPlaySound( &BlockBreakSound );
                            SetDustyState_Stuck();
                            return;                  
                        }
                        
                        if ((Dusty.CollideWithLeftSide || Dusty.CollideWithRightSide || Dusty.CollideWithTopSide)
							&& Block->Jello)
						{
						    sxPlaySound( &JelloSound );
							SetDustyState_Fall();
							return;                
						}	          					
					}
				}
			}
		}
	}
}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                  Dusty Display & Update Implementation                                                  //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void DisplayDusty()
{
	switch (Dusty.State)
	{
	case DUSTYSTATE_STAND:			    DisplayDusty_Stand(); break;
	case DUSTYSTATE_JUMP:			    DisplayDusty_Jump(); break;
	case DUSTYSTATE_FALL:				DisplayDusty_Fall(); break;
	case DUSTYSTATE_HOP:			    DisplayDusty_Hop(); break;
	case DUSTYSTATE_WALLJUMP:			DisplayDusty_WallJump(); break;
	case DUSTYSTATE_PREPARELAUNCH:      DisplayDusty_PrepareLaunch(); break;
	case DUSTYSTATE_LAUNCH:             DisplayDusty_Launch(); break;	
	case DUSTYSTATE_DIE:				DisplayDusty_Die(); break;
	case DUSTYSTATE_STUCK:              DisplayDusty_Stuck(); break;
	default:						    break;
	}

	if (DevMode)
	{
		// Draw a yellow + at Dusty's root location.
		gxDrawString((int)Dusty.FloatX-4, (int)Dusty.FloatY-4 + ScrollY, 8, gxRGB32(255, 255, 0), "+");

		// Draw a red + at all four corners of Dusty.
		gxDrawString((int)Dusty.FloatX + Dusty.Left,  (int)Dusty.FloatY + Dusty.Top    + ScrollY, 8, gxRGB32(255, 0, 0), "+");//Upper Left
		gxDrawString((int)Dusty.FloatX + Dusty.Right, (int)Dusty.FloatY + Dusty.Top    + ScrollY, 8, gxRGB32(255, 0, 0), "+");//Upper Right
		gxDrawString((int)Dusty.FloatX + Dusty.Left,  (int)Dusty.FloatY + Dusty.Bottom + ScrollY, 8, gxRGB32(255, 0, 0), "+");//Lower Left
		gxDrawString((int)Dusty.FloatX + Dusty.Right, (int)Dusty.FloatY + Dusty.Bottom + ScrollY, 8, gxRGB32(255, 0, 0), "+");//Lower Right
	}
}

void UpdateDusty()
{
	if (Dusty.State != DUSTYSTATE_DIE)
		UpdateDusty_Collision();

	switch (Dusty.State)
	{
	case DUSTYSTATE_STAND:			    UpdateDusty_Stand(); break;
	case DUSTYSTATE_JUMP:			    UpdateDusty_Jump(); break;
	case DUSTYSTATE_FALL:				UpdateDusty_Fall(); break;
	case DUSTYSTATE_HOP:				UpdateDusty_Hop(); break;
	case DUSTYSTATE_WALLJUMP:			UpdateDusty_WallJump(); break;
	case DUSTYSTATE_PREPARELAUNCH:      UpdateDusty_PrepareLaunch(); break;
	case DUSTYSTATE_LAUNCH:             UpdateDusty_Launch(); break;
	case DUSTYSTATE_DIE:				UpdateDusty_Die(); break;
	case DUSTYSTATE_STUCK:              UpdateDusty_Stuck(); break;
	default:						    break;
	}
}
