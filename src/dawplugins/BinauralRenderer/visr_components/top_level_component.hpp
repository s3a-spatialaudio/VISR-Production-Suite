/* Copyright Institute of Sound and Vibration Research - All rights reserved */

#ifndef VISR_DAWPLUGINS_BINAURAL_RENDERER_TOP_LEVEL_COMPONENT_HPP_INCLUDED
#define VISR_DAWPLUGINS_BINAURAL_RENDERER_TOP_LEVEL_COMPONENT_HPP_INCLUDED

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


namespace visr{
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
 * Top-level component encapsulating a binaural renderer and metadata network receiver/decoder, potentially exposing the object metadata and the tracking position.
 */
class TopLevelComponent: public visr::CompositeComponent
{
public:
    explicit TopLevelComponent( visr::SignalFlowContext const & context,
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
                                // bool listenerPositionInput = true,
                                bool exposeObjectVector = false,
                                bool exposeListenerPosition = false,
                                std::string const & trackingReceiverClassName = std::string(),
                                std::string const & trackingReceiverKeywordArguments = std::string(),
                                std::string trackingReceiverModule = std::string()
                                );
      
    ~TopLevelComponent();
    
private:
    
    visr::rcl::UdpReceiver mSceneReceiver;
    
    visr::rcl::SceneDecoder mSceneDecoder;

    /**
     * Conditionally instantiated internal head tracking receiver.
     */
    std::unique_ptr<visr::pythoncomponents::Wrapper> mInternalTrackingReceiver;

    std::unique_ptr<visr::pythoncomponents::Wrapper> mBinauralRenderer;

    using TrackingInput = visr::ParameterInput<visr::pml::DoubleBufferingProtocol, visr::pml::ListenerPosition>;
    using TrackingOutput = visr::ParameterOutput<visr::pml::DoubleBufferingProtocol, visr::pml::ListenerPosition>;

    using ObjectOutput = visr::ParameterOutput< visr::pml::DoubleBufferingProtocol, visr::pml::ObjectVector >;
    
    std::unique_ptr< ObjectOutput > mObjectVectorOutput;

    std::unique_ptr< TrackingInput > mTrackingInput;

    std::unique_ptr< TrackingOutput > mTrackingOutput;

    visr::AudioInput mInput;
    visr::AudioOutput mOutput;
};

} // namespace binaural_renderer
} // namespace dawplugins
} // namespace visr_production_suite

#endif // VISR_DAWPLUGINS_BINAURAL_RENDERER_TOP_LEVEL_COMPONENT_HPP_INCLUDED
