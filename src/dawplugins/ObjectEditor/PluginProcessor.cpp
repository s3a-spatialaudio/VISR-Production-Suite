#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <libpml/initialise_parameter_library.hpp>

// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new visr_production_suite::dawplugins::object_editor::Processor();
}

namespace visr_production_suite
{
namespace dawplugins
{
namespace object_editor
{

using namespace visr;
using namespace juce;
using namespace core;

Processor::Processor()
:PluginProcessorBase(BusesProperties()
                     .withInput ("Input",  AudioChannelSet::mono(), true)
                     .withOutput ("Output", AudioChannelSet::mono(), true))
{
    //objectID
    parameters.createAndAddParameter (std::make_unique<juce::AudioProcessorValueTreeState::Parameter>
                                      (IDs::objID.toString(), IDs::objID.toString(), IDs::objID.toString(),
                                       NormalisableRange<float> (1,64,1), 1, nullptr,nullptr,false,false,true));
    //azimuth angle
    parameters.createAndAddParameter (std::make_unique<juce::AudioProcessorValueTreeState::Parameter>
                                      (IDs::azimuth.toString(), IDs::azimuth.toString(), String(),
                                       NormalisableRange<float> (-180, 180,1), 0, nullptr,nullptr));
    //elevation angle
    parameters.createAndAddParameter (std::make_unique<juce::AudioProcessorValueTreeState::Parameter>
                                      (IDs::elevation.toString(), IDs::elevation.toString(), String(),
                                       NormalisableRange<float> (-90, 90,1), 0, nullptr,nullptr));
    //radius
    parameters.createAndAddParameter ( std::make_unique<juce::AudioProcessorValueTreeState::Parameter>
                                      (IDs::radius.toString(), IDs::radius.toString(), String(),
                                       NormalisableRange<float> (0.01f, 100, 0.01f), 1.f, nullptr,nullptr));
    
    parameters.state = ValueTree (Identifier ("ObjectEditor"));
    
    // Calling PluginReset() instead of PluginSetup(), because it includes exception handling and therefore PluginSetup() should not be called directly.
    PluginReset();
    isFirst = false;
}

Processor::~Processor(){}

bool Processor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    if(layouts.getMainOutputChannelSet() != AudioChannelSet::mono() || layouts.getMainInputChannelSet() != AudioChannelSet::mono())
        return false;
    return true;
}

void Processor::PluginSetup()
{
    pml::initialiseParameterLibrary();
    objectHandler.reset(new visr_support::VISRPointSourceWrapper(parameters));
    
    // Sets the audio object handler as a Listener to the automatable parameters that represent the metadata part of the audio object.
    parameters.addParameterListener(IDs::objID.toString(), objectHandler.get());
    parameters.addParameterListener(IDs::azimuth.toString(), objectHandler.get());
    parameters.addParameterListener(IDs::elevation.toString(), objectHandler.get());
    parameters.addParameterListener(IDs::radius.toString(), objectHandler.get());
}

void Processor::PluginUnload()
{
    // Un-sets the audio object handler as a Listener to the automatable parameters that represent the metadata part of the audio object.
    if(objectHandler!=nullptr){
        parameters.removeParameterListener(IDs::objID.toString(), objectHandler.get());
        parameters.removeParameterListener(IDs::azimuth.toString(), objectHandler.get());
        parameters.removeParameterListener(IDs::elevation.toString(), objectHandler.get());
        parameters.removeParameterListener(IDs::radius.toString(), objectHandler.get());
        objectHandler.reset();
    }
}

AudioProcessorEditor* Processor::createEditor()
{
    return new Editor (this);
}

} // namespace object_editor
} // namespace dawplugins
} // namespace visr_production_suite
