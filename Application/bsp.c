/******************************************************************************
* @file    bsp.c
* @brief   Board Support Package for EK-TM4C123GXL using uC/OS-II and uC/AO
* @board   EK-TM4C123GXL (TM4C123GH6PM)
* @author  Alexandre Panhaleux
******************************************************************************/
#include <qassert_mini.h>  /* Q_ASSERT / Q_onAssert */
#include "uc_ao.h"  /* uC/OS-II API */
#include "bsp.h"      /* Board Support Package */
#include <stdbool.h>
#include "TM4C123GH6PM.h" /* Tiva C MCU header */


/* GPIOF pin bits ===============================================*/
/* LEDs on the board */
#define LED_RED      (1U << 1)
#define LED_GREEN    (1U << 3)
#define LED_BLUE     (1U << 2)

/* Buttons on the board */
#define BTN_SW1      (1U << 4)
#define BTN_SW2      (1U << 0)

/* uC/OS-II hooks ===============================================*/
void App_TimeTickHook(void) {

    TimeEvent_tick(); /* Process all armed TimeEvents first (framework “soft timers”). */

    /* Debounce structure for SW1  */
    static struct ButtonsDebouncing {
        uint32_t depressed;
        uint32_t previous;
    } buttons = { 0U, 0U };
    uint32_t current;
    uint32_t tmp;

    /* Perform the debouncing of buttons. The algorithm for debouncing
    * adapted from the book "Embedded Systems Dictionary" by Jack Ganssle
    * and Michael Barr, page 71.
    */
    current = ~GPIOF_AHB->DATA_Bits[BTN_SW1]; /* read SW1 */
    tmp = buttons.depressed; /* save the debounced depressed buttons */
    buttons.depressed |= (buttons.previous & current); /* set depressed */
    buttons.depressed &= (buttons.previous | current); /* clear released */
    buttons.previous   = current; /* update the history */
    tmp ^= buttons.depressed;     /* changed debounced depressed */
    if ((tmp & BTN_SW1) != 0U) {  /* debounced SW1 state changed? */
        if ((buttons.depressed & BTN_SW1) != 0U) { /* is SW1 depressed? */
            static Event const buttonPressedEvt = {BUTTON_PRESSED_SIG};
            Active_post(AO_timeBomb, &buttonPressedEvt);
        }
        else { /* the button is released */
            static Event const buttonReleasedEvt = {BUTTON_RELEASED_SIG};
            Active_post(AO_timeBomb, &buttonReleasedEvt);
        }
    }
}

/* Idle-task : sleep between interrupts */
void App_TaskIdleHook(void) {
#ifdef NDEBUG
    __WFI(); /* Wait-For-Interrupt */
#endif
}

/* Required uC/OS-II hooks */
void App_TaskCreateHook (OS_TCB *ptcb) { (void)ptcb; }
void App_TaskDelHook    (OS_TCB *ptcb) { (void)ptcb; }
void App_TaskReturnHook (OS_TCB *ptcb) { (void)ptcb; }
void App_TaskStatHook   (void)         {}
void App_TaskSwHook     (void)         {}
void App_TCBInitHook    (OS_TCB *ptcb) { (void)ptcb; }


/* BSP init/start ===========================================================*/
void BSP_init(void) {
    /* System clock info was set in startup; refresh the cached value. */
    SystemCoreClockUpdate();

    SYSCTL->RCGCGPIO  |= (1U << 5); /* enable Run mode for GPIOF */
    SYSCTL->GPIOHBCTL |= (1U << 5); /* enable AHB for GPIOF */

    /* LEDs as outputs */
    GPIOF_AHB->DIR |= (LED_RED | LED_BLUE | LED_GREEN);
    GPIOF_AHB->DEN |= (LED_RED | LED_BLUE | LED_GREEN);

    /* configure switch SW1 */
    GPIOF_AHB->DIR &= ~BTN_SW1; /* input */
    GPIOF_AHB->DEN |= BTN_SW1; /* digital enable */
    GPIOF_AHB->PUR |= BTN_SW1; /* pull-up resistor enable */
}
/*..........................................................................*/
void BSP_start(void) {

    SystemCoreClockUpdate();

    /* Program SysTick to drive the framework tick at OS_TICKS_PER_SEC. */
    SysTick_Config(SystemCoreClock / OS_TICKS_PER_SEC);

    /* set priorities of ALL ISRs used in the system */
    NVIC_SetPriority(SysTick_IRQn,  CPU_CFG_KA_IPL_BOUNDARY + 1U);
}

/* LED helpers ===========================================================*/
void BSP_ledRedOn(void) {
    GPIOF_AHB->DATA_Bits[LED_RED] = LED_RED;
}

void BSP_ledRedOff(void) {
    GPIOF_AHB->DATA_Bits[LED_RED] = 0U;
}

void BSP_ledBlueOn(void) {
    GPIOF_AHB->DATA_Bits[LED_BLUE] = LED_BLUE;
}

void BSP_ledBlueOff(void) {
    GPIOF_AHB->DATA_Bits[LED_BLUE] = 0U;
}

void BSP_ledGreenOn(void) {
    GPIOF_AHB->DATA_Bits[LED_GREEN] = LED_GREEN;
}

void BSP_ledGreenOff(void) {
    GPIOF_AHB->DATA_Bits[LED_GREEN] = 0U;
}

/* Assertions ===========================================================*/
Q_NORETURN Q_onAssert(char const * const module, int const id) {
    (void)module; // unused parameter
    (void)id;     // unused parameter
#ifndef NDEBUG
    // light up all LEDs
    GPIOF_AHB->DATA_Bits[LED_GREEN | LED_RED | LED_BLUE] = 0xFFU;
    // for debugging, hang on in an endless loop...
    for (;;) {
    }
#endif
    NVIC_SystemReset(); /* In Release, reset the MCU */
}
// error-handling function called by exception handlers in the startup code
Q_NORETURN assert_failed(char const * const module, int const id);
Q_NORETURN assert_failed(char const * const module, int const id) {
    Q_onAssert(module, id);
}

