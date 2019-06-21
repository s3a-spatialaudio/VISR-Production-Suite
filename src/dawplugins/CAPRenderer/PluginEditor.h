#ifndef CAPRENDERER_EDITOR_H_INCLUDED
#define CAPRENDERER_EDITOR_H_INCLUDED

#include "JuceHeader.h"
#include "PluginProcessor.h"
#include <libcore/graphics/PluginEditorLoudspeakers.h>
#include "SceneWithHeadAndLdspk.h"

namespace visr_production_suite
{
namespace dawplugins
{
namespace cap_renderer
{

class Editor  : public core::graphics::PluginEditorLoudspeakers
{
public:
    Editor (Processor* op);
    ~Editor();
    
    Processor& getProcessor(){return op;}

protected:
    void resized() override;

private:
    Processor& op;
    
    
    juce::ScopedPointer<juce::ImageButton> plugtitle;
    
    /**
     * An object-based audio scene view with loudspeakers
     */
    juce::ScopedPointer<SceneWithHeadAndLdspk> audioSceneHeadAndLdspkView;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Editor)
};
    
} // namespace cap_renderer
} // namespace dawplugins
} // namespace visr_production_suite

#endif
