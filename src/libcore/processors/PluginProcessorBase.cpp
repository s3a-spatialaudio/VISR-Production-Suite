#include "PluginProcessorBase.h"

namespace visr_production_suite
{
namespace core
{
namespace processors
{
    
using namespace juce;

PluginProcessorBase::PluginProcessorBase(const BusesProperties& ioConfig):
AudioProcessor (ioConfig),
setupDone(false),
parameters(*this,nullptr)
{}

PluginProcessorBase::~PluginProcessorBase(){}

void PluginProcessorBase::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void PluginProcessorBase::prepareToPlay (double sampleRate, int period)
{
    if(isFirst||sampleRate != sr || period!= blockSize){
        sr = sampleRate;
        blockSize = period;
        PluginReset();
        isFirst=false;
    }
}

void PluginProcessorBase::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
}

void PluginProcessorBase::getStateInformation (MemoryBlock& destData)
{
    ScopedPointer<XmlElement> xml (parameters.state.createXml());
    copyXmlToBinary (*xml, destData);
}

void PluginProcessorBase::setStateInformation (const void* data, int sizeInBytes)
{
    ScopedPointer<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState != nullptr){
        if (xmlState->hasTagName (parameters.state.getType())){
            parameters.state = ValueTree::fromXml (*xmlState);
            PluginReset();
        }
    }
}

void PluginProcessorBase::PluginReset(){
    suspendProcessing(true);
    parameters.state.getOrCreateChildWithName("Error",nullptr).removeAllProperties(nullptr);
    try
    {
        PluginUnload();
        PluginSetup();
    }catch( std::exception const &ex )
    {
        String errorMsg;
        errorMsg << "Exception when reloading plugin components\n"<< ex.what();
        parameters.state.getOrCreateChildWithName("Error",nullptr).setProperty("ErrorMsg", errorMsg, nullptr);
        std::cerr << errorMsg << std::endl;
        return;
    }
    suspendProcessing(false);
}

} // namespace processors
} // namespace core
} // namespace visr_production_suite
