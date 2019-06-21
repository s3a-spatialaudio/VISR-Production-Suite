/*
 
 It contains the basic framework code for a JUCE plugin editor.
 */

#ifndef OBJECTEDITOR_EDITOR_H_INCLUDED
#define OBJECTEDITOR_EDITOR_H_INCLUDED

#include "JuceHeader.h"
#include "PluginProcessor.h"
#include <libcore/graphics/PluginEditorObject.h>
#include <libcore/graphics/NetSettings.h>

namespace visr_production_suite
{
namespace dawplugins
{
namespace object_editor
{

/**
 * Object Editor's class for GUI handling.
 * It contains the basic framework code for a JUCE plugin editor.
 * @author Giacomo Costantini giacomo.costantini3@gmail.com
 */
class Editor  :
public core::graphics::PluginEditorObject
{
public:
    Editor (Processor* op);
    ~Editor();
protected:
    void resized() override;
    
private:
    juce::ScopedPointer<juce::ImageButton> plugtitle;
    juce::Image juceImage2;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Editor)
};

} // namespace object_editor
} // namespace dawplugins
} // namespace visr_production_suite

#endif
