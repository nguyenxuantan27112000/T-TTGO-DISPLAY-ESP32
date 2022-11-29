# Install script for directory: /mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "TRUE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/home/xuantan/.espressif/tools/xtensa-esp32-elf/esp-2021r2-8.4.0/xtensa-esp32-elf/bin/xtensa-esp32-elf-objdump")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/mbedtls" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ FILES
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/aes.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/aesni.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/arc4.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/aria.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/asn1.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/asn1write.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/base64.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/bignum.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/blowfish.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/bn_mul.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/camellia.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ccm.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/certs.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/chacha20.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/chachapoly.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/check_config.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/cipher.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/cipher_internal.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/cmac.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/compat-1.3.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/config.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ctr_drbg.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/debug.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/des.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/dhm.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ecdh.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ecdsa.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ecjpake.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ecp.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ecp_internal.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/entropy.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/entropy_poll.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/error.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/gcm.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/havege.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/hkdf.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/hmac_drbg.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/md.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/md2.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/md4.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/md5.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/md_internal.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/memory_buffer_alloc.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/net.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/net_sockets.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/nist_kw.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/oid.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/padlock.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/pem.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/pk.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/pk_internal.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/pkcs11.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/pkcs12.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/pkcs5.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/platform.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/platform_time.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/platform_util.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/poly1305.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ripemd160.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/rsa.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/rsa_internal.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/sha1.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/sha256.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/sha512.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ssl.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ssl_cache.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ssl_ciphersuites.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ssl_cookie.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ssl_internal.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ssl_ticket.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/threading.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/timing.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/version.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/x509.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/x509_crl.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/x509_crt.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/x509_csr.h"
    "/mnt/d/esp32/esp-idf/components/mbedtls/mbedtls/include/mbedtls/xtea.h"
    )
endif()

