#include "PluginEditor.h"
#include <libcore/PluginBinaryData.h>

using namespace juce;

namespace visr_production_suite
{
namespace dawplugins
{
namespace loudspeaker_renderer
{

Editor::Editor (Processor* opan)
: PluginEditorLoudspeakers(opan, Colour(20,40,20)),
op(*opan)
{
    audioSceneLdspkView = new core::graphics::SceneWithLoudspeakers(op);
    audioSceneLdspkView->setName ("Loudspeaker Renderer's sphere");
    op.attach(audioSceneLdspkView);
    addAndMakeVisible (audioSceneLdspkView);
    
    plugtitle = new ImageButton ("ImageButton");
    juce::Image title = ImageCache::getFromMemory (PluginBinaryData::s3aLoudspeakerRenderer_png, PluginBinaryData::s3aLoudspeakerRenderer_pngSize);
    plugtitle->setImages (true, true, true,
                          title, 1.0f, Colours::transparentBlack,
                          title, 1.0f, Colours::transparentBlack,
                          title, 1.0f, Colours::transparentBlack,
                          0.5f);
    addAndMakeVisible(plugtitle);
    
    /************* SETTINGS **************************************/
    
    audioSceneLdspkView->addChildComponent( settings = new Settings(*this));
    settings->setAlwaysOnTop(true);
    
    audioSceneLdspkView->addAndMakeVisible (settingsBtn = new ImageButton ("new button"));
    
    Image settingsImage = ImageCache::getFromMemory (PluginBinaryData::settings_png, PluginBinaryData::settings_pngSize);
    settingsBtn->setImages (false, true, true,
                            settingsImage, 1.000f, Colour (180,180,180),
                            settingsImage, 1.000f, Colour (180,180,180),
                            settingsImage, .5f, Colour (0x00000000));
    settingsBtn->setButtonText ("settings");
    settingsBtn->setClickingTogglesState(true);
    settingsBtn->setAlwaysOnTop(true);
    settingsBtn->addListener(this);
    
    /**********************************************************************/
    
    setup(audioSceneLdspkView);
    resized();
    resetLayout();
}

Editor::~Editor()
{
    op.detach(audioSceneLdspkView);
}

void Editor::resized()
{
    //side + audioSceneView + padding, title + audioSceneView + padding
    setSize(170+440+10, 50+440+10);
    plugtitle->setBoundsToFit(juce::Rectangle<int>(105, 12,500,25), Justification::centredLeft,false);
    
    auto bounds = getLocalBounds().removeFromBottom(getHeight()-50);
    auto side = bounds.removeFromLeft(170);
    side.removeFromTop(35).reduced(6);

    audioSceneLdspkView->setBounds(bounds.reduced(5));
    settingsBtn->setBoundsRelative(0.01f,0.01f,0.05f,0.05f);
    settings->setBoundsInset(BorderSize<int> (1));
    
}

void Editor::toggleSettingsButton(bool isEnabled){
    settingsBtn->setToggleState(isEnabled, sendNotification);
}

void Editor::buttonClicked (Button* buttonThatWasClicked)
{
    if (buttonThatWasClicked == settingsBtn)
    {
        if(settingsBtn->getToggleState()){
            settings->refresh();
            settings->setVisible(true);
        }
        else{
            settings->setVisible(false);
        }
    }
}

} // namespace loudspeaker_renderer
} // namespace dawplugins
} // namespace visr_production_suite
