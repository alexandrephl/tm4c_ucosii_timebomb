/******************************************************************************
* @file    bsp.h
* @brief   Board Support Package interface for EK-TM4C123GXL (uC/OS-II + uC/AO)
* @board   EK-TM4C123GXL (TM4C123GH6PM)
* @author  Alexandre Panhaleux
******************************************************************************/
#ifndef BSP_H
#define BSP_H

/* Public API =========================================================*/
void BSP_init(void);
void BSP_start(void);

void BSP_ledRedOn(void);
void BSP_ledRedOff(void);
void BSP_ledBlueOn(void);
void BSP_ledBlueOff(void);
void BSP_ledGreenOn(void);
void BSP_ledGreenOff(void);

/* Event signals =========================================================*/
enum EventSignals {
    BUTTON_PRESSED_SIG = USER_SIG,
    BUTTON_RELEASED_SIG,
    TIMEOUT_SIG,
    MAX_SIG
};

/* Active Objects exported to BSP =========================================================*/
extern Active *AO_timeBomb;

#endif /* BSP_H */
