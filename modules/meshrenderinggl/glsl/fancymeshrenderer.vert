/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2019-2021 Inviwo Foundation
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

#include "utils/structs.glsl"

uniform GeometryParameters geometry;
uniform CameraParameters camera;

out vData {
    vec4 worldPosition;
    vec4 position;
    vec3 normal;
#ifdef SEND_COLOR
    vec4 color;
#endif
#ifdef SEND_TEX_COORD
    vec2 texCoord;
#endif
#ifdef SEND_SCALAR
    float scalar;
#endif
}
vertex;

void main() {
#ifdef SEND_COLOR
    vertex.color = in_Color;
#endif
#ifdef SEND_TEX_COORD
    vertex.texCoord = in_TexCoord.xy;
#endif
#ifdef SEND_SCALAR
    vertex.scalar = in_TexCoord.x;  // TODO: specify source of scalar value
#endif
    vertex.worldPosition = geometry.dataToWorld * in_Vertex;
    vertex.normal = geometry.dataToWorldNormalMatrix * in_Normal * vec3(1.0);
    vertex.position = camera.worldToClip * vertex.worldPosition;
    gl_Position = vertex.position;
}