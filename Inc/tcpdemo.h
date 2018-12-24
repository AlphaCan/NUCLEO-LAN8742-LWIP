#ifndef __TCPDEMO_
#define __TCPDEMO_


#include "stm32h7xx_hal.h"
#include "lwip.h"
#include "tcp.h"
#include "string.h"


err_t tcp_client_recv(void *arg, struct tcp_pcb *tpcb,
                             struct pbuf *p, err_t err);

err_t tcp_client_connected(void *arg, struct tcp_pcb *tpcb, err_t err);

void tcp_client_init(void);

err_t tcp_server_recv(void *arg, struct tcp_pcb *tpcb,
                              struct pbuf *p, err_t err);

err_t tcp_server_accept(void *arg, struct tcp_pcb *newpcb, err_t err);

void tcp_server_init(void);


#endif




