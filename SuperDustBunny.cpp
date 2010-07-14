//--------------------------------------------------------------------------------
//
// Super Dust Bunny
// 
// Authors: Thomas Perry <perry.thomas.12@gmail.com> & Wade Brainerd <wadetb@gmail.com>
// Copyright � 2010 by Thomas Perry and Wade Brainerd. All rights reserved.
//
//--------------------------------------------------------------------------------

#include "win/graphics.h"
#include "win/mouse.h"
#include "win/keyboard.h"
#include "win/sound.h"

bool SlowMotionMode = false;

int DustyX = 350;
int DustyY = 1024-55;
int DirectionX = 7;
int DirectionY = 7;
int DustyLeft = -10;
int DustyRight = 80;
int DustyTop = -80;
int DustyBottom = 55;
int VerticalCounter = 0;
bool IsJumping = false;
bool IsCollided = false;
int JumpQueue = 0;
int JumpRightSprite = 2;
int JumpLeftSprite = 2;
int WallStickTimer = 0;
int FallRightSprite = 2;
int FallLeftSprite = 2;
int HopRightSprite = 0;
int HopLeftSprite = 0;
int LastDirectionSprite = 1;
int SpriteTransition = 0;
int DustyState = 0;

gxSprite DustyHop01;
gxSprite DustyHop02;
gxSprite DustyHop03;
gxSprite DustyHop04;
gxSprite DustyHop05;
gxSprite LeftFaceStanding01;
gxSprite LeftFaceStanding02;
gxSprite RightFaceStanding01;
gxSprite RightFaceStanding02;
gxSprite LeftFaceWallJump01;
gxSprite RightFaceWallJump01;
gxSprite DustyDeath01;
gxSprite DustyHopLeft01;
gxSprite DustyHopLeft02;
gxSprite DustyHopLeft03;
gxSprite DustyHopLeft04;
gxSprite DustyHopLeft05;
gxSprite DustyHopLeft06;

sxSound DustyToJump;
sxSound DustyJumps;
sxSound WallJump;
sxSound BackgroundSong01;
sxSound BackgroundSong02;
sxSound BackgroundSong03;
int SongCounter = 1;
int SongDuration = 0;
int BackgroundMusic = 1;

void Init()
{
	// Use iPad "portrait mode" screen dimensions.
	gxInit(768, 1024, true);

	kbInit();
	
	sxInit();

	gxLoadSprite("Data/bunny hop0001.png", &DustyHop01);
	gxLoadSprite("Data/bunny hop0011.png", &DustyHop02);	
	gxLoadSprite("Data/bunny hop0005.png", &DustyHop03);
	gxLoadSprite("Data/bunny hop0006.png", &DustyHop04);
	gxLoadSprite("Data/bunny hop0009.png", &DustyHop05);	
	gxLoadSprite("Data/dustyhopleft0001.png", &LeftFaceStanding01);
	gxLoadSprite("Data/dustyhopleft0001.png", &LeftFaceStanding02);
	gxLoadSprite("Data/dustyhopleft0001.png",&DustyHopLeft01);
	gxLoadSprite("Data/dustyhopleft0011.png",&DustyHopLeft02);
	gxLoadSprite("Data/dustyhopleft0005.png",&DustyHopLeft03);
	gxLoadSprite("Data/dustyhopleft0006.png",&DustyHopLeft04);
	gxLoadSprite("Data/dustyhoplefttest.png",&DustyHopLeft06);
    gxLoadSprite("Data/DustyLeftWallJump.png", &LeftFaceWallJump01);
    gxLoadSprite("Data/DustyRightWallJump.png", &RightFaceWallJump01);
    
    sxLoadWav ("Data/yaahooo.wav", &DustyToJump);
    sxLoadWav ("Data/yaahooo.wav", &DustyJumps);
    sxLoadWav ("Data/cork_pop.wav", &WallJump);
    sxLoadWav ("Data/Song1.wav", &BackgroundSong01);
    sxLoadWav ("Data/Song2.wav", &BackgroundSong02);
    sxLoadWav ("Data/Song3.wav", &BackgroundSong03);
}

void Exit()
{
	kbDeinit();

	gxDeinit();
	
	sxDeinit();
}

void Display()
{      
	if (DustyState == 1) //Jumping
	{
		if (JumpRightSprite == 1)     
		{
			gxDrawSprite( DustyX-25, DustyY-63, &DustyHop03 );
		}

		if (FallRightSprite == 1)
		{
			gxDrawSprite( DustyX-25, DustyY-63, &DustyHop02 );
		}  

		if (JumpLeftSprite == 1)
		{
			gxDrawSprite( DustyX-23, DustyY-64, &DustyHopLeft03 );
		}

		if (FallLeftSprite == 1)
		{
			gxDrawSprite( DustyX-23, DustyY-64, &DustyHopLeft02 );
		}
	}
	
	if (DustyState == 2) //Moving Right
	{
		if (HopRightSprite == 1) 
		{
			gxDrawSprite( DustyX-32, DustyY-58, &DustyHop01 );
		} 

		if (HopRightSprite == 2)
		{
			gxDrawSprite( DustyX-28, DustyY-62, &DustyHop02 );
		} 

		if (HopRightSprite == 3)
		{
			gxDrawSprite( DustyX-25, DustyY-63, &DustyHop03 );
		}

		if (HopRightSprite == 4)
		{
			gxDrawSprite( DustyX-37, DustyY-46, &DustyHop04 );
		}    

		if (HopRightSprite == 5)
		{
			gxDrawSprite( DustyX-41, DustyY-74, &DustyHop05 );
		}     

		if (HopRightSprite == 6)
		{
			gxDrawSprite( DustyX-32, DustyY-58, &DustyHop01 );
		}               
	}

	if (DustyState == 3) //Moving Left
	{
		if (HopLeftSprite == 1) 
		{
			gxDrawSprite( DustyX-32, DustyY-59, &DustyHopLeft01 );
		} 

		if (HopLeftSprite == 2)
		{
			gxDrawSprite( DustyX-27, DustyY-60, &DustyHopLeft02 );
		} 

		if (HopLeftSprite == 3)
		{
			gxDrawSprite( DustyX-23, DustyY-64, &DustyHopLeft03 );
		}

		if (HopLeftSprite == 4)
		{
			gxDrawSprite( DustyX-34, DustyY-45, &DustyHopLeft04 );
		} 
		if (HopLeftSprite == 5)
		{
			gxDrawSprite( DustyX-41, DustyY-70, &DustyHopLeft06 );
		}    
		if (HopLeftSprite == 6)
		{
			gxDrawSprite( DustyX-32, DustyY-59, &DustyHopLeft01 );
		}              
	}
	
	if (DustyState == 4 && LastDirectionSprite == 1)//Collision with Right Wall
	{
            gxDrawSprite( DustyX-20, DustyY-72, &LeftFaceWallJump01 );       
	}
   
    if (DustyState == 5 && LastDirectionSprite == 0)//Collision with Left Wall
    {
            gxDrawSprite( DustyX-26, DustyY-71, &RightFaceWallJump01 ); 
    }

	if (DustyState == 0 && LastDirectionSprite == 0) //Standing
	{
		gxDrawSprite( DustyX-32, DustyY-59, &DustyHopLeft01 );
	}

	if (DustyState == 0 && LastDirectionSprite == 1)//After moving, stops to face the direction last moved.
	{
		gxDrawSprite( DustyX-32, DustyY-58, &DustyHop01 );
	}

	// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
	// Debugging aids
	// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

	// Status of common variables
	gxDrawString(5, 5, 16, gxRGB32(255, 255, 255), "DustyState: %d", DustyState);

	// Indicator for when slow motion is activated.
	if (SlowMotionMode)
	{
		gxDrawString(gxScreenWidth-101, 5, 16, gxRGB32(255, 255, 0), "[SLOW]");
	}

	// Draw a red + at Dusty's root location.
	gxDrawString(DustyX-4, DustyY-4, 8, gxRGB32(255, 0, 0), "+");
}

bool Update()
{
	kbUpdateKeys();
	
	//Background Music
if (BackgroundMusic == 1)
{	
	if (SongCounter == 1)
	{
        SongDuration = 500;//2580;    
        sxPlaySound(&BackgroundSong01);
    }
    
    if (SongDuration == 0)
    {
        sxDestroySound(&BackgroundSong01);
        sxDestroySound(&BackgroundSong02);
        sxDestroySound(&BackgroundSong03);
        SongCounter += 1;   
    }
    
    if (SongDuration != 0)
    {
        SongDuration -= 1;     
    }
        
    if (SongCounter == 2)
    {
         SongDuration = 500;//2580;
         sxPlaySound(&BackgroundSong02);
    }
    
    if (SongCounter == 3)
    {
         SongDuration = 500;//1952;
         sxPlaySound(&BackgroundSong03);
    }
    if (SongCounter >= 4)
    {
        SongCounter = 1;
    }    
}

	// Pressing escape quits the program.
	if (kbIsKeyDown(KB_ESCAPE))
	{
		return false;
	}

	// Backspace toggles slow motion mode.
	if (kbIsKeyDown(KB_BACKSLASH) && !kbWasKeyDown(KB_BACKSLASH))
	{
		SlowMotionMode = !SlowMotionMode;
	}

	if (SlowMotionMode)
	{
		bool StepOneFrame;
		if (kbIsKeyDown(KB_RETURN) && !kbWasKeyDown(KB_RETURN))
		{
			StepOneFrame = true;
		}
		else
		{
			StepOneFrame = false;
		}

		// If Return was not pressed, skip Update() for this frame.
		if (!StepOneFrame)
		{
			return true;
		}
	}

	if (DustyState == 0)//Standing Still, include Space bar check.
	{	        
        if (LastDirectionSprite == 1)
        {
            JumpRightSprite = 1;
            JumpLeftSprite = 0;
            //Reset Fall Sprites
            FallRightSprite = 0;
            FallLeftSprite = 0;
        }	   

        if (LastDirectionSprite == 0)
        {
            JumpLeftSprite = 1;
            JumpRightSprite = 0;
            //Reset Fall Sprites
            FallRightSprite = 0;
            FallLeftSprite = 0;
        }
        
		if ( kbIsKeyDown(KB_SPACE) && !kbWasKeyDown(KB_SPACE) )      
		{ 			
			VerticalCounter = 20;
			DustyY -= 10;
			DustyState = 1; //Jumping
		}

		if ( kbIsKeyDown(KB_D) )
		{
			DustyState = 2; //Moving Right
			HopRightSprite = 1;
			SpriteTransition = 30;
			LastDirectionSprite = 1;			  
		}

		if ( kbIsKeyDown(KB_A) )
		{
			DustyState = 3; //Moving Left
			HopLeftSprite = 1;
			SpriteTransition = 30;
			LastDirectionSprite = 0;   
		}
	}

	if (DustyState == 1)//Jumping
	{	      
        //Collide right side of screen check
        if (IsCollided == true && DustyX + DustyRight >= gxScreenWidth)
        {
           DustyX = gxScreenWidth - DustyRight;
        }
        
        //Collide left side of screen check
        if (IsCollided == true && DustyX + DustyLeft <= 0)
        {
           DustyX = -DustyLeft;
           //DustyX = DustyX + DirectionX;
           JumpRightSprite = 1;
           JumpLeftSprite = 0;                      
        }
        
        if (LastDirectionSprite == 1)
        {
          DustyX = DustyX + DirectionX;
          JumpRightSprite = 1;
          JumpLeftSprite = 0;        
          // Collision with right side of screen
          if (DustyX + DustyRight >= gxScreenWidth && IsCollided == false )
          {
              DustyX = gxScreenWidth - DustyRight;
              DustyState = 4;//Collide with Right wall. 
              JumpLeftSprite = 0;
              JumpRightSprite = 0;
              WallStickTimer = 15;
              IsCollided = true;
          }                                                      
        }

        if (LastDirectionSprite == 0)
        {          
          DustyX = DustyX - DirectionX;
          JumpLeftSprite = 1;
          JumpRightSprite = 0;         
          //Collision with the left side of the screen    
          if (DustyX + DustyLeft <= 0 && IsCollided == false)
          {
              DustyX = -DustyLeft;
              DustyState = 5;
              JumpLeftSprite = 0;
              JumpRightSprite = 0;
              WallStickTimer = 15;
              IsCollided = true;             
          }
                          
        }
        
		if (VerticalCounter == 0)
		{
			DustyY = DustyY + DirectionY;

			if (LastDirectionSprite == 1)
			{
				JumpRightSprite = 0;
				FallRightSprite = 1;
			}

			if (LastDirectionSprite == 0)
			{
				JumpLeftSprite = 0;
				FallLeftSprite = 1;
			}
		}
		else
		{     
			VerticalCounter -= 1;
			DustyY -= 10;
		}

		if (DustyY + DustyBottom > gxScreenHeight )
		{	
			DustyY = gxScreenHeight - DustyBottom;
			IsCollided = false;	
			DustyState = 0; //Standing
		}    
	}   

	if (DustyState == 2)//Moving right
	{
		DustyX = DustyX + DirectionX;  

		// Update animation
		SpriteTransition -= 1;

		// Collision with right side of screen
		if (DustyX + DustyRight >= gxScreenWidth )
			DustyX = gxScreenWidth - DustyRight;

		if ( !kbIsKeyDown(KB_D) && SpriteTransition == 0 )
		{			                         
			DustyState = 0; //Standing
		}

        if (kbIsKeyDown(KB_D) && kbIsKeyDown(KB_SPACE) && SpriteTransition != 0)
        {
            JumpQueue = 1;
            //sxPlaySound( &DustyToJump );
            //sxDestroySound( &DustyToJump ); 
        }
            
        if (JumpQueue == 1 && SpriteTransition == 0)
        {
            VerticalCounter = 20;
            DustyY -= 10;    
            LastDirectionSprite = 1;
            //reset JumpQueue
            JumpQueue = 0;
            //sxPlaySound( &DustyJumps );
            //sxDestroySound( &DustyJumps ); 
            DustyState = 1;
        }    
           
        if (kbIsKeyDown(KB_A))
        {
            LastDirectionSprite = 0;
            DustyState = 0;
        }
            
		if (SpriteTransition == 30)
		{
			HopRightSprite = 1;
		}  

		if (SpriteTransition == 25) 
		{
			HopRightSprite = 2;
		} 

		if (SpriteTransition == 20)
		{
			HopRightSprite = 3;
		} 

		if (SpriteTransition == 15)
		{
			HopRightSprite = 4;
		}

		if (SpriteTransition == 10)
		{
			HopRightSprite = 5;
		}

		if (SpriteTransition == 5)
		{
			HopRightSprite = 6;
		}

		if (SpriteTransition == 0)
		{
			SpriteTransition = 30;
		}	
	}	

	if (DustyState == 3)//Moving left
	{	
		DustyX = DustyX - DirectionX;	

		// Update animation
		SpriteTransition -= 1;

		// Collision with left side of screen
		if (DustyX + DustyLeft <= 0)
			DustyX = -DustyLeft;

		if ( !kbIsKeyDown(KB_A) && SpriteTransition == 0 )
		{          
			DustyState = 0; //Standing
		}

        if (kbIsKeyDown(KB_A) && kbIsKeyDown(KB_SPACE) && SpriteTransition != 0)
        {
            JumpQueue = 1;
            //sxPlaySound( &DustyToJump );
            //sxDestroySound( &DustyToJump ); 
        }

        if (JumpQueue == 1 && SpriteTransition == 0)
        {
            VerticalCounter = 20;
            DustyY -= 10;    
            LastDirectionSprite = 0;
            //Reset JumpQueue
            JumpQueue = 0;
            //sxPlaySound( &DustyJumps );
            //sxDestroySound( &DustyJumps ); 
            DustyState = 1;
        }  

        if (kbIsKeyDown(KB_D))
        {
            LastDirectionSprite = 1;
            DustyState = 0;
        }

		if (SpriteTransition == 30)
		{
			HopLeftSprite = 1;
		}  

		if (SpriteTransition == 25) 
		{
			HopLeftSprite = 2;
		} 

		if (SpriteTransition == 20)
		{
			HopLeftSprite = 3;
		} 

		if (SpriteTransition == 15)
		{
			HopLeftSprite = 4;
		}

		if (SpriteTransition == 10)
		{
			HopLeftSprite = 5;
		}

		if (SpriteTransition == 5)
		{
			HopLeftSprite = 6;
		}

		if (SpriteTransition == 0)
		{
			SpriteTransition = 30;
		}
	}

    if (DustyState == 4)//Collided with Right Wall
    {                                                  
            if (WallStickTimer == 0)
            {                   
                VerticalCounter = 0;          
                DustyState = 1;
            }
            
            if (WallStickTimer != 0)
            {                        
                WallStickTimer -= 1;
            }
  
        if (kbIsKeyDown(KB_SPACE) && LastDirectionSprite == 1)// && !kbWasKeyDown(KB_SPACE))
            {               
                LastDirectionSprite = 0;
                VerticalCounter = 20;//Move upward
                DustyY -= 10;
                sxPlaySound( &WallJump );
                //sxDestroySound( &WallJump );
                DustyState = 1;//Jumping
            }
    }   

    if (DustyState == 5)//Collided with Left Wall
    {                               
            if (WallStickTimer == 0)
            { 
                VerticalCounter = 0;
                DustyState = 1;
            }
            
            if (WallStickTimer != 0)
            {        
                WallStickTimer -= 1;
            }       
      
            if (kbIsKeyDown(KB_SPACE) && LastDirectionSprite == 0) //&& !kbWasKeyDown(KB_SPACE))
            {
                 LastDirectionSprite = 1;
                 //Resetting sprites
                 FallLeftSprite = 0;
                 JumpRightSprite = 1;
                 VerticalCounter = 20;//Move upward
                 DustyY -= 10;
                 sxPlaySound( &WallJump );
                 //sxDestroySound( &WallJump );
                 DustyState = 1;//Jumping
            }
    }

	return true;
}