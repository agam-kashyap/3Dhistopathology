/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2012-2018 Inviwo Foundation
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

#ifndef IVW_BOOLCOMPOSITEPROPERTY_H
#define IVW_BOOLCOMPOSITEPROPERTY_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/properties/compositeproperty.h>
#include <inviwo/core/properties/boolproperty.h>

namespace inviwo {

/** 
 * \ingroup properties
 * \brief A CompositeProperty with an additional check box.
 */
class IVW_CORE_API BoolCompositeProperty : public CompositeProperty {
public:
    InviwoPropertyInfo();

    BoolCompositeProperty(std::string identifier, std::string displayName, bool checked = false,
                          InvalidationLevel invalidationLevel = InvalidationLevel::InvalidResources,
                          PropertySemantics semantics = PropertySemantics::Default);

    virtual BoolCompositeProperty* clone() const override;
    virtual ~BoolCompositeProperty();
    virtual std::string getClassIdentifierForWidget() const override;

    virtual bool isChecked() const;
    virtual void setChecked(bool checked);

    virtual operator bool&();
    virtual operator const bool&() const;

    virtual BoolProperty* getBoolProperty();

private:
    BoolProperty checked_;
};

}  // namespace

#endif  // IVW_BOOLCOMPOSITEPROPERTY_H
