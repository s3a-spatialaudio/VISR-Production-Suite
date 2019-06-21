#ifndef VISRPOINTSOURCEEXTWRAPPER_H_INCLUDED
#define VISRPOINTSOURCEEXTWRAPPER_H_INCLUDED

#include <libobjectmodel/point_source_extent.hpp>
#include "VISRObjectWrapper.h"

namespace visr_production_suite
{
namespace visr_support
{

/**
 * This class is a wrapper of a point-source-with-extent audio object.
 * Contains features for handling the specific metadata of a point-source-with-extent.
 * @author Giacomo Costantini giacomo.costantini3@gmail.com
 */
class VISRSUPPORT_LIBRARY_SYMBOL VISRPointSourceExtWrapper :
public VISRObjectWrapper
{
public:
    VISRPointSourceExtWrapper(juce::AudioProcessorValueTreeState& dataModel);
    ~VISRPointSourceExtWrapper();
    
    
private:
    void VISRObjectChanged(const juce::String& pID, float newVal) override;
    void VISRObjectReceived(visr::objectmodel::Object& obj) override;
    
    
    /**
     * The encapsulated point-source-with-extent.
     */
    std::unique_ptr<visr::objectmodel::PointSourceExtent> pointSrcExt;
    
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
    
    /**
     * The width of the point source's extent.
     * It is expressed in degrees.
     */
    float width = 0.f;
    
    /**
     * The height of the point source's extent.
     * It is expressed in degrees.
     */
    float height = 0.f;
    
    /**
     * The depth of the point source's extent.
     * It is expressed in meters.
     */
    float depth = 0.f;
    
    
    /**
     * The diffusion of the point source.
     * It is expressed as a percentage.
     */
    float diffuseness = 0.f;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VISRPointSourceExtWrapper)
};

} // namespace visr_support
} // namespace visr_production_suite

#endif  // VISRPOINTSOURCEEXTWRAPPER_H_INCLUDED
