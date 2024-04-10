/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"

DigitalOut led1(LED1);
DigitalOut led2(D2);
DigitalOut led3(D3);

typedef struct {
    int counter;   /* A counter value               */
    int cycletime_ms;
} message_t;

Mail<message_t, 16> mail_box;


Thread thread(osPriorityNormal ,OS_STACK_SIZE,NULL,"Send Thread");
Thread thread2(osPriorityNormal ,OS_STACK_SIZE,NULL,"Send Thread 2");
Thread thread3(osPriorityNormal ,OS_STACK_SIZE,NULL,"Recieve Thread");



/* Send Thread */

void send_thread(void)
{   
    
    
    while(true){
        ThisThread::sleep_for(500ms);
        message_t *message = mail_box.try_alloc();
        if (message!=NULL)  {
        message->counter = 4;
        message->cycletime_ms = 100;
        mail_box.put(message);
        }
    }    
}

void send_thread2(void)
{   
    
    while(true){
        ThisThread::sleep_for(500ms);
       
        message_t *message = mail_box.try_alloc();
        if (message!=NULL)  {
        message->counter = 2;
        message->cycletime_ms = 200;
        mail_box.put(message);
        }
    }
}

void recieve_thread(void)
{
   
    while (true) {
        
        message_t *message = mail_box.try_get();
        if (message!=NULL)  {
            //message_t *message = (message_t *)evt.value.p;
            for(int x = 0; x < message->counter; x++){
                led2 = !led2;
                ThisThread::sleep_for(message->cycletime_ms);
            }
            mail_box.free(message);
        }
        
    }
}



int main(void)

{

   
    thread.start(send_thread); // Start the event queue's dispatch thread forever
   thread2.start(send_thread2); // Start the event queue's dispatch thread forever
    thread3.start(recieve_thread);

    while (true) {
        ThisThread::sleep_for(2000ms);
    }

}