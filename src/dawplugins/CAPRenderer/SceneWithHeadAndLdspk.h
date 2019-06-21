#ifndef CAPRENDERERPannerUI
#define CAPRENDERERPannerUI

#include "PluginProcessor.h"
#include <libcore/graphics/SceneWithLoudspeakers.h>

namespace visr_production_suite
{
namespace dawplugins
{
namespace cap_renderer
{

class SceneWithHeadAndLdspk :
    public core::graphics::SceneWithLoudspeakers
{
public:
    SceneWithHeadAndLdspk(Processor& p);
    ~SceneWithHeadAndLdspk();
protected:
    void paint(juce::Graphics& g) override;

    void renderOpenGL() override;
private:
    Processor& processor;
};
    
} // namespace cap_renderer
} // namespace dawplugins
} // namespace visr_production_suite

#endif
