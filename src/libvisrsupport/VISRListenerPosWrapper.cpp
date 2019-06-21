#include "VISRListenerPosWrapper.h"
#include <libpml/listener_position.hpp>
#include <librrl/audio_signal_flow.hpp>
#include <libcore/IDs.h>

namespace visr_production_suite
{
namespace visr_support
{

using namespace visr;
using namespace juce;
using namespace core;
    
VISRListenerPosWrapper::VISRListenerPosWrapper(AudioProcessorValueTreeState& model, visr::rrl::AudioSignalFlow& flow, int dynamicHeadTrackingSelect):
dataModel(model),
audioFlow(flow),
trackingSelect(dynamicHeadTrackingSelect)
{
    if(trackingSelect==1){
        dataModel.addParameterListener(IDs::listenerPosX.toString(),this);
        dataModel.addParameterListener(IDs::listenerPosY.toString(),this);
        dataModel.addParameterListener(IDs::listenerPosZ.toString(),this);
        dataModel.addParameterListener(IDs::yaw.toString(), this);
        dataModel.addParameterListener(IDs::pitch.toString(), this);
        dataModel.addParameterListener(IDs::roll.toString(), this);
    }else if(trackingSelect==2){
        startTimer (50);
    }
}

VISRListenerPosWrapper::~VISRListenerPosWrapper(){
    if(trackingSelect==1){
        dataModel.removeParameterListener(IDs::listenerPosX.toString(),this);
        dataModel.removeParameterListener(IDs::listenerPosY.toString(),this);
        dataModel.removeParameterListener(IDs::listenerPosZ.toString(),this);
        dataModel.removeParameterListener(IDs::yaw.toString(), this);
        dataModel.removeParameterListener(IDs::pitch.toString(), this);
        dataModel.removeParameterListener(IDs::roll.toString(), this);
    }
    else if(trackingSelect==2){
        stopTimer();
    }
}

void VISRListenerPosWrapper::timerCallback ()
{
    visr::pml::DoubleBufferingProtocol::InputBase * mListenerPos = dynamic_cast<visr::pml::DoubleBufferingProtocol::InputBase *>(&audioFlow.externalParameterSendPort("listenerPositionOut"));
    const visr::pml::ListenerPosition * lPos = dynamic_cast<const visr::pml::ListenerPosition *>(&(mListenerPos->data()));
    float lX = lPos->x();
    float lY = lPos->y();
    float lZ = lPos->z();
    
    float yaw = lPos->yaw();
    float pitch = lPos->pitch();
    float roll = lPos->roll();
    
    ValueTree ps = dataModel.state.getOrCreateChildWithName ( "ListenerPos", nullptr);
    ps.setProperty(IDs::listenerPosX, lX, nullptr);
    ps.setProperty(IDs::listenerPosY, lY, nullptr);
    ps.setProperty(IDs::listenerPosZ, lZ, nullptr);
    ps.setProperty(IDs::yaw, yaw, nullptr);
    ps.setProperty(IDs::pitch, pitch, nullptr);
    ps.setProperty(IDs::roll, roll, nullptr);
}

void VISRListenerPosWrapper::parameterChanged (const String& pID, float newVal){
    if(pID==IDs::listenerPosX.toString()){
        lX = newVal;
        //            std::cout<<" listenerposx "<<newVal<<std::endl;
    }
    else if(pID==IDs::listenerPosY.toString()){
        lY = newVal;
    }
    else if(pID==IDs::listenerPosZ.toString()){
        lZ = newVal;
    }
    else if(pID==IDs::yaw.toString()){
        yaw = newVal/180.0 * double_Pi;
    }
    else if(pID==IDs::pitch.toString()){
        pitch = newVal/180.0 * double_Pi;
    }
    else if(pID==IDs::roll.toString()){
        roll =  newVal/180.0 * double_Pi;
    }
    
    visr::pml::DoubleBufferingProtocol::OutputBase * listenerPort;
    listenerPort =  dynamic_cast<pml::DoubleBufferingProtocol::OutputBase *>(&audioFlow.externalParameterReceivePort("listenerPositionIn"));
    
    visr::pml::ListenerPosition* listenerPos = dynamic_cast<visr::pml::ListenerPosition *>(&(listenerPort->data()));
    
    listenerPos->setX(lX);
    listenerPos->setY(lY);
    listenerPos->setZ(lZ);
    listenerPos->setOrientation(yaw, pitch, roll);
    
    listenerPort->swapBuffers();
    
    ValueTree ps = dataModel.state.getOrCreateChildWithName ( "ListenerPos", nullptr);
    ps.setProperty(IDs::listenerPosX, lX, nullptr);
    ps.setProperty(IDs::listenerPosY, lY, nullptr);
    ps.setProperty(IDs::listenerPosZ, lZ, nullptr);
    ps.setProperty(IDs::yaw, yaw, nullptr);
    ps.setProperty(IDs::pitch, pitch, nullptr);
    ps.setProperty(IDs::roll, roll, nullptr);
}
} // namespace visr_support
} // namespace visr_production_suite
