#include "VISRObjectWrapper.h"
#include <libobjectmodel/object_vector_parser.hpp>
#include <libcore/IDs.h>
#include <sstream>

using namespace visr;
using namespace juce;

namespace visr_production_suite
{
namespace visr_support
{

VISRObjectWrapper::VISRObjectWrapper(AudioProcessorValueTreeState& model, visr::objectmodel::ObjectTypeId objtype):
UDPReceiver("ObjectWrapper UDP receiver"),
dataModel(model)
{
    {
        std::lock_guard<std::mutex> guard{ objWrapLock };
        objWrap.clear();
    }
    UDPSender::bind("127.0.0.1",60000);
    startTimer (100);
}

VISRObjectWrapper::~VISRObjectWrapper(){
    stopTimer();
    UDPSender::unbind();
}

void VISRObjectWrapper::timerCallback ()
{
    createMessage();
}

void VISRObjectWrapper::parameterChanged (const String& pID, float newVal){
    VISRObjectChanged(pID,newVal);
//    createMessage();
}

void VISRObjectWrapper::createMessage(){
    std::ostringstream jsonStream;
    {
        std::lock_guard<std::mutex> guard{ objWrapLock };
        objectmodel::ObjectVectorParser::encodeObjectVector( objWrap, jsonStream );
//        std::cout<< "Created Message: Object Sent from VISRObjectWrapper: \n\n" << jsonStream.str().substr(0,100) << std::endl;
    }
    
    std::string s = jsonStream.str();
    UDPSender::UDPsend(s);
}

void VISRObjectWrapper::handleMessage (const char* data, size_t dataSize)
{
    MemoryInputStream inStream (data, dataSize,false);
    try
    {
        std::string newMsg = inStream.readEntireStreamAsString().toStdString();
        {
            std::lock_guard<std::mutex> guard{ objWrapLock };
            objectmodel::ObjectVectorParser::updateObjectVector( newMsg, objWrap );
        }
    }
    catch( std::exception const & ex )
    {
        printf("ERROR\n");
        std::cerr << "Error while decoding a scene metadata message: " << ex.what() << std::endl;
    }
    
    try
    {
        int objid = *dataModel.getRawParameterValue(core::IDs::objID);
        visr::objectmodel::Object& obj = objWrap.at(objid-1);
        VISRObjectReceived(obj);
    }
    catch( std::exception const & ex )
    {
        // Don't abort the program when receiving a corrupted message.
        std::cerr << "Error while retrieving objects from scene " << ex.what() << std::endl;
    }
}

} // namespace visr_support
} // namespace visr_production_suite
