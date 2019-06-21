/* Copyright Institute of Sound and Vibration Research - All rights reserved */
#include "DualbandCapRenderer.hpp"

namespace visr_production_suite
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
                                         std::size_t interpolationSteps
)
: CompositeComponent( context, name, parent )
, mInput( "input", *this, numberOfInputs )
, mOutput( "output", *this, arrayConfig.getNumRegularSpeakers() )
, objectInput( "objectVector", *this, visr::pml::EmptyParameterConfig() )
, mPanningFilterbank( new visr::rcl::BiquadIirFilter( context, "PanningFilterbank", this ))
, listenerInput( "listenerPositionIn", *this, visr::pml::EmptyParameterConfig() )
, capCalculator( context, "CapCalculator", this, numberOfInputs, arrayConfig,
                 visr::rcl::CAPGainCalculator::PanningMode::LF,   // LF - normal CAP, HF - experimental HF CAP
                 true ) // Activate high-frequency output port (for VBIP extension)
, gainMatrixLow( new visr::rcl::GainMatrix( context, "LowFrequencyPanningMatrix", this ))
, gainMatrixHigh( context, "VbapGainMatrix", this )
, mCombiner( context, "Combiner", this,arrayConfig.getNumRegularSpeakers(), 2)
{
    capCalculator.setNearTripletBoundaryCosTheta(-0.7f);
    
    if (interpolationSteps == 0)
        interpolationSteps = period();
    
    std::size_t const numberOfOutputs = arrayConfig.getNumRegularSpeakers();

    
    
    
    //// Overall signal flow: input -> filterbank LF, HF -> ( panning LF, panning HF ) -> combiner -> gain-delay compensation -> output
    

    
    //// Setup and connect crossover filter bank
    //// also used for soloing either CAP or VBAP
    
    // Static crossover pair (2nd-order Linkwitz-Riley with cutoff 700 Hz @ fs=48 kHz)
//    static int const nBiquadSections = 1;
//    static visr::rbbl::BiquadCoefficient<visr::SampleType> const
//    lowpass0{ 0.001921697757295f, 0.003843395514590f, 0.001921697757295f, -1.824651307057289f, 0.832338098086468f };
//    
//    static visr::rbbl::BiquadCoefficient<visr::SampleType> const
//    highpass0{ -0.914247351285939f, 1.828494702571878f, -0.914247351285939f, -1.824651307057289f, 0.832338098086468f };
    
    
    // Pure CAP
//    static int const nBiquadSections = 1;
//    static visr::rbbl::BiquadCoefficient<visr::SampleType> const lowpass0{ 1,0,0,0,0 };
//    static visr::rbbl::BiquadCoefficient<visr::SampleType> const highpass0{ 0,0,0,0,0 };
    
    // Pure VBIP
//    static int const nBiquadSections = 1;
//    static visr::rbbl::BiquadCoefficient<visr::SampleType> const lowpass0{ 0,0,0,0,0 };
//    static visr::rbbl::BiquadCoefficient<visr::SampleType> const highpass0{ 0,0,0,0,0 };

    
    // LR2  cutoff 1000Hz @ fs=44.1 kHz
//    static int const nBiquadSections = 1;
//    static visr::rbbl::BiquadCoefficient<visr::SampleType> const
//    lowpass0{4.4363e-03,   8.8727e-03,   4.4363e-03,    -1.7336e+00,   7.5132e-01};
//    
//    static visr::rbbl::BiquadCoefficient<visr::SampleType> const
//    highpass0{-8.7122e-01,   1.7424e+00,  -8.7122e-01,    -1.7336e+00,   7.5132e-01};
    
    
    
    // LR4  cutoff 1000Hz @ fs=44.1 kHz
//    static int const nBiquadSections = 2;
//    static visr::rbbl::BiquadCoefficient<visr::SampleType> const
//    //lowpass0{2.1197e-05,   4.2394e-05,   2.1197e-05,     -1.7991e+00,   8.1751e-01};
//    lowpass0{ 1,0,0,0,0 };
//    static visr::rbbl::BiquadCoefficient<visr::SampleType> const
//    //lowpass1{1.0000e+00,   2.0000e+00,   1.0000e+00,     -1.7991e+00,   8.1751e-01};
//    lowpass1{ 1,0,0,0,0 };
//    
//    static visr::rbbl::BiquadCoefficient<visr::SampleType> const
//    //highpass0{8.1749e-01,  -1.6350e+00,   8.1749e-01,      -1.7991e+00,   8.1751e-01};
//    highpass0{ 0,0,0,0,0 };
//    static visr::rbbl::BiquadCoefficient<visr::SampleType> const
//    //highpass1{1.0000e+00,  -2.0000e+00,   1.0000e+00,      -1.7991e+00,   8.1751e-01};
//    highpass1{ 0,0,0,0,0 };
    
    
    // LR4  cutoff 1200Hz @ fs=44.1 kHz
//    static int const nBiquadSections = 2;
//    static visr::rbbl::BiquadCoefficient<visr::SampleType> const
//    lowpass0{4.2343e-05,   8.4686e-05,   4.2343e-05,        -1.7592e+00,   7.8523e-01};
//    static visr::rbbl::BiquadCoefficient<visr::SampleType> const
//    lowpass1{1.0000e+00,   2.0000e+00,   1.0000e+00,        -1.7592e+00,   7.8523e-01};
//    
//    static visr::rbbl::BiquadCoefficient<visr::SampleType> const
//    highpass0{7.8518e-01,  -1.5704e+00,   7.8518e-01,        -1.7592e+00,   7.8523e-01};
//    static visr::rbbl::BiquadCoefficient<visr::SampleType> const
//    highpass1{1.0000e+00,  -2.0000e+00,   1.0000e+00,        -1.7592e+00,   7.8523e-01};
    
    
    
    // LR4  cutoff 1400Hz @ fs=44.1 kHz
//    7.5611e-05   1.5122e-04   7.5611e-05   1.0000e+00  -1.7194e+00   7.5422e-01
//    1.0000e+00   2.0000e+00   1.0000e+00   1.0000e+00  -1.7194e+00   7.5422e-01
//    
//    7.5414e-01  -1.5083e+00   7.5414e-01   1.0000e+00  -1.7194e+00   7.5422e-01
//    1.0000e+00  -2.0000e+00   1.0000e+00   1.0000e+00  -1.7194e+00   7.5422e-01
//
    
    
    
    
    // LR8  cutoff 1000Hz @ fs=44.1 kHz
    static int const nBiquadSections = 4;
    static visr::rbbl::BiquadCoefficient<visr::SampleType> const
    lowpass0{4.6315e-10f,   9.2646e-10f,   4.6310e-10f,     -1.7501e+00f,   7.6799e-01f};
    static visr::rbbl::BiquadCoefficient<visr::SampleType> const
    lowpass1{1.0000e+00f,   2.0306e+00f,   1.0311e+00f,     -1.7502e+00f,   7.6812e-01f};
    static visr::rbbl::BiquadCoefficient<visr::SampleType> const
    lowpass2{1.0000e+00f,   1.9997e+00f,   1.0001e+00f,     -1.8776e+00f,   8.9685e-01f};
    static visr::rbbl::BiquadCoefficient<visr::SampleType> const
    lowpass3{1.0000e+00f,   1.9694e+00f,   9.6984e-01f,     -1.8778e+00f,   8.9700e-01f};
    
    
    static visr::rbbl::BiquadCoefficient<visr::SampleType> const
    highpass0{6.8889e-01f,  -1.3781e+00f,   6.8898e-01f,     -1.7501e+00f,   7.6803e-01f};
    static visr::rbbl::BiquadCoefficient<visr::SampleType> const
    highpass1{1.0000e+00f,  -2.0253e+00f,   1.0256e+00f,     -1.7502e+00f,   7.6808e-01f};
    static visr::rbbl::BiquadCoefficient<visr::SampleType> const
    highpass2{1.0000e+00f,  -1.9995e+00f,   9.9987e-01f,     -1.8777e+00f,   8.9689e-01f};
    static visr::rbbl::BiquadCoefficient<visr::SampleType> const
    highpass3{1.0000e+00f,  -1.9747e+00f,   9.7499e-01f,     -1.8777e+00f,   8.9695e-01f};
    
    
    
    // LR8  cutoff 1200Hz @ fs=44.1 kHz
//    static int const nBiquadSections = 4;
//    static visr::rbbl::BiquadCoefficient<visr::SampleType> const
//    lowpass0{1.8591e-09,   3.7189e-09,   1.8589e-09,      -1.7031e+00,   7.2829e-01};
//    static visr::rbbl::BiquadCoefficient<visr::SampleType> const
//    lowpass1{1.0000e+00,   2.0305e+00,   1.0309e+00,      -1.7032e+00,   7.2836e-01};
//    static visr::rbbl::BiquadCoefficient<visr::SampleType> const
//    lowpass2{1.0000e+00,   1.9997e+00,   1.0001e+00,      -1.8503e+00,   8.7770e-01};
//    static visr::rbbl::BiquadCoefficient<visr::SampleType> const
//    lowpass3{1.0000e+00,   1.9695e+00,   9.7000e-01,      -1.8504e+00,   8.7778e-01};
//
//
//    static visr::rbbl::BiquadCoefficient<visr::SampleType> const
//    highpass0{6.3928e-01,  -1.2788e+00,   6.3921e-01,      -1.7030e+00,   7.2825e-01};
//    static visr::rbbl::BiquadCoefficient<visr::SampleType> const
//    highpass1{1.0000e+00,  -2.0305e+00,   1.0309e+00,      -1.7032e+00,   7.2840e-01};
//    static visr::rbbl::BiquadCoefficient<visr::SampleType> const
//    highpass2{1.0000e+00,  -1.9997e+00,   1.0001e+00,      -1.8504e+00,   8.7772e-01};
//    static visr::rbbl::BiquadCoefficient<visr::SampleType> const
//    highpass3{1.0000e+00,  -1.9695e+00,   9.7000e-01,       -1.8504e+00,   8.7776e-01};
    
    
    
        
    
    visr::rbbl::BiquadCoefficientMatrix<visr::SampleType> coeffMatrix( 2*numberOfInputs, nBiquadSections );
    for( std::size_t chIdx(0); chIdx < numberOfInputs; ++chIdx )
    {
        coeffMatrix( chIdx, 0 ) = lowpass0;
        coeffMatrix( chIdx + numberOfInputs, 0 ) = highpass0;
        
        coeffMatrix( chIdx, 1 ) = lowpass1;
        coeffMatrix( chIdx + numberOfInputs, 1 ) = highpass1;
        
        coeffMatrix( chIdx, 2 ) = lowpass2;
        coeffMatrix( chIdx + numberOfInputs, 2 ) = highpass2;
        
        coeffMatrix( chIdx, 3 ) = lowpass3;
        coeffMatrix( chIdx + numberOfInputs, 3 ) = highpass3;
    }
    
    
    mPanningFilterbank->setup( 2*numberOfInputs, nBiquadSections, coeffMatrix );
    
    audioConnection( mInput, ChannelRange( 0, numberOfInputs ),
                     mPanningFilterbank->audioPort("in"), ChannelRange( 0, numberOfInputs ) );
    audioConnection( mInput, ChannelRange( 0, numberOfInputs ),
                     mPanningFilterbank->audioPort("in"), ChannelRange( numberOfInputs, 2*numberOfInputs ) );
    

    
    
    
    // Setup and connect LF CAP panning
    
    gainMatrixLow->setup( numberOfInputs, numberOfOutputs, interpolationSteps, 0.0f );
    
    parameterConnection( objectInput, capCalculator.parameterPort("objectVectorInput") );
    
    parameterConnection( listenerInput, capCalculator.parameterPort("listenerPosition") ); // head centre position and orientation
    
    parameterConnection( capCalculator.parameterPort("gainOutput"), gainMatrixLow->parameterPort("gainInput"));
    
    
    
    audioConnection( mPanningFilterbank->audioPort("out"), ChannelRange( 0, numberOfInputs ),
        gainMatrixLow->audioPort("in"), ChannelRange( 0, numberOfInputs ) );
    

    
    
    
    // Setup and connect HF VBIP panning  (also CAP HF panning in CAP HF mode)
    gainMatrixHigh.setup( numberOfInputs, numberOfOutputs, interpolationSteps, 0.0f );

    parameterConnection( capCalculator.parameterPort("hfGainOutput"), gainMatrixHigh.parameterPort("gainInput"));

    audioConnection( mPanningFilterbank->audioPort("out"), ChannelRange( numberOfInputs, 2*numberOfInputs ),
                        gainMatrixHigh.audioPort("in"), ChannelRange( 0, numberOfInputs ) );

    // Sum processed low and high frequency signals.
    audioConnection( gainMatrixLow->audioPort("out"), mCombiner.audioPort("in0"));
    audioConnection( gainMatrixHigh.audioPort("out"), mCombiner.audioPort("in1"));
    
    
    
    
    // Setup and connect listener position compensation

    mListenerCompensation.reset( new ListenerCompensationCAP( context, "TrackingListenerCompensation", this, arrayConfig ) );
    mListenerGainDelayCompensation.reset( new visr::rcl::DelayVector( context, "ListenerGainDelayCompensation", this ) );

    // for the moment, do not parse any options, but use hard-coded option values.
    visr::SampleType const cMaxDelay = 1.0f; // maximum delay (in seconds)

    // Initial gain of 0.0 to suppress transients on startup.
    mListenerGainDelayCompensation->setup( numberOfOutputs, period(), cMaxDelay, "lagrangeOrder1",
                                          visr::rcl::DelayVector::MethodDelayPolicy::Limit,
                                          visr::rcl::DelayVector::ControlPortConfig::All, 0.0f, 1.0f );
    parameterConnection( listenerInput, mListenerCompensation->parameterPort("positionInput") );
    parameterConnection( mListenerCompensation->parameterPort("delayOutput"), mListenerGainDelayCompensation->parameterPort("delayInput") );
    parameterConnection( mListenerCompensation->parameterPort("gainOutput"), mListenerGainDelayCompensation ->parameterPort("gainInput") );
    
    audioConnection( mCombiner.audioPort("out"), mListenerGainDelayCompensation->audioPort("in") );
    
    
    
    // Final output
    audioConnection( mListenerGainDelayCompensation->audioPort("out"), mOutput );
    
    
    
    
    // Temp connections
    
//    audioConnection( mInput, ChannelRange( 0, numberOfInputs ),
//                     gainMatrixLow->audioPort("in"), ChannelRange( 0, numberOfInputs ) );
//   
//    audioConnection( gainMatrixLow->audioPort("out"),
//                     mListenerGainDelayCompensation->audioPort("in") );
    
//    audioConnection( mInput, ChannelRange( 0, numberOfInputs ),
//                     gainMatrixHigh.audioPort("in"), ChannelRange( 0, numberOfInputs ) );
//
//    audioConnection( gainMatrixHigh.audioPort("out"),
//                     mListenerGainDelayCompensation->audioPort("in") );
    
    
//    audioConnection(  gainMatrixLow->audioPort("out"),
//                      mOutput );
    
//    audioConnection( mInput, ChannelRange( 0, 1 ),  // start, end (idx+1)
//                     mOutput, ChannelRange( 1, 2 ) );
//    audioConnection( mInput, ChannelRange( 0, 1 ),
//                     mOutput, ChannelRange( 0, 1  ) );
    
//    audioConnection( mInput, mOutput );
    
}


DualbandCapRenderer::~DualbandCapRenderer() = default;

} // namespace cap_renderer
} // namespace dawplugins
} // namespace visr_production_suite
