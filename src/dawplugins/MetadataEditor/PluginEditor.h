/*
 ==============================================================================
 
 This file was auto-generated!
 
 It contains the basic framework code for a JUCE plugin editor.
 
 ==============================================================================
 */

#ifndef METADATAEDITOR_INCLUDED
#define METADATAEDITOR_INCLUDED

#include "JuceHeader.h"
#include "PluginProcessor.h"
#include "MDOMetadataTable.h"
#include <libcore/graphics/PluginEditorBase.h>
#include <libcore/PluginBinaryData.h>
#include <libcore/graphics/NetSettingsBase.h>

namespace visr_production_suite
{
namespace dawplugins
{
namespace metadataeditor
{
/**
* @author Giacomo Costantini giacomo.costantini3@gmail.com
*/
class MDOTab  : public juce::Component,
public juce::Button::Listener,
public juce::ChangeListener
{
public:
    MDOTab(Processor* opan, juce::String jsonMetaProcName, juce::String jsonKeyName, juce::String modelName):
    processor(*opan),
    vtreeName(modelName.trimCharactersAtEnd("Model")),
    jsonKeyName(jsonKeyName)
    {
        processor.addChangeListener(this);
        juce::String newTargetHost = processor.parameters.state.getProperty(jsonKeyName+"_ip","127.0.0.1");
        int newTargetPort = processor.parameters.state.getProperty(jsonKeyName+"_port",9001);
        processor.parameters.state.setProperty(jsonKeyName+"_ip", newTargetHost,nullptr);
        processor.parameters.state.setProperty(jsonKeyName+"_port",newTargetPort,nullptr);
        
        mdoTable = new TableComponent(    jsonMetaProcName,jsonKeyName,
                                          processor.parameters.state.getOrCreateChildWithName(vtreeName,nullptr),
                                          processor.xmlModel->getChildByName(modelName));
        
        
        
        
        addAndMakeVisible(mdoTable);
        delRowBtn = new juce::TextButton ("-");
        addRowBtn = new juce::TextButton ("+");
        jsonSendBtn = new juce::TextButton ("Send");
        
        addRowBtn->addListener(this);
        delRowBtn->addListener(this);
        jsonSendBtn->addListener(this);
        
        autoSend = new juce::ToggleButton();
        autoSend->setToggleState((int) processor.parameters.state.getProperty("autosend",false), juce::dontSendNotification);
        autoSend->addListener(this);

        addAndMakeVisible (autoSend);
        addAndMakeVisible(addRowBtn);
        addAndMakeVisible(delRowBtn);
        addAndMakeVisible(jsonSendBtn);
        
        autoSendLab = new juce::Label ("Metadata send");
        autoSendLab->setText("auto",juce::NotificationType::dontSendNotification);
        autoSendLab->setFont (juce::Font ("Verdana",12.00f, juce::Font::plain));
        //        labLfo->setEditable (false, false, false);
        autoSendLab->setColour(juce::Label::textColourId, juce::Colours::white);
        addAndMakeVisible(autoSendLab);
        
        addAndMakeVisible (netSettings = new juce::ImageButton ("new button"));
        netSettings->setButtonText (TRANS("settings"));
        netSettings->addListener (this);
        
        netSettings->setImages (false, true, true,
                                juce::ImageCache::getFromMemory (PluginBinaryData::settings_png, PluginBinaryData::settings_pngSize), 1.000f, juce::Colour (180,180,180),
                                juce::ImageCache::getFromMemory (PluginBinaryData::settings_png, PluginBinaryData::settings_pngSize), .5f, juce::Colour (0x00000000),
                                juce::ImageCache::getFromMemory (PluginBinaryData::settings_png, PluginBinaryData::settings_pngSize), 1.000f, juce::Colour (180,180,180));
        
        metadataSendGroup = new juce::GroupComponent ("direction", "Metadata send");
        addAndMakeVisible(metadataSendGroup);
        
        numRowsGroup = new juce::GroupComponent ("direction", "Table rows");
        addAndMakeVisible(numRowsGroup);
        processor.sendChangeMessage();
        
    }
    void buttonClicked (juce::Button* buttonThatWasClicked) override{
        if (buttonThatWasClicked == addRowBtn)
        {
            mdoTable->addRow();
//            std::cout<<"PROC STATE DUMP ADD "<<processor.parameters.state.toXmlString()<<std::endl;
        }else if (buttonThatWasClicked == delRowBtn)
        {
            mdoTable->deleteRow();
        }else if (buttonThatWasClicked == autoSend)
        {
            processor.parameters.state.setProperty("autosend",autoSend->getToggleState(),nullptr);
            mdoTable->setAutosend(autoSend->getToggleState());
        } else if (buttonThatWasClicked == jsonSendBtn){
            mdoTable->UDPsend();
        }else if (buttonThatWasClicked == netSettings)
        {
            if (!netSettingsDialog)
            {
                juce::DialogWindow::LaunchOptions launchOptions;
                launchOptions.dialogTitle = juce::String("Settings");
                launchOptions.content.setOwned(new core::graphics::NetSettingsBase(processor.parameters.state, jsonKeyName));
                launchOptions.componentToCentreAround = this;
                launchOptions.escapeKeyTriggersCloseButton = true;
                launchOptions.useNativeTitleBar = true;
                launchOptions.resizable = false;
                launchOptions.useBottomRightCornerResizer = false;
                netSettingsDialog = launchOptions.launchAsync();
            }
        }
    }
    
    void changeListenerCallback (juce::ChangeBroadcaster *source) override
    {
        autoSend->setToggleState((int) processor.parameters.state.getProperty("autosend",false),juce::dontSendNotification);
        mdoTable->setAutosend((int) processor.parameters.state.getProperty("autosend",false));
        mdoTable->UDPsend();
        mdoTable->updateContent(processor.parameters.state.getOrCreateChildWithName(vtreeName,nullptr));
    }
    
    ~MDOTab(){
        processor.removeChangeListener(this);
    }
    void resized() override{
//        auto totW = getWidth();
//        auto totH = getHeight();
//        auto halfW = totW/2+120;
//        metadataSendGroup->setBounds (halfW-270, 5, 150, 60);
//        autoSendLab->setBounds(halfW-260,20, 30, 20);
//        autoSend->setBounds(halfW-257,30, 30, 30);
//        jsonSendBtn->setBounds(halfW-220,25, 50, 30);
//        netSettings->setBounds (halfW-160, 27, 26, 25);
//        numRowsGroup->setBounds (halfW-110, 5, 95, 60);
//        delRowBtn->setBounds(halfW-100,25, 30, 30);
//        addRowBtn->setBounds(halfW-60,25, 30, 30);

        
        metadataSendGroup->setBounds (6, 5, 150, 60);
        autoSendLab->setBounds(16,20, 30, 20);
        autoSend->setBounds(19,30, 30, 30);
        jsonSendBtn->setBounds(50,25, 50, 30);
        netSettings->setBounds (110, 27, 26, 25);
        
        numRowsGroup->setBounds (160, 5, 95, 60);
        delRowBtn->setBounds(170,25, 30, 30);
        addRowBtn->setBounds(210,25, 30, 30);
        
        mdoTable->setBounds(0,65,getWidth(),getHeight()-65);
    };
private:
    
    Processor& processor;
    juce::String const vtreeName;
    juce::String const jsonKeyName;
    
    juce::ScopedPointer<juce::GroupComponent> metadataSendGroup;
    juce::ScopedPointer<juce::Label> autoSendLab;
    juce::ScopedPointer<juce::ToggleButton> autoSend;
    juce::ScopedPointer<juce::TextButton> jsonSendBtn;
    juce::ScopedPointer<juce::ImageButton> netSettings;
    
    juce::ScopedPointer<juce::GroupComponent> numRowsGroup;
    juce::ScopedPointer<juce::TextButton> addRowBtn;
    juce::ScopedPointer<juce::TextButton> delRowBtn;
    
    juce::ScopedPointer<TableComponent> mdoTable;
    
    juce::Component::SafePointer<juce::DialogWindow> netSettingsDialog;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MDOTab)
};

class TabsComponent  : public juce::TabbedComponent
{
public:
    TabsComponent(Processor* op)
    : TabbedComponent (juce::TabbedButtonBar::TabsAtTop)
    {
        mob = new MDOTab(op, "addObjectMetadata","objects", "ObjectMetadataModel");
        mls = new MDOTab(op, "mdoProduction","loudspeakers","LoudspeakerMetadataModel");
        mgr = new MDOTab(op, "addObjectMetadata","groups", "GroupMetadataModel" );
        
        auto colour = findColour (juce::ResizableWindow::backgroundColourId);
        addTab ("MDO Objects",      colour, mob, true);
        addTab ("MDO Loudspeakers", colour, mls, true);
        addTab ("MDO Groups",       colour, mgr, true);
    }
    
    void resizeTables(){
        auto parent = getParentComponent();
        
        auto totW = parent->getWidth();
        auto totH = parent->getHeight();
        setSize(totW-8,totH-54);
//        mob->resized();
//        mls->resized();
//        mgr->resized();
    }
    
private:
    juce::ScopedPointer<MDOTab> mob;
    juce::ScopedPointer<MDOTab> mls;
    juce::ScopedPointer<MDOTab> mgr;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TabsComponent)
};


class Editor  : public core::graphics::PluginEditorBase
{
public:
    Editor (Processor* op);
    ~Editor();

private:
    //Overridden
    void resized() override;
    
    void paint (juce::Graphics& g) override;
    
    juce::ScopedPointer<juce::ImageButton> plugtitle;
    juce::Image juceImage2;
    
    
    TabsComponent tabs;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Editor)
};

} // namespace metadataeditor
} // namespace dawplugins
} // namespace visr_production_suite


#endif
