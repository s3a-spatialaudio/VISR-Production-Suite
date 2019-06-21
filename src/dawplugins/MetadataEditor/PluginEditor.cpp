#include "PluginEditor.h"

using namespace juce;

namespace visr_production_suite
{
namespace dawplugins
{
namespace metadataeditor
{

Editor::Editor (Processor* opan)
: PluginEditorBase(opan),
tabs(opan)
{
    setResizable(true, true);
    
    plugtitle = new ImageButton ("ImageButton");
    juceImage2 = ImageCache::getFromMemory (PluginBinaryData::s3aMetadataEditor_png, PluginBinaryData::s3aMetadataEditor_pngSize);
    plugtitle->setImages (true, true, true,
                          juceImage2, 1.0f, Colours::transparentBlack,
                          juceImage2, 1.0f, Colours::transparentBlack,
                          juceImage2, 1.0f, Colours::transparentBlack,
                          0.5f);
    addAndMakeVisible(plugtitle);
    
    addAndMakeVisible(tabs);
    
    //    op.addChangeListener(this);
    //    op.sendChangeMessage();
    setSize(800,650);
    setResizeLimits (500, 400, 4096, 4096);
    resized();
    
    tabs.setBounds (4,50,getWidth()-8,getHeight()-54);
}

Editor::~Editor()
{
}


//void Editor::resized()
//{
//    setSize(getWidth(),getHeight());
//    tabs.resizeTables();
//    
//}
void Editor::resized()
{
    // This lays out our child components...
    plugtitle->setBoundsToFit(Rectangle<int>(105, 12,500,25), Justification::centredLeft,false);
//
    tabs.resizeTables();
    
}


void Editor::paint (Graphics& g)
{
    //S3A logo colour
    g.fillAll (Colour(20,20,20));
    
//Background OBJ table
//    g.setGradientFill (ColourGradient (Colour(20,20,20).brighter(),getWidth()/2,objTable->getY()-30,
//                                       Colour(20,20,20).darker(),getWidth()/2,objTable->getY()+objTable->getHeight()+30,
//                                       false));
//    g.fillRect (0, objTable->getY()-35, getWidth(),objTable->getY()+objTable->getHeight()+30);
//   
////    DBG(ldspkTable->getY()-30);
//    //Background LDSP table
//    g.setGradientFill (ColourGradient (Colour(20,20,20).brighter(),getWidth()/2,ldspkTable->getY()-30,
//                                       Colour(20,20,20).darker(),getWidth()/2,ldspkTable->getY()+ldspkTable->getHeight()+30,
//                                       false));
//    g.fillRect (0, ldspkTable->getY()-35, getWidth(),ldspkTable->getY()+ldspkTable->getHeight()+30);
//    
//    
    //Background's border
    g.setColour (Colours::black);
    //  g.drawRect (0, 0, (getWidth()-msize)/2-20, (getWidth()+msize)/2+20, 1);
    
    //    g.drawLine(10,50,getWidth()-10,50);
    //    g.drawLine(10,290,getWidth()-10,290);
}
} // namespace metadataeditor
} // namespace dawplugins
} // namespace visr_production_suite

