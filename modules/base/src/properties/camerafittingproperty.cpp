/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2019 Inviwo Foundation
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

#include <modules/base/properties/camerafittingproperty.h>
#include <inviwo/core/util/stdextensions.h>
#include <modules/base/events/viewevent.h>

namespace inviwo {

const std::string CameraFittingProperty::classIdentifier = "org.inviwo.CameraFittingProperty";

std::string CameraFittingProperty::getClassIdentifier() const { return classIdentifier; }

CameraFittingProperty::CameraFittingProperty(std::string identifier, std::string displayName,
                                             CameraProperty* camera,
                                             std::function<std::optional<mat4>()> getBoundingBox)
    : CompositeProperty(identifier, displayName, InvalidationLevel::Valid)
    , camera_{camera}
    , getBoundingBox_{std::move(getBoundingBox)}
    , settings_("settings", "Settings")
    , updateNearFar_("updateNearFar", "Update Near/Far Distances", true)
    , updateLookRanges_("updateLookRanges", "Update Look-to/-from Ranges", true)
    , fittingRatio_("fittingRatio", "Fitting Ratio", 1.05f, 0, 2, 0.01f)
    , lookAt_{"lookAt", "Look At", buttons()}

    , setNearFarButton_("setNearFarButton", "Set Near/Far Distances", [this] { setNearFar(); })
    , setLookRangesButton_("setLookRangesButton", "Set Look-to/-from Ranges",
                           [this] { setLookRange(); }) {

    addProperties(lookAt_, settings_);
    settings_.addProperties(setNearFarButton_, setLookRangesButton_, updateNearFar_,
                            updateLookRanges_, fittingRatio_);
    settings_.setCollapsed(true);
}

CameraFittingProperty* CameraFittingProperty::clone() const {
    return new CameraFittingProperty(*this);
}

void CameraFittingProperty::invokeEvent(Event* event) {
    if (auto ve = event->getAs<ViewEvent>()) {
        std::visit(util::overloaded{[&](camerautil::Side side) { setView(side); },
                                    [&](ViewEvent::FlipUp) { flipUp(); },
                                    [&](ViewEvent::FitData) { fitData(); }

                   },
                   ve->getAction());

        ve->markAsUsed();
    } else {
        CompositeProperty::invokeEvent(event);
    }
}

std::vector<ButtonGroupProperty::Button> CameraFittingProperty::buttons() {
    return {
        {std::nullopt, ":svgicons/view-fit-to-data.svg", [this] { fitData(); }},
        {std::nullopt, ":svgicons/view-x-m.svg", [this] { setView(camerautil::Side::XNegative); }},
        {std::nullopt, ":svgicons/view-x-p.svg", [this] { setView(camerautil::Side::XPositive); }},
        {std::nullopt, ":svgicons/view-y-m.svg", [this] { setView(camerautil::Side::YNegative); }},
        {std::nullopt, ":svgicons/view-y-p.svg", [this] { setView(camerautil::Side::YPositive); }},
        {std::nullopt, ":svgicons/view-z-m.svg", [this] { setView(camerautil::Side::ZNegative); }},
        {std::nullopt, ":svgicons/view-z-p.svg", [this] { setView(camerautil::Side::ZPositive); }},
        {std::nullopt, ":svgicons/view-flip.svg", [this] { flipUp(); }}};
}

void CameraFittingProperty::setView(camerautil::Side side) {
    if (camera_ && getBoundingBox_) {
        if (auto bb = getBoundingBox_(); bb) {
            using namespace camerautil;
            setCameraView(*camera_, *bb, side, fittingRatio_,
                          updateNearFar_ ? UpdateNearFar::Yes : UpdateNearFar::No,
                          updateLookRanges_ ? UpdateLookRanges::Yes : UpdateLookRanges::No);
        }
    }
}

void CameraFittingProperty::fitData() {
    if (camera_ && getBoundingBox_) {
        if (auto bb = getBoundingBox_(); bb) {
            using namespace camerautil;
            setCameraView(*camera_, *bb, fittingRatio_,
                          updateNearFar_ ? UpdateNearFar::Yes : UpdateNearFar::No,
                          updateLookRanges_ ? UpdateLookRanges::Yes : UpdateLookRanges::No);
        }
    }
}

void CameraFittingProperty::flipUp() {
    if (camera_) {
        camera_->setLookUp(-camera_->getLookUp());
    }
}

void CameraFittingProperty::setNearFar() {
    if (camera_ && getBoundingBox_) {
        if (auto bb = getBoundingBox_(); bb) {
            camerautil::setCameraNearFar(*camera_, *bb);
        }
    }
}

void CameraFittingProperty::setLookRange() {
    if (camera_ && getBoundingBox_) {
        if (auto bb = getBoundingBox_(); bb) {
            camerautil::setCameraLookRanges(*camera_, *bb);
        }
    }
}

CameraFittingProperty::CameraFittingProperty(const CameraFittingProperty& rhs)
    : CompositeProperty(rhs)
    , camera_{rhs.camera_}
    , getBoundingBox_{rhs.getBoundingBox_}
    , settings_{rhs.settings_}
    , updateNearFar_{rhs.updateNearFar_}
    , updateLookRanges_{rhs.updateLookRanges_}
    , fittingRatio_{rhs.fittingRatio_}
    , lookAt_{rhs.lookAt_, buttons()}
    , setNearFarButton_{rhs.setNearFarButton_, [this] { setNearFar(); }}
    , setLookRangesButton_{rhs.setLookRangesButton_, [this] { setLookRange(); }} {

    addProperties(lookAt_, settings_);
    settings_.addProperties(setNearFarButton_, setLookRangesButton_, updateNearFar_,
                            updateLookRanges_, fittingRatio_);
}

CameraFittingProperty::CameraFittingProperty(std::string identifier, std::string displayName,
                                             CameraProperty& camera, VolumeInport& volumePort)
    : CameraFittingProperty(
          identifier, displayName, &camera, [vp = &volumePort]() -> std::optional<mat4> {
              if (vp->hasData()) {
                  return vp->getData()->getCoordinateTransformer().getDataToWorldMatrix();
              } else {
                  return std::nullopt;
              }
          }) {}

}  // namespace inviwo
