/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2013-2018 Inviwo Foundation
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

#include <inviwo/core/util/settings/systemsettings.h>
#include <inviwo/core/common/inviwoapplication.h>

namespace inviwo {

SystemSettings::SystemSettings(InviwoApplication* app)
    : Settings("System Settings", app)
    , applicationUsageMode_("applicationUsageMode", "Application usage mode",
                            {{"applicationMode", "Application Mode", UsageMode::Application},
                             {"developerMode", "Developer Mode", UsageMode::Development}},
                            1)
    , poolSize_("poolSize", "Pool Size", defaultPoolSize(), 0, 32)
    , enablePortInspectors_("enablePortInspectors", "Enable port inspectors", true)
    , portInspectorSize_("portInspectorSize", "Port inspector size", 128, 1, 1024)
#if __APPLE__
    , enableTouchProperty_("enableTouch", "Enable touch", false)
#else
    , enableTouchProperty_("enableTouch", "Enable touch", true)
#endif
    , enablePickingProperty_("enablePicking", "Enable picking", true)
    , enableSoundProperty_("enableSound", "Enable sound", true)
    , logStackTraceProperty_("logStackTraceProperty", "Error stack trace log", false)
    , followObjectDuringRotation_("followObjectDuringRotation",
                                  "Follow Object During Camera Rotation", false)
    , runtimeModuleReloading_("runtimeModuleReloding", "Runtime Module Reloading", false)
    , enableResourceManager_("enableResourceManager", "Enable Resource Manager", false) {

    addProperty(applicationUsageMode_);
    addProperty(poolSize_);
    addProperty(enablePortInspectors_);
    addProperty(portInspectorSize_);
    addProperty(enableTouchProperty_);
    addProperty(enablePickingProperty_);
    addProperty(enableSoundProperty_);
    addProperty(logStackTraceProperty_);
    addProperty(followObjectDuringRotation_);
    addProperty(runtimeModuleReloading_);
    addProperty(enableResourceManager_);

    logStackTraceProperty_.onChange(
        [this]() { LogCentral::getPtr()->setLogStacktrace(logStackTraceProperty_.get()); });

    runtimeModuleReloading_.onChange([this]() {
        if (isDeserializing_) return;
        LogInfo("Inviwo needs to be restarted for Runtime Module Reloading change to take effect");
    });

    load();
}

size_t SystemSettings::defaultPoolSize() { return std::thread::hardware_concurrency() / 2; }

}  // namespace inviwo
