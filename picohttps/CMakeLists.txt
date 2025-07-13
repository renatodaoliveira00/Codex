# Generated Cmake Pico project file
# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Define o tipo de placa
set(PICO_BOARD pico_w CACHE STRING "Tipo de placa")

# Inclui o Raspberry Pi Pico SDK (deve ser antes do projeto)
include(pico_sdk_import.cmake)

# Declara o projeto CMake
project(picohttps C CXX ASM)

# Inicializa o SDK do Raspberry Pi Pico
pico_sdk_init()

# Aponta o executável para o código-fonte principal
add_executable(

    # Alvo
    picohttps

    # Fonte
    picohttps.c
)

target_compile_definitions(

    # Alvo
    picohttps

    PRIVATE WIFI_SSID=\"@\" 
    PRIVATE WIFI_PASSWORD=\"internet\"

)

# Configura caminhos de busca do pré-processador
#
#   Para incluir arquivos de cabeçalho adicionais (ou seja, externos ao Pico SDK)
#
target_include_directories(

    # Alvo
    picohttps

    # Diretório de trabalho atual
    #
    #   Para incluir arquivos de cabeçalho locais do projeto, por exemplo;
    #
    #   - picohttps.h               # Cabeçalho local do projeto
    #   - lwipopts.h                # Configuração da biblioteca lwIP
    #   - mbedtls_config.h          # Configuração da biblioteca Mbed TLS
    #
    PRIVATE ${CMAKE_CURRENT_LIST_DIR}

    # Inclusões de porta do Mbed TLS
    #
    #   Cabeçalhos internos para a porta Mbed TLS fornecida com lwIP. Normalmente não estão
    #   no caminho de busca do pré-processador, mas são necessários ao usar
    #   interfaces subjacentes do Mbed TLS não expostas pela porta Mbed TLS (mais
    #   especificamente, casting para uma chamada `mbedtls_ssl_set_hostname` para configuração da extensão TLS SNI).
    #
    #   Veja a função `connect_to_host` em `picohttps.c` para detalhes.
    #
    ${PICO_LWIP_PATH}/src/apps/altcp_tls/

)

# --- HABILITA O POLLING DA REDE EM SEGUNDO PLANO ---
target_compile_definitions(picohttps PRIVATE
    PICO_CYW43_ARCH_POLL_INTERVAL_MS=10
)

# Configura o linker
#
#   Para linkar (estaticamente) arquivos de bibliotecas adicionais (ou seja, externos ao Pico SDK)
#
#   https://cmake.org/cmake/help/latest/command/target_link_libraries.html
#
target_link_libraries(

    # Alvo
    picohttps

    # Biblioteca padrão C
    pico_stdlib

    # I/O padrão via USB
    pico_stdio_usb

    # Bibliotecas sem fio do Pico W
    #
    #   Inclui bibliotecas para driver de hardware (`pico_cyw43_driver`) e pilha TCP/IP
    #   (`pico_lwip`).
    #
    pico_cyw43_arch_lwip_threadsafe_background

    # Biblioteca TLS
    #
    #   Necessária para HTTPS (= TCP + TLS + HTTP).
    #
    #   Uma porta da biblioteca Mbed TLS está incluída no lwIP, e portanto no
    #   Pico SDK. __Obs.: esta não é uma distribuição completa do MbedTLS__, mas
    #   apenas fornece uma interface compatível com lwIP para o Mbed TLS.
    #
    pico_lwip_mbedtls
    #
    #   Uma distribuição completa do Mbed TLS está incluída no Pico SDK
    #   (atualmente como submódulo). Isso atualmente (Pico SDK 1.5.0) não está bem
    #   documentado.
    #
    pico_mbedtls                                 
    # mbedtls distribuído com pico-sdk
    pico_stdlib
    pico_cyw43_arch_lwip_threadsafe_background
    pico_lwip_mqtt
    pico_lwip_sntp
    pico_lwip_mbedtls   
)


# Configura saída binária
#
#
#   Solicita que formatos binários adicionais (.uf2, .hex, etc.) sejam gerados.
#
#   Definido em macros do Pico SDK.
#
#   https://github.com/raspberrypi/pico-sdk/blob/master/src/rp2_common.cmake
#
pico_add_extra_outputs(

    # Alvo
    picohttps

)