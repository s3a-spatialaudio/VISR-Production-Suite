/*
 ==============================================================================
 
 This file was auto-generated!
 
 It contains the basic framework code for a JUCE plugin editor.
 
 ==============================================================================
 */

#ifndef MULTIRENDERER_EDITOR_H_INCLUDED
#define MULTIRENDERER_EDITOR_H_INCLUDED

#include "JuceHeader.h"
#include "PluginProcessor.h"
#include "PannerUI.h"
#include <libdawutilities/PluginEditorRender.h>

namespace visr
{
namespace dawplugins
{
namespace multi_renderer
{

class Editor  : public PluginEditorRender,
    public juce::ChangeListener
{
public:
    Editor (Processor* op, PannerUI* pui, int pannersize=30);
    ~Editor();
    Processor& op;
    void populateRendererMenu();
private:
    void resized() override;
    void changeListenerCallback (juce::ChangeBroadcaster *source) override;
    void handleAsyncUpdate() override;
    
    juce::ScopedPointer<juce::ImageButton> plugtitle;
    juce::Image juceImage;
    
    juce::ScopedPointer<juce::ComboBox> rendererSelector;
    void comboBoxChanged (juce::ComboBox*) override;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Editor)
};

    
} // namespace multi_renderer
} // namespace dawplugins
} // namespace visr

#endif
