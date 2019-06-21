#include "PluginEditor.h"
#include <libcore/PluginBinaryData.h>

using namespace juce;

namespace visr_production_suite
{
namespace dawplugins
{
namespace cap_renderer
{

Editor::Editor (Processor* opan)
: PluginEditorLoudspeakers(opan, Colour(20,40,20)),
op(*opan)
{
    audioSceneHeadAndLdspkView = new SceneWithHeadAndLdspk(op);
    audioSceneHeadAndLdspkView->setName ("CAP Renderer's sphere");
    op.attach(audioSceneHeadAndLdspkView);
    addAndMakeVisible (audioSceneHeadAndLdspkView);
    
    plugtitle = new ImageButton ("ImageButton");
    juce::Image title = ImageCache::getFromMemory (PluginBinaryData::s3aCAPRenderer_png, PluginBinaryData::s3aCAPRenderer_pngSize);
    plugtitle->setImages (true, true, true,
                          title, 1.0f, Colours::transparentBlack,
                          title, 1.0f, Colours::transparentBlack,
                          title, 1.0f, Colours::transparentBlack,
                          0.5f);
    addAndMakeVisible(plugtitle);
    
    setup(audioSceneHeadAndLdspkView);
    resized();
    resetLayout();
}

Editor::~Editor()
{
    op.detach(audioSceneHeadAndLdspkView);
}

void Editor::resized()
{
    //side + audioSceneView + padding, title + audioSceneView + padding
    setSize(170+440+10, 50+440+10);
    plugtitle->setBoundsToFit(juce::Rectangle<int>(105, 12,500,25), Justification::centredLeft,false);
    
    auto bounds = getLocalBounds().removeFromBottom(getHeight()-50);
    auto side = bounds.removeFromLeft(170);
    side.removeFromTop(35).reduced(6);
    
    audioSceneHeadAndLdspkView->setBounds(bounds.reduced(5));
}
    
} // namespace cap_renderer
} // namespace dawplugins
} // namespace visr_production_suite
