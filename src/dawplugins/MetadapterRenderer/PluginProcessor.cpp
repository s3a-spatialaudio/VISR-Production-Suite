#include "PluginProcessor.h"
#include "PluginEditor.h"

#include <libcore/quasiAllpassFIR_f64_n63_initializer_list.h>
#include <libpml/initialise_parameter_library.hpp>

#include <libpythonsupport/gil_ensure_guard.hpp>
#include <libpythonsupport/initialisation_guard.hpp>
#include <libpythoncomponents/wrapper.hpp>

#include <boost/filesystem/path.hpp>
#include "CurrentDefaultPaths.h"

using namespace visr;
using namespace juce;

// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
return new visr_production_suite::dawplugins::metadapter_renderer::Processor();
}

namespace visr_production_suite
{
namespace dawplugins
{
namespace metadapter_renderer
{

Processor::Processor():
PluginProcessorLoudspeakers(BusesProperties()
                      .withInput ("Object1",  AudioChannelSet::mono())
                      .withInput ("Object2",  AudioChannelSet::mono())
                      .withInput ("Object3",  AudioChannelSet::mono())
                      .withInput ("Object4",  AudioChannelSet::mono())
                      .withInput ("Object5",  AudioChannelSet::mono())
                      .withInput ("Object6",  AudioChannelSet::mono())
                      .withInput ("Object7",  AudioChannelSet::mono())
                      .withInput ("Object8",  AudioChannelSet::mono())
                      .withInput ("Object9",  AudioChannelSet::mono())
                      .withInput ("Object10",  AudioChannelSet::mono())
                      .withInput ("Object11",  AudioChannelSet::mono())
                      .withInput ("Object12",  AudioChannelSet::mono())
                      .withInput ("Object13",  AudioChannelSet::mono())
                      .withInput ("Object14",  AudioChannelSet::mono())
                      .withInput ("Object15",  AudioChannelSet::mono())
                      .withInput ("Object16",  AudioChannelSet::mono())
                      .withInput ("Object17",  AudioChannelSet::mono())
                      .withInput ("Object18",  AudioChannelSet::mono())
                      .withInput ("Object19",  AudioChannelSet::mono())
                      .withInput ("Object20",  AudioChannelSet::mono())
                      .withInput ("Object21",  AudioChannelSet::mono())
                      .withInput ("Object22",  AudioChannelSet::mono())
                      .withInput ("Object23",  AudioChannelSet::mono())
                      .withInput ("Object24",  AudioChannelSet::mono())
                      .withInput ("Object25",  AudioChannelSet::mono())
                      .withInput ("Object26",  AudioChannelSet::mono())
                      .withInput ("Object27",  AudioChannelSet::mono())
                      .withInput ("Object28",  AudioChannelSet::mono())
                      .withInput ("Object29",  AudioChannelSet::mono())
                      .withInput ("Object30",  AudioChannelSet::mono())
                      .withInput ("Object31",  AudioChannelSet::mono())
                      .withInput ("Object32",  AudioChannelSet::mono())
                      .withInput ("Object33",  AudioChannelSet::mono())
                      .withInput ("Object34",  AudioChannelSet::mono())
                      .withInput ("Object35",  AudioChannelSet::mono())
                      .withInput ("Object36",  AudioChannelSet::mono())
                      .withInput ("Object37",  AudioChannelSet::mono())
                      .withInput ("Object38",  AudioChannelSet::mono())
                      .withInput ("Object39",  AudioChannelSet::mono())
                      .withInput ("Object40",  AudioChannelSet::mono())
                      .withInput ("Object41",  AudioChannelSet::mono())
                      .withInput ("Object42",  AudioChannelSet::mono())
                      .withInput ("Object43",  AudioChannelSet::mono())
                      .withInput ("Object44",  AudioChannelSet::mono())
                      .withInput ("Object45",  AudioChannelSet::mono())
                      .withInput ("Object46",  AudioChannelSet::mono())
                      .withInput ("Object47",  AudioChannelSet::mono())
                      .withInput ("Object48",  AudioChannelSet::mono())
                      .withInput ("Object49",  AudioChannelSet::mono())
                      .withInput ("Object50",  AudioChannelSet::mono())
                      .withInput ("Object51",  AudioChannelSet::mono())
                      .withInput ("Object52",  AudioChannelSet::mono())
                      .withInput ("Object53",  AudioChannelSet::mono())
                      .withInput ("Object54",  AudioChannelSet::mono())
                      .withInput ("Object55",  AudioChannelSet::mono())
                      .withInput ("Object56",  AudioChannelSet::mono())
                      .withInput ("Object57",  AudioChannelSet::mono())
                      .withInput ("Object58",  AudioChannelSet::mono())
                      .withInput ("Object59",  AudioChannelSet::mono())
                      .withInput ("Object60",  AudioChannelSet::mono())
                      .withInput ("Object61",  AudioChannelSet::mono())
                      .withInput ("Object62",  AudioChannelSet::mono())
                      .withInput ("Object63",  AudioChannelSet::mono())
                      .withInput ("Object64",  AudioChannelSet::mono())
                      .withOutput ("Output1",  AudioChannelSet::mono())
                      .withOutput ("Output2",  AudioChannelSet::mono())
                      .withOutput ("Output3",  AudioChannelSet::mono())
                      .withOutput ("Output4",  AudioChannelSet::mono())
                      .withOutput ("Output5",  AudioChannelSet::mono())
                      .withOutput ("Output6",  AudioChannelSet::mono())
                      .withOutput ("Output7",  AudioChannelSet::mono())
                      .withOutput ("Output8",  AudioChannelSet::mono())
                      .withOutput ("Output9",  AudioChannelSet::mono())
                      .withOutput ("Output10",  AudioChannelSet::mono())
                      .withOutput ("Output11",  AudioChannelSet::mono())
                      .withOutput ("Output12",  AudioChannelSet::mono())
                      .withOutput ("Output13",  AudioChannelSet::mono())
                      .withOutput ("Output14",  AudioChannelSet::mono())
                      .withOutput ("Output15",  AudioChannelSet::mono())
                      .withOutput ("Output16",  AudioChannelSet::mono())
                      .withOutput ("Output17",  AudioChannelSet::mono())
                      .withOutput ("Output18",  AudioChannelSet::mono())
                      .withOutput ("Output19",  AudioChannelSet::mono())
                      .withOutput ("Output20",  AudioChannelSet::mono())
                      .withOutput ("Output21",  AudioChannelSet::mono())
                      .withOutput ("Output22",  AudioChannelSet::mono())
                      .withOutput ("Output23",  AudioChannelSet::mono())
                      .withOutput ("Output24",  AudioChannelSet::mono())
                      .withOutput ("Output25",  AudioChannelSet::mono())
                      .withOutput ("Output26",  AudioChannelSet::mono())
                      .withOutput ("Output27",  AudioChannelSet::mono())
                      .withOutput ("Output28",  AudioChannelSet::mono())
                      .withOutput ("Output29",  AudioChannelSet::mono())
                      .withOutput ("Output30",  AudioChannelSet::mono())
                      .withOutput ("Output31",  AudioChannelSet::mono())
                      .withOutput ("Output32",  AudioChannelSet::mono())
                      .withOutput ("Output33",  AudioChannelSet::mono())
                      .withOutput ("Output34",  AudioChannelSet::mono())
                      .withOutput ("Output35",  AudioChannelSet::mono())
                      .withOutput ("Output36",  AudioChannelSet::mono())
                      .withOutput ("Output37",  AudioChannelSet::mono())
                      .withOutput ("Output38",  AudioChannelSet::mono())
                      .withOutput ("Output39",  AudioChannelSet::mono())
                      .withOutput ("Output40",  AudioChannelSet::mono())
                      .withOutput ("Output41",  AudioChannelSet::mono())
                      .withOutput ("Output42",  AudioChannelSet::mono())
                      .withOutput ("Output43",  AudioChannelSet::mono())
                      .withOutput ("Output44",  AudioChannelSet::mono())
                      .withOutput ("Output45",  AudioChannelSet::mono())
                      .withOutput ("Output46",  AudioChannelSet::mono())
                      .withOutput ("Output47",  AudioChannelSet::mono())
                      .withOutput ("Output48",  AudioChannelSet::mono())
                      .withOutput ("Output49",  AudioChannelSet::mono())
                      .withOutput ("Output50",  AudioChannelSet::mono())
                      .withOutput ("Output51",  AudioChannelSet::mono())
                      .withOutput ("Output52",  AudioChannelSet::mono())
                      .withOutput ("Output53",  AudioChannelSet::mono())
                      .withOutput ("Output54",  AudioChannelSet::mono())
                      .withOutput ("Output55",  AudioChannelSet::mono())
                      .withOutput ("Output56",  AudioChannelSet::mono())
                      .withOutput ("Output57",  AudioChannelSet::mono())
                      .withOutput ("Output58",  AudioChannelSet::mono())
                      .withOutput ("Output59",  AudioChannelSet::mono())
                      .withOutput ("Output60",  AudioChannelSet::mono())
                      .withOutput ("Output61",  AudioChannelSet::mono())
                      .withOutput ("Output62",  AudioChannelSet::mono())
                      .withOutput ("Output63",  AudioChannelSet::mono())
                      .withOutput ("Output64",  AudioChannelSet::mono())
                      )
{
    parameters.state = ValueTree (Identifier ("MetadapterRenderer"));
}

Processor::~Processor(){}

bool Processor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    bool isMono = true;
    for(int i=0;i<getBusCount(true);i++){
        if ( layouts.getChannelSet(true, i) != AudioChannelSet::mono() )
            isMono = false;
    }
    
    for(int i=0;i<getBusCount(false);i++){
        if ( layouts.getChannelSet(false, i) != AudioChannelSet::mono() )
            isMono = false;
    }
    return isMono;
}

void Processor::PluginSetup()
{
    context = new visr::SignalFlowContext(blockSize,static_cast<visr::SamplingFrequencyType>(sr));
    parameters.state.getOrCreateChildWithName("Error",nullptr).removeAllProperties(nullptr);
    pml::initialiseParameterLibrary();
    
    ValueTree loudspeakerConfiguration = parameters.state.getOrCreateChildWithName("loudspeakerConfiguration", nullptr);
    String currentLayoutFilePath = loudspeakerConfiguration.getProperty("ldspkFile","");
    
    String defaultMDOconfigBuildPath = core::defaultSrcPath+"/python/metadapter/config/mdo_production.xml";
    String defaultMDOconfigInstallPath = core::defaultInstallPath+"/python/metadapter/config/mdo_production.xml";
    
    
    if(parameters.state.getOrCreateChildWithName("MetadapterSettings", nullptr).getProperty("ConfigFile","").toString().toStdString().empty()){
        
        if(File(defaultMDOconfigBuildPath).exists()){
            //        printf("default HRIR exists in build directory");
            parameters.state.getOrCreateChildWithName("MetadapterSettings", nullptr).setProperty("ConfigFile", defaultMDOconfigBuildPath,nullptr);
        }else if(File(defaultMDOconfigInstallPath).exists()){
            //            printf("default HRIR exists in installation directory");
            parameters.state.getOrCreateChildWithName("MetadapterSettings", nullptr).setProperty("ConfigFile", defaultMDOconfigInstallPath,nullptr);
            
        }
        //              isFirst=false;
    }

    
    if( !currentLayoutFilePath.isEmpty() ){
        currentLayoutFile = File(currentLayoutFilePath);
        if (currentLayoutFile.exists()){
            VISRFindLayouts(currentLayoutFile.getParentDirectory());
            VISRCheckLayout(currentLayoutFile);
        }
    }else{
        if(parameters.state.getOrCreateChildWithName("MetadapterSettings", nullptr).getProperty("ConfigFile").toString() == defaultMDOconfigBuildPath ||
           parameters.state.getOrCreateChildWithName("MetadapterSettings", nullptr).getProperty("ConfigFile").toString() == defaultMDOconfigInstallPath)
            VISRCheckLayout(stereoMDO);
        else VISRCheckLayout(defaultLdspArray);
    }
    
    VISRChangeLayout();
    if(wrapperType==wrapperType_Standalone){
        maxInputChannels = maxOutputChannels;
    }else{
        maxInputChannels=getTotalNumInputChannels();
    }
    
    inputBuffer = AudioSampleBuffer(maxInputChannels, static_cast<int>(context->period()) );
    inputBuffer.clear();
    
    /********************************* SETTING TOP LEVEL COMPONENT AND ITS CALLBACK  **********************************/
    /* Set up the filter matrix for the diffusion filters. */
    std::size_t const diffusionFilterLength = 63; // fixed filter length of the filters in the compiled-in matrix
    std::size_t const diffusionFiltersInFile = 64; // Fixed number of filters in file.
    
    // First create a filter matrix containing all filters from a initializer list that is compiled into the program.
    efl::BasicMatrix<visr::SampleType> allDiffusionCoeffs( diffusionFiltersInFile,
                                                          diffusionFilterLength,
                                                          quasiAllpassFIR_f64_n63_initializer_list,
                                                          cVectorAlignmentSamples );
    
    // Create a second filter matrix that matches the number of required filters.
    efl::BasicMatrix<SampleType> diffusionCoeffs( loudspeakerArray.getNumRegularSpeakers(), diffusionFilterLength, cVectorAlignmentSamples );
    for( std::size_t idx( 0 ); idx < diffusionCoeffs.numberOfRows( ); ++idx )
    {
        efl::vectorCopy( allDiffusionCoeffs.row( idx ), diffusionCoeffs.row( idx ), diffusionFilterLength, cVectorAlignmentSamples );
    }
    
    pythonsupport::InitialisationGuard::initialise();
    
    String const numReverbObjects = parameters.state.getOrCreateChildWithName("ReverbSettings", nullptr)
    .getProperty("MaximumReverbObjectNumber");
    String const discreteReflectionsPerObject =  parameters.state.getOrCreateChildWithName("ReverbSettings", nullptr)
    .getProperty("DiscreteReflectionsNumber");
    String const lateReverbFilterLength = parameters.state.getOrCreateChildWithName("ReverbSettings", nullptr)
    .getProperty("LateReverbLength");
    
    String const lateReverbDecorrelationFilters = parameters.state.getOrCreateChildWithName("ReverbSettings", nullptr)
    .getProperty("LateReverbDecorrelationFiltersFile");
    
    
    String reverbConfig ="{";
    bool isFirstSetting=true;
    
    if(!numReverbObjects.isEmpty()){
        reverbConfig.append("\"numReverbObjects\": "+numReverbObjects,50);
        isFirstSetting=false;
    }
    if(!discreteReflectionsPerObject.isEmpty()){
        if(!isFirstSetting) reverbConfig.append(", ",2);
        else isFirstSetting=false;
        reverbConfig.append("\"discreteReflectionsPerObject\": "+discreteReflectionsPerObject,50);
    }
    if(!lateReverbFilterLength.isEmpty()){
        if(!isFirstSetting) reverbConfig.append(", ",2);
        else isFirstSetting=false;
        reverbConfig.append("\"lateReverbFilterLength\": "+lateReverbFilterLength,50);
    }
    if(!lateReverbDecorrelationFilters.isEmpty()){
        if(!isFirstSetting) reverbConfig.append(", ",2);
        reverbConfig.append("\"lateReverbDecorrelationFilters\": \""+lateReverbDecorrelationFilters+"\"" ,500);
    }
    
    reverbConfig +="}";
    if(reverbConfig=="{}") reverbConfig = std::string();
    
    
    String const metaconfig = parameters.state.getOrCreateChildWithName("MetadapterSettings", nullptr).getProperty("ConfigFile");
    boost::filesystem::path const metaConfigPath{ metaconfig.toStdString() };
    
    std::string const oscControlPort = parameters.state.getOrCreateChildWithName("MetadapterSettings", nullptr).getProperty("OscControlPort").toString().toStdString();
    
    std::string const jsonControlPort = parameters.state.getOrCreateChildWithName("MetadapterSettings", nullptr).getProperty("JSONControlPort","9001").toString().toStdString();
    
    
    /********************************* SETUP JUCE-TOPLEVELCOMPONENT wrapper  **********************************/
    component.reset();
    component.reset(new MetadapterRenderer( *context,
                                           "RendererWithMetadapter",
                                           nullptr,
                                           loudspeakerArray,
                                           maxInputChannels,
                                           maxOutputChannels,
                                           parameterUpdatePeriod,
                                           diffusionCoeffs,
                                           "",
                                           8001,
                                           0,
                                           reverbConfig.toStdString(),
                                           false,
                                           metaConfigPath.generic_string(), // Python doesn't like backslashes as path separators, even on Windows.
                                           oscControlPort,
                                           jsonControlPort
                                           ));
    
    componentHandler.reset(new visr_support::VISRComponentWrapperObjVector(parameters, *component, views, 8001, 0, true));
    {
        const juce::CriticalSection& lock = getCallbackLock();
        lock.enter();
        setupDone = true;
        lock.exit();
    }
}

void Processor::PluginUnload()
{
    {
        const juce::CriticalSection& lock = getCallbackLock();
        lock.enter();
        setupDone = false;
        if(componentHandler!=nullptr) componentHandler.reset();
        lock.exit();
    }
}

void Processor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    if (!setupDone)
    {
        buffer.clear();
        return;
    }
    try{
        {
            // Ensure that the current thread is initialized for using the Python C API.
            // The GIL is released immediately as the scope is left.
            visr::pythonsupport::GilEnsureGuard guard;
        }
        
        PluginProcessorLoudspeakers::prepareBuffer(buffer);
        // calls process function of the toplevel component, through the VISRComponentWrapper
        componentHandler->process(inputBuffer.getArrayOfReadPointers(),buffer.getArrayOfWritePointers());
    }catch( std::exception const &ex )
    {
        String errorMsg;
        errorMsg << "Exception in audio callback\n"<< ex.what();
        parameters.state.getOrCreateChildWithName("Error",nullptr).setProperty("ErrorMsg", errorMsg, nullptr);
        std::cerr << errorMsg << std::endl;
        suspendProcessing(true);
    }
}

AudioProcessorEditor* Processor::createEditor()
{
    return new Editor (this);
}

} // namespace metadapter_renderer
} // namespace dawplugins
} // namespace visr_production_suite
