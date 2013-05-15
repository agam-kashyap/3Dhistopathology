#include <inviwo/core/datastructures/transferfunctiondatapoint.h>

namespace inviwo{
    TransferFunctionDataPoint::TransferFunctionDataPoint(){}
    TransferFunctionDataPoint::TransferFunctionDataPoint(vec2* pos):pos_(pos){}
    TransferFunctionDataPoint::TransferFunctionDataPoint(vec2* pos, vec4* rgba):pos_(pos), rgba_(rgba){}
    TransferFunctionDataPoint::~TransferFunctionDataPoint() {}

    const vec2* TransferFunctionDataPoint::getPos(){
        return pos_;
    }

    void TransferFunctionDataPoint::setPos( vec2* pos ){
        pos_ = pos;
    }

    const vec4* TransferFunctionDataPoint::getRgba(){
        return rgba_;
    }

    void TransferFunctionDataPoint::setRgba( vec4* rgba ){
        rgba_ = rgba;
    }
    void TransferFunctionDataPoint::setRgb( vec3* rgb ){
        rgba_->rgb = *rgb;
    }

    void TransferFunctionDataPoint::setA( float alpha ){
        rgba_->a = alpha;
    }

};