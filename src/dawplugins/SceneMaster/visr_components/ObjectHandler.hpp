/* Copyright Institute of Sound and Vibration Research - All rights reserved */

#ifndef OBJECTHANDLER_HPP_INCLUDED
#define OBJECTHANDLER_HPP_INCLUDED

#include <libvisr/composite_component.hpp>
#include <libvisr/audio_input.hpp>
#include <libvisr/audio_output.hpp>

#include <librcl/scene_decoder.hpp>
#include <librcl/udp_receiver.hpp>
#include <librcl/scene_encoder.hpp>
#include <librcl/udp_sender.hpp>

#include <libpml/double_buffering_protocol.hpp>
namespace visr
{
namespace dawplugins
{
namespace scene_master
{
/**
 * Audio signal graph object for the VISR baseline renderer.
 */
class  ObjectHandler: public visr::CompositeComponent
{
public:
    /**
     * Constructor to create, initialise and interconnect all processing components.
     * @param context The signal flow context object containing information such as sampling frequency and period (block) size.
     * @param name The name of the component, used for identification and error reporting.
     * @param parent The containing component, if there is one. Use nullptr to mark this as the toplevel component.
     * @param numberOfInputs The number of inputs, i.e., the number of audio object signals
     * @param numberOfOutputs The number of output channels. This number can be higher than the number of loudspeakers plus the number of subwoofers if the
     * routing to output channels contains gaps. The output channels between 0 and \p numbeOfOutputs to which no signal is routed are configured to output zeros.
     * @param sceneReceiverPort The UDP port for receiving the scene data messages.
     */
    explicit ObjectHandler( visr::SignalFlowContext const & context,
                           char const * name,
                           CompositeComponent * parent,
                           std::size_t numberOfInputs,
                           std::size_t numberOfOutputs,
                           std::size_t sceneReceiverPort,
                           std::string const & sceneSendIP,
                           std::size_t sceneSendPort );
    
        ~ObjectHandler();
    
private:
    
    visr::rcl::UdpReceiver mSceneReceiver;
    visr::rcl::SceneDecoder mSceneDecoder;
    
    visr::rcl::SceneEncoder mSceneEncoder;
    visr::rcl::UdpSender mSceneSender;
//    
//    visr::ParameterInput< visr::pml::DoubleBufferingProtocol, visr::pml::ObjectVector > mObjectVectorInput;
    visr::ParameterOutput< visr::pml::DoubleBufferingProtocol, visr::pml::ObjectVector > mObjectVectorOutput;
    
    visr::AudioInput mInput;
    visr::AudioOutput mOutput;
};
} // namespace scene_master
} // namespace dawplugins
} // namespace visr

#endif // VISR_SIGNALFLOWS_METADATAEXPOSING_HPP_INCLUDED
