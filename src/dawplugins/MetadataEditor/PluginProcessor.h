/*
 ==============================================================================
 
 This file was auto-generated!
 
 It contains the basic framework code for a JUCE plugin processor.
 
 ==============================================================================
 */

#ifndef METADATAEDITOR_PROCESSOR_H_INCLUDED
#define METADATAEDITOR_PROCESSOR_H_INCLUDED

#include "JuceHeader.h"

namespace visr_production_suite
{
namespace dawplugins
{
namespace metadataeditor
{
/**
* @author Giacomo Costantini giacomo.costantini3@gmail.com
*/
class Processor  :
public juce::AudioProcessor,
public juce::ChangeBroadcaster
{
public:
    Processor();
    ~Processor();
    void prepareToPlay (double sampleRate, int period) override {};
    const juce::String getName() const override {return "VISR Metadata Editor";};
    
    juce::AudioProcessorEditor* createEditor() override;
    
    void processBlock (juce::AudioSampleBuffer& buffer, juce::MidiBuffer& midiMessages) override;
    
    
    void releaseResources() override {};
    double getTailLengthSeconds() const override{return 0.0;};
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool hasEditor() const override{return true;};
    int getNumPrograms() override{return 0;};
    int getCurrentProgram() override{
        return 0;
    };
    void setCurrentProgram (int index) override{
    };
    const juce::String getProgramName (int index) override {return juce::String();};
    void changeProgramName (int index, const juce::String& newName) override{};
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    bool silenceInProducesSilenceOut() const override{return false;};
    
    juce::ScopedPointer<juce::XmlElement> xmlModel;
    juce::AudioProcessorValueTreeState parameters;
    
private:
    juce::File presetDir;
    juce::AudioSampleBuffer inputBuffer;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Processor)
};
} // namespace metadataeditor
} // namespace dawplugins
} // namespace visr_production_suite


#endif
