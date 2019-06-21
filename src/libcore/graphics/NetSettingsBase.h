#ifndef NETSETTINGSBASE_HEADER
#define NETSETTINGSBASE_HEADER

#include "../export_symbols.hpp"
#include "JuceHeader.h"
#include <libcore/processors/PluginProcessorBase.h>

namespace visr_production_suite
{
namespace core
{
namespace graphics
{

/**
 * Network settings window to be used when you just need to update a ValueTree 
 * without resetting a PluginProcessorBase after an ip and/or port change.
 * @author Giacomo Costantini giacomo.costantini3@gmail.com
 */
class CORE_LIBRARY_SYMBOL NetSettingsBase  : public juce::Component,
public juce::TextEditor::Listener,
public juce::AsyncUpdater
{
public:
    /**
     * Constructor to create the network settings window.
     * @param vt The ValueTree to be updated with the values of ip and port fields.
     * @param name The name to be prepended for the stored ip and port properties.
     */
    NetSettingsBase (juce::ValueTree vt, juce::String name);
    ~NetSettingsBase();

protected:
    void paint (juce::Graphics& g) override;
    void resized() override;
    void textEditorFocusLost (juce::TextEditor& ed) override;
    void textEditorReturnKeyPressed (juce::TextEditor& ed) override;

    /** Called back to do whatever task you need to be done asynchronously.
     *  @see juce::AsyncUpdater
     */
    void handleAsyncUpdate() override;
    
private:
    juce::String contentID;
    juce::ValueTree properties;
    juce::ScopedPointer<juce::TextEditor> ip;
    juce::ScopedPointer<juce::TextEditor> port;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NetSettingsBase)
};
    
} // namespace graphics
} // namespace core
} // namespace visr_production_suite

#endif   // NETSETTINGSBASE_HEADER
