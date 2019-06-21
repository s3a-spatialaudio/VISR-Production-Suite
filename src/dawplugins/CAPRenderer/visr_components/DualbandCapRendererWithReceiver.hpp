/* Copyright Institute of Sound and Vibration Research - All rights reserved */

#ifndef DUALBANDCAP_HPP_INCLUDED
#define DUALBANDCAP_HPP_INCLUDED

#include <librcl/biquad_iir_filter.hpp>
#include <librcl/gain_matrix.hpp>
#include <librcl/add.hpp>
#include <librcl/panning_calculator.hpp>
#include <librcl/delay_vector.hpp>
#include <librcl/position_decoder.hpp>
#include <librcl/scene_decoder.hpp>
#include <librcl/udp_receiver.hpp>
#include <librcl/cap_gain_calculator.hpp>
#include <libvisr/composite_component.hpp>
#include <libvisr/audio_input.hpp>
#include <libvisr/audio_output.hpp>
#include <libpml/listener_position.hpp>
#include <libpml/object_vector.hpp>
#include <libpml/double_buffering_protocol.hpp>
#include <libpanning/LoudspeakerArray.h>
#include "listener_compensation_cap.hpp"
namespace visr
{
namespace dawplugins
{
namespace cap_renderer
{
    
/**
 * Audio signal graph object for the VISR baseline renderer.
 */
class  DualbandCapRenderer: public visr::CompositeComponent
{
public:
    explicit DualbandCapRenderer( visr::SignalFlowContext const & context,
                                 char const * name,
                                 CompositeComponent * parent,
                                 std::size_t numberOfInputs,
                                 visr::panning::LoudspeakerArray const & loudspeakerConfiguration,
                                 std::size_t sceneReceiverPort,
                                 std::size_t interpolationSteps = 0
//                                 rcl::BiquadIirFilter filterBank
                                 );

    ~DualbandCapRenderer();
    
private:
    
    visr::rcl::UdpReceiver mSceneReceiver;
    
    visr::rcl::SceneDecoder mSceneDecoder;
    
    
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
    //@}
    
    
    
  
    visr::ParameterInput< visr::pml::DoubleBufferingProtocol, visr::pml::ListenerPosition > listenerInput;
    
    
    std::unique_ptr<visr::rcl::BiquadIirFilter> mPanningFilterbank;
    std::unique_ptr<visr::rcl::GainMatrix> gainMatrixLow;
    visr::rcl::GainMatrix gainMatrixHigh;
    visr::ParameterOutput< visr::pml::DoubleBufferingProtocol, visr::pml::ObjectVector > mObjectVectorOutput;
//
    visr::rcl::Add mCombiner;
    visr::rcl::PanningCalculator vbapCalculator;
    visr::AudioInput mInput;
    visr::AudioOutput mOutput;
    
//    MatrixTerminator mt;
    
    visr::rcl::CAPGainCalculator capCalculator;
    std::unique_ptr<ListenerCompensationCAP> mListenerCompensation;
    
    std::unique_ptr<visr::rcl::DelayVector> mListenerGainDelayCompensation;
    
};
} // namespace cap_renderer
} // namespace dawplugins
} // namespace visr

#endif // DUALBANDCAP_HPP_INCLUDED
