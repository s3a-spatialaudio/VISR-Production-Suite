

#include "Settings.h"
#include "PluginEditor.h"

using namespace juce;

namespace visr_production_suite
{
namespace dawplugins
{
namespace binaural_renderer
{
            
Settings::Settings (Editor& edit)
:
properties(edit.getProcessor().parameters.state),
editor(edit),
general_l(String(), "General"),
tracking_l(String(), "Tracking"),
hrir_l(String(), "HRIR"),
hoa_l(String(), "HOA"),
brir_l(String(), "BRIR"),
trackOff_l (String(), "Off:"),
trackOSC_l (String(), "OSC:"),
trackInternal_l(String(), "Internal:"),
trackInternalDevice_l(String(), "Internal tracker:"),
trackInternalParams_l(String(), "Internal tracker options:"),
maxNumObj_l (String(), "Max # objects:"),
hrirSOFA_l(String(), "SOFA file path:"),
hrirDynITD_l(String(), "dynamic ITD:"),
hrirInterp_l(String(), "interpolation:"),
hrirCrossf_l(String(), "crossfade:"),
hoaSOFA_l(String(), "SOFA file path:"),
brirSOFA_l(String(), "SOFA file path:"),
brirLdspk_l(String(), "Ldspk Config file path:"),
brirDynITD_l(String(), "dynamic ITD:"),
brirInterp_l(String(), "interpolation:"),
brirCrossf_l(String(), "crossfade:"),
brirObjRendererOpt_l(String(), "object renderer options:"),
brirTruncationLength_l(String(), "brir truncation length:")
{
//    std::cout<<" old valuetree "<<properties.toXmlString()<<std::endl;
    
    addAndMakeVisible(trackOff_l);
    addAndMakeVisible(trackOSC_l);
    addAndMakeVisible(trackInternal_l);
    addAndMakeVisible(trackInternalDevice_l);
    addAndMakeVisible(trackInternalParams_l);
    
    addAndMakeVisible(maxNumObj_l);
    
    addAndMakeVisible(hrirSOFA_l);
    addAndMakeVisible(hrirDynITD_l);
    addAndMakeVisible(hrirInterp_l);
    addAndMakeVisible(hrirCrossf_l);
    
    addAndMakeVisible(hoaSOFA_l);
    
    addAndMakeVisible(brirSOFA_l);
    addAndMakeVisible(brirLdspk_l);
    addAndMakeVisible(brirDynITD_l);
    addAndMakeVisible(brirInterp_l);
    addAndMakeVisible(brirCrossf_l);
    addAndMakeVisible(brirObjRendererOpt_l);
    addAndMakeVisible(brirTruncationLength_l);
    
    general_l.setJustificationType(Justification::centred);
    general_l.setColour(Label::textColourId, Colours::black);
    tracking_l.setJustificationType(Justification::centred);
    tracking_l.setColour(Label::textColourId, Colours::black);
    hrir_l.setJustificationType(Justification::centred);
    hrir_l.setColour(Label::textColourId, Colours::black);
    hoa_l.setJustificationType(Justification::centred);
    hoa_l.setColour(Label::textColourId, Colours::black);
    brir_l.setJustificationType(Justification::centred);
    brir_l.setColour(Label::textColourId, Colours::black);
    
    addAndMakeVisible(general_l);
    addAndMakeVisible(tracking_l);
    addAndMakeVisible(hrir_l);
    addAndMakeVisible(hoa_l);
    addAndMakeVisible(brir_l);
    
    addAndMakeVisible (trackOff = new juce::ToggleButton());
    trackOff->setRadioGroupId(1234);
    
    addAndMakeVisible (trackOSC = new juce::ToggleButton());
    trackOSC->setRadioGroupId(1234);
    
    addAndMakeVisible (trackInternal = new juce::ToggleButton());
    trackInternal->setRadioGroupId(1234);
    
    
    addAndMakeVisible (trackInternalDevice = new TextEditor ("new text editor"));
    trackInternalDevice->setMultiLine (false);
    trackInternalDevice->setReturnKeyStartsNewLine (false);
    trackInternalDevice->setReadOnly (false);
    trackInternalDevice->setScrollbarsShown (false);
    trackInternalDevice->setCaretVisible (true);
    trackInternalDevice->setPopupMenuEnabled (true);
//    trackInternalDevice->setInputRestrictions(3,"1234567890");
    trackInternalDevice->setText (properties.getOrCreateChildWithName("TrackingSettings", nullptr)
                        .getProperty("InternalTracker",""), juce::dontSendNotification);
    
    addAndMakeVisible (trackInternalParams = new TextEditor ("new text editor"));
    trackInternalParams->setMultiLine (false);
    trackInternalParams->setReturnKeyStartsNewLine (false);
    trackInternalParams->setReadOnly (false);
    trackInternalParams->setScrollbarsShown (false);
    trackInternalParams->setCaretVisible (true);
    trackInternalParams->setPopupMenuEnabled (true);
    //    trackInternalDevice->setInputRestrictions(3,"1234567890");
    trackInternalParams->setText (properties.getOrCreateChildWithName("TrackingSettings", nullptr)
                                  .getProperty("InternalTrackerOptions",""), juce::dontSendNotification);
    
    
    int radioSelect = (int) properties.getOrCreateChildWithName("TrackingSettings", nullptr).getProperty("trackingType", 0);
    if(radioSelect==0) trackOff->setToggleState(true, juce::dontSendNotification);
    else if(radioSelect==1) trackOSC->setToggleState(true, juce::dontSendNotification);
    else if(radioSelect==2) trackInternal->setToggleState(true, juce::dontSendNotification);
    
    addAndMakeVisible (hrirSOFA = new ComboBox());
    hrirSOFA->addListener (this);
    hrirSOFA->setEditableText(true);
    hrirSOFA->setJustificationType (Justification::centred);
    hrirSOFA->setScrollWheelEnabled(true);
    hrirSOFA->setTextWhenNothingSelected("select your HRIR SOFA dataset");
    
//    printf("%s\n", defaultHRIRbuildPath.toStdString().c_str());
//    printf("%s\n", defaultHRIRinstallPath.toStdString().c_str());
//    hrirSOFA->setSelectedItemIndex(0,NotificationType::dontSendNotification);
    
    addAndMakeVisible (hrirDynITD = new juce::ToggleButton());
    hrirDynITD->setToggleState((int) properties.getOrCreateChildWithName("HRIRSettings", nullptr)
                               .getProperty("HRIR_dynamicITD",false), juce::dontSendNotification);
    
    addAndMakeVisible (hrirInterp = new juce::ToggleButton());
    hrirInterp->setToggleState((int) properties.getOrCreateChildWithName("HRIRSettings", nullptr)
                               .getProperty("HRIR_interpolation",true), juce::dontSendNotification);
    
    addAndMakeVisible (hrirCrossf = new juce::ToggleButton());
    hrirCrossf->setToggleState((int) properties.getOrCreateChildWithName("HRIRSettings", nullptr)
                               .getProperty("HRIR_crossfade",false), juce::dontSendNotification);
    
    addAndMakeVisible (hoaSOFA = new ComboBox());
    hoaSOFA->addListener (this);
    hoaSOFA->setEditableText(true);
    hoaSOFA->setJustificationType (Justification::centred);
    hoaSOFA->setScrollWheelEnabled(true);
    hoaSOFA->setTextWhenNothingSelected("select your HOA to binaural filter");
    //    hoaSOFA->setSelectedItemIndex(0,NotificationType::dontSendNotification);
    
    
    addAndMakeVisible (brirSOFA = new ComboBox());
    brirSOFA->addListener (this);
    brirSOFA->setEditableText(true);
    brirSOFA->setJustificationType (Justification::centred);
    brirSOFA->setScrollWheelEnabled(true);
    brirSOFA->setTextWhenNothingSelected("select your BRIR SOFA dataset");
//    brirSOFA->setSelectedItemIndex(0,NotificationType::dontSendNotification);
    
    
    
    addAndMakeVisible (brirLdspk = new ComboBox());
    brirLdspk->addListener (this);
    brirLdspk->setEditableText(true);
    brirLdspk->setJustificationType (Justification::centred);
    brirLdspk->setScrollWheelEnabled(true);
    brirLdspk->setTextWhenNothingSelected("select the loudspeaker configuration of BRIR SOFA dataset");
//    brirLdspk->setSelectedItemIndex(0,NotificationType::dontSendNotification);
    
    
    
    addAndMakeVisible (brirDynITD = new juce::ToggleButton());
    brirDynITD->setToggleState((int)properties.getOrCreateChildWithName("BRIRSettings", nullptr)
                               .getProperty("BRIR_dynamicITD",false), juce::dontSendNotification);
    
    addAndMakeVisible (brirInterp = new juce::ToggleButton());
    brirInterp->setToggleState((int) properties.getOrCreateChildWithName("BRIRSettings", nullptr)
                               .getProperty("BRIR_interpolation",false), juce::dontSendNotification);
    
    addAndMakeVisible (brirCrossf = new juce::ToggleButton());
    brirCrossf->setToggleState((int) properties.getOrCreateChildWithName("BRIRSettings", nullptr)
                               .getProperty("BRIR_crossfade",false), juce::dontSendNotification);
    
    addAndMakeVisible (brirObjRendererOpt = new TextEditor ("new text editor"));
    brirObjRendererOpt->setMultiLine (false);
    brirObjRendererOpt->setReturnKeyStartsNewLine (false);
    brirObjRendererOpt->setReadOnly (false);
    brirObjRendererOpt->setScrollbarsShown (false);
    brirObjRendererOpt->setCaretVisible (true);
    brirObjRendererOpt->setPopupMenuEnabled (true);
    //    trackInternalDevice->setInputRestrictions(3,"1234567890");
    brirObjRendererOpt->setText (properties.getOrCreateChildWithName("BRIRSettings", nullptr)
                                 .getProperty("ObjectRendererOptions",""), juce::dontSendNotification);
    

    
    addAndMakeVisible(save = new juce::TextButton("Ok"));
    save->addListener(this);
    addAndMakeVisible(cancel = new juce::TextButton("Cancel"));
    cancel->addListener(this);
    
    
        addAndMakeVisible (maxNumObj = new TextEditor ("new text editor"));
        maxNumObj->setMultiLine (false);
        maxNumObj->setReturnKeyStartsNewLine (false);
        maxNumObj->setReadOnly (false);
        maxNumObj->setScrollbarsShown (false);
        maxNumObj->setCaretVisible (true);
        maxNumObj->setPopupMenuEnabled (true);
        maxNumObj->setInputRestrictions(3,"1234567890");
        maxNumObj->setText (properties.getOrCreateChildWithName("GeneralSettings", nullptr)
                            .getProperty("MaximumObjectNumber",10), juce::dontSendNotification);
    
    
    addAndMakeVisible (brirTruncationLength = new TextEditor ("new text editor"));
    brirTruncationLength->setMultiLine (false);
    brirTruncationLength->setReturnKeyStartsNewLine (false);
    brirTruncationLength->setReadOnly (false);
    brirTruncationLength->setScrollbarsShown (false);
    brirTruncationLength->setCaretVisible (true);
    brirTruncationLength->setPopupMenuEnabled (true);
    brirTruncationLength->setInputRestrictions(6,"1234567890");
    brirTruncationLength->setText (properties.getOrCreateChildWithName("BRIRSettings", nullptr)
                        .getProperty("irTruncationLength",8192), juce::dontSendNotification);

    
    
    setSize (400, 550);
}

Settings::~Settings()
{
}


void Settings::refresh(){

    int radioSelect = (int) properties.getOrCreateChildWithName("TrackingSettings", nullptr).getProperty("trackingType", 0);
    if(radioSelect==0) trackOff->setToggleState(true, juce::dontSendNotification);
    else if(radioSelect==1) trackOSC->setToggleState(true, juce::dontSendNotification);
    else if(radioSelect==2) trackInternal->setToggleState(true, juce::dontSendNotification);
    
    trackInternalDevice->setText (properties.getOrCreateChildWithName("TrackingSettings", nullptr)
                                  .getProperty("InternalTracker",""), juce::dontSendNotification);
    trackInternalParams->setText (properties.getOrCreateChildWithName("TrackingSettings", nullptr)
                                  .getProperty("InternalTrackerOptions",""), juce::dontSendNotification);
    hrirSOFA->clear(NotificationType::dontSendNotification);
    
    String path = properties.getOrCreateChildWithName("HRIRSettings", nullptr).getProperty("HRIRsofaFile","");
//    if(!path.isEmpty()){
        hrirSOFA->setText(path);
//    }
    hrirSOFA->addItem(findFileMsg,1);
    
    hrirDynITD->setToggleState((int) properties.getOrCreateChildWithName("HRIRSettings", nullptr)
                               .getProperty("HRIR_dynamicITD",false), juce::dontSendNotification);
    
    hrirInterp->setToggleState((int) properties.getOrCreateChildWithName("HRIRSettings", nullptr)
                               .getProperty("HRIR_interpolation",true), juce::dontSendNotification);
    
    hrirCrossf->setToggleState((int) properties.getOrCreateChildWithName("HRIRSettings", nullptr)
                               .getProperty("HRIR_crossfade",false), juce::dontSendNotification);
    
    hoaSOFA->clear(NotificationType::dontSendNotification);

    path = properties.getOrCreateChildWithName("HOASettings", nullptr).getProperty("HOAsofaFile","");
//    if(!path.isEmpty()){
        hoaSOFA->setText(path);
//    }
    hoaSOFA->addItem(findFileMsg,1);
    
    brirSOFA->clear(NotificationType::dontSendNotification);
    
    path = properties.getOrCreateChildWithName("BRIRSettings", nullptr).getProperty("BRIRsofaFile","");
//    if(!path.isEmpty()){
        brirSOFA->setText(path);
//    }
    brirSOFA->addItem(findFileMsg,1);
    
    brirLdspk->clear(NotificationType::dontSendNotification);

    path = properties.getOrCreateChildWithName("BRIRSettings", nullptr).getProperty("BRIRLoudspeakerLayoutFile","");
//    if(!path.isEmpty()){
        brirLdspk->setText(path);
//    }
    brirLdspk->addItem(findFileMsg,1);
    
    brirDynITD->setToggleState((int)properties.getOrCreateChildWithName("BRIRSettings", nullptr)
                               .getProperty("BRIR_dynamicITD",false), juce::dontSendNotification);
    
    brirInterp->setToggleState((int) properties.getOrCreateChildWithName("BRIRSettings", nullptr)
                               .getProperty("BRIR_interpolation",false), juce::dontSendNotification);
    
    brirCrossf->setToggleState((int) properties.getOrCreateChildWithName("BRIRSettings", nullptr)
                               .getProperty("BRIR_crossfade",false), juce::dontSendNotification);
    
    brirObjRendererOpt->setText (properties.getOrCreateChildWithName("BRIRSettings", nullptr)
                                 .getProperty("ObjectRendererOptions",""), juce::dontSendNotification);
    
    maxNumObj->setText (properties.getOrCreateChildWithName("GeneralSettings", nullptr)
                        .getProperty("MaximumObjectNumber",10), juce::dontSendNotification);
    //

    brirTruncationLength->setText (properties.getOrCreateChildWithName("BRIRSettings", nullptr)
                                   .getProperty("irTruncationLength",8192), juce::dontSendNotification);

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
    
//    g.drawLine(bounds.getX(), 10, bounds.getWidth(),10 ); //zeroblock
    g.drawLine(bounds.getX(), 35, bounds.getWidth(), 35 ); //firstblock
    g.drawLine(bounds.getX(), 125, bounds.getWidth(),125); //secondblock
    g.drawLine(bounds.getX(), 195, bounds.getWidth(),195); //thirdblock
    g.drawLine(bounds.getX(), 255, bounds.getWidth(),255);//fourthblock
    g.drawLine(bounds.getX(), 395, bounds.getWidth(),395 ); //savearea
    
//    g.drawLine(bounds.getX(), 30, bounds.getWidth(),30 );
    g.drawLine(bounds.getX(), 55, bounds.getWidth(),55 );
    g.drawLine(bounds.getX(), 145, bounds.getWidth(), 145);
    g.drawLine(bounds.getX(), 215, bounds.getWidth(), 215);
    g.drawLine(bounds.getX(), 275, bounds.getWidth(), 275);
    g.drawLine(bounds.getX(), 435, bounds.getWidth(), 435);
    
}

void Settings::resized()
{
    auto bounds = getLocalBounds().reduced(10,0);
    
    auto generalBlock = bounds.removeFromTop(30).withTrimmedTop(5);

    auto row = generalBlock.removeFromTop(20);
//    general_l.setBounds(row.reduced(5));
//    
////    zeroBlock.removeFromTop(7);
//    row = generalBlock.removeFromTop(30);
    maxNumObj_l.setBounds(row.removeFromLeft(150));
    maxNumObj->setBounds (row.removeFromLeft(30));
    
    auto trackingBlock = bounds.removeFromTop (90);
    row = trackingBlock.removeFromTop(30);
    tracking_l.setBounds(row.reduced(5));
    
    row = trackingBlock.removeFromTop(20);
    trackOff_l.setBounds(row.removeFromLeft(70));
    trackOff->setBounds (row.removeFromLeft(30));
    
    
    row = trackingBlock.removeFromTop(20);
    trackOSC_l.setBounds(row.removeFromLeft(70));
    trackOSC->setBounds (row.removeFromLeft(30));
    
    trackInternalDevice->setBounds (row.removeFromRight(170));
    trackInternalDevice_l.setBounds(row.removeFromRight(150));
    
    row = trackingBlock.removeFromTop(20);
    trackInternal_l.setBounds(row.removeFromLeft(70));
    trackInternal->setBounds (row.removeFromLeft(30));
    
    trackInternalParams->setBounds (row.removeFromRight(170));
    trackInternalParams_l.setBounds(row.removeFromRight(150));

    
    auto hrirBlock = bounds.removeFromTop (70);
    row = hrirBlock.removeFromTop(30);
    hrir_l.setBounds(row.reduced(5));
    
    
    row = hrirBlock.removeFromTop(20);
    hrirSOFA_l.setBounds(row.removeFromLeft(150));
    hrirSOFA->setBounds (row.removeFromLeft(300));
    
    row = hrirBlock.removeFromTop(20);
    hrirDynITD_l.setBounds(row.removeFromLeft(100));
    hrirDynITD->setBounds (row.removeFromLeft(30));
    
    hrirInterp_l.setBounds(row.removeFromLeft(100));
    hrirInterp->setBounds (row.removeFromLeft(30));
    
    hrirCrossf_l.setBounds(row.removeFromLeft(100));
    hrirCrossf->setBounds (row.removeFromLeft(30));
    
    auto hoaBlock = bounds.removeFromTop (60);
    row = hoaBlock.removeFromTop(30);
    hoa_l.setBounds(row.reduced(5));
    
    row = hoaBlock.removeFromTop(20);
    hoaSOFA_l.setBounds(row.removeFromLeft(150));
    hoaSOFA->setBounds (row.removeFromLeft(300));

    auto brirBlock = bounds.removeFromTop (130);
    row = brirBlock.removeFromTop(30);
    brir_l.setBounds(row.reduced(5));

    row = brirBlock.removeFromTop(20);
    brirSOFA_l.setBounds(row.removeFromLeft(150));
    brirSOFA->setBounds (row.removeFromLeft(300));
   
    row = brirBlock.removeFromTop(20);
    brirLdspk_l.setBounds(row.removeFromLeft(150));
    brirLdspk->setBounds (row.removeFromLeft(300));
    
    row = brirBlock.removeFromTop(20);
    brirObjRendererOpt_l.setBounds(row.removeFromLeft(150));
    brirObjRendererOpt->setBounds (row.removeFromLeft(300));
    
    row = brirBlock.removeFromTop(20);
    brirTruncationLength_l.setBounds(row.removeFromLeft(150));
    brirTruncationLength->setBounds (row.removeFromLeft(70));
    
    row = brirBlock.removeFromTop(20);
    brirDynITD_l.setBounds(row.removeFromLeft(100));
    brirDynITD->setBounds (row.removeFromLeft(30));
    
    brirInterp_l.setBounds(row.removeFromLeft(100));
    brirInterp->setBounds (row.removeFromLeft(30));
    
    brirCrossf_l.setBounds(row.removeFromLeft(100));
    brirCrossf->setBounds (row.removeFromLeft(30));
    
    
    auto saveArea = bounds.removeFromBottom(45).reduced(bounds.getWidth()/4,0);
    
    cancel->setBounds(saveArea.removeFromLeft(100).reduced(10));
    save->setBounds(saveArea.removeFromLeft(100).reduced(10));
    
//
//    
//    cancel->setBounds(saveArea.withTrimmedLeft(saveArea.getWidth()/2-100).removeFromTop(40).removeFromLeft(100));
//    save->setBounds(saveArea.withTrimmedLeft(saveArea.getWidth()/2).removeFromLeft(100));
    
}

void Settings::handleAsyncUpdate(){
    //    String pnm = ip->getText();
    //    String ptx = port->getText();
    //    if(!pnm.isEmpty()) properties.setProperty(contentID+"_ip", ip->getText(),nullptr);
    //    if(!ptx.isEmpty()) properties.setProperty(contentID+"_port", port->getText(),nullptr);
    //    std::cout<<" old valuetree "<<properties.toXmlString()<<std::endl;
    ValueTree trackingNode = properties.getOrCreateChildWithName("TrackingSettings", nullptr);
    trackingNode.setProperty("trackingType", trackOff->getToggleState()==true ? 0 : (trackOSC->getToggleState()==true ? 1 : 2 ), nullptr);
    
    if(trackOff->getToggleState() || trackOSC->getToggleState()) editor.calibration->setVisible(false);
    else  editor.calibration->setVisible(true);
    
    String device = trackInternalDevice->getText();
    if(!device.isEmpty())
        trackingNode.setProperty("InternalTracker",device,nullptr);
    
    String deviceParam = trackInternalParams->getText();
    if(!deviceParam.isEmpty())
        trackingNode.setProperty("InternalTrackerOptions",deviceParam,nullptr);
    
    ValueTree generalNode = properties.getOrCreateChildWithName("GeneralSettings", nullptr);
    String numobj = maxNumObj->getText();
    if(!numobj.isEmpty())
        generalNode.setProperty("MaximumObjectNumber",numobj,nullptr);
    
    ValueTree hrirNode = properties.getOrCreateChildWithName("HRIRSettings", nullptr);
    String hrirPath = hrirSOFA->getText();
//    if(!hrirPath.isEmpty() && hrirPath!=hrirSOFA->getTextWhenNothingSelected())
        hrirNode.setProperty("HRIRsofaFile",hrirPath,nullptr);
    
    hrirNode.setProperty("HRIR_dynamicITD",hrirDynITD->getToggleState(),nullptr);
    hrirNode.setProperty("HRIR_interpolation",hrirInterp->getToggleState(),nullptr);
    hrirNode.setProperty("HRIR_crossfade",hrirCrossf->getToggleState(),nullptr);
    
    ValueTree hoaNode = properties.getOrCreateChildWithName("HOASettings", nullptr);
    String hoaPath = hoaSOFA->getText();
//    if(!hoaPath.isEmpty() && hoaPath!=hoaSOFA->getTextWhenNothingSelected())
        hoaNode.setProperty("HOAsofaFile",hoaPath,nullptr);
    
    ValueTree brirNode = properties.getOrCreateChildWithName("BRIRSettings", nullptr);
    String brirPath = brirSOFA->getText();
//    if(!brirPath.isEmpty() && brirPath!=brirSOFA->getTextWhenNothingSelected())
        brirNode.setProperty("BRIRsofaFile",brirPath,nullptr);
    
    String brirLdspPath = brirLdspk->getText();
//    if(!brirLdspPath.isEmpty() && brirLdspPath!=brirLdspk->getTextWhenNothingSelected())
        brirNode.setProperty("BRIRLoudspeakerLayoutFile",brirLdspPath,nullptr);
    
    String trunc = brirTruncationLength->getText();
    if(!trunc.isEmpty())
        brirNode.setProperty("irTruncationLength",trunc,nullptr);
    

    brirNode.setProperty("BRIR_dynamicITD",brirDynITD->getToggleState(),nullptr);
    brirNode.setProperty("BRIR_interpolation",brirInterp->getToggleState(),nullptr);
    brirNode.setProperty("BRIR_crossfade",brirCrossf->getToggleState(),nullptr);
    
    String objectRendOpt = brirObjRendererOpt->getText();
    if(!objectRendOpt.isEmpty())
        brirNode.setProperty("ObjectRendererOptions",objectRendOpt,nullptr);
    
    
//    std::cout<<" new valuetree "<<properties.toXmlString()<<std::endl;
    //    int newTargetPort = processor.parameters.state.getProperty(contentID+"_port");
    //    std::cout<<"NET PROP SET: "<<contentID+"_ip"<<" = "<<processor.parameters.state.getProperty(contentID+"_ip").toString().toRawUTF8()<<", "<<contentID+"_port"<<" = "<<
    //    std::to_string(newTargetPort)<<std::endl;
    
    
    editor.toggleSettingsButton(false);
    editor.getProcessor().PluginReset();
}

void Settings::comboBoxChanged (ComboBox* cb)
{
    auto selectedID = cb->getSelectedId();
    if(cb->getItemText(selectedID-1).equalsIgnoreCase(findFileMsg)){
        
        
        SafePointer<Settings> safeThis (this);
        
        
        if(cb == hrirSOFA){
            File f(properties.getOrCreateChildWithName("HRIRSettings", nullptr)
                   .getProperty("HRIRsofaFile","").toString());
            String mainDir;
            f.exists()? mainDir = f.getParentDirectory().getFullPathName() : "";
            fc.reset (new FileChooser ("Choose a SOFA file to use...", mainDir,
                                       "*.sofa"));
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
                                     
                                     safeThis->hrirSOFA->setText(chosen);
                                 }else{
                                     if(f.exists()){
                                         safeThis->hrirSOFA->setText(f.getFullPathName());
                                     }else{
                                         safeThis->hrirSOFA->setSelectedItemIndex(-1,dontSendNotification);
                                     }
                                 }
                             });
        }else if(cb == hoaSOFA){
            File f(properties.getOrCreateChildWithName("HOASettings", nullptr)
                   .getProperty("HOAsofaFile","").toString());
            String mainDir;
            f.exists()? mainDir = f.getParentDirectory().getFullPathName() : "";
            fc.reset (new FileChooser ("Choose a SOFA file to use...", mainDir,
                                       "*.sofa"));
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
                                     
                                     safeThis->hoaSOFA->setText(chosen);
                                 }else{
                                     if(f.exists()){
                                         safeThis->hoaSOFA->setText(f.getFullPathName());
                                     }else{
                                         safeThis->hoaSOFA->setSelectedItemIndex(-1,dontSendNotification);
                                     }
                                 }
                             });
        }else if(cb == brirSOFA){
            File f(properties.getOrCreateChildWithName("BRIRSettings", nullptr)
                   .getProperty("BRIRsofaFile","").toString());
            
            String mainDir;
            f.exists()? mainDir = f.getParentDirectory().getFullPathName() : "";
            fc.reset (new FileChooser ("Choose a SOFA file to use...", mainDir,
                                       "*.sofa"));
            
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
                                     
                                     safeThis->brirSOFA->setText(chosen);
                                 }else{
                                     if(f.exists()){
                                         safeThis->brirSOFA->setText(f.getFullPathName());
                                     }else{
                                         safeThis->brirSOFA->setSelectedItemIndex(-1,dontSendNotification);
                                     }
                                 }
                             });
        }else if(cb == brirLdspk){
            File f(properties.getOrCreateChildWithName("BRIRSettings", nullptr)
                         .getProperty("BRIRLoudspeakerLayoutFile","").toString());
            
            String mainDir;
            f.exists()? mainDir = f.getParentDirectory().getFullPathName() : "";
            fc.reset (new FileChooser ("Choose a BRIR loudspeaker layout file to use...", mainDir,
                                       "*.xml"));

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
                                     
                                     safeThis->brirLdspk->setText(chosen);
                                 }else{
                                     if(f.exists()){
                                         safeThis->brirLdspk->setText(f.getFullPathName());
                                     }else{
                                         safeThis->brirLdspk->setSelectedItemIndex(-1,dontSendNotification);
                                     }
                                 }
                             });
        }

        
        
    }
    //        if (layoutFolder.browseForFileToOpen())
    //        {
    //            File nextLayout (layoutFolder.getResult());
    //                triggerAsyncUpdate();
    //
    //        }else{
    //            cb->setSelectedItemIndex(currentLayoutID, NotificationType::dontSendNotification);
    //            updatePopupDirectories();
    //        }
}






void Settings::buttonClicked (Button* buttonThatWasClicked)
{
    //    std::cout<<"CLICKED "<<buttonThatWasClicked->getName()<<std::endl;
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

