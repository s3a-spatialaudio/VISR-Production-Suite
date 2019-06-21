#include "PluginProcessorLoudspeakers.h"
#include "../graphics/PluginEditorLoudspeakers.h"
#include "../IDs.h"

namespace visr_production_suite
{
namespace core
{
namespace processors
{

using namespace visr;
using namespace juce;

PluginProcessorLoudspeakers::PluginProcessorLoudspeakers(const BusesProperties& ioConfig):
PluginProcessorScene(ioConfig){}

PluginProcessorLoudspeakers::~PluginProcessorLoudspeakers(){}

AudioProcessorEditor* PluginProcessorLoudspeakers::createEditor()
{
    return new graphics::PluginEditorLoudspeakers (this);
}

void PluginProcessorLoudspeakers::prepareToPlay (double sampleRate, int period)
{
    if(isFirst || sampleRate != sr || period!= blockSize){
        parameterUpdatePeriod = period;
        PluginProcessorBase::prepareToPlay(sampleRate,period);
    }
}

void PluginProcessorLoudspeakers::VISRFindLayouts(File layoutFolder){
    loudspLayoutFiles.clear();
    layoutFolder.findChildFiles(loudspLayoutFiles, File::findFiles, true, "*.xml");
    lastDirectory = layoutFolder;
}

void PluginProcessorLoudspeakers::VISRChangeLayout()
{
    currentLayoutFile = proposedLayoutFile;
    loudspeakerArray = ldspkcheck;
    
    ValueTree loudspeakerConfiguration = parameters.state.getOrCreateChildWithName("loudspeakerConfiguration", nullptr);
    loudspeakerConfiguration.setProperty("ldspkFile",currentLayoutFile.getFullPathName(),nullptr);
}

int PluginProcessorLoudspeakers::VISRCheckLayout(File newLayout)
{
    if (!newLayout.existsAsFile())
    {
        throw std::invalid_argument("The file you selected doesn't exist");
    }
    proposedLayoutFile = newLayout;
    
    juce::String spklayout = proposedLayoutFile.getFullPathName();
    //  ldspktemp.reset(new visr::panning::LoudspeakerArray(spklayout.toStdString()));
    
    visr::panning::LoudspeakerArray ldspktemp;
    ldspktemp.loadXmlFile(spklayout.toStdString());
    int ldspnum =ldspktemp.getNumRegularSpeakers();
    int subnum =ldspktemp.getNumSubwoofers();
    int outputChannels = ldspnum+subnum;
    
    //FOR STANDALONE VERSION
    if(wrapperType==wrapperType_Standalone){
        deviceManager.initialise (512, 512, nullptr, true);
        AudioIODevice* device = deviceManager.getCurrentAudioDevice();
        const BigInteger activeInputChannels = device->getActiveInputChannels();
        const BigInteger activeOutputChannels = device->getActiveOutputChannels();
        //        int activeIn = activeInputChannels.getHighestBit() + 1;
        int activeOut = activeOutputChannels.getHighestBit() + 1;
        if(activeOut < outputChannels){
            //            std::cout<<"ldspchansize: "<<ldspktemp.getNumRegularSpeakers()<<" active "<<(int)(activeOutputChannels.getHighestBit()+1)<<std::endl;
            throw std::invalid_argument("Not enough channels\n Your audio device's output channels are not enough to match the loudspeaker configuration that you're trying to load. Please select a compatible loudspeaker configuration, or choose an audio device with more channels.");
        }
    }
    if(outputChannels>getTotalNumOutputChannels()) {
        throw std::invalid_argument("Not enough channels on track\n Your track's output channels are not enough to match the loudspeaker configuration that you're trying to load. Please select a compatible loudspeaker configuration, or give the track more channels.");
    }
    std::vector< visr::panning::LoudspeakerArray::ChannelIndex> channels(ldspktemp.getLoudspeakerChannels(), ldspktemp.getLoudspeakerChannels()+ldspnum);
    if( ldspktemp.getNumSubwoofers() > 0 ) // Dereference subwoofer channels only if there are subs.
    {
        channels.insert( channels.end(), ldspktemp.getSubwooferChannels(), ldspktemp.getSubwooferChannels() + subnum );
    }
    
    sort(channels.begin(),channels.end());
    bool containsGaps = false;
    if(std::adjacent_find( channels.begin(), channels.end(),
                          []( visr::panning::LoudspeakerArray::ChannelIndex const & lhs, visr::panning::LoudspeakerArray::ChannelIndex const & rhs ) { return lhs + 1 != rhs; } ) != channels.end() )
        
    {
        maxOutputChannels = *std::max_element(channels.begin(), channels.end());
        containsGaps = true;
        ldspkcheck = ldspktemp;
        return containsGaps;
    }
    
    maxOutputChannels = ldspktemp.getNumRegularSpeakers()+ldspktemp.getNumSubwoofers();
    ldspkcheck = ldspktemp;
    return containsGaps;
    //    if(!popupGapResult) return;
}

int PluginProcessorLoudspeakers::VISRCheckLayout(std::string newLayout)
{
    //  ldspkcheck.reset(new visr::panning::LoudspeakerArray());
    visr::panning::LoudspeakerArray ldspktemp;
    ldspktemp.loadXmlString( newLayout );
    
    int ldspnum =ldspktemp.getNumRegularSpeakers();
    int subnum =ldspktemp.getNumSubwoofers();
    int outputChannels = ldspnum+subnum;
    
    //FOR STANDALONE VERSION
    if(wrapperType==wrapperType_Standalone){
        deviceManager.initialise (512, 512, nullptr, true);
        AudioIODevice* device = deviceManager.getCurrentAudioDevice();
        const BigInteger activeInputChannels = device->getActiveInputChannels();
        const BigInteger activeOutputChannels = device->getActiveOutputChannels();
        //        int activeIn = activeInputChannels.getHighestBit() + 1;
        int activeOut = activeOutputChannels.getHighestBit() + 1;
        if(activeOut<outputChannels){
            //            std::cout<<"ldspchansize: "<<ldspktemp.getNumRegularSpeakers()<<" active "<<(int)(activeOutputChannels.getHighestBit()+1)<<std::endl;
            //            requestedChannels = getTotalNumOutputChannels();
            //            ldspkcheck = ldspktemp;
            throw std::invalid_argument("Not enough channels\n Your audio device's output channels are not enough to match the loudspeaker configuration that you're trying to load. Please select a compatible loudspeaker configuration, or choose an audio device with more channels.");
        }
    }
    if(outputChannels>getTotalNumOutputChannels()) {
        //        requestedChannels = getTotalNumOutputChannels();
        //        ldspkcheck = ldspktemp;
        throw std::invalid_argument("Not enough channels on track\n Your track's output channels are not enough to match the loudspeaker configuration that you're trying to load. Please select a compatible loudspeaker configuration, or give the track more channels.");
    }
    
    std::vector< visr::panning::LoudspeakerArray::ChannelIndex> channels(ldspktemp.getLoudspeakerChannels(), ldspktemp.getLoudspeakerChannels()+ldspnum);
    if( subnum > 0 )
    {
        channels.insert( channels.end(), ldspktemp.getSubwooferChannels(), ldspktemp.getSubwooferChannels() + subnum );
    }
    
    sort(channels.begin(),channels.end());
    
    bool containsGaps = false;
    if(std::adjacent_find( channels.begin(), channels.end(),
                          []( visr::panning::LoudspeakerArray::ChannelIndex const & lhs, visr::panning::LoudspeakerArray::ChannelIndex const & rhs ) { return lhs + 1 != rhs; } ) != channels.end() )
        
    {
        maxOutputChannels = *std::max_element(channels.begin(), channels.end());
        containsGaps = true;
        ldspkcheck = ldspktemp;
        return containsGaps;
    }
    
    maxOutputChannels = ldspktemp.getNumRegularSpeakers()+ldspktemp.getNumSubwoofers();
    ldspkcheck = ldspktemp;
    return containsGaps;
}

int PluginProcessorLoudspeakers::VISRCheckLayout(unsigned int newLayout)
{
    if (newLayout < (unsigned int)loudspLayoutFiles.size())
    {
        return VISRCheckLayout(loudspLayoutFiles.getUnchecked(newLayout));
    }
    throw std::invalid_argument("The dropdown menu item element number exceeds the number of layouts in the list");
}

void PluginProcessorLoudspeakers::getStateInformation (juce::MemoryBlock& destData)
{
    juce::ScopedPointer<juce::XmlElement> xml (parameters.state.createXml());
    if(xml->getChildByName(IDs::objectVector)!=nullptr)
        xml->getChildByName(IDs::objectVector)->deleteAllChildElements();
    
    copyXmlToBinary (*xml, destData);
    //    std::cout<<"NEWROW DUMP "<<parameters.state.toXmlString()<<std::endl;
}

void PluginProcessorLoudspeakers::prepareBuffer (AudioSampleBuffer& buffer )
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
