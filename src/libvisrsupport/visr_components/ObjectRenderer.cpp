/* Copyright Institute of Sound and Vibration Research - All rights reserved */

#include "ObjectRenderer.hpp"

namespace visr_production_suite
{
namespace visr_support
{
namespace visr_components
{
    
ObjectRenderer::ObjectRenderer( visr::SignalFlowContext const & context,
                                   char const * name,
                                   CompositeComponent * parent,
                                   visr::panning::LoudspeakerArray const & loudspeakerConfiguration,
                                   std::size_t numberOfInputs,
                                   std::size_t numberOfOutputs
                                   )
: ObjectRenderer( context, name, parent, loudspeakerConfiguration, numberOfInputs, numberOfOutputs,
                   4096ul,
                   visr::efl::BasicMatrix<visr::SampleType>(numberOfOutputs, 0, visr::cVectorAlignmentSamples ),
                   std::string(), 0 /*number of EQ sections*/, std::string(), false )
{}

ObjectRenderer::ObjectRenderer( visr::SignalFlowContext const & context,
                                   char const * name,
                                   CompositeComponent * parent,
                                   visr::panning::LoudspeakerArray const & loudspeakerConfiguration,
                                   std::size_t numberOfInputs,
                                   std::size_t numberOfOutputs,
                                   std::size_t interpolationPeriod,
                                   visr::efl::BasicMatrix<visr::SampleType> const & diffusionFilters,
                                   std::string const & trackingConfiguration,
                                   std::size_t numberOfObjectEqSections,
                                   std::string const & reverbConfig,
                                   bool frequencyDependentPanning )
: CompositeComponent( context, name, parent )
, mCoreRenderer( context, "CoreRenderer", this, loudspeakerConfiguration, numberOfInputs, numberOfOutputs,
                interpolationPeriod, diffusionFilters, trackingConfiguration, numberOfObjectEqSections,
                reverbConfig, frequencyDependentPanning )
, mInput( "input", *this, numberOfInputs )
, mOutput( "output", *this, numberOfOutputs )
, azPosInput( "azPositionInput", *this, visr::pml::EmptyParameterConfig() )
, elPosInput( "elPositionInput", *this, visr::pml::EmptyParameterConfig() )
, radiusPosInput( "radiusPositionInput", *this, visr::pml::EmptyParameterConfig() )
, objectIDInput("objID", *this, visr::pml::EmptyParameterConfig() )
{
    audioConnection( mInput, mCoreRenderer.audioPort( "audioIn") );
    audioConnection( mCoreRenderer.audioPort( "audioOut"), mOutput );
    mObjectVectorEncoder.reset( new visr_support::visr_components::ObjectVectorEncoder( context, "ObjectVectorEncoder", this) );
    mObjectVectorEncoder->setup();
    parameterConnection( azPosInput, mObjectVectorEncoder->parameterPort("azPosInput") );
    parameterConnection( elPosInput, mObjectVectorEncoder->parameterPort("elPosInput") );
    parameterConnection( radiusPosInput, mObjectVectorEncoder->parameterPort("radiusPosInput") );
     parameterConnection( objectIDInput, mObjectVectorEncoder->parameterPort("objectIDInput") );
    parameterConnection( mObjectVectorEncoder->parameterPort("objectVectorOut"), mCoreRenderer.parameterPort("objectDataInput") );
}

ObjectRenderer::~ObjectRenderer( )
{
}
    
} // namespace visr_components
} // namespace visr_support
} // namespace visr_production_suite
