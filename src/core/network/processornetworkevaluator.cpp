#include <inviwo/core/network/processornetworkevaluator.h>
#include <inviwo/core/processors/canvasprocessor.h>
#include <inviwo/core/processors/progressbarowner.h>

namespace inviwo {

ProcessorNetworkEvaluator::ProcessorNetworkEvaluator(ProcessorNetwork* processorNetwork)
    : processorNetwork_(processorNetwork) { 
    registeredCanvases_.clear();
    initializeNetwork();
    defaultContext_ = 0;
    eventInitiator_ = 0;
    linkEvaluator_  = 0;
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
        if (!processors[i]->isInitialized())
            processors[i]->initialize();
    linkEvaluator_  = new LinkEvaluator();
}

void ProcessorNetworkEvaluator::registerCanvas(Canvas* canvas, std::string associatedProcessName) {    
    if (std::find(registeredCanvases_.begin(), registeredCanvases_.end(), canvas)!=registeredCanvases_.end()) {
        //already registered, hence force deregisterCanvas
        deregisterCanvas(canvas);
    }
    registeredCanvases_.push_back(canvas);
    canvas->setNetworkEvaluator(this);
    std::vector<CanvasProcessor*> canvasProcessors = processorNetwork_->getProcessorsByType<CanvasProcessor>();
    for (size_t i=0; i<canvasProcessors.size(); i++) {
        if (canvasProcessors[i]->getIdentifier() == associatedProcessName) {
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


void ProcessorNetworkEvaluator::saveSnapshotAllCanvases(std::string dir, std::string default_name, std::string ext){
    std::vector<inviwo::CanvasProcessor*> pv = processorNetwork_->getProcessorsByType<inviwo::CanvasProcessor>();
    int i = 0;
    for(std::vector<inviwo::CanvasProcessor*>::iterator it = pv.begin(); it != pv.end(); it++){
        std::stringstream ss;
        if (default_name == "" || default_name == "UPN")
            ss << (*it)->getIdentifier();
        else
            ss << default_name << i+1;
        std::string path(dir + ss.str() + ext);
        LogInfo("Saving canvas to: " + path);
        (*it)->createSnapshot((path).c_str());
        ++i;
    }
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

void ProcessorNetworkEvaluator::propagateInteractionEvent(Processor* processor, InteractionEvent* event) {
	if (!hasBeenVisited(processor)) {
		processorsVisited_.push_back(processor);
		std::vector<Processor*> directPredecessors = getDirectPredecessors(processor);
		for (size_t i=0; i<directPredecessors.size(); i++) {
			if (directPredecessors[i]->hasInteractionHandler())
				directPredecessors[i]->invokeInteractionEvent(event);
			// TODO: transform positions based on subcanvas arrangement
			//directPredecessors[i]->invalidate();  //TODO: Check if this is needed
			propagateInteractionEvent(directPredecessors[i], event);
		}
	}
}

void ProcessorNetworkEvaluator::propagateInteractionEvent(Canvas* canvas, InteractionEvent* event) {
	// find the canvas processor from which the event was emitted
	eventInitiator_=0;
	processorNetwork_->lock();
	std::vector<Processor*> processors = processorNetwork_->getProcessors();
	for (size_t i=0; i<processors.size(); i++) {
		if ((dynamic_cast<CanvasProcessor*>(processors[i])) &&
			(dynamic_cast<CanvasProcessor*>(processors[i])->getCanvas()==canvas)) {
				eventInitiator_ = processors[i];
				i = processors.size();
		}
	}

    if (!eventInitiator_) {
        processorNetwork_->unlock();
        return;
    }
	processorsVisited_.clear();
	propagateInteractionEvent(eventInitiator_, event);
	processorNetwork_->unlock();
	eventInitiator_ = 0;
	//eventInitiator->invalidate(); //TODO: Check if this is needed
}

/// /NEW ------------------------------------------------------------

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

Processor* ProcessorNetworkEvaluator::retrieveCanvasProcessor(Canvas* canvas) {
    // find the canvas processor which contains the canvas
    Processor* canvasProcessor = 0;
    std::vector<Processor*> processors = processorNetwork_->getProcessors();
    for (size_t i=0; i<processors.size(); i++) {
        if ((dynamic_cast<CanvasProcessor*>(processors[i])) &&
            (dynamic_cast<CanvasProcessor*>(processors[i])->getCanvas()==canvas)) {
                canvasProcessor = processors[i];
                i = processors.size();
        }
    }
    
    return canvasProcessor;
}

void ProcessorNetworkEvaluator::propagateResizeEvent(Canvas* canvas, ResizeEvent* resizeEvent) {
    if (processorNetwork_->islocked()) return;

    // avoid continues evaluation when port change
    processorNetwork_->lock();
    
    // find the canvas processor from which the event was emitted
	eventInitiator_= 0;
    eventInitiator_= retrieveCanvasProcessor(canvas);
    ivwAssert(eventInitiator_!=0, "Invalid resize event encountered.");

    // propagate size of canvas to all preceding processors through port
    // event initiator is a canvas processor, hence one ImageInport should exist
    ImageInport* imageInport = dynamic_cast<ImageInport*>(eventInitiator_->getInports()[0]);    
    imageInport->changeDataDimensions(resizeEvent);

    // enable network evaluation again
    processorNetwork_->unlock();

    // TODO: remove this invalidate
    // instead dimension property of event initiator (CanvasProcessor) should be invalid
    eventInitiator_->invalidate(PropertyOwner::INVALID_OUTPUT);
    eventInitiator_ = 0;
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
                connectedProperties.push_back(plinks[j]->getDestinationProperty());
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

std::vector<ProcessorLink*> ProcessorNetworkEvaluator::getSortedProcessorLinks() {
    std::vector<ProcessorLink*> unsortedProcessorLinks = processorNetwork_->getProcessorLinks();
    std::vector<ProcessorLink*> sortedProcessorLinks;

    ProcessorLink* nextInvalidLink = 0;

    //Find initial invalid link (whose property is currently modified)
    for (size_t i=0; i<unsortedProcessorLinks.size(); i++) { 
        if (!unsortedProcessorLinks[i]->isValid()) { 
            nextInvalidLink = unsortedProcessorLinks[i];
            break;
        }
    }

    if (!nextInvalidLink)  return sortedProcessorLinks; //return empty list

    //Find link connected to the invalid link.
    //If it does not exist pick any unsorted processor link and repeat.    
    while (sortedProcessorLinks.size()!=unsortedProcessorLinks.size()) {        
        sortedProcessorLinks.push_back(nextInvalidLink);
        
        Processor* srcProc = nextInvalidLink->getSourceProcessor();
        Processor* dstProc = nextInvalidLink->getDestinationProcessor();

        //Find link connected to the invalid link.
        nextInvalidLink = 0;
        for (size_t i=0; i<unsortedProcessorLinks.size(); i++) {
            if ( std::find(sortedProcessorLinks.begin(), sortedProcessorLinks.end(), unsortedProcessorLinks[i])==sortedProcessorLinks.end()) {
                std::vector<PropertyLink*> propertyLinks = unsortedProcessorLinks[i]->getPropertyLinks();                
                for (size_t j=0; j<propertyLinks.size(); j++) { 
                    if (propertyLinks[j]->getSourceProperty()->getOwner() == dstProc ||
                        propertyLinks[j]->getSourceProperty()->getOwner() == srcProc) {
                        nextInvalidLink = unsortedProcessorLinks[i];
                        break;
                    }
                }
            }
        }

        //Pick any unsorted link.
        if (!nextInvalidLink) {
            for (size_t i=0; i<unsortedProcessorLinks.size(); i++) {
                if ( std::find(sortedProcessorLinks.begin(), sortedProcessorLinks.end(), unsortedProcessorLinks[i])==sortedProcessorLinks.end()) {
                    if (!unsortedProcessorLinks[i]->isValid()) { 
                        nextInvalidLink = unsortedProcessorLinks[i];
                        break;
                    }
                }
            }
        }

        //there is no more invalid links
        if (!nextInvalidLink) break;        
    }

    return sortedProcessorLinks;
}


void ProcessorNetworkEvaluator::evaluate() {
    if (processorNetwork_->islocked())
        return;    
    
    // lock processor network to avoid concurrent evaluation
    processorNetwork_->lock();    
    bool repaintRequired = false;

    //perform linking
    std::vector<ProcessorLink*> processorLinks = getSortedProcessorLinks();        
    for (size_t i=0; i<processorLinks.size(); i++) {         
        if (!processorLinks[i]->isValid())
            processorLinks[i]->evaluate(linkEvaluator_);
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
            if (!processorsSorted_[i]->isReady())
                if (!dynamic_cast<CanvasProcessor*>(processorsSorted_[i]))
                    inValidTopology = true;
    if (inValidTopology) {
        processorNetwork_->unlock();
        return;
    }
   
    defaultContext_->activate();
    for (size_t i=0; i<processorsSorted_.size(); i++) {
        if (!processorsSorted_[i]->isValid()) {
            // re-initialize resources (e.g., shaders) if necessary
            if (processorsSorted_[i]->getInvalidationLevel() >= PropertyOwner::INVALID_RESOURCES)
                processorsSorted_[i]->initializeResources();                
            
            // reset the progress indicator
            ProgressBarOwner* progressBarOwner = dynamic_cast<ProgressBarOwner*>(processorsSorted_[i]);
            if (progressBarOwner)
                progressBarOwner->getProgressBar().resetProgress();

            // do the actual processing
            processorsSorted_[i]->process();
            repaintRequired = true;

            // set the progress indicator to finished
            if (progressBarOwner)
                progressBarOwner->getProgressBar().finishProgress();

            // validate processor
            processorsSorted_[i]->setValid();
        }
    }       

    // unlock processor network to allow next evaluation
    processorNetwork_->unlock();

    if (repaintRequired)
        for (size_t i=0; i<registeredCanvases_.size(); i++)
            registeredCanvases_[i]->repaint();
    defaultContext_->activate();
}

} // namespace
