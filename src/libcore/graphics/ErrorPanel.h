#ifndef ERRORPANEL_H_INCLUDED
#define ERRORPANEL_H_INCLUDED

#include "../export_symbols.hpp"
#include "JuceHeader.h"

namespace visr_production_suite
{
namespace core
{
namespace graphics
{

/**
 * Panel to show the catched errors in the plugin GUI.
 * @author Giacomo Costantini giacomo.costantini3@gmail.com 
 */
class CORE_LIBRARY_SYMBOL ErrorPanel  :
public juce::Component,
public juce::TextEditor::Listener
{
public:
    ErrorPanel();
    ~ErrorPanel(){};
    
    void textEditorTextChanged (juce::TextEditor& t) override;
    
    /**
     * Text field which is to be filled with the error message
     */
    juce::ScopedPointer<juce::TextEditor> errorText;
protected:
    void resized() override;
    void paint (juce::Graphics& g) override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ErrorPanel)
};
    
} // namespace graphics
} // namespace core
} // namespace visr_production_suite

#endif
