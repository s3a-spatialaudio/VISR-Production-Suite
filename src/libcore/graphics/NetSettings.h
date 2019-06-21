#ifndef NETSETTINGS_HEADER
#define NETSETTINGS_HEADER

#include "../export_symbols.hpp"
#include "JuceHeader.h"
#include "NetSettingsBase.h"
#include <libcore/processors/PluginProcessorBase.h>

namespace visr_production_suite
{
namespace core
{
namespace graphics
{

/** 
 * Network settings window to be used to change ip and/or ports 
 * and reset the audio processor
 * @author Giacomo Costantini giacomo.costantini3@gmail.com
 */
class CORE_LIBRARY_SYMBOL NetSettings  : public NetSettingsBase
{
public:
    NetSettings (processors::PluginProcessorBase& processor);
    ~NetSettings();
    
    
    /** Called back to do whatever task you need to be done asynchronously.
     *  @see juce::AsyncUpdater
     */
    void handleAsyncUpdate() override;

private:
    processors::PluginProcessorBase& processor;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NetSettings)
};
    
} // namespace graphics
} // namespace core
} // namespace visr_production_suite

#endif   // NETSETTINGS_HEADER
