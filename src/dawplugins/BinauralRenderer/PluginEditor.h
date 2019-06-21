#ifndef BINAURAL_RENDERER_EDITOR_H_INCLUDED
#define BINAURAL_RENDERER_EDITOR_H_INCLUDED

#include "JuceHeader.h"
#include "PluginProcessor.h"
#include <libcore/graphics/SceneWithHeadOrientation.h>
#include "Settings.h"
#include <libcore/graphics/PluginEditorBase.h>

namespace visr_production_suite
{
namespace dawplugins
{
namespace binaural_renderer
{

/**
 * Binaural Renderer's class for GUI handling.
 * It contains the basic framework code for a JUCE plugin editor.
 * @author Giacomo Costantini giacomo.costantini3@gmail.com
 */
class Editor  : public core::graphics::PluginEditorBase,
    public juce::Button::Listener,
    public juce::ComboBox::Listener,
    public juce::AsyncUpdater
{
public:
    Editor (Processor* op);
    ~Editor();
    
    juce::ScopedPointer<juce::TextButton> calibration;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;
    void populateRendererMenu();
    void toggleSettingsButton(bool isEnabled);
    
    Processor& getProcessor(){return op;}
    
protected:
    
    void resized() override;
    void handleAsyncUpdate() override;
    void comboBoxChanged (juce::ComboBox* cb) override;
    
private:
    
    Processor& op;
    
    /**
     * An object-based audio scene view with a visualisation 
     * of the yaw-pitch-roll orientation parameters of listener's head.
     */
    juce::ScopedPointer<core::graphics::SceneWithHeadOrientation> sceneHeadOrientationView;
    
    juce::ScopedPointer<juce::ImageButton> plugtitle;
    
    juce::MemoryOutputStream outStream;
    juce::ScopedPointer<juce::DatagramSocket> socketSend;
    
    juce::ScopedPointer<juce::ImageButton> settingsObj;
    juce::ScopedPointer<Settings> settings;
    
    juce::ScopedPointer<juce::ComboBox> rendererSelector;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Editor)
};
    
} // namespace binaural_renderer
} // namespace dawplugins
} // namespace visr_production_suite

#endif // BINAURAL_RENDERER_EDITOR_H_INCLUDED
