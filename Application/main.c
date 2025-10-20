/******************************************************************************
* @file    main.c
* @brief   TimeBomb demo (HSM on Active Object) using uC/OS-II + uc_ao
* @board   EK-TM4C123GXL (TM4C123GH6PM)
* @author  Alexandre Panhaleux
******************************************************************************/
#include <qassert_mini.h>  /* embedded-system-friendly assertions */
#include "uc_ao.h"  /* uC/OS-II API, port and compile-time configuration */
#include "bsp.h"      /* Board Support Package */
#include <stdbool.h>

Q_DEFINE_THIS_MODULE("main") /* module name for Q_ASSERT() */

/* Config =========================================================*/
enum {
    TIME_BOMB_BLINKS = 5U,
    HALF_SECOND_TICKS = OS_TICKS_PER_SEC / 2U
};

/* The TimeBomb AO =========================================================*/
typedef struct TimeBomb TimeBomb;

struct TimeBomb{
    Active super; /* inherit base active object class */

    TimeEvent te; /* Time event (TIMEOUT_SIG) */
    uint32_t blink_ctr;
};

/* State handlers */
State TimeBomb_initial(TimeBomb * const me, Event const * const e);
State TimeBomb_wait4button(TimeBomb * const me, Event const * const e);
State TimeBomb_blink(TimeBomb * const me, Event const * const e);
State TimeBomb_pause(TimeBomb * const me, Event const * const e);
State TimeBomb_boom(TimeBomb * const me, Event const * const e);

/* State definitions =========================================================*/
/* Initial transition for the TimeBomb AO */
State TimeBomb_initial(TimeBomb * const me, Event const * const e){
    return TRAN(TimeBomb_wait4button);
}
/* Idle (waiting) state */
State TimeBomb_wait4button(TimeBomb * const me, Event const * const e){
    State status;
    switch(e->sig){
        case ENTRY_SIG:{
            BSP_ledGreenOn();
            status = HANDLED_STATUS;
            break;
        }
        case EXIT_SIG:{
            BSP_ledGreenOff();
            status = HANDLED_STATUS;
            break;
        }
        case BUTTON_PRESSED_SIG:{
            me->blink_ctr = TIME_BOMB_BLINKS;
            status = TRAN(TimeBomb_blink);
            break;
        }
        default:{
            status = IGNORED_STATUS;
            break;
        }
    }
    return status;
}

/* Active blinking state (LED ON phase) */
State TimeBomb_blink(TimeBomb * const me, Event const * const e){
    State status;
    switch(e->sig){
        case ENTRY_SIG:{
            BSP_ledRedOn();
            TimeEvent_arm(&me->te, OS_TICKS_PER_SEC/2, 0U);
            status = HANDLED_STATUS;
            break;
        }
        case EXIT_SIG:{
            BSP_ledRedOff();
            status = HANDLED_STATUS;
            break;
        }
        case TIMEOUT_SIG:{
            status = TRAN(TimeBomb_pause);
            break;
        }
        default:{
            status = IGNORED_STATUS;
            break;
        }
    }
    return status;
}

/* Pause state (LED OFF phase between blinks) */
State TimeBomb_pause(TimeBomb * const me, Event const * const e){
    State status;
    switch(e->sig){
        case ENTRY_SIG:{
            TimeEvent_arm(&me->te, OS_TICKS_PER_SEC/2, 0U);
            status = HANDLED_STATUS;
            break;
        }
        case EXIT_SIG:{
            status = HANDLED_STATUS;
            break;
        }
        case TIMEOUT_SIG:{
            --me->blink_ctr;
            if(me->blink_ctr > 0U){
                status = TRAN(TimeBomb_blink);
            }else{
                status = TRAN(TimeBomb_boom);
            }
            break;
        }
        default:{
            status = IGNORED_STATUS;
            break;
        }
    }
    return status;
}

/* Final “explosion” state */
State TimeBomb_boom(TimeBomb * const me, Event const * const e){
    State status;
        switch(e->sig){
            case ENTRY_SIG:{
                BSP_ledRedOn();
                BSP_ledGreenOn();
                BSP_ledBlueOn();
                status = HANDLED_STATUS;
                break;
            }
            default:{
                status = IGNORED_STATUS;
                break;
            }
        }
        return status;
}

/* Constructor =========================================================*/
void TimeBomb_ctor(TimeBomb * const me) {
    Active_ctor(&me->super, (StateHandler)&TimeBomb_initial);
    TimeEvent_ctor(&me->te, TIMEOUT_SIG, &me->super);
    me->blink_ctr = 0U;
}

/* AO instance and RTOS resources declaration =========================================================*/
OS_STK stack_timeBomb[100]; /* task stack */
static Event *timeBomb_queue[10]; /* event queue storage */
static TimeBomb timeBomb; /* AO instance */
Active *AO_timeBomb = &timeBomb.super;



/* the main function =========================================================*/
int main() {

    BSP_init(); /* initialize the BSP */
    OSInit();   /* initialize uC/OS-II */


    /* create AO and start it */
    TimeBomb_ctor(&timeBomb);
    Active_start(AO_timeBomb,
                      2U,       /* priority (1-based) */
                      timeBomb_queue,
                      sizeof(timeBomb_queue)/sizeof(timeBomb_queue[0]),
                      stack_timeBomb,
                      sizeof(stack_timeBomb),
                      0U);

    BSP_start(); /* configure and start the interrupts */

    OSStart(); /* start the uC/OS-II scheduler... */
    return 0; /* NOTE: the scheduler does NOT return */
}

/*******************************************************************************
* NOTE1:
* The call to uC/OS-II API OSTaskCreateExt() assumes that the pointer to the
* top-of-stack (ptos) is at the end of the provided stack memory. This is
* correct only for CPUs with downward-growing stack, but must be changed for
* CPUs with upward-growing stack.
*/
