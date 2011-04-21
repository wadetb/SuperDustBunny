#include "Common.h"
#include "Dusty.h"
#include "Tutorial.h"
#include "Chapter.h"
#include "Dust.h"
#include "Vacuum.h"
#include "Recorder.h"
#include "Settings.h"
#include "Stapler.h"


SDusty Dusty;
SStapler Stapler;


// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                  Dusty initialization function                                                          //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void InitDusty()
{
	Dusty.State = DUSTYSTATE_STAND; 

	Dusty.Direction = DIRECTION_RIGHT;

	Dusty.Left = -32;
	Dusty.Right = 32;
	Dusty.Top = -120;
	Dusty.Bottom = 0;

	Dusty.FloatVelocityX = 0.0f;
	Dusty.FloatVelocityY = 0.0f;
	
	Dusty.FloatGravity = 0.5f;

	Dusty.SpriteTransition = 5;

	Dusty.NoCollision = false;

	Dusty.WallStickTimer = 0;
	Dusty.LastWall = DIRECTION_NONE;
	Dusty.WallJumpTimer = 0;

    Dusty.JumpGraceTimer = 0;
    Dusty.AirJumpCount = 0;
    
	Dusty.GainLife = false;
	
	Dusty.CollideWithLeftSide = false;
	Dusty.CollideWithRightSide = false;
	Dusty.CollideWithTopSide = false;
	Dusty.CollideWithBottomSide = false;
};

void SetDustyPosition(float x, float y)
{
	Dusty.FloatX = x;
	Dusty.FloatY = y;
}


// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                  State switching function declarations                                                  //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void SetDustyState_Stand();
void SetDustyState_Jump( bool OffWall );
void SetDustyState_JumpWithVelocity( float VX, float VY );
void SetDustyState_Fall();
void SetDustyState_Hop( EDirection Direction );
void SetDustyState_WallJump();
void SetDustyState_CornerJump();
void SetDustyState_PrepareLaunch();
void SetDustyState_Launch();


void UpdateDusty_JumpCommon();

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                  General purpose control                                                                //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

bool UpdateDusty_CheckSwipeJump(float Angle, float Range, float Power)
{
    while (GetInput_SwipeValid())
    {
        if ( GetInput_CheckSwipeDir( Angle, Range ) )
        {
            float dX, dY;
            GetInput_NextSwipeDir(&dX, &dY);
            
            float L = Length(dX, dY);
            dX /= L;
            dY /= L;
            
            dX *= Power;
            dY *= Power;
            
            GetInput_ConsumeSwipe(Length(dX, dY));
            
            SetDustyState_JumpWithVelocity(dX, dY);
            
            AddDebugLine(Dusty.FloatX, Dusty.FloatY + ScrollY, Dusty.FloatX + cosf(DegreesToRadians(Angle+Range))*100, Dusty.FloatY + -sinf(DegreesToRadians(Angle+Range))*100 + ScrollY, gxRGB32(192, 192, 128), 0.5f);
            AddDebugLine(Dusty.FloatX, Dusty.FloatY + ScrollY, Dusty.FloatX + cosf(DegreesToRadians(Angle-Range))*100, Dusty.FloatY + -sinf(DegreesToRadians(Angle-Range))*100 + ScrollY, gxRGB32(192, 192, 128), 0.5f);
            
            AddDebugLine(Dusty.FloatX, Dusty.FloatY + ScrollY, Dusty.FloatX + dX*10, Dusty.FloatY + dY*10 + ScrollY, gxRGB32(192, 128, 128), 0.5f);
            
            return true;
        }
        
        GetInput_ConsumeSwipeSegment();
    }
    
    return false;
}

bool UpdateDusty_CheckSwipeDir(float Angle, float Range)
{
    if ( GetInput_CheckSwipeDir( Angle, Range ) )
    {
        AddDebugLine(Dusty.FloatX, Dusty.FloatY + ScrollY, Dusty.FloatX + cosf(DegreesToRadians(Angle+Range))*100, Dusty.FloatY + -sinf(DegreesToRadians(Angle+Range))*100 + ScrollY, gxRGB32(128, 128, 192), 0.5f);
        AddDebugLine(Dusty.FloatX, Dusty.FloatY + ScrollY, Dusty.FloatX + cosf(DegreesToRadians(Angle-Range))*100, Dusty.FloatY + -sinf(DegreesToRadians(Angle-Range))*100 + ScrollY, gxRGB32(128, 128, 192), 0.5f);
        
        //AddDebugLine(Dusty.FloatX, Dusty.FloatY + ScrollY, Dusty.FloatX + dX*10, Dusty.FloatY + dY*10 + ScrollY, gxRGB32(128, 128, 192), 0.5f);
        
        return true;
    }
    
    return false;
}


// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                  General purpose display                                                                //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void DisplayDustySprite(gxSprite* Sprite, float XAdj = 0.0f, float XMirrorAdj = 0.0f, float YAdj = 0.0f)
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

	AddLitSpriteScaled(LIGHTLIST_FOREGROUND, Sprite, Dusty.FloatX + OffsetX + XAdj + XMirrorAdj*ScaleX, Dusty.FloatY + YAdj + ScrollY, ScaleX, 1.0f);
}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                  DUSTYSTATE_STAND Implementation                                                        //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void SetDustyState_Stand()
{
	Dusty.LastWall = DIRECTION_NONE;

	if (Dusty.CollideMaterial != MATERIAL_ICE)
		Dusty.FloatVelocityX = 0;

	Dusty.FloatVelocityY = 0;

	Dusty.SpriteTransition = 0;	

    Dusty.AirJumpCount = 0;

	Dusty.State = DUSTYSTATE_STAND;
}

void DisplayDusty_Stand()
{
	if (Dusty.CollideMaterial == MATERIAL_ICE && fabsf(Dusty.FloatVelocityX) > 1)
	{
		gxSprite* SlideSprites[] =
		{
			&DustySlide1Sprite,
			&DustySlide2Sprite,
			&DustySlide3Sprite
		};

		int Index = (Dusty.SpriteTransition/5) % 3;
		DisplayDustySprite(SlideSprites[Index], -128, 5, -221);
	}
	else
	{
		if (Dusty.SpriteTransition <= 4)
			DisplayDustySprite(&DustyHop5Sprite, -119, 18, -218);
		else
		{
			if (Dusty.SpriteTransition % 40 < 10)
				DisplayDustySprite(&DustyIdle1Sprite, -124, 5, -221);
			else if (Dusty.SpriteTransition % 40 < 20)
				DisplayDustySprite(&DustyIdle2Sprite, -124, 5, -221);
			else
				DisplayDustySprite(&DustyIdle2Sprite, -124, 5, -221);
		}
	}
}

void UpdateDusty_Stand()
{
	// Check for hitting something sticky.
	if (Dusty.CollideMaterial == MATERIAL_STICKY)
	{
		SetDustyState_Stuck();
		return;
	}

	if (Dusty.CollideMaterial == MATERIAL_ICE)
	{
		Dusty.FloatX += Dusty.FloatVelocityX;
		Dusty.FloatVelocityX *= 0.9f;
	}
    
    if (Settings.ControlStyle == CONTROL_TILT)
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
    }
    else
    {
        // WTB - Testing a version without hopping, just jumping.
#ifdef ALLOW_HOP
        if ( UpdateDusty_CheckSwipeJump(90.0f, 90.0f, 10.0f) )
            return;
        
        if ( UpdateDusty_CheckSwipeDir(0.0f, 45.0f) )
        {
            SetDustyState_Hop(DIRECTION_RIGHT);
            return;
        }
        if ( UpdateDusty_CheckSwipeDir(180.0f, 45.0f) )
        {
            SetDustyState_Hop(DIRECTION_LEFT);
            return;
        }
#else
        if ( UpdateDusty_CheckSwipeJump(90.0f, 180.0f, 10.0f) )
            return;
#endif
    }
    
	if (Dusty.CollideWithBottomSide == false)
	{
		SetDustyState_Fall();
		return;
	}

	// If Dusty comes to a stand atop a corner, use the corner jump state to prevent him from floating
	// in the air.
	if (Dusty.CollideWithBottomLeftCorner)
	{
		Dusty.Direction = DIRECTION_LEFT;
		SetDustyState_CornerJump();
		return;
	}

	if (Dusty.CollideWithBottomRightCorner)
	{
		Dusty.Direction = DIRECTION_RIGHT;
		SetDustyState_CornerJump();
		return;
	}

	Dusty.SpriteTransition += 1;
}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                  DUSTYSTATE_JUMP Implementation                                                         //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void SetDustyState_Jump( bool OffWall )
{
	// Jump tutorial is kind of distracting right now.
	//if (Tutorial.JumpDisplayed == false)
	//{
	//	SetGameState_Crumb(TUTORIALSTATE_JUMP);
	//}

	if ( OffWall )
	{
		sxPlaySound( &DustyWallJumpSound );
	}
	else
	{
		//sxPlaySound( &DustyJumps );
	}

	Dusty.FloatY -= 40.0f;

	Dusty.FloatVelocityX = 6.0f;
	Dusty.FloatVelocityY = -16.0f;
	if (Dusty.Direction == DIRECTION_LEFT)
		Dusty.FloatVelocityX = -Dusty.FloatVelocityX;

	Dusty.State = DUSTYSTATE_JUMP;
}

void SetDustyState_JumpWithVelocity( float VX, float VY )
{
	Dusty.FloatY -= 40.0f;
    
	Dusty.FloatVelocityX = VX;
	Dusty.FloatVelocityY = VY;
    if (Dusty.FloatVelocityX < 0)
        Dusty.Direction = DIRECTION_LEFT;
    else
        Dusty.Direction = DIRECTION_RIGHT;
    
	Dusty.State = DUSTYSTATE_JUMP;
}

// This is a specialized way to get into the Jump state, without the initial velocity boost.
void SetDustyState_Fall()
{
	Dusty.State = DUSTYSTATE_JUMP;
    
    Dusty.JumpGraceTimer = 20;
}

void DisplayDusty_Jump()
{
	gxSprite* Hop2Sprites[3] =
	{
		&DustyHop2Sprite,
		&DustyHop2bSprite,
		&DustyHop2cSprite,
	};

	gxSprite* Hop4Sprites[3] =
	{
		&DustyHop4Sprite,
		&DustyHop4bSprite,
		&DustyHop4cSprite,
	};

	if (Dusty.FloatVelocityY >= 5)
		DisplayDustySprite(Hop4Sprites[(Dusty.SpriteTransition/5) % 3], -124, -18, -150);
	else if (Dusty.FloatVelocityY <= -5)
		DisplayDustySprite(Hop2Sprites[(Dusty.SpriteTransition/5) % 3], -124, -18, -150);
	else
		DisplayDustySprite(&DustyHop3Sprite, -124, -18, -140);
}

void UpdateDusty_Jump()
{       
	Dusty.FloatX = Dusty.FloatX + Dusty.FloatVelocityX;                                                   
    Dusty.FloatY += Dusty.FloatVelocityY;
      
	Dusty.FloatVelocityY += Dusty.FloatGravity;
   
	if (Dusty.FloatVelocityY > 15.0f)
		Dusty.FloatVelocityY = 15.0f;

    // Grace period jump.  Allows a midair jump within a few frames of falling.
    if (Settings.FallGracePeriod && Dusty.JumpGraceTimer > 0)
    {
        Dusty.JumpGraceTimer--;
        
        if (Settings.ControlStyle == CONTROL_TILT)
        {
            if (GetInput_Jump())
            {
                Dusty.JumpGraceTimer = 0;

                // Spawn some dust motes.
                for (int i = 0; i < 10; i++)
                    MakeDustMote(Dusty.FloatX, Dusty.FloatY - 50);
                
                SetDustyState_Jump( false );
                return;
            }
        }
        else
        {
#ifdef ALLOW_HOP
            if ( UpdateDusty_CheckSwipeJump(90.0f, 45.0f, 10.0f) )
            {
                Dusty.JumpGraceTimer = 0;
                
                // Spawn some dust motes.
                for (int i = 0; i < 10; i++)
                    MakeDustMote(Dusty.FloatX, Dusty.FloatY - 50);                    
                
                return;
            }
#endif
        }
    }
    
    // A single mid-air double jump.
    // TODO: Work out rules about when this can be activated.  Y velocity?  Timer?
    if (Settings.DoubleJump && Dusty.AirJumpCount == 0)
    {
        if (Settings.ControlStyle == CONTROL_TILT)
        {
            if (GetInput_Jump())
            {
                Dusty.AirJumpCount++;
                
                // Spawn some dust motes.
                for (int i = 0; i < 10; i++)
                    MakeDustMote(Dusty.FloatX, Dusty.FloatY - 50);

                SetDustyState_Jump( false );
                return;
            }
        }
        else
        {
            // SWIPE TODO
        }        
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
	case 0:    DisplayDustySprite(&DustyHop2Sprite, -122,  0, -201); break;
	case 1:    DisplayDustySprite(&DustyHop2Sprite, -122,  0, -212); break;
	case 2:    DisplayDustySprite(&DustyHop2Sprite, -122,  0, -218); break;
	case 3:    DisplayDustySprite(&DustyHop2Sprite, -122,  0, -224); break;
	case 4:    DisplayDustySprite(&DustyHop2Sprite, -122,  0, -228); break;
	case 5:    DisplayDustySprite(&DustyHop2Sprite, -122,  0, -236); break;
	case 6:    DisplayDustySprite(&DustyHop3Sprite, -122,  0, -226); break;
	case 7:    DisplayDustySprite(&DustyHop3Sprite, -122,  0, -226); break;
	case 8:    DisplayDustySprite(&DustyHop3Sprite, -122,  0, -226); break;
	case 9:    DisplayDustySprite(&DustyHop3Sprite, -122,  0, -226); break;
	case 10:   DisplayDustySprite(&DustyHop3Sprite, -122,  0, -226); break;
	case 11:   DisplayDustySprite(&DustyHop3Sprite, -122,  0, -226); break;
	case 12:   DisplayDustySprite(&DustyHop4Sprite, -119,  0, -220); break;
	case 13:   DisplayDustySprite(&DustyHop4Sprite, -119,  0, -215); break;
	case 14:   DisplayDustySprite(&DustyHop4Sprite, -119,  0, -210); break;
	case 15:   DisplayDustySprite(&DustyHop4Sprite, -119,  0, -205); break;
	case 16:   DisplayDustySprite(&DustyHop4Sprite, -119,  0, -200); break;
	case 17:   DisplayDustySprite(&DustyHop4Sprite, -119,  0, -195); break;
	case 18:   DisplayDustySprite(&DustyHop5Sprite, -119, 18, -217); break;
	case 19:   DisplayDustySprite(&DustyHop5Sprite, -119, 18, -217); break;
	case 20:   DisplayDustySprite(&DustyHop5Sprite, -119, 18, -217); break;
	case 21:   DisplayDustySprite(&DustyHop5Sprite, -119, 18, -217); break;
	case 22:   DisplayDustySprite(&DustyHop5Sprite, -119, 18, -217); break;
	case 23:   DisplayDustySprite(&DustyHop5Sprite, -119, 18, -217); break;
	default:   break;
	}
}

void UpdateDusty_Hop()
{
	// Check for hitting something sticky.
	if (Dusty.CollideMaterial == MATERIAL_STICKY)
	{
		SetDustyState_Stuck();
		return;
	}

    // Set Velocity first.
    float VelocitySign = Dusty.Direction == DIRECTION_LEFT ? -1.0f : 1.0f;
    
	if (Dusty.CollideMaterial == MATERIAL_ICE)
		Dusty.FloatVelocityX = 6.0f * VelocitySign;
	else
		Dusty.FloatVelocityX = 8.25f * VelocitySign;
    
	// Jump immediately if requested.
    if (Settings.ControlStyle == CONTROL_TILT)
    {
        if (GetInput_Jump())
        {
            SetDustyState_Jump( false );
        }
    }
    else
    {
#ifdef ALLOW_HOP
        if ( UpdateDusty_CheckSwipeJump(90.0f, 45.0f, 10.0f) )
            return;
        
        float VX, VY;
        GetInput_NextSwipeDir(&VX, &VY);

        // Change hop direction.
        if ( Dusty.Direction == DIRECTION_LEFT )
        {
            if (UpdateDusty_CheckSwipeDir(0.0f, 45.0f))
            {
                Dusty.Direction = DIRECTION_RIGHT;
                Dusty.FloatVelocityX = -Dusty.FloatVelocityX;
            }
        }
        if ( Dusty.Direction == DIRECTION_RIGHT )
        {
            if (UpdateDusty_CheckSwipeDir(180.0f, 45.0f))
            {
                Dusty.Direction = DIRECTION_LEFT;
                Dusty.FloatVelocityX = -Dusty.FloatVelocityX;
            }
        }
#endif
    }

	// Update animation
	Dusty.SpriteTransition += 1;

	// Check for end of hop animation.
	if (Dusty.SpriteTransition == 24)
	{
		// Spawn some dust motes.
		for (int i = 0; i < 6; i++)
			MakeDustMote(Dusty.FloatX, Dusty.FloatY);

        if (Settings.ControlStyle == CONTROL_TILT)
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
        else
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
    
	// Change direction mid hop.
    if (Settings.ControlStyle == CONTROL_TILT)
    {
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
	Dusty.SpriteTransition++;

	// Air control.
    if (Settings.ControlStyle == CONTROL_TILT)
    {
        if (GetInput_MoveLeft())
        {
            if (Dusty.FloatVelocityX >= -6)
                Dusty.FloatVelocityX -= 1.0f;
        }
        if (GetInput_MoveRight())
        {
            if (Dusty.FloatVelocityX <= 6)
                Dusty.FloatVelocityX += 1.0f;
        }
    }
    else
    {
        float dX, dY;
        GetInput_NextSwipeDir(&dX, &dY);
        
        float L = sqrtf(dX*dX + dY*dY);
        if (L > 5.0f)
        {
            if (fabsf(Dusty.FloatVelocityX) < 10)
                Dusty.FloatVelocityX += dX / 20.0f;
            if (fabsf(Dusty.FloatVelocityY) < 10)
                Dusty.FloatVelocityY += dY / 20.0f;
        }
        
        GetInput_ConsumeSwipe(1.5f*Length(Dusty.FloatVelocityX, Dusty.FloatVelocityY));
    }

    if (Dusty.FloatVelocityX > 0)
        Dusty.Direction = DIRECTION_RIGHT;
    else if (Dusty.FloatVelocityX < 0)
        Dusty.Direction = DIRECTION_LEFT;

	// Collision with corners is indicated by separate collision variables being set.
	if (Dusty.CollideWithBottomLeftCorner)
	{
		Dusty.Direction = DIRECTION_LEFT;
		SetDustyState_CornerJump();
		return;
	}
	if (Dusty.CollideWithBottomRightCorner)
	{
		Dusty.Direction = DIRECTION_RIGHT;
		SetDustyState_CornerJump();
		return;
	}

	// Collision with either side translates to a possible wall jump.
	// Dusty is not allowed to collide with the same side twice in a row, unless he stands once in between.
	// Wade: Currently this stuff is tweaked around as an experiment- he can only walljump again after a delay.
	Dusty.WallJumpTimer++;

	if (Dusty.CollideWithLeftSide && Dusty.Direction == DIRECTION_LEFT && (Dusty.WallJumpTimer >= 30 || Dusty.LastWall != DIRECTION_LEFT))
	{
		// Spawn some dust motes.
		for (int i = 0; i < 6; i++)
			MakeDustMote(Dusty.FloatX, Dusty.FloatY - 50);

        SetDustyState_WallJump();
        return;
	}

	if (Dusty.CollideWithRightSide && Dusty.Direction == DIRECTION_RIGHT && (Dusty.WallJumpTimer >= 30 || Dusty.LastWall != DIRECTION_RIGHT))
    {
		// Spawn some dust motes.
		for (int i = 0; i < 6; i++)
			MakeDustMote(Dusty.FloatX, Dusty.FloatY - 50);

		SetDustyState_WallJump();
        return;
    }

	// When landing on something, revert to standing.
	if (Dusty.CollideWithBottomSide == true )
	{	
		// Spawn some dust motes.
		for (int i = 0; i < 3; i++)
			MakeDustMote(Dusty.FloatX, Dusty.FloatY);

		SetDustyState_Stand();
		return;
	} 
}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                  DUSTYSTATE_WALLJUMP Implementation                                               //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void SetDustyState_WallJump()
{   	
	if (Tutorial.WallJumpDisplayed == false)
	{
		SetGameState_Tutorial(TUTORIALSTATE_WALLJUMP);
	}

	if (Dusty.CollideMaterial == MATERIAL_ICE)
		Dusty.WallStickTimer = 0;
	else
		Dusty.WallStickTimer = 15;

	Dusty.FloatVelocityX = 0;
	Dusty.FloatVelocityY = 0;

	Dusty.LastWall = Dusty.Direction;

	Dusty.WallJumpTimer = 0;
    Dusty.AirJumpCount = 0;

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

	DisplayDustySprite(&DustyWallJumpSprite, -128, -8, -200);
}

void UpdateDusty_WallJump()
{
	// Check for hitting something sticky.
	if (Dusty.CollideMaterial == MATERIAL_STICKY)
	{
		SetDustyState_Stuck();
		return;
	}

	if (Dusty.WallStickTimer > 0)
		Dusty.WallStickTimer--;

	if (Dusty.WallStickTimer <= 0)
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

			if (Dusty.CollideMaterial == MATERIAL_ICE)
			{
				Dusty.FloatVelocityY += Dusty.FloatGravity;

				if (Dusty.FloatVelocityY > 15.0f)
					Dusty.FloatVelocityY = 15.0f;
			}
			else
			{
				Dusty.FloatVelocityY += Dusty.FloatGravity/2;

				if (Dusty.FloatVelocityY > 7.0f)
					Dusty.FloatVelocityY = 7.0f;
			}
		}
	}

	// Jump off wall by pressing jump
    if (Settings.ControlStyle == CONTROL_TILT)
    {
        if (GetInput_Jump())
        {
            SetDustyState_Jump( true );
            return;
        }

        // Let go of wall by pressing direction.
        // TODO: GetInput_HardMoveLeft/Right.
        if ( ( Dusty.Direction == DIRECTION_LEFT  && GetInput_MoveLeft() ) ||
             ( Dusty.Direction == DIRECTION_RIGHT && GetInput_MoveRight() ) )
        {
            SetDustyState_Fall();
            return;
        }
    }
    else
    {
#ifdef ALLOW_HOP
        // Swipe down to fall.
        if ( UpdateDusty_CheckSwipeDir( -90.0f, 20.0f ) )
        {            
            SetDustyState_Fall();
            return;                
        }
        
        if ( Dusty.Direction == DIRECTION_RIGHT )
        {
            if ( UpdateDusty_CheckSwipeJump(0.0f, 45.0f, 10.0f) )
                return;
        }
        
        if ( Dusty.Direction == DIRECTION_LEFT )
        {
            if ( UpdateDusty_CheckSwipeJump(180.0f, 45.0f, 10.0f) )
                return;
        }
#else
        if ( Dusty.Direction == DIRECTION_RIGHT )
        {
            if ( UpdateDusty_CheckSwipeJump(0.0f, 90.0f, 10.0f) )
                return;
        }
        
        if ( Dusty.Direction == DIRECTION_LEFT )
        {
            if ( UpdateDusty_CheckSwipeJump(180.0f, 90.0f, 10.0f) )
                return;
        }        
#endif
    }
}	

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                  DUSTYSTATE_CORNERJUMP Implementation                                               //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void SetDustyState_CornerJump()
{   	
	Dusty.FloatVelocityX = 0;
	Dusty.FloatVelocityY = 0;

	Dusty.LastWall = Dusty.Direction;

	Dusty.WallJumpTimer = 0;
    Dusty.AirJumpCount = 0;
    
	Dusty.SpriteTransition = 0;

	// Switch directions when entering a corner jump.
	if (Dusty.Direction == DIRECTION_RIGHT)
		Dusty.Direction = DIRECTION_LEFT;
	else
		Dusty.Direction = DIRECTION_RIGHT;

	Dusty.State = DUSTYSTATE_CORNERJUMP;
}

void DisplayDusty_CornerJump()
{
	DisplayDustySprite(&DustyCornerJumpSprite, -128, -32, -150);
}

void UpdateDusty_CornerJump()
{                                 
	// Force Dusty to pause in Corner jump for a few frames.
	Dusty.SpriteTransition += 1;
	if (Dusty.SpriteTransition <= 5)
		return;

	// Jump off wall by pressing jump
    if (Settings.ControlStyle == CONTROL_TILT)
    {
        if (GetInput_Jump())
        {
            SetDustyState_Jump( true );
            return;
        }

        // Let go of wall by pressing direction away from corner.
        if ( ( Dusty.Direction == DIRECTION_LEFT  && GetInput_MoveLeft() ) ||
             ( Dusty.Direction == DIRECTION_RIGHT && GetInput_MoveRight() ) )
        {
            SetDustyState_Fall();
            return;
        }

        // Get up and walk by pressing direction towards corner.
        if ( ( Dusty.Direction == DIRECTION_LEFT  && GetInput_MoveRight() ) ||
             ( Dusty.Direction == DIRECTION_RIGHT && GetInput_MoveLeft() ) )
        {
            EDirection NewDirection;

            // Help him along to prevent going back into corner jump.
            if (Dusty.Direction == DIRECTION_LEFT)
            {
                NewDirection = DIRECTION_RIGHT;
                Dusty.FloatX += 48;
            }
            else
            {
                NewDirection = DIRECTION_LEFT;
                Dusty.FloatX -= 48;
            }

            SetDustyState_Hop(NewDirection);
            return;
        }
    }
    else
    {
        // Let go of wall by pressing direction away from corner.
        // Get up and walk by pressing direction towards corner.
        if ( Dusty.Direction == DIRECTION_LEFT )
        {            
#ifdef ALLOW_HOP
            if ( UpdateDusty_CheckSwipeJump(135.0f, 45.0f, 10.0f) )
                return;
            if ( UpdateDusty_CheckSwipeJump(90.0f, 45.0f, 10.0f) )
                return;
            
            if ( UpdateDusty_CheckSwipeDir(225.0f, 45.0f) )
            {
                Dusty.FloatVelocityX = -5;
                
                SetDustyState_Fall();
                return;                
            }
            if (UpdateDusty_CheckSwipeDir(0.0f, 45.0f) )
            {
                Dusty.FloatX += 48;                
                
                SetDustyState_Hop(DIRECTION_RIGHT);
                return;
            }
#else
            if ( UpdateDusty_CheckSwipeJump(135.0f, 135.0f, 10.0f) )
                return;
#endif
        }
        else
        {
#ifdef ALLOW_HOP
            if ( UpdateDusty_CheckSwipeJump(45.0f, 45.0f, 10.0f) )
                return;
            if ( UpdateDusty_CheckSwipeJump(90.0f, 45.0f, 10.0f) )
                return;
            
            if ( UpdateDusty_CheckSwipeDir(-45.0f, 45.0f) )
            {
                Dusty.FloatVelocityX = 5;
                
                SetDustyState_Fall();
                return;                                
            }
            if ( UpdateDusty_CheckSwipeDir(180.0f, 45.0f) )
            {
                Dusty.FloatX -= 48;                
                
                SetDustyState_Hop(DIRECTION_LEFT);
                return;                
            }
#else
            if ( UpdateDusty_CheckSwipeJump(45.0f, 135.0f, 10.0f) )
                return;
#endif
        }
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
	if (Dusty.SpriteTransition <= 4)
		DisplayDustySprite(&DustyHop5Sprite, -119, 18, -218);
	else
	{
		if (Dusty.SpriteTransition % 40 < 10)
			DisplayDustySprite(&DustyIdle1Sprite, -124, 5, -221);
		else if (Dusty.SpriteTransition % 40 < 20)
			DisplayDustySprite(&DustyIdle2Sprite, -124, 5, -221);
		else
			DisplayDustySprite(&DustyIdle2Sprite, -124, 5, -221);
	}
}

void UpdateDusty_PrepareLaunch()
{
    while (GetInput_SwipeValid())
        GetInput_ConsumeSwipeSegment();
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
	DisplayDusty_Jump();
}

void UpdateDusty_Launch()
{
	Dusty.FloatX += Dusty.FloatVelocityX;
	Dusty.FloatY += Dusty.FloatVelocityY;
	
	Dusty.FloatVelocityY += Dusty.FloatGravity * 0.75f; // Reduced gravity when launching.

	if (Dusty.FloatVelocityY > 15.0f)
		Dusty.FloatVelocityY = 15.0f;

	UpdateDusty_JumpCommon();
}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                  DUSTYSTATE_DIE Implementation                                                          //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void SetDustyState_Die()
{
	Dusty.FloatVelocityX = Dusty.FloatX > (float)gxScreenWidth/2 ? -15.0f : 15.0f;
	Dusty.FloatVelocityY = -30;

	Dusty.SpriteTransition = 0;

	//sxPlaySound( &DieSound );    

	Dusty.NoCollision = true;

	Dusty.State = DUSTYSTATE_DIE;
}

void DisplayDusty_Die()
{
	DisplayDustySprite(&DustyDeathSprite, -125, 0, -181);
}

void UpdateDusty_Die()
{
	Dusty.SpriteTransition += 1;

	Dusty.FloatX += Dusty.FloatVelocityX;
	Dusty.FloatY += Dusty.FloatVelocityY;

	Dusty.FloatVelocityY += Dusty.FloatGravity;

	float DirX, DirY;
	GetVacuumForce(Dusty.FloatX, Dusty.FloatY, &DirX, &DirY, (float)Dusty.SpriteTransition/400.0f);
	Dusty.FloatVelocityX += DirX;
	Dusty.FloatVelocityY += DirY;

	Dusty.FloatVelocityX *= 0.99f;
	Dusty.FloatVelocityY *= 0.99f;

	if (Distance(Dusty.FloatX, Dusty.FloatY, (float)gxScreenWidth/2, Vacuum.Y) < 40.0f)
	{
		if (Dusty.Lives > 0) //Check before the Die Screen Transition
		{
            if (!Settings.InfiniteLives)
                Dusty.Lives -= 1;
			StopRecording(RESULT_RESTART_PAGE);
			SetGameState_Transition(GAMETRANSITION_RESTART_PAGE);
		}
		else
		{
			StopRecording(RESULT_DIE);
			SetGameState_Transition(GAMETRANSITION_DIE_SCREEN);
		}
		return;
	} 
}


// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                  DUSTYSTATE_STUCK Implementation                                                        //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void SetDustyState_Stuck()
{
	Dusty.StuckTimer = 30;
	Dusty.StuckJumpCount = 0;

	Dusty.State = DUSTYSTATE_STUCK;
}

void DisplayDusty_Stuck()
{
	// TODO: Get sprites for attempting to escape stickiness, and use them when StuckTimer >= 20 or so.

	if (Dusty.CollideWithLeftSide || Dusty.CollideWithRightSide)
	{
		DisplayDustySprite(&DustyWallJumpSprite, -128, -8, -200);
	}
	else
	{
		DisplayDustySprite(&DustyIdle3Sprite, -124, 5, -221);
	}
}

void UpdateDusty_Stuck()
{
	if (Dusty.StuckTimer > 0)
	{
		Dusty.StuckTimer--;
		return;
	}

    if (Settings.ControlStyle == CONTROL_TILT)
    {
        if (GetInput_Jump())
        {
            Dusty.StuckJumpCount++;

            if (Dusty.StuckJumpCount == 2)
            {
                SetDustyState_Jump(false);
                return;
            }

            Dusty.StuckTimer = 30;
        }
    }
    else
    {
        // SWIPE TODO
    }
}

void SetDustyState_Hurt()
{   
    Dusty.SpriteTransition = 0;

	if (Dusty.CollideWithBottomSide)
	{
		Dusty.FloatVelocityY = -15;
		Dusty.FloatY += -40;
		Dusty.CollideWithBottomSide = false;
	}

	Dusty.State = DUSTYSTATE_HURT;
}

void DisplayDusty_Hurt()
{       
	// TODO: Need new sprite
	DisplayDusty_Die();
}

void UpdateDusty_Hurt()
{
	Dusty.FloatX = Dusty.FloatX + Dusty.FloatVelocityX;                                                   
	Dusty.FloatY += Dusty.FloatVelocityY;

	Dusty.FloatVelocityY += Dusty.FloatGravity;

	if (Dusty.FloatVelocityY > 15.0f)
		Dusty.FloatVelocityY = 15.0f;

	// If Dusty lands or the timer expires, revert to stand state.
	Dusty.SpriteTransition += 1;
    if (Dusty.SpriteTransition >= 60 || Dusty.CollideWithBottomSide)
    {
		SetDustyState_Stand();
		return;
    }
}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                  DustyState_StaplerLaunch() Implementation                                              //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void SetDustyState_StaplerLaunch()
{
	Dusty.State = DUSTYSTATE_STAPLERLAUNCH;
}

void DisplayDusty_StaplerLaunch()
{
	DisplayDusty_Jump();
}

void UpdateDusty_StaplerLaunch()
{
    Dusty.FloatY -= 40.0f;
    Dusty.FloatVelocityY = -32.0f;
    //Dusty.FloatVelocityY = Stapler.PowerJump;
	UpdateDusty_JumpCommon();
}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//
//                                                  UpdateDusty_Collision Implementation                                                   //
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -//

void UpdateDusty_Collision()
{
	// Initialize all collision variables to false.  One or more of these will be set to true in this function.
	// This function also corrects Dusty's position to not intersect anything.
	Dusty.CollideWithRightSide = false;
	Dusty.CollideWithLeftSide = false;
	Dusty.CollideWithBottomSide = false;
	Dusty.CollideWithTopSide = false;

	UpdateStapler_Collision();

	Dusty.CollideWithBottomLeftCorner = false;
	Dusty.CollideWithBottomRightCorner = false;

	Dusty.CollideMaterial = MATERIAL_NORMAL;

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
	if (Dusty.FloatY + Dusty.Bottom >= Chapter.PageHeight * 64 )
	{	
		Dusty.CollideWithBottomSide = true;
		Dusty.FloatY = (float)Chapter.PageHeight * 64 - (float)Dusty.Bottom;
	} 

	for (int y = 0; y < Chapter.PageHeight; y++)
	{
		for (int x = 0; x < Chapter.PageWidth; x++)
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
				// Check to see if Dusty's rectangle also overlaps with the block in X.
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

					bool BlockCollideWithLeftSide = false;
					bool BlockCollideWithRightSide = false;
					bool BlockCollideWithTopSide = false;
					bool BlockCollideWithBottomSide = false;

					int CornerThreshold = 32;

					// Prefer to collide with the side of the block that would push Dusty out the least distance.
					// (Only consider sides that are not adjacent to another solid block).
					if (LeftBlockIsEmpty && LeftDistance < RightDistance && LeftDistance < DownDistance && LeftDistance < UpDistance)
					{
						BlockCollideWithRightSide = true;
						Dusty.CollideWithRightSide = true;//Collision with Dusty's Right Side but the left side of the platform
						Dusty.FloatX -= LeftDistance;

						if (Dusty.FloatVelocityX > 0 && TopBlockIsEmpty && abs(UpDistance - LeftDistance) < CornerThreshold)
						{
 							Dusty.CollideWithBottomRightCorner = true;
							Dusty.FloatY -= UpDistance;
							Dusty.FloatVelocityY = 0;
						}

						if (Dusty.FloatVelocityX > 0)
							Dusty.FloatVelocityX = 0;
					}
					if (RightBlockIsEmpty && RightDistance < LeftDistance && RightDistance < DownDistance && RightDistance < UpDistance)
					{
						BlockCollideWithLeftSide = true;
						Dusty.CollideWithLeftSide = true;//Collision with Dusty's Left Side but the right side of the platform
						Dusty.FloatX += RightDistance;

						if (Dusty.FloatVelocityX < 0 && TopBlockIsEmpty && abs(UpDistance - RightDistance) < CornerThreshold)
						{
							Dusty.CollideWithBottomLeftCorner = true;
							Dusty.FloatY -= UpDistance;
							Dusty.FloatVelocityY = 0;
						}

						if (Dusty.FloatVelocityX < 0)
							Dusty.FloatVelocityX = 0;
					}
					if (BottomBlockIsEmpty && DownDistance < RightDistance && DownDistance < LeftDistance && DownDistance < UpDistance)
					{
						BlockCollideWithTopSide = true;
						Dusty.CollideWithTopSide = true;//Collision with Dusty's Top Side but the Bottom side of the platform
						Dusty.FloatY += DownDistance;
						if (Dusty.FloatVelocityY < 0)
							Dusty.FloatVelocityY = 0;
					}
					if (TopBlockIsEmpty && UpDistance < RightDistance && UpDistance < DownDistance && UpDistance < LeftDistance)
					{
						BlockCollideWithBottomSide = true;
						Dusty.CollideWithBottomSide = true;//Collision with Dusty's Bottom Side but the Top side of the platform
						Dusty.FloatY -= UpDistance;
						if (Dusty.FloatVelocityY > 0)
							Dusty.FloatVelocityY = 0;

						// Top side corner jump is back in the game.
						if (LeftBlockIsEmpty && abs(LeftDistance - UpDistance) < CornerThreshold)
						{
							Dusty.CollideWithBottomRightCorner = true;
							Dusty.FloatX -= LeftDistance;
						}
						if (RightBlockIsEmpty && abs(RightDistance - UpDistance) < CornerThreshold)
						{
							Dusty.CollideWithBottomLeftCorner = true;
							Dusty.FloatX += RightDistance;
						}
					}

					int BlockID = GetBlockID(x, y);
					if (BlockID < SPECIALBLOCKID_FIRST)
					{			
						SBlock* Block = &Chapter.Blocks[BlockID];
												        						
						if (Dusty.CollideMaterial == MATERIAL_NORMAL)
						{
							Dusty.CollideMaterial = Block->Material;
						}

						if (Block->Type == BLOCKTYPE_NAIL && Dusty.State != DUSTYSTATE_HURT)
						{
							int SharpDir = ((SNailProperties*)Block->Properties)->Dir;

							bool Sharp = false;
							if (BlockCollideWithBottomSide && SharpDir ==   0) Sharp = true;
							if (BlockCollideWithLeftSide   && SharpDir ==  90) Sharp = true;
							if (BlockCollideWithTopSide    && SharpDir == 180) Sharp = true;
							if (BlockCollideWithRightSide  && SharpDir == 270) Sharp = true;

							if (Sharp)
							{
								SetDustyState_Hurt();
							}
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
	case DUSTYSTATE_HOP:			    DisplayDusty_Hop(); break;
	case DUSTYSTATE_WALLJUMP:			DisplayDusty_WallJump(); break;
	case DUSTYSTATE_CORNERJUMP:			DisplayDusty_CornerJump(); break;
	case DUSTYSTATE_PREPARELAUNCH:      DisplayDusty_PrepareLaunch(); break;
	case DUSTYSTATE_LAUNCH:             DisplayDusty_Launch(); break;	
	case DUSTYSTATE_DIE:				DisplayDusty_Die(); break;
	case DUSTYSTATE_STUCK:				DisplayDusty_Stuck(); break;
	case DUSTYSTATE_HURT:				DisplayDusty_Hurt(); break;
	case DUSTYSTATE_STAPLERLAUNCH:		DisplayDusty_StaplerLaunch(); break;
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
	if (!Dusty.NoCollision)
	{
		UpdateDusty_Collision();
	}
		
	if (Dusty.State != DUSTYSTATE_DIE && Distance(Dusty.FloatX, Dusty.FloatY, Chapter.EndX, Chapter.EndY) < 100)
	{
		AdvanceToNextPage();
		return;
	}

	switch (Dusty.State)
	{
	case DUSTYSTATE_STAND:			    UpdateDusty_Stand(); break;
	case DUSTYSTATE_JUMP:			    UpdateDusty_Jump(); break;
	case DUSTYSTATE_HOP:				UpdateDusty_Hop(); break;
	case DUSTYSTATE_WALLJUMP:			UpdateDusty_WallJump(); break;
	case DUSTYSTATE_CORNERJUMP:			UpdateDusty_CornerJump(); break;
	case DUSTYSTATE_PREPARELAUNCH:      UpdateDusty_PrepareLaunch(); break;
	case DUSTYSTATE_LAUNCH:             UpdateDusty_Launch(); break;
	case DUSTYSTATE_DIE:				UpdateDusty_Die(); break;
	case DUSTYSTATE_STUCK:				UpdateDusty_Stuck(); break;
	case DUSTYSTATE_HURT:				UpdateDusty_Hurt(); break;
	case DUSTYSTATE_STAPLERLAUNCH:		UpdateDusty_StaplerLaunch(); break;
	}
}
