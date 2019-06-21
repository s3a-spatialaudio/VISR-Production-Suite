#include "PannerUI.h"
#include <libefl/cartesian_spherical_conversion.hpp>
#include <libefl/degree_radian_conversion.hpp>

using namespace juce;

namespace visr
{
namespace dawplugins
{
namespace multi_renderer
{
    
PannerUI::PannerUI(Processor * p) :
PannerUILdspk(),
processor(p)
{
}


PannerUI::~PannerUI()
{
}

void PannerUI::setLoudspeakerArray(){
    if (processor&&processor->setupDone){
        loudspeakerArray = (processor->loudspeakerArray);
        repaint();
    }
}


void PannerUI::paint(Graphics& g){
    PannerUIBase::paint(g);
    for(auto p : processor->parameters.state.getChildWithName(IDs::objectVector)){
        if(p.hasType(IDs::pointSource+p.getProperty(IDs::objID))){
            int obid = p.getProperty(IDs::objID);
            float mTheta = efl::degree2radian((float)p.getProperty(IDs::elevation));
            
            float xCurr = cosf(mTheta);
            float yCurr = -sinf(mTheta)*rotX;
            float center = 0.f;
            float rb = xCurr*rotY;
            ellipseCentreToTopLeft(center,yCurr,xCurr,rb);
            float xpix = (1+center)*hfwidth;
            float ypix = (1+yCurr)*hfheight;
            g.setGradientFill (ColourGradient (col.sliderPalette[obid]->brighter().withAlpha(0.2f),
                                               xpix,ypix,
                                               col.sliderPalette[obid]->darker().withAlpha(0.2f),
                                               xpix+xCurr*hfwidth,ypix+rb*hfheight,
                                               true));
            g.fillEllipse (xpix,ypix,xCurr*hfwidth,rb*hfheight);
            
        }
    }
    for(auto p : processor->parameters.state.getChildWithName(IDs::objectVector)){
        
        if(p.hasType(IDs::pointSource+p.getProperty(IDs::objID))){
            int obid = p.getProperty(IDs::objID);
            float mPhi = efl::degree2radian((float)p[IDs::azimuth]);
            float mTheta = efl::degree2radian((float)p[IDs::elevation]);
            std::tie( x, y, z ) = efl::spherical2cartesian( mPhi, mTheta, 1.f );
            
            float srcx =(1-y)*hfwidth -ssize/2;
            float srcy =(1-(x*rotY+sinf(mTheta)*rotX))*hfheight -ssize/2;
            g.setGradientFill (ColourGradient (col.sliderPalette[obid]->brighter(),srcx,srcy,
                                               col.sliderPalette[obid]->darker(), srcx+ssize,srcy+ssize,
                                               false));
            g.fillEllipse(srcx, srcy, ssize, ssize);
        }
    }
    PannerUILdspk::paint(g);
//    std::cout<<processor->objectvector.toXmlString()<<std::endl;
}

    
} // namespace multi_renderer
} // namespace dawplugins
} // namespace visr
