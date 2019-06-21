#include "VISRComponentWrapper.h"

using namespace visr;

namespace visr_production_suite
{
namespace visr_support
{
VISRComponentWrapper::VISRComponentWrapper( CompositeComponent& tlcomp):
res(true),
topLevelComponent(tlcomp)
{
    audioFlow.reset();
    audioFlow.reset(new rrl::AudioSignalFlow( topLevelComponent ));
}

VISRComponentWrapper::~VISRComponentWrapper(){
}

void VISRComponentWrapper::process( SampleType const * const * captureSamples, SampleType * const * playbackSamples){
    audioFlow->processFunction(audioFlow.get(),captureSamples,playbackSamples,res);
}

CommunicationProtocolBase::Input & VISRComponentWrapper::getOutputPort( char const * portName ){
    return audioFlow->externalParameterSendPort(portName);
}

CommunicationProtocolBase::Output & VISRComponentWrapper::getInputPort( char const * portName ){
    return audioFlow->externalParameterReceivePort(portName);
}

} // namespace visr_support
} // namespace visr_production_suite
