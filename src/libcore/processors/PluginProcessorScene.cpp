#include "PluginProcessorScene.h"
#include "../IDs.h"

namespace visr_production_suite
{
namespace core
{
namespace processors
{

using namespace visr;
using namespace juce;

PluginProcessorScene::PluginProcessorScene(const BusesProperties& ioConfig):
PluginProcessorBase(ioConfig){}

PluginProcessorScene::~PluginProcessorScene(){}

void PluginProcessorScene::getStateInformation (juce::MemoryBlock& destData)
{
    juce::ScopedPointer<juce::XmlElement> xml (parameters.state.createXml());
    if(xml->getChildByName(IDs::objectVector)!=nullptr)
        xml->getChildByName(IDs::objectVector)->deleteAllChildElements();
    
    copyXmlToBinary (*xml, destData);
    //    std::cout<<"NEWROW DUMP "<<parameters.state.toXmlString()<<std::endl;
}

void PluginProcessorScene::prepareBuffer (AudioSampleBuffer& buffer )
{
    if(wrapperType==wrapperType_Standalone){
        buffer.setSize(maxOutputChannels, buffer.getNumSamples());
    }
    
    inputBuffer.clear();
    
    int j =0;
    for (int i=0; i < jmin(getBusCount(true),buffer.getNumChannels()); i++) {
        auto bufferBus = getBusBuffer (buffer, true, i);
        
        for (int w=0; w < bufferBus.getNumChannels(); w++) {
            inputBuffer.copyFrom(j, 0, bufferBus, w, 0, bufferBus.getNumSamples());
            j++;
        }
    }
    buffer.clear();
}
    
} // namespace processors
} // namespace core
} // namespace visr_production_suite
