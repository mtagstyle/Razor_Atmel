/**********************************************************************************************************************
File: user_app2.c                                                                

Description:
This is a user_app2.c file template 

**********************************************************************************************************************/
#include "configuration.h"

#define MAX_BITS 8
#define COUNT_INTERVAL 500
/*=====================================================================================================================
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_UserApp2"
=====================================================================================================================*/
/* New variables */
volatile u32 G_u32UserApp2Flags;                       /* Global state flags */

/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */

/*=====================================================================================================================
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp2_" and be declared as static.
=====================================================================================================================*/
static fnCode_type   UserApp2_StateMachine;            /* The state machine function pointer */
static u16           msCounter;
static u8            bitCounter;
static LedNumberType ledPositions[MAX_BITS];

/*=====================================================================================================================
Public Functions
=====================================================================================================================*/

/*=====================================================================================================================
Protected Functions
=====================================================================================================================*/
/*
Function: UserApp2Initialize

Description:
Initializes the State Machine and its variables.

Requires:
  -

Promises:
  - 
*/
void UserApp2Initialize(void)
{
    /* Set the bit positions of each LED in our local data structure, from LEAST significant bit, to MOST significant bit */
    ledPositions[0] = RED;
    ledPositions[1] = ORANGE;
    ledPositions[2] = YELLOW;
    ledPositions[3] = GREEN;
    ledPositions[4] = CYAN;
    ledPositions[5] = BLUE;
    ledPositions[6] = PURPLE;
    ledPositions[7] = WHITE;

    /* Backlight to white */  
    LedOn(LCD_RED);
    LedOn(LCD_GREEN);
    LedOn(LCD_BLUE);

    /* Set our local timers to 0 */
    msCounter = 0;
    bitCounter = 0;
    
    /* Set all LED's in our bit counter to off */
    u8 i;
    for( i = 0; i < MAX_BITS; i++ )
    {
        LedOff(ledPositions[i]);
    }

    /* If good initialization, set state to Idle */
    if( 1 )
    {
        UserApp2_StateMachine = UserApp2SM_Idle;
    }
    else
    {
        /* The task isn't properly initialized, so shut it down and don't run */
        UserApp2_StateMachine = UserApp2SM_FailedInit;
    }
}

/*
Function UserApp2RunActiveState()

Description:
Selects and runs one iteration of the current state in the state machine.
All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
  - State machine function pointer points at current state

Promises:
  - Calls the function to pointed by the state machine function pointer
*/
void UserApp2RunActiveState(void)
{
  UserApp2_StateMachine();
}

/*=====================================================================================================================
Private Functions
=====================================================================================================================*/
static void UserApp2Helper_displayCount(uint8_t bit_value)
{
    u8 masked_bits = bit_value;

    // Iterate through each bit
    for(u8 i = 0; i < MAX_BITS; i++)
    {
        if(masked_bits & 0x01)
        {
            LedOn(ledPositions[i]);
        }
        else
        {
            LedOff(ledPositions[i]);
        }

        masked_bits = masked_bits >> 1;
    }
}

/*=====================================================================================================================
State Machine Function Definitions
=====================================================================================================================*/
static void UserApp2SM_Idle(void)
{
    if(msCounter == COUNT_INTERVAL)
    {
        UserApp2Helper_displayCount(bitCounter);
        bitCounter = (bitCounter + 1) % 256;
        msCounter = 0;
    }
    msCounter++;
}

/* Handle an error */
static void UserApp2SM_Error(void)          
{
  
} 

/* State to sit in if init failed */
static void UserApp2SM_FailedInit(void)          
{
    
}

/*=====================================================================================================================
End of File
=====================================================================================================================*/
