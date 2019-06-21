#include "JuceHeader.h"
#include <libcore/PluginBinaryData.h>
namespace visr_production_suite
{
namespace dawplugins
{
namespace metadataeditor
{

class TableComponent    : public juce::Component,
public juce::TableListBoxModel
{
public:
    TableComponent(juce::String const jsonMetaProcName, juce::String content, juce::ValueTree meta, juce::XmlElement * model);
    
    ~TableComponent();
    // This is overloaded from TableListBoxModel, and must return the total number of rows in our table
    int getNumRows() override;
    
    void addRow();
    
    void deleteRow();
    void updateContent (juce::ValueTree newTree);
    
    void setAutosend(bool isOn);
    bool getAutosend();
    
    std::string createJSON();
    void UDPsend();
    
    bool connect (const juce::String& newTargetHost, int newTargetPort);
    bool disconnect();
    bool sendOutputStream ();
    
    // This is overloaded from TableListBoxModel, and should fill in the background of the whole row
    void paintRowBackground (juce::Graphics& g, int rowNumber, int /*width*/, int /*height*/, bool rowIsSelected) override;
    
    // This is overloaded from TableListBoxModel, and must paint any cells that aren't using custom
    // components.
    void paintCell (juce::Graphics& g, int rowNumber, int columnId,
                    int width, int height, bool /*rowIsSelected*/) override;
    
    // This is overloaded from TableListBoxModel, and tells us that the user has clicked a table header
    // to change the sort order.
    void sortOrderChanged (int newSortColumnId, bool isForwards) override;
    
    // This is overloaded from TableListBoxModel, and must update any custom components that we're using
    Component* refreshComponentForCell (int rowNumber, int columnId, bool /*isRowSelected*/,
                                        Component* existingComponentToUpdate) override;
    // This is overloaded from TableListBoxModel, and should choose the best width for the specified
    // column.
    int getColumnAutoSizeWidth (int columnId) override;
    //==============================================================================
    void resized() override;
    
    juce::String contentName;
    juce::String const jsonMetaProcName;
private:
    juce::String getAttributeNameForColumnId (const int columnId) const;
    
    juce::MemoryOutputStream outStream;
    juce::Font font;
    juce::ScopedPointer<juce::DatagramSocket> socket;
    juce::String targetHostName;
    int targetPortNumber = 0;
    
    juce::ScopedPointer<juce::XmlElement> demoData;   // This is the XML document loaded from the embedded file "demo table data.xml"
    juce::XmlElement * columnList; // A pointer to the sub-node of demoData that contains the list of columns
    
    bool autosend;
    juce::ValueTree metadata;
    int numRows=0;            // The number of rows of data we've got
    
    class CustomComponent    : public juce::Component,
    public juce::Slider::Listener,
    public juce::Button::Listener,
    public juce::TextEditor::Listener,
    private juce::ComboBox::Listener
    {
    public:
        CustomComponent (TableComponent& td, juce::XmlElement * column);
        void refresh(juce::ValueTree newTree);
        void resized() override;
        
        void mouseDown (const juce::MouseEvent& e) override;
        void comboBoxChanged (juce::ComboBox*) override;
        void sliderValueChanged (juce::Slider* sliderThatWasMoved) override;
        void buttonClicked (juce::Button* buttonThatWasClicked) override;
        void textEditorFocusLost(juce::TextEditor&) override;
        void textEditorReturnKeyPressed (juce::TextEditor&) override;
        
        ~CustomComponent();
        
    private:
        TableComponent& owner;
        juce::ComboBox comboBox;
        juce::ToggleButton checkBox;
        juce::TextEditor textLabel;
        juce::Slider numSlider;
        juce::XmlElement* field;
        juce::ValueTree value;
        juce::String fieldName;
    };
    
    //==============================================================================
    // A comparator used to sort our data when the user clicks a column header
    
    class DemoDataSorter
    {
    public:
        DemoDataSorter (const juce::String& attributeToSortBy, bool forwards);
        int compareElements (const juce::ValueTree &first, const juce::ValueTree &second);
    private:
        juce::String attributeToSort;
        int direction;
    };
    
    class TableListBoxUDP: public juce::TableListBox
    {
    public:
        TableListBoxUDP(TableComponent& td);
        //        ~TableListBoxUDP(){};
        
        //        void selectedRowsChanged (int row) override{
        //            owner.UDPsend();
        //        }
        TableComponent& owner;
    };
    juce::ScopedPointer<TableListBoxUDP> table;     // the table component itself
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TableComponent)
};
} // namespace metadataeditor
} // namespace dawplugins
} // namespace visr_production_suite

