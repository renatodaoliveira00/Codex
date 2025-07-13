/* Configuração do Mbed TLS para o exemplo Pico HTTPS *************************
 *                                                                            *
 *  Configuração para a biblioteca Mbed TLS incluída no Pico SDK e            *
 *  necessária para o exemplo Pico HTTPS.                                     *
 *                                                                            *
 *  Obs.: Nem todas as opções são estritamente necessárias; esta é apenas uma  *
 *  configuração de exemplo.                                                  *
 *                                                                            *
 *  https://github.com/Mbed-TLS/mbedtls/blob/v2.28.2/include/mbedtls/config.h *
 *                                                                            *
 ******************************************************************************/



/* Diversos ******************************************************************/

// Solução para alguns arquivos fonte do Mbed TLS que usam INT_MAX sem incluir limits.h
#include <limits.h>



/* Sistema *******************************************************************/

#define MBEDTLS_HAVE_TIME



/* Funcionalidades do Mbed TLS ***********************************************/

// Entropia
#define MBEDTLS_NO_PLATFORM_ENTROPY                 // Sem fonte de entropia de hardware
#define MBEDTLS_ENTROPY_HARDWARE_ALT                // Coletor de entropia customizado (pico-sdk:pico_mbedtls.c)

// Cifras simétricas
#define MBEDTLS_CIPHER_MODE_CBC                     // Cipher block chaining
#define MBEDTLS_CIPHER_MODE_CFB                     // Cipher feedback mode
#define MBEDTLS_CIPHER_MODE_CTR                     // Counter block cipher mode
#define MBEDTLS_CIPHER_MODE_OFB                     // Output feedback mode
#define MBEDTLS_CIPHER_MODE_XTS                     // XOR-encrypt-XOR
#define MBEDTLS_CIPHER_PADDING_PKCS7                // Modos de preenchimento (padding)
#define MBEDTLS_CIPHER_PADDING_ONE_AND_ZEROS
#define MBEDTLS_CIPHER_PADDING_ZEROS_AND_LEN
#define MBEDTLS_CIPHER_PADDING_ZEROS

// Remoção de suítes de cifra fracas
#define MBEDTLS_REMOVE_ARC4_CIPHERSUITES            // ARC4
#define MBEDTLS_REMOVE_3DES_CIPHERSUITES            // 3DES

// Curvas elípticas
#define MBEDTLS_ECP_DP_SECP192R1_ENABLED
#define MBEDTLS_ECP_DP_SECP224R1_ENABLED
#define MBEDTLS_ECP_DP_SECP256R1_ENABLED
#define MBEDTLS_ECP_DP_SECP384R1_ENABLED
#define MBEDTLS_ECP_DP_SECP521R1_ENABLED
#define MBEDTLS_ECP_DP_SECP192K1_ENABLED
#define MBEDTLS_ECP_DP_SECP224K1_ENABLED
#define MBEDTLS_ECP_DP_SECP256K1_ENABLED
#define MBEDTLS_ECP_DP_BP256R1_ENABLED
#define MBEDTLS_ECP_DP_BP384R1_ENABLED
#define MBEDTLS_ECP_DP_BP512R1_ENABLED
#define MBEDTLS_ECP_DP_CURVE25519_ENABLED
#define MBEDTLS_ECP_DP_CURVE448_ENABLED
#define MBEDTLS_ECP_NIST_OPTIM                      // Otimizações NIST
#define MBEDTLS_ECDSA_DETERMINISTIC                 // ECDSA determinístico (mais seguro)

// Troca de chaves
#define MBEDTLS_KEY_EXCHANGE_RSA_ENABLED
#define MBEDTLS_KEY_EXCHANGE_ECDHE_RSA_ENABLED
#define MBEDTLS_KEY_EXCHANGE_ECDHE_ECDSA_ENABLED
#define MBEDTLS_KEY_EXCHANGE_ECDH_ECDSA_ENABLED
#define MBEDTLS_KEY_EXCHANGE_ECDH_RSA_ENABLED

// PKCS
#define MBEDTLS_PKCS1_V15                           // Codificação PKCS#1 v1.5
#define MBEDTLS_PKCS1_V21                           // Codificação PKCS#1 v2.1

// Registros TLS
#define MBEDTLS_SSL_ALL_ALERT_MESSAGES              // Enviar registros de alerta
#define MBEDTLS_SSL_RECORD_CHECKING                 // Validar registros

// Extensões TLS
#define MBEDTLS_SSL_ENCRYPT_THEN_MAC                // Extensão TLS (RFC 7366)
#define MBEDTLS_SSL_EXTENDED_MASTER_SECRET          // Extensão TLS (RFC 7627)
#define MBEDTLS_SSL_MAX_FRAGMENT_LENGTH             // Extensão TLS (RFC 6066)
#define MBEDTLS_SSL_SERVER_NAME_INDICATION          // Extensão TLS (RFC 6066)
#define MBEDTLS_SSL_TRUNCATED_HMAC                  // Extensão TLS (RFC 6066)

// Protocolos
#define MBEDTLS_SSL_PROTO_TLS1_2                    // Habilitar TLS versão 1.2

// X.509
#define MBEDTLS_X509_CHECK_KEY_USAGE                // Verificar extensão keyUsage
#define MBEDTLS_X509_CHECK_EXTENDED_KEY_USAGE       // Verificar extensão extendedKeyUsage



/* Módulos ********************************************************************/

// Cifras
#define MBEDTLS_CIPHER_C                            // Código genérico de cifra simétrica
#define MBEDTLS_AES_C                               // AES
#define MBEDTLS_GCM_C                               // Modo Galois/Counter

// Parsers
#define MBEDTLS_ASN1_PARSE_C                        // ASN1
#define MBEDTLS_PEM_PARSE_C                         // PEM
#define MBEDTLS_PK_PARSE_C                          // PK

// Hashes
#define MBEDTLS_MD_C                                // Código genérico de hash
#define MBEDTLS_MD5_C                               // MD5
#define MBEDTLS_POLY1305_C                          // Poly1305 MAC
#define MBEDTLS_SHA256_C                            // SHA 256
#define MBEDTLS_SHA512_C                            // SHA 512

// Curvas elípticas
#define MBEDTLS_ECDH_C                              // Diffie-Hellman
#define MBEDTLS_ECDSA_C                             // Assinatura
#define MBEDTLS_ECP_C                               // Implementação GF(p)

// RSA
#define MBEDTLS_RSA_C                               // RSA

// Chave pública
#define MBEDTLS_PK_C                                // Código genérico de chave pública
#define MBEDTLS_PKCS5_C                             // PKCS#5
#define MBEDTLS_PKCS12_C                            // PKCS#12

// SSL/TLS
#define MBEDTLS_SSL_TLS_C                           // Código genérico TLS
#define MBEDTLS_SSL_CLI_C                           // Código cliente TLS

// Certificados X.509
#define MBEDTLS_X509_USE_C                          // Núcleo
#define MBEDTLS_X509_CRT_PARSE_C                    // Análise de certificado

// Requisitos
#define MBEDTLS_ENTROPY_C                           // para ALTCP TLS
#define MBEDTLS_BIGNUM_C                            // para definir MBEDTLS_ECP_C, MBEDTLS_RSA_C, MBEDTLS_X509_USE_C
#define MBEDTLS_BASE64_C                            // para MBEDTLS_PEM_PARSE_C
#define MBEDTLS_HMAC_DRBG_C                         // para MBEDTLS_ECDSA_DETERMINISTIC
#define MBEDTLS_CTR_DRBG_C                          // para MBEDTLS_AES_C
#define MBEDTLS_OID_C                               // para MBEDTLS_RSA_C
#define MBEDTLS_ASN1_WRITE_C                        // para MBEDTLS_ECDSA_C

// Diversos
#define MBEDTLS_ERROR_C                             // Conversão de código de erro
#define MBEDTLS_PLATFORM_C                          // Redefinição de libc

// Depuração
//#define MBEDTLS_DEBUG_C                           // Funções de depuração
//#define MBEDTLS_SSL_DEBUG_ALL                     // Saída de depuração



/* Configuração do módulo *************************************************************/

