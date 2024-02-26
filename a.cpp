// Copyright (c) 2023-2024 iFarbod. All Rights Reserved.
//
// SPDX-License-Identifier: GPL-3.0-or-later

// For printing builtin macros for a specific target triple, use this:
// echo | clang --target=x -E -dM -
// Where `x` is the target you want to print the macros for, for example, it can be `thumbv7a-linux-androideabi`:
// echo | clang --target=thumbv7a-linux-androideabi -E -dM -
//
// When the host is same as target: (can also use clang++/g++ if you care about the C++ macros):
// echo | clang -E -dM -
// echo | gcc -E -dM -
//
// You can use `clang -dumpmachine` to get the default target triple for your host machine.
// (e.g. x86_64-pc-windows-msvc)
//
// The following command can be used to verify this file defining the correct macros for a certain platform:
// bat compiler.hpp | clang --target=x86_64-unknown-dragonfly -Wno-pragma-once-outside-header -E -dM - | rg -i ztl
// gcc -E -dM compiler.hpp | grep -i ztl | sort
//
// Replace `bat` with `type` on Windows or `cat` otherwise. `rg` can be replaces with `grep` or `find` if you don't have
// it installed.
//
// List of targets:
//
// Windows:
// x86_64-pc-windows-msvc: x86-64 (64-bit) Windows
// aarch64-pc-windows-msvc: ARM64 Windows
// i386-pc-windows-msvc/i686-pc-windows-msvc: x86 (32-bit) Windows
//
// Linux:
// x86_64-pc-linux-gnu: x86-64 (64-bit) Linux
// aarch64-pc-linux-gnu: ARM64 Linux
// i686-pc-linux-gnu: x86 (32-bit) Linux
// armv7-linux-gnueabi: ARMv7-A Linux
// mips-linux-gnu/mipsel-linux-gnu: MIPS Linux (el = little endian)
// mips64-linux-gnu/mips64el-linux-gnu: MIPS64 Linux (el = little endian)
// loongarch64-linux-gnu: LoongArch64 Linux, LP64D ABI (kernel 5.19, glibc 2.36)
// ppc-linux-gnu/ppcle-linux-gnu: PowerPC Linux (le = little endian, can also use `powerpc[64][le]` alias)
// ppc64-linux-gnu/ppc64le-linux-gnu: PPC64 Linux (le = little endian, can also use `powerpc[64][le]` alias)
// riscv64-linux-gnu: RISC-V Linux
// s390x-linux-gnu: S390x Linux
//
// macOS:
// x86_64-apple-darwin: x86-64 (64-bit) macOS
// aarch64-apple-darwin: ARM64 macOS
// arm64e-apple-darwin: ARM64 v8.3+ late-generation M series Apple chips, macOS
//
// BSD:
// x86_64-unknown-freebsd: 64-bit FreeBSD
// x86_64-unknown-netbsd: NetBSD/amd64
// x86_64-unknown-openbsd: ?
// x86_64-unknown-dragonfly: ?
//
// Android:
// aarch64-linux-android: ARM64 Android
// armv7a-linux-androideabi: ARMv7-A Android with or without Thumb (-mthumb)
// i686-linux-android: x86 Android
// x86_64-linux-android: x86-64 Android
// riscv64-linux-android: RISC-V 64 on Android (API 35+)
//
// iOS:
// aarch64-apple-ios: ARM64 iOS
// arm64e-apple-ios: ARM64 iOS next gen chips (?)
// aarch64-apple-ios-sim: Apple iOS Simulator on ARM64
//
// tvOS:
// aarch64-apple-tvos: ARM64 tvOS
// x86_64-apple-tvos: x86 64-bit tvOS
// aarch64-apple-tvos-sim: ARM64 tvOS Simulator
//
// Emscripten:
// wasm32-unknown-emscripten: WebAssembly via Emscripten
// wasm64-unknown-emscripten: memory64 WebAssembly proposal, via Emscripten. (Not widely supported yet)
//
// WASI:
// wasm32-wasi: WebAssembly with WASI

// MARK: Compiler detection

// Clang can simulate both MSVC and GCC, so it will be checked before them.
#ifdef __clang__
#define ZTL_COMPILER_CLANG 1
#define ZTL_COMPILER_NAME "clang"
// #if defined(__APPLE_CC__)
#if defined(__apple_build_version__)
#define ZTL_COMPILER_CLANG_APPLE 1
#endif
// Most codebases check for _MSC_VER, we believe _MSC_BUILD is more accurate.
#elif defined(_MSC_BUILD)
#define ZTL_COMPILER_MSVC 1
#define ZTL_COMPILER_NAME "msvc"
#elif defined(__GNUC__)
#define ZTL_COMPILER_GCC 1
#define ZTL_COMPILER_NAME "gcc"
#else
#error Unsupported compiler
#endif

// MARK: Compiler version

// TODO(iFarbod): Compiler version detection,
// e.g. _MSC_FULL_VER (14.x and 19.x versions, with patch+buildnumber)

#if defined(ZTL_COMPILER_CLANG)
// __clang_version__: __clang_major__.__clang_minor__.__clang_patchlevel__

#define ZTL_COMPILER_VERSION_MAJOR __clang_major__
#define ZTL_COMPILER_VERSION_MINOR __clang_minor__
#define ZTL_COMPILER_VERSION_PATCH __clang_patchlevel__

#elif defined(ZTL_COMPILER_MSVC)

// Visual Studio 2022 version  17.7.0:  17.7.0 , 2022, 19.37, 14.37
// Visual Studio 2022 version  17.8.0:  17.8.0 , 2022, 19.38, 14.38
// Visual Studio 2022 version  17.9.0:  17.9.0 , 2022, 19.39, 14.39
// Visual Studio 2022 version 17.10.0: 17.10.0 , 2022, 19.40, 14.40 (Preview)

// Usually `19` until MSVC decides to change their ABI.
#define ZTL_COMPILER_VERSION_MAJOR (_MSC_FULL_VER / 10000000)
// 38 or 39
#define ZTL_COMPILER_VERSION_MINOR ((_MSC_FULL_VER / 100000) % 100)
// 33321
#define ZTL_COMPILER_VERSION_PATCH (_MSC_FULL_VER % 100000)
// Usually 0, sometimes 1 (esp. on Clang emulating msvc)
#define ZTL_COMPILER_VERSION_BUILD _MSC_BUILD

#elif defined(ZTL_COMPILER_GCC)

// Clang defines this as 4.2.1 always, that hasn't changed for at least a few years.
// __VERSION__: __GNUC__.__GNUC_MINOR__.__GNUC_PATCHLEVEL__

#define ZTL_COMPILER_VERSION_MAJOR __GNUC__
#define ZTL_COMPILER_VERSION_MINOR __GNUC_MINOR__
#define ZTL_COMPILER_VERSION_PATCH __GNUC_PATCHLEVEL__

#endif

// If we're on MSVC and `/Zc:__cplusplus` is not set, we'll get C++98. They will not fix this to avoid breaking
// backwards compatibility.
#if defined(ZTL_COMPILER_MSVC) && __cplusplus == 199711L
#define ZTL_CPP_LANG _MSVC_LANG
#else
#define ZTL_CPP_LANG __cplusplus
#endif

// MARK: Convenience C++ version number macros

// To get this value, `g++ -std=c++2c`
#define ZTL_CPP_LANG_FUTURE 202400L

// #define ZTL_CPP_LANG_26 202600L
#define ZTL_CPP_LANG_23 202302L
#define ZTL_CPP_LANG_20 202002L
#define ZTL_CPP_LANG_17 201703L
#define ZTL_CPP_LANG_14 201402L
#define ZTL_CPP_LANG_11 201103L
#define ZTL_CPP_LANG_98 199711L

// MARK: Architecture
#if defined(__x86_64__) || defined(__amd64__) || defined(_M_AMD64)
#define ZTL_ARCH_X86_64 1
#define ZTL_ARCH_X86_FAMILY 1

// https://clang.llvm.org/docs/UsersManual.html#x86
// echo | clang -march=x86-64-v4 -E -dM - | rg -i avx
#if defined(__AVX512BW__) && defined(__AVX512CD__) && defined(__AVX512DQ__) && defined(__AVX512F__) && \
    defined(__AVX512VL__)
// -march=x86-64-v3
#define ZTL_ARCH_X86_64_LEVEL 4
#elif defined(__AVX2__)
// -march=x86-64-v3
#define ZTL_ARCH_X86_64_LEVEL 3
#elif defined(__SSE4_2__) && defined(__SSSE3__)
#define ZTL_ARCH_X86_64_LEVEL 2
// #elif ARCH == x64
#define ZTL_ARCH_X86_64_LEVEL 1
#endif

#elif defined(__i386__) || defined(_M_IX86)
#define ZTL_ARCH_X86 1
#define ZTL_ARCH_X86_FAMILY 1

#elif defined(__aarch64__) || defined(_M_ARM64)
#define ZTL_ARCH_ARM64 1
#define ZTL_ARCH_ARM_FAMILY 1
#define ZTL_ARCH_AARCH64 1
#define ZTL_ARCH_AARCH64_FAMILY 1

// TODO(iFarbod): How good is arm v9 support, will android add it to NDK?
// -march=armv9-a
#if defined(__ARM_ARCH) && __ARM_ARCH == 9
#define ZTL_ARCH_ARMV9 1
#elif defined(__ARM_ARCH) && __ARM_ARCH == 8
#define ZTL_ARCH_ARMV8 1
#endif

#if defined(__AARCH64EL__)
#define ZTL_ARCH_AARCH64EL 1
#elif defined(__AARCH64EB__)
#define ZTL_ARCH_AARCH64EB 1
#endif

// Apple "Sillicon"
#if defined(__arm64e__)
#define ZTL_ARCH_ARM64E 1
#endif

// ARM has to be checked after aarch64 because they also define arm.
#elif defined(__arm__) || defined(_M_ARM)
#define ZTL_ARCH_ARM 1
#define ZTL_ARCH_ARM_FAMILY 1

// TODO(iFarbod): maybe also do #if defined(__ARM_ARCH_PROFILE) && __ARM_ARCH_PROFILE == 'A'
#if defined(__ARM_ARCH) && __ARM_ARCH == 7
#define ZTL_ARCH_ARMV7 1
#endif

#if defined(__ARMEL__)
#define ZTL_ARCH_ARMEL 1
#elif defined(__ARMEB__)
#define ZTL_ARCH_ARMEB 1
#endif

// clang --target=armv7a-linux-androideabi -mthumb
#if defined(__THUMBEL__)
#define ZTL_ARCH_THUMBEL 1
#elif defined(__THUMBEB__)
#define ZTL_ARCH_THUMBEB 1
#endif

#elif defined(__mips__)
#define ZTL_ARCH_MIPS_FAMILY 1

#if __mips == 32
#define ZTL_ARCH_MIPS32 1
#elif defined(__mips64__)
#define ZTL_ARCH_MIPS64 1
#endif

#if defined(__MIPSEL__)
#define ZTL_ARCH_MIPSEL 1
#elif defined(__MIPSEB__)
#define ZTL_ARCH_MIPSEB 1
#endif

#elif defined(__loongarch__)
#define ZTL_ARCH_LOONGARCH_FAMILY 1

#if defined(__loongarch64) || __loongarch_grlen == 64
#define ZTL_ARCH_LOONGARCH64 1
#elif defined(__loongarch32) || __loongarch_grlen == 32
#define ZTL_ARCH_LOONGARCH32 1
#endif

#elif defined(__PPC__)
#define ZTL_ARCH_POWERPC_FAMILY 1

#if defined(__PPC64__)
#define ZTL_ARCH_POWERPC64 1
#else
#define ZTL_ARCH_POWERPC32 1
#endif

// BIG endian is not defined on NetBSD and OpenBSD OS triples.
// https://github.com/llvm/llvm-project/blob/c098f2d1669fb1ec6a5eac81277bd16b334ec956/clang/lib/Basic/Targets/PPC.cpp#L305
#if defined(_LITTLE_ENDIAN)
#define ZTL_ARCH_PPCLE 1
#elif defined(_BIG_ENDIAN)
#define ZTL_ARCH_PPCBE 1
#else
// By default we assume this.
#define ZTL_ARCH_PPCBE 1
#endif

#elif defined(__riscv)
#define ZTL_ARCH_RISCV_FAMILY 1

#if defined(__riscv_xlen) && __riscv_xlen == 64
#define ZTL_ARCH_RISCV64 1
#elif defined(__riscv_xlen) && __riscv_xlen == 32
#define ZTL_ARCH_RISCV32 1
#endif

#elif defined(__s390__)
#define ZTL_ARCH_S390_FAMILY 1

#if defined(__s390x__)
#define ZTL_ARCH_S390X 1
#endif

#elif defined(__sparc__)
#define ZTL_ARCH_SPARC_FAMILY 1

#if defined(__sparc64__) && defined(__sparcv9__)
#define ZTL_ARCH_SPARCV9 1
#elif defined(__sparcv8__)
#define ZTL_ARCH_SPARCV8 1
#endif

#elif defined(__wasm__)

#define ZTL_ARCH_WASM_FAMILY 1

#if defined(__wasm32__)
#define ZTL_ARCH_WASM32 1
#elif defined(__wasm64__)
// MEMORY64
#define ZTL_ARCH_WASM64 1
#endif

#endif

#if defined(ZTL_COMPILER_MSVC)
#if defined(_M_AMD64) || defined(_M_ARM64)
#define ZTL_CPU_ARCH_64_BITS 1
#else
#define ZTL_CPU_ARCH_32_BITS 1
#endif
#elif defined(ZTL_COMPILER_CLANG) || defined(ZTL_COMPILER_GCC)
#if defined(__LP64__)
#define ZTL_CPU_ARCH_64_BITS 1
#else
#define ZTL_CPU_ARCH_32_BITS 1
#endif
#endif

// MARK: Endianness

// MSVC always builds little endian on Windows.
#if defined(__LITTLE_ENDIAN__) || (defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__) || \
    defined(ZTL_COMPILER_MSVC)
#define ZTL_ENDIAN_LITTLE 1
#elif defined(__BIG_ENDIAN__) || (defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
#define ZTL_ENDIAN_BIG 1
#elif defined(__PDP_ENDIAN__) || (defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_PDP_ENDIAN__)
// Also known as middle-endian.
#define ZTL_ENDIAN_PDP 1
#else
// TODO(iFarbod): We should error here
#define ZTL_ENDIAN_UNKNOWN 1
#endif

// MARK: Platforms

#if defined(_WIN32) || defined(_WIN64)
#define ZTL_OS_WINDOWS 1
// Android must be checked before Linux.
#elif defined(__ANDROID__)
#define ZTL_OS_ANDROID 1
#elif defined(__linux__)
#define ZTL_OS_LINUX 1
// __ENVIRONMENT_* set of macros are nicer because we don't have to pull in the `TargetConditionals` header file.
#elif defined(__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__)
#define ZTL_OS_MACOS 1
#elif defined(__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__)
#define ZTL_OS_IOS 1
#elif defined(__ENVIRONMENT_TV_OS_VERSION_MIN_REQUIRED__)
#define ZTL_OS_TVOS 1
#elif defined(__FreeBSD__)
#define ZTL_OS_FREEBSD 1
#elif defined(__NetBSD__)
#define ZTL_OS_NETBSD 1
#elif defined(__OpenBSD__)
#define ZTL_OS_OPENBSD 1
#elif defined(__DragonFly__)
#define ZTL_OS_DRAGONFLY 1
#elif defined(__EMSCRIPTEN__)
#define ZTL_OS_EMSCRIPTEN 1
#elif defined(__wasi__)
#define ZTL_OS_WASI 1
#else
#error Unsupported target OS
#endif

#if defined(ZTL_OS_MACOS) || defined(ZTL_OS_IOS) || defined(ZTL_OS_TVOS)
// #if defined(__APPLE__)
#define ZTL_OS_APPLE 1
#endif

#if defined(ZTL_OS_IOS) || defined(ZTL_OS_TVOS)
#define ZTL_OS_APPLE_UIKIT 1
#endif

#if defined(ZTL_OS_FREEBSD) || defined(ZTL_OS_NETBSD) || defined(ZTL_OS_OPENBSD) || defined(ZTL_OS_DRAGONFLY)
#define ZTL_OS_BSD 1
#endif

#if defined(ZTL_OS_ANDROID) || defined(ZTL_OS_LINUX) || defined(ZTL_OS_MACOS) || defined(ZTL_OS_IOS) || \
    defined(ZTL_OS_TVOS) || defined(ZTL_OS_BSD) || defined(ZTL_OS_EMSCRIPTEN) || defined(ZTL_OS_WASI)
#define ZTL_UNIX_FAMILY 1
// #define ZTL_OS_POSIX 1
#endif

// MARK: Standard library

#ifdef _CPPLIB_VER
// MSVC's STL
#define ZTL_STL_MSFT 1
#elif defined(_LIBCPP_VERSION)
// llvm STL, libcxx/libc++
#define ZTL_STL_LLVM 1
#elif defined(__GLIBCXX__)
// libstdc++
#define ZTL_STL_GNU 1
#endif

// TODO(iFarbod): GLibC version number
#if defined(__GLIBC__) && !defined(__UCLIBC__)
#define ZTL_GLIBC 1
// __GLIBC__.__GLIBC_MINOR__

#if __GLIBC__ < 2 || (__GLIBC__ == 2 && __GLIBC_MINOR__ < 17)
#error This code requires glibc version 2.17 or later (glibc 2.17 was released in 2012-12-25)
#endif

#endif

// TODO(iFarbod): Bionic detection
// #ifdef __BIONIC__


int main() {
  return 0;
}
