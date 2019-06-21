#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

#include "JuceHeader.h"
#include "../export_symbols.hpp"

namespace visr_production_suite
{
namespace core
{
namespace processors
{

/**
 * Core audio processor containing many feature which are in common to many plugins
 * @author Giacomo Costantini giacomo.costantini3@gmail.com
 */
class CORE_LIBRARY_SYMBOL PluginProcessorBase  : public juce::AudioProcessor
{
public:
    PluginProcessorBase(const BusesProperties& ioConfig);
    ~PluginProcessorBase();
    
    void prepareToPlay (double sampleRate, int period) override;
    void releaseResources() override;
    void processBlock (juce::AudioSampleBuffer& buffer, juce::MidiBuffer& midiMessages) override;
    double getTailLengthSeconds() const override{return 0.0;};
    bool acceptsMidi() const override{return false;};
    bool producesMidi() const override{return false;};
    bool hasEditor() const override{return true;};
    int getNumPrograms() override{return 0;};
    int getCurrentProgram() override{
        return 0;
    };
    void setCurrentProgram (int index) override{
    };
    const juce::String getProgramName(int index) override {return juce::String();};
    void changeProgramName (int index, const juce::String& newName) override{};
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    bool silenceInProducesSilenceOut() const override{return false;};
    
    /**
     * Stub function to perform everything that is needed to have a working audio processor
     */
    virtual void PluginSetup() = 0;
    /**
     * Stub function to perform everything that is needed to unload the audio processor
     */
    virtual void PluginUnload() = 0;
    
    void PluginReset();
    bool setupDone;
    
    juce::AudioProcessorValueTreeState parameters;
    
protected:
    bool isFirst = true;
    std::size_t sr = 48000;
    std::size_t blockSize = 1024;
    juce::AudioSampleBuffer inputBuffer;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessorBase)
};
    
} // namespace processors
} // namespace core
} // namespace visr_production_suite

#endif
