#ifndef LOUDSPEAKER_RENDERER_EDITOR_H_INCLUDED
#define LOUDSPEAKER_RENDERER_EDITOR_H_INCLUDED

#include "JuceHeader.h"
#include "PluginProcessor.h"
#include <libcore/graphics/SceneWithLoudspeakers.h>
#include <libcore/graphics/PluginEditorLoudspeakers.h>
#include "Settings.h"

namespace visr_production_suite
{
namespace dawplugins
{
namespace loudspeaker_renderer
{

/**
 * Loudspeaker Renderer's class for GUI handling.
 * It contains the basic framework code for a JUCE plugin editor.
 * @author Giacomo Costantini giacomo.costantini3@gmail.com
 */
class Editor  : public core::graphics::PluginEditorLoudspeakers
{
public:
    Editor (Processor* op);
    ~Editor();
    
    void toggleSettingsButton(bool isEnabled);
    
    Processor& getProcessor(){return op;}
    
protected:
    void buttonClicked (juce::Button* buttonThatWasClicked) override;
    void resized() override;
    
private:
    Processor& op;
    juce::ScopedPointer<juce::ImageButton> plugtitle;
    juce::ScopedPointer<juce::ImageButton> settingsBtn;
    juce::ScopedPointer<Settings> settings;
    
    /**
     * An object-based audio scene view with loudspeakers
     */
    juce::ScopedPointer<core::graphics::SceneWithLoudspeakers> audioSceneLdspkView;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Editor)
};

} // namespace loudspeaker_renderer
} // namespace dawplugins
} // namespace visr_production_suite

#endif
