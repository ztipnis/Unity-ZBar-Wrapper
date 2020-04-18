#include <zbar.h>
#include <stdint.h>
#include <cstring>
#include <memory>
#include <algorithm>
#include <utility>
#include <vector>
#include <array>
#include <limits>
#include "wrapper.h"

#define IS_BIG_ENDIAN (*(uint16_t *)"\0\xff" < 0x100)

namespace ZBWrapper{

class symbolSet{
private:
    std::vector<zbar::Symbol> symbols;
    std::vector<zbar::Symbol>::iterator iter;
public:

    symbolSet(const SymbolIterator begin, const SymbolIterator end) : iter(symbols.begin()){
        std::move(begin, end, std::back_inserter(symbols));
    }
    zw_symbol_t* nextSymbol(){
        if(iter != symbols.end()){
            zbar::Symbol sym = *(iter++);
            int symbolLen = sym.get_data_length();
            int minx=INT_MAX, maxx=INT_MIN, miny=INT_MAX, maxy=INT_MIN;
            for(auto piter = sym.point_begin(); piter != sym.point_end(); piter++){
                if(piter->x < minx){
                    mix = piter->x;
                }
                if(piter->x > maxx){
                    maxx = piter->x;
                }
                if(piter->y < miny){
                    miy = piter->y
                }
                if(piter->y > maxy){
                    maxy = piter->y
                }
            }
            zw_symbol_t* s = new zw_symbol_t;
            s->x0 = minx;
            s->x1 = maxx;
            s->y0 = miny;
            s->y1 = maxy;
            s->symbol_data = new char[symbolLen+1];
            std::strcpy(s->symbol_data, sym.get_data().c_str());
            return s;
        }else{
            return NULL;
        }
    }
    zw_symbolset_t* make_opaque(){ return static_cast<zw_symbolset_t*>(this);}



};

symbolSet* from_opaque(zw_symbolset_t* opaque){return static_cast<symbolSet*>(opaque);}

class wrapper{
    typedef uint8_t byte;
private:
    zbar::ImageScanner scanner;
public:
    void cfg_set_QR(bool on){
        scanner.set_config(ZBAR_QRCODE, ZBAR_CFG_ENABLE, (int)on);
    }
    void cfg_set_all(bool on){
        scanner.set_config(0, ZBAR_CFG_ENABLE, (int)on);
    }
    std::unique_ptr<symbolSet> scanBytesRGBA32(byte[] imgData, int width, int height){
        std::array<uint32_t, width*height> bArray;
        uint32_t* data = (uint32_t*)imgData; //dangerous cast from uint8_t to uint32_t. Will be wrong on little endian
        std::copy_n(data, width*height, std::begin(bArray));
        std::vector<uint32_t> rawData;
        if(!IS_BIG_ENDIAN){
            //Order is wrong
            //htonl is not portable without different headers on each system.
            //std::transform is
            std::transform(bArray.cbegin(), bArray.cend(), std::back_inserter(rawData), [](uint32_t reversed) -> uint32_t{
                uint8_t* reversedOrder = (uint8_t*)(&reversed);
                uint32_t correct = (reversedOrder[0] << 24) | (reversedOrder[1] << 16)
                                   | (reversedOrder[2] << 8) | (reversedOrder[3] <<0);
                return correct;
            });
        }else{
            std::move(bArray.begin(), bArray.end(), std::back_inserter(rawData));
        }
        zbar::Image img(width, height, "RBGA", rawData.data(), width*height);
        int n = scanner.scan(img);
        auto sset = std::make_unique<symbolSet>(img.symbol_begin(), img.symbol_end());
        img.set_data(NULL,0);
        return std::move(sset);
    }
    zbar_wrapper_t* make_opaque(){
        return static_cast<zbar_wrapper_t*>(this);
    }
};

wrapper* from_opaque(zbar_wrapper_t* opaque){return static_cast<wrapper*>(opaque);}
}



zbar_wrapper_t* zbar_wrapper_init(){
    ZBWrapper::wrapper* __wrapper__ = new ZBWrapper::wrapper;
    return __wrapper__->make_opaque();
}
void zbar_wrapper_destroy(zbar_wrapper_t* zbWrapper){
    ZBWrapper::wrapper __wrapper__ = from_opaque(zbWrapper);
    delete wrapper;
}
zw_symbolset_t* zbar_scan_RGBA32(zbar_wrapper_t* zbWrapper, uint8_t* imgData, int width, int height){
    ZBWrapper::wrapper* __wrapper__ = from_opaque(zbWrapper);
    ZBWrapper::symbolSet* __SymbolSet__ = __wrapper__->scanBytesRGBA32(imgData, width, height).release();
    return __SymbolSet__->make_opaque();
}
zw_symbol_t* zbar_symbol_next(zw_symbolset_t* ssPtr){
    ZBWrapper::symbolSet* __SymbolSet__ = from_opaque(ssPtr);
    return __SymbolSet__->nextSymbol();
}
void zbar_symbol_destroy(zw_symbol_t* sPtr){
    delete[] sPtr->symbol_data;
    delete sPtr;
}
void zbar_symbolset_destroy(zw_symbolset_t* ssPtr){
    ZBWrapper::symbolSet* __SymbolSet__ = from_opaque(ssPtr);
    delete __SymbolSet__;
}











