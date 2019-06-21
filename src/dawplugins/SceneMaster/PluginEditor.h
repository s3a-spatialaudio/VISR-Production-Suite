#ifndef SCENEMASTER_EDITOR_H_INCLUDED
#define SCENEMASTER_EDITOR_H_INCLUDED

#include "JuceHeader.h"
#include "PluginProcessor.h"
#include "MetadataTable.h"
#include <libcore/graphics/PluginEditorBase.h>
#include <libcore/graphics/Scene.h>

namespace visr_production_suite
{
namespace dawplugins
{
namespace scene_master
{
/**
 * Scene Master's class for GUI handling.
 * It contains the basic framework code for a JUCE plugin editor.
 * @author Giacomo Costantini giacomo.costantini3@gmail.com
 */
class Editor  : public core::graphics::PluginEditorBase,
public juce::Button::Listener
{
public:
    Editor (Processor* op);
    ~Editor();
private:
    void resized() override;
    void paint (juce::Graphics& g) override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;
    
    Processor& op;
    
    juce::ScopedPointer<juce::ImageButton> plugtitle;
    juce::ScopedPointer<juce::Label> lo;
    juce::ScopedPointer<SceneTableModel> objTable;
    juce::ScopedPointer<juce::ImageButton> settingsObj;
    juce::Component::SafePointer<juce::DialogWindow> settingsObjDialog;
    
    /**
     * An object-based audio scene view
     */
    juce::ScopedPointer<core::graphics::Scene> audioSceneView;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Editor)
};
    
} // namespace scene_master    
} // namespace dawplugins
} // namespace visr_production_suite
#endif
