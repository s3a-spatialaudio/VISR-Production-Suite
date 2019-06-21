/* Copyright Institute of Sound and Vibration Research - All rights reserved */

#ifndef OBJECTRENDERER_HPP_INCLUDED
#define OBJECTRENDERER_HPP_INCLUDED

#include <libvisrsupport/visr_components/ObjectVectorEncoder.hpp>
#include <libsignalflows/core_renderer.hpp>
#include <libpml/double_buffering_protocol.hpp>
#include <libpml/scalar_parameter.hpp>
namespace visr_production_suite
{
namespace visr_support
{
namespace visr_components
{
    /**
     * Audio signal graph object for the VISR baseline renderer.
     */
class ObjectRenderer: public visr::CompositeComponent
    {
    public:
      /**
       * Constructor to create, initialise and interconnect all processing components.
       * @param context The signal flow context object containing information such as sampling frequency and period (block) size.
       * @param name The name of the component, used for identification and error reporting.
       * @param parent The containing component, if there is one. Use nullptr to mark this as the toplevel component.
       * @param loudspeakerConfiguration The configuration of the reproduction array, including the routing to physical output channels,
       * potentially virtual loudspeakers and subwoofer configuration.
       * @param numberOfInputs The number of inputs, i.e., the number of audio object signals
       * @param numberOfOutputs The number of output channels. This number can be higher than the number of loudspeakers plus the number of subwoofers if the
       * routing to output channels contains gaps. The output channels between 0 and \p numbeOfOutputs to which no signal is routed are configured to output zeros.
       * @param interpolationPeriod The interpolation period used in the VBAP gain matrix, i.e., the number of samples it takes to fade to a new gain value. Must be multiple of \p period.
       * @param diffusionFilters A matrix of floating-point values containing the the FIR coefficients of the decorrelation filter that creates diffuse sound components.
       * @param trackingConfiguration The configuration of the tracker (empty string disables tracking)
       * @param sceneReceiverPort The UDP port for receiving the scene data messages.
       * @param numberOfObjectEqSections The number of biquad sections alocated to each object signal.
       * @param reverbConfig A JSON message containing configuration options for the late reverberation part.
       *        - numReverbObjects (integer) The maximum number of reverb objects (at a given time)
       *        - lateReverbFilterLength (floating-point) The length of the late reverberation filter (in seconds)
       *        - discreteReflectionsPerObject (integer) The number of discrete reflections per reverb object.
       *        - lateReverbDecorrelationFilters (string) Absolute or relative file path (relative to start directory of the renderer) to a multichannel audio file (typically WAV)
       *          containing the filter coefficients for the decorrelation of the late part.
       * @param frequencyDependentPanning Flag specifiying whether the frequency-dependent VBAP algorithm shall be activated (true) or not (false)
       */
      explicit ObjectRenderer( visr::SignalFlowContext const & context,
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
                                bool frequencyDependentPanning );
      
      /**
       * Simplified constructor using default values for several parameters.
       */
      explicit ObjectRenderer( visr::SignalFlowContext const & context,
                                char const * name,
                                CompositeComponent * parent,
                                visr::panning::LoudspeakerArray const & loudspeakerConfiguration,
                                std::size_t numberOfInputs,
                                std::size_t numberOfOutputs );
      
      ~ObjectRenderer();
      
    private:
 
      visr::signalflows::CoreRenderer mCoreRenderer;
      std::unique_ptr<visr_support::visr_components::ObjectVectorEncoder> mObjectVectorEncoder;
      visr::AudioInput mInput;
      visr::AudioOutput mOutput;

      visr::ParameterInput< visr::pml::DoubleBufferingProtocol, visr::pml::ScalarParameter<float>> azPosInput;
      visr::ParameterInput< visr::pml::DoubleBufferingProtocol, visr::pml::ScalarParameter<float>> elPosInput;
      visr::ParameterInput< visr::pml::DoubleBufferingProtocol, visr::pml::ScalarParameter<float>> radiusPosInput;
         visr::ParameterInput< visr::pml::DoubleBufferingProtocol, visr::pml::ScalarParameter<int>> objectIDInput;
    };
    
} // namespace visr_components
} // namespace visr_support
} // namespace visr_production_suite
#endif // OBJECTRENDERER_HPP_INCLUDED
