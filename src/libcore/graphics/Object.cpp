#include "Object.h"
#include <libcore/IDs.h>
#include <libefl/cartesian_spherical_conversion.hpp>
#include <libefl/degree_radian_conversion.hpp>

namespace visr_production_suite
{
namespace core
{
namespace graphics
{

using namespace juce;

Object::Object(processors::PluginProcessorBase& proc) : OpenGLBase(),
sphere_source(0.038f, 12, 12),
processor(proc)
{
    openGLContextChild.setRenderer (this);
    
    /**************************************************************/
    //    openGLContextChild.setComponentPaintingEnabled (false);
    openGLContextChild.setComponentPaintingEnabled (true);
    openGLContextChild.setMultisamplingEnabled(true);
    if(continuousRepainting)openGLContextChild.setContinuousRepainting(true);
    else openGLContextChild.setContinuousRepainting(false);

    openGLContextChild.attachTo (*this);
    
    if(!continuousRepainting) startTimer(50);
    
    errorComponent = new ErrorPanel();
    errorComponent->setMouseClickGrabsKeyboardFocus(false);
    errorComponent->setInterceptsMouseClicks(true,true);
    addChildComponent(errorComponent);
    errorComponent->setAlwaysOnTop(true);
    
    error = processor.parameters.state.getOrCreateChildWithName("Error",nullptr);
    error.addListener(this);
    
    if(error.getProperty("ErrorMsg")!=""){
        String errm = error.getProperty("ErrorMsg");
        errorComponent->errorText->setText(errm);
        errorComponent->setVisible(true);
    }else{
        errorComponent->setVisible(false);
    }
}

Object::~Object()
{
    if(!continuousRepainting) stopTimer();
    openGLContextChild.detach();
}

float Object::getParamValue(String paramId){
    if(processor.parameters.getParameter(paramId)!=nullptr){
        return processor.parameters.getParameter(paramId)->getValue();
    }
    else return 0;
}

void Object::mouseDown(const juce::MouseEvent &e)
{
    if(e.mods.isLeftButtonDown()){
        processor.parameters.getParameter(IDs::azimuth)->beginChangeGesture();
    }else if (e.mods.isRightButtonDown()){
        processor.parameters.getParameter(IDs::elevation)->beginChangeGesture();
    }
}

void Object::mouseDrag(const juce::MouseEvent &e)
{
    // Absolute Mouse Mode
    float xpix = -(float)e.getPosition().x/hfwidth+1.f;
    float zpix = -(float)e.getPosition().y/hfheight+1.f;
    float ypix = (zpix-sinf(mTheta)*rotX)/rotY;
    xpix = jlimit(-1.f,1.f,xpix);
    ypix = jlimit(-1.f,1.f,ypix);
    zpix = jlimit(-1.f,1.f,zpix);
    // Azimuth
    if (!e.mods.isRightButtonDown()){
        mPhi = atan2f(xpix,ypix);
    }
    // Elevation
    if(!e.mods.isLeftButtonDown()){
        mTheta = asin(zpix);
    }
    
    float azNorm = (mPhi/(float_Pi*2))+0.5f;
    float elNorm = (mTheta/(float_Pi))+0.5f;
    processor.parameters.getParameter(IDs::azimuth)->setValueNotifyingHost(azNorm);
    processor.parameters.getParameter(IDs::elevation)->setValueNotifyingHost(elNorm);
}

void Object::mouseUp(const juce::MouseEvent &e)
{
    if(e.mods.isLeftButtonDown()){
        processor.parameters.getParameter(IDs::azimuth)->endChangeGesture();
    }else if (e.mods.isRightButtonDown()){
        processor.parameters.getParameter(IDs::elevation)->endChangeGesture();
    }
}

void Object::resized(){
    OpenGLBase::resized();
    ssize = width/22;
    errorComponent->setBoundsInset (BorderSize<int> (1));
}

void Object::renderOpenGL()
{
    //    OpenGLHelpers::clear (Colours::black.withAlpha (1.f));
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    const float desktopScale = (float) openGLContextChild.getRenderingScale();
    
    float viewport_width = roundToInt (desktopScale * getWidth());
    float viewport_height = roundToInt (desktopScale * getHeight());
    glViewport(0,0, viewport_width, viewport_height);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.f, 1.f, -1.f, 1.f, 0.f, 0.f);
    
    /************ DRAW BACKGROUND GRID FROM IMAGE ***********************************/
    
    glEnable (GL_TEXTURE_2D);
    texture.bind();
    //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBegin(GL_QUADS);
    
    //    (0,1) ... (1,1)   -> (-1,1) ... (1,1)
    //    :         :            :         :
    //    (0,0) ... (1,0)   -> (-1,-1) ... (1,-1)
    glTexCoord2f(0.0, 0.0); glVertex2f(-1.0f*scale, -1.0f*scale);
    glTexCoord2f(1.0, 0.0); glVertex2f( 1.0f*scale, -1.0f*scale);
    glTexCoord2f(1.0, 1.0); glVertex2f( 1.0f*scale,  1.0f*scale);
    glTexCoord2f(0.0, 1.0); glVertex2f(-1.0f*scale,  1.0f*scale);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    
    /************************************************/
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable( GL_BLEND );
    
    int num_segments = 39;
    
    /************ DRAW BACKGROUND RADIAL GRADIENT ***********************************/
    
    drawCircle(0,0,1.5f,num_segments,scale, Colour::fromFloatRGBA(0.6f,0.6f,0.6f,0.5f),true, Colour::fromFloatRGBA(0.2f,0.2f,0.2f,0.5f));
    
    /********************** ELLIPSE (SOURCE PLANE)***********************/
    int objid = *processor.parameters.getRawParameterValue(IDs::objID)-1;
    mPhi   = visr::efl::degree2radian( *processor.parameters.getRawParameterValue(IDs::azimuth));
    mTheta = visr::efl::degree2radian( *processor.parameters.getRawParameterValue(IDs::elevation));
    
    xc = 0;
    yc = sinf(mTheta)*rotX;
    //        zc = rotY;
    radiusX = cosf(mTheta);
    radiusY = radiusX*rotY;
    
    float angle;
    ////    Ellipse source_plane(x1,y1,radiusX,radiusY);
    ////    source_plane.draw(x1, y1, 1);
    Colour objColour = *col.sliderPalette[objid];
    glColor4f(objColour.getFloatRed(),objColour.getFloatGreen(),objColour.getFloatBlue(),0.25f);
    
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xc*scale,yc*scale);
    
    for(int ii = 0; ii <= num_segments; ii++)
    {
        angle = 2 * float_Pi * ii / float(num_segments);
        x = xc+sin(angle)*radiusX;
        y = yc+cos(angle)*radiusY;
        glVertex2f(x*scale,y*scale);
    }
    glEnd();
    
    /*********************** SPHERE ( POINT SOURCE )**********************/
    
    std::tie( x, y, z ) = visr::efl::spherical2cartesian( mPhi, mTheta, 1.f );
    drawCircle(-y,x*rotY+sinf(mTheta)*rotX,0.05f,num_segments,scale, objColour ,true, objColour.darker(0.3));
    
    //    sphere_source.draw(srcx*scale, srcy*scale, 1);
    glDisable( GL_BLEND );
}

void Object::valueTreePropertyChanged (ValueTree& treeWhosePropertyHasChanged,
                                               const Identifier& property) {
    if(property.toString() == "ErrorMsg"){
        if(treeWhosePropertyHasChanged.getProperty(property)!=""){
            String errm = processor.parameters.state.getChildWithName("Error").getProperty("ErrorMsg");
            errorComponent->errorText->setText(errm);
            errorComponent->setVisible(true);
        }else{
            errorComponent->setVisible(false);
        }
    }
}
    
} // namespace graphics
} // namespace core
} // namespace visr_production_suite
