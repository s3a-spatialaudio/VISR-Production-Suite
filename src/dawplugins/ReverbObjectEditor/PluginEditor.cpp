#include "PluginEditor.h"
#include <libcore/PluginBinaryData.h>
#include <iterator> // for iterators

using namespace juce;
using namespace visr;

template <typename T>
std::vector<T> as_vector (boost::property_tree::ptree const& pt, boost::property_tree::ptree::key_type const& key)
{
    std::vector<T> r;
    for(auto& item : pt.get_child(key)){
        r.push_back(item.second.get_value<T>());
    }
    return r;
}

namespace visr_production_suite
{
namespace dawplugins
{
namespace reverbobject_editor
{
    
Editor::Editor (Processor* opan)
: PluginEditorObject(opan),
processor(*opan)
{
    
    plugtitle = new ImageButton ("ImageButton");
    juceImage2 = ImageCache::getFromMemory (PluginBinaryData::s3aPointSourceWithReverb_png, PluginBinaryData::s3aPointSourceWithReverb_pngSize);
    plugtitle->setImages (true, true, true,
                          juceImage2, 1.0f, Colours::transparentBlack,
                          juceImage2, 1.0f, Colours::transparentBlack,
                          juceImage2, 1.0f, Colours::transparentBlack,
                          0.5f);

    addAndMakeVisible(plugtitle);
    addRoomsToComboBox();
    bindEditor(*opan);
    setBoundariesOfUI();
    addControls();
    resized();
}

/*
 *
 */
Editor::~Editor(){}

void Editor::resized()
{
    plugtitle->setBoundsToFit(juce::Rectangle<int>(105, 12,500,25), Justification::centredLeft,false);
}

void Editor::enableRoomLoading()
{
    for (int i = 1; i <= room.getNumItems(); i++)
    {
        room.setItemEnabled(i, true);
    }
}

void Editor::setBoundariesOfUI()
{
    /* Header */
    //=====================================================================
    int header = 20;

    room.setJustificationType(juce::Justification::left);
    room.setColour(juce::ComboBox::ColourIds::backgroundColourId, juce::Colours::black);
    room.setColour(juce::ComboBox::ColourIds::outlineColourId, juce::Colours::darkgrey);
    room.setColour(juce::ComboBox::ColourIds::textColourId,juce::Colours::whitesmoke);
    room.setColour(juce::ComboBox::ColourIds::arrowColourId, juce::Colours::whitesmoke);
    room.setBounds(10, 50, 120, header);

    rsaoImportButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::black);
    rsaoImportButton.setButtonText("Import JSON");
    rsaoImportButton.setClickingTogglesState(true);
    rsaoImportButton.setBounds(10, 80, 120, header);
    rsaoImportButton.addListener(this);
    // the Import button is currently disabled
    // addAndMakeVisible(rsaoImportButton);
}
    
void Editor::addRoomsToComboBox(){
    try {
        // Given the array element "roomlibrary", optionally return boost::null.
        if(processor.getRoomIRLibraryTree().get_child_optional("roomlibrary"))
        {
            int counter = 1;
            // Given the JSON array roomlibrary, iterate through the "name" keys to identify their string names. This will fill the combo box UI.
            BOOST_FOREACH(const boost::property_tree::ptree::value_type& child, processor.getRoomIRLibraryTree().get_child("roomlibrary"))
            {
                std::string roomName = child.second.get<std::string>("name");
                // juce::ComboBox counter value cannot be 0. We must subtract this when comparing with boost iterations in libdawsupport.
                room.addItem(roomName, counter);
                std::cout<< "Room Editor UI Combo: Room Added: " << roomName << " at value: " << counter << std::endl;
                counter++;
            }
        } else {
            std::string errorMessage = std::string("There is an issue with syntax in the " + processor.getRoomIRLibraryFile().getFullPathName().toStdString() + ". Please check if this JSON is valid JSON, and the JSON is wrapped in a roomlibrary array." );
            throw boost::property_tree::ptree_error(errorMessage);
        }
    } catch( std::exception const &ex )
    {
        String errorMsg;
        errorMsg << "Exception in file load callback\n"<< ex.what();
        processor.parameters.state.getOrCreateChildWithName("Error",nullptr).setProperty("ErrorMsg", errorMsg, nullptr);
        std::cerr << errorMsg << std::endl;
        //            processor->parameters.state.getOrCreateChildWithName("Error",nullptr).removeProperty("ErrorMsg", nullptr);
    }
    catch (const boost::property_tree::ptree_error &ex)
    {
        String errorMsg;
        errorMsg << "Boost Exception in file load callback\n"<< ex.what();
        processor.parameters.state.getOrCreateChildWithName("Error",nullptr).setProperty("ErrorMsg", errorMsg, nullptr);
        std::cerr << errorMsg << std::endl;
    }
}
    
void Editor::openButtonClicked() {
    
    juce::FileChooser chooser ("Select a .json file to use", juce::File(), "*.json");
    
    if(chooser.browseForFileToOpen())
    {
        juce::File file (chooser.getResult());
        
        try{
            
            // Create an object instance that will only be used as a reference.
            std::unique_ptr<visr::objectmodel::PointSourceWithReverb> newPointSrcWithRev;
            newPointSrcWithRev.reset(new visr::objectmodel::PointSourceWithReverb(101));
            
            // Create new room tree property tree from the file chosen above.
            boost::property_tree::ptree newRoomTree;
            // Populate the new room into a the empty property tree json
            boost::property_tree::read_json(file.getFullPathName().toStdString(), newRoomTree);
            // Check the room parses for reference before adding, based upon the libobjectmodel's parse reverb object model.
            parse(newRoomTree, *newPointSrcWithRev);
            
            // If successful, append the current library room array with the new property tree.
            processor.getRoomIRLibraryTree().add_child(boost::property_tree::ptree::path_type("RSAORoom"), newRoomTree);
            //            currentJSONRoomTree.add_child("roomlibrary", newRoomTree);
            //            currentJSONRoomTree.push_back(std::make_pair("", newRoomTree));
            //            for(auto i: as_vector<boost::property_tree::ptree::path_type>(currentJSONRoomTree, "roomlibrary")){
            //                std::cout << i << std::endl;
            //            }
            
//            std::stringstream ss("{\"roomlibrary\": [{\"name\": \"this\" }, {\"name\": \"is\" }] }");
//            boost::property_tree::ptree pt;
//            boost::property_tree::read_json(ss, pt);
            
        } catch( std::exception const &ex )
        
        {
            String errorMsg;
            errorMsg << "Exception in file load callback\n"<< ex.what();
            processor.parameters.state.getOrCreateChildWithName("Error",nullptr).setProperty("ErrorMsg", errorMsg, nullptr);
            std::cerr << errorMsg << std::endl;
        }
        
    }
}

void Editor::parse(boost::property_tree::ptree const & tree, objectmodel::Object & src){
    using ptree = boost::property_tree::ptree;
    try
    {
        // note: cannot check for object type id since src might be a subclass of PointSource
        objectmodel::PointSourceWithReverb & reverbPointSrc = dynamic_cast<objectmodel::PointSourceWithReverb&>(src);
        
        // Parse all members inherited from the base class PointSource
        //        objectmodel::PointSourceParser::parse( tree, reverbPointSrc );
        
        // Parse data members specific to PointSourceWithReverb
        if( tree.count( "room" ) != 1 )
        {
            throw std::invalid_argument( "Object must contain exactly one \"room\" element." );
        }
        ptree const & roomTree = tree.get_child( "room" );
        
        if( roomTree.count("ereflect") == 0 )
        {
            reverbPointSrc.setNumberOfDiscreteReflections( 0 );
        }
        else
        {
            ptree const & ereflTree = roomTree.get_child( "ereflect" );
            std::size_t const numEarlyReflections = ereflTree.count( "" );
            reverbPointSrc.setNumberOfDiscreteReflections( numEarlyReflections );
            auto const earlyNodes = ereflTree.equal_range( "" );
            std::size_t earlyIndex{ 0 };
            for( ptree::const_assoc_iterator treeIt( earlyNodes.first ); treeIt != earlyNodes.second; ++treeIt, ++earlyIndex )
            {
                objectmodel::PointSourceWithReverb::DiscreteReflection & refl = reverbPointSrc.discreteReflection( earlyIndex );
                ptree const earlyTree = treeIt->second;
                objectmodel::PointSourceWithReverb::Coordinate posX, posY, posZ;
                // changed compared to PointSourceWithReverbParser::parse : we don't need to parse the coordinates, that are already there in the object
                posX = reverbPointSrc.x();
                posY = reverbPointSrc.y();
                posZ = reverbPointSrc.z();
                
                
                refl.setPosition( posX, posY, posZ );
                refl.setDelay( earlyTree.get<SampleType>( "delay" ) );
                refl.setLevel( earlyTree.get<objectmodel::LevelType>( "level" ) );
                
                ptree const & biquadTree = earlyTree.get_child( "biquadsos" );
                rbbl::BiquadCoefficientList<SampleType> biqList;
                biqList.loadJson( biquadTree );
                if( biqList.size() > objectmodel::PointSourceWithReverb::cNumDiscreteReflectionBiquads )
                {
                    throw std::invalid_argument( "PointSourceWithReverbParser: The number of biquad sections for an early reflection exceeds the maximum admissible value." );
                }
                refl.setReflectionFilters( biqList );
            }
        }
        
        if( roomTree.count( "lreverb" ) != 1 )
        {
            throw std::invalid_argument( "Object must contain exactly one \"room.lreverb\" element." );
        }
        ptree const & lateTree = roomTree.get_child( "lreverb" );
        reverbPointSrc.setLateReverbOnset( lateTree.get<SampleType>( "delay" ) );
        std::string const lateLevelString = lateTree.get<std::string>( "level" );
        rbbl::FloatSequence<SampleType> lateLevels( lateLevelString );
        if( lateLevels.size() != objectmodel::PointSourceWithReverb::cNumberOfSubBands )
        {
            throw std::invalid_argument( "The number of elements in the \"lreverb.late\" attribute must match the fixed number of subbands." );
        }
        reverbPointSrc.setLateReverbLevels( lateLevels.values( ), lateLevels.size( ) );
        
        std::string const lateAttacksString = lateTree.get<std::string>( "attacktime" );
        rbbl::FloatSequence<SampleType> const lateAttacks( lateAttacksString );
        if( lateAttacks.size( ) != objectmodel::PointSourceWithReverb::cNumberOfSubBands )
        {
            throw std::invalid_argument( "The number of elements in the \"lreverb.late\" attribute must match the fixed number of subbands." );
        }
        reverbPointSrc.setLateReverbAttackTimes(lateAttacks.values( ), lateAttacks.size( ) );
        
        std::string const lateDecaysString = lateTree.get<std::string>( "decayconst" );
        rbbl::FloatSequence<SampleType> const lateDecays( lateDecaysString );
        if( lateDecays.size( ) != objectmodel::PointSourceWithReverb::cNumberOfSubBands )
        {
            throw std::invalid_argument( "The number of elements in the \"lreverb.late\" attribute must match the fixed number of subbands." );
        }
        reverbPointSrc.setLateReverbDecayCoeffs( lateDecays.values( ), lateDecays.size( ) );
    }
    // TODO: distinguish between boost property_tree parse errors and bad dynamic casts.
    catch( std::exception const & ex )
    {
        throw std::invalid_argument( std::string( "Error while parsing point source with reverb: object: ") + ex.what() );
    }
}



void Editor::addControls()
{
    addAndMakeVisible(room);
}

void Editor::bindEditor(Processor& reverbProcessor)
{
    using APVTS = juce::AudioProcessorValueTreeState;
    combos.add(new APVTS::ComboBoxAttachment (reverbProcessor.parameters, core::IDs::loadRoom.toString(), room));
}

juce::Component* Editor::getComponentUnderTheMouse() const
{
    if (auto* component = juce::Desktop::getInstance().getMainMouseSource().getComponentUnderMouse())
    {
        //This is needed because combo boxes are composed of multiple sub components...
        auto componentID = component->getComponentID();
        while(componentID.isEmpty() && component != nullptr)
        {
            component = component->getParentComponent();
            if(component != nullptr)
                componentID = component->getComponentID();
        }
        return component;
    }
    return nullptr;
}

juce::String Editor::getParameterKeyForComponentUnderTheMouse() const
{
    if(auto* component = getComponentUnderTheMouse())
    {
        return component->getComponentID();
    }
    return{};
}

void Editor::buttonClicked(juce::Button* button )
{
    if(button == &rsaoImportButton)
    {
        openButtonClicked();
    }
}


void Editor::createJSONFromRIR()
{
    
    juce::String filePath = File::getSpecialLocation(juce::File::SpecialLocationType::userDesktopDirectory).getFullPathName();
    
    filePath.append("/Encoder_RSAO_Python/", 35);
    
    //   "&& /usr/local/bin/python3 main.py";
    
    std::string encoderPath = "cd " + filePath.toStdString() +  " && /usr/local/bin/python3 main.py";
    
    int i;
    printf ("Checking if processor is available...");
    if (system(NULL)) puts ("Ok");
    else exit (EXIT_FAILURE);
    printf ("Executing command DIR...\n");
    i=system (encoderPath.c_str());
    //if python interpreter has shut down then check for file?
}

constexpr const char* Editor::parameterKeys[numberOfParameters];
constexpr const char* Editor::parameterNames[numberOfParameters];
constexpr const char* Editor::parameterDescriptions[numberOfParameters];
    
} // namespace reverbobject_editor
} // namespace dawplugins
} // namespace visr_production_suite
