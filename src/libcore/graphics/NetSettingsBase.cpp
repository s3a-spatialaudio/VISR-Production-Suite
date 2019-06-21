#include "NetSettings.h"

namespace visr_production_suite
{
namespace core
{
namespace graphics
{

using namespace juce;

NetSettingsBase::NetSettingsBase (ValueTree vt, String name)
    :  contentID(name),
    properties(vt)
{
    addAndMakeVisible (ip = new TextEditor ("new text editor"));
    ip->addListener (this);
    ip->setMultiLine (false);
    ip->setReturnKeyStartsNewLine (false);
    ip->setReadOnly (false);
    ip->setScrollbarsShown (false);
    ip->setCaretVisible (true);
    ip->setPopupMenuEnabled (true);
    ip->setText (properties.getProperty(contentID+"_ip","127.0.0.1"));

    addAndMakeVisible (port = new TextEditor ("new text editor"));
    port->addListener (this);
    port->setMultiLine (false);
    port->setReturnKeyStartsNewLine (false);
    port->setReadOnly (false);
    port->setScrollbarsShown (false);
    port->setCaretVisible (true);
    port->setPopupMenuEnabled (true);
    port->setText (properties.getProperty(contentID+"_port",8001));

    setSize (203, 145);
}

NetSettingsBase::~NetSettingsBase(){}

//==============================================================================
void NetSettingsBase::paint (Graphics& g)
{
    g.fillAll (Colours::white);

    g.setGradientFill (ColourGradient (Colour (0xff4e4e4e),
                                       static_cast<float> (proportionOfWidth (0.6314f)), static_cast<float> (proportionOfHeight (0.5842f)),
                                       Colours::black,
                                       static_cast<float> (proportionOfWidth (0.1143f)), static_cast<float> (proportionOfHeight (0.0800f)),
                                       true));
    g.fillRect (0, 0, 220, 360);

    g.setColour (Colours::black);
    g.drawRect (0, 0, 220, 360, 1);

    g.setColour (Colour(20,20,20).brighter().withAlpha(0.8f));
    g.fillRoundedRectangle (19.0f, 45.0f, 163.0f, 90.0f, 4.000f);

    g.setColour (Colours::black);
    g.setFont (Font (14.20f, Font::bold));
    g.drawText ("UDP send",
                101, 46, 80, 26,
                Justification::centredLeft, true);

    g.setColour (Colours::white);
    g.setFont (Font (17.20f, Font::bold));
    g.drawText ("Network settings",
                -2, 2, 208, 30,
                Justification::centred, true);

    g.setColour (Colours::black);
    g.setFont (Font (14.20f, Font::plain));
    g.drawText ("ip",
                24, 78, 25, 26,
                Justification::centredRight, true);

    g.setColour (Colours::black);
    g.setFont (Font (14.20f, Font::plain));
    g.drawText ("port",
                24, 101, 25, 26,
                Justification::centredRight, true);
}

void NetSettingsBase::resized()
{
    ip->setBounds (56, 80, 120, 20);
    port->setBounds (56, 106, 120, 20);
}

void NetSettingsBase::textEditorFocusLost (TextEditor& ed)
{
    this->triggerAsyncUpdate();
}

void NetSettingsBase::textEditorReturnKeyPressed (TextEditor& ed)
{
    this->triggerAsyncUpdate();
}

void NetSettingsBase::handleAsyncUpdate(){
    String pnm = ip->getText();
    String ptx = port->getText();
    if(!pnm.isEmpty()) properties.setProperty(contentID+"_ip", ip->getText(),nullptr);
    if(!ptx.isEmpty()) properties.setProperty(contentID+"_port", port->getText(),nullptr);
}
    
} // namespace graphics
} // namespace core
} // namespace visr_production_suite


