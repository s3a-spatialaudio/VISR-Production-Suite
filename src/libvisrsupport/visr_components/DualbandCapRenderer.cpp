/* Copyright Institute of Sound and Vibration Research - All rights reserved */
#include "DualbandCapRenderer.hpp"

namespace visr_production_suite
{
namespace visr_support
{
namespace visr_components
{    

DualbandCapRenderer::DualbandCapRenderer( visr::SignalFlowContext const & context,
                                         char const * name,
                                         CompositeComponent * parent,
                                         std::size_t numberOfInputs,
                                         visr::panning::LoudspeakerArray const & arrayConfig,
                                         std::size_t interpolationSteps
)
: CompositeComponent( context, name, parent )
, objectInput( "objectVector", *this, visr::pml::EmptyParameterConfig() )
, listenerInput( "tracking", *this, visr::pml::EmptyParameterConfig() )
, gainMatrixLow( new visr::rcl::GainMatrix( context, "LowFrequencyPanningMatrix", this ))
, mInput( "input", *this, numberOfInputs )
, mOutput( "output", *this, arrayConfig.getNumRegularSpeakers() )
, capCalculator( context, "CapCalculator", this, numberOfInputs, arrayConfig)
{
    if (interpolationSteps == 0)
        interpolationSteps = period();
    
    int numberOfOutputs = arrayConfig.getNumRegularSpeakers();

    // Signal flow: input -> filterbank LF, HF -> ( panning LF, panning HF ) -> combiner -> gain-delay compensation -> output
    
    // Setup and connect filter bank

    // Static crossover pair (2nd-order Linkwitz-Riley with cutoff 700 Hz @ fs=48 kHz)
    static visr::rbbl::BiquadCoefficient<visr::SampleType> const lowpass{ 0.001921697757295f, 0.003843395514590f, 0.001921697757295f, -1.824651307057289f, 0.832338098086468f };
    // Numerator coeffs are negated to account for the 180 degree phase shift of the original design.
    static visr::rbbl::BiquadCoefficient<visr::SampleType> const highpass{ -0.914247351285939f, 1.828494702571878f, -0.914247351285939f, -1.824651307057289f, 0.832338098086468f };
    
    visr::rbbl::BiquadCoefficientMatrix<visr::SampleType> coeffMatrix( 2*numberOfInputs, 1 );
    for( std::size_t chIdx(0); chIdx < numberOfInputs; ++chIdx )
    {
        coeffMatrix( chIdx, 0 ) = highpass;
        coeffMatrix( chIdx + numberOfInputs, 0 ) = lowpass;
    }
    
    
    // Setup and connect LF CAP panning
    
    parameterConnection( objectInput, capCalculator.parameterPort("objectVectorInput") );
    
    parameterConnection( listenerInput, capCalculator.parameterPort("listenerPosition") );
    
    gainMatrixLow->setup( numberOfInputs, numberOfOutputs, interpolationSteps, 0.0f );
    
    parameterConnection( capCalculator.parameterPort("gainOutput"), gainMatrixLow->parameterPort("gainInput"));
    
    // Setup and connect listener position compensation

    mListenerCompensation.reset( new ListenerCompensationCAP( context, "TrackingListenerCompensation", this, arrayConfig ) );
    mListenerGainDelayCompensation.reset( new visr::rcl::DelayVector( context, "ListenerGainDelayCompensation", this ) );

    // for the moment, do not parse any options, but use hard-coded option values.
    visr::SampleType const cMaxDelay = 1.0f; // maximum delay (in seconds)

    // Initial gain of 0.0 to suppress transients on startup.
    mListenerGainDelayCompensation->setup( numberOfOutputs, period(), cMaxDelay, "lagrangeOrder0",
                                          visr::rcl::DelayVector::MethodDelayPolicy::Limit,
                                          visr::rcl::DelayVector::ControlPortConfig::All, 0.0f, 1.0f );
    parameterConnection( listenerInput, mListenerCompensation->parameterPort("positionInput") );
    parameterConnection( mListenerCompensation->parameterPort("delayOutput"), mListenerGainDelayCompensation->parameterPort("delayInput") );
    parameterConnection( mListenerCompensation->parameterPort("gainOutput"), mListenerGainDelayCompensation ->parameterPort("gainInput") );
    
    // Final output
    audioConnection( mListenerGainDelayCompensation->audioPort("out"), mOutput );
    // Temp connections
    
    audioConnection( mInput, ChannelRange( 0, numberOfInputs ),
                     gainMatrixLow->audioPort("in"), ChannelRange( 0, numberOfInputs ) );
    
    audioConnection( gainMatrixLow->audioPort("out"), mListenerGainDelayCompensation->audioPort("in") );
    
}
    
DualbandCapRenderer::~DualbandCapRenderer( ) {}

} // namespace visr_components
} // namespace visr_support
} // namespace visr_production_suite
