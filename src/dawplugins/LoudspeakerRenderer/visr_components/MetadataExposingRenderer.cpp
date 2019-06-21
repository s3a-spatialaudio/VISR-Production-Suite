/* Copyright Institute of Sound and Vibration Research - All rights reserved */
#include "MetadataExposingRenderer.hpp"
#include <libpanning/LoudspeakerArray.h>
namespace visr_production_suite
{
namespace dawplugins
{
namespace loudspeaker_renderer
{
    
MetadataExposingRenderer::MetadataExposingRenderer( visr::SignalFlowContext const & context,
                                           char const * name,
                                           CompositeComponent * parent,
                                           visr::panning::LoudspeakerArray const & loudspeakerConfiguration,
                                           std::size_t numberOfInputs,
                                           std::size_t numberOfOutputs
                                           )
        : MetadataExposingRenderer( context, name, parent, loudspeakerConfiguration, numberOfInputs, numberOfOutputs,
                           4096ul,
                           visr::efl::BasicMatrix<visr::SampleType>(numberOfOutputs, 0, visr::cVectorAlignmentSamples ),
                           std::string(), 4242, 0 /*number of EQ sections*/, std::string(), false )
        {}
        
        MetadataExposingRenderer::MetadataExposingRenderer( visr::SignalFlowContext const & context,
                                           char const * name,
                                           CompositeComponent * parent,
                                           visr::panning::LoudspeakerArray const & loudspeakerConfiguration,
                                           std::size_t numberOfInputs,
                                           std::size_t numberOfOutputs,
                                           std::size_t interpolationPeriod,
                                           visr::efl::BasicMatrix<visr::SampleType> const & diffusionFilters,
                                           std::string const & trackingConfiguration,
                                           std::size_t sceneReceiverPort,
                                           std::size_t numberOfObjectEqSections,
                                           std::string const & reverbConfig,
                                           bool frequencyDependentPanning )
        : CompositeComponent( context, name, parent )
        , mSceneReceiver( context, "SceneReceiver", this, sceneReceiverPort, visr::rcl::UdpReceiver::Mode::Asynchronous )
        , mSceneDecoder( context, "SceneDecoder", this )
        , mCoreRenderer( context, "CoreRenderer", this, loudspeakerConfiguration, numberOfInputs, numberOfOutputs,
                        interpolationPeriod, diffusionFilters, trackingConfiguration, numberOfObjectEqSections,
                        reverbConfig, frequencyDependentPanning )
        , mObjectVectorOutput( "objectVectorOutput", *this, visr::pml::EmptyParameterConfig())
        , mInput( "input", *this, numberOfInputs )
        , mOutput( "output", *this, numberOfOutputs )

        {
            audioConnection( mInput, mCoreRenderer.audioPort( "audioIn") );
            audioConnection( mCoreRenderer.audioPort( "audioOut"), mOutput );
            parameterConnection( mSceneReceiver.parameterPort("messageOutput"), mSceneDecoder.parameterPort("datagramInput") );
            parameterConnection( mSceneDecoder.parameterPort("objectVectorOutput"), mCoreRenderer.parameterPort("objectDataInput") );
            parameterConnection( mSceneDecoder.parameterPort("objectVectorOutput"), mObjectVectorOutput);
            
            if( not trackingConfiguration.empty() )
            {
                mTrackingReceiver.reset( new visr::rcl::UdpReceiver( context, "TrackingReceiver", nullptr, 8888, visr::rcl::UdpReceiver::Mode::Synchronous ) );
                mTrackingPositionDecoder.reset( new visr::rcl::PositionDecoder( context, "TrackingPositionDecoder",nullptr,visr::panning::XYZ( 0.0f, 0.0f, 0.0f ) ) );
            }
        }
        
        MetadataExposingRenderer::~MetadataExposingRenderer( )
        {
        }
} // namespace loudspeaker_renderer
} // namespace dawplugins
} // namespace visr_production_suite
