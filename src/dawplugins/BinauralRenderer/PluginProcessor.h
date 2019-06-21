#ifndef BINAURAL_RENDERER_PROCESSOR_H_INCLUDED
#define BINAURAL_RENDERER_PROCESSOR_H_INCLUDED

#include <libcore/processors/PluginProcessorScene.h>
#include <libvisrsupport/VISRComponentWrapperObjVector.h>
#include <libvisr/signal_flow_context.hpp>
#include "visr_components/binaural_renderer_component.hpp"
#include "visr_components/tracker_component.hpp"
#include <librrl/audio_signal_flow.hpp>
#include <libvisrsupport/VISRListenerPosWrapper.h>
#include <libvisrsupport/VISRListenerPosWrapper.h>
namespace visr_production_suite
{
namespace dawplugins
{
namespace binaural_renderer
{

/**
 * Binaural Renderer's main class.
 * Handles audio processing, VISR-related classes and VISR components, and instantiates the Editor object for GUI handling.
 * The name processor is due to the fact that it is a JUCE plugin processor.
 * @author Giacomo Costantini giacomo.costantini3@gmail.com
 */
class Processor  : public core::processors::PluginProcessorScene
{
public:
    Processor();
    ~Processor();
    //  void prepareToPlay (double sampleRate, int period)override;
    void processBlock (juce::AudioSampleBuffer& buffer, juce::MidiBuffer& midiMessages) override;
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
    const juce::String getName() const override {return "VISR Binaural Renderer";};
    juce::AudioProcessorEditor* createEditor() override;
    
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
    
    enum class RendererType
    {
        HRIR = 0,   // HRIR approach
        HOA =1, // HOA approach
        BRIR =2, // Virtual Loudspeaker approach
        REND_MAX = 3
    };
    
    
    void recalibrate();
    
    RendererType rt;
    std::map<RendererType,std::string> rendNames = {{RendererType::HRIR,"HRIR" }, {RendererType::HOA,"HOA"}, {RendererType::BRIR,"BRIR"}};
private:
    
    /**
     * The VISR audio context, which specifies the audio block size and the sample rate.
     */
    juce::ScopedPointer<visr::SignalFlowContext> context;
    
    /**
     * Instance of the top-level VISR component.
     */
    std::unique_ptr<BinauralRendererComponent> component;
    
    
    std::unique_ptr<TrackerComponent> trackerComponent;
    /**
     * Instance of a wrapper of an object-based VISR renderer.
     */
    std::unique_ptr<visr_support::VISRComponentWrapperObjVector> componentHandler;
    
    std::unique_ptr<visr_support::VISRComponentWrapper> componentHandlerTracker;
    
    bool useHeadTracking = false;
    
    /**
     * Set of component observers
     */
    std::vector < class VISRObjectVectorObserver * > views;
    
    juce::AudioDeviceManager  deviceManager;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Processor)
};

} // namespace binaural_renderer
} // namespace dawplugins
} // namespace visr_production_suite

#endif // BINAURAL_RENDERER_PROCESSOR_H_INCLUDED
