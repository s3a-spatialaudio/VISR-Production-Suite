/* Copyright Institute of Sound and Vibration Research - All rights reserved */

#include "tracker_component.hpp"

#include <libpythoncomponents/wrapper.hpp>

#include <ciso646>

namespace visr_production_suite
{
namespace dawplugins
{
namespace binaural_renderer
{

using namespace visr;
    
TrackerComponent::TrackerComponent( visr::SignalFlowContext const & context,
                                char const * name,
                                CompositeComponent * parent,
                                std::string const & moduleSearchPath,
                                std::string const & trackingReceiverClassName /*= std::string()*/,
                                std::string const & trackingReceiverKeywordArguments /*= std::string() */,
                                std::string trackingReceiverModule /*= std::string()*/
                                )
  : CompositeComponent( context, name, parent )
  , mTrackingOutput( new TrackingOutput( "listenerPositionOut", *this, pml::EmptyParameterConfig() ) )
  , mCalibrationInput( new CalibrationInput( "calibrationInput", *this, pml::EmptyParameterConfig() ) )
{

    mInternalTrackingReceiver.reset( new pythoncomponents::Wrapper( context, "TrackingReceiver", this,
                                                                   trackingReceiverModule.c_str(),
                                                                   trackingReceiverClassName.c_str(),
                                                                   "",
                                                                   trackingReceiverKeywordArguments.c_str(),
                                                                   moduleSearchPath.c_str() ) );

        parameterConnection( mInternalTrackingReceiver->parameterPort( "orientation" ), *mTrackingOutput );
        parameterConnection( *mCalibrationInput, mInternalTrackingReceiver->parameterPort( "calibration" ));
    
    
}

TrackerComponent::~TrackerComponent() = default;

} // namespace binaural_renderer
} // namespace dawplugins
} // namespace visr_production_suite
