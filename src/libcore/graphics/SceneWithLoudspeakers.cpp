#include "SceneWithLoudspeakers.h"

namespace visr_production_suite
{
namespace core
{
namespace graphics
{

using namespace juce;
using namespace visr;

SceneWithLoudspeakers::SceneWithLoudspeakers(processors::PluginProcessorLoudspeakers& p) :
Scene(p),
processor(p){
    
}

SceneWithLoudspeakers::~SceneWithLoudspeakers()
{
}

void SceneWithLoudspeakers::paint (Graphics& g)
{
    g.setColour (Colour (50,50,50));
    g.fillEllipse(hfwidth-hfwidth/20,hfheight-hfheight/20, hfheight/10, hfheight/10);
    
    //SPEAKERS
    std::list<float> tempz = elevs;
    
    for(std::size_t i=0;i<loudspeakerArray.getNumRegularSpeakers();i++){
        panning::XYZ pos = loudspeakerArray.getPosition(i);
        if(fabs(pos.x)>1 || fabs(pos.y)>1 || fabs(pos.z)>1)    pos.normalise();
        float zpos = pos.z;
        float el = asinf(pos.z);
        float az = atan2( pos.y, pos.x );
      
        if(ringState[zpos]){ // if ring is enabled, through its checkbox
            //SPEAKERS CIRCLES
            g.setColour (ringColour[zpos]);
            float xCurr = cosf(el);
            float yCurr = -sinf(el)*rotX;
            float center = 0.f;
            float rb = xCurr*rotY;
            
            center*=scale;
            yCurr*=scale;
            xCurr*=scale;
            rb*=scale;
            ellipseCentreToTopLeft(center,yCurr,xCurr,rb);
            float xpix = (1+center)*hfwidth;
            float ypix = (1+yCurr)*hfheight;
            
            //CIRCLE JOINING SPEAKERS WITH SAME ELEVATION
            if(std::find(tempz.begin(), tempz.end(), zpos) != tempz.end()){
                g.drawEllipse (xpix,ypix,xCurr*hfwidth,rb*hfheight,2.0);
                tempz.remove(zpos);
            }
            
            az +=double_Pi*0.5;
            float _x = (cosf(az)*cosf(el));
            float _y = (sinf(az)*cosf(el))*rotY+sinf(el)*rotX;
            float _ldspx =(1+_x*scale)*hfwidth -(ssize+4)/2;
            float _ldspy =(1-_y*scale)*hfheight -(ssize+4)/2;
            g.fillRoundedRectangle(_ldspx, _ldspy, (ssize+4),(ssize+4),3);
            //SPEAKERS NUMBERS
            g.setColour (ringColour[zpos].darker().darker().withAlpha(0.9f));
            g.drawRoundedRectangle(_ldspx, _ldspy, (ssize+4),(ssize+4),3,2);
            g.drawFittedText(std::to_string(loudspeakerArray.getSpeakerChannel(i)),_ldspx+2,_ldspy+2,ssize,ssize, Justification::centred,1);
        }
    }
}

/*================================================================*/
void SceneWithLoudspeakers::enableRing(std::string elev){
    float el = atof(elev.c_str());
    ringState[el] = true;
    repaint();
}

void SceneWithLoudspeakers::disableRing(std::string elev){
    float el = atof(elev.c_str());
    ringState[el] = false;
    repaint();
}

void SceneWithLoudspeakers::resetLayout(){
    setLoudspeakerArray();
    elevs.clear();
    for(std::size_t i=0;i<loudspeakerArray.getNumRegularSpeakers();i++){
        visr::panning::XYZ pos = loudspeakerArray.getPosition(i);
        if(pos.x>1 || pos.y>1 || pos.z>1)    pos.normalise();
        elevs.push_back(pos.z);
    }
    elevs.sort();
    elevs.unique();
    
    Colour startCol = Colour(0,200,100).withAlpha(0.8f);
    int inx = 0;
    for(auto v : elevs){
        inx++;
        float rot = 1.f*inx/(pow(elevs.size(),2.5));
        if(v!=0) ringColour[v] =startCol.withRotatedHue(rot);
        else ringColour[v] =startCol;
        ringState[v] = true;
    }
}

void SceneWithLoudspeakers::setLoudspeakerArray(){
    if (&processor && processor.setupDone){
        loudspeakerArray = (processor.loudspeakerArray);
        SceneWithLoudspeakers::repaint();
    }
}
    
} // namespace graphics
} // namespace core
} // namespace visr_production_suite
