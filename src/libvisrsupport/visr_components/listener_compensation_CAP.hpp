#ifndef __S3A_renderer_dsp__listenerCompensationCAP__
#define __S3A_renderer_dsp__listenerCompensationCAP__

#include <libvisr/atomic_component.hpp>
#include <libvisr/parameter_input.hpp>
#include <libvisr/parameter_output.hpp>
#include <libpanning/LoudspeakerArray.h>
#include <libefl/basic_vector.hpp>

#include <libpml/double_buffering_protocol.hpp>
#include <libpml/listener_position.hpp>
#include <libpml/vector_parameter.hpp>

namespace visr_production_suite
{
namespace visr_support
{
namespace visr_components
{

class ListenerCompensationCAP: public visr::AtomicComponent
{
public:
  using SampleType = visr::SampleType;
public:
  /**
   * Constructor.
   * @param context Configuration object containing basic execution parameters.
   * @param name The name of the component. Must be unique within the containing composite component (if there is one).
   * @param parent Pointer to a containing component if there is one. Specify \p nullptr in case of a top-level component.
   * @param arrayConfig The loudspeaker configuration.
   */
  explicit ListenerCompensationCAP( visr::SignalFlowContext const & context,
                                 char const * name,
                                 visr::CompositeComponent * parent,
                                 visr::panning::LoudspeakerArray const & arrayConfig );

  /**
   * Disabled (deleted) copy constructor
   */
  ListenerCompensationCAP(ListenerCompensationCAP const &) = delete;

  /**
   * The process function.
   * It takes a listener position as input and calculates a gain vector and a delay vector.
   */
  void process() override;

private:
  std::size_t getNumSpeakers( )  const
  {
    return m_array.getNumSpeakers( );
  }

  int setListenerPosition( Afloat x, Afloat y, Afloat z )
  { //assigning the position of the listener
    m_listenerPos.set( x, y, z );
    return 0;
  }

  /**
   * Internal method to calculate the compensation gains.
   * @param [out] gainComp The result vector for the calculated gains (linear scale). It must have the dimension 'numberOfLoudspeakers'.
   * @return 0 in case of success.
   */
  int calcGainComp( visr::efl::BasicVector<Afloat> & gainComp ); // this function calculates the gain compensation

  /**
  * Internal method to calculate the compensation delays.
  * @param [out] delayComp The result vector for the calculated delays (in seconds). It must have the dimension 'numberOfLoudspeakers'.
  * @return 0 in case of success.
  */
  int calcDelayComp( visr::efl::BasicVector<Afloat> & delayComp ); // this function calculates the delay compensation

  visr::panning::LoudspeakerArray m_array; //passing the address of the loudspeaker array
  visr::panning::XYZ m_listenerPos; //position of the listener
  std::size_t const mNumberOfLoudspeakers;

  visr::ParameterInput<visr::pml::DoubleBufferingProtocol, visr::pml::ListenerPosition > mPositionInput;
  visr::ParameterOutput<visr::pml::DoubleBufferingProtocol, visr::pml::VectorParameter<Afloat> > mGainOutput;
  visr::ParameterOutput<visr::pml::DoubleBufferingProtocol, visr::pml::VectorParameter<Afloat> > mDelayOutput;
};

    
} // namespace visr_components
} // namespace visr_support
} // namespace visr_production_suite

#endif /* __S3A_renderer_dsp__listenerCompensationCAP__ */
