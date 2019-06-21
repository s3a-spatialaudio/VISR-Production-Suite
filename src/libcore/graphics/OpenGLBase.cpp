#include "OpenGLBase.h"
#include <libcore/PluginBinaryData.h>
#include <libcore/IDs.h>

namespace visr_production_suite
{
namespace core
{
namespace graphics
{

using namespace juce;

OpenGLBase::OpenGLBase() :
rotAngle(float_Pi/12.f),
isFirst(true)
{
    rotX = cosf(rotAngle);
    rotY = sinf(rotAngle);
//    addAndMakeVisible( sph);
    
    resized();
}

OpenGLBase::~OpenGLBase(){}

void OpenGLBase::resized(){
    width = getWidth();
    height = getHeight();
    hfwidth = (float)this->width/2;
    hfheight = (float)this->height/2;
    ssize = width/27;
}

void OpenGLBase::ellipseCentreToTopLeft(float& x, float& y, float& ra, float& rb){
    x = x-ra;
    y = y-rb;
    ra = 2*ra;
    rb = 2*rb;
}
    
} // namespace graphics
} // namespace core
} // namespace visr_production_suite
