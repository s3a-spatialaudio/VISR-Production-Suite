#ifndef __JUCE_SETTINGS_HEADER
#define __JUCE_SETTINGS_HEADER

#include "JuceHeader.h"


namespace visr_production_suite
{
namespace dawplugins
{
namespace binaural_renderer
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
    juce::ScopedPointer<juce::TextEditor> maxNumObj;
    
    Editor& editor;
    juce::ScopedPointer<juce::ToggleButton> trackOff;
    juce::ScopedPointer<juce::ToggleButton> trackOSC;
    juce::ScopedPointer<juce::ToggleButton> trackInternal;
    juce::ScopedPointer<juce::TextEditor> trackInternalDevice;
    juce::ScopedPointer<juce::TextEditor> trackInternalParams;
    
    
    
//    juce::ScopedPointer<juce::ToggleButton> maxNumObj;
    
    juce::ScopedPointer<juce::ComboBox> hrirSOFA;
    juce::ScopedPointer<juce::ToggleButton> hrirDynITD;
    juce::ScopedPointer<juce::ToggleButton> hrirInterp;
    juce::ScopedPointer<juce::ToggleButton> hrirCrossf;
    
    juce::ScopedPointer<juce::ComboBox> hoaSOFA;
    
    juce::ScopedPointer<juce::ComboBox> brirSOFA;
    juce::ScopedPointer<juce::ComboBox> brirLdspk;
    juce::ScopedPointer<juce::ToggleButton> brirDynITD;
    juce::ScopedPointer<juce::ToggleButton> brirInterp;
    juce::ScopedPointer<juce::ToggleButton> brirCrossf;
    juce::ScopedPointer<juce::TextEditor> brirObjRendererOpt;
    juce::ScopedPointer<juce::TextEditor> brirTruncationLength;
    
    juce::Label general_l;
    juce::Label tracking_l;
    juce::Label hrir_l;
    juce::Label hoa_l;
    juce::Label brir_l;
    
    juce::Label trackOff_l;
    juce::Label trackOSC_l;
    juce::Label trackInternal_l;
    juce::Label trackInternalDevice_l;
    juce::Label trackInternalParams_l;
    
    juce::Label maxNumObj_l;
    
    juce::Label hrirSOFA_l;
    juce::Label hrirDynITD_l;
    juce::Label hrirInterp_l;
    juce::Label hrirCrossf_l;
    
    juce::Label hoaSOFA_l;
    
    juce::Label brirSOFA_l;
    juce::Label brirLdspk_l;
    juce::Label brirDynITD_l;
    juce::Label brirInterp_l;
    juce::Label brirCrossf_l;
    juce::Label brirObjRendererOpt_l;
    juce::Label brirTruncationLength_l;
    
    juce::ScopedPointer<juce::TextButton> save;
    juce::ScopedPointer<juce::TextButton> cancel;
    juce::ScopedPointer<juce::FileChooser> fc;
    juce::PropertyPanel propertyPanel;
    
    juce::String findFileMsg = "Select from folder...";
    
    juce::String defaultHRIRbuildpath = "/Users/costa/visr-production-suite/visr-production-suite/resources/SOFA/koln/HRIR_L2354.sofa";
    juce::String defaultHRIRinstallPath = "/Applications/VISR-Production-Suite-0.10.4/resources/SOFA/koln/HRIR_L2354.sofa";
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Settings)
};

    
} // namespace binaural_renderer
} // namespace dawplugins
} // namespace visr_production_suite


#endif   // __JUCE_SETTINGS_HEADER
