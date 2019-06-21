#ifndef VISRPOINTSOURCEWRAPPER_H_INCLUDED
#define VISRPOINTSOURCEWRAPPER_H_INCLUDED

#include <libobjectmodel/point_source.hpp>
#include "VISRObjectWrapper.h"

namespace visr_production_suite
{
namespace visr_support
{

/**
 * This class is a wrapper of a point source audio object.
 * Contains features for handling the specific metadata of a point source.
 * @author Giacomo Costantini giacomo.costantini3@gmail.com
 */
class VISRSUPPORT_LIBRARY_SYMBOL VISRPointSourceWrapper :
public VISRObjectWrapper
{
public:
    
    VISRPointSourceWrapper(juce::AudioProcessorValueTreeState& dataModel);
    ~VISRPointSourceWrapper();
    
private:
    void VISRObjectChanged(const juce::String& pID, float newVal) override;
    void VISRObjectReceived(visr::objectmodel::Object& obj) override;
    
    /**
     * The encapsulated point source.
     */
    std::unique_ptr<visr::objectmodel::PointSource> pointSrc;
    
    /**
     * The azimuthal angle of the point source's position with respect to the listener's head vertical plane.
     * It is expressed in radians.
     */
    float phi = 0.f;
    /**
     * The elevation angle of the point source's position with respect to the listener's head horizontal plane.
     * It is expressed in radians.
     */
    float theta = 0.f;
    /**
     * The distance from the center of an hypothethical sphere centred on the listener's head, expressed in meters
     */
    float radius = 1.f;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VISRPointSourceWrapper)
};
} // namespace visr_support
} // namespace visr_production_suite

#endif  // VISRPOINTSOURCEWRAPPER_H_INCLUDED
