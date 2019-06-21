#ifndef SceneWithLoudspeakers_H_INCLUDED
#define SceneWithLoudspeakers_H_INCLUDED

#include <libcore/processors/PluginProcessorLoudspeakers.h>

#include "Scene.h"
#include <libpanning/LoudspeakerArray.h>
#include <map>
#include <list>

namespace visr_production_suite
{
namespace core
{
namespace graphics
{

/**
 * Component for drawing a loudspeaker layout view.
 * It inherits from Scene, which draws the underlying audio scene view.
 * @author Giacomo Costantini giacomo.costantini3@gmail.com
 */
class CORE_LIBRARY_SYMBOL SceneWithLoudspeakers :
    public Scene
{
public:
    SceneWithLoudspeakers(processors::PluginProcessorLoudspeakers& p);
    ~SceneWithLoudspeakers();
    
    std::list<float> elevs;
    
    /**
     * Function to enable the drawing of a ring of Loudspeakers
     * @param elev elevation of the ring to be enabled
     */
    void enableRing(std::string elev);
    
    /**
     * Function to disable the drawing of a ring of Loudspeakers
     * @param elev elevation of the ring to be disabled
     */
    void disableRing(std::string elev);
    
    /**
     * Resets and repaints the loudspeaker layout.
     * It should be called everytime the loudspeaker layout is changed.
     */
    void resetLayout();
    
protected:
    void paint(juce::Graphics& g) override;
    
    
private:
    processors::PluginProcessorLoudspeakers& processor;
    
    visr::panning::LoudspeakerArray loudspeakerArray;
    void setLoudspeakerArray();
    
    std::map<float, juce::Colour> ringColour;
    std::map<float, bool> ringState;
};
    
} // namespace graphics
} // namespace core
} // namespace visr_production_suite

#endif
