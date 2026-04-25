/* config.h.  Generated from config.h.in by configure.  */
/* config.h.in.  Generated from configure.ac by autoheader.  */

/* Define if building universal (internal helper macro) */
/* #undef AC_APPLE_UNIVERSAL_BUILD */

/* Default FAPI profile used for testing */
#define DEFAULT_TEST_FAPI_PROFILE "P_ECC"

/* Disable Vendor Extensions */
/* #undef DISABLE_VENDOR */

/* DISABLE WEAK CRYPTO ALGORITHMS */
/* #undef DISABLE_WEAK_CRYPTO */

/* "The default TCTIs configuration string" */
/* #undef ESYS_TCTI_DEFAULT_CONFIG */

/* "The default TCTI library file" */
/* #undef ESYS_TCTI_DEFAULT_MODULE */

/* Perform integration tests without EK certificate verification */
#define FAPI_TEST_EK_CERT_LESS 1

/* If lib curl has curl_url_strerror function */
#define HAVE_CURL_URL_STRERROR 1

/* Define to 1 if you have the declaration of `cygwin_conv_path', and to 0 if
   you don't. */
/* #undef HAVE_DECL_CYGWIN_CONV_PATH */

/* Define if you have the GNU dld library. */
/* #undef HAVE_DLD */

/* Define to 1 if you have the `dlerror' function. */
#define HAVE_DLERROR 1

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 1

/* Define if you have the _dyld_func_lookup function. */
/* #undef HAVE_DYLD */

/* Support EVP_sm3 in openssl */
#define HAVE_EVP_SM3 1

/* Support EVP_sm4_cfb in openssl */
#define HAVE_EVP_SM4_CFB 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define if you have the libdl library or equivalent. */
#define HAVE_LIBDL 1

/* Define if libdlloader will be built on this platform */
#define HAVE_LIBDLLOADER 1

/* Define to 1 if you have the <minix/config.h> header file. */
/* #undef HAVE_MINIX_CONFIG_H */

/* Define to 1 if you have the `reallocarray' function. */
#define HAVE_REALLOCARRAY 1

/* Define if you have the shl_load function. */
/* #undef HAVE_SHL_LOAD */

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdio.h> header file. */
#define HAVE_STDIO_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define to 1 if you have the <wchar.h> header file. */
#define HAVE_WCHAR_H 1

/* use mssim TCTI for integration tests */
/* #undef INTEGRATION_TCTI_MSSIM */

/* libftdi version 0.x */
/* #undef LIBFTDI_VERSION */

/* Support for writing to a log file is enabled */
#define LOG_FILE_ENABLED 1

/* Define to the sub-directory where libtool stores uninstalled libraries. */
#define LT_OBJDIR ".libs/"

/* "Trace log level" */
#define MAXLOGLEVEL 6

/* mbedTLS cryptographic backend */
/* #undef MBED */

/* Define if debugging is disabled */
#define NDEBUG 1

/* disable use of dlopen */
/* #undef NO_DL */

/* OpenSSL cryptographic backend */
#define OSSL 1

/* Name of package */
#define PACKAGE "tpm2-tss"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "https://github.com/tpm2-software/tpm2-tss/issues"

/* Define to the full name of this package. */
#define PACKAGE_NAME "tpm2-tss"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "tpm2-tss 4.1.2"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "tpm2-tss"

/* Define to the home page for this package. */
#define PACKAGE_URL "https://github.com/tpm2-software/tpm2-tss"

/* Define to the version of this package. */
#define PACKAGE_VERSION "4.1.2"

/* Allow usage of self generated root certificate */
/* #undef SELF_GENERATED_CERTIFICATE */

/* Define to 1 if all of the C90 standard headers exist (not just the ones
   required in a freestanding environment). This macro is provided for
   backward compatibility; new code need not use it. */
#define STDC_HEADERS 1

/* System config dir */
#define SYSCONFDIR "/home/nik/workspace/x86_mock_out/etc"

/* TCTI FOR COMMAND BASED ACCESS TO TPM2 DEVICE */
#define TCTI_CMD 1

/* TCTI FOR DEV TPM */
#define TCTI_DEVICE 1

/* TCTI FOR FUZZING */
/* #undef TCTI_FUZZING */

/* TCTI FOR USB BASED ACCESS TO I2C BASED TPM OVER THE FTDI MPSSE USB TO I2C
   BRIDGE */
/* #undef TCTI_I2C_FTDI */

/* TCTI HELPER FOR I2C BASED ACCESS TO TPM2 DEVICE */
#define TCTI_I2C_HELPER 1

/* TCTI FOR MS SIMULATOR */
#define TCTI_MSSIM 1

/* TCTI FOR TCM */
#define TCTI_SECMODULE 1

/* TCTI FOR SPIDEV BASED ACCESS TO TPM */
#define TCTI_SPIDEV 1

/* TCTI FOR USB BASED ACCESS TO SPI BASED TPM OVER THE FTDI MPSSE USB TO SPI
   BRIDGE */
/* #undef TCTI_SPI_FTDI */

/* TCTI HELPER FOR SPI BASED ACCESS TO TPM2 DEVICE */
#define TCTI_SPI_HELPER 1

/* TCTI FOR USB BASED ACCESS TO LETSTRUST-TPM2GO */
/* #undef TCTI_SPI_LTT2GO */

/* TCTI FOR SWTPM */
#define TCTI_SWTPM 1

/* FAPI forced async spinning */
/* #undef TEST_FAPI_ASYNC */

/* Enable for UNIT testing */
/* #undef UNIT */

/* Enable extensions on AIX 3, Interix.  */
#ifndef _ALL_SOURCE
# define _ALL_SOURCE 1
#endif
/* Enable general extensions on macOS.  */
#ifndef _DARWIN_C_SOURCE
# define _DARWIN_C_SOURCE 1
#endif
/* Enable general extensions on Solaris.  */
#ifndef __EXTENSIONS__
# define __EXTENSIONS__ 1
#endif
/* Enable GNU extensions on systems that have them.  */
#ifndef _GNU_SOURCE
# define _GNU_SOURCE 1
#endif
/* Enable X/Open compliant socket functions that do not require linking
   with -lxnet on HP-UX 11.11.  */
#ifndef _HPUX_ALT_XOPEN_SOCKET_API
# define _HPUX_ALT_XOPEN_SOCKET_API 1
#endif
/* Identify the host operating system as Minix.
   This macro does not affect the system headers' behavior.
   A future release of Autoconf may stop defining this macro.  */
#ifndef _MINIX
/* # undef _MINIX */
#endif
/* Enable general extensions on NetBSD.
   Enable NetBSD compatibility extensions on Minix.  */
#ifndef _NETBSD_SOURCE
# define _NETBSD_SOURCE 1
#endif
/* Enable OpenBSD compatibility extensions on NetBSD.
   Oddly enough, this does nothing on OpenBSD.  */
#ifndef _OPENBSD_SOURCE
# define _OPENBSD_SOURCE 1
#endif
/* Define to 1 if needed for POSIX-compatible behavior.  */
#ifndef _POSIX_SOURCE
/* # undef _POSIX_SOURCE */
#endif
/* Define to 2 if needed for POSIX-compatible behavior.  */
#ifndef _POSIX_1_SOURCE
/* # undef _POSIX_1_SOURCE */
#endif
/* Enable POSIX-compatible threading on Solaris.  */
#ifndef _POSIX_PTHREAD_SEMANTICS
# define _POSIX_PTHREAD_SEMANTICS 1
#endif
/* Enable extensions specified by ISO/IEC TS 18661-5:2014.  */
#ifndef __STDC_WANT_IEC_60559_ATTRIBS_EXT__
# define __STDC_WANT_IEC_60559_ATTRIBS_EXT__ 1
#endif
/* Enable extensions specified by ISO/IEC TS 18661-1:2014.  */
#ifndef __STDC_WANT_IEC_60559_BFP_EXT__
# define __STDC_WANT_IEC_60559_BFP_EXT__ 1
#endif
/* Enable extensions specified by ISO/IEC TS 18661-2:2015.  */
#ifndef __STDC_WANT_IEC_60559_DFP_EXT__
# define __STDC_WANT_IEC_60559_DFP_EXT__ 1
#endif
/* Enable extensions specified by ISO/IEC TS 18661-4:2015.  */
#ifndef __STDC_WANT_IEC_60559_FUNCS_EXT__
# define __STDC_WANT_IEC_60559_FUNCS_EXT__ 1
#endif
/* Enable extensions specified by ISO/IEC TS 18661-3:2015.  */
#ifndef __STDC_WANT_IEC_60559_TYPES_EXT__
# define __STDC_WANT_IEC_60559_TYPES_EXT__ 1
#endif
/* Enable extensions specified by ISO/IEC TR 24731-2:2010.  */
#ifndef __STDC_WANT_LIB_EXT2__
# define __STDC_WANT_LIB_EXT2__ 1
#endif
/* Enable extensions specified by ISO/IEC 24747:2009.  */
#ifndef __STDC_WANT_MATH_SPEC_FUNCS__
# define __STDC_WANT_MATH_SPEC_FUNCS__ 1
#endif
/* Enable extensions on HP NonStop.  */
#ifndef _TANDEM_SOURCE
# define _TANDEM_SOURCE 1
#endif
/* Enable X/Open extensions.  Define to 500 only if necessary
   to make mbstate_t available.  */
#ifndef _XOPEN_SOURCE
/* # undef _XOPEN_SOURCE */
#endif


/* Version number of package */
#define VERSION "4.1.2"

/* Define WORDS_BIGENDIAN to 1 if your processor stores words with the most
   significant byte first (like Motorola and SPARC, unlike Intel). */
#if defined AC_APPLE_UNIVERSAL_BUILD
# if defined __BIG_ENDIAN__
#  define WORDS_BIGENDIAN 1
# endif
#else
# ifndef WORDS_BIGENDIAN
/* #  undef WORDS_BIGENDIAN */
# endif
#endif
