/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2021 Inviwo Foundation
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

#include <inviwo/customhistopathology/processors/customvolumeraycaster.h>
#include <inviwo/core/io/serialization/serialization.h>
#include <inviwo/core/io/serialization/versionconverter.h>
#include <inviwo/core/interaction/events/keyboardevent.h>
#include <modules/opengl/image/layergl.h>
#include <modules/opengl/volume/volumegl.h>
#include <modules/opengl/texture/texture2d.h>
#include <modules/opengl/texture/textureunit.h>
#include <modules/opengl/texture/textureutils.h>
#include <modules/opengl/shader/shaderutils.h>
#include <modules/opengl/volume/volumeutils.h>
#include <inviwo/core/common/inviwoapplication.h>
#include <inviwo/core/util/rendercontext.h>
#include <inviwo/core/algorithm/boundingbox.h>

namespace inviwo {

// The Class Identifier has to be globally unique. Use a reverse DNS naming scheme
const ProcessorInfo CustomVolumeRayCaster::processorInfo_{
    "org.inviwo.CustomVolumeRayCaster",     // Class identifier
    "Custom Volume Ray Caster",             // Display name
    "Undefined",                            // Category
    CodeState::Experimental,                // Code state
    "Custom, DVR, Raycasting",              // Tags
};
const ProcessorInfo CustomVolumeRayCaster::getProcessorInfo() const { return processorInfo_; }

CustomVolumeRayCaster::CustomVolumeRayCaster()
    : PoolProcessor()
    , shader_("customraycasting.frag", Shader::Build::No)
    , volumePort_("volume")
    , entryPort_("entry")
    , exitPort_("exit")
    , backgroundPort_("bg")
    , outport_("outport")
    , channel_("channel", "Render Channel", {{"Channel 1", "Channel 1", 0}}, 0)
    , raycasting_("raycaster", "Raycasting")
    , isotfComposite_Cell_("isotfComposite_Cell", "TF & IsoValues - Cell", &volumePort_,
                            InvalidationLevel::InvalidResources)
    , transferFunc_("transferFunction", "Transfer Function - 1", &volumePort_, 
                            InvalidationLevel::InvalidResources)
    , camera_("camera", "Camera", util::boundingBox(volumePort_))
    , lighting_("lighting", "Lighting", &camera_)
    , positionIndicator_("positionindicator", "Position Indicator")
    , toggleShading_("toggleShading", "Toggle Shading", [this](Event* e) { toggleShading(e);}, IvwKey::L)
    
{
    shader_.onReload([this]() { invalidate(InvalidationLevel::InvalidResources); });         

    addPort(volumePort_, "VolumePortGroup");
    addPort(entryPort_, "ImagePortGroup1");
    addPort(exitPort_, "ImagePortGroup1");
    addPort(outport_, "ImagePortGroup1");
    addPort(backgroundPort_, "ImagePortGroup1");

    backgroundPort_.setOptional(true);

    channel_.setSerializationMode(PropertySerializationMode::All);

    volumePort_.onChange([this]() {
        if (volumePort_.hasData()) {
            size_t channels = volumePort_.getData()->getDataFormat()->getComponents();

            if (channels == channel_.size()) return;

            std::vector<OptionPropertyIntOption> channelOptions;
            for (size_t i = 0; i < channels; i++) {
                channelOptions.emplace_back("Channel " + toString(i + 1),
                                            "Channel " + toString(i + 1), static_cast<int>(i));
            }
            channel_.replaceOptions(channelOptions);
            channel_.setCurrentStateAsDefault();
        }
    });
    backgroundPort_.onConnect([&]() { this->invalidate(InvalidationLevel::InvalidResources); });
    backgroundPort_.onDisconnect([&]() { this->invalidate(InvalidationLevel::InvalidResources); });

    // change the currently selected channel when a pre-computed gradient is selected
    raycasting_.gradientComputation_.onChange([this]() {
        if (channel_.size() == 4) {
            if (raycasting_.gradientComputation_.get() ==
                CustomRaycastingProperty::GradientComputation::PrecomputedXYZ) {
                channel_.set(3);
            } else if (raycasting_.gradientComputation_.get() ==
                       CustomRaycastingProperty::GradientComputation::PrecomputedYZW) {
                channel_.set(0);
            }
        }
    });

    addProperty(channel_);
    addProperty(raycasting_);
    addProperty(camera_);
    addProperty(lighting_);
    addProperty(positionIndicator_);
    addProperty(toggleShading_);

    /*
    * Added a custom transfer function
    * To change - take the colour value of the cells
    * 
    */
    TransferFunction tf({
                        {0.47644903694373958, vec4(0.78823531, 0.54509807, 0.80000001, 0.99101126)},
                        {0.079709668090377911, vec4(0,0,0,0)}, 
                        {0.62681159420289856, vec4(0.34509805, 0.046005949, 0.16066225, 0)}
                        });
    TransferFunctionProperty tf_new("transferFunction", "Transfer Function Cell", tf, &volumePort_);
    isotfComposite_Cell_.set(&tf_new);
    addProperty(isotfComposite_Cell_);

    addProperty(transferFunc_);
}

void CustomVolumeRayCaster::initializeResources() {
    utilgl::addDefines(shader_, raycasting_, isotfComposite_Cell_, camera_, lighting_, positionIndicator_);
    utilgl::addShaderDefinesBGPort(shader_, backgroundPort_);
    shader_.build();
}

void CustomVolumeRayCaster::process() {
    if (volumePort_.isChanged()) {
        dispatchOne(
            [volume = volumePort_.getData()]() {
                volume->getRep<kind::GL>();
                glFinish();
                return volume;
            },
            [this](std::shared_ptr<const Volume> volume) {
                raycast(*volume);
                newResults();
            });
    } else {
        raycast(*volumePort_.getData());
    }
}


void CustomVolumeRayCaster::raycast(const Volume& volume)
{
    if (!volume.getRep<kind::GL>()) {
        throw Exception("Could not find VolumeGL representation", IVW_CONTEXT);
    }
    utilgl::activateAndClearTarget(outport_);
    shader_.activate();

    TextureUnitContainer units;
    utilgl::bindAndSetUniforms(shader_, units, volume, "volume");
    utilgl::bindAndSetUniforms(shader_, units, isotfComposite_Cell_);
    utilgl::bindAndSetUniforms(shader_, units, entryPort_, ImageType::ColorDepthPicking);
    utilgl::bindAndSetUniforms(shader_, units, exitPort_, ImageType::ColorDepth);
    if (backgroundPort_.hasData()) {
        utilgl::bindAndSetUniforms(shader_, units, backgroundPort_, ImageType::ColorDepthPicking);
    }
    if (auto normals = entryPort_.getData()->getColorLayer(1)) {
        utilgl::bindAndSetUniforms(shader_, units,
                                   *normals->getRepresentation<LayerGL>()->getTexture(),
                                   std::string_view{"entryNormal"});
        shader_.setUniform("useNormals", true);
    } else {
        shader_.setUniform("useNormals", false);
    }

    // NEW-------------------
    utilgl::bindAndSetUniforms(shader_, units, transferFunc_);
    //NEW END----------------
    utilgl::setUniforms(shader_, outport_, camera_, lighting_, raycasting_, positionIndicator_,
                        channel_, isotfComposite_Cell_);

    utilgl::singleDrawImagePlaneRect();

    shader_.deactivate();
    utilgl::deactivateCurrentTarget();

}

void CustomVolumeRayCaster::toggleShading(Event*) {
    if (lighting_.shadingMode_.get() == ShadingMode::None) {
        lighting_.shadingMode_.set(ShadingMode::Phong);
    } else {
        lighting_.shadingMode_.set(ShadingMode::None);
    }
}

// override to do member renaming.
void CustomVolumeRayCaster::deserialize(Deserializer& d) {
    util::renamePort(d, {{&entryPort_, "entry-points"}, {&exitPort_, "exit-points"}});
    Processor::deserialize(d);
}

}  // namespace inviwo