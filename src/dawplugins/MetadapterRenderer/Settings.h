#ifndef __JUCE_SETTINGS_HEADER
#define __JUCE_SETTINGS_HEADER

#include "JuceHeader.h"


namespace visr_production_suite
{
namespace dawplugins
{
namespace metadapter_renderer
{

class Editor;        
class Settings  : public juce::Component,
//public juce::TextEditor::Listener,
public juce::ComboBox::Listener,
public juce::Button::Listener,
public juce::AsyncUpdater
{
public:
    Settings (Editor& editor);
    ~Settings();

    void paint (juce::Graphics& g) override;
    void resized() override;
    void refresh();
//    void textEditorFocusLost (juce::TextEditor& ed) override;
//    void textEditorReturnKeyPressed (juce::TextEditor& ed) override;
    void comboBoxChanged (juce::ComboBox* cb) override;
    void handleAsyncUpdate()  override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;
    
private:
    juce::String contentID;
    juce::ValueTree properties;
    juce::ScopedPointer<juce::TextEditor> maxNumRevObj;
    juce::ScopedPointer<juce::TextEditor> numDiscreteReflections;
    juce::ScopedPointer<juce::TextEditor> lateReverbLength;
    juce::ScopedPointer<juce::ComboBox> lateReverbDecorrelationFilters;
    juce::ScopedPointer<juce::ComboBox> metaConfigFile;
    juce::ScopedPointer<juce::TextEditor> oscControlPort;
    juce::ScopedPointer<juce::TextEditor> jsonControlPort;
    
    Editor& editor;
    
    juce::Label maxNumRevObj_l;
    juce::Label numDiscreteReflections_l;
    juce::Label lateReverbLength_l;
    juce::Label lateReverbDecorrelationFilters_l;
    juce::Label metaConfigFile_l;
    juce::Label oscControlPort_l;
    juce::Label jsonControlPort_l;
    
    juce::Label metadapterSettings_l;
    juce::Label reverbSettings_l;
    
    juce::ScopedPointer<juce::FileChooser> fc;
    
    juce::ScopedPointer<juce::TextButton> save;
    juce::ScopedPointer<juce::TextButton> cancel;
    juce::PropertyPanel propertyPanel;
    
    juce::String findFileMsg = "Select from folder...";
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Settings)
};

    
} // namespace metadapter_renderer
} // namespace dawplugins
} // namespace visr_production_suite


#endif   // __JUCE_SETTINGS_HEADER
