#ifndef PLUGINEDITOR_OBJECT_H_INCLUDED
#define PLUGINEDITOR_OBJECT_H_INCLUDED

#include "../export_symbols.hpp"
#include <libcore/processors/PluginProcessorBase.h>
#include "OpenGLBase.h"
#include "PluginEditorBase.h"
#include "Object.h"

namespace visr_production_suite
{
namespace core
{
namespace graphics
{

/**
 * Plugin window template for interactive panning. The window includes default panning Sliders
 * and a mouse-responsive graphic Panner.
 * @see OpenGLBase
 * @author Giacomo Costantini giacomo.costantini3@gmail.com
 */
class CORE_LIBRARY_SYMBOL PluginEditorObject  : public PluginEditorBase,
//public juce::ValueTree::Listener,
public juce::Slider::Listener
{
public:
    /**
     * Plugin editor constructor
     * @param processor reference to the processor for attaching the sliders to the automatable parameters
     */
    PluginEditorObject (processors::PluginProcessorBase* processor);
    ~PluginEditorObject();

protected:
    processors::PluginProcessorBase& op;
    
    void paint (juce::Graphics& g) override;
    void resized() override;
    void sliderValueChanged (juce::Slider* slider) override;

private:
    juce::ScopedPointer<juce::Slider> objAz;
    juce::ScopedPointer<juce::Slider> objEl;
    juce::ScopedPointer<juce::Slider> objRad;
    juce::ScopedPointer<juce::Slider> objID;
    juce::Label objAzLabel;
    juce::Label objElLabel;
    juce::Label objRadLabel;
    juce::Label objIDLabel;
    
    juce::ScopedPointer<juce::AudioProcessorValueTreeState::SliderAttachment> att_objAz;
    juce::ScopedPointer<juce::AudioProcessorValueTreeState::SliderAttachment> att_objEl;
    juce::ScopedPointer<juce::AudioProcessorValueTreeState::SliderAttachment> att_objRad;
    juce::ScopedPointer<juce::AudioProcessorValueTreeState::SliderAttachment> att_objID;
    juce::ScopedPointer<Object> metadataView;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditorObject)
};
    
} // namespace graphics
} // namespace core
} // namespace visr_production_suite

#endif
