#include "SceneWithHeadAndLdspk.h"
#include <libcore/IDs.h>
#include <libefl/cartesian_spherical_conversion.hpp>
#include <libefl/degree_radian_conversion.hpp>

using namespace juce;
using namespace visr_production_suite::core;

namespace visr_production_suite
{
namespace dawplugins
{
namespace cap_renderer
{

SceneWithHeadAndLdspk::SceneWithHeadAndLdspk(Processor& p) :
SceneWithLoudspeakers(p),
processor(p)
{
}

SceneWithHeadAndLdspk::~SceneWithHeadAndLdspk(){}

void SceneWithHeadAndLdspk::paint(Graphics& g){
    if(processor.parameters.state.getChildWithName("ListenerPos").isValid()){
        
        ValueTree vt = processor.parameters.state.getChildWithName("ListenerPos");
        float const yaw = (float)vt.getProperty(IDs::yaw);
        float const pitch = (float)vt.getProperty(IDs::pitch);
        float roll = (float)vt.getProperty(IDs::roll);
        float headradius = 50;
        
        //this is just for painting coherence reasons
        if (roll>(double_Pi/2)||roll<(-double_Pi/2)) roll+=double_Pi;
        
        float s_yaw = sinf(yaw);
        float c_yaw = cosf(yaw);
        float s_pitch = sinf(pitch);
        float c_pitch = cosf(pitch);
        float s_roll = sinf(roll);
        float c_roll = cosf(roll);
        
        //YP stays for yaw-pitch, which refers to the plane where the arrow lays
        float arrowEdgeYP_x =(s_yaw * c_pitch) * hfwidth * scale;
        float arrowEdgeYP_y =(c_yaw * c_pitch * rotY + s_pitch * rotX) * hfheight * scale;
        //YP stays for yaw-roll, which refers to the plane where the arrow lays
        float arrowEdgeYR_x = (c_yaw * c_roll) * headradius;
        float arrowEdgeYR_y = (s_yaw * rotY + s_roll * rotX) * headradius;
        //        std::cout<<" pitch "<<pitch<<" c_roll "<<c_roll<<"  s_roll "<<s_roll<<std::endl;
        
        //right ear arrow drawing
        g.setGradientFill (ColourGradient (Colours::white,hfwidth,hfheight,
                                           Colours::white.darker(), hfwidth+arrowEdgeYR_x,hfheight-arrowEdgeYR_y, false));
        Path arrowR;
        arrowR.addArrow(Line<float>(hfwidth,hfheight, hfwidth+arrowEdgeYR_x, hfheight-arrowEdgeYR_y), 5, ssize,ssize);
        g.fillPath(arrowR);
        
        //left arrow drawing
        g.setGradientFill (ColourGradient (Colours::white,hfwidth,hfheight,
                                           Colours::white.darker(), hfwidth-arrowEdgeYR_x, hfheight+arrowEdgeYR_y, false));
        Path arrowL;
        arrowL.addArrow(Line<float>(hfwidth,hfheight, hfwidth-arrowEdgeYR_x, hfheight+arrowEdgeYR_y), 5, ssize,ssize);
        g.fillPath(arrowL);
        
        // this condition is here because we want to draw yaw-pitch arrow over the listener's head if the face is pointing towards the real listener
        if (yaw<=(double_Pi/2) && yaw>=-(double_Pi/2)){
            
            g.setGradientFill (ColourGradient (Colours::white,hfwidth,hfheight,
                                               Colours::white.darker(), hfwidth-arrowEdgeYP_x,hfheight-arrowEdgeYP_y, false));
            Path arrowYP;
            arrowYP.addArrow(Line<float>(hfwidth,hfheight, hfwidth-arrowEdgeYP_x, hfheight-arrowEdgeYP_y), 5, ssize, ssize);
            g.fillPath(arrowYP);
            
            SceneWithLoudspeakers::paint(g);
        }else{
            
            SceneWithLoudspeakers::paint(g);
            
            g.setGradientFill (ColourGradient (Colours::white,hfwidth,hfheight,
                                               Colours::white.darker(), hfwidth-arrowEdgeYP_x,hfheight-arrowEdgeYP_y, false));
            Path arrowYP;
            arrowYP.addArrow(Line<float>(hfwidth,hfheight, hfwidth-arrowEdgeYP_x, hfheight-arrowEdgeYP_y), 5, ssize, ssize);
            g.fillPath(arrowYP);
        }
    }else{
        //LISTENER
        SceneWithLoudspeakers::paint(g);
    }
    //    std::cout<<processor->objectvector.toXmlString()<<std::endl;
}

void SceneWithHeadAndLdspk::renderOpenGL()
{
    SceneWithLoudspeakers::renderOpenGL();
    repaint();
}
    
} // namespace cap_renderer
} // namespace dawplugins
} // namespace visr_production_suite
