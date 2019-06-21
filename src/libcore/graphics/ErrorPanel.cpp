#include "ErrorPanel.h"

namespace visr_production_suite
{
namespace core
{
namespace graphics
{

ErrorPanel::ErrorPanel(){
    errorText = new juce::TextEditor();
    errorText->setColour(juce::TextEditor::ColourIds::textColourId, juce::Colours::red.brighter());
    errorText->setColour(juce::TextEditor::ColourIds::backgroundColourId, juce::Colours::transparentBlack);
    errorText->setJustification(juce::Justification::centred);
    errorText->setMultiLine(true);
    errorText->setSelectAllWhenFocused(true);
    errorText->setReadOnly(true);
    errorText->addListener(this);
    addAndMakeVisible(errorText);
    resized();
};

void ErrorPanel::textEditorTextChanged (juce::TextEditor& t){
    t.setSize(getWidth(), t.getTextHeight()+5);
    errorText->setCentrePosition(getWidth()/2, getHeight()/2);
};


void ErrorPanel::resized() {
    errorText->setSize(getLocalBounds().getWidth(),getLocalBounds().getHeight());
    errorText->setCentrePosition(getWidth()/2, getHeight()/2);
};

void ErrorPanel::paint(juce::Graphics& g){
    g.setColour( juce::Colours::black.withAlpha(0.6f));
    g.fillRect (0, 0, getWidth(),getHeight());
};
    
} // namespace graphics
} // namespace core
} // namespace visr_production_suite
