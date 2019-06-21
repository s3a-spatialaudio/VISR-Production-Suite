#ifndef SceneWithHeadOrientation_H_INCLUDED
#define SceneWithHeadOrientation_H_INCLUDED

#include "Scene.h"
#include <libcore/processors/PluginProcessorScene.h>
#include <libcore/graphics/PluginLookAndFeel.h>

namespace visr_production_suite
{
namespace core
{
namespace graphics
{

/**
 * Component for drawing a head orientation view with arrows.
 * It inherits from Scene, which draws the underlying audio scene view.
 * @author Giacomo Costantini giacomo.costantini3@gmail.com
 */
class SceneWithHeadOrientation :
    public core::graphics::Scene
{
public:
    SceneWithHeadOrientation(core::processors::PluginProcessorScene& p);
    ~SceneWithHeadOrientation();
    
protected:
    void paint(juce::Graphics& g) override;
    void renderOpenGL() override;  
private:
    core::processors::PluginProcessorScene& processor;
};

} // namespace graphics
} // namespace core
} // namespace visr_production_suite

#endif
