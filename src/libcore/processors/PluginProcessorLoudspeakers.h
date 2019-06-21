#ifndef PLUGIN_PROCESSOR_LOUDSPEAKERS_H_INCLUDED
#define PLUGIN_PROCESSOR_LOUDSPEAKERS_H_INCLUDED

#include "../export_symbols.hpp"
#include "PluginProcessorScene.h"
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
 * Audio processor containing many features which are in common to many plugins 
 * that deal with object-based scenes and loudspeaker layouts
 * @author Giacomo Costantini giacomo.costantini3@gmail.com
 */
class CORE_LIBRARY_SYMBOL PluginProcessorLoudspeakers  : public PluginProcessorScene
{
public:
    PluginProcessorLoudspeakers(const BusesProperties& ioConfig);
    ~PluginProcessorLoudspeakers();
    
    void VISRChangeLayout();
    int  VISRCheckLayout(unsigned int newLayout);
    int  VISRCheckLayout(juce::File newLayout);
    int  VISRCheckLayout(std::string newLayout);
    void VISRFindLayouts(juce::File layoutFolder);

    std::size_t parameterUpdatePeriod;
    
    visr::panning::LoudspeakerArray loudspeakerArray;
    visr::panning::LoudspeakerArray ldspkcheck;
    
    juce::File loudspLayoutsDir;
    juce::Array<juce::File> loudspLayoutFiles;
    juce::File currentLayoutFile, proposedLayoutFile;
    juce::File lastDirectory;
    bool popupGapResult;
    
    void getStateInformation (juce::MemoryBlock& destData) override;
    
protected:
    void prepareToPlay (double sampleRate, int period)override;
    
    virtual juce::AudioProcessorEditor* createEditor() override;
    
    
    
    void prepareBuffer (juce::AudioSampleBuffer& buffer);
    int maxInputChannels = 0;
    int maxOutputChannels = 0;
    
    juce::AudioDeviceManager  deviceManager;
    std::string defaultLdspArray = "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
    "<panningConfiguration dimension=\"2\" infinite=\"false\">"
    "<loudspeaker id=\"M+030\" channel=\"1\">"
    "<polar az=\"30.0\" el=\"0\" r=\"1\"/>"
    "</loudspeaker>"
    "<loudspeaker id=\"M-030\" channel=\"2\">"
    "<polar az=\"-30.0\" el=\"0\" r=\"1\"/>"
    "</loudspeaker>"
    "<virtualspeaker id=\"VotD\">"
    "<polar az=\"180.0\" el=\"0\" r=\"1.0\"/>"
    "<route lspId=\"M+030\" gainDB=\"-6.02\"/>"
    "<route lspId=\"M-030\" gainDB=\"-6.02\"/>"
    "</virtualspeaker>"
    "<triplet l1=\"M+030\" l2=\"VotD\"/>"
    "<triplet l1=\"M-030\" l2=\"VotD\"/>"
    "<triplet l1=\"M-030\" l2=\"M+030\"/>"
    "</panningConfiguration>";
    
    
    std::string bs2051_0_2_0 = "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
    "<panningConfiguration dimension=\"2\" infinite=\"false\">"
    "<loudspeaker channel=\"1\" id=\"1\">"
    "<polar az=\"-30\" el=\"0\" r=\"1\"/>"
    "</loudspeaker>"
    "<loudspeaker channel=\"2\" id=\"2\">"
    "<polar az=\"30\" el=\"0\" r=\"1\"/>"
    "</loudspeaker>"
    "<virtualspeaker id=\"3\">"
    "<polar az=\"180\" el=\"0\" r=\"1\"/>"
    "</virtualspeaker>"
    "<triplet l1=\"1\" l2=\"3\"/>"
    "<triplet l1=\"2\" l2=\"1\"/>"
    "<triplet l1=\"3\" l2=\"2\"/>"
    "</panningConfiguration>";
    
    std::string bs2051_0_5_0 = "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
    "<panningConfiguration dimension=\"2\" infinite=\"false\">"
    "<loudspeaker channel=\"12\" id=\"1\">"
    "<polar az=\"0\" el=\"0\" r=\"1\"/>"
    "</loudspeaker>"
    "<loudspeaker channel=\"24\" id=\"2\">"
    "<polar az=\"-30\" el=\"0\" r=\"1\"/>"
    "</loudspeaker>"
    "<loudspeaker channel=\"8\" id=\"3\">"
    "<polar az=\"30\" el=\"0\" r=\"1\"/>"
    "</loudspeaker>"
    "<loudspeaker channel=\"23\" id=\"4\">"
    "<polar az=\"-135\" el=\"0\" r=\"1\"/>"
    "</loudspeaker>"
    "<loudspeaker channel=\"7\" id=\"5\">"
    "<polar az=\"135\" el=\"0\" r=\"1\"/>"
    "</loudspeaker>"
    "<triplet l1=\"4\" l2=\"5\"/>"
    "<triplet l1=\"2\" l2=\"4\"/>"
    "<triplet l1=\"1\" l2=\"2\"/>"
    "<triplet l1=\"3\" l2=\"1\"/>"
    "<triplet l1=\"5\" l2=\"3\"/>"
    "</panningConfiguration>";
    
    std::string stereo = "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
    "<panningConfiguration dimension=\"2\" infinite=\"false\">"
    "<loudspeaker id=\"M+030\" channel=\"1\">"
    "<polar az=\"30.0\" el=\"0\" r=\"2.37201\"/>"
    "</loudspeaker>"
    "<loudspeaker id=\"M-030\" channel=\"2\">"
    "<polar az=\"-30.0\" el=\"0\" r=\"2.37201\"/>"
    "</loudspeaker>"
    "<virtualspeaker id=\"VotD\">"
    "<polar az=\"180.0\" el=\"0\" r=\"1.0\"/>"
    "<route lspId=\"M+030\" gainDB=\"-6.02\"/>"
    "<route lspId=\"M-030\" gainDB=\"-6.02\"/>"
    "</virtualspeaker>"
    "<triplet l1=\"M+030\" l2=\"VotD\"/>"
    "<triplet l1=\"M-030\" l2=\"VotD\"/>"
    "<triplet l1=\"M-030\" l2=\"M+030\"/>"
    "</panningConfiguration>";
    
    std::string dolby_7_1 = "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
    "<panningConfiguration dimension=\"2\" infinite=\"false\">"
    "<loudspeaker channel=\"1\" id=\"L\">"
    "<polar az=\"30\" el=\"0\" r=\"1\"/>"
    "</loudspeaker>"
    "<loudspeaker channel=\"2\" id=\"R\">"
    "<polar az=\"-30\" el=\"0\" r=\"1\"/>"
    "</loudspeaker>"
    "<loudspeaker channel=\"3\" id=\"C\">"
    "<polar az=\"0\" el=\"0\" r=\"1\"/>"
    "</loudspeaker>"
    "<loudspeaker channel=\"5\" id=\"Lsr\">"
    "<polar az=\"135\" el=\"0\" r=\"1\"/>"
    "</loudspeaker>"
    "<loudspeaker channel=\"6\" id=\"Rsr\">"
    "<polar az=\"-135\" el=\"0\" r=\"1\"/>"
    "</loudspeaker>"
    "<loudspeaker channel=\"7\" id=\"Lss\">"
    "<polar az=\"90\" el=\"0\" r=\"1\"/>"
    "</loudspeaker>"
    "<loudspeaker channel=\"8\" id=\"Rss\">"
    "<polar az=\"-90\" el=\"0\" r=\"1\"/>"
    "</loudspeaker>"
    "<subwoofer assignedLoudspeakers=\"L, R, C, Lsr, Rsr, Lss, Rss\" channel=\"4\" delay=\"0\" eq=\"lowpass\" gainDB=\"0\" weights=\"1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000\"/>"
    "<triplet l1=\"L\" l2=\"C\"/>"
    "<triplet l1=\"R\" l2=\"C\"/>"
    "<triplet l1=\"Lsr\" l2=\"Lss\"/>"
    "<triplet l1=\"Rsr\" l2=\"Rss\"/>"
    "<triplet l1=\"Lss\" l2=\"L\"/>"
    "<triplet l1=\"Rss\" l2=\"R\"/>"
    "<triplet l1=\"Lsr\" l2=\"Rsr\"/>"
    "</panningConfiguration>";

    std::string capLdspArray1 = "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
    "<panningConfiguration dimension=\"2\" infinite=\"false\">"
    "<loudspeaker channel=\"1\" id=\"1\">"
    "<polar az=\"-90\" el=\"0\" r=\"1\"/>"
    "</loudspeaker>"
    "<loudspeaker channel=\"2\" id=\"2\">"
    "<polar az=\"90\" el=\"0\" r=\"1\"/>"
    "</loudspeaker>"
    "<loudspeaker channel=\"3\" id=\"3\">"
    "<polar az=\"0\" el=\"0\" r=\"1\"/>"
    "</loudspeaker>"
    "<virtualspeaker id=\"4\">"
    "<polar az=\"180\" el=\"0\" r=\"1\"/>"
    "</virtualspeaker>"
    "<triplet l1=\"1\" l2=\"3\"/>"
    "<triplet l1=\"2\" l2=\"4\"/>"
    "<triplet l1=\"3\" l2=\"2\"/>"
    "<triplet l1=\"4\" l2=\"1\"/>"
    "</panningConfiguration>";
    
    std::string capLdspArray2 = "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
    "<panningConfiguration dimension=\"2\" infinite=\"false\">"
    "<loudspeaker channel=\"2\" id=\"1\">"
    "<polar az=\"-90\" el=\"0\" r=\"0.81\"/>"
    "</loudspeaker>"
    "<loudspeaker channel=\"1\" id=\"2\">"
    "<polar az=\"90\" el=\"0\" r=\"0.81\"/>"
    "</loudspeaker>"
    "<virtualspeaker id=\"3\">"
    "<polar az=\"180\" el=\"0\" r=\"0.81\"/>"
    "</virtualspeaker>"
    "<triplet l1=\"1\" l2=\"3\"/>"
    "<triplet l1=\"3\" l2=\"2\"/>"
    "</panningConfiguration>";
    
    
    std::string stereoMDO ="<?xml version=\"1.0\" encoding=\"utf-8\"?>"
    "<panningConfiguration dimension=\"2\" infinite=\"false\">"
    "<!-- Stereo -->"
    "<loudspeaker channel=\"1\" id=\"1\" mdo=\"0\" name=\"Main left\" gainDB=\"0\" delay=\"0.0\" function=\"vbap\">"
    "<polar az=\"30\" el=\"0\" r=\"1\"/>"
    "</loudspeaker>"
    "<loudspeaker channel=\"2\" id=\"2\" mdo=\"0\" name=\"Main right\" gainDB=\"0\" delay=\"0.0\" function=\"vbap\">"
    "<polar az=\"-30\" el=\"0\" r=\"1\"/>"
    "</loudspeaker>"
    "<!-- MDO devices -->"
    "<loudspeaker channel=\"3\" id=\"3\" mdo=\"1\" name=\"MDO1\" gainDB=\"0.0\" delay=\"0.0\" mdoZone=\"1\" switch=\"1\">"
    "<polar az=\"0\" el=\"0\" r=\"1\"/>"
    "</loudspeaker>"
    "<loudspeaker channel=\"4\" id=\"4\" mdo=\"1\" name=\"MDO2\" gainDB=\"0.0\" delay=\"0.0\" mdoZone=\"2\" switch=\"1\">"
    "<polar az=\"0\" el=\"0\" r=\"1\"/>"
    "</loudspeaker>"
    "<loudspeaker channel=\"5\" id=\"5\" mdo=\"1\" name=\"MDO3\" gainDB=\"0.0\" delay=\"0.0\" mdoZone=\"3\" switch=\"1\">"
    "<polar az=\"0\" el=\"0\" r=\"1\"/>"
    "</loudspeaker>"
    "<loudspeaker channel=\"6\" id=\"6\" mdo=\"1\" name=\"MDO4\" gainDB=\"0.0\" delay=\"0.0\" mdoZone=\"4\" switch=\"1\">"
    "<polar az=\"0\" el=\"0\" r=\"1\"/>"
    "</loudspeaker>"
    "<loudspeaker channel=\"7\" id=\"7\" mdo=\"1\" name=\"MDO5\" gainDB=\"0.0\" delay=\"0.0\" mdoZone=\"5\" switch=\"1\">"
    "<polar az=\"0\" el=\"0\" r=\"1\"/>"
    "</loudspeaker>"
    "<loudspeaker channel=\"8\" id=\"8\" mdo=\"1\" name=\"MDO6\" gainDB=\"0.0\" delay=\"0.0\" mdoZone=\"6\" switch=\"1\">"
    "<polar az=\"0\" el=\"0\" r=\"1\"/>"
    "</loudspeaker>"
    "<!-- 'Virtual' speaker -->"
    "<virtualspeaker id=\"9\">"
    "<polar az=\"180.0\" el=\"0\" r=\"1.0\"/>"
    "<route lspId=\"1\" gainDB=\"-6.0\"/>"
    "<route lspId=\"2\" gainDB=\"-6.0\"/>"
    "</virtualspeaker>"
    "<triplet l1=\"1\" l2=\"2\"/>"
    "<triplet l1=\"2\" l2=\"9\"/>"
    "<triplet l1=\"2\" l2=\"1\"/>"
    "</panningConfiguration>";
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessorLoudspeakers)
};
    
} // namespace processors
} // namespace core
} // namespace visr_production_suite

#endif
