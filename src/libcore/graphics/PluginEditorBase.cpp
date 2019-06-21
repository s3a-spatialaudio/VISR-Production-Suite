#include "PluginEditorBase.h"
#include <libcore/PluginBinaryData.h>

namespace visr_production_suite
{
namespace core
{
namespace graphics
{

using namespace juce;

PluginEditorBase::PluginEditorBase(AudioProcessor * opan, Colour backgr)
: AudioProcessorEditor (opan),
ttw(this,100),
background(backgr)
{
    setLookAndFeel(&pluginLookAndFeel);

    logo = new ImageButton ("ImageButton");
    juceImage = ImageCache::getFromMemory (PluginBinaryData::s3a_logo_wh_png, PluginBinaryData::s3a_logo_wh_pngSize);
    logo->setImages (true, true, true,
                     juceImage, 1.0f, Colours::transparentBlack,
                     juceImage, 0.7f, Colours::transparentBlack.darker(),
                     juceImage, 1.0f, Colours::transparentBlack,
                     0.5f);
    String s = ProjectInfo::versionString;
    logo->setTooltip("v"+s);
    addAndMakeVisible(logo);
    
    setSize (475,370);
    resized();
}

PluginEditorBase::~PluginEditorBase(){
    // this is to avoid the leak warning: the look and feel needs to be freed before it's destroyed
    setLookAndFeel(nullptr);
}


void PluginEditorBase::paint (Graphics& g)
{    
        g.setGradientFill (ColourGradient (background.brighter(),getWidth()/2,getHeight()/2,
                                           background.darker(),getWidth(),getHeight(),
                                           true));
        g.fillRect (0, 0, getWidth(),getHeight());
        g.setGradientFill (ColourGradient (Colour(70,70,70),3,52,
                                           Colour(60,60,60),3,50,
                                           false));
//        g.drawLine(3,50,getWidth(),50,2);

    //Background's border
    g.setColour(background.brighter(0.3));
    g.setGradientFill (ColourGradient (background.brighter(0.5),getWidth()/2,50,
                                       background.brighter(0.05),getWidth(),60,
                                       true));
    g.drawLine(3,50,getWidth(),50);
    
    
//    g.setColour(Colour(70,70,70));
//    g.drawLine(10,50, (float)getWidth()-10,50);
//    g.drawLine(170,50,170, (float)getHeight());
}

void PluginEditorBase::resized()
{
    logo->setBounds (10, 5, 70, 40);
}
    
} // namespace graphics
} // namespace core
} // namespace visr_production_suite
