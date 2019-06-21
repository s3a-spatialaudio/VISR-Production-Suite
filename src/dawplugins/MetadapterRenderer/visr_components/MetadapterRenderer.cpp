/* Copyright Institute of Sound and Vibration Research - All rights reserved */

#include "MetadapterRenderer.hpp"

#include <libpythonsupport/gil_ensure_guard.hpp>
#include <libpythonsupport/initialisation_guard.hpp>
#include <libpythoncomponents/wrapper.hpp>

#include <boost/format.hpp>

#include <ciso646>
#include <string>
namespace visr_production_suite
{
namespace dawplugins
{
namespace metadapter_renderer
{

MetadapterRenderer::MetadapterRenderer( visr::SignalFlowContext const & context,
                                       char const * name,
                                       visr::CompositeComponent * parent,
                                       visr::panning::LoudspeakerArray const & loudspeakerConfiguration,
                                       std::size_t numberOfInputs,
                                       std::size_t numberOfOutputs
                                       )
: MetadapterRenderer( context, name, parent,
                     loudspeakerConfiguration,
                     numberOfInputs, numberOfOutputs,
                     4096ul,
                     visr::efl::BasicMatrix<visr::SampleType>(numberOfOutputs, 0, visr::cVectorAlignmentSamples ),
                     std::string(), 4242, 0 /*number of EQ sections*/,
                     std::string(), false, std::string(),std::string(), std::string())
{}


MetadapterRenderer::MetadapterRenderer( visr::SignalFlowContext const & context,
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
                                       bool frequencyDependentPanning,
                                       std::string const & metadapterConfig,
                                       std::string const & oscControlPort,
                                       std::string const & jsonControlPort)
: CompositeComponent( context, name, parent )
, mSceneDecoder( nullptr )
, mCoreRenderer( context,
                "CoreRenderer",
                this,
                loudspeakerConfiguration,
                numberOfInputs,
                numberOfOutputs,
                interpolationPeriod,
                diffusionFilters,
                trackingConfiguration,
                numberOfObjectEqSections,
                reverbConfig,
                frequencyDependentPanning )
, mObjectVectorInput( "objectVector", *this, visr::pml::EmptyParameterConfig() )
, mObjectVectorOutput( "objectVectorOutput", *this, visr::pml::EmptyParameterConfig())
, mInput( "input", *this, numberOfInputs )
, mOutput( "output", *this, numberOfOutputs )
{
    audioConnection( mInput, mCoreRenderer.audioPort( "audioIn") );
    audioConnection( mCoreRenderer.audioPort( "audioOut"), mOutput );
    
    if( metadapterConfig.empty() )
    {
        parameterConnection( mObjectVectorInput, mCoreRenderer.parameterPort( "objectDataInput" )  );
        parameterConnection( mObjectVectorInput, mObjectVectorOutput);
    }else{

        bool const useOscControlPort{ not oscControlPort.empty() };
        bool const useJSONControlPort{ not jsonControlPort.empty() };
        std::string const formatString = "{'processorConfig': '%s', 'objectVectorInput': True, 'objectVectorOutput': True, 'oscControlPort': %d, 'jsonControlPort': %d }";
        std::string const kwArgs = str( boost::format( formatString ) % metadapterConfig % useOscControlPort % useJSONControlPort );
        visr::pythonsupport::InitialisationGuard::initialise();
        
        mSceneDecoder.reset( new visr::pythoncomponents::Wrapper( context, "Metadapter", this,
                                                                 "metadapter.visrintegration",     //  char const * moduleName,
                                                                 "Component",     //  char const * componentClassName,
                                                                 "",               //  char const * positionalArguments = "",
                                                                 kwArgs.c_str(),   //  char const * keywordArguments = "",
                                                                 ""                // No module search path
                                                                 ) );
        parameterConnection( mObjectVectorInput, mSceneDecoder->parameterPort( "objectIn" ) );
        parameterConnection( mSceneDecoder->parameterPort( "objectOut" ), mCoreRenderer.parameterPort( "objectDataInput" ) );
        parameterConnection( mSceneDecoder->parameterPort( "objectOut" ), mObjectVectorOutput);

        if( useOscControlPort ){
            auto const portNumber = std::stoi( oscControlPort );
            mMetadapterOscControlReceiver.reset( new visr::rcl::UdpReceiver( context, "metadapterOscControlPort", this,
                                                                            portNumber, visr::rcl::UdpReceiver::Mode::Asynchronous ) );
            parameterConnection( mMetadapterOscControlReceiver->parameterPort("messageOutput"), mSceneDecoder->parameterPort("oscControlIn") );
        }
        
        if( useJSONControlPort ){
            auto const portNumber = std::stoi( jsonControlPort );
            mMetadapterJSONControlReceiver.reset( new visr::rcl::UdpReceiver( context, "metadapterJSONControlPort", this,
                                                                            portNumber, visr::rcl::UdpReceiver::Mode::Asynchronous ) );
            parameterConnection( mMetadapterJSONControlReceiver->parameterPort("messageOutput"), mSceneDecoder->parameterPort("jsonControlIn") );
        }
    }
}

MetadapterRenderer::~MetadapterRenderer( )
{
}

} // namespace metadapter_renderer
} // namespace dawplugins
} // namespace visr_production_suite
