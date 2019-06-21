#include "PluginEditorObject.h"
#include <libcore/PluginBinaryData.h>
#include <libcore/IDs.h>

using namespace juce;

namespace visr_production_suite
{
namespace core
{
namespace graphics
{

PluginEditorObject::PluginEditorObject (processors::PluginProcessorBase * opan)
: PluginEditorBase (opan),
op(*opan),
objAzLabel (String(), "Azimuth:"),
objElLabel (String(), "Elevation:"),
objRadLabel(String(), "Radius:"),
objIDLabel (String(), "Object number:")
{
    metadataView = new Object(op);
    addAndMakeVisible (metadataView);
    metadataView->setName ("new metadataView");
    
    objAz = new Slider();
    addAndMakeVisible (objAz);
    objAz->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    objAz->setTextBoxStyle (Slider::TextBoxRight, false, 50, 20);
    objAz->addListener(this);
    
    objEl = new Slider();
    addAndMakeVisible (objEl);
    objEl->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    objEl->setTextBoxStyle (Slider::TextBoxRight, false, 50, 20);
    objEl->addListener(this);
    
    objRad = new Slider();
    addAndMakeVisible (objRad);
    objRad->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    objRad->setTextBoxStyle (Slider::TextBoxRight, false, 50, 20);
    objRad->addListener(this);
    
    objID = new Slider("ID");
    addAndMakeVisible (objID);
    objID->setSliderStyle (Slider::IncDecButtons);
    objID->setIncDecButtonsMode(Slider::incDecButtonsNotDraggable);
    objID->setTextBoxStyle (Slider::TextBoxRight, false, 50, 20);
    
    objID->addListener(this);
    
    objAzLabel.attachToComponent (objAz, false);
    objAzLabel.setFont (Font (12.0f));
    objElLabel.attachToComponent (objEl, false);
    objElLabel.setFont (Font (12.0f));
    objRadLabel.attachToComponent (objRad, false);
    objRadLabel.setFont (Font (12.0f));
    objIDLabel.attachToComponent (objID, false);
    objIDLabel.setFont (Font (12.0f));
    
    att_objAz = new juce::AudioProcessorValueTreeState::SliderAttachment (op.parameters, IDs::azimuth.toString(), *objAz);
    att_objEl = new juce::AudioProcessorValueTreeState::SliderAttachment (op.parameters, IDs::elevation.toString(), *objEl);
    att_objRad = new juce::AudioProcessorValueTreeState::SliderAttachment (op.parameters, IDs::radius.toString(), *objRad);
    att_objID = new juce::AudioProcessorValueTreeState::SliderAttachment (op.parameters, IDs::objID.toString(), *objID);
    
    resized();
}

PluginEditorObject::~PluginEditorObject()
{}

void PluginEditorObject::paint (Graphics& g)
{
    //Background
    g.setGradientFill (ColourGradient (Colour(20,20,20).brighter(),getWidth()/2,getHeight()/2,
                                       Colour(20,20,20).darker(),getWidth(),getHeight(),
                                       true));
    g.fillRect (0, 0, getWidth(),getHeight());
    
    g.setColour(Colour(100,100,100).withAlpha(0.2f));
    g.fillRect (0, 0, getWidth(),50);
    
    g.setColour(Colour(100,100,100).withAlpha(0.2f));
    g.fillRect (0, 50, 140,getHeight());
    
    //Background's border
    g.setColour(Colour(70,70,70));
    
    g.drawLine(10,50,getWidth(),50);
    g.drawLine(140,50,140,getHeight());
}

void PluginEditorObject::resized()
{
    //slidersSide + metadataView + padding, title + metadataView + padding
    setSize (140+320+10,50+320+10);
    
    auto bounds = getLocalBounds().removeFromBottom(getHeight()-50);
    auto side = bounds.removeFromLeft(140);
    
    auto panelWidth = bounds.getHeight() / 5.5;
    side.removeFromLeft(10);
    objRad->setBounds (side.removeFromBottom (panelWidth).reduced(10));
    objEl->setBounds (side.removeFromBottom (panelWidth).reduced(10));
    objAz->setBounds (side.removeFromBottom (panelWidth).reduced(10));
    side.removeFromBottom (panelWidth*0.3).reduced(10);
    objID->setBounds(side.removeFromBottom (panelWidth*0.7).reduced(10));
    
    metadataView->setBounds(bounds.reduced(5));
}

void PluginEditorObject::sliderValueChanged (Slider* slider){
    metadataView->repaint();
}
    
} // namespace graphics
} // namespace core
} // namespace visr_production_suite
