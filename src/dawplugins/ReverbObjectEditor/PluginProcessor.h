#ifndef REVERBOBJECTEDITOR_PROCESSOR_H_INCLUDED
#define REVERBOBJECTEDITOR_PROCESSOR_H_INCLUDED

#include <libcore/processors/PluginProcessorBase.h>
#include <libvisrsupport/VISRPointSourceRevWrapper.h>
#include <libcore/IDs.h>


namespace visr_production_suite
{
namespace dawplugins
{
namespace reverbobject_editor
{

/**
 * Main Processor class for the ReverbObjectEditor VST3. Uses a forward declaration of the PannerUI, and inherits functionality from the PluginProcessorBase.
 */
class Processor : public core::processors::PluginProcessorBase
{
public:
    /**
     * Constructor
     * Get RoomIRs.json file in order to detect the number of rooms available.
     */
    Processor();

    /**
     * Destructor.
     */
    ~Processor();

    /**
     * Get name of VST3 plugin.
     * @return [description]
     */
    const juce::String getName() const override {return "VISR Reverb Object Editor";};

    /**
     * JUCE's standard function for creating the editor.
     * @return [description]
     */
    juce::AudioProcessorEditor * createEditor() override;

    /**
     * Setup VISR objectHandler parameters and add metadata. This points to the VISRPointSourceRevWrapper.
     */
    void PluginSetup() override;

    /**
     * On plugin deletion, remove parameter listeners.
     */
    void PluginUnload() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    void prepareToPlay (double sampleRate, int period) override{};

    /**
     * Getter function for RoomIRs JSON file.
     * @return roomIRLibraryFile
     */
    juce::File getRoomIRLibraryFile(){
        return roomIRLibraryFile;
    }
    
    /**
     * Getter function for the roomIRLibraryTree
     * @return roomIRLibraryTree
     */
    boost::property_tree::ptree & getRoomIRLibraryTree(){
        return roomIRLibraryTree;
    }
private:
    
    /**
     * Create parameters for the ReverbObjectEditor.
     * This includes detection of the number of rooms present in the RoomIRs, and to determine the number of rooms available, and set the juce::NormalisableRange, necessary for createAndAddParameter(IDs::loadRoom ...) function
     */
    void constructParameters();
    
    std::unique_ptr<visr_support::VISRPointSourceRevWrapper> objectHandler;
    
    /**
     * RoomIRs JSON file.
     * @return [description]
     */
    juce::File roomIRLibraryFile;

    /**
     * Property Tree that is populated by the RoomIRs JSON file.
     * @return [description]
     */
    boost::property_tree::ptree roomIRLibraryTree;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Processor)

};

} // namespace reverbobject_editor
} // namespace dawplugins
} // namespace visr_production_suite

#endif
