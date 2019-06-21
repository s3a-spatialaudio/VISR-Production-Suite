#include "VISRPointSourceExtWrapper.h"
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
    
VISRPointSourceExtWrapper::VISRPointSourceExtWrapper(AudioProcessorValueTreeState& param):
VISRObjectWrapper(param,visr::objectmodel::ObjectTypeId::PointSourceExtent)
{
    float* objn = dataModel.getRawParameterValue(IDs::objID);
    
    pointSrcExt.reset(new visr::objectmodel::PointSourceExtent(*objn -1));
    if( pointSrcExt )
    {
        if(!UDPReceiver::bind(*objn+7000)){
            String errorMsg;
            errorMsg << "Binding to receiving Port failed, your Point Source With Extent ID should be unique\n";
            dataModel.state.getOrCreateChildWithName("Error",nullptr).setProperty("ErrorMsg", errorMsg, nullptr);
            std::cerr << errorMsg << std::endl;
        }else{
            dataModel.state.getOrCreateChildWithName("Error",nullptr).removeAllProperties(nullptr);
        }
        
        pointSrcExt->resetNumberOfChannels(1);
        pointSrcExt->setChannelIndex(0,pointSrcExt->id());
        
        phi = efl::degree2radian(*param.getRawParameterValue(IDs::azimuth));
        theta = efl::degree2radian(*param.getRawParameterValue(IDs::elevation));
        radius = *param.getRawParameterValue(IDs::radius);
        
        objectmodel::Object::Coordinate xPos, yPos, zPos;
        std::tie( xPos, yPos, zPos ) = efl::spherical2cartesian( phi, theta, radius );
        
        pointSrcExt->setX(xPos);
        pointSrcExt->setY(yPos);
        pointSrcExt->setZ(zPos);
        
        pointSrcExt->setWidth(0.f);
        pointSrcExt->setHeight(0.f);
        pointSrcExt->setDepth(0.f);
        pointSrcExt->setLevel(0.5);
        pointSrcExt->setDiffuseness(0);
        
        pointSrcExt->setGroupId(5);
        pointSrcExt->setPriority(5);
        
        {
            std::lock_guard<std::mutex> guard{ objWrapLock };
            objWrap.insert( *pointSrcExt );
        }
    }
}
VISRPointSourceExtWrapper::~VISRPointSourceExtWrapper(){}

void VISRPointSourceExtWrapper::VISRObjectChanged(const String& pID, float newVal){
    if( pointSrcExt )
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
        else if(pID==IDs::width.toString()){
            width = newVal;
        }
        else if(pID==IDs::height.toString()){
            height = newVal;
        }
        else if(pID==IDs::depth.toString()){
            depth = newVal;
        }
        else if(pID==IDs::diffuseness.toString()){
            diffuseness = newVal/100.f;
        }
        else if(pID==IDs::objID.toString()){
            if(!UDPReceiver::bind(7000+newVal)){
                String errorMsg;
                errorMsg << "Binding to receiving Port failed, your Point Source With Extent ID should be unique\n";
                dataModel.state.getOrCreateChildWithName("Error",nullptr).setProperty("ErrorMsg", errorMsg, nullptr);
                std::cerr << errorMsg << std::endl;
            }else{
                dataModel.state.getOrCreateChildWithName("Error",nullptr).removeAllProperties(nullptr);
            }
            int newIDandChan = newVal-1;
            
            pointSrcExt.reset(new visr::objectmodel::PointSourceExtent(newIDandChan));
            pointSrcExt->resetNumberOfChannels(1);
            pointSrcExt->setChannelIndex(0,newIDandChan);
            
            pointSrcExt->setLevel(0.5);
            pointSrcExt->setGroupId(5);
            pointSrcExt->setPriority(5);
        }
        
        objectmodel::Object::Coordinate xPos, yPos, zPos;
        std::tie( xPos, yPos, zPos ) = efl::spherical2cartesian( phi, theta, radius );
        
        pointSrcExt->setX(xPos);
        pointSrcExt->setY(yPos);
        pointSrcExt->setZ(zPos);
        pointSrcExt->setWidth(width);
        pointSrcExt->setHeight(height);
        pointSrcExt->setDepth(depth);
        pointSrcExt->setDiffuseness(diffuseness);
        
        {
            std::lock_guard<std::mutex> guard{ objWrapLock };
            objWrap.clear();
            objWrap.insert( *pointSrcExt );
        }
    }
}


void VISRPointSourceExtWrapper::VISRObjectReceived (visr::objectmodel::Object& obj)
{
    visr::objectmodel::PointSourceExtent * mEps = dynamic_cast< visr::objectmodel::PointSourceExtent *>(&(obj));

    float x,y,z, depth_, diffuseness_;
    int width_, height_;
    {
        std::lock_guard<std::mutex> guard{ objWrapLock };
        x = mEps->x();
        y = mEps->y();
        z = mEps->z();
        width_ = mEps->width();
        height_ = mEps->height();
        depth_ = mEps->depth();
        diffuseness_ = mEps->diffuseness();
    }
    
    objectmodel::Object::Coordinate az_, el_, radius_;
    std::tie( az_, el_, radius_ ) = efl::cartesian2spherical(x, y, z);
    
    float normPar = dataModel.getParameterRange(IDs::azimuth).convertTo0to1(std::round(efl::radian2degree(az_)));
    dataModel.getParameter(IDs::azimuth)->setValueNotifyingHost( normPar );
    
    normPar = dataModel.getParameterRange(IDs::elevation).convertTo0to1(std::round(efl::radian2degree(el_)));
    dataModel.getParameter(IDs::elevation)->setValueNotifyingHost( normPar );
    
    normPar = dataModel.getParameterRange(IDs::radius).convertTo0to1(radius_);
    dataModel.getParameter(IDs::radius)->setValueNotifyingHost(normPar);
    
    normPar = dataModel.getParameterRange(IDs::width).convertTo0to1(width_);
    dataModel.getParameter(IDs::width)->setValueNotifyingHost(normPar);
    
    normPar = dataModel.getParameterRange(IDs::height).convertTo0to1(height_);
    dataModel.getParameter(IDs::height)->setValueNotifyingHost(normPar);
    
    normPar = dataModel.getParameterRange(IDs::depth).convertTo0to1(depth_);
    dataModel.getParameter(IDs::depth)->setValueNotifyingHost(normPar);
    
    normPar = dataModel.getParameterRange(IDs::diffuseness).convertTo0to1(diffuseness_);
    dataModel.getParameter(IDs::depth)->setValueNotifyingHost(normPar);
}

} // namespace visr_support
} // namespace visr_production_suite
