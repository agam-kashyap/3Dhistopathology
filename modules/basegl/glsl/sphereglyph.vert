/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2015-2021 Inviwo Foundation
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
 *********************************************************************************/

// Owned by the SphereRenderer Processor

#include "utils/structs.glsl"

uniform GeometryParameters geometry;

uniform vec4 defaultColor = vec4(1, 0, 0, 1);
uniform float defaultRadius = 0.1f;
uniform sampler2D metaColor;

uniform float selectionMix = 0.0;
uniform float selectionScaleFactor = 1.0;
uniform vec4 selectionColor = vec4(1.0, 0.769, 0.247, 1.0);

out vec4 worldPosition;
out vec4 sphereColor;
flat out float sphereRadius;
flat out uint pickID;

void main(void) {
#if defined(HAS_SCALARMETA) && defined(USE_SCALARMETACOLOR) && !defined(FORCE_COLOR)
    sphereColor = texture(metaColor, vec2(in_ScalarMeta, 0.5));
#elif defined(HAS_COLOR) && !defined(FORCE_COLOR)
    sphereColor = in_Color;
#else
    sphereColor = defaultColor;
#endif
    sphereColor = mix(sphereColor, selectionColor, selectionMix);

#if defined(HAS_RADII) && !defined(FORCE_RADIUS)
    sphereRadius = in_Radii;
#else 
    sphereRadius = defaultRadius;
#endif
    sphereRadius = mix(sphereRadius, sphereRadius * selectionScaleFactor, selectionMix);

#if defined(HAS_PICKING)
    pickID = in_Picking;
#else 
    pickID = 0;
#endif

    worldPosition = geometry.dataToWorld * vec4(in_Position.xyz, 1.0);
    gl_Position = worldPosition;
}
