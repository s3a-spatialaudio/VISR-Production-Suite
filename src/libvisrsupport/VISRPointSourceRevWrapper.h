#ifndef VISRPOINTSOURCEREVWRAPPER_H_INCLUDED
#define VISRPOINTSOURCEREVWRAPPER_H_INCLUDED

#include <libobjectmodel/point_source_with_reverb.hpp>
#include <libobjectmodel/point_source_with_reverb_parser.hpp>

#include <librbbl/float_sequence.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <libobjectmodel/point_source_parser.hpp>

#include "VISRObjectWrapper.h"

namespace visr_production_suite
{
namespace visr_support
{

/**
 * This class is a wrapper of a point-source-with-reverb audio object.
 * Contains features for handling the specific metadata of a point-source-with-reverb object.
 * @author Giacomo Costantini giacomo.costantini3@gmail.com
 */
class VISRSUPPORT_LIBRARY_SYMBOL VISRPointSourceRevWrapper :
    public VISRObjectWrapper, visr::objectmodel::PointSourceParser
{
public:
    /**
     * [VISRPointSourceRevWrapper description]
     * @param dataModel [description]
     */
    VISRPointSourceRevWrapper(juce::AudioProcessorValueTreeState& dataModel, boost::property_tree::ptree& roomIRTree);

    /**
     * VISRPointSourceRevWrapper description Destructor
     */
    ~VISRPointSourceRevWrapper();

private:

    /**
     * This function parses JSON into a PointSourceWithReverb object before being encoded, found in resources/RoomIRs.json File.
     * It reads JSON into a boost::property_tree::ptree, which is then parsed by parseRoom function.
     * @param src [description]
     */
    void handleRoomChangeRequest(visr::objectmodel::PointSourceWithReverb & src);

    /**
     * Detects any parameter changes made by the user through listening.
     * @param pID    [description]
     * @param newVal [description]
     */
    void VISRObjectChanged(const juce::String& pID, float newVal) override;

    /**
     * Mutex lock insertion.
     * @param obj [description]
     */
    void VISRObjectReceived(visr::objectmodel::Object& obj) override;

    /**
     * The encapsulated point-source-with-reverb.
     */
    std::unique_ptr<visr::objectmodel::PointSourceWithReverb> pointSrcWithRev;

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
     * parse function is based on the visr::objectmodel::point_source_with_reverb_parser, except it omits inherited object parsing, because it needs unique channel and ID properties to be ignored.
     * @param tree property tree to read from.
     * @param src object that is parsed to.
     */
    void parse(boost::property_tree::ptree const & tree, visr::objectmodel::Object & src);

    /**
     * TODO: Remove. Unused - Handled by the handleRoomChangeRequest
     * [loadRoom description]
     * @param tree     [description]
     * @param src      [description]
     * @param roomName [description]
     */
    void loadRoom( boost::property_tree::ptree const & tree, visr::objectmodel::Object & src, std::string roomName );

    /**
     * Property tree that is populated by the roomIRLibraryFile json file.
     */
    boost::property_tree::ptree roomIRLibraryTree;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VISRPointSourceRevWrapper)
};
} // namespace visr_support
} // namespace visr_production_suite

#endif  // VISRPOINTSOURCEREVWRAPPER_H_INCLUDED
