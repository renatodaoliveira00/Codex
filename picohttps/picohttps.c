/* Exemplo de cliente MQTTS no Pico W com sincronização de tempo ************
 *
 *  Inspirado nos conceitos de um guia para ESP8266, este código demonstra
 *  uma conexão MQTT segura (TLS) robusta no Raspberry Pi Pico W.
 *
 *  Funcionalidades Chave:
 *  1. Conexão Wi-Fi.
 *  2. Sincronização de tempo com um servidor NTP (essencial para validação de
 *     certificados TLS).
 *  3. Conexão segura ao broker HiveMQ Cloud usando a biblioteca MQTT da lwIP.
 *  4. Publicação de uma mensagem ao pressionar um botão.
 *
 ******************************************************************************/

/* --- Inclusões --- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "hardware/gpio.h"
#include "lwip/apps/mqtt.h"
#include "lwip/apps/sntp.h"
#include "lwip/dns.h"
#include "lwip/altcp_tls.h" 

/* --- Configurações --- */
#define BUTTON_GPIO         5

// Configurações do Broker MQTT (HiveMQ Cloud)
#define MQTT_BROKER_HOST    "bb76a95f718542fbaede96f1be105d07.s1.eu.hivemq.cloud"
#define MQTT_BROKER_PORT    8883
#define MQTT_CLIENT_ID      "PicoW_SensorClient"
#define MQTT_USER           "PicoW" // Insira o usuário do seu cluster HiveMQ
#define MQTT_PASSWORD       "Mqtt#tls0"   // Insira a senha do seu usuário
#define MQTT_TOPIC          "pico/sensores"

// Certificado da Autoridade Certificadora (CA) para o HiveMQ Cloud.
// Este é o ISRG Root X1, que é o certificado raiz para Let's Encrypt, usado pelo HiveMQ.
// Incorporar o certificado no código simplifica drasticamente o projeto, eliminando
// a necessidade de um sistema de arquivos (LittleFS) e scripts externos.
const char* mqtt_ca_cert =
"-----BEGIN CERTIFICATE-----\n"
"MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw\n"
"TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh\n"
"cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4\n"
"WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu\n"
"ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY\n"
"MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0PiD6yWVR0Rll\n"
"olpAf8kpdBC+cuf8cNHt4B4ogsbJeXMLhE6gEPU24IEbEkeITC2sRNg1ovoTPqmB\n"
"WVyuzdNEJA+EDcFramework+hsNqcur4VFRm6vHiI2Ren7SHRma4roR6cEXPJkY\n"
"Ko1Iq+EzgnxPxgQIA1LAd4zBSRoAAbhiLqKNzMSx4sxD+SDdda8R9cZo5iIpVeWs\n"
"6yjpPkkHyVo5hejY7Cj4GkazSVRi8N3DNvPk5JGNw0bWnFPdWAe0Wd7VFNJpXFLE\n"
"P6Uu150WbIZZMzx5L/a/dos5se72+coHtUdzaRaQm4Yb3yp+jvxXcM9s0qHNB3gW\n"
"VUXqbVfdh5WgqXLB_Gz1bL26YDKuyMA0uD8ymAHXtLyTr6ucS3iV1_9pl5QZAj0Y\n"
"ecodZjdrc6eKYUSAE0w3EMU4mDa4vHI244lORTgSKshIK3CEFn/T6lI2pG/bRxpx\n"
"3DTcGBDziBFG2q/eUD6E2opIwxshpbgnVbK48jDH6xpYx8Amsy8gYgE2wBbyA5xH\n"
"iX8EXgR1A0j3ATc2cyYHqxIX3HNAU4SFc/JCzsilVJXyuixD0Ztwn0XAow4gAPfl\n"
"iSUj6r8jIuKFH3rLgwdQVSRif/5afw1vrYofhB2xOsAm32XhF75d6e2ZXm+4jMCC\n"
"A0j1o18qRQq4i+uL6xuAA8CRqGWS4TzU9zLpgwIMyD1so3g42+vjA2p6iY6k952l\n"
"aORMgZ7T5m3uGFxK7bVRGwqk//0vmyg5a272LAnaWbq9a82LqgFeQo2fVsPEA2+n\n"
"G1zjcCjLnTdM9FF8Kjovy2di2pQGsyOCi6pEFSvuas6ynTiGDLX/AeA2GPcoGE6Y\n"
"o4I4YIM0V4sMXiWDPVzc8i08bAaa2mYpKERj0L71K2dMUGVn144_Gv/d4k+zoi3a\n"
"kY/lMRem+rt24JzMkA26jL/kBCs+jK3+wV942I+syLq1pLpBNz3BBLVv1+T2SsEu\n"
"A/i25g+_St+G2i6bK72v1/sO4vrvJ/QazP76YWrUXo8gEzde/j3gZ4u3O9d2+gQH\n"
"L/7S/bO1R1/H3BN2_n6gQmsSMyL3SAq"
"-----END CERTIFICATE-----\n";


// Estrutura para manter o estado do nosso cliente
typedef struct MQTT_CLIENT_T_ {
    ip_addr_t remote_addr;
    mqtt_client_t *mqtt_client;
    bool connected;
} MQTT_CLIENT_T;

// Protótipos de Callbacks MQTT
static void mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status);
static void mqtt_incoming_publish_cb(void *arg, const char *topic, u32_t tot_len);
static void mqtt_incoming_data_cb(void *arg, const u8_t *data, u16_t len, u8_t flags);

// Função para sincronizar o tempo, confiando no polling em segundo plano
void sync_time_with_ntp() {
    printf("Iniciando sincronização de tempo via NTP...\n");

    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_init();

    printf("SNTP inicializado. Aguardando a primeira sincronização...\n");

    // Loop de espera com timeout
    int max_retries = 30; // Tentar por no máximo 30 segundos
    time_t now = 0;
    const uint32_t year_2022_in_seconds = 1640995200; 

    while (time(&now) < year_2022_in_seconds && max_retries-- > 0) {
        printf(".");
        sleep_ms(1000);
    }

    if (time(&now) >= year_2022_in_seconds) {
        char buf[32];
        strftime(buf, sizeof(buf), "%d/%m/%Y %H:%M:%S", localtime(&now));
        printf("\n>>> SUCESSO: Tempo sincronizado! Hora atual: %s\n", buf);
    } else {
        printf("\n>>> FALHA: Timeout ao sincronizar o tempo.\n");
        printf("Isso geralmente indica um problema de conectividade com a internet ou configuração do SDK.\n");
        while(true); // Trava o programa
    }

    sntp_stop();
}


int main() {
    stdio_init_all();
    srand(time(NULL));

    if (cyw43_arch_init()) {
        printf("Falha ao inicializar o módulo Wi-Fi\n");
        return 1;
    }
    cyw43_arch_enable_sta_mode();

    printf("Conectando a %s...\n", WIFI_SSID);
    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000)) {
        printf("Falha ao conectar ao Wi-Fi\n");
        return 1;
    }
    printf("Conectado com sucesso!\n");

    // Passo crucial: Sincronizar o tempo antes de tentar a conexão TLS
    sync_time_with_ntp();

    // --- Início da Lógica MQTT ---
    MQTT_CLIENT_T* state = calloc(1, sizeof(MQTT_CLIENT_T));
    if (!state) {
        printf("Falha ao alocar estado do cliente\n");
        return 1;
    }
    
    // Cria uma nova instância de cliente MQTT
    state->mqtt_client = mqtt_client_new();
    if (state->mqtt_client == NULL) {
        printf("Falha ao criar cliente MQTT\n");
        free(state);
        return 1;
    }
    
    // Configura a conexão
    struct mqtt_connect_client_info_t ci;
    memset(&ci, 0, sizeof(ci));
    ci.client_id = MQTT_CLIENT_ID;
    ci.client_user = MQTT_USER;
    ci.client_pass = MQTT_PASSWORD;
    ci.keep_alive = 60; // Keep-alive em segundos
    ci.will_topic = NULL; // Sem Last Will and Testament

    // Configura o TLS (MQTTS)
    struct altcp_tls_config *tls_config;
    tls_config = altcp_tls_create_config_client((const u8_t*)mqtt_ca_cert, strlen(mqtt_ca_cert) + 1);
    if(tls_config == NULL){
        printf("Falha ao criar configuração TLS\n");
        free(state);
        return 1;
    }
    ci.tls_config = tls_config;

    // Resolve o IP do broker e inicia a conexão
    cyw43_arch_lwip_begin();
    err_t err = dns_gethostbyname(MQTT_BROKER_HOST, &state->remote_addr, NULL, NULL);
    if (err == ERR_OK) {
        mqtt_client_connect(state->mqtt_client, &state->remote_addr, MQTT_BROKER_PORT, mqtt_connection_cb, state, &ci);
    } else if (err == ERR_INPROGRESS) {
        // DNS em andamento
    } else {
        printf("Erro imediato na resolução DNS: %d\n", err);
    }
    cyw43_arch_lwip_end();

    printf("Tentando conectar ao broker MQTT...\n");
    while(!state->connected) {
       cyw43_arch_poll();
       sleep_ms(100);
    }

    printf("Publicando dados de sensor a cada 5 segundos...\n");

    while (true) {
        if (!state->connected) {
            printf("Desconectado. Tentando reconectar...\n");
            sleep_ms(5000);
            continue;
        }

        float temp, umid;
        read_sensor(&temp, &umid);

        char payload[64];
        snprintf(payload, sizeof(payload), "{\"temperatura\":%.2f,\"umidade\":%.1f}", temp, umid);

        cyw43_arch_lwip_begin();
        err_t pub_err = mqtt_publish(state->mqtt_client, MQTT_TOPIC, payload, strlen(payload), 1, 0, NULL, NULL);
        cyw43_arch_lwip_end();

        if (pub_err == ERR_OK) {
            printf("Publicado: %s\n", payload);
        } else {
            printf("Falha ao publicar mensagem MQTT (erro %d)\n", pub_err);
        }

        for (int i = 0; i < 500; i++) { // 5 segundos com polling
            cyw43_arch_poll();
            sleep_ms(10);
        }
    }
}

/* --- Implementação dos Callbacks MQTT --- */

// Chamado quando o status da conexão muda
static void mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status) {
    MQTT_CLIENT_T* state = (MQTT_CLIENT_T*)arg;
    if (status == MQTT_CONNECT_ACCEPTED) {
        printf("Callback: Conexão MQTT aceita!\n");
        state->connected = true;
        
        // Configura os callbacks para dados recebidos
        mqtt_set_inpub_callback(client, mqtt_incoming_publish_cb, mqtt_incoming_data_cb, arg);
        
        // Exemplo: se inscrever em um tópico após conectar
        // mqtt_subscribe(client, "pico/comandos", 1, NULL, NULL);

    } else {
        printf("Callback: Conexão MQTT falhou, status: %d\n", status);
        // Possíveis erros: -1 (timeout), -2 (erro TLS), 5 (não autorizado), etc.
        state->connected = false;
    }
}

// Chamado quando o início de uma mensagem publicada chega
static void mqtt_incoming_publish_cb(void *arg, const char *topic, u32_t tot_len) {
    printf("Mensagem recebida no tópico '%s' (tamanho total: %lu)\n", topic, tot_len);
}

// Chamado com os dados (payload) de uma mensagem recebida
static void mqtt_incoming_data_cb(void *arg, const u8_t *data, u16_t len, u8_t flags) {
    printf("Payload recebido: %.*s\n", len, (const char *)data);
    // Se for o último fragmento, flags terá o bit MQTT_DATA_FLAG_LAST definido
    if(flags & MQTT_DATA_FLAG_LAST) {
        printf("--- Fim da mensagem ---\n");
    }
}

// Função para simular leitura de sensor
void read_sensor(float *temp, float *umid) {
    // Simulação: valores aleatórios
    *temp = 20.0f + (rand() % 1000) / 100.0f; // 20.00 a 29.99
    *umid = 50.0f + (rand() % 500) / 10.0f;   // 50.0 a 99.9
}