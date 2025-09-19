<<<<<<< HEAD
#pragma once

#include <cstddef>
#include <cstdint>
#ifndef _ASSET_H_
#define _ASSET_H_

extern "C" {

typedef struct __attribute__((__packed__)) _asset {
        uint8_t* buf;
        size_t size;
} asset;
}

#define ASSET(x)                                                                                                       \
    extern "C" {                                                                                                       \
    extern uint8_t _binary_static_##x##_start[], _binary_static_##x##_size[];                                          \
    static asset x = {_binary_static_##x##_start, (size_t)_binary_static_##x##_size};                                  \
    }

#define ASSET_LIB(x)                                                                                                   \
    extern "C" {                                                                                                       \
    extern uint8_t _binary_static_lib_##x##_start[], _binary_static_lib_##x##_size[];                                  \
    static asset x = {_binary_static_lib_##x##_start, (size_t)_binary_static_lib_##x##_size};                          \
    }

=======
#pragma once

#include <cstddef>
#include <cstdint>
#ifndef _ASSET_H_
#define _ASSET_H_

extern "C" {

typedef struct __attribute__((__packed__)) _asset {
        uint8_t* buf;
        size_t size;
} asset;
}

#define ASSET(x)                                                                                                       \
    extern "C" {                                                                                                       \
    extern uint8_t _binary_static_##x##_start[], _binary_static_##x##_size[];                                          \
    static asset x = {_binary_static_##x##_start, (size_t)_binary_static_##x##_size};                                  \
    }

#define ASSET_LIB(x)                                                                                                   \
    extern "C" {                                                                                                       \
    extern uint8_t _binary_static_lib_##x##_start[], _binary_static_lib_##x##_size[];                                  \
    static asset x = {_binary_static_lib_##x##_start, (size_t)_binary_static_lib_##x##_size};                          \
    }

>>>>>>> 1d9f8e89a47b9fdcfcd5046e5cc23ef5a95901bd
#endif // _ASSET_H_