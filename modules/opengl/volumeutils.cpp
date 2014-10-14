/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2012-2014 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * Contact: Erik Sund�n
 *
 *********************************************************************************/

#include "volumeutils.h"
#include "glformats.h"

#include <inviwo/core/datastructures/volume/volume.h>
#include <inviwo/core/ports/volumeport.h>
#include <modules/opengl/glwrap/shader.h>

namespace inviwo {

namespace utilgl {

void setShaderUniforms(Shader* shader, const Volume* volume, const std::string& samplerID) {
    vec3 dimF = static_cast<vec3>(volume->getDimension());
    shader->setUniform(samplerID + ".dimensions_", dimF);
    shader->setUniform(samplerID + ".dimensionsRCP_", vec3(1.f) / dimF);
    shader->setUniform(samplerID + ".worldToTexture_",
                       volume->getCoordinateTransformer().getWorldToTextureMatrix());
    shader->setUniform(samplerID + ".textureToWorld_", volume->getCoordinateTransformer().getTextureToWorldMatrix());
    float gradientSpacing = volume->getWorldSpaceGradientSpacing();
    // Scale the world matrix by the gradient spacing and the transform it to texture space.
    // Note that since we are dealing with real values we can multiply the scalar after the transform as well
    shader->setUniform(samplerID + ".textureSpaceGradientSpacing_", gradientSpacing*mat3(volume->getCoordinateTransformer().getWorldToTextureMatrix()));
    shader->setUniform(samplerID + ".worldSpaceGradientSpacing_", gradientSpacing);

    dvec2 dataRange = volume->dataMap_.dataRange;
    DataMapper defaultRange(volume->getDataFormat());

    double typescale = 1.0 - getGLFormats()->getGLFormat(volume->getDataFormat()->getId()).scaling;
    defaultRange.dataRange = defaultRange.dataRange * typescale;

    double scalingFactor = 1.0;
    double signedScalingFactor = 1.0;
    double offset = 0.0;
    double signedOffset = 0.0;

    double invRange = 1.0 / (dataRange.y - dataRange.x);
    double defaultToDataRange = (defaultRange.dataRange.y - defaultRange.dataRange.x) * invRange;
    double defaultToDataOffset = (dataRange.x - defaultRange.dataRange.x) /
                                 (defaultRange.dataRange.y - defaultRange.dataRange.x);

    switch (getGLFormats()->getGLFormat(volume->getDataFormat()->getId()).normalization) {
        case GLFormats::NONE:
            scalingFactor = invRange;
            offset = -dataRange.x;
            signedScalingFactor = scalingFactor;
            signedOffset = offset;
            break;
        case GLFormats::NORMALIZED:
            scalingFactor = defaultToDataRange;
            offset = -defaultToDataOffset;
            signedScalingFactor = scalingFactor;
            signedOffset = offset;
            break;
        case GLFormats::SIGN_NORMALIZED:
            scalingFactor = 0.5 * defaultToDataRange;
            offset = 1.0 - 2 * defaultToDataOffset;
            signedScalingFactor = defaultToDataRange;
            signedOffset = -defaultToDataOffset;
            break;
    }
    // offset scaling because of reversed scaling in the shader, i.e. (1 - formatScaling_)
    shader->setUniform(samplerID + ".formatScaling_", static_cast<float>(1.0 - scalingFactor));
    shader->setUniform(samplerID + ".formatOffset_", static_cast<float>(offset));

    shader->setUniform(samplerID + ".signedFormatScaling_",
                       static_cast<float>(1.0 - signedScalingFactor));
    shader->setUniform(samplerID + ".signedFormatOffset_", static_cast<float>(signedOffset));
}

void setShaderUniforms(Shader* shader, const VolumeInport& port, const std::string& samplerID) {
    setShaderUniforms(shader, port.getData(), samplerID);
}

}
}  // namespace
