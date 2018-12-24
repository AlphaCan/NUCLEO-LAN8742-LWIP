/**
  2   *****************************************************************************
  3   * @file    tcp_client.c
  4   * @author  Zorb
  5   * @version V1.0.0
  6   * @date    2018-09-04
  7   * @brief   tcp??????
  8   *****************************************************************************
  9   * @history
 10   *
 11   * 1. Date:2018-09-04
 12   *    Author:Zorb
 13   *    Modification:????
 14   *
 15   *****************************************************************************
 16   */
 
#include "tcpdemo.h"


#define TCP_REMOTE_PORT    8881 /* ???? */
#define TCP_LOCAL_PORT     8880 /* ???? */

/******************************************************************************
  * ??  : ????????
  * ??  : -
  * ??  : -
 ******************************************************************************/
err_t tcp_client_recv(void *arg, struct tcp_pcb *tpcb,
                             struct pbuf *p, err_t err)
{
     uint32_t i;
    
     /* ???? */
     //tcp_write(tpcb, p->payload, p->len, 1);
    
     if (p != NULL)
     {
         struct pbuf *ptmp = p;
         
         /* ???????? */
         printf("get msg from %d:%d:%d:%d port:%d:\r\n",
             *((uint8_t *)&tpcb->remote_ip.addr),
             *((uint8_t *)&tpcb->remote_ip.addr + 1),
             *((uint8_t *)&tpcb->remote_ip.addr + 2),
             *((uint8_t *)&tpcb->remote_ip.addr + 3),
             tpcb->remote_port);
         
         while(ptmp != NULL)
         {
             for (i = 0; i < p->len; i++)
             {
                 printf("%c", *((char *)p->payload + i));
             }
             
             ptmp = p->next;
         }
         
         printf("\r\n");
         
         tcp_recved(tpcb, p->tot_len);
         
         /* ??????? */
         pbuf_free(p);
     }
     else if (err == ERR_OK)
     {
         printf("tcp client closed\r\n");
         
         tcp_recved(tpcb, p->tot_len);
         
         return tcp_close(tpcb);
     }
 
     return ERR_OK;
 }
 
 /******************************************************************************
  * ??  : ?????????
  * ??  : -
  * ??  : -
 ******************************************************************************/
err_t tcp_client_connected(void *arg, struct tcp_pcb *tpcb, err_t err)
 {
     printf("tcp client connected\r\n");
     
     tcp_write(tpcb, "tcp client connected", strlen("tcp client connected"), 0);
 
     /* ???????? */
     tcp_recv(tpcb, tcp_client_recv);
 
     return ERR_OK;
 }
 
 /******************************************************************************
  * ??  : ??tcp???
  * ??  : ?
  * ??  : ?
 ******************************************************************************/
 void tcp_client_init(void)
 {
     struct tcp_pcb *tpcb;
     ip_addr_t serverIp;
 
     /* ???IP */
     IP4_ADDR(&serverIp, 192, 168, 0, 103);
 
     /* ??tcp??? */
     tpcb = tcp_new();
     
     if (tpcb != NULL)
     {
         err_t err;
         
         /* ???????IP?? */
         err = tcp_bind(tpcb, IP_ADDR_ANY, TCP_LOCAL_PORT);
 
         if (err == ERR_OK)
         {
             /* ????? */
             tcp_connect(tpcb, &serverIp, TCP_REMOTE_PORT, tcp_client_connected);
         }
         else
         {
             memp_free(MEMP_TCP_PCB, tpcb);
             
             printf("can not bind pcb\r\n");
         }
     }
 }
 
 
 /******************************************************************************
  * ??  : ??????
  * ??  : -
  * ??  : -
 ******************************************************************************/
err_t tcp_server_recv(void *arg, struct tcp_pcb *tpcb,
                              struct pbuf *p, err_t err)
 {
     uint32_t i;
     
     /* ???? */
     //tcp_write(tpcb, p->payload, p->len, 1);
     
     if (p != NULL)
     {
         struct pbuf *ptmp = p;
         
         /* ???????? */
         printf("get msg from %d:%d:%d:%d port:%d:\r\n",
             *((uint8_t *)&tpcb->remote_ip.addr),
             *((uint8_t *)&tpcb->remote_ip.addr + 1),
             *((uint8_t *)&tpcb->remote_ip.addr + 2),
             *((uint8_t *)&tpcb->remote_ip.addr + 3),
             tpcb->remote_port);
         
         while(ptmp != NULL)
         {
             for (i = 0; i < p->len; i++)
             {
                 printf("%c", *((char *)p->payload + i));
             }
             
             ptmp = p->next;
         }
         
         printf("\r\n");
         
         tcp_recved(tpcb, p->tot_len);
         
         /* ??????? */
         pbuf_free(p);
     }
     else if (err == ERR_OK)
     {
         printf("tcp client closed\r\n");
         
         tcp_recved(tpcb, p->tot_len);
         
        return tcp_close(tpcb);
     }
 
     return ERR_OK;
 }
 
 /******************************************************************************
  * ??  : ?????????
  * ??  : -
  * ??  : -
 ******************************************************************************/
err_t tcp_server_accept(void *arg, struct tcp_pcb *newpcb, err_t err)
 {
     printf("tcp client connected\r\n");
     
     printf("ip %d:%d:%d:%d port:%d\r\n",
         *((uint8_t *)&newpcb->remote_ip.addr),
         *((uint8_t *)&newpcb->remote_ip.addr + 1),
         *((uint8_t *)&newpcb->remote_ip.addr + 2),
         *((uint8_t *)&newpcb->remote_ip.addr + 3),
         newpcb->remote_port);
     
     tcp_write(newpcb, "tcp client connected", strlen("tcp client connected"), 0);
     
     /* ???????? */
     tcp_recv(newpcb, tcp_server_recv);
 
     return ERR_OK;
 }
 
 /******************************************************************************
  * ??  : ??tcp???
  * ??  : ?
  * ??  : ?
 ******************************************************************************/
 void tcp_server_init(void)
 {
     struct tcp_pcb *tpcb;
 
     /* ??tcp??? */
     tpcb = tcp_new();
 
     if (tpcb != NULL)
     {
         err_t err;
         
         /* ??????,???????ip?? */
         err = tcp_bind(tpcb, IP_ADDR_ANY, TCP_LOCAL_PORT);
 
         if (err == ERR_OK)
         {
             /* ?? */
             tpcb = tcp_listen(tpcb);
 
             /* ???????? */
             tcp_accept(tpcb, tcp_server_accept);
             
             printf("tcp server listening\r\n");
//             printf("tcp server ip:%d:%d:%d:%d prot:%d\r\n",
//                 *((uint8_t *)&ipaddr.addr),                 
// 				 *((uint8_t *)&ipaddr.addr + 1),
//                  *((uint8_t *)&ipaddr.addr + 2),
//                 *((uint8_t *)&ipaddr.addr + 3),
//                 tpcb->local_port);
         }
         else
         {
             memp_free(MEMP_TCP_PCB, tpcb);
             
             printf("can not bind pcb\r\n");
         }
 
     }
 }
 
 
 
 
 /******************************** END OF FILE ********************************/
