#ifndef VISROBJECTWRAPPER_H_INCLUDED
#define VISROBJECTWRAPPER_H_INCLUDED

#include "export_symbols.hpp"

#include "JuceHeader.h"
#include <libobjectmodel/object_vector.hpp>
#include <libcore/network/UDPReceiver.h>
#include <libcore/network/UDPSender.h>
#include <mutex>

namespace visr_production_suite
{
namespace visr_support
{

/**
 * This class is a wrapper of an audio object.
 * Contains features for handling audio object's metadata.
 * @author Giacomo Costantini giacomo.costantini3@gmail.com
 */
class VISRSUPPORT_LIBRARY_SYMBOL VISRObjectWrapper
:
    public juce::AudioProcessorValueTreeState::Listener,
    public core::network::UDPReceiver,
    public core::network::UDPSender,
    public juce::Timer
{
public:
    /**
     * Constructor to create an audio object wrapper.
     * @param param The JUCE structure which contains the automatable parameters,
     * which are the metadata associated to the object.
     * @param objtype The type of audio object to be instantiated.
     */
    VISRObjectWrapper(juce::AudioProcessorValueTreeState& param,visr::objectmodel::ObjectTypeId objtype);
    ~VISRObjectWrapper();
    
protected:
    void timerCallback () override;
    void parameterChanged (const juce::String& pID, float newVal) override;
    void createMessage() override;
    void handleMessage (const char* data, size_t dataSize) override;
    
    /**
     * Method to handle changes in object-type-specific metadata.
     * @param pID The ID of the plugin's automatable parameter.
     * @param newVal New value of the parameter.
     */
    virtual void VISRObjectChanged(const juce::String& pID, float newVal) = 0;
    
    /**
     * Method to handle the reception of object-type-specific metadata.
     * @param obj Represents the new status of the object, to be transfered to the current one.
     */
    virtual void VISRObjectReceived(visr::objectmodel::Object& obj) = 0;
    
    /**
     * Reference to the JUCE structure which contains the current state of the data model.
     * Here it is used to access the JUCE parameters that are also metadata of the audio object.
     */
    juce::AudioProcessorValueTreeState& dataModel;
    
    /**
     * An vector of objects used to take advantage of VISR object encoding/decoding features.
     * It contains the encapsulated audio object.
     */
    visr::objectmodel::ObjectVector objWrap;
    
    /**
     * Mutex object to serialise access to the wrapping object vector.
     */
    std::mutex objWrapLock;
private:
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VISRObjectWrapper)
};

} // namespace visr_support
} // namespace visr_production_suite

#endif  // VISROBJECTWRAPPER_H_INCLUDED
