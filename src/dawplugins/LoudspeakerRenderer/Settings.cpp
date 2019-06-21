

#include "Settings.h"
#include "PluginEditor.h"

using namespace juce;

namespace visr_production_suite
{
namespace dawplugins
{
namespace loudspeaker_renderer
{
            
Settings::Settings (Editor& edit)
:
properties(edit.getProcessor().parameters.state),
editor(edit),
maxNumRevObj_l (String(), "Max # reverb objects:"),
numDiscreteReflections_l(String(), "Number of descrete reflections:"),
lateReverbLength_l(String(), "Late reverb length (s) :"),
lateReverbDecorrelationFilters_l(String(), "Late reverb decorrelation filters:"),
lateReverbCrossfadeTime_l(String(), "Crossfade Filter Length"),
reverbSettings_l(String(), "Reverb settings")
{
    //    std::cout<<" old valuetree "<<properties.toXmlString()<<std::endl;
    addAndMakeVisible(maxNumRevObj_l);
    addAndMakeVisible(numDiscreteReflections_l);
    addAndMakeVisible(lateReverbLength_l);
    addAndMakeVisible(lateReverbDecorrelationFilters_l);
    addAndMakeVisible(lateReverbCrossfadeTime_l);

    lateReverbDecorrelationFilters_l.setJustificationType(Justification::centred);
    
    reverbSettings_l.setJustificationType(Justification::centred);
    reverbSettings_l.setColour(Label::textColourId, Colours::black);
    addAndMakeVisible(reverbSettings_l);
    
    addAndMakeVisible (lateReverbDecorrelationFilters = new ComboBox());
    lateReverbDecorrelationFilters->addListener (this);
    lateReverbDecorrelationFilters->setEditableText(true);
    lateReverbDecorrelationFilters->setJustificationType (Justification::centred);
    lateReverbDecorrelationFilters->setScrollWheelEnabled(true);
    lateReverbDecorrelationFilters->setTextWhenNothingSelected("paste your path here");
    
    addAndMakeVisible (lateReverbLength = new TextEditor ("new text editor"));
    lateReverbLength->setMultiLine (false);
    lateReverbLength->setReturnKeyStartsNewLine (false);
    lateReverbLength->setReadOnly (false);
    lateReverbLength->setScrollbarsShown (false);
    lateReverbLength->setCaretVisible (true);
    lateReverbLength->setPopupMenuEnabled (true);
    lateReverbLength->setInputRestrictions(4,"1234567890.");
    
    addAndMakeVisible(save = new juce::TextButton("Ok"));
    save->addListener(this);
    addAndMakeVisible(cancel = new juce::TextButton("Cancel"));
    cancel->addListener(this);
    
    addAndMakeVisible (maxNumRevObj = new TextEditor ("new text editor"));
    maxNumRevObj->setMultiLine (false);
    maxNumRevObj->setReturnKeyStartsNewLine (false);
    maxNumRevObj->setReadOnly (false);
    maxNumRevObj->setScrollbarsShown (false);
    maxNumRevObj->setCaretVisible (true);
    maxNumRevObj->setPopupMenuEnabled (true);
    maxNumRevObj->setInputRestrictions(3,"1234567890");
    
    addAndMakeVisible (numDiscreteReflections = new TextEditor ("new text editor"));
    numDiscreteReflections->setMultiLine (false);
    numDiscreteReflections->setReturnKeyStartsNewLine (false);
    numDiscreteReflections->setReadOnly (false);
    numDiscreteReflections->setScrollbarsShown (false);
    numDiscreteReflections->setCaretVisible (true);
    numDiscreteReflections->setPopupMenuEnabled (true);
    numDiscreteReflections->setInputRestrictions(3,"1234567890");
    
    addAndMakeVisible (lateReverbCrossfadeTime = new TextEditor ("new text editor"));
    lateReverbCrossfadeTime->setMultiLine (false);
    lateReverbCrossfadeTime->setReturnKeyStartsNewLine (false);
    lateReverbCrossfadeTime->setReadOnly (false);
    lateReverbCrossfadeTime->setScrollbarsShown (false);
    lateReverbCrossfadeTime->setCaretVisible (true);
    lateReverbCrossfadeTime->setPopupMenuEnabled (true);
    lateReverbCrossfadeTime->setInputRestrictions(3,"1234567890");
    
    refresh();
    setSize (400, 550);
}

Settings::~Settings()
{
}

void Settings::refresh(){
    lateReverbDecorrelationFilters->clear(NotificationType::dontSendNotification);
    
    
    String path = properties.getOrCreateChildWithName("ReverbSettings", nullptr)
    .getProperty("LateReverbDecorrelationFiltersFile");
    if(!path.isEmpty()){
        lateReverbDecorrelationFilters->setText(path);
    }
    lateReverbDecorrelationFilters->addItem(findFileMsg,1);
    
    //    lateReverbDecorrelationFilters->setSelectedItemIndex(0,NotificationType::dontSendNotification);
    
    lateReverbLength->setText (properties.getOrCreateChildWithName("ReverbSettings", nullptr)
                               .getProperty("LateReverbLength"), juce::dontSendNotification);
    
    maxNumRevObj->setText (properties.getOrCreateChildWithName("ReverbSettings", nullptr)
                           .getProperty("MaximumReverbObjectNumber",0), juce::dontSendNotification);
    
    numDiscreteReflections->setText (properties.getOrCreateChildWithName("ReverbSettings", nullptr)
                                     .getProperty("DiscreteReflectionsNumber"), juce::dontSendNotification);
    lateReverbCrossfadeTime->setText(properties.getOrCreateChildWithName("ReverbSettings", nullptr)
                                     .getProperty("LateReverbCrossfadeTime"), juce::dontSendNotification);
}
//==============================================================================
void Settings::paint (Graphics& g)
{
    //    g.fillAll (Colours::white);
    
    g.setGradientFill (ColourGradient (Colour (0xff4e4e4e).withAlpha(0.8f),
                                       getWidth()/2, getHeight()/2,
                                       Colours::grey.withAlpha(0.8f),
                                       getWidth(), getHeight(),
                                       true));
    g.fillRect (0, 0, getWidth(),getHeight());
    
    g.setColour (Colours::black);
    auto bounds = getLocalBounds().reduced(5);
    
    g.drawLine(bounds.getX(), 50, bounds.getWidth(),50 ); //row1
    g.drawLine(bounds.getX(), 70, bounds.getWidth(),70 );
    
    g.drawLine(bounds.getX(), 435, bounds.getWidth(),435 ); //savearea
    g.drawLine(bounds.getX(), 470, bounds.getWidth(),470 );
    
    
}

void Settings::resized()
{
    auto bounds = getLocalBounds().reduced(10,0);
    bounds.removeFromTop (40);
    
    auto row =  bounds.removeFromTop (40);
    reverbSettings_l.setBounds(row.reduced(5));
    
    row = bounds.removeFromTop (30);
    maxNumRevObj_l.setBounds(row.removeFromLeft(200).reduced(5));
    maxNumRevObj->setBounds (row.removeFromLeft(30).reduced(0,5));
    
    row = bounds.removeFromTop (30);
    numDiscreteReflections_l.setBounds(row.removeFromLeft(200).reduced(5));
    numDiscreteReflections->setBounds (row.removeFromLeft(70).reduced(0,5));
    
    row = bounds.removeFromTop (30);
    lateReverbLength_l.setBounds(row.removeFromLeft(200).reduced(5));
    lateReverbLength->setBounds (row.removeFromLeft(70).reduced(0,5));
    
    row = bounds.removeFromTop (30);
    lateReverbDecorrelationFilters_l.setBounds(row.removeFromLeft(200).reduced(5));
    lateReverbDecorrelationFilters->setBounds (row.removeFromLeft(300).reduced(0,5));
    
    row = bounds.removeFromTop (30);
    lateReverbCrossfadeTime_l.setBounds(row.removeFromLeft(200).reduced(5));
    lateReverbCrossfadeTime->setBounds (row.removeFromLeft(300).reduced(0,5));
    
    auto saveArea = bounds.removeFromBottom(45).reduced(bounds.getWidth()/4,0);
    
    cancel->setBounds(saveArea.removeFromLeft(100).reduced(10));
    save->setBounds(saveArea.removeFromLeft(100).reduced(10));
    
}

void Settings::handleAsyncUpdate(){
    ValueTree reverbNode = properties.getOrCreateChildWithName("ReverbSettings", nullptr);
    String numrevobj = maxNumRevObj->getText();
    if(!numrevobj.isEmpty())
        reverbNode.setProperty("MaximumReverbObjectNumber",numrevobj,nullptr);
    
    String lrdPath = lateReverbDecorrelationFilters->getText();
    if(lrdPath!=lateReverbDecorrelationFilters->getTextWhenNothingSelected())
        reverbNode.setProperty("LateReverbDecorrelationFiltersFile",lrdPath,nullptr);
    
    String lrLength = lateReverbLength->getText();
    if(!lrLength.isEmpty())
        reverbNode.setProperty("LateReverbLength",lrLength,nullptr);
    
    String numdReflections = numDiscreteReflections->getText();
    if(!numdReflections.isEmpty())
        reverbNode.setProperty("DiscreteReflectionsNumber",numdReflections,nullptr);
    
    String crsFilterTime = lateReverbCrossfadeTime->getText();
    if(!crsFilterTime.isEmpty())
        reverbNode.setProperty("LateReverbCrossfadeTime",crsFilterTime,nullptr);
    
    //    std::cout<<" new valuetree "<<properties.toXmlString()<<std::endl;
    
    editor.toggleSettingsButton(false);
    editor.getProcessor().PluginReset();
}

void Settings::comboBoxChanged (ComboBox* cb)
{
    auto selectedID = cb->getSelectedId();
    if(cb->getItemText(selectedID-1).equalsIgnoreCase(findFileMsg)){
        SafePointer<Settings> safeThis (this);
        if(cb == lateReverbDecorrelationFilters){
            File f(properties.getOrCreateChildWithName("ReverbSettings", nullptr)
                   .getProperty("LateReverbDecorrelationFiltersFile").toString());
            String mainDir;
            f.exists()? mainDir = f.getParentDirectory().getFullPathName() : "";
            
            fc.reset (new FileChooser ("Choose a reverb configuration file to use...", mainDir,
                                       "*.wav;*.txt"));
            fc->launchAsync ( FileBrowserComponent::openMode
                             | FileBrowserComponent::canSelectFiles,
                             [safeThis,f] (const FileChooser& chooser)
                             {
                                 if (safeThis != nullptr && chooser.getURLResults().size() > 0)
                                 {
                                     String chosen;
                                     auto result = chooser.getURLResult();
                                     chosen << (result.isLocalFile() ? result.getLocalFile().getFullPathName()
                                                : result.toString (false));
                                     safeThis->lateReverbDecorrelationFilters->setText(chosen);
                                 }else{
                                     if(f.exists()){
                                         safeThis->lateReverbDecorrelationFilters->setText(f.getFullPathName());
                                     }else{
                                         safeThis->lateReverbDecorrelationFilters->setSelectedItemIndex(-1,dontSendNotification);
                                     }
                                 }
                             });
        }
    }
}

void Settings::buttonClicked (Button* buttonThatWasClicked)
{
    if (buttonThatWasClicked == save)
    {
        triggerAsyncUpdate();
    }
    if (buttonThatWasClicked == cancel)
    {
        editor.toggleSettingsButton(false);
    }
}
    
} // namespace binaural_renderer
} // namespace dawplugins
} // namespace visr_production_suite

