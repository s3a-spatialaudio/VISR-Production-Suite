#include "VISRComponentWrapperMulti.h"
#include <libefl/cartesian_spherical_conversion.hpp>
#include <libefl/degree_radian_conversion.hpp>
#include <libcore/IDs.h>
#include <libpml/listener_position.hpp>
#include <libobjectmodel/object_vector_parser.hpp>
#include <libobjectmodel/point_source.hpp>
#include <libobjectmodel/point_source_extent.hpp>
#include <libobjectmodel/point_source_with_reverb.hpp>

namespace visr_production_suite
{
namespace visr_support
{

using namespace visr;
using namespace juce;
using namespace core;

VISRComponentWrapperMulti::VISRComponentWrapperMulti(AudioProcessorValueTreeState& dataModel, std::shared_ptr<visr::CompositeComponent>* renderers, int nRenderers):
UDPReceiver("Component Wrapper for multiple renderers with Object Vector UDP receiver"),
juceObjectVector(dataModel.state.getOrCreateChildWithName(IDs::objectVector, nullptr))
,parameters(dataModel)
{
    if(!UDPReceiver::bind(8001)){
        String errorMsg;
        errorMsg << "Binding to receiving Port failed, this usually happens when you have multiple instances of this plugin\n";
        dataModel.state.getOrCreateChildWithName("Error",nullptr).setProperty("ErrorMsg", errorMsg, nullptr);
        std::cerr << errorMsg << std::endl;
    }else{
        dataModel.state.getOrCreateChildWithName("Error",nullptr).removeAllProperties(nullptr);
    }
    
    topLevelComponents = renderers;
    objv = new visr::pml::ObjectVector();
    for(int w=0;w<nRenderers;w++){
        audioFlow[w].reset(new rrl::AudioSignalFlow( *topLevelComponents[w].get() ));
    }
    update();
}

VISRComponentWrapperMulti::~VISRComponentWrapperMulti(){
    mObjectVectorOutput = nullptr;
    objv = nullptr;
    UDPReceiver::unbind();
    topLevelComponents = nullptr;
    listenerPort = nullptr;
}

void VISRComponentWrapperMulti::setup(){
    currRenderer = parameters.state.getProperty("RendererType",0);
}

void VISRComponentWrapperMulti::addParameterInput(String paramID, const String compPortName){
    parameters.addParameterListener(paramID, this);
}

void VISRComponentWrapperMulti::parameterChanged (const String& pID, float newVal){
    if(topLevelComponents[currRenderer].get()->name() == "SceneRendererCAP"){
        if(pID==IDs::listenerPosX.toString()){
            lX = newVal;
            //            std::cout<<" listenerposx "<<newVal<<std::endl;
        }
        else if(pID==IDs::listenerPosY.toString()){
            lY = newVal;
        }
        else if(pID==IDs::listenerPosZ.toString()){
            lZ = newVal;
        }
        else if(pID==IDs::yaw.toString()){
            yaw = newVal;
        }
        else if(pID==IDs::pitch.toString()){
            pitch = newVal;
        }
        else if(pID==IDs::roll.toString()){
            roll = newVal;
        }
        listenerPort =  dynamic_cast<pml::DoubleBufferingProtocol::OutputBase *>(&audioFlow[currRenderer]->externalParameterReceivePort("tracking"));
        
        visr::pml::ListenerPosition* listenerPos = dynamic_cast<visr::pml::ListenerPosition *>(&(listenerPort->data()));
        listenerPos->setX(lX);
        listenerPos->setY(lY);
        listenerPos->setZ(lZ);
        listenerPos->setOrientation(yaw, pitch, roll);
        listenerPort->swapBuffers();
    }
}

void VISRComponentWrapperMulti::process( SampleType const * const * captureSamples, SampleType * const * playbackSamples){
    {
        std::lock_guard<std::mutex> l(m);
        audioFlow[currRenderer]->processFunction(audioFlow[currRenderer].get(),captureSamples,playbackSamples,res);
    }
}

void VISRComponentWrapperMulti::handleMessage (const char* data, size_t dataSize)
{
    MemoryInputStream inStream (data, dataSize,false);
    try
    {
        std::string newMsg = inStream.readEntireStreamAsString().toStdString();
        objectmodel::ObjectVectorParser::fillObjectVector(newMsg, *objv );
        
    }
    catch( std::exception const & ex )
    {
        printf("ERROR\n");
        std::cerr << "SceneDecoder: Error while decoding a scene metadata message: " << ex.what() << std::endl;
    }
    sendObjectVector();
    updateJuceObjectVector();
}

void VISRComponentWrapperMulti::update(){
    currRenderer = parameters.state.getProperty("RendererType",0);
    sendObjectVector();
    updateJuceObjectVector();
}


void VISRComponentWrapperMulti::sendObjectVector(){
    mObjectVectorOutput = dynamic_cast<visr::pml::DoubleBufferingProtocol::OutputBase *>(&audioFlow[currRenderer]->externalParameterReceivePort("objectVector"));
    {
        std::lock_guard<std::mutex> l(m);
        objectmodel::ObjectVector * objects = dynamic_cast<visr::pml::ObjectVector *>(&(mObjectVectorOutput->data()));
        
        *objects = *objv;
        mObjectVectorOutput->swapBuffers();
    }
}

void VISRComponentWrapperMulti::updateJuceObjectVector(){
    try
    {
        juceObjectVector.removeAllChildren(nullptr);
        for( visr::pml::ObjectVector::const_iterator runIt( objv->cbegin()); runIt != objv->cend(); ++runIt )
        {
            visr::objectmodel::Object & obj( *runIt );
            if(obj.type() == visr::objectmodel::ObjectTypeId::PointSource){
                visr::objectmodel::PointSource * mPs = dynamic_cast< visr::objectmodel::PointSource *>(&(obj));
                String psID;
                psID<<IDs::pointSource<<std::to_string(mPs->id()+1);
                ValueTree ps = juceObjectVector.getOrCreateChildWithName ( psID, nullptr);
                
                float x = mPs->x();
                float y = mPs->y();
                float z = mPs->z();
                
                objectmodel::Object::Coordinate az_, el_, radius_;
                std::tie( az_, el_, radius_ ) = efl::cartesian2spherical(x, y, z);
                
                ps.setProperty(IDs::objID, (int)mPs->id()+1, nullptr);
                ps.setProperty(IDs::azimuth, std::round(efl::radian2degree(az_)), nullptr);
                ps.setProperty(IDs::elevation, std::round(efl::radian2degree(el_)), nullptr);
                ps.setProperty(IDs::radius, radius_, nullptr);
            }
            else if(obj.type() == visr::objectmodel::ObjectTypeId::PointSourceExtent){
                
                visr::objectmodel::PointSource * mPsExt = dynamic_cast< visr::objectmodel::PointSourceExtent *>(&(obj));
                
                String psID;
                psID<<IDs::pointSource<<std::to_string(mPsExt->id()+1);
                ValueTree ps = juceObjectVector.getOrCreateChildWithName ( psID, nullptr);
                
                float x = mPsExt->x();
                float y = mPsExt->y();
                float z = mPsExt->z();
                
                objectmodel::Object::Coordinate az_, el_, radius_;
                std::tie( az_, el_, radius_ ) = efl::cartesian2spherical(x, y, z);
                //
                ps.setProperty(IDs::objID, (int)mPsExt->id()+1, nullptr);
                ps.setProperty(IDs::azimuth,std::round(efl::radian2degree(az_)), nullptr);
                ps.setProperty(IDs::elevation, std::round(efl::radian2degree(el_)), nullptr);
                ps.setProperty(IDs::radius, radius_, nullptr);
            }
            
            else if(obj.type() == visr::objectmodel::ObjectTypeId::PointSourceWithReverb){
                visr::objectmodel::PointSource * mPswr = dynamic_cast< visr::objectmodel::PointSourceWithReverb *>(&(obj));
                
                String psID;
                psID<<IDs::pointSource<<std::to_string(mPswr->id()+1);
                
                float x = mPswr->x();
                float y = mPswr->y();
                float z = mPswr->z();
                
                objectmodel::Object::Coordinate az_, el_, radius_;
                std::tie( az_, el_, radius_ ) = efl::cartesian2spherical(x, y, z);
                
                ValueTree ps = juceObjectVector.getOrCreateChildWithName ( psID, nullptr);
                ps.setProperty(IDs::objID, (int)mPswr->id()+1, nullptr);
                ps.setProperty(IDs::azimuth,std::round(efl::radian2degree(az_)), nullptr);
                ps.setProperty(IDs::elevation, std::round(efl::radian2degree(el_)), nullptr);
                ps.setProperty(IDs::radius, radius_, nullptr);
            }
        }
    }
    catch( std::exception const & ex )
    {
        // Don't abort the program when receiving a corrupted message.
        std::cerr << "ObjectVectorDecoder: Error while decoding a position message: " << ex.what() << std::endl;
    }
}

} // namespace visr_support
} // namespace visr_production_suite
