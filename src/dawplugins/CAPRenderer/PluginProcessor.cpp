#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <libpml/initialise_parameter_library.hpp>


using namespace visr;
using namespace juce;
using namespace visr_production_suite::core;
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new visr_production_suite::dawplugins::cap_renderer::Processor();
}

namespace visr_production_suite
{
namespace dawplugins
{
namespace cap_renderer
{

Processor::Processor():
PluginProcessorLoudspeakers(BusesProperties()
                      .withInput ("Object1",  AudioChannelSet::mono(), true)
                      .withInput ("Object2",  AudioChannelSet::mono(), true)
                      .withInput ("Object3",  AudioChannelSet::mono(), true)
                      .withInput ("Object4",  AudioChannelSet::mono(), true)
                      .withInput ("Object5",  AudioChannelSet::mono(), true)
                      .withInput ("Object6",  AudioChannelSet::mono(), true)
                      .withInput ("Object7",  AudioChannelSet::mono(), true)
                      .withInput ("Object8",  AudioChannelSet::mono(), true)
                      .withInput ("Object9",  AudioChannelSet::mono(), true)
                      .withInput ("Object10",  AudioChannelSet::mono(), true)
                      .withInput ("Object11",  AudioChannelSet::mono(), true)
                      .withInput ("Object12",  AudioChannelSet::mono(), true)
                      .withInput ("Object13",  AudioChannelSet::mono(), true)
                      .withInput ("Object14",  AudioChannelSet::mono(), true)
                      .withInput ("Object15",  AudioChannelSet::mono(), true)
                      .withInput ("Object16",  AudioChannelSet::mono(), true)
                      .withInput ("Object17",  AudioChannelSet::mono(), true)
                      .withInput ("Object18",  AudioChannelSet::mono(), true)
                      .withInput ("Object19",  AudioChannelSet::mono(), true)
                      .withInput ("Object20",  AudioChannelSet::mono(), true)
                      .withInput ("Object21",  AudioChannelSet::mono(), true)
                      .withInput ("Object22",  AudioChannelSet::mono(), true)
                      .withInput ("Object23",  AudioChannelSet::mono(), true)
                      .withInput ("Object24",  AudioChannelSet::mono(), true)
                      .withInput ("Object25",  AudioChannelSet::mono(), true)
                      .withInput ("Object26",  AudioChannelSet::mono(), true)
                      .withInput ("Object27",  AudioChannelSet::mono(), true)
                      .withInput ("Object28",  AudioChannelSet::mono(), true)
                      .withInput ("Object29",  AudioChannelSet::mono(), true)
                      .withInput ("Object30",  AudioChannelSet::mono(), true)
                      .withInput ("Object31",  AudioChannelSet::mono(), true)
                      .withInput ("Object32",  AudioChannelSet::mono(), true)
                      .withInput ("Object33",  AudioChannelSet::mono(), true)
                      .withInput ("Object34",  AudioChannelSet::mono(), true)
                      .withInput ("Object35",  AudioChannelSet::mono(), true)
                      .withInput ("Object36",  AudioChannelSet::mono(), true)
                      .withInput ("Object37",  AudioChannelSet::mono(), true)
                      .withInput ("Object38",  AudioChannelSet::mono(), true)
                      .withInput ("Object39",  AudioChannelSet::mono(), true)
                      .withInput ("Object40",  AudioChannelSet::mono(), true)
                      .withInput ("Object41",  AudioChannelSet::mono(), true)
                      .withInput ("Object42",  AudioChannelSet::mono(), true)
                      .withInput ("Object43",  AudioChannelSet::mono(), true)
                      .withInput ("Object44",  AudioChannelSet::mono(), true)
                      .withInput ("Object45",  AudioChannelSet::mono(), true)
                      .withInput ("Object46",  AudioChannelSet::mono(), true)
                      .withInput ("Object47",  AudioChannelSet::mono(), true)
                      .withInput ("Object48",  AudioChannelSet::mono(), true)
                      .withInput ("Object49",  AudioChannelSet::mono(), true)
                      .withInput ("Object50",  AudioChannelSet::mono(), true)
                      .withInput ("Object51",  AudioChannelSet::mono(), true)
                      .withInput ("Object52",  AudioChannelSet::mono(), true)
                      .withInput ("Object53",  AudioChannelSet::mono(), true)
                      .withInput ("Object54",  AudioChannelSet::mono(), true)
                      .withInput ("Object55",  AudioChannelSet::mono(), true)
                      .withInput ("Object56",  AudioChannelSet::mono(), true)
                      .withInput ("Object57",  AudioChannelSet::mono(), true)
                      .withInput ("Object58",  AudioChannelSet::mono(), true)
                      .withInput ("Object59",  AudioChannelSet::mono(), true)
                      .withInput ("Object60",  AudioChannelSet::mono(), true)
                      .withInput ("Object61",  AudioChannelSet::mono(), true)
                      .withInput ("Object62",  AudioChannelSet::mono(), true)
                      .withInput ("Object63",  AudioChannelSet::mono(), true)
                      .withInput ("Object64",  AudioChannelSet::mono(), true)
                      .withOutput ("Output1",  AudioChannelSet::mono(), true)
                      .withOutput ("Output2",  AudioChannelSet::mono(), true)
                      .withOutput ("Output3",  AudioChannelSet::mono(), true)
                      .withOutput ("Output4",  AudioChannelSet::mono(), true)
                      .withOutput ("Output5",  AudioChannelSet::mono(), true)
                      .withOutput ("Output6",  AudioChannelSet::mono(), true)
                      .withOutput ("Output7",  AudioChannelSet::mono(), true)
                      .withOutput ("Output8",  AudioChannelSet::mono(), true)
                      .withOutput ("Output9",  AudioChannelSet::mono(), true)
                      .withOutput ("Output10",  AudioChannelSet::mono(), true)
                      .withOutput ("Output11",  AudioChannelSet::mono(), true)
                      .withOutput ("Output12",  AudioChannelSet::mono(), true)
                      .withOutput ("Output13",  AudioChannelSet::mono(), true)
                      .withOutput ("Output14",  AudioChannelSet::mono(), true)
                      .withOutput ("Output15",  AudioChannelSet::mono(), true)
                      .withOutput ("Output16",  AudioChannelSet::mono(), true)
                      .withOutput ("Output17",  AudioChannelSet::mono(), true)
                      .withOutput ("Output18",  AudioChannelSet::mono(), true)
                      .withOutput ("Output19",  AudioChannelSet::mono(), true)
                      .withOutput ("Output20",  AudioChannelSet::mono(), true)
                      .withOutput ("Output21",  AudioChannelSet::mono(), true)
                      .withOutput ("Output22",  AudioChannelSet::mono(), true)
                      .withOutput ("Output23",  AudioChannelSet::mono(), true)
                      .withOutput ("Output24",  AudioChannelSet::mono(), true)
                      .withOutput ("Output25",  AudioChannelSet::mono(), true)
                      .withOutput ("Output26",  AudioChannelSet::mono(), true)
                      .withOutput ("Output27",  AudioChannelSet::mono(), true)
                      .withOutput ("Output28",  AudioChannelSet::mono(), true)
                      .withOutput ("Output29",  AudioChannelSet::mono(), true)
                      .withOutput ("Output30",  AudioChannelSet::mono(), true)
                      .withOutput ("Output31",  AudioChannelSet::mono(), true)
                      .withOutput ("Output32",  AudioChannelSet::mono(), true)
                      .withOutput ("Output33",  AudioChannelSet::mono(), true)
                      .withOutput ("Output34",  AudioChannelSet::mono(), true)
                      .withOutput ("Output35",  AudioChannelSet::mono(), true)
                      .withOutput ("Output36",  AudioChannelSet::mono(), true)
                      .withOutput ("Output37",  AudioChannelSet::mono(), true)
                      .withOutput ("Output38",  AudioChannelSet::mono(), true)
                      .withOutput ("Output39",  AudioChannelSet::mono(), true)
                      .withOutput ("Output40",  AudioChannelSet::mono(), true)
                      .withOutput ("Output41",  AudioChannelSet::mono(), true)
                      .withOutput ("Output42",  AudioChannelSet::mono(), true)
                      .withOutput ("Output43",  AudioChannelSet::mono(), true)
                      .withOutput ("Output44",  AudioChannelSet::mono(), true)
                      .withOutput ("Output45",  AudioChannelSet::mono(), true)
                      .withOutput ("Output46",  AudioChannelSet::mono(), true)
                      .withOutput ("Output47",  AudioChannelSet::mono(), true)
                      .withOutput ("Output48",  AudioChannelSet::mono(), true)
                      .withOutput ("Output49",  AudioChannelSet::mono(), true)
                      .withOutput ("Output50",  AudioChannelSet::mono(), true)
                      .withOutput ("Output51",  AudioChannelSet::mono(), true)
                      .withOutput ("Output52",  AudioChannelSet::mono(), true)
                      .withOutput ("Output53",  AudioChannelSet::mono(), true)
                      .withOutput ("Output54",  AudioChannelSet::mono(), true)
                      .withOutput ("Output55",  AudioChannelSet::mono(), true)
                      .withOutput ("Output56",  AudioChannelSet::mono(), true)
                      .withOutput ("Output57",  AudioChannelSet::mono(), true)
                      .withOutput ("Output58",  AudioChannelSet::mono(), true)
                      .withOutput ("Output59",  AudioChannelSet::mono( ), true)
                      .withOutput ("Output60",  AudioChannelSet::mono(), true)
                      .withOutput ("Output61",  AudioChannelSet::mono(), true)
                      .withOutput ("Output62",  AudioChannelSet::mono(), true)
                      .withOutput ("Output63",  AudioChannelSet::mono(), true)
                      .withOutput ("Output64",  AudioChannelSet::mono(), true)
                      )
    {
     
    parameters.createAndAddParameter (std::make_unique<juce::AudioProcessorValueTreeState::Parameter>
                                      (IDs::listenerPosX.toString(), IDs::listenerPosX.toString(), IDs::listenerPosX.toString(),
                                      NormalisableRange<float> (-10,10,0.001f), 0, nullptr,nullptr));
    
    parameters.createAndAddParameter (std::make_unique<juce::AudioProcessorValueTreeState::Parameter>
                                      (IDs::listenerPosY.toString(), IDs::listenerPosY.toString(), IDs::listenerPosY.toString(),
                                      NormalisableRange<float> (-10,10,0.001f), 0, nullptr,nullptr));
    
    parameters.createAndAddParameter (std::make_unique<juce::AudioProcessorValueTreeState::Parameter>
                                      (IDs::listenerPosZ.toString(), IDs::listenerPosZ.toString(), IDs::listenerPosZ.toString(),
                                      NormalisableRange<float> (-10,10,0.001f), 0, nullptr,nullptr));
    
    parameters.createAndAddParameter (std::make_unique<juce::AudioProcessorValueTreeState::Parameter>
                                      (IDs::yaw.toString(), IDs::yaw.toString(), IDs::yaw.toString(),
                                      NormalisableRange<float> (-180.0,180.0, 0.001), 0, nullptr,nullptr));
    
    parameters.createAndAddParameter (std::make_unique<juce::AudioProcessorValueTreeState::Parameter>
                                      (IDs::pitch.toString(), IDs::pitch.toString(), IDs::pitch.toString(),
                                      NormalisableRange<float> (-90,90, 0.001), 0, nullptr,nullptr));
    
    parameters.createAndAddParameter (std::make_unique<juce::AudioProcessorValueTreeState::Parameter>
                                      (IDs::roll.toString(), IDs::roll.toString(), IDs::roll.toString(),
                                      NormalisableRange<float> (-180,180, 0.001), 0, nullptr,nullptr));
    
    
    parameters.state = ValueTree (Identifier ("CAPRenderer"));
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
    suspendProcessing(true);
    context = new visr::SignalFlowContext(blockSize,static_cast<visr::SamplingFrequencyType>(sr));
    
    parameters.state.getOrCreateChildWithName("Error",nullptr).removeAllProperties(nullptr);
    pml::initialiseParameterLibrary();
    
    ValueTree loudspeakerConfiguration = parameters.state.getOrCreateChildWithName("loudspeakerConfiguration", nullptr);
    String currentLayoutFilePath = loudspeakerConfiguration.getProperty("ldspkFile","");
    //    std::cout<<"LOADED STATE PREPTOPLAY "<<parameters.state.toXmlString()<<std::endl;
    
    if( !currentLayoutFilePath.isEmpty() ){
        currentLayoutFile = File(currentLayoutFilePath);
        if (currentLayoutFile.exists()){
            VISRFindLayouts(currentLayoutFile.getParentDirectory());
            VISRCheckLayout(currentLayoutFile);
        }
    }else{
        VISRCheckLayout(defaultLdspArray);
    }
    
    VISRChangeLayout();
    
    if(wrapperType==wrapperType_Standalone){
        maxInputChannels = maxOutputChannels;
    }else{
        maxInputChannels = getTotalNumInputChannels();
    }
    
    inputBuffer = AudioSampleBuffer(maxInputChannels,  static_cast<int>(context->period()) );
    inputBuffer.clear();
    
    /********************************* SETTING TOP LEVEL COMPONENT AND ITS CALLBACK  **********************************/
    //    componentHandler.reset();
    
    component.reset();
    component.reset(new DualbandCapRenderer( *context,
                                      "CAPRenderer",
                                      nullptr,
                                      maxInputChannels,
                                      loudspeakerArray,
                                      context->period()
                                      ));
    
    componentHandler.reset(new visr_support::VISRComponentWrapperObjVector(parameters, *component,views, 8001, 1));
    {
        const juce::CriticalSection& lock = getCallbackLock();
        lock.enter();
        setupDone = true;
        lock.exit();
    }
    
    suspendProcessing(false);
}

void Processor::PluginUnload()
{
    {
        suspendProcessing(true);
        const juce::CriticalSection& lock = getCallbackLock();
        lock.enter();
        setupDone = false;
        if(componentHandler!=nullptr) componentHandler.reset();
        lock.exit();
        suspendProcessing(false);
    }
}
    //void processBlockBypassed (AudioBuffer<float>& buffer,
//                           MidiBuffer& midiMessages){
//
//}
    
    
void Processor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    if (!setupDone)
    {
        buffer.clear();
        return;
    }
    
    try{
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

} // namespace cap_renderer
} // namespace dawplugins
} // namespace visr_production_suite

