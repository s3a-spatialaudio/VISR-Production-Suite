#ifndef OBJECT_H_INCLUDED
#define OBJECT_H_INCLUDED

#include "OpenGLBase.h"
#include <libcore/processors/PluginProcessorBase.h>
#include <libcore/graphics/PluginLookAndFeel.h>
#include <libcore/PluginBinaryData.h>
#include <juce_opengl/juce_opengl.h>
#include <libcore/graphics/ErrorPanel.h>

namespace visr_production_suite
{
namespace core
{
namespace graphics
{

/**
 * Component for drawing an audio object view.
 * It also handles mouse dragging for manual object panning.
 * @author Giacomo Costantini giacomo.costantini3@gmail.com
 */
class CORE_LIBRARY_SYMBOL Object : public virtual OpenGLBase,
public juce::Timer,
public juce::ValueTree::Listener,
public juce::OpenGLRenderer
{
public:
    Object(processors::PluginProcessorBase& processor);
    ~Object();
    void timerCallback() override {
        openGLContextChild.triggerRepaint();
    };
    void renderOpenGL() override;
    void newOpenGLContextCreated() override{
        juce::Image im = juce::ImageCache::getFromMemory (PluginBinaryData::sphere320_new_png, PluginBinaryData::sphere320_new_pngSize);
        texture.loadImage (im);
    };
    void openGLContextClosing() override{};
    
protected:
    void valueTreePropertyChanged (juce::ValueTree& treeWhosePropertyHasChanged, const juce::Identifier& property) override;
    void valueTreeChildAdded (juce::ValueTree&, juce::ValueTree&) override{}
    void valueTreeChildRemoved (juce::ValueTree&, juce::ValueTree&, int) override {}
    void valueTreeChildOrderChanged (juce::ValueTree&, int, int) override {}
    void valueTreeParentChanged (juce::ValueTree&) override {}
    
    float radiusX,radiusY,xc,yc;
    float xCurr,yCurr,zCurr;
    float center,rb;
    float xpix,ypix;
    float srcx,srcy;
    float phi,theta;
    int obid;
    PluginLookAndFeel::SourcePalette col;
    
    /**
     * Defines how to update the involved parameters when dragging the source with the mouse.
     */
    void notifyParams();
    
    /**
     * Utility function to get the value of a parameter from the audio processor
     */
    float getParamValue(juce::String paramId);
    
    /**
     * Callback for defining what to do when a mouse button is pressed
     */
    void mouseDown (const juce::MouseEvent& e) override;
    
    /**
     * Callback for defining what to do when a mouse button is released
     */
    void mouseUp (const juce::MouseEvent& e) override;
    
    /**
     * Callback for defining what to do when dragging
     */
    void mouseDrag (const juce::MouseEvent& e) override;
    
    void resized() override;
    
private:
    const bool continuousRepainting = false;
    const float scale = 0.93f;
    juce::OpenGLContext openGLContextChild;
    SolidSphere sphere_source;
    
    juce::OpenGLTexture texture;
    processors::PluginProcessorBase& processor;
    juce::ScopedPointer<ErrorPanel> errorComponent;
    juce::ValueTree error;
};
    
} // namespace graphics
} // namespace core
} // namespace visr_production_suite

#endif
