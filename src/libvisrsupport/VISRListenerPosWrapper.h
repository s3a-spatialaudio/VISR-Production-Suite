#ifndef VISRLISTENERPOSWRAPPER_H_INCLUDED
#define VISRLISTENERPOSWRAPPER_H_INCLUDED

#include "export_symbols.hpp"
#include "JuceHeader.h"

#include <librrl/audio_signal_flow.hpp>
#include <libpml/double_buffering_protocol.hpp>

namespace visr_production_suite
{
namespace visr_support
{

/**
 * This class is a wrapper around a Listener Position object.
 * Contains features for handling the listener position and orientation.
 * @author Giacomo Costantini giacomo.costantini3@gmail.com
 */
class VISRSUPPORT_LIBRARY_SYMBOL VISRListenerPosWrapper
:
    public juce::AudioProcessorValueTreeState::Listener,
    public juce::Timer
{
public:
    /**
     * Constructor to create a Listener Position component wrapper.
     * @param dataModel The JUCE structure which contains the current state of the data model
     * (e.g. state of automatable parameters and of the object-based scene).
     * @param flow the audio flow where to inject the listener's metadata.
     */
    VISRListenerPosWrapper(juce::AudioProcessorValueTreeState& dataModel,visr::rrl::AudioSignalFlow& flow, int listenerPosEnabled);
    ~VISRListenerPosWrapper();
protected:
    void timerCallback () override;
    void parameterChanged (const juce::String& pID, float newVal) override;
    
private:
    /**
     * Reference to the JUCE structure which contains the current state of the data model. 
     * Here it is used to access the JUCE parameters that are also metadata of the listener.
     */
    juce::AudioProcessorValueTreeState& dataModel;

    /**
     * Reference to the plugin's audio flow where to inject the listener's metadata.
     */
    visr::rrl::AudioSignalFlow& audioFlow;
    
    /**
     * The X coordinate of listener's position with respect to the center 
     * of the loudspeaker configuration ( i.e. P(0,0,0) is the 3D center).
     * It is expressed in meters.
     */
    float lX = 0;
    
    /**
     * The Y coordinate of listener's position with respect to the center
     * of the loudspeaker configuration ( i.e. P(0,0,0) is the 3D center).
     * It is expressed in meters.
     */
    float lY = 0;
    
    /**
     * The Z coordinate of listener's position with respect to the center
     * of the loudspeaker configuration ( i.e. P(0,0,0) is the 3D center).
     * It is expressed in meters.
     */
    float lZ = 0;
    
    /**
     * Expresses the orientation of the listener by specifying the amount of rotation in the x-y plane.
     * It is expressed in radians.
     */
    float yaw = 0;
    /**
     * Expresses the orientation of the listener by specifying the amount of rotation in the x-z plane.
     * It is expressed in radians.
     */
    float pitch = 0;
    /**
     * Expresses the orientation of the listener by specifying the amount of rotation in the y-z plane.
     * It is expressed in radians.
     */
    float roll = 0;
    
    /** 0 = no dynamic tracking
     *  1 = tracking through OSC mapping of parameters
     *  2 = tracking internal to the VISR component
     */
    int trackingSelect;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VISRListenerPosWrapper)
};

} // namespace visr_support
} // namespace visr_production_suite

#endif  // VISRLISTENERPOSWRAPPER_H_INCLUDED
