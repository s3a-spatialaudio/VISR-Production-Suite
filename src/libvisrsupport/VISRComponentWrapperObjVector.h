#ifndef VISRCOMPONENTWRAPPEROBJVECTOR_H_INCLUDED
#define VISRCOMPONENTWRAPPEROBJVECTOR_H_INCLUDED

#include "VISRObjectVectorObserver.h"
#include "VISRComponentWrapper.h"
#include "VISRListenerPosWrapper.h"
#include <libpml/object_vector.hpp>
#include <libcore/network/UDPReceiver.h>
#include <mutex>

namespace visr_production_suite
{
namespace visr_support
{
/**
 * This class is a wrapper around a VISR renderer.
 * It handles a received object-based scene, and features methods
 * to receive it from the network and to forward it to the wrapped VISR component.
 * @author Giacomo Costantini giacomo.costantini3@gmail.com
 */
class VISRSUPPORT_LIBRARY_SYMBOL VISRComponentWrapperObjVector:
public VISRComponentWrapper,
private core::network::UDPReceiver
{
public:
    /**
     * Creates an instance of this class.
     * @param dataModel The JUCE structure which contains the current state of the data model
     * (e.g. state of automatable parameters and of the object-based scene).
     * @param topLevelComponent VISR top-level component.
     * @param observers List of observers that are to be informed when updating the object-based scene
     * @param port UDP port to receive the object-based scene.
     * @param dynamicHeadTrackingSelect flag to enable/disable listener tracking input. Default is 0, which means internal tracking
     */
    VISRComponentWrapperObjVector(juce::AudioProcessorValueTreeState& dataModel,visr::CompositeComponent& topLevelComponent, std::vector < class VISRObjectVectorObserver * >& observers, int port, int dynamicHeadTrackingSelect=0,bool objvAfterProcessing=false);
    ~VISRComponentWrapperObjVector();
    
    visr::objectmodel::ObjectVector getScene();
    
protected:
    void handleMessage (const char* data, size_t dataSize) override;
private:
    /**
     * Method to forward the received object-based scene to the VISR renderer.
     */
    void forwardToComponent();
    
    /**
     * The object-based scene, which is a set of audio objects.
     */
    visr::objectmodel::ObjectVector scene;
    
    /**
     * Optional listener position wrapper to provide listener tracking
     */
    std::unique_ptr<VISRListenerPosWrapper> listener;
    
    /**
     * Set of scene observers
     */
    std::vector < class VISRObjectVectorObserver * >& sceneObservers;
    
    /**
     * Lock for atomic operations on the object-based scene
     */
    std::mutex compSceneLock;
    
    bool afterProcessing;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VISRComponentWrapperObjVector)
};

} // namespace visr_support
} // namespace visr_production_suite

#endif  // VISRCOMPONENTWRAPPEROBJVECTOR_H_INCLUDED
