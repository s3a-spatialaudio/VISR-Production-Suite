#include "NetSettings.h"

namespace visr_production_suite
{
namespace core
{
namespace graphics
{

NetSettings::NetSettings (processors::PluginProcessorBase& Processor)
    :   NetSettingsBase(Processor.parameters.state,Processor.getName()),
        processor(Processor){}

NetSettings::~NetSettings(){}


void NetSettings::handleAsyncUpdate(){
    NetSettingsBase::handleAsyncUpdate();
    processor.PluginReset();
}
    
} // namespace graphics
} // namespace core
} // namespace visr_production_suite
