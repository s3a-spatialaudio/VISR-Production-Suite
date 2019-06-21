#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "CurrentDefaultPaths.h"

using namespace juce;

// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new visr_production_suite::dawplugins::metadataeditor::Processor();
}


namespace visr_production_suite
{
namespace dawplugins
{
namespace metadataeditor
{

Processor::Processor()
:AudioProcessor (BusesProperties()
                 .withInput ("Input",  AudioChannelSet::mono(), true)
                 .withOutput ("Output", AudioChannelSet::mono(), true)),
parameters(*this,nullptr)
{
    parameters.state = ValueTree (Identifier ("MDOMetadata"));
//    parameters.state.addListener(this);
    
//#ifdef __APPLE__
//    presetDir = presetDir.getSpecialLocation(File::commonApplicationDataDirectory).getChildFile("Application Support").getChildFile("MetadataEditor");
//#else
//    presetDir = presetDir.getSpecialLocation(File::commonApplicationDataDirectory).getChildFile("MetadataEditor");
//#endif

    File configFile(core::defaultSrcPath+"/resources/metadataeditor.xml");
    
    if(configFile.exists())
        xmlModel = XmlDocument::parse(File::getCurrentWorkingDirectory().getChildFile (configFile.getFullPathName()));
    else{
        File configFile(core::defaultInstallPath+"/resources/metadataeditor.xml");
        if(configFile.exists())
            xmlModel = XmlDocument::parse(File::getCurrentWorkingDirectory().getChildFile (configFile.getFullPathName()));
    }
}

Processor::~Processor()
{}

//==============================================================================

AudioProcessorEditor* Processor::createEditor()
{
    return new Editor (this);
}

void Processor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();
    
    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
}

//==============================================================================
void Processor::getStateInformation (MemoryBlock& destData)
{
    ScopedPointer<XmlElement> xml (parameters.state.createXml());
    copyXmlToBinary (*xml, destData);
//    std::cout<<"SAVE STATE DUMP "<<parameters.state.toXmlString()<<std::endl;
 
}

void Processor::setStateInformation (const void* data, int sizeInBytes)
{
    ScopedPointer<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    
    if (xmlState != nullptr){
        if (xmlState->hasTagName (parameters.state.getType())){
//            std::cout<<"LOAD STATE DUMP "<<parameters.state.toXmlString()<<std::endl;
            parameters.state = ValueTree::fromXml (*xmlState);
        }
    }
}

bool Processor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool Processor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

} // namespace metadataeditor
} // namespace dawplugins
} // namespace visr_production_suite

