/* Copyright Institute of Sound and Vibration Research - All rights reserved. */

#include "PluginProcessor.h"
#include "PluginEditor.h"


#include <libpml/initialise_parameter_library.hpp>

#include <libpythonsupport/initialisation_guard.hpp>
#include <libpythonsupport/gil_ensure_guard.hpp>

#include <boost/format.hpp>
#include <boost/filesystem/path.hpp>
#include "CurrentDefaultPaths.h"
#include <libcore/IDs.h>

//==============================================================================
// This creates new instances of the plugin..
// This must be defined in the global namespace.
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
return new visr_production_suite::dawplugins::binaural_renderer::Processor();
}

namespace visr_production_suite
{
namespace dawplugins
{
namespace binaural_renderer
{

using namespace juce;
using namespace core;
using namespace visr;
    
Processor::Processor():
PluginProcessorScene(BusesProperties()
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
                      .withOutput ("Output", AudioChannelSet::stereo()))
{
    maxOutputChannels = getBusesLayout().getNumChannels(false, 0);
    
    parameters.createAndAddParameter (std::make_unique<juce::AudioProcessorValueTreeState::Parameter> (IDs::listenerPosX.toString(), IDs::listenerPosX.toString(), IDs::listenerPosX.toString(),
                                                                                                       NormalisableRange<float> (-10,10,0.01f), 0, nullptr,nullptr));
    
    parameters.createAndAddParameter (std::make_unique<juce::AudioProcessorValueTreeState::Parameter>(IDs::listenerPosY.toString(), IDs::listenerPosY.toString(), IDs::listenerPosY.toString(),
                                                                                                      NormalisableRange<float> (-10,10,0.01f), 0, nullptr,nullptr));
    
    parameters.createAndAddParameter (std::make_unique<juce::AudioProcessorValueTreeState::Parameter>(IDs::listenerPosZ.toString(), IDs::listenerPosZ.toString(), IDs::listenerPosZ.toString(),
                                                                                                      NormalisableRange<float> (-10,10,0.01f), 0, nullptr,nullptr));
    
    parameters.createAndAddParameter (std::make_unique<juce::AudioProcessorValueTreeState::Parameter>(IDs::yaw.toString(), IDs::yaw.toString(), IDs::yaw.toString(),
                                                                                                      NormalisableRange<float> (-180.0f, 180.0f, 0.01f), 0, nullptr,nullptr));
    
    parameters.createAndAddParameter (std::make_unique<juce::AudioProcessorValueTreeState::Parameter>(IDs::pitch.toString(), IDs::pitch.toString(), IDs::pitch.toString(),
                                                                                                      NormalisableRange<float> (-90.0f, 90.0f, 0.01f), 0, nullptr,nullptr));
    
    parameters.createAndAddParameter (std::make_unique<juce::AudioProcessorValueTreeState::Parameter>(IDs::roll.toString(), IDs::roll.toString(), IDs::roll.toString(),
                                                                                                      NormalisableRange<float> (-180.0f,180.0f, 0.01f), 0, nullptr,nullptr));
    
    parameters.state = ValueTree (Identifier ("BinauralRenderer"));
}

void Processor::recalibrate(){
    if(!setupDone) return;
    try{
        //        ParameterOutput< pml::MessageQueueProtocol, pml::StringParameter > mDatagramOutput;
        visr::pml::MessageQueueProtocol::OutputBase * calibrationPort;
        
        calibrationPort =  dynamic_cast<pml::MessageQueueProtocol::OutputBase *>(&componentHandlerTracker->getInputPort("calibrationInput"));
        visr::pml::StringParameter calibrationString( "re-calibrate!...please");
        calibrationPort->enqueue(std::make_unique<visr::pml::StringParameter>(calibrationString));
        //        calibrationPort->enqueue( & pml::StringParameter( "re-calibrate!...please"));
    }catch( std::exception const &ex )
    {
        String errorMsg;
        errorMsg << "Exception while recalibrating\n"<< ex.what();
        parameters.state.getOrCreateChildWithName("Error",nullptr).setProperty("ErrorMsg", errorMsg, nullptr);
        std::cerr << errorMsg << std::endl;
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
        
        PluginProcessorScene::prepareBuffer(buffer);
        // calls process function of the toplevel component, through the VISRComponentWrapper
        if( useHeadTracking && componentHandlerTracker != nullptr){
            
            
            componentHandlerTracker->process(inputBuffer.getArrayOfReadPointers(),buffer.getArrayOfWritePointers());
//            trackerFlow->process(inputBuffer.getArrayOfReadPointers(),buffer.getArrayOfWritePointers());
            
//            visr::pml::DoubleBufferingProtocol::InputBase * mListenerPos = dynamic_cast<visr::pml::DoubleBufferingProtocol::InputBase *>(&trackerFlow->externalParameterSendPort("listenerPositionOut"));
            
            visr::pml::DoubleBufferingProtocol::InputBase * mListenerPos = dynamic_cast<visr::pml::DoubleBufferingProtocol::InputBase *>(&componentHandlerTracker->getOutputPort("listenerPositionOut"));
            const visr::pml::ListenerPosition * lPos = dynamic_cast<const visr::pml::ListenerPosition *>(&(mListenerPos->data()));
            
            visr::pml::DoubleBufferingProtocol::OutputBase * listenerPort;
            listenerPort =  dynamic_cast<pml::DoubleBufferingProtocol::OutputBase *>(&componentHandler->getInputPort("listenerPositionIn"));
            
            visr::pml::ListenerPosition* listenerPos = dynamic_cast<visr::pml::ListenerPosition *>(&(listenerPort->data()));
//            printf(" tracker data -> yaw: %f pitch: %f roll: %f\n",lPos->yaw(),lPos->pitch(),lPos->roll());
//            listenerPos->setX(lPos->x());
//            listenerPos->setY(lPos->y());
//            listenerPos->setZ(lPos->z());
            listenerPos->setOrientation(lPos->yaw(), lPos->pitch(), lPos->roll());
            listenerPort->swapBuffers();
        }
        
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

bool Processor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    bool isMono = true;
    for(int i=0;i<getBusCount(true);i++){
        if ( layouts.getChannelSet(true, i) != AudioChannelSet::mono() )
            isMono = false;
    }
    if(layouts.getMainOutputChannelSet() != AudioChannelSet::stereo() || !isMono)
        return false;
    return true;
}

Processor::~Processor(){

}

void Processor::PluginSetup()
{
    context = new visr::SignalFlowContext(blockSize,static_cast<visr::SamplingFrequencyType>(sr));
    parameters.state.getOrCreateChildWithName("Error",nullptr).removeAllProperties(nullptr);
    
    pythonsupport::InitialisationGuard::initialise();
    pml::initialiseParameterLibrary();
    
    if(wrapperType==wrapperType_Standalone) maxInputChannels=2;
    else maxInputChannels = parameters.state.getOrCreateChildWithName("GeneralSettings", nullptr).getProperty("MaximumObjectNumber",10);
    
    inputBuffer = AudioSampleBuffer(getTotalNumInputChannels(), static_cast<int>(context->period()) );
    inputBuffer.clear();
    
    /********************************* SETUP JUCE-TOPLEVELCOMPONENT wrapper  **********************************/
    try
    {
        // Global configuration settings
        const int dynamicHeadTrackingSelect = parameters.state.getOrCreateChildWithName( "TrackingSettings", nullptr ).getProperty( "trackingType", 0 );
        useHeadTracking = (dynamicHeadTrackingSelect != 0);
        
        if(parameters.state.getChildWithName("ListenerPos").isValid()){
            parameters.state.removeChild(parameters.state.getChildWithName("ListenerPos"), nullptr);
        }
        
        bool const internalHeadTracking = (dynamicHeadTrackingSelect == 2);
        
        
        if(dynamicHeadTrackingSelect==1){
            //      parameters.createAndAddParameter (IDs::listenerPosX.toString(), IDs::listenerPosX.toString(), IDs::listenerPosX.toString(),
            //                                        NormalisableRange<float> (-10,10,0.01f), 0, nullptr,nullptr);
            //
            //      parameters.createAndAddParameter (IDs::listenerPosY.toString(), IDs::listenerPosY.toString(), IDs::listenerPosY.toString(),
            //                                        NormalisableRange<float> (-10,10,0.01f), 0, nullptr,nullptr);
            //
            //      parameters.createAndAddParameter (IDs::listenerPosZ.toString(), IDs::listenerPosZ.toString(), IDs::listenerPosZ.toString(),
            //                                        NormalisableRange<float> (-10,10,0.01f), 0, nullptr,nullptr);
            //
            //      parameters.createAndAddParameter (IDs::yaw.toString(), IDs::yaw.toString(), IDs::yaw.toString(),
            //                                        NormalisableRange<float> (-180.0f, 180.0f, 0.01f), 0, nullptr,nullptr);
            //
            //      parameters.createAndAddParameter (IDs::pitch.toString(), IDs::pitch.toString(), IDs::pitch.toString(),
            //                                        NormalisableRange<float> (-90.0f, 90.0f, 0.01f), 0, nullptr,nullptr);
            //
            //      parameters.createAndAddParameter (IDs::roll.toString(), IDs::roll.toString(), IDs::roll.toString(),
            //                                        NormalisableRange<float> (-180.0f,180.0f, 0.01f), 0, nullptr,nullptr);
            //
            //
        
        }
        
        
        
        // no quotes!!   tracker.RazorAHRS
        std::string const headTrackingReceiverClassName = internalHeadTracking
        ? parameters.state.getOrCreateChildWithName( "TrackingSettings", nullptr )
        .getProperty( "InternalTracker" ).toString().toStdString()
        : std::string();
        
        //  AF    {'port': '/dev/cu.usbserial-AJ03GR8O',  'calibrationInput': True, 'yawRightHand' : True, 'pitchRightHand' : True, 'rollRightHand' : True, 'calibration' : True}
        //  GC    {'port': '/dev/cu.usbserial-AJ03GSC8',  'calibrationInput': True, 'yawRightHand' : True, 'pitchRightHand' : True, 'rollRightHand' : True, 'calibration' : True}
        // AF when swapping roll and pitch (razor_ahrs.py) :  {'port': '/dev/cu.usbserial-AJ03GR8O',  'calibrationInput': True, 'yawRightHand' : True, 'rollRightHand' : True}
        // GC when swapping roll and pitch (razor_ahrs.py) :  {'port': '/dev/cu.usbserial-AJ03GSC8',  'calibrationInput': True, 'yawRightHand' : True, 'rollRightHand' : True}
        
        /*razor_ahrs.py swap:
         ypr.orientation = [deg2rad(self.orientation[0] + self.yawOffset),
                              deg2rad(self.orientation[2] + self.rollOffset),
                              -deg2rad(self.orientation[1] + self.pitchOffset)]
         */
        
        std::string headTrackingReceiverParameters = internalHeadTracking
        ? parameters.state.getOrCreateChildWithName( "TrackingSettings", nullptr )
        .getProperty( "InternalTrackerOptions").toString().toStdString()
        : std::string();
        
//        if(!headTrackingReceiverParameters.empty()){
//            std::string delimiter = "'calibrationPort': ";
//            std::string port_ = headTrackingReceiverParameters.substr(headTrackingReceiverParameters.find(delimiter)+delimiter.length(),4);
//            int port = std::stoi(port_);
//            
//            headTrackingReceiverParameters.replace(headTrackingReceiverParameters.find(delimiter)+delimiter.length(), 4, std::to_string(port+1));
//            String s = headTrackingReceiverParameters;
//            parameters.state.getOrCreateChildWithName( "TrackingSettings", nullptr ).setProperty("InternalTrackerOptions",s,nullptr);
//            //              std::cout<<"PORT "<<parameters.state.getOrCreateChildWithName( "TrackingSettings", nullptr ).getProperty( "InternalTrackerOptions").toString().toStdString()<<std::endl;
//            //          }
//            
//
//        }
        
        if(internalHeadTracking && headTrackingReceiverClassName.empty() && headTrackingReceiverParameters.empty()){
            String errorMsg;
            errorMsg << "You selected internal tracking without specifying tracker's name and options";
            parameters.state.getOrCreateChildWithName("Error",nullptr).setProperty("ErrorMsg", errorMsg, nullptr);
        }
        
        RendererType const renderMethod = static_cast<RendererType> ((int)parameters.state.getProperty( "RendererType", 0 ));
        
        std::string keywordArgs; // Python keyword arguments, filled in the switch statments
        std::string rendererClassName;
        
        switch( renderMethod )
        {
            case RendererType::HRIR:
            {
                rendererClassName = "DynamicHrirRenderer";
                String defaultHRIRbuildPath = defaultSrcPath+"/resources/SOFA/koln/HRIR_L2354.sofa";
                String defaultHRIRinstallPath = defaultInstallPath+"/resources/SOFA/koln/HRIR_L2354.sofa";
                
                if(parameters.state.getOrCreateChildWithName("HRIRSettings", nullptr)
                   .getProperty("HRIRsofaFile","").toString().toStdString().empty()){
                    std::cout<<" SOFA SIZE "<<File(defaultHRIRbuildPath).getSize()<<std::endl;
                    if(File(defaultHRIRbuildPath).exists() && File(defaultHRIRbuildPath).getSize()>=2350155){
                        //        printf("default HRIR exists in build directory");
                        parameters.state.getOrCreateChildWithName("HRIRSettings", nullptr)
                        .setProperty("HRIRsofaFile", defaultHRIRbuildPath,nullptr);
                    }else if(File(defaultHRIRinstallPath).exists() && File(defaultHRIRinstallPath).getSize()>1024){
                        //            printf("default HRIR exists in installation directory");
                        parameters.state.getOrCreateChildWithName("HRIRSettings", nullptr)
                        .setProperty("HRIRsofaFile", defaultHRIRinstallPath,nullptr);
                        
                    }
                    
                    //              isFirst=false;
                }
                std::string const sofaFile = parameters.state.getOrCreateChildWithName("HRIRSettings", nullptr)
                .getProperty("HRIRsofaFile","").toString().toStdString();
                
                bool const dynamicITD =  parameters.state.getOrCreateChildWithName("HRIRSettings", nullptr)
                .getProperty("HRIR_dynamicITD",false);
                bool const hrirInterpolation = parameters.state.getOrCreateChildWithName("HRIRSettings", nullptr)
                .getProperty("HRIR_interpolation",true);
                bool const filterCrossfading = parameters.state.getOrCreateChildWithName("HRIRSettings", nullptr)
                .getProperty("HRIR_crossfade",false);
                
                // Using generic file paths avoids problems with Python, including Unicode encodings of path with backslashes (especially \\U)
                std::string const sofaFileGeneric = boost::filesystem::path( sofaFile ).generic_string();
                
                static char const * kwArgsTemplate = "{ 'numberOfObjects': %d, 'sofaFile': '%s', 'headTracking': %d, 'dynamicITD': %d, 'hrirInterpolation': %d, 'filterCrossfading': %d }";
                
                keywordArgs = (boost::format( kwArgsTemplate )
                               % maxInputChannels % sofaFileGeneric % useHeadTracking % dynamicITD % hrirInterpolation % filterCrossfading).str();
                break;
            }
            case RendererType::BRIR:
            {
                rendererClassName = "ObjectToVirtualLoudspeakerRenderer";
                std::string const sofaFile = parameters.state.getOrCreateChildWithName( "BRIRSettings", nullptr )
                .getProperty( "BRIRsofaFile", "").toString().toStdString();
                std::string const lspFile = parameters.state.getOrCreateChildWithName( "BRIRSettings", nullptr )
                .getProperty( "BRIRLoudspeakerLayoutFile","").toString().toStdString();
                bool const dynamicITD = parameters.state.getOrCreateChildWithName( "BRIRSettings", nullptr )
                .getProperty( "BRIR_dynamicITD", false );
                bool const hrirInterpolation = parameters.state.getOrCreateChildWithName( "BRIRSettings", nullptr )
                .getProperty( "BRIR_interpolation", true );
                bool const filterCrossfading = parameters.state.getOrCreateChildWithName( "BRIRSettings", nullptr )
                .getProperty( "BRIR_crossfade", false );
                
                // maxInputChannels = parameters.state.getOrCreateChildWithName( "GeneralSettings", nullptr ).getProperty( "MaximumObjectNumber", 64 );
                
                int const irTruncationLength = parameters.state.getOrCreateChildWithName( "BRIRSettings", nullptr )
                .getProperty( "irTruncationLength", 8192 );
                
                // Using generic file paths avoids problems with Python, including Unicode encodings of path with backslashes (especially \\U)
                std::string const sofaFileGeneric = boost::filesystem::path( sofaFile ).generic_string();
                std::string const lspFileGeneric = boost::filesystem::path( lspFile ).generic_string();
                
                // TODO: provide option for object renderer options
                std::string const objectRendererOptions = parameters.state.getOrCreateChildWithName( "BRIRSettings", nullptr )
                .getProperty( "ObjectRendererOptions","{ }").toString().toStdString();
                
                static char const * kwArgsTemplate = "{ 'numberOfObjects': %d, 'sofaFile': '%s', 'loudspeakerConfiguration': '%s', 'headTracking': %d, 'dynamicITD': %d, 'hrirInterpolation': %d, 'filterCrossfading': %d, "
                "'objectRendererOptions': %s, 'irTruncationLength' : %d }";
                
                keywordArgs = (boost::format( kwArgsTemplate )
                               % maxInputChannels % sofaFileGeneric % lspFileGeneric % useHeadTracking % dynamicITD % hrirInterpolation % filterCrossfading % objectRendererOptions % irTruncationLength).str();
                break;
            }
            case RendererType::HOA:
            {
                rendererClassName = "HoaObjectToBinauralRenderer";
                std::string const sofaFile = parameters.state.getOrCreateChildWithName( "HOASettings", nullptr )
                .getProperty( "HOAsofaFile", "" ).toString().toStdString();
                
                // Using generic file paths avoids problems with Python, including Unicode encodings of path with backslashes (especially \\U)
                std::string const sofaFileGeneric = boost::filesystem::path( sofaFile ).generic_string();
                
                static char const * kwArgsTemplate = "{ 'numberOfObjects': %d, 'sofaFile': '%s',  'headTracking': %d }";
                
                keywordArgs = (boost::format( kwArgsTemplate ) % maxInputChannels % sofaFileGeneric % useHeadTracking ).str();
            }
            case RendererType::REND_MAX:
            {
            }
        } // switch
        
        
        
        
        if(internalHeadTracking){
            if(trackerComponent == nullptr){
                trackerComponent.reset( new TrackerComponent( *context,
                                                             "BST",              // component name (can be chosen freely)
                                                             nullptr,            // parent component (nulptr because this is top level)
                                                             std::string(),             // Optional module search path
                                                             headTrackingReceiverClassName,
                                                             headTrackingReceiverParameters,
                                                             "visr_bst"
                                                             )) ;
                
                componentHandlerTracker.reset(new visr_support::VISRComponentWrapper(*trackerComponent));
//                trackerFlow.reset(new rrl::AudioSignalFlow( *trackerComponent));
//                listener.reset(new dawsupport::VISRListenerPosWrapper(parameters, *trackerFlow.get(),2));
                
            }
        }
        
        
        
        
        //    // Destroy the old renderer first. This reduces memory consumption considerably,
        //    // and might avoid problems with singleton resources such as communication ports.
        component.reset();
        
        //      std::cout<<"MCOMPCONF"<<std::endl<<std::endl;;
        //      
        //      printf("className: %s\n kwargs: %s\n usehead: %d\n headrecname: %s\n headrecparam: %s\n ", rendererClassName.c_str(), keywordArgs.c_str(), useHeadTracking, headTrackingReceiverClassName.c_str(), headTrackingReceiverParameters.c_str());
        //      
        //      std::cout<<std::endl<<std::endl;;
        //    TopLevelComponentNoNet component ( *context,
        component.reset( new BinauralRendererComponent( *context,
                                                    "BST",              // component name (can be chosen freely)
                                                    nullptr,            // parent component (nulptr because this is top level)
                                                    maxInputChannels,
                                                    std::string( "visr_bst" ), // Name of Python module
                                                    rendererClassName,         // Class name (definition must be in Python module above)
                                                    std::string(),             // Positional arguments passed to the class's init__()           method
                                                    keywordArgs,               // Keyword arguments passed to the class's init__() method
                                                    std::string(),             // Optional module search path
                                                    8001,                      // sceneReceiverPort,
                                                    useHeadTracking,           // useTracking,
                                                    true,                      // exposeObjectVector
                                                    useHeadTracking,                     // bool exposeListenerPosition = false
                                                    headTrackingReceiverClassName,
                                                    headTrackingReceiverParameters,
                                                    "visr_bst"
                                                    )) ;
        
        
        componentHandler.reset(new visr_support::VISRComponentWrapperObjVector(parameters, *component,  views, 8001, dynamicHeadTrackingSelect));
        
        //      componentHandler.reset(new VISRComponentWrapperObjVector(parameters, component, 8001, dynamicHeadTrackingSelect));
        //      componentHandler->enableOBjectVectorHandling("objectVectorOut");
        //      if(useHeadTracking) componentHandler->enableListenerPositionHandling("listenerPositionOut");
        
    }
    catch( std::exception const &ex )
    {
        setupDone = false;
        String errorMsg;
        
        String excMsg = ex.what();
        if(excMsg.contains("Wrapper: Error while instantiating the Python object of component \"Renderer\": reason: ValueError: SOFA file does not exist."))
            errorMsg << "No SOFA file has been found, you can easily add one from the settings panel. Please refer to the documentation:\n (https://cvssp.org/data/s3a/public/VISRPluginSuite/html/tutorial_1.html#binaural-renderer-track) \n";
        else
            errorMsg << "Exception while instantiating binaural renderer:\n" << ex.what();
        
        
        parameters.state.getOrCreateChildWithName("Error",nullptr).setProperty("ErrorMsg", errorMsg, nullptr);
        std::cerr << errorMsg << std::endl;
        
        // this ensures that the exception is caught in the PluginReset method, therefore avoids releasing the suspended processing
        std::runtime_error error(errorMsg.toRawUTF8());
        throw error;
    }
    
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

AudioProcessorEditor* Processor::createEditor()
{
    return new Editor (this);
}


} // namespace binaural_renderer
} // namespace dawplugins
} // namespace visr_production_suite

