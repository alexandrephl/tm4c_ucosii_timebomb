/*****************************************************************************
* Active Object pattern implementation based on uC/OS-II (uC/AO)
*
*                    Q u a n t u m  L e a P s
*                    ------------------------
*                    Modern Embedded Software
*
* Copyright (C) 2020 Quantum Leaps, LLC. All rights reserved.
*
* SPDX-License-Identifier: APACHE-2.0
*
* This software is distributed by Quantum Leaps, LLC under the terms of
* Apache License Version 2.0, which is the same license used for uC/OS-II RTOS.
* The text of the license is available at: www.apache.org/licenses/LICENSE-2.0.
*
* Contact information:
* <www.state-machine.com>
* <info@state-machine.com>
*****************************************************************************/
#ifndef UC_AO_H
#define UC_AO_H

#include <qassert_mini.h>  /* embedded-systems-friendly assertions (DbC) */
#include "ucos_ii.h"  /* uC/OS-II API, port and compile-time configuration */

/*---------------------------------------------------------------------------*/
/* Event facilities... */

typedef uint16_t Signal; /* event signal */

enum ReservedSignals {
    INIT_SIG, /* dispatched to AO before entering event-loop */
    ENTRY_SIG, /* FOR TRIGGERING THE ENTRY ACTION IN A STATE */
    EXIT_SIG, /* FOR TRIGGERING THE EXIT ACTION IN A STATE */
    USER_SIG  /* first signal available to the users */
};

/* Event base class */
typedef struct {
    Signal sig; /* event signal */
    /* event parameters added in subclasses of Event */
} Event;
/*---------------------------------------------------------------------------*/
/* Finite state Machine facilities... */
typedef struct Fsm Fsm;
typedef enum {
    TRAN_STATUS,
    HANDLED_STATUS,
    IGNORED_STATUS,
    INIT_STATUS
}State;

typedef State (*StateHandler) (Fsm * const me, Event const * const e);

#define TRAN(target_) (((Fsm*)me)->state = (StateHandler)(target_), TRAN_STATUS)



enum { INITIAL_BLINK_TIME = (OS_TICKS_PER_SEC / 4) };

struct Fsm{
    /*add private data for the AO */
    StateHandler state;

};

void Fsm_ctor(Fsm * const me, StateHandler initial);
void Fsm_init(Fsm * const me, Event const * const e);
void Fsm_dispatch(Fsm * const me, Event const * const e);

/*---------------------------------------------------------------------------*/
/* Actvie Object facilities... */

typedef struct Active Active; /* forward declaration */

typedef void (*DispatchHandler)(Active * const me, Event const * const e);

/* Active Object base class */
struct Active {
    Fsm super;        /*inherite Fsm*/
    INT8U thread;     /* private thread (the unique uC/OS-II task priority) */
    OS_EVENT *queue;  /* private message queue */


    /* active object data added in subclasses of Active */
};

void Active_ctor(Active * const me, StateHandler initial);
void Active_start(Active * const me,
                  uint8_t prio,       /* priority (1-based) */
                  Event **queueSto,
                  uint32_t queueLen,
                  void *stackSto,
                  uint32_t stackSize,
                  uint16_t opt);
void Active_post(Active * const me, Event const * const e);

/*---------------------------------------------------------------------------*/
/* Time Event facilities... */

/* Time Event class */
typedef struct {
    Event super;       /* inherit Event */
    Active *act;       /* the AO that requested this TimeEvent */
    uint32_t timeout;  /* timeout counter; 0 means not armed */
    uint32_t interval; /* interval for periodic TimeEvent, 0 means one-shot */
} TimeEvent;

void TimeEvent_ctor(TimeEvent * const me, Signal sig, Active *act);
void TimeEvent_arm(TimeEvent * const me, uint32_t timeout, uint32_t interval);
void TimeEvent_disarm(TimeEvent * const me);

/* static (i.e., class-wide) operation */
void TimeEvent_tick(void);

#endif /* UC_AO_H */
