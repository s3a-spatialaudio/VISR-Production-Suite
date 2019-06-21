#include "JuceHeader.h"
#include <libobjectmodel/point_source.hpp>
#include <libobjectmodel/point_source_with_reverb.hpp>
#include <libobjectmodel/object_vector.hpp>
#include <libobjectmodel/object_vector_parser.hpp>
#include <libobjectmodel/object.hpp>
#include <libvisrsupport/VISRObjectVectorObserver.h>
#include <libcore/IDs.h>
#include <libobjectmodel/object_vector.hpp>
#include <mutex>
namespace visr_production_suite
{
namespace dawplugins
{
namespace scene_master
{

class SceneTableModel    :
public juce::Component,
public juce::TableListBoxModel,
public VISRObjectVectorObserver
{
public:
    SceneTableModel(const Processor& p, juce::XmlElement& model);
    ~SceneTableModel();
    
protected:
    // This is overloaded from TableListBoxModel, and should choose the best width for the specified
    // column.
    int getColumnAutoSizeWidth (int columnId) override;
    //==============================================================================
    void resized() override;
    
    // This is overloaded from TableListBoxModel, and should fill in the background of the whole row
    void paintRowBackground (juce::Graphics& g, int rowNumber, int /*width*/, int /*height*/, bool rowIsSelected) override;
    
    // This is overloaded from TableListBoxModel, and must paint any cells that aren't using custom
    // components.
    void paintCell (juce::Graphics& g, int rowNumber, int columnId,
                    int width, int height, bool /*rowIsSelected*/) override;
    
    // This is overloaded from TableListBoxModel, and must update any custom components that we're using
    juce::Component* refreshComponentForCell (int rowNumber, int columnId, bool /*isRowSelected*/,
                                              juce::Component* existingComponentToUpdate) override;
    
    // This is overloaded from TableListBoxModel, and must return the total number of rows in our table
    int getNumRows() override {return numRows;}
    
    void update() override;
private:
    /**
     * The table component itself
     */
    juce::ScopedPointer<juce::TableListBox> sceneTable;     //
    
    /**
     * The object-based scene, which is a set of audio objects.
     */
    visr::objectmodel::ObjectVector scene;
    int numRows;            // The number of rows in the table
    
    const Processor& processor;
    juce::ValueTree properties;
    juce::XmlElement& tableStructure; // This is the XML document used as model for the table
    
    void addRow(){ numRows++;};
    void deleteRow(){numRows--;};

    void UDPsend(int objid );
    juce::String toJSON(int objid);
    bool createSocket();
    bool deleteSocket();
    bool sendOutputStream(const juce::String& newTargetHost, int newTargetPort);
    juce::MemoryOutputStream outStream;
    juce::ScopedPointer<juce::DatagramSocket> socket;
    
    juce::Font font;
    juce::String getAttributeNameForColumnId (const int columnId) const;
    
    class CustomComponent    : public juce::Component,
        private juce::Slider::Listener,
        private juce::TextEditor::Listener
    {
    public:
        CustomComponent (SceneTableModel& td, int rownumber, juce::XmlElement& column);
        ~CustomComponent();
        void refresh(visr::objectmodel::Object& newTree);
    protected:
        void resized() override;
        void mouseDown (const juce::MouseEvent& e) override;
        void sliderValueChanged (juce::Slider* sliderThatWasMoved) override;
        void textEditorFocusLost(juce::TextEditor&) override;
        void textEditorReturnKeyPressed (juce::TextEditor&) override;
    private:
        SceneTableModel& owner;
        juce::TextEditor textLabel;
        juce::Slider numSlider;
        juce::XmlElement& field;
        juce::String fieldName;
        visr::objectmodel::Object* value;
        int rowNum;
    };
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SceneTableModel)
};
} // namespace_master
} // namespace dawplugins
} // namespace visr_production_suite
