/* Copyright Institute of Sound and Vibration Research - All rights reserved */

#include "MetadataExposingRenderer.hpp"

namespace visr_production_suite
{
namespace visr_support
{
namespace visr_components
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
, mCoreRenderer( context, "CoreRenderer", this, loudspeakerConfiguration, numberOfInputs, numberOfOutputs,
                interpolationPeriod, diffusionFilters, trackingConfiguration, numberOfObjectEqSections,
                reverbConfig, frequencyDependentPanning )
, mObjectVectorInput( "objectVector", *this, visr::pml::EmptyParameterConfig() )
, mInput( "input", *this, numberOfInputs )
, mOutput( "output", *this, numberOfOutputs )
{
    audioConnection( mInput, mCoreRenderer.audioPort( "audioIn") );
    audioConnection( mCoreRenderer.audioPort( "audioOut"), mOutput );
    parameterConnection( mObjectVectorInput, mCoreRenderer.parameterPort("objectDataInput") );
    if( not trackingConfiguration.empty() )
    {
        mTrackingReceiver.reset( new visr::rcl::UdpReceiver( context, "TrackingReceiver", nullptr, 8888, visr::rcl::UdpReceiver::Mode::Synchronous ) );
        mTrackingPositionDecoder.reset( new visr::rcl::PositionDecoder( context, "TrackingPositionDecoder",nullptr,visr::panning::XYZ( 0.0f, 0.0f, 0.0f ) ) );
    }
}

MetadataExposingRenderer::~MetadataExposingRenderer( ){}
    
} // namespace visr_components
} // namespace visr_support
} // namespace visr_production_suite
