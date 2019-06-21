#ifndef OBJECTEDITOR_PROCESSOR_H_INCLUDED
#define OBJECTEDITOR_PROCESSOR_H_INCLUDED

#include <libcore/processors/PluginProcessorBase.h>
#include <libvisrsupport/VISRPointSourceWrapper.h>
#include <libcore/IDs.h>

namespace visr_production_suite
{
namespace dawplugins
{
namespace object_editor
{

/**
 * Object Editor's main class.
 * Handles audio processing, VISR-related classes and VISR components, and instantiates the Editor object for GUI handling.
 * The name processor is due to the fact that it is a JUCE plugin processor.
 * @author Giacomo Costantini giacomo.costantini3@gmail.com
 */
class Processor  : public core::processors::PluginProcessorBase
{
public:
    Processor();
    ~Processor();
    
    const juce::String getName() const override {return "VISR Object Editor";};
    juce::AudioProcessorEditor* createEditor() override;
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
    void prepareToPlay (double sampleRate, int period) override{};
    
    void PluginSetup() override;
    void PluginUnload() override;
private:
    /**
     * Instance of a wrapper of a point source audio object.
     */
    std::unique_ptr<visr_support::VISRPointSourceWrapper> objectHandler;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Processor)
};
    
} // namespace object_editor
} // namespace dawplugins
} // namespace visr_production_suite

#endif
