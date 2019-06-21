#include "PluginEditor.h"
#include <libdawutilities/PluginBinaryData.h>






using namespace juce;

namespace visr
{
namespace dawplugins
{
namespace multi_renderer
{

Editor::Editor (Processor* opan, PannerUI* pui, int pannersize)
: PluginEditorRender(opan, pui, pannersize),
op(*opan)
{
    
    plugtitle = new ImageButton ("ImageButton");
    juceImage = ImageCache::getFromMemory (PluginBinaryData::s3aSceneMultiRenderer_png, PluginBinaryData::s3aSceneMultiRenderer_pngSize);
    plugtitle->setImages (true, true, true,
                          juceImage, 1.0f, Colours::transparentBlack,
                          juceImage, 1.0f, Colours::transparentBlack,
                          juceImage, 1.0f, Colours::transparentBlack,
                          0.5f);
    addAndMakeVisible(plugtitle);
    
    op.addChangeListener(this);
    op.sendChangeMessage();
    
    
    rendererSelector = new ComboBox();
    rendererSelector->addListener (this);
    rendererSelector->setJustificationType (Justification::centred);
    rendererSelector->setScrollWheelEnabled(true);
    rendererSelector->setTextWhenNothingSelected("default VBAP renderer");
    rendererSelector->setSelectedItemIndex(200,NotificationType::dontSendNotification);
    populateRendererMenu();
    addAndMakeVisible (rendererSelector);
    
    resized();
    setup();
    resetLayout();
    
    
    /*******TEMPORARY*******/
    layoutSelector->setVisible(false);
    /**************/
    
}

Editor::~Editor()
{
    op.removeChangeListener(this);
}

void Editor::populateRendererMenu()
{
    rendererSelector->clear(NotificationType::dontSendNotification);
    int i = 0;
    Processor::RendererType rt;
    for (; i < Processor::REND_MAX; i++) {
        rt = static_cast<Processor::RendererType> (i);
        rendererSelector->addItem(op.rendNames[rt], i+1);
        if (op.rt == rt)
            rendererSelector->setSelectedId(i+1);
    }
    rendererSelector->addSeparator();
    rendererSelector->addItem(String("Select from folder..."),i+1);
    
    int currRend = op.parameters.state.getProperty("RendererType", 0);
        rendererSelector->setSelectedId(currRend+1,sendNotification);
        triggerAsyncUpdate();
    
    
}

void Editor::resized()
{
    rendererSelector->setBounds (10, 60, 150, 20);
    
    
    plugtitle->setBounds (105, 7, 300, 40);
}
void Editor::comboBoxChanged (ComboBox* cb)
{
    auto selectedID = cb->getSelectedId();
    PluginEditorRender::comboBoxChanged(cb);
    if (cb== rendererSelector){
        int currRend = op.parameters.state.getProperty("RendererType", 0);
        if(selectedID<(Processor::REND_MAX+1) && selectedID!=currRend+1){
            op.parameters.state.setProperty("RendererType", selectedID-1, nullptr);
            triggerAsyncUpdate();
        }
    }
}

void Editor::changeListenerCallback (ChangeBroadcaster *source)
{
    panner->repaint();
    //    panner->setSource(op.parameters.getParameter("azParamOut")->getValue(),op.parameters.getParameter("elParamOut")->getValue(),true);
    //    printf("LDSPK phi %f the %f \n",op.parameters.getParameter("azParamOut")->getValue(),op.parameters.getParameter("elParamOut")->getValue());
}


void Editor::handleAsyncUpdate(){
    op.resetRenderer();
    
    resetLayout();
}

    
} // namespace multi_renderer
} // namespace dawplugins
} // namespace visr
