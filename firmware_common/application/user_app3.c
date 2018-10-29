/**********************************************************************************************************************
File: user_app3.c                                                                

Description:
This is a user_app3.c file template 

------------------------------------------------------------------------------------------------------------------------
API:

Public functions:


Protected System functions:
void UserApp3Initialize(void)
Runs required initialzation for the task.  Should only be called once in main init section.

void UserApp3RunActiveState(void)
Runs current task state.  Should only be called once in main loop.


**********************************************************************************************************************/
#include "configuration.h"

#define COLOR_CYCLE_TIME   (u16)60    /* Time to hold each color */

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_UserApp3"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserApp3Flags;                       /* Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp3_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp3_StateMachine;            /* The state machine function pointer */
static LedNumberType aeCurrentLed[]  = {LCD_GREEN, LCD_RED, LCD_BLUE, LCD_GREEN, LCD_RED, LCD_BLUE};
static bool abLedRateIncreasing[]   =  {TRUE,      FALSE,   TRUE,     FALSE,     TRUE,    FALSE};

static u8 u8CurrentLedIndex  = 0;
static u8 u8LedCurrentLevel  = 0;
static u8 u8DutyCycleCounter = 0;
static u16 u16Counter = COLOR_CYCLE_TIME;
/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------
Function: UserApp3Initialize

Description:
Initializes the State Machine and its variables.

Requires:
  -

Promises:
  - 
*/
void UserApp3Initialize(void)
{
  /* Start with red LED on 100%, green and blue off */
  LedPWM(LCD_RED, LED_PWM_100);
  LedPWM(LCD_GREEN, LED_PWM_0);
  LedPWM(LCD_BLUE, LED_PWM_0);

  /* If good initialization, set state to Idle */
  if( 1 )
  {
    UserApp3_StateMachine = UserApp3SM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp3_StateMachine = UserApp3SM_FailedInit;
  }

} /* end UserApp3Initialize() */

  
/*----------------------------------------------------------------------------------------------------------------------
Function UserApp3RunActiveState()

Description:
Selects and runs one iteration of the current state in the state machine.
All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
  - State machine function pointer points at current state

Promises:
  - Calls the function to pointed by the state machine function pointer
*/
void UserApp3RunActiveState(void)
{
  UserApp3_StateMachine();

} /* end UserApp3RunActiveState */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/
static void UserApp3_UpdatePulseState(void)
{
    /* Update the current level based on which way it's headed */
    if( abLedRateIncreasing[u8CurrentLedIndex] )
    {
        u8LedCurrentLevel++;
    }
    else
    {
        u8LedCurrentLevel--;
    }

    /* Change direction once we're at the end */
    u8DutyCycleCounter++;
    if(u8DutyCycleCounter == 20)
    {
        u8DutyCycleCounter = 0;

        /* Watch for the indexing variable to reset */
        u8CurrentLedIndex++;
        if(u8CurrentLedIndex == sizeof(aeCurrentLed))
        {
            u8CurrentLedIndex = 0;
        }

        /* Set the current level based on what direction we're now going */
        u8LedCurrentLevel = 20;
        if(abLedRateIncreasing[u8CurrentLedIndex])
        {
            u8LedCurrentLevel = 0;
        }
    }
}

/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ??? */
static void UserApp3SM_Idle(void)
{
    u16Counter--;
    /* Check for update color every COLOR_CYCLE_TIME ms */  
    if(u16Counter == 0)
    {
        u16Counter = COLOR_CYCLE_TIME;

        /* Update the current level based on which way it's headed */
        UserApp3_UpdatePulseState();

        /* Update the value to the current LED */   
        LedPWM( (LedNumberType)aeCurrentLed[u8CurrentLedIndex], (LedRateType)u8LedCurrentLevel);
    }
} /* end UserApp3SM_Idle() */

#if 0
/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp3SM_Error(void)          
{
  
} /* end UserApp3SM_Error() */
#endif


/*-------------------------------------------------------------------------------------------------------------------*/
/* State to sit in if init failed */
static void UserApp3SM_FailedInit(void)          
{
    
} /* end UserApp3SM_FailedInit() */


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
