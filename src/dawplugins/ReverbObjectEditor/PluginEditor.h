#ifndef REVERBOBJECTEDITOR_EDITOR_H_INCLUDED
#define REVERBOBJECTEDITOR_EDITOR_H_INCLUDED

#include "JuceHeader.h"
#include "PluginProcessor.h"
#include <libcore/graphics/PluginEditorObject.h>

// For Room Loading
#include <libobjectmodel/point_source_with_reverb.hpp>
#include <libobjectmodel/point_source_with_reverb_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>

namespace visr_production_suite
{
namespace dawplugins
{
namespace reverbobject_editor
{

    
class Editor: public core::graphics::PluginEditorObject, public juce::Button::Listener
{
public:
    /**
     * Constructor
     * @param op  object processor
     */
    Editor (Processor* op);
    
    /**
     * Destructor
     */
    ~Editor();
    
protected:
    
    /**
     * Standard JUCE functionality for resize()
     */
    void resized() override;
    
    /**
     * Standard JUCE function that detects button clicked, including the loadJsonButton
     * @param buttonThatWasClicked [description]
     */
    void buttonClicked (juce::Button* buttonThatWasClicked) override;
    
private:
    /**
     * Unused. TODO: Remove.
     * Was initiall created to provide hover tooltips for users. Was not necessary.
     */
    enum class Parameter { objectLevel=0, azimuth, elevation, radius, rLevel, rDelay, rFrequency, rResonance, onSetDelay, lAttackTime, lDecay1, lDecay2, lDecay3, lDecay4, lDecay5, lDecay6, lDecay7, lDecay8, lDecay9, lLevel1, lLevel2, lLevel3, lLevel4, lLevel5, lLevel6, lLevel7, lLevel8, lLevel9 };
    
    /**
     * Reference to the processor, initialised within the constructor.
     */
    Processor& processor ;

    void addControls();
    
    juce::ScopedPointer<juce::ComboBox> layoutSelector;
    
    bool isDefaultConfigOn = true;
    
    void addSliderListeners();
    
    juce::AudioProcessorParameter* getParameterFromSlider (const juce::Slider* slider) const;
    
    void enableRoomLoading();
    
    /**
     * Plugin title image.
     */
    juce::ScopedPointer<juce::ImageButton> plugtitle;
    juce::Image juceImage2;
    
    void setBoundariesOfUI();
    
    void bindEditor(Processor& reverbProcessor);
    
    void addRoomsToComboBox();
    
    /**
     * Parse functionality used to load ComboBox UIs, and for new imported JSON files,
     * @param tree [description]
     * @param src  [description]
     */
    void parse(boost::property_tree::ptree const & tree, visr::objectmodel::Object & src);
    
    /**
     * Load a .json file to add new JSON to the RoomRIRs.
     */
    void openButtonClicked();
    
    /**
     * [OPTIONAL] implementation - can playback a .wav file that is loaded.
     */
    juce::AudioTransportSource transportSource;
    
    /**
     * Load room parameters from JSON file
     */
    juce::ToggleButton loadJsonButton;
    
    
    juce::TextButton createRIRButton;
    
    void createJSONFromRIR();
    
    /**
     * Combo box for loading RIR from the library
     */
    juce::ComboBox room;
    juce::Label infoLabel;
    juce::TextButton rsaoImportButton;
    juce::TextButton rsaoCalculateButton;
    juce::ComboBox reflectionCountBox;
    juce::Array<juce::ScopedPointer<juce::AudioProcessorValueTreeState::SliderAttachment>> sliders;
    juce::Array<juce::ScopedPointer<juce::AudioProcessorValueTreeState::ComboBoxAttachment>> combos;
    juce::Array<juce::ScopedPointer<juce::AudioProcessorValueTreeState::ButtonAttachment>> buttons;
    
    void setTooltipsAndComponentIDs();
    juce::Component* getComponentUnderTheMouse() const;
    juce::String getParameterKeyForComponentUnderTheMouse() const;
    juce::String getParameterInfo(juce::String paramKey) const;
    juce::SharedResourcePointer<juce::TooltipWindow> tooltip;
    
    static constexpr int numberOfParameters = int (Parameter::lLevel9) + 1;
    
    static constexpr const char* parameterNames[numberOfParameters] =
    {   "Object Level",
        "Azimuth",
        "Elevation",
        "Distance",
        "Reflection Levels",
        "Reflection Delays",
        "Reflection Frequency",
        "Reflection Resonance",
        "onSetDelay",
        "Late Attack",
        "Late Level 1",
        "Late Level 2",
        "Late Level 3",
        "Late Level 4",
        "Late Level 5",
        "Late Level 6",
        "Late Level 7",
        "Late Level 8",
        "Late Level 9",
        "Late Decay 1",
        "Late Decay 2",
        "Late Decay 3",
        "Late Decay 4",
        "Late Decay 5",
        "Late Decay 6",
        "Late Decay 7",
        "Late Decay 8",
        "Late Decay 9"
    };
    
    static constexpr const char* parameterKeys[numberOfParameters] =
    {   "objLevel",
        "azimuth",
        "elevation",
        "radius",
        "Reflection Levels",
        "Reflection Delays",
        "Reflection Frequency",
        "Reflection Resonance",
        "onSetDelay",
        "Late Attack",
        "Late Level 1",
        "Late Level 2",
        "Late Level 3",
        "Late Level 4",
        "Late Level 5",
        "Late Level 6",
        "Late Level 7",
        "Late Level 8",
        "Late Level 9",
        "Late Decay 1",
        "Late Decay 2",
        "Late Decay 3",
        "Late Decay 4",
        "Late Decay 5",
        "Late Decay 6",
        "Late Decay 7",
        "Late Decay 8",
        "Late Decay 9"
    };
    
    static constexpr const char* parameterDescriptions[numberOfParameters] =
    {
        "Object-level input",
        "360 rotation of source",
        "180 magnitude of source",
        "Distance of source from the centre",
        "Early reflection levels for all reflections"
        "Time of arrival for all early reflections",
        "Cutoff for early reflection frequency filter",
        "Resonance for the early reflection frequency filter",
        "Onset delay for the late reverb",
        "Overall attack time of the late reverb",
        "Late Level 1",
        "Late Level 2",
        "Late Level 3",
        "Late Level 4",
        "Late Level 5",
        "Late Level 6",
        "Late Level 7",
        "Late Level 8",
        "Late Level 9",
        "Late Decay 1",
        "Late Decay 2",
        "Late Decay 3",
        "Late Decay 4",
        "Late Decay 5",
        "Late Decay 6",
        "Late Decay 7",
        "Late Decay 8",
        "Late Decay 9"
    };
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Editor)
};


} // namespace reverbobject_editor
} // namespace dawplugins
} // namespace visr_production_suite

#endif
