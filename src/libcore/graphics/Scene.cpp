#include "Scene.h"
#include <libefl/cartesian_spherical_conversion.hpp>
#include <libefl/degree_radian_conversion.hpp>
#include <libobjectmodel/object_type.hpp>
#include <libobjectmodel/object.hpp>
#include <libobjectmodel/point_source.hpp>

namespace visr_production_suite
{
namespace core
{
namespace graphics
{

using namespace juce;
using namespace visr;

Scene::Scene(processors::PluginProcessorScene& p) :
OpenGLBase(),
processor(p){

    errorComponent = new core::graphics::ErrorPanel();
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
    
    openGLContextSM.setRenderer (this);
    openGLContextSM.setComponentPaintingEnabled (true);
    //    openGLContextSM.setComponentPaintingEnabled (false);
    openGLContextSM.setMultisamplingEnabled(true);
    if(continuousRepainting) openGLContextSM.setContinuousRepainting(true);
    else openGLContextSM.setContinuousRepainting(false); // manually repaint
    
    openGLContextSM.attachTo (*this);
}

Scene::~Scene()
{
    openGLContextSM.detach();
}

void Scene::resized(){
    OpenGLBase::resized();
    errorComponent->setBoundsInset (BorderSize<int> (1));
}

void Scene::update(){
    {
        std::lock_guard<std::mutex> guard{ sceneLockRenderer };
        scene = processor.getScene();
    }
    //    repaint();
    if(!continuousRepainting) openGLContextSM.triggerRepaint();
}

void Scene::renderOpenGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    const float desktopScale = (float) openGLContextSM.getRenderingScale();
    
    float viewport_width = roundToInt (desktopScale * getWidth());
    float viewport_height = roundToInt (desktopScale * getHeight());
    glViewport(0, 0, viewport_width, viewport_height);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.f, 1.f, -1.f, 1.f, -1.f, 1.f);
    
    glColor3f(1, 1, 1);
    
    /************ DRAW BACKGROUND GRID FROM IMAGE ***********************************/
    
    glEnable (GL_TEXTURE_2D);
    texture.bind();
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
    
    {
        std::lock_guard<std::mutex> guard{ sceneLockRenderer };
        
        for( visr::objectmodel::ObjectVector::const_iterator runIt( scene.cbegin()); runIt != scene.cend(); ++runIt )
        {
            
            if(runIt->type() == visr::objectmodel::ObjectTypeId::PointSource || runIt->type() == visr::objectmodel::ObjectTypeId::PointSourceWithReverb){
                
                visr::objectmodel::PointSource * mPs = dynamic_cast< visr::objectmodel::PointSource *>(&(*runIt));
                x = mPs->x();
                y = mPs->y();
                z = mPs->z();
                
                std::tie( mPhi,mTheta, mRadius) = efl::cartesian2spherical(x, y, z);
                int obid = (int)mPs->id();
                
                /********************** ELLIPSE (SOURCE PLANE)***********************/
                xc = 0;
                yc = sinf(mTheta)*rotX;
                //        zc = rotY;
                radiusX = cosf(mTheta);
                radiusY = radiusX*rotY;
                
                float angle;
                
                ////    Ellipse source_plane(x1,y1,radiusX,radiusY);
                ////    source_plane.draw(x1, y1, 1);
                Colour objColour = *col.sliderPalette[obid];
                glColor4f(objColour.getFloatRed(),objColour.getFloatGreen(),objColour.getFloatBlue(),0.05f);
                //    glAlphaFunc(<#GLenum func#>, <#GLclampf ref#>)
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
                //            this is needed because radius can be !=1  as it is a parameter in Object Editor.
                //            the radius used for painting, instead, should always be 1.
                std::tie( x, y, z ) = visr::efl::spherical2cartesian( mPhi, mTheta, 1.f );
                drawCircle(-y,x*rotY+sinf(mTheta)*rotX,0.04f,num_segments,scale, objColour ,true, objColour.darker(0.3));
                
            }
            /*********************************************/
        }
        glDisable( GL_BLEND );
    }
}

void Scene::valueTreePropertyChanged (ValueTree& treeWhosePropertyHasChanged,
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
