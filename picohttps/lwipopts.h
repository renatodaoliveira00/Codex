/* Configuração do lwIP para o exemplo Pico HTTPS *****************************
 *                                                                            *
 *  Configuração para a biblioteca de rede lwIP incluída no Pico SDK e        *
 *  necessária para o exemplo Pico HTTPS.                                      *
 *                                                                            *
 *  Obs.: Nem todas as opções são estritamente necessárias; esta é apenas uma  *
 *  configuração de exemplo.                                                  *
 *                                                                            *
 *  https://www.nongnu.org/lwip/2_1_x/group__lwip__opts.html                  *
 *  https://github.com/lwip-tcpip/lwip/blob/master/src/include/lwip/opt.h     *
 *                                                                            *
 ******************************************************************************/

#ifndef _LWIPOPTS_EXAMPLE_COMMONH_H
#define _LWIPOPTS_EXAMPLE_COMMONH_H

/* Opções do sistema **********************************************************/

// Executar sem sistema operacional
//
//  Sem sistema operacional na plataforma Pico W
//
#define NO_SYS                      1



/* Opções de memória **********************************************************/

// Desabilitar malloc nativo da biblioteca C
//
//  Incompatível com `pico_cyw43_arch_lwip_threadsafe_background`
//
#define MEM_LIBC_MALLOC             0

// Alinhamento de bytes
#define MEM_ALIGNMENT               4       // bytes

// Tamanho do heap
#define MEM_SIZE                    4000    // bytes



/* Opções de pool de memória **************************************************/

// Máximo de pacotes ARP em fila
#define MEMP_NUM_ARP_QUEUE          10
//
// Máximo de segmentos TCP em fila
#define MEMP_NUM_TCP_SEG            32



/* Opções de ARP **************************************************************/

// Habilitar suporte a ARP
//
//  Necessário para a camada IP da pilha de rede
//
#define LWIP_ARP                    1



/* Opções de ICMP *************************************************************/

// Habilitar suporte a ICMP
//
//  Provavelmente necessário para a camada IP da pilha de rede?
//
#define LWIP_ICMP                   1



/* Opções de IP ***************************************************************/

// Habilitar suporte a IPv4
#define LWIP_IPV4                   1



/* Opções de DHCP *************************************************************/

// Habilitar suporte a DHCP
//
//  Necessário para conectar à rede sem fio
//
#define LWIP_DHCP                   1

// Desabilitar detecção de conflito de endereço
#define LWIP_DHCP_DOES_ACD_CHECK    0

// Desabilitar verificação ARP
#define DHCP_DOES_ARP_CHECK         0

/* Opções de DNS **************************************************************/

// Habilitar suporte a DNS
//
//  Necessário para resolução de nomes de host
//
#define LWIP_DNS                    1



/* Opções de UDP **************************************************************/

// Habilitar suporte a UDP
//
//  Provavelmente necessário para consultas DNS?
//
#define LWIP_UDP                    1



/* Opções de TCP **************************************************************/

// Habilitar suporte a TCP
#define LWIP_TCP                    1

// Tamanho máximo do segmento
#define TCP_MSS                     1460

// Tamanho da janela
#define TCP_WND                     (16 * TCP_MSS)

// Tamanho do buffer de envio
#define TCP_SND_BUF                 (8 * TCP_MSS)

// Comprimento da fila de envio
#define TCP_SND_QUEUELEN            ((4 * (TCP_SND_BUF) + (TCP_MSS - 1)) / (TCP_MSS))

// Opções de TCP
#define LWIP_TCP_KEEPALIVE          1


/* Opções de ALTCP ************************************************************/

// Habilitar suporte a ALTCP
//
//  ALTCP é a interface do lwIP para TCP + X. No caso do exemplo Pico HTTPS,
//  X deve ser TLS, como requerido para HTTPS.
//
#define LWIP_ALTCP                  1

// Habilitar interface TLS compatível com ALTCP
//
//  Ou seja, definir X como TLS em ALTCP = TCP + X
//
#define LWIP_ALTCP_TLS              1

// Habilitar interface TLS compatível com ALTCP
//
//  Uma porta da biblioteca Mbed TLS está incluída no lwIP. __Obs.: esta não é uma
//  distribuição completa do MbedTLS__, mas apenas fornece uma interface compatível
//  com lwIP para o Mbed TLS.
//
#define LWIP_ALTCP_TLS_MBEDTLS      1



/* Opções do Mbed TLS *********************************************************/

// Exigir autenticação TLS (certificado)
//
//  Ignorar erros de certificado leva a problemas graves…
//
#define ALTCP_MBEDTLS_AUTHMODE      MBEDTLS_SSL_VERIFY_REQUIRED


/* Opções de interface de rede ************************************************/

// Desabilitar suporte à API NETIF
//
//  Não é necessário. API sequencial, portanto apenas para plataformas com SO.
//
#define LWIP_NETIF_API              0

// Definir nome da interface a partir do hostname
#define LWIP_NETIF_HOSTNAME         1

// Habilitar callback em mudança de estado da interface
#define LWIP_NETIF_STATUS_CALLBACK  1

// Habilitar callback em mudança de estado do link
#define LWIP_NETIF_LINK_CALLBACK    1

// Tentar colocar todos os dados TX em um único pbuf
#define LWIP_NETIF_TX_SINGLE_PBUF   1



/* Opções da API sequencial ***************************************************/

// Desabilitar suporte a socket
//
//  Não é necessário. API sequencial, portanto apenas para plataformas com SO.
//
#define LWIP_SOCKET                 0

// Desabilitar suporte a netconn
//
//  Não é necessário. API sequencial, portanto apenas para plataformas com SO.
//
#define LWIP_NETCONN                0



/* Opções de estatísticas *****************************************************/

// Habilitar estatísticas
#define LWIP_STATS                  1

// Habilitar função de exibição de estatísticas
#define LWIP_STATS_DISPLAY          1

// Habilitar estatísticas de memória
#define MEM_STATS                   1

// Desabilitar estatísticas do sistema
#define SYS_STATS                   0

// Desabilitar estatísticas de pool de memória
#define MEMP_STATS                  0

// Desabilitar estatísticas de link
#define LINK_STATS                  0



/* Opções de depuração ********************************************************/

// Habilitar depuração do lwIP
//#define LWIP_DEBUG                  1

// Habilitar depuração da porta TLS
//#define ALTCP_MBEDTLS_DEBUG         LWIP_DBG_ON

// Habilitar depuração do Mbed TLS
//#define ALTCP_MBEDTLS_LIB_DEBUG     LWIP_DBG_ON

#endif //_LWIPOPTS_EXAMPLE_COMMONH_H
