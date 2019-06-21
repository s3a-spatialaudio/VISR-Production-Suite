#include "ObjectVectorEncoder.hpp"
#include <libefl/cartesian_spherical_conversion.hpp>
#include <libefl/degree_radian_conversion.hpp>

namespace visr_production_suite
{
namespace visr_support
{
namespace visr_components
{
    
using namespace visr;
    
ObjectVectorEncoder::ObjectVectorEncoder( visr::SignalFlowContext const & context,
                                         char const * name,
                                         visr::CompositeComponent * parent /*= nullptr*/ )
: AtomicComponent( context, name, parent )
, azPosInput( "azPosInput", *this, visr::pml::EmptyParameterConfig() )
, elPosInput( "elPosInput", *this, visr::pml::EmptyParameterConfig() )
, radiusPosInput( "radiusPosInput", *this, visr::pml::EmptyParameterConfig() )
, objectIDInput( "objectIDInput", *this, visr::pml::EmptyParameterConfig() )
, mObjectVectorOut( "objectVectorOut", *this, visr::pml::EmptyParameterConfig() )
{
}

ObjectVectorEncoder::~ObjectVectorEncoder()
{
}

void ObjectVectorEncoder::setup()
{
    mPs.reset(new visr::objectmodel::PointSource(0));
    mPs->resetNumberOfChannels(1);
    mPs->setChannelIndex(0,mPs->id());
    mPs->setX(0.f);
    mPs->setY(0.f);
    mPs->setZ(0.f);
    mPs->setLevel(0.5);
    
    mPs->setGroupId(5);
    mPs->setPriority(5);
    scene.reset(new visr::pml::ObjectVector());
    scene->clear();
    scene->insert(*mPs );
}

void ObjectVectorEncoder::process()
{
    if( azPosInput.changed() || elPosInput.changed() || radiusPosInput.changed() || objectIDInput.changed() )
    {
        if(azPosInput.changed()){
            phi = efl::degree2radian(azPosInput.data().value());
        }
        
        if(elPosInput.changed()){
            theta = efl::degree2radian(elPosInput.data().value());
        }
        
        if(radiusPosInput.changed()){
            radius = (radiusPosInput.data().value());
        }
        if(objectIDInput.changed()){
            int newIDandChan = objectIDInput.data().value()-1;
            mPs.reset(new visr::objectmodel::PointSource(newIDandChan));
            mPs->resetNumberOfChannels(1);
            mPs->setChannelIndex(0,newIDandChan);
            
            mPs->setLevel(0.5);
            mPs->setGroupId(5);
            mPs->setPriority(5);
        }
        try
        {
            objectmodel::Object::Coordinate xPos, yPos, zPos;
            std::tie( xPos, yPos, zPos ) = efl::spherical2cartesian( phi, theta, radius );
            
            mPs->setX(xPos);
            mPs->setY(yPos);
            mPs->setZ(zPos);
            
            scene->clear();
            scene->insert( *mPs );
            azPosInput.resetChanged();
            elPosInput.resetChanged();
            radiusPosInput.resetChanged();
            objectIDInput.resetChanged();
        }
        catch( std::exception const & ex )
        {
            // Don't abort the program when receiving a corrupted message.
            std::cerr << "ObjectVectorEncoder: Error while decoding a position message: " << ex.what() << std::endl;
        }
        
        mObjectVectorOut.data() = *scene.get();
        mObjectVectorOut.swapBuffers();
    }
}
    
} // namespace visr_components
} // namespace visr_support
} // namespace visr_production_suite
