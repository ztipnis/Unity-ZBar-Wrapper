#include <stdint.h>
#ifndef __WRAPPER_H__
#define __WRAPPER_H__
#ifdef __cplusplus
extern "C"{
#endif
    typedef struct wrapper_z zbar_wrapper_t;
    typedef struct symbolset_z zw_symbolset_t;
    typedef struct symbol_z{
        int x0, y0, x1, y1;
        char* symbol_data;
    } zw_symbol_t;

    zbar_wrapper_t* zbar_wrapper_init();
    void zbar_wrapper_destroy(zbar_wrapper_t*);
    zw_symbolset_t* zbar_wrapper_scan_RGBA32(zbar_wrapper_t*, uint8_t*, int, int);
    zw_symbol_t* zbar_wrapper_symbol_next(zw_symbolset_t*);
    void zbar_wrapper_symbol_destroy(zw_symbol_t*);
    void zbar_wrapper_symbolset_destroy(zw_symbolset_t*);

#ifdef __cplusplus
}
#endif

#endif