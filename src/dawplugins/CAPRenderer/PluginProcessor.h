#ifndef CAPRENDERER_PROCESSOR_H_INCLUDED
#define CAPRENDERER_PROCESSOR_H_INCLUDED

#include <libcore/processors/PluginProcessorLoudspeakers.h>
#include "visr_components/DualbandCapRenderer.hpp"
#include <libvisrsupport/VISRComponentWrapperObjVector.h>
#include <libcore/IDs.h>

namespace visr_production_suite
{
namespace dawplugins
{
namespace cap_renderer
{

class Processor  : public core::processors::PluginProcessorLoudspeakers
{
public:
    Processor();
    ~Processor();
    
//    void processBlockBypassed (juce::AudioBuffer<float>& buffer,
//                                       juce::MidiBuffer& midiMessages) override;
    const juce::String getName() const override {return "VISR CAP Renderer";};
    juce::AudioProcessorEditor* createEditor() override;
    void processBlock (juce::AudioSampleBuffer& buffer, juce::MidiBuffer& midiMessages) override;
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
    
    void PluginSetup() override;
    void PluginUnload() override;
    
    /**
     * Attach a component observer, e.g. a GUI element
     */
    void attach(VISRObjectVectorObserver *obs) {
        views.push_back(obs);
    }
    
    /**
     * Detach a component observer, e.g. a GUI element
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
     * Instance of the top-level VISR component.
     */
    std::unique_ptr<DualbandCapRenderer> component;
    juce::ScopedPointer<visr::SignalFlowContext> context;
    
    /**
     * Instance of a wrapper of an object-based VISR renderer.
     */
    std::unique_ptr<visr_support::VISRComponentWrapperObjVector> componentHandler;
    
    /**
     * Set of component observers
     */
    std::vector < VISRObjectVectorObserver * > views;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Processor)
};
    
} // namespace cap_renderer
} // namespace dawplugins
} // namespace visr_production_suite

#endif
