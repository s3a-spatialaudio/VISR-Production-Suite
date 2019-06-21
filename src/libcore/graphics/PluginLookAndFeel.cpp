#include "JuceHeader.h"
#include "PluginLookAndFeel.h"
#include "../PluginBinaryData.h"

//S3A logo colour
// Colour(147,194,52)
namespace visr_production_suite
{
namespace core
{
namespace graphics
{

using namespace juce;
//==============================================================================
PluginLookAndFeel::PluginLookAndFeel()
{
    //    setColour (TextButton::buttonColourId,          Colours::yellow);
}

PluginLookAndFeel::~PluginLookAndFeel(){}

void PluginLookAndFeel::drawRotarySlider	(	Graphics & 	g,
                                             int 	x,
                                             int 	y,
                                             int 	width,
                                             int 	height,
                                             float 	sliderPosProportional,
                                             float 	rotaryStartAngle,
                                             float 	rotaryEndAngle,
                                             Slider & 	slider )
{
    Image myStrip = ImageCache::getFromMemory (PluginBinaryData::visrknobgrey4x_png,PluginBinaryData::visrknobgrey4x_pngSize);
    
    const double fractRotation = (slider.getValue() - slider.getMinimum())  /   (slider.getMaximum() - slider.getMinimum()); //value between 0 and 1 for current amount of rotation
    const int nFrames = myStrip.getHeight()/myStrip.getWidth(); // number of frames for vertical film strip
    const int frameIdx = (int)ceil(fractRotation * ((double)nFrames-1.0) ); // current index from 0 --> nFrames-1
    
    const float radius = jmin (width / 2.0f, height / 2.0f) ;
    const float centreX = x + width * 0.5f;
    const float centreY = y + height * 0.5f;
    const float rx = centreX - radius - 1.0f;
    const float ry = centreY - radius - 1.0f;
    
    g.drawImage(myStrip,
                (int)rx,
                (int)ry,
                2*(int)radius,
                2*(int)radius,   //Dest
                0,
                frameIdx*myStrip.getWidth(),
                myStrip.getWidth(),
                myStrip.getWidth()); //Source
    
    g.setColour(slider.findColour (Slider::trackColourId).withAlpha(0.4f));
    g.fillEllipse(
                  (int)rx,
                  (int)ry,
                  2*(int)radius,
                  2*(int)radius   //Dest
                  ); //Source
    
}

void PluginLookAndFeel::drawButtonBackground (Graphics& g,
                                              Button& button,
                                              const Colour& backgroundColour,
                                              bool shouldDrawButtonAsHighlighted,
                                              bool shouldDrawButtonAsDown)
{
    Colour bkcolour(Colours::dimgrey);
    auto cornerSize = 6.0f;
    auto bounds = button.getLocalBounds().toFloat().reduced (0.5f, 0.5f);
    
    auto baseColour = bkcolour.withMultipliedSaturation (button.hasKeyboardFocus (true) ? 1.3f : 0.9f)
    .withMultipliedAlpha (button.isEnabled() ? 1.0f : 0.5f);
    
    if (shouldDrawButtonAsDown || shouldDrawButtonAsHighlighted)
        baseColour = baseColour.contrasting (shouldDrawButtonAsDown ? 0.2f : 0.05f);
    
    g.setColour (baseColour);
    
    if (button.isConnectedOnLeft() || button.isConnectedOnRight())
    {
        Path path;
        path.addRoundedRectangle (bounds.getX(), bounds.getY(),
                                  bounds.getWidth(), bounds.getHeight(),
                                  cornerSize, cornerSize,
                                  ! button.isConnectedOnLeft(),
                                  ! button.isConnectedOnRight(),
                                  ! button.isConnectedOnLeft(),
                                  ! button.isConnectedOnRight());
        
        g.fillPath (path);
        
        g.setColour (Colour(30,30,30).withAlpha(0.4f));
        g.strokePath (path, PathStrokeType (1.0f));
    }
    else
    {
        g.fillRoundedRectangle (bounds, cornerSize);
        
        g.setColour (button.findColour (ComboBox::outlineColourId));
        g.drawRoundedRectangle (bounds, cornerSize, 1.0f);
    }
}



void PluginLookAndFeel::drawTableHeaderBackground (Graphics& g, TableHeaderComponent& header)
{
    //    g.fillAll (Colours::white);
    
    auto area = header.getLocalBounds();
    //    area.removeFromTop (area.getHeight() / 2);
    
    auto backgroundColour = header.findColour (TableHeaderComponent::backgroundColourId);
    
    g.setGradientFill (ColourGradient (backgroundColour,
                                       0.0f, (float) area.getY(),
                                       backgroundColour.withMultipliedSaturation (.5f),
                                       0.0f, (float) area.getBottom(),
                                       false));
    g.fillRoundedRectangle (area.toFloat(),3);
    
    g.setColour (header.findColour (TableHeaderComponent::outlineColourId));
    g.fillRoundedRectangle (area.removeFromBottom (1).toFloat(),10);
    
    for (int i = header.getNumColumns (true); --i >= 0;)
        g.fillRoundedRectangle (header.getColumnPosition (i).removeFromRight (1).toFloat(),10);
}

void PluginLookAndFeel::drawTableHeaderColumn (Graphics& g, TableHeaderComponent& header,
                                               const String& columnName, int /*columnId*/,
                                               int width, int height, bool isMouseOver, bool isMouseDown,
                                               int columnFlags)
{
    auto highlightColour = header.findColour (TableHeaderComponent::highlightColourId);
    
    if (isMouseDown)
        g.fillAll (highlightColour);
    else if (isMouseOver)
        g.fillAll (highlightColour.withMultipliedAlpha (0.625f));
    
    Rectangle<int> area (width, height);
    area.reduce (4, 0);
    
    if ((columnFlags & (TableHeaderComponent::sortedForwards | TableHeaderComponent::sortedBackwards)) != 0)
    {
        Path sortArrow;
        sortArrow.addTriangle (0.0f, 0.0f,
                               0.5f, (columnFlags & TableHeaderComponent::sortedForwards) != 0 ? -0.8f : 0.8f,
                               1.0f, 0.0f);
        
        g.setColour (Colour (0x99000000));
        g.fillPath (sortArrow, sortArrow.getTransformToScaleToFit (area.removeFromRight (height / 2).reduced (2).toFloat(), true));
    }
    
    g.setColour (header.findColour (TableHeaderComponent::textColourId));
    g.setFont (Font (height * 0.5f, Font::bold));
    g.drawFittedText (columnName, area, Justification::centred, 1);
}

} // namespace graphics
} // namespace core
} // namespace visr_production_suite
