#include "PluginEditor.h"
#include <libcore/PluginBinaryData.h>

using namespace juce;

namespace visr_production_suite
{
namespace dawplugins
{
namespace binaural_renderer
{

Editor::Editor (Processor* opan)
: PluginEditorBase(opan, Colour(20,40,20)),
op(*opan)
{
    sceneHeadOrientationView = new core::graphics::SceneWithHeadOrientation(op);
    sceneHeadOrientationView->setName ("Binaural Renderer's sphere");
    op.attach(sceneHeadOrientationView);
    addAndMakeVisible (sceneHeadOrientationView);
    
    plugtitle = new ImageButton ("ImageButton");
    juce::Image title = ImageCache::getFromMemory (PluginBinaryData::s3aBinauralRenderer_png, PluginBinaryData::s3aBinauralRenderer_pngSize);
    plugtitle->setImages (true, true, true,
                          title, 1.0f, Colours::transparentBlack,
                          title, 1.0f, Colours::transparentBlack,
                          title, 1.0f, Colours::transparentBlack,
                          0.5f);
    addAndMakeVisible(plugtitle);
    
    rendererSelector = new ComboBox();
    rendererSelector->addListener (this);
    rendererSelector->setJustificationType (Justification::centred);
    rendererSelector->setScrollWheelEnabled(true);
    rendererSelector->setTextWhenNothingSelected("default VBAP renderer");
    rendererSelector->setSelectedItemIndex(200,NotificationType::dontSendNotification);
    populateRendererMenu();
    addAndMakeVisible (rendererSelector);
    
    /************* SETTINGS **************************************/
    
    sceneHeadOrientationView->addChildComponent( settings = new Settings(*this));
    settings->setAlwaysOnTop(true);
    
    addAndMakeVisible (settingsObj = new ImageButton ("new button"));
    
    Image settingsImage = ImageCache::getFromMemory (PluginBinaryData::settings_png, PluginBinaryData::settings_pngSize);
    settingsObj->setImages (false, true, true,
                            settingsImage, 1.000f, Colour (180,180,180),
                            settingsImage, 1.000f, Colour (180,180,180),
                            settingsImage, .5f, Colour (0x00000000));
    settingsObj->setButtonText (TRANS("settings"));
    settingsObj->setClickingTogglesState(true);
    settingsObj->addListener(this);
    
    /**********************************************************************/
    
    addChildComponent(calibration = new TextButton("calibrate"));
    calibration->addListener(this);
    
    const int dynamicHeadTrackingSelect = op.parameters.state.getOrCreateChildWithName( "TrackingSettings", nullptr ).getProperty( "trackingType", 0 );
    bool const useHeadTracking = (dynamicHeadTrackingSelect == 2);
    
    if(useHeadTracking) calibration->setVisible(true);
    else calibration->setVisible(false);
    
    PluginEditorBase::resized();
    resized();
}

Editor::~Editor()
{
    op.detach(sceneHeadOrientationView);
}

void Editor::populateRendererMenu()
{
    rendererSelector->clear(NotificationType::dontSendNotification);
    int i = 0;
    Processor::RendererType rt;
    for (; i < static_cast<int>(Processor::RendererType::REND_MAX); i++) {
        rt = static_cast<Processor::RendererType> (i);
        rendererSelector->addItem(op.rendNames[rt], i+1);
    }
    int currRend = op.parameters.state.getProperty("RendererType", 0);
    rendererSelector->setSelectedId(currRend+1,dontSendNotification);
}

void Editor::resized()
{
    //side + audioSceneView + padding, title + audioSceneView + padding
    setSize(170+440+10, 50+440+10);
    plugtitle->setBoundsToFit(juce::Rectangle<int>(80, 12,350,25), Justification::centredLeft,false);
    
    auto bounds = getLocalBounds().removeFromBottom(getHeight()-50);
    auto side = bounds.removeFromLeft(170);
    auto row = side.removeFromTop(35).reduced(6);;
    sceneHeadOrientationView->setBounds(bounds.reduced(5));
    rendererSelector->setBounds (row.withTrimmedRight(40));
    settingsObj->setBounds (row.removeFromRight(35));
    
    calibration->setBounds (side.removeFromTop(35).reduced(5).withTrimmedRight(40));
    settings->setBoundsInset(BorderSize<int> (1));
    
}
void Editor::comboBoxChanged (ComboBox* cb)
{
    auto selectedID = cb->getSelectedId();
    if (cb== rendererSelector){
        int currRend = op.parameters.state.getProperty("RendererType", 0);
        if(selectedID<(static_cast<int>(Processor::RendererType::REND_MAX)+1) && selectedID!=currRend+1){
            op.parameters.state.setProperty("RendererType", selectedID-1, nullptr);
            triggerAsyncUpdate();
        }
    }
}

void Editor::handleAsyncUpdate(){
    op.PluginReset();
}

void Editor::toggleSettingsButton(bool isEnabled){
    settingsObj->setToggleState(isEnabled, sendNotification);
}

void Editor::buttonClicked (Button* buttonThatWasClicked)
{
    if (buttonThatWasClicked == settingsObj)
    {
        if(settingsObj->getToggleState()){
            settings->refresh();
            settings->setVisible(true);
            
        }
        else settings->setVisible(false);
    }else if(buttonThatWasClicked == calibration){
        op.recalibrate();
        //      , "{'port': '/dev/cu.usbserial-AJ03GR8O', 'calibrationPort': 8889, 'yawRightHand' : True }"
        //      , "{'port': '/dev/cu.usbserial-AJ03GSC8', 'calibrationPort': 8889, 'yawRightHand' : True }"
//        std::string const headTrackingReceiverParameters = op.parameters.state.getOrCreateChildWithName( "TrackingSettings", nullptr ).getProperty( "InternalTrackerOptions").toString().toStdString();
//        
//        if(!headTrackingReceiverParameters.empty()){
//            std::string delimiter = "'calibrationPort': ";
//            std::string port_ = headTrackingReceiverParameters.substr(headTrackingReceiverParameters.find(delimiter)+delimiter.length(),4);
//            int port = std::stoi(port_);
//            
//            socketSend = nullptr;
//            socketSend = new DatagramSocket (true);
//            socketSend->bindToPort (0);
//            std::ostringstream jsonStream;
//            std::string s = jsonStream.str();
//            outStream.write( s.c_str(), s.length() );
//            
//            if (socketSend != nullptr)
//            {
//                const int streamSize = (int) outStream.getDataSize();
//                /*const int bytesWritten = */ socketSend->write ("127.0.0.1", port,
//                                                                 outStream.getData(), streamSize);
//                // TODO: Provide other error reporting mechanism.
//                // return bytesWritten == streamSize;
//            }
//            outStream.reset();
//        }
    }
}

} // namespace binaural_renderer
} // namespace dawplugins
} // namespace visr_production_suite
