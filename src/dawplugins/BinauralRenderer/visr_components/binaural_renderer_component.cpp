/* Copyright Institute of Sound and Vibration Research - All rights reserved */

#include "binaural_renderer_component.hpp"

#include <libpythoncomponents/wrapper.hpp>

#include <ciso646>

namespace visr_production_suite
{
namespace dawplugins
{
namespace binaural_renderer
{
using namespace visr;
    
BinauralRendererComponent::BinauralRendererComponent( visr::SignalFlowContext const & context,
                                char const * name,
                                CompositeComponent * parent,
                                std::size_t numberOfObjects,
                                std::string const & rendererModuleName,
                                std::string const & rendererClassName,
                                std::string const & rendererPositionalArguments,
                                std::string const & rendererKeywordArguments,
                                std::string const & moduleSearchPath,
                                std::size_t sceneReceiverPort,
                                std::size_t useTracking,
                                // bool listenerPositionInput /*= true*/,
                                bool exposeObjectVector /*= false*/,
                                bool exposeListenerPosition /*= false*/,
                                std::string const & trackingReceiverClassName /*= std::string()*/,
                                std::string const & trackingReceiverKeywordArguments /*= std::string() */,
                                std::string trackingReceiverModule /*= std::string()*/
                                )
  : CompositeComponent( context, name, parent )
  , mObjectVectorInput( "objectVector", *this, visr::pml::EmptyParameterConfig() )
  , mObjectVectorOutput( exposeObjectVector ? new ObjectOutput( "objectVectorOut", *this, pml::EmptyParameterConfig() ) : nullptr )
  , mTrackingOutput( exposeListenerPosition ? new TrackingOutput( "listenerPositionOut", *this, pml::EmptyParameterConfig() ) : nullptr )
  , mInput( "audioIn", *this, numberOfObjects )
  , mOutput( "audioOut", *this, 2 )
{
  if( (not useTracking) and exposeListenerPosition )
  {
    throw std::invalid_argument( "The option \"exposeListenerPosition\" must not be active if head tracking is off." );
  }

  mBinauralRenderer.reset( new pythoncomponents::Wrapper( context, "Renderer", this,
    rendererModuleName.c_str(), rendererClassName.c_str(), rendererPositionalArguments.c_str(), rendererKeywordArguments.c_str(), moduleSearchPath.c_str() ) );
  audioConnection( mInput, mBinauralRenderer->audioPort("audioIn") );
  audioConnection( mBinauralRenderer->audioPort( "audioOut" ), mOutput );

  parameterConnection( mObjectVectorInput, mBinauralRenderer->parameterPort("objectVector") );
  if( exposeObjectVector )
  {
    parameterConnection( mObjectVectorInput, *mObjectVectorOutput );
  }

  if( useTracking )
  {
      mTrackingInput.reset( new TrackingInput( "listenerPositionIn", *this, pml::EmptyParameterConfig() ) );
      parameterConnection( *mTrackingInput, mBinauralRenderer->parameterPort( "tracking" ) );
      // Supported albeit somewhat stupid constellation:
      // The external tracking input is directly routed to the external output
      if( exposeListenerPosition )
      {
        parameterConnection( *mTrackingInput,
                             *mTrackingOutput );
      }
  } // if( useTracking )
}

BinauralRendererComponent::~BinauralRendererComponent() = default;

} // namespace binaural_renderer
} // namespace dawplugins
} // namespace visr_production_suite
