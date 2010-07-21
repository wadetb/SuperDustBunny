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

void UpdateDusty_JumpCommon();

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                  Dusty initialization function                                                          //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void InitDusty()
{
	Dusty.State = DUSTYSTATE_STAND;
	Dusty.X = 350;
	Dusty.Y = 1024;
	Dusty.DirectionX = 6;
	Dusty.DirectionY = 6;
	Dusty.Left = -40;
	Dusty.Right = 40;
	Dusty.Top = -120;
	Dusty.Bottom = 0;
	Dusty.Gravity = 10;
	Dusty.Velocity = 10;

	Dusty.VerticalCounter = 0;
	Dusty.JumpQueue = 0;
	Dusty.WallStickTimer = 0;
	Dusty.HopRightSprite = 0;
	Dusty.HopLeftSprite = 0;
	Dusty.LastDirectionSprite = 1;
	Dusty.SpriteTransition = 0;
	Dusty.LeftSide = 0;
	Dusty.RightSide = 768;
	Dusty.TopSide = 0;
	Dusty.BottomSide = 1024;

	Dusty.RightSideIsInPlatform = false;
	Dusty.LeftSideIsInPlatform = false;
	Dusty.TopSideIsInPlatform = false;
	Dusty.BottomSideIsInPlatform = false;
	Dusty.AreRecsIntersecting = false;
	Dusty.IsJumping = false;
	Dusty.CanWallJump = true;
	Dusty.StraightFallTrigger = false;
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
void SetDustyState_Hop_Right();
void SetDustyState_Hop_Left();
void SetDustyState_WallJump_Right();
void SetDustyState_WallJump_Left();
void SetDustyState_PrepareLaunch();
void SetDustyState_Launch();

void SetPlatformState_Stationary();

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                  DUSTYSTATE_STAND Implementation                                                        //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void SetDustyState_Stand()
{
	Dusty.CanWallJump = true;
	Dusty.State = DUSTYSTATE_STAND;
}

void DisplayDusty_Stand()
{
	if (Dusty.LastDirectionSprite == 0) //Standing
	{
		gxDrawSprite( Dusty.X-112, (Dusty.Y - 221) + ScrollY, &DustyHopLeft01 );
	}

	if (Dusty.LastDirectionSprite == 1)//After moving, stops to face the direction last moved.
	{
		gxDrawSprite( Dusty.X-140, (Dusty.Y - 221) + ScrollY, &DustyHop01 );
	}
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
		SetDustyState_Hop_Right();
		return;
	}

	if ( GetInput_MoveLeft() )
	{
		SetDustyState_Hop_Left();
		return;
	}

	if (Dusty.CollideWithBottomSide == false)
	{
		SetDustyState_Fall();
		return;
	}
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
	
	Dusty.VerticalCounter = 27;
	Dusty.Y -= 10;
	Dusty.State = DUSTYSTATE_JUMP;
}

void DisplayDusty_Jump()
{
	if (Dusty.LastDirectionSprite == 1)     
	{
		gxDrawSprite( Dusty.X-107, (Dusty.Y - 177) + ScrollY, &DustyHop03 );
	} 

	if (Dusty.LastDirectionSprite == 0)
	{
		gxDrawSprite( Dusty.X-145, (Dusty.Y - 177) + ScrollY, &DustyHopLeft03 );
	}
}

void UpdateDusty_Jump()
{         
	if (Dusty.LastDirectionSprite == 1)
	{
		Dusty.X = Dusty.X + Dusty.DirectionX;                                                   
	}

	if (Dusty.LastDirectionSprite == 0)
	{          
		Dusty.X = Dusty.X - Dusty.DirectionX;                         
	}

	if (Dusty.VerticalCounter == 0)
	{
		SetDustyState_Fall();
		return;
	}
	else
	{     
		Dusty.VerticalCounter -= 1;
		Dusty.Y -= 10;
	}

	UpdateDusty_JumpCommon();
}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                  DUSTYSTATE_FALL Implementation                                                         //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void SetDustyState_Fall()
{
	Dusty.State = DUSTYSTATE_FALL;
}

void DisplayDusty_Fall()
{
    if (Dusty.LastDirectionSprite == 1)
    {
        gxDrawSprite( Dusty.X-125, (Dusty.Y - 221) + ScrollY, &DustyHop02 );
    } 
    
	if (Dusty.LastDirectionSprite == 0)
	{
		gxDrawSprite( Dusty.X-130, (Dusty.Y - 217) + ScrollY, &DustyHopLeft02 );
	}
}

void UpdateDusty_Fall()
{
	Dusty.Y = Dusty.Y + Dusty.DirectionY;

	if (Dusty.LastDirectionSprite == 1)
	{
		Dusty.X = Dusty.X + Dusty.DirectionX;	                                                  
	}

	if (Dusty.LastDirectionSprite == 0)
	{          
		Dusty.X = Dusty.X - Dusty.DirectionX;                          
	}

	if (Dusty.CollideWithBottomSide == true )
	{	
		SetDustyState_Stand();
		return;
	} 

	UpdateDusty_JumpCommon();
}


// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                  DUSTYSTATE_HOP_RIGHT Implementation                                                    //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void SetDustyState_Hop_Right()
{
	Dusty.HopRightSprite = 1;
	Dusty.SpriteTransition = 30;
	Dusty.LastDirectionSprite = 1;
	Dusty.State = DUSTYSTATE_HOP_RIGHT;
}

void DisplayDusty_Hop_Right()
{
	if (Dusty.HopRightSprite == 1) 
	{
		gxDrawSprite( Dusty.X-140, (Dusty.Y - 221) + ScrollY, &DustyHop01 );
	} 

	if (Dusty.HopRightSprite == 2)
	{
		gxDrawSprite( Dusty.X-140, (Dusty.Y - 221) + ScrollY, &DustyHop02 );
	} 

	if (Dusty.HopRightSprite == 3)
	{
		gxDrawSprite( Dusty.X-140, (Dusty.Y - 221) + ScrollY, &DustyHop03 );
	}

	if (Dusty.HopRightSprite == 4)
	{
		gxDrawSprite( Dusty.X-140, (Dusty.Y - 221) + ScrollY, &DustyHop04 );
	}    

	if (Dusty.HopRightSprite == 5)
	{
		gxDrawSprite( Dusty.X-140, (Dusty.Y - 221) + ScrollY, &DustyHop05 );
	}     

	if (Dusty.HopRightSprite == 6)
	{
		gxDrawSprite( Dusty.X-140, (Dusty.Y - 221) + ScrollY, &DustyHop01 );
	}   
}	

void UpdateDusty_Hop_Right()
{
	Dusty.X = Dusty.X + Dusty.DirectionX;  

	// Update animation
	Dusty.SpriteTransition -= 1;

	//Walk off right side of Platform
	if (Dusty.CollideWithBottomSide == false)
	{
		SetDustyState_Fall();
		return;
	}
    
    if (Dusty.JumpQueue == 1 && Dusty.SpriteTransition == 0)
    {		
        Dusty.LastDirectionSprite = 1;
        Dusty.JumpQueue = 0;
        SetDustyState_Jump( false );
        return;
    }  
    
	if ( !GetInput_MoveRight() && Dusty.SpriteTransition == 0 )
	{			                         
		SetDustyState_Stand();
		return;
	}

	if (GetInput_Jump() && Dusty.SpriteTransition != 0)
	{
        Dusty.JumpQueue = 1;
		//sxPlaySound( &DustyToJump );
		//sxDestroySound( &DustyToJump ); 
	}

	if (GetInput_MoveLeft())
	{
		Dusty.LastDirectionSprite = 0;
		SetDustyState_Hop_Left();
		return;
	}

	if (Dusty.SpriteTransition == 30)
	{
		Dusty.HopRightSprite = 1;
	}  

	if (Dusty.SpriteTransition == 25) 
	{
		Dusty.HopRightSprite = 2;
	} 

	if (Dusty.SpriteTransition == 20)
	{
		Dusty.HopRightSprite = 3;
	} 

	if (Dusty.SpriteTransition == 15)
	{
		Dusty.HopRightSprite = 4;
	}

	if (Dusty.SpriteTransition == 10)
	{
		Dusty.HopRightSprite = 5;
	}

	if (Dusty.SpriteTransition == 5)
	{
		Dusty.HopRightSprite = 6;
	}

	if (Dusty.SpriteTransition == 0)
	{
		Dusty.SpriteTransition = 30;
	}		
}	

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                  DUSTYSTATE_HOP_LEFT Implementation                                                     //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void SetDustyState_Hop_Left()
{
	Dusty.HopLeftSprite = 1;
	Dusty.SpriteTransition = 30;
	Dusty.LastDirectionSprite = 0;  
	Dusty.State = DUSTYSTATE_HOP_LEFT;
}

void DisplayDusty_Hop_Left()
{
	if (Dusty.HopLeftSprite == 1) 
	{
		gxDrawSprite( Dusty.X-112, (Dusty.Y - 221) + ScrollY, &DustyHopLeft01 );
	} 

	if (Dusty.HopLeftSprite == 2)
	{
		gxDrawSprite( Dusty.X-112, (Dusty.Y - 221) + ScrollY, &DustyHopLeft02 );
	} 

	if (Dusty.HopLeftSprite == 3)
	{
		gxDrawSprite( Dusty.X-112, (Dusty.Y - 221) + ScrollY, &DustyHopLeft03 );
	}

	if (Dusty.HopLeftSprite == 4)
	{
		gxDrawSprite( Dusty.X-112, (Dusty.Y - 221) + ScrollY, &DustyHopLeft04 );
	} 
	if (Dusty.HopLeftSprite == 5)
	{
		gxDrawSprite( Dusty.X-112, (Dusty.Y - 221) + ScrollY, &DustyHopLeft06 );
	}    
	if (Dusty.HopLeftSprite == 6)
	{
		gxDrawSprite( Dusty.X-112, (Dusty.Y - 221) + ScrollY, &DustyHopLeft01 );
	}              
}

void UpdateDusty_Hop_Left()
{
	Dusty.X = Dusty.X - Dusty.DirectionX;	

	// Update animation
	Dusty.SpriteTransition -= 1;

	//Walk off left side of Platform
	if (Dusty.CollideWithBottomSide == false)
	{
		SetDustyState_Fall();
		return;
	}
	
    if (Dusty.JumpQueue == 1 && Dusty.SpriteTransition == 0)
    {
        Dusty.LastDirectionSprite = 0;
        Dusty.JumpQueue = 0;
        SetDustyState_Jump( false );
        return;
    } 
    
	if ( !GetInput_MoveLeft() && Dusty.SpriteTransition == 0 )
	{          
		SetDustyState_Stand();
		return;
	}

	if (GetInput_Jump() && Dusty.SpriteTransition != 0)
	{
        Dusty.JumpQueue = 1;
		//sxPlaySound( &DustyToJump );
		//sxDestroySound( &DustyToJump ); 
	}

	if (GetInput_MoveRight())
	{
		Dusty.LastDirectionSprite = 1;
		SetDustyState_Hop_Right();
		return;
	}

	if (Dusty.SpriteTransition == 30)
	{
		Dusty.HopLeftSprite = 1;
	}  

	if (Dusty.SpriteTransition == 25) 
	{
		Dusty.HopLeftSprite = 2;
	} 

	if (Dusty.SpriteTransition == 20)
	{
		Dusty.HopLeftSprite = 3;
	} 

	if (Dusty.SpriteTransition == 15)
	{
		Dusty.HopLeftSprite = 4;
	}

	if (Dusty.SpriteTransition == 10)
	{
		Dusty.HopLeftSprite = 5;
	}

	if (Dusty.SpriteTransition == 5)
	{
		Dusty.HopLeftSprite = 6;
	}

	if (Dusty.SpriteTransition == 0)
	{
		Dusty.SpriteTransition = 30;
	}
}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                  UpdateDusty_JumpCommon Implementation										           //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void UpdateDusty_JumpCommon()
{  
	// Collision with right side of screen
	if (Dusty.CollideWithRightSide == true && Dusty.CanWallJump == true && Dusty.LastDirectionSprite == 1)
	{
		SetDustyState_WallJump_Right();
		return;
	} 

	//Collision with the left side of the screen    
	if (Dusty.CollideWithLeftSide == true && Dusty.CanWallJump == true && Dusty.LastDirectionSprite == 0)
	{	
		SetDustyState_WallJump_Left();
		return;
	}
}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                  DUSTYSTATE_WALLJUMP_RIGHT Implementation                                               //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void SetDustyState_WallJump_Right()
{   
	Dusty.WallStickTimer = 15;
	Dusty.CanWallJump = true;
	Dusty.State = DUSTYSTATE_WALLJUMP_RIGHT;//Collide with Right wall. 
}

void DisplayDusty_WallJump_Right()
{
	gxDrawSprite( Dusty.X-115, (Dusty.Y - 200) + ScrollY, &LeftFaceWallJump01 );       
}

void UpdateDusty_WallJump_Right()//Collided with Right Wall
{                                                
	if (Dusty.WallStickTimer == 0)
	{                   
		Dusty.VerticalCounter = 0;          
		if (Dusty.CollideWithRightSide == true)
		{
			Dusty.Y = Dusty.Y + Dusty.DirectionY;
			if (Dusty.CollideWithBottomSide == true)
			{			  
				SetDustyState_Stand();
				return;
			}  
		}
		else
		{
			Dusty.LastDirectionSprite = 0;
			SetDustyState_Fall();
			return;
		}
	}

	if (Dusty.WallStickTimer != 0)
	{                        
		Dusty.WallStickTimer -= 1;
	}

	if (GetInput_Jump() && Dusty.LastDirectionSprite == 1)
	{               
		Dusty.LastDirectionSprite = 0;
		Dusty.CanWallJump = true;
		SetDustyState_Jump( true );
		return;
	}
}	

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                  DUSTYSTATE_WALLJUMP_LEFT Implementation                                                //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -// 

void SetDustyState_WallJump_Left()// Collided with Left Wall
{	
	Dusty.CanWallJump = true;
	Dusty.WallStickTimer = 15;
	Dusty.State = DUSTYSTATE_WALLJUMP_LEFT;
} 

void DisplayDusty_WallJump_Left()
{
	gxDrawSprite( Dusty.X-145, (Dusty.Y - 200) + ScrollY, &RightFaceWallJump01 ); 
}

void UpdateDusty_WallJump_Left()
{
	if (Dusty.WallStickTimer == 0)
	{ 
		Dusty.VerticalCounter = 0;
		if (Dusty.CollideWithLeftSide == true)
		{
			Dusty.Y = Dusty.Y + Dusty.DirectionY;
			if (Dusty.CollideWithBottomSide == true)
			{				  
				SetDustyState_Stand();
				return;
			}
		}
		else
		{
			Dusty.LastDirectionSprite = 1;
			SetDustyState_Fall();
			return;
		}
	}

	if (Dusty.WallStickTimer != 0)
	{        
		Dusty.WallStickTimer -= 1;
	}       

	if (GetInput_Jump() && Dusty.LastDirectionSprite == 0)
	{
		Dusty.LastDirectionSprite = 1;
		Dusty.CanWallJump = true;
		SetDustyState_Jump( true );
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
    if (Dusty.LastDirectionSprite == 1)
    {
        gxDrawSprite( Dusty.X-125, (Dusty.Y - 221) + ScrollY, &DustyHop02 );
    }  
    
    if (Dusty.LastDirectionSprite == 0)
    {
        gxDrawSprite( Dusty.X-130, (Dusty.Y - 217) + ScrollY, &DustyHopLeft02 );
    }
}

void UpdateDusty_PrepareLaunch()
{


}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                  DUSTYSTATE_LAUNCH Implementation                                                       //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//


void SetDustyState_Launch()
{
    Dusty.State = DUSTYSTATE_LAUNCH;
}

void DisplayDusty_Launch()
{


}

void UpdateDusty_Launch()
{


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
	if (Dusty.X + Dusty.Right >= gxScreenWidth)
	{
		Dusty.CollideWithRightSide = true;
		Dusty.X = gxScreenWidth - Dusty.Right;	
	} 

	//Collision with the left side of the screen    
	if (Dusty.X + Dusty.Left <= 0)
	{
		Dusty.CollideWithLeftSide = true;
		Dusty.X = -Dusty.Left;
	}
	//Collision with the bottom side of the screen
	if (Dusty.Y + Dusty.Bottom >= Chapter.Pages[0].Height * 64 )
	{	
		Dusty.CollideWithBottomSide = true;
		Dusty.Y = Chapter.Pages[0].Height * 64 - Dusty.Bottom;
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
			int BlockLeft   = x*64;
			int BlockRight  = x*64 + 64;
			int BlockTop    = y*64;
			int BlockBottom = y*64 + 64;

			// Check to see if Dusty's rectangle overlaps with this block.  
			// The general idea here is to compare the smaller of the bottom sides (Min of Maxes) against the larger of the top sides (Max of Mins).  
			// If the min is less than the max, there is overlap.  The same calculation is also done in X.
			//
			// The Y axis is considered first because the level is larger on the Y axis, so this will reject most blocks earlier (code optimization).
			if (Max(Dusty.Y + Dusty.Top, BlockTop) <= Min(Dusty.Y + Dusty.Bottom, BlockBottom))
			{
				// Check to see if Dusty's rectangle overlaps with the block in X.
				if(Max(Dusty.X + Dusty.Left, BlockLeft) <= Min(Dusty.X + Dusty.Right, BlockRight))
				{    
					// These variables are used to prevent collisions with internal edges (edges between blocks).  If Dusty jumps up into the bottom of a wide platform
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
					// My solution is to detect when blocks have other blocks next to them.  When they do, the internal edges between blocks are not considered
					// when calculating collisions.
					//
					bool LeftBlockIsEmpty   = IsBlockEmpty(x-1, y);
					bool RightBlockIsEmpty  = IsBlockEmpty(x+1, y);
					bool TopBlockIsEmpty    = IsBlockEmpty(x, y-1);
					bool BottomBlockIsEmpty = IsBlockEmpty(x, y+1);

					// Calculate the distance Dusty would have to be pushed in each possible direction to get him out of intersecting the block.
					int LeftDistance	= (Dusty.X + Dusty.Right)  - (BlockLeft);
					int RightDistance	= (BlockRight) - (Dusty.X +  Dusty.Left);
					int DownDistance	= (BlockBottom)- (Dusty.Y +  Dusty.Top );
					int UpDistance		= (Dusty.Y + Dusty.Bottom) - (BlockTop );

					// Prefer to collide with the side of the block that would push Dusty out the least distance.
					// (Only consider sides that are not adjacent to another solid block).
					if (LeftBlockIsEmpty && LeftDistance < RightDistance && LeftDistance < DownDistance && LeftDistance < UpDistance)
					{
						Dusty.CollideWithRightSide = true;//Collision with Dusty's Right Side but the left side of the platform
						Dusty.X -= LeftDistance;
					}

					if (RightBlockIsEmpty && RightDistance < LeftDistance && RightDistance < DownDistance && RightDistance < UpDistance)
					{
						Dusty.CollideWithLeftSide = true;//Collision with Dusty's Left Side but the right side of the platform
						Dusty.X += RightDistance;
					}

					if (BottomBlockIsEmpty && DownDistance < RightDistance && DownDistance < LeftDistance && DownDistance < UpDistance)
					{
						Dusty.CollideWithTopSide = true;//Collision with Dusty's Top Side but the Bottom side of the platform
						Dusty.Y += DownDistance;
					}

					if (TopBlockIsEmpty && UpDistance < RightDistance && UpDistance < DownDistance && UpDistance < LeftDistance)
					{
						Dusty.CollideWithBottomSide = true;//Collision with Dusty's Bottom Side but the Top side of the platform
						Dusty.Y -= UpDistance;
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
	case DUSTYSTATE_HOP_RIGHT:          DisplayDusty_Hop_Right(); break;
	case DUSTYSTATE_HOP_LEFT:           DisplayDusty_Hop_Left(); break;
	case DUSTYSTATE_WALLJUMP_RIGHT:     DisplayDusty_WallJump_Right(); break;
	case DUSTYSTATE_WALLJUMP_LEFT:      DisplayDusty_WallJump_Left(); break;
	case DUSTYSTATE_PREPARELAUNCH:      DisplayDusty_PrepareLaunch(); break;
	case DUSTYSTATE_LAUNCH:             DisplayDusty_Launch(); break;	
	default:						    break;
	}

	// Draw a yellow + at Dusty's root location.
	gxDrawString(Dusty.X-4, Dusty.Y-4 + ScrollY, 8, gxRGB32(255, 255, 0), "+");

	// Draw a red + at all four corners of Dusty.
	gxDrawString(Dusty.X + Dusty.Left,  Dusty.Y + Dusty.Top    + ScrollY, 8, gxRGB32(255, 0, 0), "+");//Upper Left
	gxDrawString(Dusty.X + Dusty.Right, Dusty.Y + Dusty.Top    + ScrollY, 8, gxRGB32(255, 0, 0), "+");//Upper Right
	gxDrawString(Dusty.X + Dusty.Left,  Dusty.Y + Dusty.Bottom + ScrollY, 8, gxRGB32(255, 0, 0), "+");//Lower Left
	gxDrawString(Dusty.X + Dusty.Right, Dusty.Y + Dusty.Bottom + ScrollY, 8, gxRGB32(255, 0, 0), "+");//Lower Right
}

void UpdateDusty()
{
	UpdateDusty_Collision();

	switch (Dusty.State)
	{
	case DUSTYSTATE_STAND:			    UpdateDusty_Stand(); break;
	case DUSTYSTATE_JUMP:			    UpdateDusty_Jump(); break;
	case DUSTYSTATE_FALL:				UpdateDusty_Fall(); break;
	case DUSTYSTATE_HOP_RIGHT:          UpdateDusty_Hop_Right(); break;
	case DUSTYSTATE_HOP_LEFT:           UpdateDusty_Hop_Left(); break;
	case DUSTYSTATE_WALLJUMP_RIGHT:     UpdateDusty_WallJump_Right(); break;
	case DUSTYSTATE_WALLJUMP_LEFT:      UpdateDusty_WallJump_Left(); break;
	case DUSTYSTATE_PREPARELAUNCH:      UpdateDusty_PrepareLaunch(); break;
	case DUSTYSTATE_LAUNCH:             UpdateDusty_Launch(); break;
	default:						    break;
	}
}
