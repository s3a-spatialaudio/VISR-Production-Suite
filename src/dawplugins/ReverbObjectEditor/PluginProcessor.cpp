#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <libpml/initialise_parameter_library.hpp>
#include "CurrentDefaultPaths.h"

using namespace visr;
using namespace juce;

// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
return new visr_production_suite::dawplugins::reverbobject_editor::Processor();
}

namespace visr_production_suite
{
namespace dawplugins
{
namespace reverbobject_editor
{

using namespace visr;
using namespace juce;
using namespace core;

Processor::Processor()
:PluginProcessorBase(BusesProperties()
                     .withInput ("Input",  AudioChannelSet::mono(), true)
                     .withOutput ("Output", AudioChannelSet::mono(), true))
{
    roomIRLibraryFile = (core::defaultSrcPath+"/resources/RoomIRs.json");
    try{
        
        if(roomIRLibraryFile.exists()){
            // Populate property tree from file, if exists.
            boost::property_tree::read_json(getRoomIRLibraryFile().getFullPathName().toStdString(), getRoomIRLibraryTree());
        }else{
            roomIRLibraryFile = (core::defaultInstallPath+"/resources/RoomIRs.json");
            if(roomIRLibraryFile.exists()){
                // Populate property tree from file, if exists.
                boost::property_tree::read_json(getRoomIRLibraryFile().getFullPathName().toStdString(), getRoomIRLibraryTree());
            } else {
                std::string errorMessage = std::string("There was an error loading the current Room Library. Check it exists at the location: " + getRoomIRLibraryFile().getFullPathName().toStdString() );
                throw std::runtime_error(errorMessage);
            }
        }
        
    } catch( std::runtime_error const &ex )
    {
        String errorMsg;
        errorMsg << "Exception in file load callback\n"<< ex.what();
        parameters.state.getOrCreateChildWithName("Error",nullptr).setProperty("ErrorMsg", errorMsg, nullptr);
        std::cerr << errorMsg << std::endl;
    }
    
    constructParameters();
    PluginReset();
    isFirst=false;
}

Processor::~Processor() {}


void Processor::constructParameters()
{
    //objectID
    parameters.createAndAddParameter (std::make_unique<juce::AudioProcessorValueTreeState::Parameter>
                                      (IDs::objID.toString(), IDs::objID.toString(), IDs::objID.toString(),
                                       NormalisableRange<float> (1,64,1), 1, nullptr,nullptr,false,false,true));
    //azimuth angle
    parameters.createAndAddParameter (std::make_unique<juce::AudioProcessorValueTreeState::Parameter>
                                      (IDs::azimuth.toString(), IDs::azimuth.toString(), String(),
                                       NormalisableRange<float> (-180, 180,1), 0, nullptr,nullptr));
    //elevation angle
    parameters.createAndAddParameter (std::make_unique<juce::AudioProcessorValueTreeState::Parameter>
                                      (IDs::elevation.toString(), IDs::elevation.toString(), String(),
                                       NormalisableRange<float> (-90, 90,1), 0, nullptr,nullptr));
    //radius
    parameters.createAndAddParameter ( std::make_unique<juce::AudioProcessorValueTreeState::Parameter>
                                      (IDs::radius.toString(), IDs::radius.toString(), String(),
                                       NormalisableRange<float> (0.01f, 100, 0.01f), 1.f, nullptr,nullptr));
    
    
    int numberOfRooms = 0;
    
    try {
        // Given the array element "roomlibrary", optionally return boost::null.
        if(getRoomIRLibraryTree().get_child_optional("roomlibrary"))
        {
            // Given the JSON array roomlibrary, iterate through the "name" keys to identify their string names. This will fill the combo box UI.
            BOOST_FOREACH(const boost::property_tree::ptree::value_type& child, getRoomIRLibraryTree().get_child("roomlibrary"))
            {
                numberOfRooms++;
            }
        } else {
            std::string errorMessage = std::string("There is an issue with syntax in the " + getRoomIRLibraryFile().getFullPathName().toStdString() + ". Please check if this JSON is valid JSON, and the JSON is wrapped in a roomlibrary array." );
            throw boost::property_tree::ptree_error(errorMessage);
        }
    } catch( std::exception const &ex )
    {
        String errorMsg;
        errorMsg << "Exception in file load callback\n"<< ex.what();
        parameters.state.getOrCreateChildWithName("Error",nullptr).setProperty("ErrorMsg", errorMsg, nullptr);
        std::cerr << errorMsg << std::endl;
        //            processor->parameters.state.getOrCreateChildWithName("Error",nullptr).removeProperty("ErrorMsg", nullptr);
    }
    
    parameters.createAndAddParameter(std::make_unique<juce::AudioProcessorValueTreeState::Parameter> (IDs::loadRoom.toString(),
                                                                                                      IDs::loadRoom.toString(),
                                                                                                      String(),
                                                                                                      NormalisableRange<float> (1.f, ((float)numberOfRooms), 1.f),
                                                                                                      1.f,
                                                                                                      nullptr,
                                                                                                      nullptr));
    
    parameters.state = ValueTree(Identifier ("ReverbObjectEditor"));
    
}

bool Processor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    if(layouts.getMainOutputChannelSet() != AudioChannelSet::mono() || layouts.getMainInputChannelSet() != AudioChannelSet::mono())
        return false;
    return true;
}

void Processor::PluginSetup()
{
    visr::pml::initialiseParameterLibrary();
    objectHandler.reset(new visr_support::VISRPointSourceRevWrapper(parameters, roomIRLibraryTree));
    
    parameters.addParameterListener(IDs::objID.getCharPointer(), objectHandler.get());
    parameters.addParameterListener(IDs::azimuth.toString(), objectHandler.get());
    parameters.addParameterListener(IDs::elevation.toString(), objectHandler.get());
    parameters.addParameterListener(IDs::radius.toString(), objectHandler.get());
    parameters.addParameterListener(IDs::loadRoom.toString(), objectHandler.get());
    parameters.addParameterListener(IDs::loadJson.toString(), objectHandler.get());
}

void Processor::PluginUnload()
{
    /**
     * For each objectHandler->addMetadata there should be a corresponding removeParameterListener.
     */
    if(objectHandler!=nullptr){
        parameters.removeParameterListener(IDs::objID.getCharPointer(), objectHandler.get());
        parameters.removeParameterListener(IDs::azimuth.toString(), objectHandler.get());
        parameters.removeParameterListener(IDs::elevation.toString(), objectHandler.get());
        parameters.removeParameterListener(IDs::radius.toString(), objectHandler.get());
        parameters.removeParameterListener(IDs::loadRoom.toString(), objectHandler.get());
        objectHandler.reset();
    }
}


AudioProcessorEditor* Processor::createEditor()
{
    return new Editor (this);
}

} // namespace reverbobject_editor
} // namespace dawplugins
} // namespace visr_production_suite
