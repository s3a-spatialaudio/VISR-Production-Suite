/* Copyright Institute of Sound and Vibration Research - All rights reserved */

#include "ObjectHandler.hpp"
namespace visr
{
namespace dawplugins
{
namespace scene_master
{

ObjectHandler::ObjectHandler( visr::SignalFlowContext const & context,
                             char const * name,
                             CompositeComponent * parent,
                             std::size_t numberOfInputs,
                             std::size_t numberOfOutputs,
                             std::size_t sceneReceiverPort,
                             std::string const & sceneSendIP,
                             std::size_t sceneSendPort
                             
                             )
: CompositeComponent( context, name, parent )
, mSceneReceiver( context, "SceneReceiver", this, sceneReceiverPort, visr::rcl::UdpReceiver::Mode::Asynchronous )
, mSceneDecoder( context, "SceneDecoder", this )
, mSceneEncoder( context, "SceneEncoder", this )
, mSceneSender( context, "SceneSender", this,0, sceneSendIP, sceneSendPort, visr::rcl::UdpSender::Mode::Asynchronous  )
//, mObjectVectorInput( "objectDataInput", *this, visr::pml::EmptyParameterConfig() )
, mObjectVectorOutput( "objectVectorOutput", *this, visr::pml::EmptyParameterConfig())
, mInput( "input", *this, numberOfInputs )
, mOutput( "output", *this, numberOfOutputs )
{
    audioConnection( mInput, mOutput);
    
    
    parameterConnection( mSceneReceiver.parameterPort("messageOutput"), mSceneDecoder.parameterPort("datagramInput") );
    parameterConnection( mSceneDecoder.parameterPort("objectVectorOutput"), mSceneEncoder.parameterPort("objectInput") );
    parameterConnection( mSceneDecoder.parameterPort("objectVectorOutput"), mObjectVectorOutput );
    parameterConnection( mSceneEncoder.parameterPort("messageOutput"), mSceneSender.parameterPort("messageInput") );
}

ObjectHandler::~ObjectHandler( )
{
}
} // namespace scene_master
} // namespace dawplugins
} // namespace visr
