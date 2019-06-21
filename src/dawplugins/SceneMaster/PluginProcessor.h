#ifndef SCENEMASTER_PROCESSOR_H_INCLUDED
#define SCENEMASTER_PROCESSOR_H_INCLUDED

#include <libcore/processors/PluginProcessorScene.h>
#include <libvisrsupport/VISRObjectVectorWrapper.h>
#include <libcore/IDs.h>

namespace visr_production_suite
{
namespace dawplugins
{
namespace scene_master
{

    
/**
 * Scene Master's main class.
 * Handles audio processing, VISR-related classes and VISR components, and instantiates the Editor object for GUI handling.
 * The name processor is due to the fact that it is a JUCE plugin processor.
 * @author Giacomo Costantini giacomo.costantini3@gmail.com
 */
class Processor  : public core::processors::PluginProcessorScene
{
public:
    Processor();
    ~Processor();
    
    const juce::String getName() const override {return "VISR Scene Master";};
    juce::AudioProcessorEditor* createEditor() override;
    
    void PluginSetup() override;
    void PluginUnload() override;
    
    /**
     * Retrieves the current object-based audio scene
     */
    visr::objectmodel::ObjectVector getScene() const override{
        if(objectVectorHandler!=nullptr) return objectVectorHandler->getScene();
        else return visr::objectmodel::ObjectVector();
    }
    
    /**
     * Attaches a scene observer, e.g. a GUI element
     */
    void attach(VISRObjectVectorObserver *obs) {
        views.push_back(obs);
    }
    
    /**
     * Detaches a scene observer, e.g. a GUI element
     */
    void detach(VISRObjectVectorObserver *obs) {
        auto findIt = std::find(views.begin(), views.end(), obs);
        if( findIt != views.end() ){ views.erase(findIt);}
    }
    
    /**
     * Retrieves the model for the table containing the audio objects.
     */
    juce::XmlElement& getXmlModel(){
        return *xmlModel.get();
    }
    
private:
    /**
    * Instance of a wrapper of an object-based audio scene (object vector).
    */
    std::unique_ptr<visr_support::VISRObjectVectorWrapper> objectVectorHandler;
    
    /**
     * Set of audio scene observers
     */
    std::vector <VISRObjectVectorObserver * > views;
    
    const std::string model = " <?xml version=\"1.0\" encoding=\"UTF-8\"?>"
    "<ObjectMetadataModel>"
    "<Metadata columnId=\"1\" name=\"objID\" Unit=\"\" Type=\"Text\" Range=\"\" Value=\"\" Description=\"Unique (integer) object identifier\" width=\"60\" isEditable=\"false\"/>"
    "<Metadata columnId=\"2\" name=\"azimuth\" Unit=\"m\" Type=\"Int\" Range=\"-180:180\" Value=\"0\" Description=\"Object position relative to central listening position\" width=\"100\"/>"
    "<Metadata columnId=\"3\" name=\"elevation\" Unit=\"m\" Type=\"Int\" Range=\"-90:90\" Value=\"0\" Description=\"Object position relative to central listening position\" width=\"100\"/>"
    "<Metadata columnId=\"4\" name=\"radius\" Unit=\"m\" Type=\"Double\" Range=\"0.01:100\" Value=\"1\" Description=\"Object position relative to central listening position\" width=\"100\"/>"
    "<!--<Metadata columnId=\"5\" name=\"GroupId\" Unit=\"m\" Type=\"Int\" Range=\"1:inf\" Value=\"1\" Description=\"Number of the group to which the object belongs (0 = no group)\" width=\"100\"/>-->"
    "<!--<Metadata columnId=\"6\" name=\"Level\" Unit=\"\" Type=\"Double\" Range=\"-60:12\" Value=\"0\" Description=\"Sound level of the object\"width=\"100\"/>-->"
    "<!--<Metadata columnId=\"7\" name=\"Priority\" Unit=\"\" Type=\"Int\" Range=\"0:inf\" Value=\"0\" Description=\"Priority associated to this object (0 = highest priority)\" width=\"100\"/>-->"
    "</ObjectMetadataModel>";
    
    /**
     * Model for the table containing the audio objects, definining the exposed metadata.
     */
    const juce::ScopedPointer<juce::XmlElement> xmlModel = juce::XmlDocument(model).getDocumentElement();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Processor)
};

} // namespace scene_master
} // namespace dawplugins
} // namespace visr_production_suite

#endif
