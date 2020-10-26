/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2020 Inviwo Foundation
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

#include <modules/base/processors/gridsystem.h>

namespace inviwo {

// The Class Identifier has to be globally unique. Use a reverse DNS naming scheme
const ProcessorInfo GridSystem::processorInfo_{
    "org.inviwo.GridSystem",  // Class identifier
    "Grid System",            // Display name
    "Information",            // Category
    CodeState::Experimental,  // Code state
    Tags::None,               // Tags
};
const ProcessorInfo GridSystem::getProcessorInfo() const { return processorInfo_; }

GridSystem::GridSystem() : Processor() {
    transform_.setOptional(true);
    addPort(transform_);
    addPort(grid_);

    addProperties(enable_, spacing_, extent_, color_);
}

void GridSystem::process() {
    auto mesh = std::make_shared<ColoredMesh>();

    auto& ib = mesh->addIndexBuffer(DrawType::Lines, ConnectivityType::None)->getDataContainer();

    if (auto transform = transform_.getData()) {
        mesh->setModelMatrix(transform->getModelMatrix());
        mesh->setWorldMatrix(transform->getWorldMatrix());
    }

    std::array<std::vector<float>, 3> vals;
    for (unsigned i = 0; i < 3; i++) {
        const auto spacing = spacing_.get(i);
        const auto extent = extent_.get(i);

        int start = static_cast<int>(extent.x / spacing);
        const int end = static_cast<int>(extent.y / spacing);

        for (; start <= end; start++) {
            vals[i].emplace_back(start * spacing);
        }
    }

    auto createLines = [&](unsigned a, unsigned b, unsigned c) {
        const auto color = color_.get(a);
        const auto extentB = extent_.get(b);
        const auto extentC = extent_.get(c);

        for (auto v : vals[b]) {
            vec3 start{0.0f};
            vec3 end{0.0f};
            start[b] = v;
            end[b] = v;
            start[c] = extentC.x;
            end[c] = extentC.y;
            auto i0 = mesh->addVertex(start, color);
            auto i1 = mesh->addVertex(end, color);
            ib.push_back(i0);
            ib.push_back(i1);
        }
    };

    auto createGrid = [&](unsigned a, unsigned b, unsigned c) {
        if (!enable_.get(a)) return;
        createLines(a, b, c);
        createLines(a, c, b);
    };

    const static unsigned X = 0;
    const static unsigned Y = 1;
    const static unsigned Z = 2;

    createGrid(X, Y, Z);
    createGrid(Y, X, Z);
    createGrid(Z, X, Y);

    grid_.setData(mesh);
}
}  // namespace inviwo
