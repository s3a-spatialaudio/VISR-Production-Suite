#include "PluginEditorLoudspeakers.h"

namespace visr_production_suite
{
namespace core
{
namespace graphics
{

using namespace juce;

PluginEditorLoudspeakers::PluginEditorLoudspeakers (processors::PluginProcessorLoudspeakers* opan, juce::Colour background)
: PluginEditorBase (opan, background),
isDefaultConfigOn(!opan->currentLayoutFile.exists()),
op(*opan){
    error = op.parameters.state.getOrCreateChildWithName("Error",nullptr);
}

void PluginEditorLoudspeakers::setup(SceneWithLoudspeakers* pui){
    //Window's total size
    audioSceneLdspkView = pui;

    layoutSelector = new ComboBox();
    layoutSelector->addListener (this);
    layoutSelector->setJustificationType (Justification::centred);
    layoutSelector->setScrollWheelEnabled(true);
    layoutSelector->setTextWhenNothingSelected("default stereo config");
    layoutSelector->setSelectedItemIndex(200,NotificationType::dontSendNotification);
    updatePopupDirectories();
    addAndMakeVisible (layoutSelector);
    
    PluginEditorLoudspeakers::resized();
}
PluginEditorLoudspeakers::~PluginEditorLoudspeakers()
{
    audioSceneLdspkView=nullptr;
}

void PluginEditorLoudspeakers::updatePopupDirectories()
{
    layoutSelector->clear(NotificationType::dontSendNotification);
    int i = 0;
    for (; i < op.loudspLayoutFiles.size(); i++) {
        layoutSelector->addItem(op.loudspLayoutFiles.getUnchecked(i).getFileNameWithoutExtension(), i+1);
        if (op.currentLayoutFile == op.loudspLayoutFiles.getUnchecked(i))
            layoutSelector->setSelectedId(i+1);
    }
    layoutSelector->addSeparator();
    layoutSelector->addItem(String("Select from folder..."),i+1);
}

static void alertBoxResultChosen (int result, PluginEditorLoudspeakers* p)
{
    if(result==1){
        p->triggerAsyncUpdate();
        
    }else{
        if(!p->isDefaultConfigOn)
            p->layoutSelector->setSelectedItemIndex(p->op.loudspLayoutFiles.indexOf(p->op.currentLayoutFile),NotificationType::dontSendNotification);
        else
            p->layoutSelector->setSelectedItemIndex(200, NotificationType::dontSendNotification);
        return;
    }
}

void PluginEditorLoudspeakers::comboBoxChanged (ComboBox* cb)
{
    auto selectedID = cb->getSelectedId();
    
    if(cb == layoutSelector){
        int currentLayoutID;
        if(!isDefaultConfigOn)
            currentLayoutID= op.loudspLayoutFiles.indexOf(op.currentLayoutFile)+1;
        
        if(selectedID<(op.loudspLayoutFiles.size()+1) && selectedID!=currentLayoutID && !isDefaultConfigOn){
            int res;
            try{
                res = op.VISRCheckLayout(selectedID-1);
                if(res==1){
                    AlertWindow::showOkCancelBox (AlertWindow::QuestionIcon,
                                                  "Gaps in loudspeaker layout channels",
                                                  "The configuration you're loading has some gaps in loudspeaker channel numbers. Make sure you know which ones are missing. Do you want to load the layout anyway?",
                                                  String(),
                                                  String(),
                                                  0,
                                                  ModalCallbackFunction::forComponent (alertBoxResultChosen, this));
                    
                }else{
                    triggerAsyncUpdate();
                    
                }
            }catch( std::exception const &ex )
            {
                String errorMsg;
                errorMsg << ex.what();
                error.setProperty("ErrorMsg", errorMsg, nullptr);
                std::cerr << errorMsg << std::endl;
                if(!isDefaultConfigOn)
                    cb->setSelectedItemIndex(op.loudspLayoutFiles.indexOf(op.currentLayoutFile), NotificationType::dontSendNotification);
                
                return;
            }
            
        }else if(selectedID==(op.loudspLayoutFiles.size()+1)||(isDefaultConfigOn)){
            FileChooser layoutFolder ("Select the loudspeaker layout configuration file to load...",
                                      op.lastDirectory,
                                      "*.xml");
            if (layoutFolder.browseForFileToOpen())
            {
                File nextLayout (layoutFolder.getResult());
                op.VISRFindLayouts(nextLayout.getParentDirectory());
                int res;
                try{
                    res = op.VISRCheckLayout(nextLayout);
                    if(res==1){
                        AlertWindow::showOkCancelBox (AlertWindow::QuestionIcon,
                                                      "Gaps in loudspeaker layout channels",
                                                      "The configuration you're loading has some gaps in loudspeaker channel numbers. Make sure you know which ones are missing. Do you want to load the layout anyway?",
                                                      String(),
                                                      String(),
                                                      0,
                                                      ModalCallbackFunction::forComponent (alertBoxResultChosen, this));
                    }else{
                        triggerAsyncUpdate();
                    }
                    
                }catch( std::exception const &ex )
                {
                    String errorMsg;
                    errorMsg << ex.what();
                    error.setProperty("ErrorMsg", errorMsg, nullptr);
                    std::cerr << errorMsg << std::endl;
                    if(!isDefaultConfigOn)
                        cb->setSelectedItemIndex(op.loudspLayoutFiles.indexOf(op.currentLayoutFile), NotificationType::dontSendNotification);
                    else
                        cb->setSelectedItemIndex(200, NotificationType::dontSendNotification);
                    return;
                }
                
            }else{
                cb->setSelectedItemIndex(currentLayoutID, NotificationType::dontSendNotification);
                updatePopupDirectories();
            }
        }
        // if anything goes wrong, notify that by removing possible old error messages
        error.removeAllProperties(nullptr);
    }
}

void PluginEditorLoudspeakers::buttonClicked (Button* buttonThatWasClicked)
{
    if (buttonThatWasClicked->getName().contains("ring")){
        if(buttonThatWasClicked->getToggleState()){
            audioSceneLdspkView->enableRing(buttonThatWasClicked->getTooltip().toStdString());
        }else{
            audioSceneLdspkView->disableRing(buttonThatWasClicked->getTooltip().toStdString());
        }
    }
}

void PluginEditorLoudspeakers::resetLayout(){
    audioSceneLdspkView->resetLayout();
    //    updateRings();
}

void PluginEditorLoudspeakers::updateRings(){
    for(ToggleButton* tb : ringbt){
        removeChildComponent(tb);
    }
    ringbt.clear();
    
    ToggleButton* b;
    float initPos = 140;
    audioSceneLdspkView->elevs.reverse();
    for(float v : audioSceneLdspkView->elevs){
        char str[10];
        std::string s = str;
        
        b = ringbt.add(new ToggleButton("ring "+s+"°"));
        b->setTooltip(std::to_string(v));
        b->setClickingTogglesState(true);
        b->setToggleState(true, NotificationType::dontSendNotification);
        b->setColour(ToggleButton::tickColourId, Colours::white);
        b->setColour(ToggleButton::textColourId, Colours::white);
        
        b->addListener(this);
        b->setBounds(10, initPos,200,25);
        initPos+=30;
        addAndMakeVisible(b);
    }
}

void PluginEditorLoudspeakers::handleAsyncUpdate(){
    op.VISRChangeLayout();
    op.PluginReset();
    resetLayout();
    isDefaultConfigOn = false;
    updatePopupDirectories();
}

void PluginEditorLoudspeakers::resized()
{
    PluginEditorBase::resized();
    auto bounds = getLocalBounds().removeFromBottom(getHeight()-50);
    auto side = bounds.removeFromLeft(170);
    layoutSelector->setBounds (side.removeFromTop(35).reduced(6));
}
    
} // namespace graphics
} // namespace core
} // namespace visr_production_suite
