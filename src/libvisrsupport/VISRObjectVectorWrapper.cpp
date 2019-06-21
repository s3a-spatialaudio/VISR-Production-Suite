#include "VISRObjectVectorWrapper.h"
#include <libobjectmodel/object_vector_parser.hpp>
#include <libobjectmodel/object_factory.hpp>
#include <libobjectmodel/object_type.hpp>
#include <libobjectmodel/object.hpp>
#include <libobjectmodel/point_source.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace visr;
using namespace objectmodel;
using namespace juce;

namespace visr_production_suite
{
namespace visr_support
{


VISRObjectVectorWrapper::VISRObjectVectorWrapper(ValueTree state, std::vector < VISRObjectVectorObserver * >& observers, int portIn, const std::string newTargetHost, const int newTargetPort)
:
UDPReceiver("Object Vector Wrapper UDP receiver (Scene Master)"),
sceneObservers(observers)
{
    scene.clear();
    
    UDPSender::bind(newTargetHost,newTargetPort);
    
    if(!UDPReceiver::bind(portIn)){
        String errorMsg;
        errorMsg << "Binding to receiving Port failed, this usually happens when you have multiple instances of this plugin\n";
        state.getOrCreateChildWithName("Error",nullptr).setProperty("ErrorMsg", errorMsg, nullptr);
        std::cerr << errorMsg << std::endl;
    }else{
        state.getOrCreateChildWithName("Error",nullptr).removeAllProperties(nullptr);
    }
    startTimer(50);
}


VISRObjectVectorWrapper::~VISRObjectVectorWrapper(){
    stopTimer();
    UDPSender::unbind();
    UDPReceiver::unbind();
}

void VISRObjectVectorWrapper::timerCallback (){
    ObjectVector clone;
    {
        std::lock_guard<std::mutex> guard{ mSceneLock };
        
        // removes the inactive objects based on a timestamp
        clone.assign(scene);
        for( ObjectVector::const_iterator runIt( clone.cbegin()); runIt != clone.cend(); ++runIt )
        {
            Object & obj( *runIt );
            
            int objid = (int)obj.id();
            if ( expiryTimes[objid] < Time::getMillisecondCounter()){
                //                    std::cout<<"rown timercallbck: "<<objid<<std::endl;
                scene.remove(objid);
            }
        }
        
        // serialises the obect vector and sends it to the renderer
        createMessage();
        
        // sends an update trigger to all the object vector observers
        for (int i = 0; i < sceneObservers.size(); i++) sceneObservers[i]->update();
    }
}
ObjectVector VISRObjectVectorWrapper::getScene(){
    ObjectVector clone;
    try{
        //  this mutex causes nasty redrawing and stop of the processing. On the other hand clone.assign(scene) could fail if the scene is being used
        //        {
        //            std::lock_guard<std::mutex> guard{ mSceneLock };
        clone.assign(scene);
        //        }
    }catch( std::exception const & ex )
    {
        printf("ERROR\n");
        std::cerr << "Component Wrapper with Object Vector handling: Error while retrieving the scene " << ex.what() << std::endl;
    }
    return clone;
};


void VISRObjectVectorWrapper::createMessage(){
    std::ostringstream jsonStream;
    ObjectVectorParser::encodeObjectVector( scene, jsonStream );
    
    std::string s = jsonStream.str();
    // In contrast to writeString(), this one does not append the string terminator \0.
    if (!s.empty() && s[s.length()-1] == '\n' ) {
        s.erase(s.length()-1);
        //        s.append("\0");
    }
    UDPSender::UDPsend(s);
}

void VISRObjectVectorWrapper::handleMessage (const char* data, size_t dataSize)
{
    // handle the one-object messages sent from the object editors
    MemoryInputStream inStream (data, dataSize,false);
    try
    {
        std::string newMsg = inStream.readEntireStreamAsString().toStdString();
        std::istringstream streamObj( newMsg );
        updateObjectVectorAndTimestamp(streamObj);
//        std::cout<< "Handled Message: Object Received in SceneMaster: \n\n" << newMsg << std::endl;

    }
    catch( std::exception const & ex )
    {
        printf("ERROR\n");
        std::cerr << "Error while decoding a scene metadata message: " << ex.what() << std::endl;
    }
}

void VISRObjectVectorWrapper::updateObjectVectorAndTimestamp ( std::basic_istream<char> & message){
    using ptree = boost::property_tree::ptree;
    ptree propTree;
    try
    {
        read_json( message, propTree );
    }
    catch( std::exception const & ex )
    {
        throw std::invalid_argument( std::string( "Error while parsing a json object message: " ) + ex.what() );
    }

    for( auto v: propTree.get_child( "objects") )
    {
        try
        {
            ObjectId const objId = v.second.get<ObjectId>( "id" );
            
            // properties common to all source types
            std::string const objTypeStr = v.second.get<std::string>( "type" );
            
            // might throw (if the string does not match a recognised object type
            ObjectTypeId const objTypeId = stringToObjectType( objTypeStr );
            
            // Create a parser for this type (used later)
            ObjectParser const & objParser = ObjectFactory::parser( objTypeId );
            
            // This contains some redundancy with ObjectParser::parse() called later,
            // but this is probably unavoidable.
            ObjectId newId = v.second.get<ObjectId>( "id" );
            
            {
                std::lock_guard<std::mutex> guard{ mSceneLock };
                
                ObjectVector::iterator findIt = scene.find( newId );
                bool const found = findIt != scene.end();
                
                // This implementation requires the construction of a new object even if a matching object is found, but provides exception
                // safety, i.e., resets the original state of the object to be by modified, which is thus unchanged if the parser throws
                std::unique_ptr< Object > newObj( (found && (findIt->type() == objTypeId))
                                                 ? findIt->clone()
                                                 : ObjectFactory::create(objTypeId, objId ) );
                
                objParser.parse( v.second, *newObj );
                scene.insert( std::move(newObj) );
            }
            expiryTimes[objId] = Time::getMillisecondCounter() + aliveObjectsTimeWindow;
        }
        catch( std::exception const & ex )
        {
            throw std::invalid_argument( std::string( "Error while parsing the json message content: " ) + ex.what( ) );
        }
    }
}

} // namespace visr_support
} // namespace visr_production_suite

