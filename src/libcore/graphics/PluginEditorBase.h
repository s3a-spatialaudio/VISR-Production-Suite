#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include "../export_symbols.hpp"
#include "JuceHeader.h"
#include "PluginLookAndFeel.h"

namespace visr_production_suite
{
namespace core
{
namespace graphics
{

/**
 * Plugin window template. It draws the main logo, sets the layout and initialises the error panel.
 * @author Giacomo Costantini giacomo.costantini3@gmail.com
 */
class CORE_LIBRARY_SYMBOL PluginEditorBase  : public juce::AudioProcessorEditor
{
public:
    PluginEditorBase (juce::AudioProcessor* op, juce::Colour background = juce::Colour(40,40,40));
    PluginEditorBase();
    ~PluginEditorBase();

protected:
    void paint (juce::Graphics& g) override;
    void resized() override;
    
private:
    juce::ScopedPointer<juce::ImageButton> logo;
    juce::Image juceImage;
    PluginLookAndFeel pluginLookAndFeel;
    juce::TooltipWindow ttw;
    juce::Colour background;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditorBase)
};
    
} // namespace graphics
} // namespace core
} // namespace visr_production_suite

#endif




