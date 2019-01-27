#include <TimerOne.h>
/***********************************************************************
* FILENAME :        knight_rider.c             
*
* DESCRIPTION :
*       LED effect for Karcsika :)
*
* AUTHOR :    Arthur Other        START DATE :    16 Jan 99
*
*
* REF NO  VERSION DATE      WHO      DETAIL
* 0.1     22 Jan 2019       Barney   Added config table
*
* HW:  Arduino Uno Board + 8 LED strip
*KNIGHT_RIDER
*/



/**
  * Base number of LED 
*/
#define LED_BASE_NR 2

/**
  * Number of LEDs to control
*/
#define NR_OF_LEDS  8


/**
  * timer init value to have 1ms base valu
*/
#define TIMER_1MS_BASE_VALUE 1000u

/**
  * On and OFF time in ticks (timer1 ticks)
*/
#define ON_TIME_IN_TICKS  2
#define OFF_TIME_IN_TICKs 8

/**
  * On and OFF values of LED states
*/
#define LED_ON  1u
#define LED_OFF 0u


/**
  * Structure that holds LED runtime paramters
*/
typedef struct {
  uint8_t onTimerCnt; /**< Timer that holds the number of ticks until the current led is still in ON state */
  uint8_t offTimerCnt;/**< Timer that holds the number of ticks until the current led is still in ON state */
  bool    state;      /**< State of the LED (1 - ON, 0 - OFF) */
}LED;


/**
  * Structure that holds LED configuration parameters
*/
typedef struct {
  bool    startOffset; /**< Initial Start Offset (in ticks) of the current LED */
  uint8_t onTime;      /**< On time (in ticks) of the current LED */
  uint8_t offTime;     /**< Off time (in ticks) of the current LED */  
}LEDCONFIG;


/** \brief array of LEDs holding runtime parameters  */
LED arrayLED[NR_OF_LEDS];

/** \brief LED array initial configuration */
const LEDCONFIG configArrayLED[NR_OF_LEDS] = 
{
  /*Offset    ON Time               OFF Time */
  {0,         ON_TIME_IN_TICKS,     OFF_TIME_IN_TICKs},  /**< LED 0 */
  {1,         ON_TIME_IN_TICKS,     OFF_TIME_IN_TICKs},  /**< LED 1 */    
  {2,         ON_TIME_IN_TICKS,     OFF_TIME_IN_TICKs},  /**< LED 2 */    
  {3,         ON_TIME_IN_TICKS,     OFF_TIME_IN_TICKs},  /**< LED 3 */    
  {4,         ON_TIME_IN_TICKS,     OFF_TIME_IN_TICKs},  /**< LED 4 */    
  {5,         ON_TIME_IN_TICKS,     OFF_TIME_IN_TICKs},  /**< LED 5 */    
  {6,         ON_TIME_IN_TICKS,     OFF_TIME_IN_TICKs},  /**< LED 6 */    
  {7,         ON_TIME_IN_TICKS,     OFF_TIME_IN_TICKs}   /**< LED 7 */    
};


/**
  * Setup function of the Arduino (called only once at init)
  * @param NA
  * @return NA
*/ 
void setup() 
{
  // Initialize the digital pin as an output.
  // Pin 13 has an LED connected on most Arduino boards
  uint8_t index_u8=0;
  
  for (index_u8=0; (index_u8<NR_OF_LEDS); index_u8++)
  {
    pinMode(LED_BASE_NR+index_u8, OUTPUT); /**< Configure port as DO  */      
    arrayLED[index_u8].onTimerCnt=configArrayLED[index_u8].onTime; 
    if (configArrayLED[index_u8].startOffset == 0)  /**< No start offset, LED will start with LED_ON phase */
    {
      arrayLED[index_u8].state = LED_ON;
      arrayLED[index_u8].offTimerCnt=configArrayLED[index_u8].offTime;      
      digitalWrite(LED_BASE_NR+index_u8, LED_ON); /**< Set HW port state accordingly  */        
    }
    else
    {      
      if (configArrayLED[index_u8].offTime <= configArrayLED[index_u8].startOffset)
      {
      /**< Substract off time from offset  */
        arrayLED[index_u8].offTimerCnt = (configArrayLED[index_u8].startOffset);        
      }
      else
      {
       arrayLED[index_u8].offTimerCnt = configArrayLED[index_u8].offTime;  /**< Wrong configuration, offset cannot be bigger than LED OFF time, use default OFF time */
      }      
      /**< There is a startup offset, LED will start with OFF Phase */
      arrayLED[index_u8].state = LED_OFF;
      digitalWrite(LED_BASE_NR+index_u8, LED_OFF);   /**< Set HW port state accordingly  */
    }
  }  
  Timer1.initialize( 10u * TIMER_1MS_BASE_VALUE); /**< Set timer to 10 ms  */
  Timer1.attachInterrupt( timerIsr ); /**< Attach the service routine here  */ 
}




/**
  * Cyclic function of the Arduino (called in a loop)
  * @param NA
  * @return NA
*/ 
void loop()
{
  
}


/**
  * Interrupt service function of Time 1
  * @param NA
  * @return NA
*/  
void timerIsr(void)
{
  uint8_t index_u8=0;
  for (index_u8=0; (index_u8<NR_OF_LEDS); index_u8++)/**< Iterate through all configured LEDs*/
  {
      
      if (arrayLED[index_u8].state == LED_ON) /**< LED state is currently ON  */
      {
        if (arrayLED[index_u8].onTimerCnt != 0)
        {
          arrayLED[index_u8].onTimerCnt--;
        }
        else
        {/**< LED state Change is needed  */
          arrayLED[index_u8].state = LED_OFF;
          digitalWrite(LED_BASE_NR+index_u8, LED_OFF);  
          arrayLED[index_u8].onTimerCnt = configArrayLED[index_u8].onTime; /**< Reload counter with configured value  */
        }
      }
      else
      {/**< LED state is currently OFF  */        
        if (arrayLED[index_u8].offTimerCnt != 0)
        {
          arrayLED[index_u8].offTimerCnt--;
        }
        else
        { /**< LED state Change is needed  */
          arrayLED[index_u8].state = LED_ON;  
          digitalWrite(LED_BASE_NR+index_u8, LED_ON);   
          arrayLED[index_u8].offTimerCnt = configArrayLED[index_u8].offTime;  /**< Reload counter with configured value  */
        }
      }    
  }
  
}
