#ifndef PLUGIN_EDITOR_LOUDSPEAKERS_H_INCLUDED
#define PLUGIN_EDITOR_LOUDSPEAKERS_H_INCLUDED

#include <libcore/export_symbols.hpp>
#include <libcore/processors/PluginProcessorLoudspeakers.h>
#include "SceneWithLoudspeakers.h"
#include "PluginEditorBase.h"

namespace visr_production_suite
{
namespace core
{
namespace graphics
{

/**
 * Plugin window template for loudspeaker layout drawing and handling.
 * @author Giacomo Costantini giacomo.costantini3@gmail.com
 */
class CORE_LIBRARY_SYMBOL PluginEditorLoudspeakers  : public PluginEditorBase,
public juce::ComboBox::Listener,
public juce::Button::Listener,
public juce::AsyncUpdater
{
public:
    /**
     * Plugin editor constructor
     * @param processor reference to the audio processor
     */
    PluginEditorLoudspeakers (processors::PluginProcessorLoudspeakers* processor,  juce::Colour background = juce::Colour(40,40,40));
    
    ~PluginEditorLoudspeakers();
    
    /* Specific for ..Ldspk version */
    juce::ScopedPointer<juce::ComboBox> layoutSelector;
    bool isDefaultConfigOn;
    void resetLayout();
    processors::PluginProcessorLoudspeakers& op;
    
protected:
    void setup(SceneWithLoudspeakers* pui);
    void comboBoxChanged (juce::ComboBox* cb) override;
    void resized() override;
    
    SceneWithLoudspeakers* audioSceneLdspkView;
    void updateRings();
    
    juce::OwnedArray<juce::ToggleButton> ringbt;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;
    
    void handleAsyncUpdate() override;
private:
    void updatePopupDirectories();
    juce::ValueTree error;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditorLoudspeakers)
};
    
} // namespace graphics
} // namespace core
} // namespace visr_production_suite

#endif
