#include "../KeyDefines.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/irq.h"
#include "pico/util/queue.h"
#include "WireHandler.h"
#include "I2CWireHandler.h"


queue_t localEventQueue;
queue_t receivedEventQueue;

// Interrupt handler
void i2c1_irq_handler() {

    // Get interrupt status
    uint32_t status = i2c1->hw->intr_stat;
    //is a write request? Or a read request ? 
    if (status & I2C_IC_INTR_STAT_R_RX_FULL_BITS) {
        InputEvent event;
        printf("...");
        i2c_read_raw_blocking(i2c1, (uint8_t *)&event, sizeof(class InputEvent));
        event.source = PRIMARY_I2C_ADDRESS;
        printf("got %d %d\n", event.state, event.scancode);
        queue_try_add(&receivedEventQueue, &event);
        printf("Added to queue, size: %d", queue_get_level(&receivedEventQueue));
    } else if (status & I2C_IC_INTR_STAT_R_RD_REQ_BITS) {
        //Create an empty event here, if the queue is non empty then
        //it'll be populated with the head of the queue, otherwise
        //we'll just send the empty event, which will make Primary stop
        //requesting events.
        InputEvent event;        
        if(!queue_is_empty(&localEventQueue)){
            //pop the head of the queue off and load up the event with it. 
            queue_try_remove(&localEventQueue, &event);
            printf("writing to primary: %d %d \n", event.state, event.scancode);
        }
        event.source = SECONDARY_I2C_ADDRESS;
        i2c_write_raw_blocking(i2c1, (uint8_t *)&event, sizeof(class InputEvent));
        i2c1->hw->clr_rd_req;      
    }
}



I2CWireHandler::I2CWireHandler(bool primary):WireHandler() {
    printf("Initialising I2C Wire Handler");
    
    //this is the master wire handler
    //OR to slave, address 0x76
    busSpeed = i2c_init(i2c1, I2C_BUS_SPEED);
    gpio_set_function(27, GPIO_FUNC_I2C);
    gpio_set_function(26, GPIO_FUNC_I2C);
    gpio_pull_up(27);
    gpio_pull_up(26);
    
    this->primary = primary;
    if(!primary) {
        i2c_set_slave_mode(i2c1,true, SECONDARY_I2C_ADDRESS);
        
        // Enable the I2C interrupts we want to process
        i2c1->hw->intr_mask = I2C_IC_INTR_STAT_R_RX_FULL_BITS | I2C_IC_INTR_MASK_M_RD_REQ_BITS;
        // Set up the interrupt handler to service I2C interrupts
        irq_set_exclusive_handler(I2C1_IRQ, i2c1_irq_handler);

        //setup queues for the secondary, local queue to hold events for the 
        //primary to query, and the received queue to hold events SENT by the
        //primary that we'll put through the inputEvent queue in the scan method.
        queue_init(&localEventQueue, sizeof(class InputEvent), 10);      
        queue_init(&receivedEventQueue, sizeof(class InputEvent), 10);
        // Enable I2C interrupt        
        irq_set_enabled(I2C1_IRQ, true);
    }
}

//This is the wire handler bit.

bool I2CWireHandler::inputEvent(InputEvent* event, KeyboardState* kbState) {
    //This is the wire handler input Event.
    //if MASTER then we send the event to the secondarys IFF the source isn't 
    // already the secondary.
    //if SECONDARY then we add to a buffer and wait for the MASTER to call
    //the SCAN method as below at which point the buffer will be dumped
    //out to master.
    if(event->type == SCANCODE){
        printf("scancode %d , busSpeed: %d , source %d \n", event->scancode, busSpeed, event->source);
        if(primary) {
            printf("Primary\n");
            if (event->source != SECONDARY_I2C_ADDRESS) {
                printf("About to write\n");
                int connStatus = i2c_write_timeout_us(i2c1, SECONDARY_I2C_ADDRESS, (uint8_t *)event,sizeof(class InputEvent),false,I2C_TIMEOUT_US);            
                if (connStatus == PICO_ERROR_GENERIC) {
                    printf("I2C ERR\n" );
                } else if (connStatus == PICO_ERROR_TIMEOUT) {
                    printf("I2C Timeout\n");    
                } else {
                    printf("sent %d to secondary. Bytes: %d. Sizeof: %d \n", event->scancode, connStatus, sizeof(class InputEvent));
                }
            }
        } else {
            if (event->source != PRIMARY_I2C_ADDRESS) {
                queue_try_add(&localEventQueue, event);
            }
        }
    }
    return true;
}

//This is the input device bit.

bool I2CWireHandler::scan(KeyboardState *keyboardState) {
    //if PRIMARY then we poll the secondary(s)
    //if SECONDARY then we 'accept' a connection on the master if 
    //there's something there we ingest and call our handlers.
    if(primary) {
        InputEvent event;
        //we'll do a read timeout here in case the bus goes down, but the 
        //way this is _supposed_ to work there will always be an event on 
        //the secondary to query
        int returned =  i2c_read_timeout_us(i2c1, SECONDARY_I2C_ADDRESS, (uint8_t *)&event,sizeof(InputEvent),false,I2C_TIMEOUT_US);
        if (returned == PICO_ERROR_GENERIC) {
                    //printf("I2C ERR\n" );
        } else if (returned == PICO_ERROR_TIMEOUT) {

        } else {
            //IFF it's an actual keycode, then we can get a proper input event
            //from our pool, copy this event into it, and pass it up the chain.
            if(event.type == SCANCODE) {
                while (event.scancode != 0) {
                    printf("event %d : %d \n",event.scancode, event.state);
                    InputEvent* inputEvent = keyboardState->inputEventPool.getInputEvent(event);
                    keyboardState->inputEvent(inputEvent);
                    i2c_read_timeout_us(i2c1, SECONDARY_I2C_ADDRESS, (uint8_t *)&event,sizeof(InputEvent),false,I2C_TIMEOUT_US);
                }
            }
        }
    } else { //secondary, pop any events of the recieved events queue and do the inputevent
        InputEvent event;        
        while (!queue_is_empty(&receivedEventQueue)){
            //pop the head of the queue off and load up the event with it. 
            queue_try_remove(&receivedEventQueue, &event);
            printf(" Playing event into keyboardState %d %d ", event.state, event.scancode);
            InputEvent* inputEvent = keyboardState->inputEventPool.getInputEvent(event);
            keyboardState->inputEvent(inputEvent);
        } 
    }
    return true;
}
