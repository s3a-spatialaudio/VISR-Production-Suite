#include "VISRComponentWrapperJUCEParam.h"

using namespace visr;
using namespace juce;

namespace visr_production_suite
{
namespace visr_support
{

VISRComponentWrapperJUCEParam::VISRComponentWrapperJUCEParam(AudioProcessorValueTreeState& model, CompositeComponent& tlcomp): VISRComponentWrapper(tlcomp)
,dataModel (model){}

VISRComponentWrapperJUCEParam::~VISRComponentWrapperJUCEParam(){
    extReceivePorts.clear();
    tempFloatPar = nullptr;
    tempIntPar = nullptr;
    outTempPar = nullptr;
    jucePortConn.clear();
    idsIn.clear();
    idsOut.clear();
}

void VISRComponentWrapperJUCEParam::addParameterInput(String paramID, const String compPortName){
    idsIn.add(paramID);
    dataModel.addParameterListener(paramID, this);
    jucePortConn[paramID] = compPortName;
    extReceivePorts.add(new pml::DoubleBufferingProtocol::OutputBase);
}

void VISRComponentWrapperJUCEParam::addParameterOutput( const String& compPortName, String& paramID){
    idsOut.add(paramID);
    jucePortConn[paramID] = compPortName;
    extSendPorts.add(new pml::MessageQueueProtocol::InputBase);
}

void VISRComponentWrapperJUCEParam::parameterChanged (const String& pID, float newVal){
    int idx = idsIn.indexOf(pID);
    const char * extPort = jucePortConn.at(pID).toRawUTF8();
    extReceivePorts.set(idx,  dynamic_cast<pml::DoubleBufferingProtocol::OutputBase *>(&audioFlow->externalParameterReceivePort(extPort)));
    if(extReceivePorts[idx]->data().type()==pml::ScalarParameter<float>().type()){
        tempFloatPar = dynamic_cast<pml::ScalarParameter<float> *>(&(extReceivePorts[idx]->data()));
        if(tempFloatPar!=nullptr)
            *tempFloatPar = newVal;
    }else if(extReceivePorts[idx]->data().type()==pml::ScalarParameter<int>().type()){
        tempIntPar = dynamic_cast<pml::ScalarParameter<int> *>(&(extReceivePorts[idx]->data()));
        if(tempIntPar!=nullptr)
            *tempIntPar = (int)newVal;
    }
    extReceivePorts[idx]->swapBuffers();
}

void VISRComponentWrapperJUCEParam::process( SampleType const * const * captureSamples, SampleType * const * playbackSamples){
    for(int i=0; i<extSendPorts.size();i++){
        const char * extPort = jucePortConn.at(idsOut[i]).toRawUTF8();
        extSendPorts.set(i,  dynamic_cast<pml::MessageQueueProtocol::InputBase *>(&audioFlow->externalParameterSendPort(extPort)));
        
        while(!extSendPorts[i]->empty()){
            outTempPar = dynamic_cast<const pml::ScalarParameter<float> *>(&(extSendPorts[i]->front()));
            
            float normPar = dataModel.getParameterRange(idsOut[i]).convertTo0to1(outTempPar->value());
            dataModel.getParameter(idsOut[i])->setValue(normPar);
            extSendPorts[i]->pop();
        }
    }
    audioFlow->processFunction(audioFlow.get(),captureSamples,playbackSamples,res);
}

} // namespace visr_support
} // namespace visr_production_suite
