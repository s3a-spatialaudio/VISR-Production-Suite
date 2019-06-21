#include "PluginProcessor.h"
#include "PluginEditor.h"

#include <libdawutilities/quasiAllpassFIR_f64_n63_initializer_list.h>
#include <libpml/initialise_parameter_library.hpp>

#include <libpythonsupport/gil_ensure_guard.hpp>
#include <libpythonsupport/initialisation_guard.hpp>
#include <libpythoncomponents/wrapper.hpp>

#include <boost/format.hpp>

using namespace visr;
using namespace juce;


// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new visr::dawplugins::multi_renderer::Processor();
}


namespace visr
{
namespace dawplugins
{
namespace multi_renderer
{
    
Processor::Processor():
PluginProcessorRender(BusesProperties()
                          .withInput ("Input",  AudioChannelSet::stereo(), true)
                          .withOutput ("Output", AudioChannelSet::stereo(), true))
{
    
    maxInputChannels = 64;
    
    parameters.createAndAddParameter (IDs::listenerPosX.toString(), IDs::listenerPosX.toString(), IDs::listenerPosX.toString(),
                                      NormalisableRange<float> (-10,10,0.01f), 0, nullptr,nullptr,false,false,true);
    
    parameters.createAndAddParameter (IDs::listenerPosY.toString(), IDs::listenerPosY.toString(), IDs::listenerPosY.toString(),
                                      NormalisableRange<float> (-10,10,0.01f), 0, nullptr,nullptr,false,false,true);
    
    parameters.createAndAddParameter (IDs::listenerPosZ.toString(), IDs::listenerPosZ.toString(), IDs::listenerPosZ.toString(),
                                      NormalisableRange<float> (-10,10,0.01f), 0, nullptr,nullptr,false,false,true);
    
    parameters.createAndAddParameter (IDs::yaw.toString(), IDs::yaw.toString(), IDs::yaw.toString(),
                                      NormalisableRange<float> (-180.0,180.0, 0.01), 0, nullptr,nullptr,false,false,true);
    
    parameters.createAndAddParameter (IDs::pitch.toString(), IDs::pitch.toString(), IDs::pitch.toString(),
                                      NormalisableRange<float> (-90,90, 0.01), 0, nullptr,nullptr,false,false,true);
    
    parameters.createAndAddParameter (IDs::roll.toString(), IDs::roll.toString(), IDs::roll.toString(),
                                      NormalisableRange<float> (-180,180, 0.01), 0, nullptr,nullptr,false,false,true);
    
    
    parameters.state = ValueTree (Identifier ("MultiRenderer"));
    parameters.state.addListener(this);
    
    rendNames[VBAP_NOVH] = "VBAP Standard";
    rendNames[VBAP_VH] = "VBAP Virtual Height";
    rendNames[VBAP_NOSD] = "VBAP Stereo";
    rendNames[VBAP_SD] = "VBAP Stereo Downmix";
    rendNames[CAP] = "Compensated Amplitude Panning";
    rendNames[BINAURAL_STATIC] = "Static Binaural";
    rendNames[BINAURAL_DYNAMIC] = "Dynamic Binaural";
    
    //    renderers.resize(4);
}
void Processor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    
    {
        // Ensure that the current thread is initialized for using the Python C API.
        // The GIL is released immediately as the scope is left.
        visr::pythonsupport::GilEnsureGuard guard;
    }
    
    
    //FOR STANDALONE VERSION, to be deleted
    if(wrapperType==wrapperType_Standalone){
        buffer.setSize(maxOutputChannels, buffer.getNumSamples());
    }
    
    if (!setupDone)
    {
        buffer.clear();
    } else {
        
        inputBuffer.clear();
        
        for (int i=0; i < maxInputChannels; i++) {
            inputBuffer.copyFrom(i, 0, buffer, i, 0, buffer.getNumSamples());
        }
        
        buffer.clear();
        
        // call process function of the toplevel component, through the VISRComponentWrapper
        
        
        rt = static_cast<RendererType> ((int)parameters.state.getProperty("RendererType", 0));
        try
        {
            
            componentHandler->process(inputBuffer.getArrayOfReadPointers(),buffer.getArrayOfWritePointers());
            //componentHandler->processS3aButton(inputBuffer.getArrayOfReadPointers(), buffer.getArrayOfWritePointers());
            
        }catch( std::exception const &ex )
        {
            String errorMsg;
            errorMsg << "Exception in audio callback\n" << ex.what();
            parameters.state.getOrCreateChildWithName("Error",nullptr).setProperty("ErrorMsg", errorMsg, nullptr);
            std::cerr << errorMsg << std::endl;
            sendChangeMessage();
        }
        
        if(rt == BINAURAL_DYNAMIC){
            
            reroutingBuffer.clear();
            for (int i=0; i < 2; i++) {
                reroutingBuffer.copyFrom(i+62, 0, buffer, i, 0, buffer.getNumSamples());
            }
            buffer.clear();
            for (int i=0; i < maxOutputChannels; i++) {
                buffer.copyFrom(i, 0, reroutingBuffer, i, 0, reroutingBuffer.getNumSamples());
            }
        }
        else if(rt == BINAURAL_STATIC){
            
            reroutingBuffer.clear();
            for (int i=0; i < 2; i++) {
                reroutingBuffer.copyFrom(i+60, 0, buffer, i, 0, buffer.getNumSamples());
            }
            buffer.clear();
            for (int i=0; i < maxOutputChannels; i++) {
                buffer.copyFrom(i, 0, reroutingBuffer, i, 0, reroutingBuffer.getNumSamples());
            }
        }
    }
    
    
}

Processor::~Processor()
{
}

void Processor::prepareToPlay (double sampleRate, int period)
{
    if(isFirst || sampleRate != sampleRate_ || period!= period_){
        sampleRate_ = sampleRate;
        period_ = period;

        inputBuffer = AudioSampleBuffer(maxInputChannels, period);
        PluginContextChanged();
        PluginReset();
        isFirst=false;
    }
}

void Processor::resetRenderer(){
    int conf = (int)parameters.state.getProperty("RendererType", 0);
    loudspeakerArray = renderersLdspk[conf];
    componentHandler->update();
}

void Processor::PluginSetup()
{
    parameters.state.getOrCreateChildWithName("Error",nullptr).removeAllProperties(nullptr);
    //    rt = static_cast<RendererType> ((int)parameters.state.getProperty("RendererType", 0));
    //
    //    switch(rt){
    //
    //        case(VBAP_NOVH):
    //
    //            break;
    //
    //        case(VBAP_VH):
    //            break;
    //        case(VBAP_SD):
    //            break;
    //        case(CAP):
    //            break;
    //        default:
    //            break;
    //    }
    //
    //
    //
    //
    //    sendChangeMessage(); // notify editor
    //
    //
    //    setupDone = true;
    
    
    //        pythonsupport::InitialisationGuard::initialise();
    
    //        if(wrapperType != wrapperType_VST3){
    setChannelLayoutOfBus(false, 0, AudioChannelSet::discreteChannels(64));
    setChannelLayoutOfBus(true, 0, AudioChannelSet::discreteChannels(64));
    
    
    //  std::cout<<" supported "<<res<<" chanset "<<getBusesLayout().getNumChannels(false, 0)<<std::endl;
    //            setPlayConfigDetails(64, 64, sampleRate, period);
    //            processorLayoutsChanged();
    //            numChannelsChanged();
//    String tempout = getChannelLayoutOfBus(false, 0).getSpeakerArrangementAsString();
//    int temp = getTotalNumOutputChannels();
    //        std::cout<<"conf "<<tempout<<" numtot "<<temp<<std::endl;
    //        }
    reroutingBuffer = AudioSampleBuffer(getTotalNumOutputChannels(), context->period());
    
    /* Set up the filter matrix for the diffusion filters. */
    std::size_t const diffusionFilterLength = 63; // fixed filter length of the filters in the compiled-in matrix
    std::size_t const diffusionFiltersInFile = 64; // Fixed number of filters in file.
    
    // First create a filter matrix containing all filters from a initializer list that is compiled into the program.
    efl::BasicMatrix<visr::SampleType> allDiffusionCoeffs( diffusionFiltersInFile,
                                                          diffusionFilterLength,
                                                          quasiAllpassFIR_f64_n63_initializer_list,
                                                          cVectorAlignmentSamples );
    
    std::string reverbConfig ="{ \"numReverbObjects\": 1, \"discreteReflectionsPerObject\": 20, \"lateReverbFilterLength\": 2, \"lateReverbDecorrelationFilters\": \"/Applications/VISR-0.9.0/config/filters/random_phase_allpass_64ch_512taps.wav\"}";
    
    //        File custom("/Users/giacomocostantini/VISR/visr/config/isvr/audiolab_22speakers.xml");
    
    //        if(custom.exists())
    //            res = VISRCheckLayout(custom);
    //        else
    VISRCheckLayout(bs2051_0_5_0);
    loudspeakerArray = ldspkcheck;
    maxOutputChannels = requestedChannels;
    if(wrapperType==wrapperType_Standalone){
        maxInputChannels = requestedChannels;
    }else{
        
    }
    inputBuffer = AudioSampleBuffer(maxInputChannels, context->period());
    
    
    // Create a second filter matrix that matches the number of required filters.
    
    efl::BasicMatrix<SampleType> diffusionCoeffs( loudspeakerArray.getNumRegularSpeakers(), diffusionFilterLength, cVectorAlignmentSamples );
    for( std::size_t idx( 0 ); idx < diffusionCoeffs.numberOfRows( ); ++idx )
    {
        efl::vectorCopy( allDiffusionCoeffs.row( idx ), diffusionCoeffs.row( idx ), diffusionFilterLength, cVectorAlignmentSamples );
    }
    
    /********************************* SETTING TOP LEVEL COMPONENT AND ITS CALLBACK  **********************************/
    // Assume a fixed length for the interpolation period.
    // Ideally, this roughly matches the update rate of the scene sender.
    //  const std::size_t cInterpolationLength = std::max( static_cast<std::size_t>(2048), period );
    pml::initialiseParameterLibrary();
    
    renderersLdspk[VBAP_NOVH] = loudspeakerArray;
    
    
    
    renderers[VBAP_NOVH].reset(new dawsupport::visr_components::MetadataExposingRenderer( *context,
                                                            "SceneRendererVBAP",
                                                            nullptr,
                                                            loudspeakerArray,
                                                            maxInputChannels,
                                                            maxOutputChannels,
                                                            parameterUpdatePeriod,
                                                            diffusionCoeffs,
                                                            "",
                                                            8001,
                                                            //                                                    6001,
                                                            0,
                                                            reverbConfig,
                                                            false
                                                            ));
    
    
    //        res = VISRCheckLayout(bs2051_0_5_0);
    
    
    File custom("/Users/giacomocostantini/VISR/visr/config/generic/bs2051-0+5+0_virtual_height.xml");
    
    if(custom.exists())
        VISRCheckLayout(custom);
    else
        VISRCheckLayout(stereo);
    
    loudspeakerArray = ldspkcheck;
    maxOutputChannels = requestedChannels;
    if(wrapperType==wrapperType_Standalone){
        maxInputChannels = requestedChannels;
    }else{
        
    }
    inputBuffer = AudioSampleBuffer(maxInputChannels, context->period());
    
    
    // Create a second filter matrix that matches the number of required filters.
    efl::BasicMatrix<SampleType> diffusionCoeffs2( loudspeakerArray.getNumRegularSpeakers(), diffusionFilterLength, cVectorAlignmentSamples );
    for( std::size_t idx( 0 ); idx < diffusionCoeffs.numberOfRows( ); ++idx )
    {
        efl::vectorCopy( allDiffusionCoeffs.row( idx ), diffusionCoeffs.row( idx ), diffusionFilterLength, cVectorAlignmentSamples );
    }
    
    
    renderersLdspk[VBAP_VH] = loudspeakerArray;
    renderers[VBAP_VH].reset(new dawsupport::visr_components::MetadataExposingRenderer( *context,
                                                          "SceneRendererVBAP_VH",
                                                          nullptr,
                                                          loudspeakerArray,
                                                          maxInputChannels,
                                                          maxOutputChannels,
                                                          parameterUpdatePeriod,
                                                          diffusionCoeffs,
                                                          "",
                                                          8001,
                                                          //                                                       6001,
                                                          0,
                                                          reverbConfig,
                                                          false
                                                          ));
    
    
    VISRCheckLayout(stereo);
    
    loudspeakerArray = ldspkcheck;
    
    maxOutputChannels = requestedChannels;
    if(wrapperType==wrapperType_Standalone){
        maxInputChannels = requestedChannels;
    }else{
        
    }
    inputBuffer = AudioSampleBuffer(maxInputChannels, context->period());
    
    
    // Create a second filter matrix that matches the number of required filters.
    efl::BasicMatrix<SampleType> diffusionCoeffs3( loudspeakerArray.getNumRegularSpeakers(), diffusionFilterLength, cVectorAlignmentSamples );
    for( std::size_t idx( 0 ); idx < diffusionCoeffs.numberOfRows( ); ++idx )
    {
        efl::vectorCopy( allDiffusionCoeffs.row( idx ), diffusionCoeffs.row( idx ), diffusionFilterLength, cVectorAlignmentSamples );
    }
    
    
    renderersLdspk[VBAP_NOSD] = loudspeakerArray;
    renderers[VBAP_NOSD].reset(new dawsupport::visr_components::MetadataExposingRenderer( *context,
                                                            "SceneRendererVBAP_NOSD",
                                                            nullptr,
                                                            loudspeakerArray,
                                                            maxInputChannels,
                                                            maxOutputChannels,
                                                            parameterUpdatePeriod,
                                                            diffusionCoeffs,
                                                            "",
                                                            8001,
                                                            //                                                       6001,
                                                            0,
                                                            reverbConfig,
                                                            false
                                                            ));
    
    
    VISRCheckLayout(stereo);
    
    
    loudspeakerArray = ldspkcheck;
    
    maxOutputChannels = requestedChannels;
    if(wrapperType==wrapperType_Standalone){
        maxInputChannels = requestedChannels;
    }else{
        
    }
    inputBuffer = AudioSampleBuffer(maxInputChannels, context->period());
    
    
    // Create a second filter matrix that matches the number of required filters.
    efl::BasicMatrix<SampleType> diffusionCoeffs4( loudspeakerArray.getNumRegularSpeakers(), diffusionFilterLength, cVectorAlignmentSamples );
    for( std::size_t idx( 0 ); idx < diffusionCoeffs.numberOfRows( ); ++idx )
    {
        efl::vectorCopy( allDiffusionCoeffs.row( idx ), diffusionCoeffs.row( idx ), diffusionFilterLength, cVectorAlignmentSamples );
    }
    
    //            std::string configpath;
    //
    //            if(metaconfig.exists())
    //                configpath = metaconfig.getFullPathName().toStdString();
    //            else
    //                configpath = "";
    
    pythonsupport::InitialisationGuard::initialise();
    
    
    //            std::string const formatString = "{'processorConfig': '%s', 'objectVectorInput': True, 'objectVectorOutput': True, 'oscControlPort': False, 'jsonControlPort': False }";
    //            std::string const kwArgs = str( boost::format( formatString ) % metaconfig );
    //
    //            loudspeakerArray,
    //            maxInputChannels,
    //            maxOutputChannels,
    //            parameterUpdatePeriod,
    //            diffusionCoeffs,
    //            "",
    //            8001,
    //            //                                                       6001,
    //            0,
    //            reverbConfig,
    //            false,
    //            configpath
    //
    //
    //            diffusionFilter = os.path.join( configBasePath, 'filters/random_phase_allpass_64ch_512taps.wav' )
    //
    
    //            static char const * kwArgsTemplate = "{ 'loudspeakerConfigurationFile': \"%s\", 'numberOfInputs': %d, 'numberOfOutputs': %d,  'interpolationPeriod': %d, 'diffusionFiltersFile': \"%s\", 'trackingConfiguration': \"\", 'numberOfObjectEqSections': 0, 'reverbConfigFile': \"%s\", 'frequencyDependentPanning': False ,'metadapterConfigFile': \"%s\" }";
    //
    //            auto const kwArgs = (boost::format( kwArgsTemplate )
    //                                 % stereoConf % maxInputChannels % maxOutputChannels % parameterUpdatePeriod % diffusionConf % reverbConfig % metaconfig).str();
    //
    //
    std::string const metaconfig = "/Users/giacomocostantini/METADAPTER/metadapter/config/semantic_stereo.xml";
    
    std::string const stereoConf = "/Users/giacomocostantini/VISR/visr/config/surrey/stereo.xml";
    std::string const diffusionConf ="/Users/giacomocostantini/VISR/visr/config/filters/random_phase_allpass_64ch_512taps.wav";
    
    
//    std::size_t const numInputs = maxInputChannels;
//    std::size_t const numOutputs = maxOutputChannels;
    //            static char const * kwArgsTemplate = "{ 'loudspeakerConfigurationFile': \"%s\", 'numberOfInputs': %d, 'numberOfOutputs': %d, 'interpolationPeriod': %d, 'diffusionFiltersFile': \"%s\" }";
    
    //            auto const kwArgs = (boost::format( kwArgsTemplate )
    //                                 % stereoConf % numInputs % numOutputs % parameterUpdatePeriod % diffusionConf).str();
    //
    
    try{
        
        
        static char const * kwArgsTemplate = "{ 'metadapterConfigFile': \"%s\" }";
        auto const kwArgs = (boost::format( kwArgsTemplate )
                             % metaconfig).str();
        
        std::string posargs = "'"+stereoConf+"',"+std::to_string(maxInputChannels)+","+std::to_string(maxOutputChannels)+","+std::to_string(parameterUpdatePeriod)+",'"+diffusionConf+"'";
        
        
        renderersLdspk[VBAP_SD] = loudspeakerArray;
        renderers[VBAP_SD].reset(new pythoncomponents::Wrapper( *context,
                                                               "SceneRendererVBAP_SD",
                                                               nullptr,
                                                               "semanticStereo",
                                                               "SemanticStereo",
                                                               posargs.c_str(),
                                                               kwArgs.c_str(),   //  char const * keywordArguments = "",
                                                               ""                // No module search path
                                                               ));
        
        
        
    }
    catch( std::exception const &ex )
    {
        String errorMsg;
        errorMsg << "Exception while instantiating Python wrapper:\n" << ex.what();
        parameters.state.getOrCreateChildWithName("Error",nullptr).setProperty("ErrorMsg", errorMsg, nullptr);
        std::cerr << errorMsg << std::endl;
        setupDone = false;
        return;
    }
    //            renderers[VBAP_SD].reset(new MetadataExposingRenderer( *context,
    //                                                                    "SceneRendererVBAP_SD",
    //                                                                    nullptr,
    //                                                                    loudspeakerArray,
    //                                                                    maxInputChannels,
    //                                                                    maxOutputChannels,
    //                                                                    parameterUpdatePeriod,
    //                                                                    diffusionCoeffs,
    //                                                                    "",
    //                                                                    8001,
    //                                                                    //                                                       6001,
    //                                                                    0,
    //                                                                    reverbConfig,
    //                                                                    false
    //                                                                    ));
    
    
    //        res = VISRCheckLayout(dolby_7_1);
    VISRCheckLayout(capLdspArray2);
    
    loudspeakerArray = ldspkcheck;
    
    maxOutputChannels = requestedChannels;
    if(wrapperType==wrapperType_Standalone){
        maxInputChannels = requestedChannels;
    }else{
        
    }
    inputBuffer = AudioSampleBuffer(maxInputChannels, context->period());
    
    
    // Create a second filter matrix that matches the number of required filters.
    efl::BasicMatrix<SampleType> diffusionCoeffs5( loudspeakerArray.getNumRegularSpeakers(), diffusionFilterLength, cVectorAlignmentSamples );
    for( std::size_t idx( 0 ); idx < diffusionCoeffs.numberOfRows( ); ++idx )
    {
        efl::vectorCopy( allDiffusionCoeffs.row( idx ), diffusionCoeffs.row( idx ), diffusionFilterLength, cVectorAlignmentSamples );
    }
    
    
    renderersLdspk[CAP] = loudspeakerArray;
    
    renderers[CAP].reset(new dawsupport::visr_components::DualbandCapRenderer( *context,
                                                 "SceneRendererCAP",
                                                 nullptr,
                                                 maxInputChannels,
                                                 loudspeakerArray
                                                 ));
    //
    //            pythonsupport::InitialisationGuard::initialise();
    //            std::string posargs = "1,'"+defaultLdspArray+"',";
    //            renderers[CAP].reset(new pythoncomponents::Wrapper( *context,
    //                                                                  "SceneRendererCAP", // component name (can be chosen freely)
    //                                                                  nullptr,            // parent component (this is top level)
    //                                                                  "dualband_cap_renderer",      // Name of Python module
    //                                                                  "DualbandCapRenderer",    // Class name (definition must be in Python module above)
    //                                                                  posargs.c_str(), // Positional arguments passed to the class's init__()           method
    //                                                                  "",                 // Keyword arguments passed to the class's init__() method
    //                                                                  ""             // Optional module search path
    //                                                                  ));
    
    
    
    
    maxInputChannels = getBusesLayout().getNumChannels(true, 0);
    maxOutputChannels = getBusesLayout().getNumChannels(false, 0);
    inputBuffer = AudioSampleBuffer(maxInputChannels, context->period() );
    
    // Create a second filter matrix that matches the number of required filters.
    efl::BasicMatrix<SampleType> diffusionCoeffs6( loudspeakerArray.getNumRegularSpeakers(), diffusionFilterLength, cVectorAlignmentSamples );
    for( std::size_t idx( 0 ); idx < diffusionCoeffs.numberOfRows( ); ++idx )
    {
        efl::vectorCopy( allDiffusionCoeffs.row( idx ), diffusionCoeffs.row( idx ), diffusionFilterLength, cVectorAlignmentSamples );
    }
    
    
    
    /********************************* SETUP JUCE-TOPLEVELCOMPONENT wrapper  **********************************/
    try
    {
        
        std::size_t const numInputs = 64;
        bool const dynamicHeadTracking = false;
        
        //            std::string const sofaFile = "/Users/giacomocostantini/Dropbox/S3A/koln/HRIR_L2354_original.sofa";
        //            static char const * kwArgsTemplate = "{ 'numberOfObjects': %d, 'sofaFile': \"%s\", 'headTracking': %d, 'dynamicITD': %d, 'hrirInterpolation': False, 'dynamicILD': False   }";
        //
        //            auto const kwArgs = (boost::format( kwArgsTemplate )
        //                                 % numInputs % sofaFile % dynamicHeadTracking % dynamicITD ).str();
        
        //            HoaObjectToBinauralRenderer
        std::size_t const maxHOAorder = 3;
        std::string const sofaFile = "/Users/giacomocostantini/SOFA/bbc_hoa2bin_sofa/Gauss_O"+std::to_string(maxHOAorder)+"_ku100_dualband_energy.sofa";
        
        
        //            , 'fftImplementation' : 'ffts'
        static char const * kwArgsTemplate = "{ 'numberOfObjects': %d, 'maxHoaOrder': %d, 'sofaFile': \"%s\", 'headTracking': %d, 'fftImplementation' : 'ffts'  }";
        
        auto const kwArgs = (boost::format( kwArgsTemplate )
                             % numInputs % maxHOAorder % sofaFile % dynamicHeadTracking).str();
        
        
        loudspeakerArray.loadXmlString(stereo);
        renderersLdspk[BINAURAL_STATIC] = loudspeakerArray;
        renderers[BINAURAL_STATIC].reset(new pythoncomponents::Wrapper( *context,
                                                                       "BSTStatic",              // component name (can be chosen freely)
                                                                       nullptr,            // parent component (nulptr because this is top level)
                                                                       "visr_bst",         // Name of Python module
                                                                       "HoaObjectToBinauralRenderer", // Class name (definition must be in Python module above)
                                                                       "", // Positional arguments passed to the class's init__()           method
                                                                       kwArgs.c_str() ,                 // Keyword arguments passed to the class's init__() method
                                                                       ""             // Optional module search path
                                                                       ));
    }
    catch( std::exception const &ex )
    {
        String errorMsg;
        errorMsg << "Exception while instantiating Python wrapper:\n" << ex.what();
        parameters.state.getOrCreateChildWithName("Error",nullptr).setProperty("ErrorMsg", errorMsg, nullptr);
        std::cerr << errorMsg << std::endl;
        setupDone = false;
        return;
    }
    
    /********************************* SETUP JUCE-TOPLEVELCOMPONENT wrapper  **********************************/
    try
    {
        std::string const sofaFile = "/Users/giacomocostantini/SOFA/koln/HRIR_L2354_original.sofa";
        std::size_t const numInputs = 64;
//        bool const dynamicHeadTracking = false;
        bool const dynamicITD = false;
        std::string headTrackingKeywordArguments = "{'port': '/dev/cu.usbserial-AJ03GR8O', 'calibrationPort': 8889 }";
        std::string const headTrackingReceiverClassName = "RazorAHRSWithUdpCalibrationTrigger";
        
        
        std::string kwArgsTemplate = "{ 'numberOfObjects': %d, 'sofaFile': \"%s\", 'dynamicITD': %d, 'headTrackingReceiver': "+headTrackingReceiverClassName+", 'hrirInterpolation': False, 'dynamicILD': False, 'fftImplementation' : 'ffts', 'headTrackingKeywordArguments': "+headTrackingKeywordArguments+"}";
        //            , 'headTrackingReceiver': .tracker.RazorAHRSWithUdpCalibrationTrigger
        //            , 'headTrackingKeywordArguments': "+headTrackingKeywordArguments+"
        auto const kwArgs = (boost::format( kwArgsTemplate )
                             % numInputs % sofaFile % dynamicITD ).str();
        
        loudspeakerArray.loadXmlString(stereo);
        renderersLdspk[BINAURAL_DYNAMIC] = loudspeakerArray;
        renderers[BINAURAL_DYNAMIC].reset(new pythoncomponents::Wrapper( *context,
                                                                        "BST",              // component name (can be chosen freely)
                                                                        nullptr,            // parent component (nulptr because this is top level)
                                                                        "visr_bst",         // Name of Python module
                                                                        "RealtimeDynamicHrirRenderer", // Class name (definition must be in Python module above)
                                                                        "", // Positional arguments passed to the class's init__()           method
                                                                        kwArgs.c_str() ,                 // Keyword arguments passed to the class's init__() method
                                                                        ""             // Optional module search path
                                                                        ));
    }
    catch( std::exception const &ex )
    {
        String errorMsg;
        errorMsg << "Exception while instantiating Python wrapper:\n" << ex.what();
        parameters.state.getOrCreateChildWithName("Error",nullptr).setProperty("ErrorMsg", errorMsg, nullptr);
        std::cerr << errorMsg << std::endl;
        setupDone = false;
        return;
    }
    
    
    componentHandler.reset(new VISRComponentWrapperMulti(parameters, renderers, REND_MAX));
    
    componentHandler->addParameterInput(IDs::listenerPosX.toString(), "");
    componentHandler->addParameterInput(IDs::listenerPosY.toString(), "");
    componentHandler->addParameterInput(IDs::listenerPosZ.toString(), "");
    componentHandler->addParameterInput(IDs::yaw.toString(), "");
    componentHandler->addParameterInput(IDs::pitch.toString(), "");
    componentHandler->addParameterInput(IDs::roll.toString(), "");
   
    
    setupDone = true;
    
    sendChangeMessage(); // notify editor
    
}

void Processor::PluginUnload()
{
    // for each componentHandler->addParameterInput there should be a corresponding removeParameterListener
    if (setupDone) {
        if(componentHandler!=nullptr){
            //            parameters.removeParameterListener(IDs::objID.getCharPointer(), componentHandler.get());
            //            parameters.removeParameterListener(azParamName, componentHandler.get());
            //            parameters.removeParameterListener(elParamName, componentHandler.get());
            componentHandler.reset();
        }
        setupDone = false;
    }
}
void Processor::PluginContextChanged()
{
    context = new visr::SignalFlowContext(period_,static_cast<visr::SamplingFrequencyType>(sampleRate_));
}


AudioProcessorEditor* Processor::createEditor()
{
    return new Editor (this, new PannerUI(this),34);
}

//==============================================================================

void Processor::valueTreePropertyChanged (ValueTree&, const Identifier&){
    sendChangeMessage();
}

//void Processor::valueTreeChildAdded (ValueTree& par, ValueTree& ch) {
//    CachedValue<float> az,el;
//    az.referTo(ch, "az", nullptr);
//    el.referTo(ch, "el", nullptr);
//    JUCEObjectVector.add(az,el);
//}

    
} // namespace multi_renderer
} // namespace dawplugins
} // namespace visr
