/**
 * @ingroup     examples
 * @{
 *
 * @file        udp_rx.c
 * @brief       UDP receiver thread
 *
 * @author      Adit Bhartia, Mia Ahmady
 *
 * Github Link: https://github.com/usc-ee250-fall2019/finalproj-lab11-aditmia/tree/finalproj
 *
 * @}
 */

#include <inttypes.h>
#include <stdio.h>

#include "thread.h"
#include "msg.h"
#include "net/gnrc.h"
#include "udp_rx.h"
#include "timex.h"
#include "mutex.h"
#include "random.h"

#define ENABLE_DEBUG (1)
#include "debug.h"

//variables needs for thread
static char udp_rx_stack[THREAD_STACKSIZE_DEFAULT];
static msg_t rcv_queue[8];

#define PRNG_INITVAL            10  /* please use a unique int */
#define SLEEP_MSG_STR           "sleep"
#define SLEEP_MSG_LEN           5
#define SLEEP_INTERVAL_SECS     (4)

#define RAND_USLEEP_MIN         (0)
#define RAND_USLEEP_MAX         (1000000)

static void *_udp_rx(void *arg)
{
    DEBUG("IN RX FUNC \n");
 
    //get args from init, initialize queue, and register with networking stack 
    udp_rx_args_t* udp_rx_args = (udp_rx_args_t *)arg; 

    gnrc_pktsnip_t *snip;
    msg_t msg;
  
    gnrc_netreg_entry_t udp_rx_reg;
    udp_rx_reg.demux_ctx = udp_rx_args->udp_port;
    udp_rx_reg.target.pid = sched_active_pid; 
    
    mutex_t* lock = udp_rx_args->mutex;

    msg_init_queue(rcv_queue, 8);

    gnrc_netreg_register(GNRC_NETTYPE_UDP, &udp_rx_reg);

    /* initialize PRNG */
    random_init(PRNG_INITVAL);
    printf("PRNG initialized to current time: %d\n", PRNG_INITVAL);

    while (1) {

        msg_receive(&msg);

        switch (msg.type) {
            case GNRC_NETAPI_MSG_TYPE_RCV:
                DEBUG("udp_rx: data received\n");
		
		//get actual message data
                snip = msg.content.ptr;
                gnrc_pktsnip_t* payload = gnrc_pktsnip_search_type(snip, GNRC_NETTYPE_UNDEF);
                char* data = (char*) payload->data;
                
		//if we recieve sleep message
		if (strncmp(data,SLEEP_MSG_STR, SLEEP_MSG_LEN) == 0)
                {
                    DEBUG("udp_rx: SLEEP RECIEVED. About to lock..\n");
                    mutex_lock(lock);
                    printf("udp_rx: Lock recieved. Starting timer\n");
                    
		            //sleep stuff
		            xtimer_sleep(SLEEP_INTERVAL_SECS);
                    /* additional random sleep to reduce network collisions */
                    uint32_t interval = random_uint32_range(RAND_USLEEP_MIN, RAND_USLEEP_MAX);
                    xtimer_usleep(interval);
                    
		    //once timer is up, print info and unlock mutex
		    printf("Ending timer\n");
                    printf("Total Time: %f" , SLEEP_INTERVAL_SECS*1000000 + (interval));
                    mutex_unlock(lock);
                }

                gnrc_pktbuf_release(msg.content.ptr);
                break;

            default:
                DEBUG("udp_rx_thr: received something unexpected");
                break;
        }
    }

    /* should never be reached */
    DEBUG("ERROR!\n");
    return (void* )1;
}

kernel_pid_t udp_rx_init(void *args)
{

    DEBUG("IN INIT FUNC \n");

    return thread_create(udp_rx_stack, sizeof(udp_rx_stack), THREAD_PRIORITY_MAIN+1, 
        0, _udp_rx, args, "rx thread" );
}
