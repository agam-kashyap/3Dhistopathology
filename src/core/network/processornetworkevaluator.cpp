#include <inviwo/core/network/processornetworkevaluator.h>
#include <inviwo/core/processors/canvasprocessor.h>

namespace inviwo {

ProcessorNetworkEvaluator::ProcessorNetworkEvaluator(ProcessorNetwork* processorNetwork)
    : processorNetwork_(processorNetwork) { 
    registeredCanvases_.clear();
    initializeNetwork();
    defaultContext_ = 0;
}

ProcessorNetworkEvaluator::~ProcessorNetworkEvaluator() {}

void ProcessorNetworkEvaluator::activateDefaultRenderContext() {
    defaultContext_->activate();
}

void ProcessorNetworkEvaluator::initializeNetwork() {
    ivwAssert(processorNetwork_!=0, "processorNetwork_ not initialized, call setProcessorNetwork()");
    // initialize network
    std::vector<Processor*> processors = processorNetwork_->getProcessors();
    for (size_t i=0; i<processors.size(); i++)
        processors[i]->initialize();
}

void ProcessorNetworkEvaluator::registerCanvas(Canvas* canvas, std::string associatedProcessName) {
    canvas->setNetworkEvaluator(this);
    registeredCanvases_.push_back(canvas);
    std::vector<CanvasProcessor*> canvasProcessors = processorNetwork_->getProcessorsByType<CanvasProcessor>();
    for (size_t i=0; i<canvasProcessors.size(); i++) {
        if(canvasProcessors[i]->getIdentifier() == associatedProcessName) {
            canvasProcessors[i]->setCanvas(canvas);
        }
    }
}

void ProcessorNetworkEvaluator::deregisterCanvas(Canvas *canvas) {
    std::vector<CanvasProcessor*> canvasProcessors = processorNetwork_->getProcessorsByType<CanvasProcessor>();
    for (unsigned int i=0; i<canvasProcessors.size(); i++) {
        Canvas* curCanvas = canvasProcessors[i]->getCanvas();
        if (curCanvas==canvas) {
            if (std::find(registeredCanvases_.begin(), registeredCanvases_.end(), canvas)!=registeredCanvases_.end()) {
                canvas->setNetworkEvaluator(0);
                canvasProcessors[i]->setCanvas(0);
                registeredCanvases_.erase(std::remove(registeredCanvases_.begin(), registeredCanvases_.end(),
                                          canvas), registeredCanvases_.end());
                return;
            }
        }
    }
    LogError("Trying to deregister unregistered Canvas.");
}


bool ProcessorNetworkEvaluator::hasBeenVisited(Processor* processor) {
    for (size_t i=0; i<processorsVisited_.size(); i++)
        if (processorsVisited_[i] == processor)
            return true;
    return false;
}

std::vector<Processor*> ProcessorNetworkEvaluator::getDirectPredecessors(Processor* processor) {
    std::vector<Processor*> predecessors;
    std::vector<Inport*> inports = processor->getInports();
    std::vector<PortConnection*> portConnections = processorNetwork_->getPortConnections();
    for (size_t i=0; i<inports.size(); i++) {
        for (size_t j=0; j<portConnections.size(); j++) {
            const Port* curInport = portConnections[j]->getInport();
            if (curInport == inports[i]) {
                const Outport* connectedOutport = portConnections[j]->getOutport();
                predecessors.push_back(connectedOutport->getProcessor());
            }
        }
    }
    return predecessors;
}

void ProcessorNetworkEvaluator::traversePredecessors(Processor* processor) {
    if (!hasBeenVisited(processor)) {
        processorsVisited_.push_back(processor);
        std::vector<Processor*> directPredecessors = getDirectPredecessors(processor);
        for (size_t i=0; i<directPredecessors.size(); i++)
            traversePredecessors(directPredecessors[i]);
        processorsSorted_.push_back(processor);
    }
}

void ProcessorNetworkEvaluator::determineProcessingOrder() {
    std::vector<Processor*> processors = processorNetwork_->getProcessors();

    std::vector<Processor*> endProcessors;
    for (size_t i=0; i<processors.size(); i++)
        if (processors[i]->isEndProcessor())
            endProcessors.push_back(processors[i]);

    // perform topological sorting and store processor order
    // in processorsSorted_
    processorsSorted_.clear();
    processorsVisited_.clear();
    for (size_t i=0; i<endProcessors.size(); i++)
        traversePredecessors(endProcessors[i]);
}

void ProcessorNetworkEvaluator::propagateMouseEvent(Processor* processor, MouseEvent* mouseEvent) {
    if (!hasBeenVisited(processor)) {
        processorsVisited_.push_back(processor);
        std::vector<Processor*> directPredecessors = getDirectPredecessors(processor);
        for (size_t i=0; i<directPredecessors.size(); i++) {
            if (directPredecessors[i]->hasInteractionHandler())
                directPredecessors[i]->invokeInteractionEvent(mouseEvent);
            // TODO: transform positions based on subcanvas arrangement
            //directPredecessors[i]->invalidate();  //TODO: Check if this is needed
            propagateMouseEvent(directPredecessors[i], mouseEvent);
        }
    }
}

void ProcessorNetworkEvaluator::propagateMouseEvent(Canvas* canvas, MouseEvent* mouseEvent) {
    // find the canvas processor from which the event was emitted
    Processor* eventInitiator=0; 
    std::vector<Processor*> processors = processorNetwork_->getProcessors();
    for (size_t i=0; i<processors.size(); i++) {
        if ((dynamic_cast<CanvasProcessor*>(processors[i])) &&
            (dynamic_cast<CanvasProcessor*>(processors[i])->getCanvas()==canvas)) {
                eventInitiator = processors[i];
                i = processors.size();
        }
    }

    if(!eventInitiator) return;
    processorsVisited_.clear();
    propagateMouseEvent(eventInitiator, mouseEvent);
    //eventInitiator->invalidate(); //TODO: Check if this is needed
}

bool ProcessorNetworkEvaluator::isPortConnectedToProcessor(Port* port, Processor *processor) {
    bool isConnected = false;
    std::vector<PortConnection*> portConnections = processorNetwork_->getPortConnections();

    std::vector<Outport*> outports = processor->getOutports();   
    for (size_t i=0; i<outports.size(); i++) {
        for (size_t j=0; j<portConnections.size(); j++) {
            const Port* curOutport = portConnections[j]->getOutport();
            if (curOutport == outports[i]) {
                const Port* connectedInport = portConnections[j]->getInport();
                if (connectedInport == port) {
                    isConnected = true;
                    break;
                }
            }
        }
    }

    if (isConnected) return isConnected;

    std::vector<Inport*> inports = processor->getInports();   
    for (size_t i=0; i<inports.size(); i++) {
        for (size_t j=0; j<portConnections.size(); j++) {
            const Port* curInport = portConnections[j]->getInport();
            if (curInport == inports[i]) {
                const Outport* connectedOutport = portConnections[j]->getOutport();
                if (connectedOutport == port) {
                    isConnected = true;
                    break;
                }
            }
        }
    }

    return isConnected;

}

void ProcessorNetworkEvaluator::propagateResizeEvent(Processor* processor, ResizeEvent* resizeEvent) {
    if (!hasBeenVisited(processor)) {
        processor->invalidate();
        processorsVisited_.push_back(processor);
        std::vector<Processor*> directPredecessors = getDirectPredecessors(processor);
        for (size_t i=0; i<directPredecessors.size(); i++) {
            bool invalidate=false;
            
            if (directPredecessors[i]->hasInteractionHandler())
                directPredecessors[i]->invokeInteractionEvent(resizeEvent);
            
            std::vector<Outport*> outports = directPredecessors[i]->getOutports();
            for (size_t j=0; j<outports.size(); j++) {
                ImageOutport* imagePort = dynamic_cast<ImageOutport*>(outports[j]);
                if (imagePort) {
                    if (isPortConnectedToProcessor(imagePort, processor)) {
                        imagePort->changeDimensions(resizeEvent->canvasSize());
                        invalidate = true;
                    }
                }
            }
            
            std::vector<std::string> portGroups = directPredecessors[i]->getPortGroupNames();
            std::vector<Port*> ports;
            for (size_t j=0; j<portGroups.size(); j++) {
                ports.clear();
                ports = directPredecessors[i]->getPortsByGroup(portGroups[j]);

                uvec2 dimMax(0);
                for (size_t j=0; j<ports.size(); j++) {
                    ImageOutport* imageOutport = dynamic_cast<ImageOutport*>(ports[j]);
                    if (imageOutport) {
                        uvec2 dim = imageOutport->getDimensions();
                        //TODO: determine max dimension based on aspect ratio?
                        if ((dimMax.x<dim.x) || (dimMax.y<dim.y)) {
                            dimMax = imageOutport->getDimensions();
                        }
                    }
                }

                if(dimMax.x>0 && dimMax.y>0) {
                    for (size_t j=0; j<ports.size(); j++) {
                        ImageInport* imageInport = dynamic_cast<ImageInport*>(ports[j]);
                        if (imageInport) {
                            imageInport->changeDimensions(dimMax);
                        }
                    }
                }                
                
            }
            if (invalidate) directPredecessors[i]->invalidate();
            propagateResizeEvent(directPredecessors[i], resizeEvent);
        }
    }
}

void ProcessorNetworkEvaluator::propagateResizeEvent(Canvas* canvas, ResizeEvent* resizeEvent) {
    // find the canvas processor from which the event was emitted
    Processor* eventInitiator=0; 
    std::vector<Processor*> processors = processorNetwork_->getProcessors();
    for (size_t i=0; i<processors.size(); i++) {
        if ((dynamic_cast<CanvasProcessor*>(processors[i])) &&
            (dynamic_cast<CanvasProcessor*>(processors[i])->getCanvas()==canvas)) {
                eventInitiator = processors[i];
                i = processors.size();
        }
    }

    if(!eventInitiator) return;

    processorsVisited_.clear();
    propagateResizeEvent(eventInitiator, resizeEvent);

    bool invalidate=false;
    std::vector<Inport*> inports = eventInitiator->getInports();
    for (size_t j=0; j<inports.size(); j++) {
        ImageInport* imagePort = dynamic_cast<ImageInport*>(inports[j]);
        if (imagePort) {
            imagePort->changeDimensions(resizeEvent->canvasSize());
            invalidate = true;
        }
    }
    if (invalidate) eventInitiator->invalidate();
}

std::vector<PropertyLink*> ProcessorNetworkEvaluator::getConnectedPropertyLinks(Property* property) {
    std::vector<PropertyLink*> propertyLinks;
    std::vector<ProcessorLink*> links = processorNetwork_->getProcessorLinks();
    for (size_t i=0; i<links.size(); i++) {
        std::vector<PropertyLink*> plinks = links[i]->getPropertyLinks();
        for (size_t j=0; j<plinks.size(); j++) {
            if (plinks[j]->getSourceProperty()==property || plinks[j]->getDestinationProperty()==property) {
                propertyLinks.push_back(plinks[j]);
            }
        }
    }
    return propertyLinks;
}

std::vector<Property*> ProcessorNetworkEvaluator::getLinkedProperties(Property* property) {
    std::vector<Property*> connectedProperties;
    std::vector<ProcessorLink*> links = processorNetwork_->getProcessorLinks();
    for (size_t i=0; i<links.size(); i++) {
        std::vector<PropertyLink*> plinks = links[i]->getPropertyLinks();
        for (size_t j=0; j<plinks.size(); j++) {
            if (plinks[j]->getSourceProperty()==property) {
                connectedProperties.push_back( plinks[j]->getDestinationProperty());
            }            
        }
    }
    return connectedProperties;
}

bool ProcessorNetworkEvaluator::hasBeenVisited(Property* property) {
    if (std::find(propertiesVisited_.begin(), propertiesVisited_.end(), property)== propertiesVisited_.end())
        return false;
    return true;
}

void ProcessorNetworkEvaluator::evaluatePropertyLinks(Property* sourceProperty, Property* curProperty) {
    std::vector<Property*> linkedProperties = getLinkedProperties(curProperty);

    //Set current properties and its connected properties
    for (size_t i=0; i<linkedProperties.size(); i++) {
        if (!hasBeenVisited(linkedProperties[i])) {
            propertiesVisited_.push_back(linkedProperties[i]);
            linkEvaluator_->evaluate(sourceProperty, linkedProperties[i]);
            // TODO: Assumed that only one property can be invalid which is sourceProperty, 
            //       meaning user interacts with only one property at a time which is sourceProperty
            //       other properties should be assumed to be valid. hence setValid() is used.
            linkedProperties[i]->setValid();
            evaluatePropertyLinks(sourceProperty, linkedProperties[i]);
        }
    }
}

void ProcessorNetworkEvaluator::evaluatePropertyLinks(Property* sourceProperty) {
    propertiesVisited_.clear();
    //sourceProperty is considered to have the value already set. this value needs to be propagated.
    //Transfer values from sourceProperty to its connected properties recursively
    propertiesVisited_.push_back(sourceProperty);
    evaluatePropertyLinks(sourceProperty, sourceProperty);
}


void ProcessorNetworkEvaluator::evaluate() {
    if (processorNetwork_->islocked()) return;

    // lock processor network to avoid concurrent evaluation
    processorNetwork_->lock();

    std::vector<ProcessorLink*> processorLinks = processorNetwork_->getProcessorLinks();
    std::vector<Property*> sourceProperties; 
    for (size_t i=0; i<processorLinks.size(); i++) { 
        if (!processorLinks[i]->isValid()) { 
            //processorLinks[i]->evaluate(linkEvaluator_); 
            sourceProperties = processorLinks[i]->getSourceProperties(); 
            for (size_t j=0; j<sourceProperties.size(); j++) { 
                if (!sourceProperties[j]->isValid()) { 
                    evaluatePropertyLinks(sourceProperties[j]); 
                    sourceProperties[j]->setValid(); 
                } 
            } 
        }
        sourceProperties.clear();
    } 
  
    // if the processor network has changed determine the new processor order
    if (processorNetwork_->isModified()) {
        defaultContext_->activate();
        initializeNetwork();
        determineProcessingOrder();
        processorNetwork_->setModified(false);
    }

    bool inValidTopology = false;
    for (size_t i=0; i<processorsSorted_.size(); i++)
        if (!processorsSorted_[i]->isValid())
            if (!processorsSorted_[i]->allInportsConnected())
                if (!dynamic_cast<CanvasProcessor*>(processorsSorted_[i]))
                    inValidTopology = true;
    if (inValidTopology) {
        processorNetwork_->unlock();
        return;
    }

    // call before process on all processors
    for (size_t i=0; i<processorsSorted_.size(); i++)
        if (!processorsSorted_[i]->isValid())
            processorsSorted_[i]->beforeProcess();

    bool repaintRequired = false;
    defaultContext_->activate();
    for (size_t i=0; i<processorsSorted_.size(); i++) {
        if (!processorsSorted_[i]->isValid()) {
            // re-initialize resources (e.g., shaders) if necessary
            if (processorsSorted_[i]->getInvalidationLevel() >= Processor::INVALID_RESOURCES)
                processorsSorted_[i]->initializeResources();

            // do the actual processing
            processorsSorted_[i]->process();
            repaintRequired = true;
        }
    }

    // call after process on all processors and validate them
    for (size_t i=0; i<processorsSorted_.size(); i++)
        if (!processorsSorted_[i]->isValid()) {
            processorsSorted_[i]->afterProcess();
            processorsSorted_[i]->setValid();
        }

    // unlock processor network to allow next evaluation
    processorNetwork_->unlock();

    if (repaintRequired)
        for (size_t i=0; i<registeredCanvases_.size(); i++)
            registeredCanvases_[i]->repaint();
    defaultContext_->activate();
}

} // namespace
