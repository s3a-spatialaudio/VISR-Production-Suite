#ifndef PLUGIN_PROCESSOR_SCENE_H_INCLUDED
#define PLUGIN_PROCESSOR_SCENE_H_INCLUDED

#include "../export_symbols.hpp"
#include "PluginProcessorBase.h"
#include <libpanning/LoudspeakerArray.h>
#include <libvisrsupport/VISRComponentWrapper.h>
#include <libobjectmodel/object_vector.hpp>

namespace visr_production_suite
{
namespace core
{
namespace processors
{
/**
 * Audio processor containing many features which are in common to many plugins that deal with object-based scenes
 * @author Giacomo Costantini giacomo.costantini3@gmail.com
 */
class CORE_LIBRARY_SYMBOL PluginProcessorScene  : public PluginProcessorBase
{
public:
    PluginProcessorScene(const BusesProperties& ioConfig);
    ~PluginProcessorScene();
    
    void getStateInformation (juce::MemoryBlock& destData) override;
    virtual visr::objectmodel::ObjectVector getScene() const = 0;
    
protected:
    
    void prepareBuffer (juce::AudioSampleBuffer& buffer);
    int maxInputChannels = 0;
    int maxOutputChannels = 0;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessorScene)
};

    
} // namespace processors
} // namespace core
} // namespace visr_production_suite

#endif
