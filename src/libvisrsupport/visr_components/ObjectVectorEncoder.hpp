#ifndef OBJECTVECTORENCODER_HPP_INCLUDED
#define OBJECTVECTORENCODER_HPP_INCLUDED

#include <libvisr/atomic_component.hpp>
#include <libvisr/parameter_input.hpp>
#include <libvisr/parameter_output.hpp>

#include <libpml/double_buffering_protocol.hpp>
#include <libpml/scalar_parameter.hpp>
#include <libpml/object_vector.hpp>
#include <libobjectmodel/point_source.hpp>

namespace visr_production_suite
{
namespace visr_support
{
namespace visr_components
{    
    
/**
 * Component to decode listener position data from messages (typically received from a network).
 * This component has neither audio inputs or outputs.
 */
class ObjectVectorEncoder: public visr::AtomicComponent
{
public:
    /**
     * Constructor.
     * @param context Configuration object containing basic execution parameters.
     * @param name The name of the component. Must be unique within the containing composite component (if there is one).
     * @param parent Pointer to a containing component if there is one. Specify \p nullptr in case of a top-level component.
     */
    explicit ObjectVectorEncoder( visr::SignalFlowContext const & context,
                                 char const * name,
                                 visr::CompositeComponent * parent = nullptr );
    
    /**
     * Disabled (deleted) copy constructor
     */
    ObjectVectorEncoder( ObjectVectorEncoder const & ) = delete;
    
    /**
     * Destructor.
     */
    ~ObjectVectorEncoder();
    
    /**
     * Method to initialise the component.
     * At the moment, there are arguments to customize the component, but this might change in the future.
     */
    void setup();
protected:
    /**
     * The process function. Decodes all messages arriving through the "messageInput" parameter input port and sends
     * them through the parameter output "positionOutput".
     */
    void process() override;
private:
    std::unique_ptr<visr::objectmodel::PointSource> mPs;
    std::unique_ptr<visr::pml::ObjectVector> scene;
    visr::ParameterInput< visr::pml::DoubleBufferingProtocol, visr::pml::ScalarParameter<float>> azPosInput;
    visr::ParameterInput< visr::pml::DoubleBufferingProtocol, visr::pml::ScalarParameter<float>> elPosInput;
    visr::ParameterInput< visr::pml::DoubleBufferingProtocol, visr::pml::ScalarParameter<float>> radiusPosInput;
    visr::ParameterInput< visr::pml::DoubleBufferingProtocol, visr::pml::ScalarParameter<int>> objectIDInput;
    float phi = 0.5;
    float theta = 0.5;
    float radius = 1.0;
    visr::ParameterOutput< visr::pml::DoubleBufferingProtocol, visr::pml::ObjectVector > mObjectVectorOut;
};

} // namespace extpointsource_renderer
} // namespace dawplugins
} // namespace visr_production_suite

#endif // #ifndef OBJECTVECTORENCODER_HPP_INCLUDED
