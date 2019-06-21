#include "JuceHeader.h"
#ifndef IDS_H_INCLUDED
#define IDS_H_INCLUDED

/**
 * This is a convenient way of having const strings which are shared throughout
 * the different libraries/plugins.
 * @author Giacomo Costantini giacomo.costantini3@gmail.com
 */
namespace visr_production_suite
{
namespace core
{

namespace IDs
{
#define DECLARE_ID(name) const juce::Identifier name (#name);
    DECLARE_ID (objectVector)
    DECLARE_ID (pointSource)
    DECLARE_ID (pointSourceExtent)
    DECLARE_ID (pointSourceWithReverb)
    
    DECLARE_ID (objID)
    DECLARE_ID (azimuth)
    DECLARE_ID (elevation)
    DECLARE_ID (radius)
    DECLARE_ID (width)
    DECLARE_ID (height)
    DECLARE_ID (depth)
    DECLARE_ID (diffuseness)
    DECLARE_ID (objLevel)
    
    DECLARE_ID (loadRoom)
    DECLARE_ID (loadJson)
    DECLARE_ID (numberOfReflections)
    
    // PointSouceWithReverb Param IDs
    DECLARE_ID (rPositionX)
    DECLARE_ID (rPositionY)
    DECLARE_ID (rPositionZ)
    DECLARE_ID (rDelay)
    DECLARE_ID (rLevel)
    DECLARE_ID (rFrequency)
    DECLARE_ID (rResonance)
    DECLARE_ID (rFilterType)
    
    DECLARE_ID (onSetDelay)
    DECLARE_ID (r_filter_pole_)
    DECLARE_ID (r_filter_pole_1)
    DECLARE_ID (r_filter_pole_2)
    DECLARE_ID (r_filter_pole_3)
    DECLARE_ID (r_filter_pole_4)
    DECLARE_ID (r_filter_pole_5)
    DECLARE_ID (r_filter_pole_6)
    DECLARE_ID (r_filter_pole_7)
    DECLARE_ID (r_filter_pole_8)
    DECLARE_ID (r_filter_pole_9)
    DECLARE_ID (r_filter_pole_10)
    DECLARE_ID (lateAttack_band)
    DECLARE_ID (lateDecay_band)
    DECLARE_ID (lateLevel_band)
    DECLARE_ID (lateAttack_band1)
    DECLARE_ID (lateDecay_band1)
    DECLARE_ID (lateLevel_band1)
    DECLARE_ID (lateAttack_band2)
    DECLARE_ID (lateDecay_band2)
    DECLARE_ID (lateLevel_band2)
    DECLARE_ID (lateAttack_band3)
    DECLARE_ID (lateDecay_band3)
    DECLARE_ID (lateLevel_band3)
    DECLARE_ID (lateAttack_band4)
    DECLARE_ID (lateDecay_band4)
    DECLARE_ID (lateLevel_band4)
    DECLARE_ID (lateAttack_band5)
    DECLARE_ID (lateDecay_band5)
    DECLARE_ID (lateLevel_band5)
    DECLARE_ID (lateAttack_band6)
    DECLARE_ID (lateDecay_band6)
    DECLARE_ID (lateLevel_band6)
    DECLARE_ID (lateAttack_band7)
    DECLARE_ID (lateDecay_band7)
    DECLARE_ID (lateLevel_band7)
    DECLARE_ID (lateAttack_band8)
    DECLARE_ID (lateDecay_band8)
    DECLARE_ID (lateLevel_band8)
    DECLARE_ID (lateAttack_band9)
    DECLARE_ID (lateDecay_band9)
    DECLARE_ID (lateLevel_band9)
    
    
    DECLARE_ID (listenerPosX)
    DECLARE_ID (listenerPosY)
    DECLARE_ID (listenerPosZ)
    DECLARE_ID (yaw)
    DECLARE_ID (pitch)
    DECLARE_ID (roll)
    
#undef DECLARE_ID
} // namespace IDs
} // namespace core
} // namespace visr_production_suite

#endif



