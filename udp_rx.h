/**
 * @ingroup     examples
 * @{
 *
 * @file        udp_rx.h
 * @brief       UDP receiver thread header file
 *
 * @author      Adit Bhartia, Mia Ahmady
 *
 * Github Link: https://github.com/usc-ee250-fall2019/finalproj-lab11-aditmia/tree/finalproj
 *
 * @}
 */

/* Header files are used to share global functions and variables. */

#include "kernel_types.h"
#include "mutex.h"

typedef struct {
    mutex_t *mutex;
    uint32_t udp_port;
} udp_rx_args_t;

kernel_pid_t udp_rx_init(void *args);