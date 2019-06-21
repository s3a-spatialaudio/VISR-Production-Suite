#ifndef VSTWRAPPERMULTI_H_INCLUDED
#define VSTWRAPPERMULTI_H_INCLUDED

#include "VISRComponentWrapper.h"
#include <libcore/network/UDPReceiver.h>
#include <librrl/audio_signal_flow.hpp>
#include <libobjectmodel/object.hpp>
#include <libpml/object_vector.hpp>
#include <libpml/double_buffering_protocol.hpp>
#include <mutex>
#include <memory>

namespace visr_production_suite
{
namespace visr_support
{

//TODO discuss if and how we should put effort on making the MultiRenderer plugin usable.

/**
 * This class is a wrapper around a set of VISR top-level rendering components.
 * The role of this class is to make it easier to handle multiple audio flows and multiple renderers in a single plugin.
 * @author Giacomo Costantini giacomo.costantini3@gmail.com
 */
class VISRSUPPORT_LIBRARY_SYMBOL VISRComponentWrapperMulti :
public juce::AudioProcessorValueTreeState::Listener,
private core::network::UDPReceiver
{
public:
    //==============================================================================
    VISRComponentWrapperMulti(juce::AudioProcessorValueTreeState& param,  std::shared_ptr<visr::CompositeComponent>* renderers, int nRenderers);
    ~VISRComponentWrapperMulti();
    void setup();
    void addParameterInput(juce::String paramID, const juce::String compPortName);
    void process( float const * const * captureSamples,
                 float * const * playbackSamples);
    void update();
    
protected:
    void parameterChanged (const juce::String& pID, float newVal) override;
    void handleMessage (const char* data, size_t dataSize) override;
    
private:
    void sendObjectVector();
    void updateJuceObjectVector();
    
    std::shared_ptr<visr::CompositeComponent> flow;
    visr::pml::DoubleBufferingProtocol::OutputBase * listenerPort;
    visr::pml::ObjectVector * objv;
    std::mutex m;
    bool res;
    
    float lX = 0;
    float lY = 0;
    float lZ = 0;
    float yaw = 0;
    float pitch = 0;
    float roll = 0;
    std::shared_ptr<visr::rrl::AudioSignalFlow> audioFlow[7];
    std::shared_ptr<visr::CompositeComponent> *topLevelComponents;
    
    juce::ValueTree juceObjectVector;
    int currRenderer;
    visr::pml::DoubleBufferingProtocol::OutputBase * mObjectVectorOutput;
    juce::AudioProcessorValueTreeState& parameters;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VISRComponentWrapperMulti)
};

} // namespace visr_support
} // namespace visr_production_suite


#endif  // VSTWRAPPERMULTI_H_INCLUDED
