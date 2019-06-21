/* Copyright Institute of Sound and Vibration Research - All rights reserved */

#include "DualbandCapRendererWithReceiver.hpp"
#include <libpml/biquad_parameter.hpp>
namespace visr
{
namespace dawplugins
{
namespace cap_renderer
{
    
DualbandCapRenderer::DualbandCapRenderer( visr::SignalFlowContext const & context,
                                         char const * name,
                                         CompositeComponent * parent,
                                         std::size_t numberOfInputs,
                                         visr::panning::LoudspeakerArray const & arrayConfig,
                                         std::size_t sceneReceiverPort,
                                         std::size_t interpolationSteps
//                                                   visr::rcl::BiquadIirFilter filterBank =
)
: CompositeComponent( context, name, parent )
        , mSceneReceiver( context, "SceneReceiver", this )
, mSceneDecoder( context, "SceneDecoder", this )
, listenerInput( "tracking", *this, visr::pml::EmptyParameterConfig() )
, mPanningFilterbank( new visr::rcl::BiquadIirFilter( context, "PanningFilterbank", this ))
, gainMatrixLow( new visr::rcl::GainMatrix( context, "LowFrequencyPanningMatrix", this ))
, gainMatrixHigh( context, "VbapGainMatrix", this )
, mObjectVectorOutput( "objectVectorOutput", *this, visr::pml::EmptyParameterConfig())
, mCombiner( context, "Combiner", this,arrayConfig.getNumRegularSpeakers(), 2)
, vbapCalculator( context, "VbapGainCalculator", this, numberOfInputs, arrayConfig, false, visr::rcl::PanningCalculator::PanningMode::HF)
, mInput( "input", *this, numberOfInputs )
, mOutput( "output", *this, arrayConfig.getNumRegularSpeakers() )
, capCalculator( context, "CapCalculator", this, numberOfInputs, arrayConfig)
{
    if (interpolationSteps == 0)
        interpolationSteps = period();
    
    int numberOfOutputs = arrayConfig.getNumRegularSpeakers();
    
    
    
    
    
    
    mSceneReceiver.setup( sceneReceiverPort, visr::rcl::UdpReceiver::Mode::Asynchronous );
    parameterConnection( mSceneReceiver.parameterPort("messageOutput"), mSceneDecoder.parameterPort("datagramInput") );
    
    parameterConnection( mSceneDecoder.parameterPort("objectVectorOutput"), mObjectVectorOutput );
    
    
    
    mListenerCompensation.reset( new ListenerCompensationCAP( context, "TrackingListenerCompensation", this, arrayConfig ) );
    mListenerGainDelayCompensation.reset( new visr::rcl::DelayVector( context, "ListenerGainDelayCompensation", this ) );
    
    // for the very moment, do not parse any options, but use hard-coded option values.
    visr::SampleType const cMaxDelay = 1.0f; // maximum delay (in seconds)
    
    // We start with a initial gain of 0.0 to suppress transients on startup.
    mListenerGainDelayCompensation->setup( numberOfOutputs, period(), cMaxDelay, "lagrangeOrder0",
                                          visr::rcl::DelayVector::MethodDelayPolicy::Limit,
                                          visr::rcl::DelayVector::ControlPortConfig::All,
                                          0.0f, 1.0f );
    parameterConnection( listenerInput, mListenerCompensation->parameterPort("positionInput") );
    parameterConnection( mListenerCompensation->parameterPort("delayOutput"), mListenerGainDelayCompensation->parameterPort("delayInput") );
    parameterConnection( mListenerCompensation->parameterPort("gainOutput"), mListenerGainDelayCompensation->parameterPort("gainInput") );
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    // Static crossover pair (2nd-order Linkwitz-Riley with cutoff 700 Hz @ fs=48 kHz)
    static visr::rbbl::BiquadCoefficient<visr::SampleType> const lowpass{ 0.001921697757295f, 0.003843395514590f, 0.001921697757295f,
        -1.824651307057289f, 0.832338098086468f };
    // Numerator coeffs are negated to account for the 180 degree phase shift of the original design.
    static visr::rbbl::BiquadCoefficient<visr::SampleType> const highpass{ -0.914247351285939f, 1.828494702571878f, -0.914247351285939f,
        -1.824651307057289f, 0.832338098086468f };
    
    visr::rbbl::BiquadCoefficientMatrix<visr::SampleType> coeffMatrix( 2*numberOfInputs, 1 );
    for( std::size_t chIdx(0); chIdx < numberOfInputs; ++chIdx )
    {
        coeffMatrix( chIdx, 0 ) = highpass;
        coeffMatrix( chIdx + numberOfInputs, 0 ) = lowpass;
    }
    
    mPanningFilterbank->setup( 2*numberOfInputs, 1, coeffMatrix );
    audioConnection( mInput, ChannelRange( 0, numberOfInputs ), mPanningFilterbank->audioPort("in"), ChannelRange( 0, numberOfInputs ) );
    audioConnection( mInput, ChannelRange( 0, numberOfInputs ), mPanningFilterbank->audioPort("in"), ChannelRange( numberOfInputs, 2*numberOfInputs ) );
    
    
    
    audioConnection( mCombiner.audioPort("out"), mListenerGainDelayCompensation->audioPort("in")  );
    audioConnection( mListenerGainDelayCompensation->audioPort("out"), mOutput );
    
    
// LF CAP panning
    parameterConnection( mSceneDecoder.parameterPort("objectVectorOutput"), capCalculator.parameterPort("objectVectorInput") );
    
    
    parameterConnection( listenerInput, capCalculator.parameterPort("listenerPosition") );
    
    gainMatrixLow->setup( numberOfInputs, numberOfOutputs, interpolationSteps, 0.0f );
    
    parameterConnection( capCalculator.parameterPort("gainOutput"), gainMatrixLow->parameterPort("gainInput"));
    
    
    
    
    
    audioConnection( mPanningFilterbank->audioPort("out"), ChannelRange( 0, numberOfInputs ),gainMatrixLow->audioPort("in"), ChannelRange( 0, numberOfInputs ) );
    
    audioConnection( gainMatrixLow->audioPort("out"), mCombiner.audioPort("in0"));
    
    
    parameterConnection( mSceneDecoder.parameterPort("objectVectorOutput"), vbapCalculator.parameterPort("objectVectorInput") );
    gainMatrixHigh.setup( numberOfInputs, numberOfOutputs, interpolationSteps, 0.0f );
    
    parameterConnection( vbapCalculator.parameterPort("vbipGains"), gainMatrixHigh.parameterPort("gainInput"));
    
//    parameterConnection( vbapCalculator.parameterPort("lowFrequencyGainOutput"), mt.parameterPort("lowFrequencyGainInput"));
    
    
    audioConnection( mPanningFilterbank->audioPort("out"), ChannelRange( numberOfInputs, 2*numberOfInputs ),  gainMatrixHigh.audioPort("in"), ChannelRange( 0, numberOfInputs ) );

    audioConnection( gainMatrixHigh.audioPort("out"),
                    mCombiner.audioPort("in1"));
}

DualbandCapRenderer::~DualbandCapRenderer( )
{
}
    
} // namespace cap_renderer
} // namespace dawplugins
} // namespace visr
