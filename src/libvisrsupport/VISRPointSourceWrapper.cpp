#include "VISRPointSourceWrapper.h"
#include <libefl/cartesian_spherical_conversion.hpp>
#include <libefl/degree_radian_conversion.hpp>
#include <libcore/IDs.h>

namespace visr_production_suite
{
namespace visr_support
{

using namespace visr;
using namespace juce;
using namespace core;

VISRPointSourceWrapper::VISRPointSourceWrapper(AudioProcessorValueTreeState& param):
VISRObjectWrapper(param,visr::objectmodel::ObjectTypeId::PointSource)
{
    float* objn = dataModel.getRawParameterValue(IDs::objID);
    pointSrc.reset(new visr::objectmodel::PointSource(*objn -1));
    
    
    if( pointSrc )
    {
        if(!UDPReceiver::bind(*objn+60000)){
            String errorMsg;
            errorMsg << "Binding to receiving Port failed, your PointSource ID should be unique\n";
            dataModel.state.getOrCreateChildWithName("Error",nullptr).setProperty("ErrorMsg", errorMsg, nullptr);
            std::cerr << errorMsg << std::endl;
        }else{
            dataModel.state.getOrCreateChildWithName("Error",nullptr).removeAllProperties(nullptr);
        }
        pointSrc->resetNumberOfChannels(1);
        pointSrc->setChannelIndex(0,pointSrc->id());
        phi = efl::degree2radian(*param.getRawParameterValue(IDs::azimuth));
        theta = efl::degree2radian(*param.getRawParameterValue(IDs::elevation));
        radius = *param.getRawParameterValue(IDs::radius);
        
        objectmodel::Object::Coordinate xPos, yPos, zPos;
        std::tie( xPos, yPos, zPos ) = efl::spherical2cartesian( phi, theta, radius );
        
        pointSrc->setX(xPos);
        pointSrc->setY(yPos);
        pointSrc->setZ(zPos);
        pointSrc->setLevel(0.5);
        pointSrc->setGroupId(5);
        pointSrc->setPriority(5);
    
        {
            std::lock_guard<std::mutex> guard{ objWrapLock };
            objWrap.insert( *pointSrc );
        }
    }
}
VISRPointSourceWrapper::~VISRPointSourceWrapper(){
 UDPReceiver::unbind();
}

void VISRPointSourceWrapper::VISRObjectChanged(const juce::String& pID, float newVal){
    if( pointSrc )
    {
        if(pID==IDs::azimuth.toString()){
            phi = efl::degree2radian( newVal);
        }
        
        else if(pID==IDs::elevation.toString()){
            theta = efl::degree2radian( newVal);
        }
        
        else if(pID==IDs::radius.toString()){
            radius = newVal;
        }
        else if(pID==IDs::objID.toString()){
            if(!UDPReceiver::bind(60000+newVal)){
                String errorMsg;
                errorMsg << "Binding to receiving Port failed, your PointSource ID should be unique\n";
                dataModel.state.getOrCreateChildWithName("Error",nullptr).setProperty("ErrorMsg", errorMsg, nullptr);
                std::cerr << errorMsg << std::endl;
            }else{
                dataModel.state.getOrCreateChildWithName("Error",nullptr).removeAllProperties(nullptr);
            }
            int newIDandChan = newVal-1;
            pointSrc.reset(new visr::objectmodel::PointSource(newIDandChan));
            pointSrc->resetNumberOfChannels(1);
            pointSrc->setChannelIndex(0,newIDandChan);
            
            pointSrc->setLevel(0.5);
            pointSrc->setGroupId(5);
            pointSrc->setPriority(5);
        }
        objectmodel::Object::Coordinate xPos, yPos, zPos;
        std::tie( xPos, yPos, zPos ) = efl::spherical2cartesian( phi, theta, radius );
        
        pointSrc->setX(xPos);
        pointSrc->setY(yPos);
        pointSrc->setZ(zPos);
        
        {
            std::lock_guard<std::mutex> guard{ objWrapLock };
            objWrap.clear();
            objWrap.insert( *pointSrc );
        }
    }
}

void VISRPointSourceWrapper::VISRObjectReceived (visr::objectmodel::Object& obj)
{
    visr::objectmodel::PointSource * mPs = dynamic_cast< visr::objectmodel::PointSource *>(&(obj));
    // TODO: check whether the lock might affect responsiveness,
    // but we need to ensure that the object remains valid as long as it is used in this method.
    float x,y,z;
    {
        std::lock_guard<std::mutex> guard{ objWrapLock };
        x = mPs->x();
        y = mPs->y();
        z = mPs->z();
    }
    
    objectmodel::Object::Coordinate az_, el_, radius_;
    std::tie( az_, el_, radius_ ) = efl::cartesian2spherical(x, y, z);
    
    float normPar = dataModel.getParameterRange(IDs::azimuth).convertTo0to1(std::round(efl::radian2degree(az_)));
    dataModel.getParameter(IDs::azimuth)->setValueNotifyingHost( normPar );
    
    normPar = dataModel.getParameterRange(IDs::elevation).convertTo0to1(std::round(efl::radian2degree(el_)));
    dataModel.getParameter(IDs::elevation)->setValueNotifyingHost( normPar );
    
    normPar = dataModel.getParameterRange(IDs::radius).convertTo0to1(radius_);
    dataModel.getParameter(IDs::radius)->setValueNotifyingHost(normPar);
}
} // namespace visr_support
} // namespace visr_production_suite

