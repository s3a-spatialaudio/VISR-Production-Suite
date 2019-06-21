#ifndef VISROBJECTVECTORWRAPPER_H_INCLUDED
#define VISROBJECTVECTORWRAPPER_H_INCLUDED

#include "export_symbols.hpp"
#include "JuceHeader.h"
#include "VISRObjectVectorObserver.h"
#include <libobjectmodel/object_vector.hpp>
#include <libcore/network/UDPReceiver.h>
#include <libcore/network/UDPSender.h>
#include <map>
#include <mutex>

namespace visr_production_suite
{
namespace visr_support
{

/**
 * This class is a controller for the object-based scene.
 * Contains features for receiving and handling audio objects and forwarding the object-based scene.
 * @author Giacomo Costantini giacomo.costantini3@gmail.com
 */
class VISRSUPPORT_LIBRARY_SYMBOL VISRObjectVectorWrapper
:   private juce::Timer,
    public core::network::UDPReceiver,
    public core::network::UDPSender
{
public:
    /**
     * Constructor to create the object-based scene controller.
     */
    VISRObjectVectorWrapper(juce::ValueTree v, std::vector < VISRObjectVectorObserver * >& observers, int portIn, const std::string newTargetHost, const int newTargetPort);
    ~VISRObjectVectorWrapper();
    
    visr::objectmodel::ObjectVector getScene();
    
protected:
    /**
     * An internal timer updates the status of the scene and sends it to the Renderer with a given frequency
     */
    void timerCallback () override;
    
    /**
     * Serialises the obect vector and sends it to the renderer
     */
    void createMessage() override;
    
    /**
     * Handles the new metadata received from each object
     */
    void handleMessage (const char* data, size_t dataSize) override;
    
private:
    /**
     * A revised version of the visr::objectmodel::updateObjectVector with the addition of object based expiry time tagging. This is done to be able to remove inactive objects.
     */
    void updateObjectVectorAndTimestamp ( std::basic_istream<char> & message);
    
    
    /**
     * Set of scene observers
     */
    std::vector < VISRObjectVectorObserver * >& sceneObservers;
    
    /**
     * The object-based scene, which is a set of audio objects.
     */
    visr::objectmodel::ObjectVector scene;
    
    /**
     * Time window after an object is received, within which that object is considered alive. Expressed in milliseconds.
     */
    const int aliveObjectsTimeWindow = 1000;
    
    /**
     * A set of expiry times to be assigned to the objects based on their timestamps of arrival
     */
    std::map< int, juce::uint32> expiryTimes;
    
    /**
     * Lock for atomic operations on the scene
     */
    std::mutex mSceneLock;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VISRObjectVectorWrapper)
};
} // namespace visr_support
} // namespace visr_production_suite

#endif  // VISROBJECTVECTORWRAPPER_H_INCLUDED
