file(GLOB SOURCES_MAYAQUA "*.c" "Crypto/*.c")
file(GLOB HEADERS_MAYAQUA "*.h" "Crypto/*.h")

if(WIN32)
  add_library(mayaqua STATIC ${SOURCES_MAYAQUA} ${HEADERS_MAYAQUA})
else()
  add_library(mayaqua SHARED ${SOURCES_MAYAQUA} ${HEADERS_MAYAQUA})
endif()

target_include_directories(mayaqua PUBLIC .)

set_target_properties(mayaqua
  PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY "${BUILD_DIRECTORY}"
  LIBRARY_OUTPUT_DIRECTORY "${BUILD_DIRECTORY}"
  RUNTIME_OUTPUT_DIRECTORY "${BUILD_DIRECTORY}"
)

find_package(OpenSSL REQUIRED)

include(CheckSymbolExists)

set(CMAKE_REQUIRED_INCLUDES ${OPENSSL_INCLUDE_DIR})
set(CMAKE_REQUIRED_LIBRARIES OpenSSL::Crypto)

check_symbol_exists(EVP_PKEY_get_raw_public_key "openssl/evp.h" HAVE_EVP_PKEY_GET_RAW_PUBLIC_KEY)

unset(CMAKE_REQUIRED_INCLUDES)
unset(CMAKE_REQUIRED_LIBRARIES)

if(NOT HAVE_EVP_PKEY_GET_RAW_PUBLIC_KEY)
  message(FATAL_ERROR "Required EVP_PKEY_get_raw_public_key() not found in OpenSSL library!")
endif()

find_package(ZLIB REQUIRED)

# Required because we include <openssl/opensslv.h> in Encrypt.h.
target_include_directories(mayaqua PUBLIC ${OPENSSL_INCLUDE_DIR})

target_link_libraries(mayaqua
  PRIVATE
    libhamcore
    OpenSSL::SSL
    OpenSSL::Crypto
    ZLIB::ZLIB
)

if(WIN32)
  set_target_properties(mayaqua
    PROPERTIES
    COMPILE_PDB_NAME "mayaqua"
    COMPILE_PDB_OUTPUT_DIRECTORY "${BUILD_DIRECTORY}"
  )

  target_link_libraries(mayaqua
    PRIVATE
      "DbgHelp.Lib"
      "dwmapi.lib"
      "iphlpapi.lib"
      "newdev.lib"
      "Psapi.Lib"
      "Secur32.Lib"
      "setupapi.lib"
      "winmm.lib"
      "WtsApi32.Lib"
    )
endif()

if(UNIX)
  find_package(Threads REQUIRED)

  # In some cases libiconv is not included in libc
  find_library(LIB_ICONV iconv HINTS "${ICONV_LIB_PATH}")

  find_library(LIB_M m)
  find_library(LIB_RT rt)

  target_link_libraries(mayaqua
    PRIVATE
      Threads::Threads
      $<$<BOOL:${LIB_ICONV}>:${LIB_ICONV}>
      $<$<BOOL:${LIB_M}>:${LIB_M}>
      $<$<BOOL:${LIB_RT}>:${LIB_RT}>
  )

  if (CMAKE_SYSTEM_PROCESSOR MATCHES "^(armv7l|aarch64|s390x)$" OR NOT HAVE_SYS_AUXV OR SKIP_CPU_FEATURES)
    add_definitions(-DSKIP_CPU_FEATURES)
  else()
    add_subdirectory(3rdparty/cpu_features)
    set_property(TARGET cpu_features PROPERTY POSITION_INDEPENDENT_CODE ON)
    target_link_libraries(mayaqua PRIVATE cpu_features)
  endif()

  if(${CMAKE_SYSTEM_NAME} STREQUAL "SunOS")
    target_link_libraries(mayaqua PRIVATE nsl socket)
  endif()

  install(TARGETS mayaqua
    COMPONENT "common"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
  )
endif()
