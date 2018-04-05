/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2014-2018 Inviwo Foundation
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

#ifndef IVW_PROPERTYFACTORYOBJECT_H
#define IVW_PROPERTYFACTORYOBJECT_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/properties/property.h>

namespace inviwo {

class IVW_CORE_API PropertyFactoryObject {
public:
    PropertyFactoryObject(const std::string& className);
    virtual ~PropertyFactoryObject();

    virtual std::unique_ptr<Property> create(std::string identifier,
                             std::string displayName) = 0;

    std::string getClassIdentifier() const;

private:
    std::string className_;
};

template<typename T>
class PropertyFactoryObjectTemplate : public PropertyFactoryObject {
public:
    PropertyFactoryObjectTemplate()
        : PropertyFactoryObject(T::CLASS_IDENTIFIER) {}

    virtual ~PropertyFactoryObjectTemplate() {}

    virtual std::unique_ptr<Property> create(std::string identifier,
                             std::string displayName) {
        return util::make_unique<T>(identifier, displayName);
    }
};

} // namespace

#endif // IVW_PROPERTYFACTORYOBJECT_H
