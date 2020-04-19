%module ZBarWrapper
%{
    #include <stdint.h>
    typedef struct wrapper_z zbar_wrapper_t;
    typedef struct symbolset_z zw_symbolset_t;
    typedef struct symbol_z{
        int x0, y0, x1, y1;
        char* symbol_data;
    } zw_symbol_t;

    extern zbar_wrapper_t* zbar_wrapper_init();
    extern void zbar_wrapper_destroy(zbar_wrapper_t*);
    extern zw_symbolset_t* zbar_wrapper_scan_RGBA32(zbar_wrapper_t*, uint8_t*, int, int);
    extern zw_symbol_t* zbar_wrapper_symbol_next(zw_symbolset_t*);
    extern void zbar_wrapper_symbol_destroy(zw_symbol_t*);
    extern void zbar_wrapper_symbolset_destroy(zw_symbolset_t*);
%}
#include <stdint.h>
typedef struct wrapper_z zbar_wrapper_t;
typedef struct symbolset_z zw_symbolset_t;
typedef struct symbol_z{
    int x0, y0, x1, y1;
    char* symbol_data;
} zw_symbol_t;

extern zbar_wrapper_t* zbar_wrapper_init();
extern void zbar_wrapper_destroy(zbar_wrapper_t*);
extern zw_symbolset_t* zbar_wrapper_scan_RGBA32(zbar_wrapper_t*, uint8_t*, int, int);
extern zw_symbol_t* zbar_wrapper_symbol_next(zw_symbolset_t*);
extern void zbar_wrapper_symbol_destroy(zw_symbol_t*);
extern void zbar_wrapper_symbolset_destroy(zw_symbolset_t*);