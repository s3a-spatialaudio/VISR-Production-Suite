#include "PluginEditor.h"
#include <libcore/PluginBinaryData.h>
#include <libcore/graphics/NetSettings.h>

using namespace juce;
namespace visr_production_suite
{
namespace dawplugins
{
namespace scene_master
{

Editor::Editor (Processor* opan)
: PluginEditorBase(opan,Colour(20,30,50)),
op(*opan)
{
    audioSceneView = new core::graphics::Scene(op);
    audioSceneView->setName ("Scene Master's sphere");
    op.attach(audioSceneView);
    addAndMakeVisible (audioSceneView);
    
    plugtitle = new ImageButton ("ImageButton");
    Image title = ImageCache::getFromMemory (PluginBinaryData::s3aSceneMaster_png, PluginBinaryData::s3aSceneMaster_pngSize);
    plugtitle->setImages (true, true, true,
                          title, 1.0f, Colours::transparentBlack,
                          title, 1.0f, Colours::transparentBlack,
                          title, 1.0f, Colours::transparentBlack,
                          0.5f);
    addAndMakeVisible(plugtitle);
    
    objTable = new SceneTableModel(op, op.getXmlModel());
    op.attach(objTable);
    addAndMakeVisible(objTable);
    
    addAndMakeVisible(lo = new Label ("Audio Scene"));
    lo->setText("Audio Scene",NotificationType::dontSendNotification);
    lo->setFont (Font ("Verdana",16.00f, Font::bold));
    lo->setColour(Label::textColourId, Colours::white.withAlpha(0.7f));
    
    addAndMakeVisible (settingsObj = new ImageButton ("new button"));
    settingsObj->setButtonText ("settings");
    settingsObj->addListener (this);
    Image settingsImage = ImageCache::getFromMemory (PluginBinaryData::settings_png, PluginBinaryData::settings_pngSize);
    settingsObj->setImages (false, true, true,
                            settingsImage, 1.000f, Colour (180,180,180),
                            settingsImage, .5f, Colour (0x00000000),
                            settingsImage, 1.000f, Colour (180,180,180));
//    setup();
    PluginEditorBase::resized();
    resized();
}

Editor::~Editor()
{
    op.detach(audioSceneView);
    op.detach(objTable);
}

void Editor::resized()
{
    
    lo->setBounds(10,65, 200, 30);
    settingsObj->setBounds (360, 65, 26, 25);
    objTable->setBounds(5,90,390,365);
    plugtitle->setBoundsToFit(juce::Rectangle<int>(105, 12,500,25), Justification::centredLeft,false);
    audioSceneView->setBounds(400, 55,400,400);
    //table + audioSceneView + padding, title + audioSceneView + padding
    setSize(400+400+10,470);
}


void Editor::paint (Graphics& g)
{
    PluginEditorBase::paint(g);
    // Object table Background
    g.setGradientFill (ColourGradient (Colour(30,30,30).brighter().withAlpha(0.5f),
                                       objTable->getWidth()/2+2,objTable->getHeight()/2+objTable->getY()-35,
                                       Colour(30,30,30).darker().withAlpha(0.5f),
                                       objTable->getWidth(),objTable->getHeight()+35, true));
    
    g.fillRoundedRectangle(5, objTable->getY()-35, objTable->getWidth(),objTable->getHeight()+35,3);
}


void Editor::buttonClicked (Button* buttonThatWasClicked)
{
    if (buttonThatWasClicked == settingsObj)
    {
        if (!settingsObjDialog)
        {
            juce::DialogWindow::LaunchOptions launchOptions;
            launchOptions.dialogTitle = juce::String("Settings");
            launchOptions.content.setOwned(new core::graphics::NetSettings(op));
            launchOptions.componentToCentreAround = this;
            launchOptions.escapeKeyTriggersCloseButton = true;
            launchOptions.useNativeTitleBar = true;
            launchOptions.resizable = false;
            launchOptions.useBottomRightCornerResizer = false;
            settingsObjDialog = launchOptions.launchAsync();
        }
    }
}

} // namespace scene_master
} // namespace dawplugins
} // namespace visr_production_suite
