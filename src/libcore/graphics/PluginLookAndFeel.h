#ifndef PLUGINLOOKANDFEEL_HEADER
#define PLUGINLOOKANDFEEL_HEADER
#include "JuceHeader.h"

namespace visr_production_suite
{
namespace core
{
namespace graphics
{

/**
 * The default look-and-feel for plugins.
 */
class  PluginLookAndFeel   : public juce::LookAndFeel_V4
{
public:
    
    //==============================================================================
    /** Creates the the default look-and-feel for VISR plugins. */
    PluginLookAndFeel();
    
    ~PluginLookAndFeel();
    
    /** Changes the look and feel of the rotary sliders (knobs). */
    void drawRotarySlider	(	juce::Graphics & 	g,
                             int 	x,
                             int 	y,
                             int 	width,
                             int 	height,
                             float 	sliderPosProportional,
                             float 	rotaryStartAngle,
                             float 	rotaryEndAngle,
                             juce::Slider & 	slider ) override;
    
    void drawButtonBackground (juce::Graphics&, juce::Button&, const juce::Colour& backgroundColour,
                               bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;
    
    
    /** Changes the look and feel of the backrounds of table headers. */
    void drawTableHeaderBackground (juce::Graphics&, juce::TableHeaderComponent&) override;
    
    /** Changes the look and feel of table headers. */
    void drawTableHeaderColumn (juce::Graphics&, juce::TableHeaderComponent&, const juce::String& columnName,
                                int columnId, int width, int height, bool isMouseOver,
                                bool isMouseDown, int columnFlags) override;
    
    /** Palette of 12*10 colours to be used for graphical point sources */
    struct SourcePalette{
        juce::Array<juce::Colour *> sliderPalette = {
            
            new juce::Colour(184,241,47),
            new juce::Colour(47, 240, 0),
            new juce::Colour(0,255,255),
            new juce::Colour(84,172,255),
            new juce::Colour(166,74,255),
            new juce::Colour(255, 37, 198),
            new juce::Colour(255, 37, 37),
            new juce::Colour(255, 87, 51 ),
            new juce::Colour(232, 132, 31),
            new juce::Colour(255,222,58),
            
            /********************* 10 colours, repeated below *************************/
            new juce::Colour(184,241,47),
            new juce::Colour(47, 240, 0),
            new juce::Colour(0,255,255),
            new juce::Colour(84,172,255),
            new juce::Colour(166,74,255),
            new juce::Colour(255, 37, 198),
            new juce::Colour(255, 37, 37),
            new juce::Colour(255, 87, 51 ),
            new juce::Colour(232, 132, 31),
            new juce::Colour(255,222,58),
            
            new juce::Colour(184,241,47),
            new juce::Colour(47, 240, 0),
            new juce::Colour(0,255,255),
            new juce::Colour(84,172,255),
            new juce::Colour(166,74,255),
            new juce::Colour(255, 37, 198),
            new juce::Colour(255, 37, 37),
            new juce::Colour(255, 87, 51 ),
            new juce::Colour(232, 132, 31),
            new juce::Colour(255,222,58),
            
            new juce::Colour(184,241,47),
            new juce::Colour(47, 240, 0),
            new juce::Colour(0,255,255),
            new juce::Colour(84,172,255),
            new juce::Colour(166,74,255),
            new juce::Colour(255, 37, 198),
            new juce::Colour(255, 37, 37),
            new juce::Colour(255, 87, 51 ),
            new juce::Colour(232, 132, 31),
            new juce::Colour(255,222,58),
            
            new juce::Colour(184,241,47),
            new juce::Colour(47, 240, 0),
            new juce::Colour(0,255,255),
            new juce::Colour(84,172,255),
            new juce::Colour(166,74,255),
            new juce::Colour(255, 37, 198),
            new juce::Colour(255, 37, 37),
            new juce::Colour(255, 87, 51 ),
            new juce::Colour(232, 132, 31),
            new juce::Colour(255,222,58),
            
            new juce::Colour(184,241,47),
            new juce::Colour(47, 240, 0),
            new juce::Colour(0,255,255),
            new juce::Colour(84,172,255),
            new juce::Colour(166,74,255),
            new juce::Colour(255, 37, 198),
            new juce::Colour(255, 37, 37),
            new juce::Colour(255, 87, 51 ),
            new juce::Colour(232, 132, 31),
            new juce::Colour(255,222,58),
            
            new juce::Colour(184,241,47),
            new juce::Colour(47, 240, 0),
            new juce::Colour(0,255,255),
            new juce::Colour(84,172,255),
            new juce::Colour(166,74,255),
            new juce::Colour(255, 37, 198),
            new juce::Colour(255, 37, 37),
            new juce::Colour(255, 87, 51 ),
            new juce::Colour(232, 132, 31),
            new juce::Colour(255,222,58),
            
            new juce::Colour(184,241,47),
            new juce::Colour(47, 240, 0),
            new juce::Colour(0,255,255),
            new juce::Colour(84,172,255),
            new juce::Colour(166,74,255),
            new juce::Colour(255, 37, 198),
            new juce::Colour(255, 37, 37),
            new juce::Colour(255, 87, 51 ),
            new juce::Colour(232, 132, 31),
            new juce::Colour(255,222,58),
            
            new juce::Colour(184,241,47),
            new juce::Colour(47, 240, 0),
            new juce::Colour(0,255,255),
            new juce::Colour(84,172,255),
            new juce::Colour(166,74,255),
            new juce::Colour(255, 37, 198),
            new juce::Colour(255, 37, 37),
            new juce::Colour(255, 87, 51 ),
            new juce::Colour(232, 132, 31),
            new juce::Colour(255,222,58),
            
            new juce::Colour(184,241,47),
            new juce::Colour(47, 240, 0),
            new juce::Colour(0,255,255),
            new juce::Colour(84,172,255),
            new juce::Colour(166,74,255),
            new juce::Colour(255, 37, 198),
            new juce::Colour(255, 37, 37),
            new juce::Colour(255, 87, 51 ),
            new juce::Colour(232, 132, 31),
            new juce::Colour(255,222,58),
            
            new juce::Colour(184,241,47),
            new juce::Colour(47, 240, 0),
            new juce::Colour(0,255,255),
            new juce::Colour(84,172,255),
            new juce::Colour(166,74,255),
            new juce::Colour(255, 37, 198),
            new juce::Colour(255, 37, 37),
            new juce::Colour(255, 87, 51 ),
            new juce::Colour(232, 132, 31),
            new juce::Colour(255,222,58),
            
            new juce::Colour(184,241,47),
            new juce::Colour(47, 240, 0),
            new juce::Colour(0,255,255),
            new juce::Colour(84,172,255),
            new juce::Colour(166,74,255),
            new juce::Colour(255, 37, 198),
            new juce::Colour(255, 37, 37),
            new juce::Colour(255, 87, 51 ),
            new juce::Colour(232, 132, 31),
            new juce::Colour(255,222,58)
            
            /********************* 10*12 colours *************************/
        };
    };
    
private:
    //S3A logo colour
    //Colour(136, 167, 2);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginLookAndFeel)
};

} // namespace graphics
} // namespace core
} // namespace visr_production_suite

#endif   // LookAndFeelHEADER
