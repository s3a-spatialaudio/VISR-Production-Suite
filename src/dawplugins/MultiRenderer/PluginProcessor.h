/*
 ==============================================================================
 
 This file was auto-generated!
 
 It contains the basic framework code for a JUCE plugin processor.
 
 ==============================================================================
 */

#ifndef MULTIRENDERER_PROCESSOR_H_INCLUDED
#define MULTIRENDERER_PROCESSOR_H_INCLUDED

#include <libdawutilities/PluginProcessorRender.h>
#include <libdawsupport/VISRComponentWrapperMulti.h>

#include <libdawsupport/visr_components/MetadataExposingRenderer.hpp>
#include <libdawsupport/visr_components/DualbandCapRenderer.hpp>
#include <libdawutilities/IDs.h>

namespace visr
{
namespace dawplugins
{
namespace multi_renderer
{
    
struct PointSource {
    PointSource (const juce::ValueTree& v) : state (v)
    {
//        jassert (v.hasType (IDs::pointSource));
    }
    int getID() const
    {
        return state[IDs::objID];
    }
    double getAz() const
    {
        return state[IDs::azimuth];
    }
    double getEl() const
    {
        return state[IDs::elevation];
    }
    void setAz (double az)
    {
//        jassert (time >= 0.0);
        state.setProperty (IDs::azimuth, fmax (0.0, az), nullptr);
    }
    void setEl (double el)
    {
//        jassert (time >= 0.0);
        state.setProperty (IDs::elevation, fmax (0.0, el), nullptr);
    }
    juce::ValueTree state;
};

class Processor  : public PluginProcessorRender,
public juce::ValueTree::Listener
{
public:
    Processor();
    ~Processor();
    void prepareToPlay (double sampleRate, int period)override;
    void processBlock (juce::AudioSampleBuffer& buffer, juce::MidiBuffer& midiMessages) override;
    const juce::String getName() const override {return JucePlugin_Name;};
    juce::AudioProcessorEditor* createEditor() override;
    void valueTreePropertyChanged (juce::ValueTree&, const juce::Identifier&) override;
    void valueTreeChildAdded (juce::ValueTree&, juce::ValueTree&) override{}
    void valueTreeChildRemoved (juce::ValueTree&, juce::ValueTree&, int) override {}
    void valueTreeChildOrderChanged (juce::ValueTree&, int, int) override {}
    void valueTreeParentChanged (juce::ValueTree&) override {}
    void PluginSetup() override;
    void PluginUnload() override;
    void PluginContextChanged() override;
    void PluginProcess(juce::AudioSampleBuffer&, juce::AudioSampleBuffer&) override{};
    
    std::unique_ptr<VISRComponentWrapperMulti> componentHandler;
    
    juce::ScopedPointer<visr::SignalFlowContext> context;
    
    void resetRenderer();
    
    enum RendererType
    {
        VBAP_NOVH = 0,   // VBAP algorithm
        VBAP_VH =1, // VBAP instance for virtual height deom
        VBAP_NOSD =2, // VBAP instance for stereo downmix
        VBAP_SD =3, // VBAP instance for stereo downmix
        CAP = 4,    // Compensated Amplitude Panning
        BINAURAL_STATIC = 5,    // Binaural renderer
        BINAURAL_DYNAMIC = 6,    // Binaural renderer
        REND_MAX = 7
    };
    
    RendererType rt;
    std::shared_ptr<visr::CompositeComponent> renderers [REND_MAX];
    visr::panning::LoudspeakerArray renderersLdspk[REND_MAX];
    
    std::map<RendererType,std::string> rendNames;
    juce::AudioSampleBuffer reroutingBuffer;
    
   
private:
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Processor)
};
    
    
} // namespace multi_renderer
} // namespace dawplugins
} // namespace visr
    
#endif
