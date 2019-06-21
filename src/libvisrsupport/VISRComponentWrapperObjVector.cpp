#include "VISRComponentWrapperObjVector.h"
#include <libobjectmodel/object_vector_parser.hpp>
#include <libefl/cartesian_spherical_conversion.hpp>
#include <libefl/degree_radian_conversion.hpp>
#include <libobjectmodel/point_source.hpp>
#include <libobjectmodel/point_source_extent.hpp>
#include <libobjectmodel/point_source_with_reverb.hpp>
#include <libcore/IDs.h>
#include <libpml/listener_position.hpp>

using namespace visr;
using namespace juce;

namespace visr_production_suite
{
namespace visr_support
{
VISRComponentWrapperObjVector::VISRComponentWrapperObjVector(AudioProcessorValueTreeState& dataModel, visr::CompositeComponent& tlcomp, std::vector < VISRObjectVectorObserver * >& observers, int port, int dynamicHeadTrackingSelect, bool objvAfterProcessing):
VISRComponentWrapper(tlcomp),
UDPReceiver("Component Wrapper with Object Vector UDP receiver"),
sceneObservers(observers),
afterProcessing(objvAfterProcessing)
{
    scene.clear();
    String errorMsg;
    if(!UDPReceiver::bind(port)){
        errorMsg << "Binding to receiving Port failed, this usually happens when you have multiple instances of this plugin\n";
        dataModel.state.getOrCreateChildWithName("Error",nullptr).setProperty("ErrorMsg", errorMsg, nullptr);
        std::cerr << errorMsg << std::endl;
    }else{
        //Temporary: if you have another error somewhere, without this if() you'll lose it
        if(errorMsg == "Binding to receiving Port failed, this usually happens when you have multiple instances of this plugin\n")
            dataModel.state.getOrCreateChildWithName("Error",nullptr).removeAllProperties(nullptr);
    }
    if(dynamicHeadTrackingSelect){
        listener.reset(new VISRListenerPosWrapper(dataModel, *audioFlow.get(),dynamicHeadTrackingSelect));
    }
}

VISRComponentWrapperObjVector::~VISRComponentWrapperObjVector(){
    UDPReceiver::unbind();
}

visr::objectmodel::ObjectVector VISRComponentWrapperObjVector::getScene(){
    visr::objectmodel::ObjectVector clone;
    try{
        if(afterProcessing){
            const visr::objectmodel::ObjectVector * objv;
            visr::pml::DoubleBufferingProtocol::InputBase * mObjectVector = dynamic_cast<visr::pml::DoubleBufferingProtocol::InputBase *>(&audioFlow->externalParameterSendPort("objectVectorOutput"));
            if(mObjectVector!=nullptr){
                objv = dynamic_cast<const visr::objectmodel::ObjectVector *>(&(mObjectVector->data()));
                if(objv!=nullptr)
                    scene = *objv;
            }
        }
        clone.assign(scene);
    }catch( std::exception const & ex )
    {
        printf("ERROR\n");
        std::cerr << "Component Wrapper with Object Vector handling: Error while retrieving the scene " << ex.what() << std::endl;
    }
    return clone;
};

void VISRComponentWrapperObjVector::handleMessage (const char* data, size_t dataSize)
{
    MemoryInputStream inStream (data, dataSize,false);
    try
    {
        std::string newMsg = inStream.readEntireStreamAsString().toStdString();
        {
            std::lock_guard<std::mutex> l(compSceneLock);
            objectmodel::ObjectVectorParser::fillObjectVector(newMsg, scene );
            //            std::cout<< "Handled Message: Object Received within LoudSpeakerRenderer: \n\n" << newMsg << std::endl;
            
        }
    }
    catch( std::exception const & ex )
    {
        printf("ERROR\n");
        std::cerr << "Component Wrapper with Object Vector handling: Error while decoding a scene metadata message: " << ex.what() << std::endl;
    }
    forwardToComponent();
    for (int i = 0; i < sceneObservers.size(); i++) sceneObservers[i]->update();
}

void VISRComponentWrapperObjVector::forwardToComponent(){
    visr::pml::DoubleBufferingProtocol::OutputBase * mObjectVectorOutput = dynamic_cast<visr::pml::DoubleBufferingProtocol::OutputBase *>(&audioFlow->externalParameterReceivePort("objectVector"));
    {
        try{
            std::lock_guard<std::mutex> l(compSceneLock);
            objectmodel::ObjectVector * objects = dynamic_cast<visr::pml::ObjectVector *>(&(mObjectVectorOutput->data()));
            *objects = scene;
            mObjectVectorOutput->swapBuffers();
        }
        catch( std::exception const & ex )
        {
            printf("ERROR\n");
            std::cerr << "Component Wrapper with Object Vector handling: Error while forwarding a scene metadata to the top level component. Message: " << ex.what() << std::endl;
        }
    }
}

} // namespace visr_support
} // namespace visr_production_suite
