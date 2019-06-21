#include "VISRPointSourceRevWrapper.h"
#include <libefl/cartesian_spherical_conversion.hpp>
#include <libefl/degree_radian_conversion.hpp>
#include <libcore/IDs.h>
#include <limits>
#include <istream>

namespace visr_production_suite
{
namespace visr_support
{

using namespace visr;
using namespace juce;
using namespace core;

VISRPointSourceRevWrapper::VISRPointSourceRevWrapper(AudioProcessorValueTreeState& param, boost::property_tree::ptree& roomIRTree):
VISRObjectWrapper(param,visr::objectmodel::ObjectTypeId::PointSourceWithReverb),
roomIRLibraryTree(roomIRTree)
{
    float* objn = dataModel.getRawParameterValue(IDs::objID);
    
    pointSrcWithRev.reset(new visr::objectmodel::PointSourceWithReverb(*objn -1));
    
    if (pointSrcWithRev)
    {
        if(!UDPReceiver::bind(*objn+60000)){
            String errorMsg;
            errorMsg << "Binding to receiving Port failed, your PointSourceReverb ID should be unique\n";
            dataModel.state.getOrCreateChildWithName("Error",nullptr).setProperty("ErrorMsg", errorMsg, nullptr);
            std::cerr << errorMsg << std::endl;
        }else {
            dataModel.state.getOrCreateChildWithName("Error",nullptr).removeAllProperties(nullptr);
        }
        
        handleRoomChangeRequest(*pointSrcWithRev);
        
        pointSrcWithRev->resetNumberOfChannels(1);
        pointSrcWithRev->setChannelIndex(0,pointSrcWithRev->id());
        phi = efl::degree2radian(*param.getRawParameterValue(IDs::azimuth));
        theta = efl::degree2radian(*param.getRawParameterValue(IDs::elevation));
        radius = *param.getRawParameterValue(IDs::radius);
        
        objectmodel::Object::Coordinate xPos, yPos, zPos;
        std::tie( xPos, yPos, zPos ) = efl::spherical2cartesian( phi, theta, radius );
        
        pointSrcWithRev->setX(xPos);
        pointSrcWithRev->setY(yPos);
        pointSrcWithRev->setZ(zPos);
        pointSrcWithRev->setLevel(0.5);
        pointSrcWithRev->setGroupId(5);
        pointSrcWithRev->setPriority(5);
        
        {
            std::lock_guard<std::mutex> guard{ objWrapLock };
            objWrap.insert( *pointSrcWithRev );
        }
    }
}

VISRPointSourceRevWrapper::~VISRPointSourceRevWrapper(){
    UDPReceiver::unbind();
}
void VISRPointSourceRevWrapper::VISRObjectChanged(const String& pID, float newVal){
    
    if( pointSrcWithRev )
    {
        if(pID==IDs::azimuth.toString()){
            phi = efl::degree2radian( newVal);
            objectmodel::Object::Coordinate xPos, yPos, zPos;
            std::tie( xPos, yPos, zPos ) = efl::spherical2cartesian( phi, theta, radius );
            
            pointSrcWithRev->setX(xPos);
            pointSrcWithRev->setY(yPos);
            pointSrcWithRev->setZ(zPos);
        }
        
        else if(pID==IDs::elevation.toString()){
            theta = efl::degree2radian( newVal);
            objectmodel::Object::Coordinate xPos, yPos, zPos;
            std::tie( xPos, yPos, zPos ) = efl::spherical2cartesian( phi, theta, radius );
            
            pointSrcWithRev->setX(xPos);
            pointSrcWithRev->setY(yPos);
            pointSrcWithRev->setZ(zPos);
        }
        
        else if(pID==IDs::radius.toString()){
            radius = newVal;
            objectmodel::Object::Coordinate xPos, yPos, zPos;
            std::tie( xPos, yPos, zPos ) = efl::spherical2cartesian( phi, theta, radius );
            
            pointSrcWithRev->setX(xPos);
            pointSrcWithRev->setY(yPos);
            pointSrcWithRev->setZ(zPos);
        }
        else if(pID==IDs::objID.toString()){
            if(!UDPReceiver::bind(60000+newVal)){
                String errorMsg;
                errorMsg << "Binding to receiving Port failed, your PointSource ID should be unique\n";
                dataModel.state.getOrCreateChildWithName("Error",nullptr).setProperty("ErrorMsg", errorMsg, nullptr);
                std::cerr << errorMsg << std::endl;
            }else {
                dataModel.state.getOrCreateChildWithName("Error",nullptr).removeAllProperties(nullptr);
            }
            int newIDandChan = newVal-1;
            
            
            pointSrcWithRev.reset(new visr::objectmodel::PointSourceWithReverb(newIDandChan));
            
            pointSrcWithRev->resetNumberOfChannels(1);
            pointSrcWithRev->setChannelIndex(0,newIDandChan);
            
            pointSrcWithRev->setLevel(0.5);
            pointSrcWithRev->setGroupId(5);
            pointSrcWithRev->setPriority(5);
            
            
            objectmodel::Object::Coordinate xPos, yPos, zPos;
            std::tie( xPos, yPos, zPos ) = efl::spherical2cartesian( phi, theta, radius );
            
            pointSrcWithRev->setX(xPos);
            pointSrcWithRev->setY(yPos);
            pointSrcWithRev->setZ(zPos);
            
            handleRoomChangeRequest(*pointSrcWithRev);
            //            pointSrcWithRev->setNumberOfDiscreteReflections(6);
        }
        else if(pID==IDs::loadRoom.toString())
        {
            handleRoomChangeRequest(*pointSrcWithRev);
        }
        
        {
            std::lock_guard<std::mutex> guard{ objWrapLock };
            objWrap.clear();
            objWrap.insert( *pointSrcWithRev );
        }
    }
}


void VISRPointSourceRevWrapper::VISRObjectReceived (visr::objectmodel::Object& obj)
{
    visr::objectmodel::PointSourceWithReverb * mPswr = dynamic_cast< visr::objectmodel::PointSourceWithReverb *>(&(obj));
    
    float x,y,z;
    {
        std::lock_guard<std::mutex> guard{ objWrapLock };
        x = mPswr->x();
        y = mPswr->y();
        z = mPswr->z();
    }
    
    objectmodel::Object::Coordinate az_, el_, radius_;
    std::tie( az_, el_, radius_ ) = efl::cartesian2spherical(x, y, z);
    
    //        printf("%i CARTES az %f el %f r %f\n",mPswr->id(),az,el,radius);
    //                float normPar = dataModel.getParameterRange(IDs::objID).convertTo0to1(((int)mPs->id()+1));
    //                dataModel.getParameter(IDs::objID)->setValueNotifyingHost(normPar);
    float normPar = dataModel.getParameterRange(IDs::azimuth).convertTo0to1(std::round(efl::radian2degree(az_)));
    dataModel.getParameter(IDs::azimuth)->setValueNotifyingHost( normPar );
    
    normPar = dataModel.getParameterRange(IDs::elevation).convertTo0to1(std::round(efl::radian2degree(el_)));
    dataModel.getParameter(IDs::elevation)->setValueNotifyingHost( normPar );
    
    normPar = dataModel.getParameterRange(IDs::radius).convertTo0to1(radius_);
    dataModel.getParameter(IDs::radius)->setValueNotifyingHost(normPar);
    
}

void VISRPointSourceRevWrapper::parse(boost::property_tree::ptree const & tree, objectmodel::Object & src){
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

// This is unused now, the handleRoomChangeRequest(...) function is now responsible for this functionality. TODO: Remove.
void VISRPointSourceRevWrapper::loadRoom( boost::property_tree::ptree const & tree, visr::objectmodel::Object & src, std::string roomName )
{
    try {
        
        if(boost::optional<const boost::property_tree::ptree&> child = tree.get_child("roomlibrary"))
        {
            int counter = 0;
            // Current state of the Room number
            int roomNumber = *dataModel.getRawParameterValue(IDs::loadRoom) - 1;
            
            BOOST_FOREACH(const boost::property_tree::ptree::value_type& child, tree.get_child("roomlibrary"))
            {
                
                if (counter == roomNumber){
                    std::cout<< "Room Choosen and Located: " << child.second.get<std::string>("name") << std::endl;
                    
                    parse(child.second.get_child("value"), src);
                    counter = roomNumber;
                    
                    break;
                }
                counter++;
                // Pass this to find the child value of the room based on the iteration number, so if it is 0, then parse the first valued iteration.
            }
            // For parsing single room objects - legacy now.
            //    else if {
            //        parser.parse(tree, src);
            //    }
            
        } else {
            std::string errorMessage = std::string("There is an issue with syntax in the ReverbIR File . Please check if this JSON is valid JSON, and the JSON is wrapped in a roomlibrary [] array." );
            throw std::runtime_error(errorMessage);
        }
        
    } catch( std::exception const &ex )
    
    {
        String errorMsg;
        errorMsg << "Exception in file load callback\n"<< ex.what();
        dataModel.state.getOrCreateChildWithName("Error",nullptr).setProperty("ErrorMsg", errorMsg, nullptr);
        std::cerr << errorMsg << std::endl;
    }
    
    
}

void VISRPointSourceRevWrapper::handleRoomChangeRequest(visr::objectmodel::PointSourceWithReverb & src)
{
    try {
        int roomNum = *dataModel.getRawParameterValue(IDs::loadRoom);
        
        // std::cout<< "VISRPointSourceRevWrapper: Room Editor Number Retrieved: " << roomNum << std::endl;
        int counter = 1;
        // Default string name of first room of first combo box. This should change according to the BOOST_FOREACH below.
        std::string roomName = "emm_main";
        
        if(boost::optional<const boost::property_tree::ptree&> child = roomIRLibraryTree.get_child("roomlibrary"))
        {
            // Get the current string value of specified room from chosen UI combo box from reverb Plugin Editor
            BOOST_FOREACH(const boost::property_tree::ptree::value_type& child, roomIRLibraryTree.get_child("roomlibrary"))
            {
                if ( counter == (roomNum) ){
                    roomName = child.second.get<std::string>("name") ;
                    // Parse room with object.
                    // std::cout<< "VISRPointSourceRevWrapper: Room Choosen and Located: " << roomName << std::endl;
                    parse(child.second.get_child("value"), src);
                    src.setNumberOfDiscreteReflections(6);
                    break;
                }
                counter++;
            }
        }
    } catch( std::exception const &ex )
    {
        String errorMsg;
        errorMsg << "Exception in file load callback\n"<< ex.what();
        dataModel.state.getOrCreateChildWithName("Error",nullptr).setProperty("ErrorMsg", errorMsg, nullptr);
        std::cerr << errorMsg << std::endl;
    }
}
} // namespace visr_support
} // namespace visr_production_suite
