/* Copyright Institute of Sound and Vibration Research - All rights reserved */

#ifndef DUALBANDCAP_HPP_INCLUDED
#define DUALBANDCAP_HPP_INCLUDED

#include <librcl/biquad_iir_filter.hpp>
#include <librcl/gain_matrix.hpp>
#include <librcl/add.hpp>
#include <librcl/panning_calculator.hpp>
#include <librcl/delay_vector.hpp>

#include <libvisr/composite_component.hpp>
#include <libvisr/audio_input.hpp>
#include <libvisr/audio_output.hpp>

#include <libpanning/LoudspeakerArray.h>
#include "listener_compensation_cap.hpp"

#include <librcl/position_decoder.hpp>
#include <librcl/udp_receiver.hpp>
#include <librcl/cap_gain_calculator.hpp>
#include <libpml/listener_position.hpp>
#include <libpml/object_vector.hpp>
#include <libpml/double_buffering_protocol.hpp>

namespace visr_production_suite
{
namespace visr_support
{
namespace visr_components
{    

/**
 * Audio signal graph object for the CAP renderer.
 */
class  DualbandCapRenderer: public visr::CompositeComponent
{
public:
    explicit DualbandCapRenderer( visr::SignalFlowContext const & context,
                                 char const * name,
                                 CompositeComponent * parent,
                                 std::size_t numberOfInputs,
                                 visr::panning::LoudspeakerArray const & loudspeakerConfiguration,
                                 std::size_t interpolationSteps = 0
                                 );

    ~DualbandCapRenderer();
    
private:
    /**
     * Tracking-related members
     */
    //@{
    /**
     * UDP port for receiving listener position updates
     */
    std::unique_ptr<visr::rcl::UdpReceiver> mTrackingReceiver;
    
    /**
     * Component that transforms JSON strings into the internal listener position format
     */
    std::unique_ptr<visr::rcl::PositionDecoder> mTrackingPositionDecoder;
    
    visr::ParameterInput< visr::pml::DoubleBufferingProtocol, visr::pml::ObjectVector > objectInput;
    visr::ParameterInput< visr::pml::DoubleBufferingProtocol, visr::pml::ListenerPosition > listenerInput;
    
    std::unique_ptr<visr::rcl::GainMatrix> gainMatrixLow;
    visr::AudioInput mInput;
    visr::AudioOutput mOutput;
    
    visr::rcl::CAPGainCalculator capCalculator;
    
    std::unique_ptr<ListenerCompensationCAP> mListenerCompensation;
    std::unique_ptr<visr::rcl::DelayVector> mListenerGainDelayCompensation;
};
    
} // namespace visr_components
} // namespace visr_support
} // namespace visr_production_suite

#endif // VISR_SIGNALFLOWS_METADATAEXPOSING_HPP_INCLUDED
