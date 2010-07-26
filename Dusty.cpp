//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                         // 
//                                                          Super Dust Bunny                                                               //
//                                                                                                                                         //
//                               Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>                      //
//                                      Copyright � 2010 by Thomas Perry and Wade Brainerd. All rights reserved.                              //
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

	Dusty.CanWallJump = true;
	Dusty.WallStickTimer = 0;

	Dusty.CollideWithLeftSide = false;
	Dusty.CollideWithRightSide = false;
	Dusty.CollideWithTopSide = false;
	Dusty.CollideWithBottomSide = false;
	Dusty.CanCollideWithWall = true;
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
	Dusty.CanWallJump = true;
	Dusty.CanCollideWithWall = true;

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
		gxDrawSpriteScaled( (int)(Dusty.FloatX + OffsetX - 119 + 35*ScaleX), (int)(Dusty.FloatY - 217 + ScrollY), ScaleX, 1.0f, &DustyHop02 );
	else
		gxDrawSpriteScaled( (int)(Dusty.FloatX + OffsetX - 124 - 18*ScaleX), (int)(Dusty.FloatY - 221 + ScrollY), ScaleX, 1.0f, &DustyHop01 );
}

void UpdateDusty_Stand()
{		
	if ( GetInput_Jump() )      
	{
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
		sxPlaySound( &WallJump );
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
		gxDrawSpriteScaled( (int)(Dusty.FloatX + OffsetX - 124 - 18*ScaleX), (int)(Dusty.FloatY - 160 + ScrollY), ScaleX, 1.0f, &DustyHop03 );
	else
		gxDrawSpriteScaled( (int)(Dusty.FloatX + OffsetX - 124 - 18*ScaleX), (int)(Dusty.FloatY - 160 + ScrollY), ScaleX, 1.0f, &DustyHop04 );
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
		gxDrawSpriteScaled( (int)(Dusty.FloatX + OffsetX - 124 - 18*ScaleX), (int)(Dusty.FloatY - 160 + ScrollY), ScaleX, 1.0f, &DustyHop05 );
	else
		gxDrawSpriteScaled( (int)(Dusty.FloatX + OffsetX - 124 - 18*ScaleX), (int)(Dusty.FloatY - 160 + ScrollY), ScaleX, 1.0f, &DustyHop04 );
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
	case 0:   gxDrawSpriteScaled( (int)(Dusty.FloatX + OffsetX - 122), (int)(Dusty.FloatY - 201 + ScrollY), ScaleX, 1.0f, &DustyHop03 ); break;
	case 1:   gxDrawSpriteScaled( (int)(Dusty.FloatX + OffsetX - 122), (int)(Dusty.FloatY - 212 + ScrollY), ScaleX, 1.0f, &DustyHop03 ); break;
	case 2:   gxDrawSpriteScaled( (int)(Dusty.FloatX + OffsetX - 122), (int)(Dusty.FloatY - 218 + ScrollY), ScaleX, 1.0f, &DustyHop03 ); break;
	case 3:   gxDrawSpriteScaled( (int)(Dusty.FloatX + OffsetX - 122), (int)(Dusty.FloatY - 224 + ScrollY), ScaleX, 1.0f, &DustyHop03 ); break;
	case 4:   gxDrawSpriteScaled( (int)(Dusty.FloatX + OffsetX - 122), (int)(Dusty.FloatY - 228 + ScrollY), ScaleX, 1.0f, &DustyHop03 ); break;
	case 5:   gxDrawSpriteScaled( (int)(Dusty.FloatX + OffsetX - 122), (int)(Dusty.FloatY - 236 + ScrollY), ScaleX, 1.0f, &DustyHop03 ); break;
	case 6:   gxDrawSpriteScaled( (int)(Dusty.FloatX + OffsetX - 122), (int)(Dusty.FloatY - 226 + ScrollY), ScaleX, 1.0f, &DustyHop04 ); break;
	case 7:   gxDrawSpriteScaled( (int)(Dusty.FloatX + OffsetX - 122), (int)(Dusty.FloatY - 226 + ScrollY), ScaleX, 1.0f, &DustyHop04 ); break;
	case 8:   gxDrawSpriteScaled( (int)(Dusty.FloatX + OffsetX - 122), (int)(Dusty.FloatY - 226 + ScrollY), ScaleX, 1.0f, &DustyHop04 ); break;
	case 9:   gxDrawSpriteScaled( (int)(Dusty.FloatX + OffsetX - 122), (int)(Dusty.FloatY - 226 + ScrollY), ScaleX, 1.0f, &DustyHop04 ); break;
	case 10:  gxDrawSpriteScaled( (int)(Dusty.FloatX + OffsetX - 122), (int)(Dusty.FloatY - 226 + ScrollY), ScaleX, 1.0f, &DustyHop04 ); break;
	case 11:  gxDrawSpriteScaled( (int)(Dusty.FloatX + OffsetX - 122), (int)(Dusty.FloatY - 226 + ScrollY), ScaleX, 1.0f, &DustyHop04 ); break;
	case 12:  gxDrawSpriteScaled( (int)(Dusty.FloatX + OffsetX - 119), (int)(Dusty.FloatY - 220 + ScrollY), ScaleX, 1.0f, &DustyHop05 ); break;
	case 13:  gxDrawSpriteScaled( (int)(Dusty.FloatX + OffsetX - 119), (int)(Dusty.FloatY - 215 + ScrollY), ScaleX, 1.0f, &DustyHop05 ); break;
	case 14:  gxDrawSpriteScaled( (int)(Dusty.FloatX + OffsetX - 119), (int)(Dusty.FloatY - 210 + ScrollY), ScaleX, 1.0f, &DustyHop05 ); break;
	case 15:  gxDrawSpriteScaled( (int)(Dusty.FloatX + OffsetX - 119), (int)(Dusty.FloatY - 205 + ScrollY), ScaleX, 1.0f, &DustyHop05 ); break;
	case 16:  gxDrawSpriteScaled( (int)(Dusty.FloatX + OffsetX - 119), (int)(Dusty.FloatY - 200 + ScrollY), ScaleX, 1.0f, &DustyHop05 ); break;
	case 17:  gxDrawSpriteScaled( (int)(Dusty.FloatX + OffsetX - 119), (int)(Dusty.FloatY - 195 + ScrollY), ScaleX, 1.0f, &DustyHop05 ); break;
	case 18:  gxDrawSpriteScaled( (int)(Dusty.FloatX + OffsetX - 119 + 35*ScaleX), (int)(Dusty.FloatY - 217 + ScrollY), ScaleX, 1.0f, &DustyHop02 ); break;
	case 19:  gxDrawSpriteScaled( (int)(Dusty.FloatX + OffsetX - 119 + 35*ScaleX), (int)(Dusty.FloatY - 217 + ScrollY), ScaleX, 1.0f, &DustyHop02 ); break;
	case 20:  gxDrawSpriteScaled( (int)(Dusty.FloatX + OffsetX - 119 + 35*ScaleX), (int)(Dusty.FloatY - 217 + ScrollY), ScaleX, 1.0f, &DustyHop02 ); break;
	case 21:  gxDrawSpriteScaled( (int)(Dusty.FloatX + OffsetX - 119 + 35*ScaleX), (int)(Dusty.FloatY - 217 + ScrollY), ScaleX, 1.0f, &DustyHop02 ); break;
	case 22:  gxDrawSpriteScaled( (int)(Dusty.FloatX + OffsetX - 119 + 35*ScaleX), (int)(Dusty.FloatY - 217 + ScrollY), ScaleX, 1.0f, &DustyHop02 ); break;
	case 23:  gxDrawSpriteScaled( (int)(Dusty.FloatX + OffsetX - 119 + 35*ScaleX), (int)(Dusty.FloatY - 217 + ScrollY), ScaleX, 1.0f, &DustyHop02 ); break;
	}
}

void UpdateDusty_Hop()
{
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

	// Collision with either side of screen
	if (Dusty.CanWallJump && Dusty.CanCollideWithWall == true)
	{
		if ( (Dusty.Direction == DIRECTION_LEFT && Dusty.CollideWithLeftSide) ||
			 (Dusty.Direction == DIRECTION_RIGHT && Dusty.CollideWithRightSide) )
		{
			SetDustyState_WallJump();			
			return;
		}
	}
}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                  DUSTYSTATE_WALLJUMP_RIGHT Implementation                                               //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void SetDustyState_WallJump()
{   
	Dusty.WallStickTimer = 15;
	Dusty.CanWallJump = true;

	Dusty.FloatVelocityX = 0;
	Dusty.FloatVelocityY = 0;

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

	gxDrawSpriteScaled( (int)(Dusty.FloatX + OffsetX - 128 - 8*ScaleX), (int)(Dusty.FloatY - 200 + ScrollY), ScaleX, 1.0f, &RightFaceWallJump01 );
}

void UpdateDusty_WallJump()
{                                                
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
	    Dusty.CanCollideWithWall = false;
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
    Dusty.State = DUSTYSTATE_PREPARELAUNCH;
}

void DisplayDusty_PrepareLaunch()
{
	// Until we get sprites, it looks better to just hide him in the barrel.

    //if (Dusty.LastDirectionSprite == 1)
    //{
    //    gxDrawSprite( Dusty.FloatX-125, (Dusty.FloatY - 221) + ScrollY, &DustyHop02 );
    //}  
    //
    //if (Dusty.LastDirectionSprite == 0)
    //{
    //    gxDrawSprite( Dusty.FloatX-130, (Dusty.FloatY - 217) + ScrollY, &DustyHopLeft02 );
    //}
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

	sxPlaySound( &LaunchSound01 );    

    Dusty.State = DUSTYSTATE_LAUNCH;
}

void DisplayDusty_Launch()
{
	if (Dusty.FloatVelocityY > 0) // Falling
	{
		if (Dusty.FloatVelocityX >= 0)
		{
			gxDrawSprite( (int)Dusty.FloatX-125, (int)(Dusty.FloatY - 221) + ScrollY, &DustyHop02 );
		}
		else
		{
			gxDrawSprite( (int)Dusty.FloatX-130, (int)(Dusty.FloatY - 217) + ScrollY, &DustyHopLeft02 );
		}
	}
	else // Rising
	{
		if (Dusty.FloatVelocityX >= 0)
		{
			gxDrawSprite( (int)Dusty.FloatX-107, (int)(Dusty.FloatY - 177) + ScrollY, &DustyHop03 );
		} 
		else
		{
			gxDrawSprite( (int)Dusty.FloatX-145, (int)(Dusty.FloatY - 177) + ScrollY, &DustyHopLeft03 );
		}
	}
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

	if ((Dusty.CollideWithLeftSide == true || Dusty.CollideWithRightSide == true) && Dusty.CanWallJump)
	{
		SetDustyState_WallJump();
		return;
	}
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

extern gxSprite DustyDie;

void DisplayDusty_Die()
{
	gxDrawSprite( (int)Dusty.FloatX-125, (int)(Dusty.FloatY - 181) + ScrollY, &DustyDie );
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
	if (Dusty.FloatY + Dusty.Bottom >= Chapter.Pages[0].Height * 64 )
	{	
		Dusty.CollideWithBottomSide = true;
		Dusty.FloatY = (float)Chapter.Pages[0].Height * 64 - (float)Dusty.Bottom;
	} 

	for (int y = 0; y < Chapter.Pages[0].Height; y++)
	{
		for (int x = 0; x < Chapter.Pages[0].Width; x++)
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
					float UpDistance		= (Dusty.FloatY + Dusty.Bottom) - (BlockTop );

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
					
					
					//int BlockID = Chapter.Pages[0].Blocks[y * Chapter.Pages[0].Width + x];
					SBlock* Block = &Chapter.Blocks[GetBlockID(x, y)];
					
					if (Dusty.CollideWithTopSide == true && Block->Destructible == true)
					{
						Chapter.Pages[0].Blocks[y * Chapter.Pages[0].Width + x] = SPECIALBLOCKID_BLANK;
					}
					
					if (Dusty.CollideWithBottomSide == true && Block->Destructible == true)
					{
						Chapter.Pages[0].Blocks[y * Chapter.Pages[0].Width + x] = SPECIALBLOCKID_BLANK;
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
	default:						    break;
	}
}
