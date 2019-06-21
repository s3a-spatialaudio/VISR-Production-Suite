/* Copyright Institute of Sound and Vibration Research - All rights reserved */

#ifndef TRACKER_COMPONENT_HPP_INCLUDED
#define TRACKER_COMPONENT_HPP_INCLUDED

#include <libvisr/composite_component.hpp>
#include <libvisr/audio_input.hpp>
#include <libvisr/audio_output.hpp>

#include <librcl/position_decoder.hpp>
#include <librcl/scene_decoder.hpp>
#include <librcl/udp_receiver.hpp>

#include <libpml/listener_position.hpp>
#include <libpml/object_vector.hpp>
#include <libpml/double_buffering_protocol.hpp>
#include <libpml/message_queue_protocol.hpp>

#include <memory>
#include <string>

namespace visr
{

namespace pythoncomponents
{
class Wrapper;
}
}

namespace visr_production_suite
{
namespace dawplugins
{
namespace binaural_renderer
{

/**
 */
class TrackerComponent: public visr::CompositeComponent
{
public:
    
    explicit TrackerComponent( visr::SignalFlowContext const & context,
                                char const * name,
                                CompositeComponent * parent,
                                std::string const & moduleSearchPath,
                                std::string const & trackingReceiverClassName = std::string(),
                                std::string const & trackingReceiverKeywordArguments = std::string(),
                                std::string trackingReceiverModule = std::string()
                                );
      
    ~TrackerComponent();
    
private:

    /**
     * internal head tracking receiver.
     */
    std::unique_ptr<visr::pythoncomponents::Wrapper> mInternalTrackingReceiver;
    
    using TrackingInput = visr::ParameterInput<visr::pml::DoubleBufferingProtocol, visr::pml::ListenerPosition>;
    using TrackingOutput = visr::ParameterOutput<visr::pml::DoubleBufferingProtocol, visr::pml::ListenerPosition>;
    using CalibrationInput = visr::ParameterInput<visr::pml::MessageQueueProtocol, visr::pml::StringParameter>;
    std::unique_ptr< CalibrationInput > mCalibrationInput;
    
    std::unique_ptr< TrackingInput > mTrackingInput;
    std::unique_ptr< TrackingOutput > mTrackingOutput;
};

} // namespace binaural_renderer
} // namespace dawplugins
} // namespace visr_production_suite

#endif // TRACKER_COMPONENT_HPP_INCLUDED
