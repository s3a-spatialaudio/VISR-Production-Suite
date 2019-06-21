#ifndef VISRCOMPONENTWRAPPER_H_INCLUDED
#define VISRCOMPONENTWRAPPER_H_INCLUDED

#include "export_symbols.hpp"
#include "JuceHeader.h"
#include <librrl/audio_signal_flow.hpp>
#include <libvisr/composite_component.hpp>

namespace visr_production_suite
{
namespace visr_support
{

/**
 * This class is a wrapper around a VISR top-level component.
 * @author Giacomo Costantini giacomo.costantini3@gmail.com
 */
class VISRSUPPORT_LIBRARY_SYMBOL VISRComponentWrapper
{
public:
    /**
     * Instantiates the overall VISR audio flow using the provided VISR top-level component.
     * @param topLevelComponent VISR top-level component.
     */
    VISRComponentWrapper(visr::CompositeComponent& topLevelComponent);
    ~VISRComponentWrapper();
    
    /**
     * This member function performs the VISR top level component's audio processing.
     * It is called in the plugin's audio callback.
     * @param captureSamples input audio sample buffer.
     * @param playbackSamples output audio sample buffer.
     */
    virtual void process( float const * const * captureSamples,
                         float * const * playbackSamples);
    
    visr::CommunicationProtocolBase::Input & getOutputPort( char const * portName );
    visr::CommunicationProtocolBase::Output & getInputPort( char const * portName );
    
protected:
    /**
     * Stores the result of a call to the process member function
     */
    bool res;
    
    /**
     * Reference to the encapsulated VISR top-level component.
     */
    visr::CompositeComponent& topLevelComponent;
    
    /**
     * VISR audio flow to perform audio processing.
     */
    std::shared_ptr<visr::rrl::AudioSignalFlow> audioFlow;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VISRComponentWrapper)
};

} // namespace visr_support
} // namespace visr_production_suite

#endif  // VISRCOMPONENTWRAPPER_H_INCLUDED
