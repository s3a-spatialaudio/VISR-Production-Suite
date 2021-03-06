#ifndef METADAPTER_RENDERER_PROCESSOR_H_INCLUDED
#define METADAPTER_RENDERER_PROCESSOR_H_INCLUDED

#include <libcore/processors/PluginProcessorLoudspeakers.h>
#include <libvisrsupport/VISRComponentWrapperObjVector.h>
#include "visr_components/MetadapterRenderer.hpp"

namespace visr_production_suite
{
namespace dawplugins
{
namespace metadapter_renderer
{

/**
 * Metadapter Renderer's main class.
 * Handles audio processing, VISR-related classes and VISR components, and instantiates the Editor object for GUI handling.
 * The name processor is due to the fact that it is a JUCE plugin processor.
 * @author Giacomo Costantini giacomo.costantini3@gmail.com
 */
class Processor  : public core::processors::PluginProcessorLoudspeakers
{
public:
    Processor();
    ~Processor();
    
    const juce::String getName() const override {return "VISR Metadapter Renderer";};
    juce::AudioProcessorEditor* createEditor() override;
    void processBlock (juce::AudioSampleBuffer& buffer, juce::MidiBuffer& midiMessages) override;
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
    
    void PluginSetup() override;
    void PluginUnload() override;
    
    /**
     * Attaches a component observer, e.g. a GUI element
     */
    void attach(VISRObjectVectorObserver *obs) {
        views.push_back(obs);
    }
    
    /**
     * Detaches a component observer, e.g. a GUI element
     */
    void detach(VISRObjectVectorObserver *obs) {
        auto findIt = std::find(views.begin(), views.end(), obs);
        if( findIt != views.end() ){ views.erase(findIt);}
    }
    
    /**
     * Retrieves the current object-based audio scene from the toplevel VISR component
     */
    visr::objectmodel::ObjectVector getScene() const override{
        if(componentHandler!=nullptr) return componentHandler->getScene();
        else return visr::objectmodel::ObjectVector();
    }
private:
    /**
     * The VISR audio context, which specifies the audio block size and the sample rate.
     */
    juce::ScopedPointer<visr::SignalFlowContext> context;
    
    /**
     * Instance of the top-level VISR component.
     */
    std::unique_ptr<MetadapterRenderer> component;
    
    /**
     * Instance of a wrapper of an object-based VISR renderer.
     */
    std::unique_ptr<visr_support::VISRComponentWrapperObjVector> componentHandler;
    
    juce::String const defaultConfig = "<?xml version=\"1.0\" encoding=\"utf-8\"?> <metadapter></metadapter>";
    
    /**
     * Set of component observers
     */
    std::vector < class VISRObjectVectorObserver * > views;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Processor)
};

} // namespace metadapter_renderer
} // namespace dawplugins
} // namespace visr_production_suite

#endif
