#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <libpml/initialise_parameter_library.hpp>

using namespace visr;
using namespace juce;

// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new visr_production_suite::dawplugins::scene_master::Processor();
}

namespace visr_production_suite
{
namespace dawplugins
{
namespace scene_master
{

Processor::Processor():
PluginProcessorScene(BusesProperties()
                    .withInput ("Input",  AudioChannelSet::stereo(), true)
                    .withOutput ("Output", AudioChannelSet::stereo(), true))
{
    parameters.state = ValueTree (Identifier ("SceneMaster"));
}

Processor::~Processor(){}

void Processor::PluginSetup()
{
    pml::initialiseParameterLibrary();

    const std::string destAddr = parameters.state.getProperty(getName()+"_ip","127.0.0.1").toString().toStdString();
    const int destPort = parameters.state.getProperty(getName()+"_port",8001);
    
    objectVectorHandler.reset(new visr_support::VISRObjectVectorWrapper(parameters.state, views, 60000, destAddr, destPort));

    setupDone = true;
}

void Processor::PluginUnload()
{
    if(objectVectorHandler!=nullptr){
        objectVectorHandler.reset();
    }
    setupDone = false;
}

AudioProcessorEditor* Processor::createEditor()
{
    return new Editor (this);
}

} // namespace scene_master
} // namespace dawplugins
} // namespace visr_production_suite

