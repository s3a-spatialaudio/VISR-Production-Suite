// avoid annoying warning about unsafe STL functions.
#ifdef _MSC_VER
#pragma warning(disable: 4996)
#endif

#include "listener_compensation_CAP.hpp"

#include <libpml/vector_parameter_config.hpp>

// Uncomment to get debug output
//#define DEBUG_LISTENER_COMPENSATIONCAP 1

// TODO: Move definition to a separate file.
#define c_0 344

namespace visr_production_suite
{
namespace visr_support
{
namespace visr_components
{

ListenerCompensationCAP::ListenerCompensationCAP( visr::SignalFlowContext const & context,
                                                 char const * name,
                                                 visr::CompositeComponent * parent,
                                                 visr::panning::LoudspeakerArray const & arrayConfig )
: AtomicComponent( context, name, parent )
, m_listenerPos( 0.0f, 0.0f, 0.0f )
, mNumberOfLoudspeakers( arrayConfig.getNumRegularSpeakers() )
, mPositionInput( "positionInput", *this, visr::pml::EmptyParameterConfig() )
, mGainOutput( "gainOutput", *this )
, mDelayOutput( "delayOutput", *this)
{
    m_array = arrayConfig;
    visr::pml::VectorParameterConfig const vectorConfig( mNumberOfLoudspeakers );
    
    mGainOutput.setParameterConfig( vectorConfig );
    mDelayOutput.setParameterConfig( vectorConfig );
}

void ListenerCompensationCAP::process()
{
    if( mPositionInput.changed() )
    {
        visr::pml::ListenerPosition const & pos( mPositionInput.data());
        visr::efl::BasicVector<SampleType> & gains( mGainOutput.data());
        visr::efl::BasicVector<SampleType> & delays( mDelayOutput.data());
        
        if (gains.size() != mNumberOfLoudspeakers or delays.size() != mNumberOfLoudspeakers)
        {
            throw std::invalid_argument("ListenerCompensationCAP::process(): The size of the gain or delay vector does not match the number of loudspeaker channels.");
        }
        
        setListenerPosition(pos.x(), pos.y(), pos.z());
        if( calcGainComp( gains ) != 0 )
        {
            throw std::runtime_error("ListenerCompensationCAP::process(): calcGainComp() failed.");
        }
        if (calcDelayComp( delays ) != 0)
        {
            throw std::runtime_error("ListenerCompensationCAP::process(): calcDelayComp() failed.");
        }
        
#ifdef DEBUG_LISTENER_COMPENSATIONCAP
        std::cout << "DelayVector Source Gain: ";
        std::copy( gains.data(), gains.data()+gains.size(), std::ostream_iterator<float>(std::cout, " ") );
        std::cout << "Delay [s]: ";
        std::copy( delays.data(), delays.data()+delays.size(), std::ostream_iterator<float>(std::cout, " ") );
        std::cout << std::endl;
#endif // DEBUG_LISTENER_COMPENSATIONCAPCAP
        
        mPositionInput.resetChanged();
        mGainOutput.swapBuffers();
        mDelayOutput.swapBuffers();
    }
}

int ListenerCompensationCAP::calcGainComp( visr::efl::BasicVector<Afloat> & gainComp )
{
    visr::panning::XYZ l1;
    Afloat rad = 0.0f, /*max_rad = 0.0f,*/ x = 0.0f, y = 0.0f, z = 0.0f;
    
    //setting listener position
    x = m_listenerPos.x;
    y = m_listenerPos.y;
    z = m_listenerPos.z;
    
    for (std::size_t i = 0; i < m_array.getNumSpeakers(); i++) {
        
        l1 = m_array.getPosition( i );
        
        rad = std::sqrt(std::pow((l1.x - x), 2.0f) + std::pow((l1.y - y), 2.0f) + std::pow((l1.z - z), 2.0f));
        
        gainComp[i] = rad;
    }
    return 0;
}


int ListenerCompensationCAP::calcDelayComp( visr::efl::BasicVector<Afloat> & delayComp )
{
    
    Afloat rad=0.0f, max_rad=0.0f, x=0.0f, y=0.0f, z=0.0f;
    visr::panning::XYZ l1;
    
    //setting listener position
    x = m_listenerPos.x;
    y = m_listenerPos.y;
    z = m_listenerPos.z;
    
    for (std::size_t i = 0; i < m_array.getNumSpeakers(); i++) {
        
        l1 = m_array.getPosition( i );
        
        rad = std::sqrt(std::pow((l1.x-x),2.0f) + std::pow((l1.y-y),2.0f) + std::pow((l1.z-z),2.0f));
        
        delayComp[i] = rad;
        
        if (delayComp[i]>max_rad)
            max_rad = delayComp[i];
    }
    
    for ( std::size_t i = 0; i < m_array.getNumSpeakers(); i++){
        
        delayComp[i] = std::abs(delayComp[i]-max_rad)/c_0;
    }
    return 0;
}
    
} // namespace visr_components
} // namespace visr_support
} // namespace visr_production_suite
