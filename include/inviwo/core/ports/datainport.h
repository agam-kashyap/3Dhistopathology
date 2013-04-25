#ifndef IVW_DATAINPORT_H
#define IVW_DATAINPORT_H

#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/ports/inport.h>

namespace inviwo {

template<typename T>
class DataOutport;

template<typename T>
class IVW_CORE_API DataInport : public Inport {

public:
    DataInport(std::string identifier);
    virtual ~DataInport();

    void initialize();
    void deinitialize();

    virtual void connectTo(Outport* port);
    virtual void disconnectFrom(Outport* port);

    virtual const T* getData() const;

    bool hasData() const;

protected:
    DataOutport<T>* dataOutport_;
};


template <typename T>
DataInport<T>::DataInport(std::string identifier)
    : Inport(identifier),
      dataOutport_(NULL)
{
}

template <typename T>
DataInport<T>::~DataInport() {}

template <typename T>
void DataInport<T>::initialize(){}

template <typename T>
void DataInport<T>::deinitialize(){}

template <typename T>
void DataInport<T>::connectTo(Outport* port) {
    DataOutport<T>* dataPort = dynamic_cast<DataOutport<T>*>(port);
    ivwAssert(dataPort!=NULL, "Trying to connect different port types.")
    Inport::connectTo(port);
    dataOutport_ = dataPort;
}

template <typename T>
void DataInport<T>::disconnectFrom(Outport* port) {
    Inport::disconnectFrom(port);
    dataOutport_ = NULL;
}

template <typename T>
const T* DataInport<T>::getData() const {
    if (isConnected())
        return dataOutport_->getConstData();
    else 
        return NULL;
}

template <typename T>
bool DataInport<T>::hasData() const {
    if (isConnected())
        return dataOutport_->hasData();
    else 
        return false;
}

} // namespace

#endif // IVW_DATAINPORT_H
