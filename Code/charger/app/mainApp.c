/*
 * Copyright (c) 2020, yifengling0. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. No personal names or organizations' names associated with the
 *    Atomthreads project may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE ATOMTHREADS PROJECT AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE PROJECT OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>

#include "atom.h"
#include "atomport-private.h"
#include "atomtimer.h"
#include "stm8s_clk.h"
#include "device.h"
#include "event.h"
#include "uart.h"
#include "debug.h"
#include "console.h"
#include <string.h>
#include "iosdrv.h"
#include "eventmgr.h"
#include "ledmgr.h"

/* Constants */

/*
 * Idle thread stack size
 *
 * This needs to be large enough to handle any interrupt handlers
 * and callbacks called by interrupt handlers (e.g. user-created
 * timer callbacks) as well as the saving of all context when
 * switching away from this thread.
 */
#ifdef USE_CONSOLE
#define IDLE_STACK_SIZE_BYTES       64
#else
#define IDLE_STACK_SIZE_BYTES       128
#endif


/*
 * Main thread stack size
 *
 * Note that this is not a required OS kernel thread - you will replace
 * this with your own application thread.
 *
 * In this case the Main thread is responsible for calling out to the
 * test routines. Once a test routine has finished, the test status is
 * printed out on the UART and the thread remains running in a loop.
 *
 * The Main thread stack generally needs to be larger than the idle
 * thread stack, as not only does it need to store interrupt handler
 * stack saves and context switch saves, but the application main thread
 * will generally be carrying out more nested function calls and require
 * stack for application code local variables etc.
 *
 * 1KB might be adequate but if using printf() then at least 2KB would be
 * prudent otherwise the stdio functions otherwise stack overruns are
 * likely. Nearly 2KB was seen to be used on the toolchain used for
 * development.
 */

#ifdef USE_CONSOLE
#define MAIN_STACK_SIZE_BYTES       128
#else
#define MAIN_STACK_SIZE_BYTES       192
#endif


/* Local data */

/* Application threads' TCBs */
static ATOM_TCB main_tcb;

/* Main thread's stack area */
static uint8_t main_thread_stack[MAIN_STACK_SIZE_BYTES];

/* Idle thread's stack area */
static uint8_t idle_thread_stack[IDLE_STACK_SIZE_BYTES];

/* Forward declarations */
static void main_thread_func (uint32_t data);

/**
 * \b main
 *
 * Program entry point.
 *
 * Creates an application thread and starts the OS.
 */

void main ( void )
{
    int8_t status;

#ifdef USE_CONSOLE
    ConsoleInit();
    DebugInit();
#endif    

    IosDrvInit();
    device_init();

    /**
     * Note: to protect OS structures and data during initialisation,
     * interrupts must remain disabled until the first thread
     * has been restored. They are reenabled at the very end of
     * the first thread restore, at which point it is safe for a
     * reschedule to take place.
     */

    /**
     * Initialise the OS before creating our threads.
     */
    archInitSystemTickTimer();
             
    status = atomOSInit(&idle_thread_stack[0], IDLE_STACK_SIZE_BYTES, TRUE);
    
     if (status == ATOM_OK)
     {
          /* Create an application thread */ 
          status = atomThreadCreate(&main_tcb,
                       16, main_thread_func, 0,
                       &main_thread_stack[0],
                       MAIN_STACK_SIZE_BYTES,
                       TRUE);
 
         if (status == ATOM_OK)
          {
            /**
              * First application thread successfully created. It is
              * now possible to start the OS. Execution will not return
              * from atomOSStart(), which will restore the context of
              * our application thread and start executing it.
              *
              * Note that interrupts are still disabled at this point.
              * They will be enabled as we restore and execute our first
              * thread in archFirstThreadRestore().
              */
              atomOSStart();
          }
     }

    while (1)
        ;

    /* There was an error starting the OS if we reach here */
    //return (0);
}

#define TIMEOUT_TICK (20)

static ATOM_TIMER timer_cb;
static void atomMutexTimerCallback (POINTER cb_data);

static void TickTimerRegister(int timeout)
{
    timer_cb.cb_func = atomMutexTimerCallback;
    timer_cb.cb_data = &timer_cb;
    timer_cb.cb_ticks = timeout;
    atomTimerRegister (&timer_cb);
}

static void atomMutexTimerCallback (POINTER cb_data)
{
    event_mgr_set(EVENT_TIMER, TIMEOUT_TICK);
    TickTimerRegister(TIMEOUT_TICK);
}


/**
 * \b main_thread_func
 *
 * Entry point for main application thread.
 *
 * This is the first thread that will be executed when the OS is started.
 *
 * @param[in] data Unused (optional thread entry parameter)
 *
 * @return None
 */
static void main_thread_func (uint32_t data)
{
    int timeout_cnt = 0;

    DebugPrint("tws1 charger！\r\n");
    
#ifdef USE_CONSOLE    
    ConsoleRun();
    while(1){
      atomTimerDelay(100);
    }
#else    
    led_mgr_init();
    event_mgr_init();
    TickTimerRegister(TIMEOUT_TICK);
    
/* Check main thread stack usage (if enabled) */
#ifdef ATOM_STACK_CHECKING
    if (status == 0)
    {
        uint32_t used_bytes, free_bytes;

        /* Check idle thread stack usage */
        if (atomThreadStackCheck (&main_tcb, &used_bytes, &free_bytes) == ATOM_OK)
        {
            /* Check the thread did not use up to the end of stack */
            if (free_bytes == 0)
            {
                DebugPrint ("Main stack overflow\n");
                status++;
            }
       }
     }
#endif
    
   // 开启充电
    boost_mgr_set_boost(1);

    while(1){
        ATOM_EX_EVENT event;
        //watchdog_feed();
        event_mgr_wait(&event, 0);

        //处理调试命令
        
        if( event.type == EVENT_TIMER){ //超时处理
            led_mgr_tick();
            timeout_cnt++;
            if( timeout_cnt >= 25 ){
                //关盖并且没有输出电流情况下直接关闭输出节省电量
              int hall_status = hallswitch_mgr_get_status();
              
              if(hall_status == 0 && power_mgr_is_charge_amp_low() ){
                  boost_mgr_set_boost(0);
                  //watchdog_feed();
                  DebugPrint ("sleep \r\n");
                  // 进入停机模式省电
                  halt(); 
               }
               
              if(power_mgr_is_battery_low()){ //电池电量低于阈值关闭输出, 保持使用耳机状态
                  led_mgr_set_mode(LED_MODE_ERROR);
                  boost_mgr_set_boost(0);
              }
              
              timeout_cnt = 0;
            }
        }else if( event.type == EVENT_HALLSWITCH ){ //霍尔开关处理
            int hall_status = hallswitch_mgr_get_status();
            boost_mgr_set_boost(1);
            led_mgr_set_mode(LED_MODE_SWITCH);
            timeout_cnt = 0; 
        }else{
            //DebugPrint ("system failure\n");
        }
    }
#endif        

}