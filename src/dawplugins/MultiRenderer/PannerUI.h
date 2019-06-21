#ifndef SCENERENDERER_PannerUI
#define SCENERENDERER_PannerUI

#include "PluginProcessor.h"
#include <libdawutilities/PannerUILdspk.h>
#include <libdawutilities/Lookandfeel/PluginLookAndFeel.h>

namespace visr
{
namespace dawplugins
{
namespace multi_renderer
{
    
class PannerUI : public PannerUILdspk
{
public:
    PannerUI(Processor * p);
    ~PannerUI();
    Processor* processor;
    void setLoudspeakerArray() override;
    void paint(juce::Graphics& g) override;
    PluginLookAndFeel::SourcePalette col;
};
    
} // namespace multi_renderer
} // namespace dawplugins
} // namespace visr

#endif
