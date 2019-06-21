#ifndef VISRCOMPONENTWRAPPERJUCEPARAM_H_INCLUDED
#define VISRCOMPONENTWRAPPERJUCEPARAM_H_INCLUDED

#include "export_symbols.hpp"
#include "VISRComponentWrapper.h"

#include <libpml/scalar_parameter.hpp>
#include <libpml/double_buffering_protocol.hpp>
#include <libpml/message_queue_protocol.hpp>

namespace visr_production_suite
{
namespace visr_support
{

/**
 * This class is a wrapper around a VISR top-level component.
 * It maps and connects arbitrary JUCE parameters to the corresponding input/output VISR component's ports.
 * It is used when there is a one to one mapping between one or more JUCE parameters
 * and one or more corrensponding VISR ports.
 * @author Giacomo Costantini giacomo.costantini3@gmail.com
 */
class VISRSUPPORT_LIBRARY_SYMBOL VISRComponentWrapperJUCEParam :
public VISRComponentWrapper,
public juce::AudioProcessorValueTreeState::Listener
{
public:
    VISRComponentWrapperJUCEParam(juce::AudioProcessorValueTreeState& dataModel,visr::CompositeComponent& topLevelComponent);
    ~VISRComponentWrapperJUCEParam();
    
    /**
     * Maps and connects a JUCE parameter to the corresponding input VISR component's port.
     * @param paramID JUCE parameter's ID
     * @param compPortName VISR component's input port ID
     */
    void addParameterInput(juce::String paramID, const juce::String compPortName);
    
    /**
     * Maps and connects a VISR component's output port to the corresponding JUCE parameter.
     * @param compPortName VISR component's output port ID
     * @param paramID JUCE parameter's ID
     */
    void addParameterOutput( const juce::String& compPortName, juce::String& paramID);
    
    void process( float const * const * captureSamples, float * const * playbackSamples) override;
    
private:
    void parameterChanged (const juce::String& pID, float newVal) override;
    
    /**
     * Reference to the JUCE structure which contains the current state of the data model.
     * Here it is used to access the JUCE parameters that are also metadata of the listener.
     */
    juce::AudioProcessorValueTreeState& dataModel;
    
    //TODO make this class protocol-type-agnostic and parameterType-agnostic
    juce::Array<visr::pml::DoubleBufferingProtocol::OutputBase *> extReceivePorts;
    juce::Array<visr::pml::MessageQueueProtocol::InputBase *> extSendPorts;
    
    visr::pml::ScalarParameter<float> * tempFloatPar;
    visr::pml::ScalarParameter<int> * tempIntPar;
    const visr::pml::ScalarParameter<float> * outTempPar;
    
    juce::Array<juce::String> idsIn;
    juce::Array<juce::String> idsOut;
    std::map<juce::String, juce::String> jucePortConn;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VISRComponentWrapperJUCEParam)
};
} // namespace visr_support
} // namespace visr_production_suite

#endif  // VISRCOMPONENTWRAPPERJUCEPARAM_H_INCLUDED
