#ifndef SCENE_H_INCLUDED
#define SCENE_H_INCLUDED

#include "../export_symbols.hpp"
#include "OpenGLBase.h"
#include <libcore/processors/PluginProcessorScene.h>
#include <libvisrsupport/VISRObjectVectorObserver.h>
#include <libcore/graphics/PluginLookAndFeel.h>
#include <libcore/PluginBinaryData.h>
#include <juce_opengl/juce_opengl.h>
#include <libobjectmodel/object_vector.hpp>
#include <libpanning/LoudspeakerArray.h>
#include <libcore/graphics/ErrorPanel.h>
#include <map>
#include <list>
#include <mutex>

/* This is still needed because we are using old glu utilities such as glbegin.
 This may also cause the "gl.h and gl3.h are both included" warning on MacOS because glu.h includes gl.h, whereas juce_opengl.h calls gl3.h if OpenGL 3 is present.
 Those includes can (and should?) be avoided if the old glu functions are replaced with openGLcontext.extensions functions
 */
#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#ifdef _WIN32
#include <windows.h>
#endif
//#include <GL/gl.h>
#include <GL/glu.h>
#endif

namespace visr_production_suite
{
namespace core
{
namespace graphics
{

/**
 * Component for drawing an object-based audio scene view.
 * @author Giacomo Costantini giacomo.costantini3@gmail.com
 */
class CORE_LIBRARY_SYMBOL Scene :
    public OpenGLBase,
    public VISRObjectVectorObserver,
    public juce::OpenGLRenderer,
    public juce::ValueTree::Listener
{
public:
    Scene(processors::PluginProcessorScene& p);
    ~Scene();
    
protected:
    processors::PluginProcessorScene& processor;
    void update() override;
    
    const float scale = 0.93f;
    
    void renderOpenGL() override;
    void newOpenGLContextCreated() override{
        juce::Image im = juce::ImageCache::getFromMemory (PluginBinaryData::sphere320_new_png, PluginBinaryData::sphere320_new_pngSize);
        texture.loadImage (im);
    };
    
    void openGLContextClosing() override{
		texture.release();
	};

    
    void valueTreePropertyChanged (juce::ValueTree& treeWhosePropertyHasChanged, const juce::Identifier& property) override;
    void valueTreeChildAdded (juce::ValueTree&, juce::ValueTree&) override{}
    void valueTreeChildRemoved (juce::ValueTree&, juce::ValueTree&, int) override {}
    void valueTreeChildOrderChanged (juce::ValueTree&, int, int) override {}
    void valueTreeParentChanged (juce::ValueTree&) override {}
    
    void resized() override;

private:
    
    const bool continuousRepainting = false;
    visr::objectmodel::ObjectVector scene;
    
    juce::ScopedPointer<core::graphics::ErrorPanel> errorComponent;
    juce::ValueTree error;
    std::mutex sceneLockRenderer;
    PluginLookAndFeel::SourcePalette col;
    
    
    juce::OpenGLContext openGLContextSM;
    juce::OpenGLTexture texture;
    
    float radiusX,radiusY,xc,yc;
};
    
} // namespace graphics
} // namespace core
} // namespace visr_production_suite

#endif
