
#include "MDOMetadataTable.h"

#include <string>     // std::string, std::stoi
using namespace juce;
namespace visr_production_suite
{
namespace dawplugins
{
namespace metadataeditor
{

TableComponent::TableComponent(String const jsonMetaProcName, String content, ValueTree meta, XmlElement * model)   :
contentName(content),
jsonMetaProcName(jsonMetaProcName),
font (14.0f),
columnList(model),
metadata(meta)
{
    //        metadata.addListener(this);
    table = new TableListBoxUDP(*this);
    numRows = metadata.getNumChildren();
    
    // Create our table component and add it to this component..
    addAndMakeVisible (table);
    table->setModel (this);
    
    // give it a border
    table->setColour (ListBox::outlineColourId, Colours::grey);
    table->setOutlineThickness (1);
    
    // Add some columns to the table header, based on the column list in our database..
    forEachXmlChildElement (*columnList, columnXml)
    {
        table->getHeader().addColumn (columnXml->getStringAttribute ("name"),
                                      columnXml->getIntAttribute ("columnId"),
                                      columnXml->getIntAttribute ("width"),
                                      50, 400,
                                      TableHeaderComponent::defaultFlags);
    }
    // un-comment this line to have a go of stretch-to-fit mode
    //    table.getHeader().setStretchToFitActive (true);
    
    table->setMultipleSelectionEnabled (true);
}

TableComponent::~TableComponent(){
    disconnect();
}
// This is overloaded from TableListBoxModel, and must return the total number of rows in our table
int TableComponent::getNumRows()
{
    return numRows;
}
String TableComponent::getAttributeNameForColumnId (const int columnId) const
{
    forEachXmlChildElement (*columnList, columnXml)
    {
        if (columnXml->getIntAttribute ("columnId") == columnId)
            return columnXml->getStringAttribute ("name");
    }
    return {};
}

void TableComponent::addRow()
{
    ValueTree object(contentName);
    
    int count=0;
    forEachXmlChildElement (*columnList, columnXml)
    {
        if(count==0)
            object.setProperty(columnXml->getStringAttribute ("name"), (int)metadata.getChild(metadata.getNumChildren()-1).getProperty(columnXml->getStringAttribute ("name"))+1, nullptr);
        
        
        else
            object.setProperty(columnXml->getStringAttribute ("name"), columnXml->getStringAttribute ("Value"), nullptr);
        
        count++;
    }
    
    //        std::cout<<"NUMCHI "<<metadata.getNumChildren()<<std::endl;
    metadata.addChild(object,metadata.getNumChildren(),nullptr);
    
    //        std::cout<<"METADATA STATE DUMP "<<metadata.toXmlString()<<std::endl;
    if(getAutosend())  UDPsend();
    numRows++;
    table->updateContent();
}

void TableComponent::deleteRow()
{
    int sel = table->getSelectedRow();
    if(sel!=-1){
        //            std::cout<<"ROW DUMP "<<metadata.toXmlString()<<std::endl;
        metadata.removeChild(sel,nullptr);
        //            std::cout<<"DELROW DUMP "<<metadata.toXmlString()<<std::endl;
        numRows--;
        table->updateContent();
    }
    if(getAutosend())  UDPsend();
}
void TableComponent::updateContent (ValueTree newTree)
{
    metadata = newTree;
    numRows = metadata.getNumChildren();
    table->updateContent();
    //        std::cout<<"UPDATE CONTENT "<<metadata.toXmlString()<<std::endl;
}

void TableComponent::setAutosend(bool isOn){
    autosend = isOn;
}
bool TableComponent::getAutosend(){
    return autosend;
}

    std::string TableComponent::createJSON()
{
    var json;
    
    for(std::size_t objn = 0; objn<metadata.getNumChildren(); objn++)
    {
        ValueTree child = metadata.getChild(objn);
        DynamicObject* metaobj = new DynamicObject();
        for(std::size_t metan = 0; metan<child.getNumProperties();metan++)
        {
            metaobj->setProperty(child.getPropertyName(metan),child.getProperty(child.getPropertyName(metan)));
        }
        json.append(metaobj);
    }
    
    std::string jsonMsg = "{ \"" + jsonMetaProcName.toStdString()+"\" : "+
                          "{ \"" + contentName.toStdString()+"\" : "+
                          JSON::toString(json, true /* allInOneLine*/ ).toStdString() +
                          " }"+
                          " }";
    
    if (!jsonMsg.empty() && jsonMsg[jsonMsg.length()-1] == '\n' ) {
        jsonMsg.erase(jsonMsg.length()-1);
    }
    return jsonMsg;
}

void TableComponent::UDPsend(){
    
    String newTargetHost = metadata.getParent().getProperty(contentName+"_ip","127.0.0.1");
    int newTargetPort = metadata.getParent().getProperty(contentName+"_port",9001);
    connect(newTargetHost, newTargetPort);
    // Note: Manual construction of the outermost object level is not a proper solution, the JSON representation should contain the whole message.
    std::string s = createJSON();
//    std::cout<<"NEW SEND to addr "<<newTargetHost << ":"<<newTargetPort<<std::endl<<jsonMsg<<std::endl;
    outStream.write( s.c_str(), s.length() );
    //    outStream.write( jsonMsg.getCharPointer(), jsonMsg.length() );
    sendOutputStream ();
    outStream.reset();
}

bool TableComponent::connect (const String& newTargetHost, int newTargetPort)
{
    if (! disconnect())
        return false;
    
    socket = new DatagramSocket (true);
    targetHostName = newTargetHost;
    targetPortNumber = newTargetPort;
    
    if (socket->bindToPort (0)) // 0 = use any local port assigned by the OS.
        return true;
    
    socket = nullptr;
    return false;
}

bool TableComponent::disconnect()
{
    socket = nullptr;
    return true;
}

bool TableComponent::sendOutputStream ()
{
    if (socket != nullptr)
    {
        const int streamSize = (int) outStream.getDataSize();
        
        const int bytesWritten = socket->write (targetHostName, targetPortNumber,
                                                outStream.getData(), streamSize);
        return bytesWritten == streamSize;
    }
    
    // if you hit this, you tried to send some OSC data without being
    // connected to a port! You should call OSCSender::connect() first.
    jassertfalse;
    
    return false;
}

// This is overloaded from TableListBoxModel, and should fill in the background of the whole row
void TableComponent::paintRowBackground (Graphics& g, int rowNumber, int /*width*/, int /*height*/, bool rowIsSelected)
{
    const Colour alternateColour (getLookAndFeel().findColour (ListBox::backgroundColourId)
                                        .interpolatedWith (getLookAndFeel().findColour (ListBox::textColourId), 0.03f));
    if (rowIsSelected)
        g.fillAll ( Colour(131,168,46).withAlpha(0.5f));
    else if (rowNumber % 2)
        g.fillAll (alternateColour);
}

// This is overloaded from TableListBoxModel, and must paint any cells that aren't using custom
// components.
void TableComponent::paintCell (Graphics& g, int rowNumber, int columnId,
                int width, int height, bool /*rowIsSelected*/)
{
    g.setColour (getLookAndFeel().findColour (ListBox::textColourId));
    g.setFont (font);
    
    
    
    if (const XmlElement* rowElement = columnList->getChildElement(rowNumber))
    {
        const String text (rowElement->getStringAttribute (getAttributeNameForColumnId (columnId)));
        
        g.drawText (text, 2, 0, width - 4, height, Justification::centredLeft, true);
    }
    
    g.setColour (getLookAndFeel().findColour (ListBox::backgroundColourId));
    g.fillRect (width - 1, 0, 1, height);
}


// This is overloaded from TableListBoxModel, and tells us that the user has clicked a table header
// to change the sort order.
void TableComponent::sortOrderChanged (int newSortColumnId, bool isForwards)
{
    if (newSortColumnId != 0)
    {
        DemoDataSorter sorter (getAttributeNameForColumnId (newSortColumnId), isForwards);
        metadata.sort(sorter, nullptr, false);
        //            columnList->sortChildElements (sorter);
        table->updateContent();
    }
}

// This is overloaded from TableListBoxModel, and must update any custom components that we're using
Component* TableComponent::refreshComponentForCell (int rowNumber, int columnId, bool /*isRowSelected*/,
                                    Component* existingComponentToUpdate)
{
    
    CustomComponent* metadataField = static_cast<CustomComponent*> (existingComponentToUpdate);
    
    // If an existing component is being passed-in for updating, we'll re-use it, but
    // if not, we'll have to create one.
    if (metadataField == nullptr)
        metadataField = new CustomComponent (*this,columnList->getChildElement(columnId-1));
    
    metadataField->refresh(metadata.getChild(rowNumber));
    return metadataField;
}

// This is overloaded from TableListBoxModel, and should choose the best width for the specified
// column.
int TableComponent::getColumnAutoSizeWidth (int columnId)
{
    if (columnId == 5)
        return 100; // (this is the ratings column, containing a custom combobox component)
    
    int widest = 32;
    
    // find the widest bit of text in this column..
    for (int i = getNumRows(); --i >= 0;)
    {
        if (const XmlElement* rowElement = columnList->getChildElement(i))
        {
            const String text (rowElement->getStringAttribute (getAttributeNameForColumnId (columnId)));
            widest = jmax (widest, font.getStringWidth (text));
        }
    }
    return widest + 8;
}

//==============================================================================
void TableComponent::resized()
{
    // position our table with a gap around its edge
    table->setBoundsInset (BorderSize<int> (8));
}

TableComponent::CustomComponent::CustomComponent (TableComponent& td, XmlElement * column)  :
    owner (td),
    field(column)
    {
        
        fieldName = field->getStringAttribute("name");
        if(field->getStringAttribute("Type")=="Choice"){
            StringArray tokens;
            tokens.addTokens (field->getStringAttribute("Range"),  ";", "\"");
            
            for (int i=0; i<tokens.size(); i++)
            {
                comboBox.addItem (tokens[i], i+1); // holds next token
            }
            addAndMakeVisible (comboBox);
            
            comboBox.addListener (this);
            comboBox.setWantsKeyboardFocus (false);
            comboBox.setTooltip(field->getStringAttribute("Description"));
            comboBox.addMouseListener(this, true);
        }
        else if(field->getStringAttribute("Type")=="Int" || field->getStringAttribute("Type")=="Double" ){
            StringArray tokens;
            tokens.addTokens (field->getStringAttribute("Range"),  ":", "\"");
            addAndMakeVisible (numSlider);
            
            if(field->getStringAttribute("Type")=="Int"){
                int min, max;
                if(tokens[0] == "-inf") min = -INT_MAX;
                else min = std::stoi(tokens[0].toStdString());
                
                if(tokens[1] == "inf") max = INT_MAX;
                else max = std::stoi(tokens[1].toStdString());
                
                numSlider.setRange (min, max, 1);
                //                    numSlider.setValue((int)value.getProperty(fieldName), dontSendNotification);
            }
            
            else if(field->getStringAttribute("Type")=="Double"){
                float min,max;
                if(tokens[0] == "-inf") min = -FLT_MAX;
                else min = std::stoi(tokens[0].toStdString());
                
                if(tokens[1] == "inf") max = FLT_MAX;
                else max = std::stoi(tokens[1].toStdString());
                
                numSlider.setRange (min, max, 0.01);
                //                    numSlider.setValue((double)value.getProperty(fieldName), dontSendNotification);
            }
            numSlider.setTooltip(field->getStringAttribute("Description"));
            numSlider.addMouseListener(this, true);
            numSlider.setSliderStyle (Slider::IncDecButtons);
            numSlider.setTextBoxStyle (Slider::TextBoxLeft, false, 50, 20);
            numSlider.setIncDecButtonsMode (Slider::incDecButtonsDraggable_Vertical);
            numSlider.setScrollWheelEnabled(false);
            
            numSlider.addListener(this);
            
        }
        else if(field->getStringAttribute("Type")=="Boolean"){
            addAndMakeVisible (checkBox);
            //                checkBox.setToggleState((int)value.getProperty(fieldName), dontSendNotification);
            checkBox.setTooltip(field->getStringAttribute("Description"));
            checkBox.addMouseListener(this, true);
            checkBox.addListener(this);
        }
        else if(field->getStringAttribute("Type")=="Text"){
            addAndMakeVisible (textLabel);
            //                textLabel.setText(value.getProperty(fieldName), dontSendNotification);
            textLabel.setTooltip(field->getStringAttribute("Description"));
            textLabel.addMouseListener(this, true);
            textLabel.addListener(this);
        }
        
    }
    
    void TableComponent::CustomComponent::refresh(ValueTree newTree){
        value = newTree;
        fieldName = field->getStringAttribute("name");
        if(field->getStringAttribute("Type")=="Choice"){
            comboBox.setSelectedId ((int)newTree.getProperty(fieldName),dontSendNotification);
        }
        else if(field->getStringAttribute("Type")=="Int"){
            numSlider.setValue((int)newTree.getProperty(fieldName), dontSendNotification);
        }
        
        else if(field->getStringAttribute("Type")=="Double"){
            numSlider.setValue((double)newTree.getProperty(fieldName), dontSendNotification);
        }
        else if(field->getStringAttribute("Type")=="Boolean"){
            checkBox.setToggleState((int)newTree.getProperty(fieldName), dontSendNotification);
        }
        else if(field->getStringAttribute("Type")=="Text"){
            textLabel.setText(newTree.getProperty(fieldName), dontSendNotification);
        }
    }
    
    
    void TableComponent::CustomComponent::resized()
    {
        numSlider.setBoundsInset (BorderSize<int> (2));
        comboBox.setBoundsInset (BorderSize<int> (2));
        textLabel.setBoundsInset (BorderSize<int> (2));
        checkBox.setBoundsInset (BorderSize<int> (2));
    }
    
    
    void TableComponent::CustomComponent::mouseDown (const MouseEvent& e) {
        //            int row = owner.metadata.indexOf(value);
        //            std::cout<<"rownumbermd "<<rownumber<<std::endl;;
        owner.table->selectRow(owner.metadata.indexOf(value));
    }
    void TableComponent::CustomComponent::comboBoxChanged (ComboBox*)
    {
        value.setProperty(fieldName,comboBox.getSelectedId(),nullptr);
        if(owner.getAutosend())  owner.UDPsend();
        //                std::cout<<"load "<<owner.metadata.getChildWithName(owner.tableID).toXmlString()<<std::endl;
    }
    void TableComponent::CustomComponent::sliderValueChanged (Slider* sliderThatWasMoved)
    {
        value.setProperty(fieldName,numSlider.getValue(),nullptr);
        if(owner.getAutosend())  owner.UDPsend();
    }
    
    void TableComponent::CustomComponent::buttonClicked (Button* buttonThatWasClicked)
    {
        value.setProperty(fieldName,checkBox.getToggleState()==0?0:1,nullptr);
        if(owner.getAutosend())  owner.UDPsend();
    }
    void TableComponent::CustomComponent::textEditorFocusLost(TextEditor&) {
        textEditorReturnKeyPressed(textLabel); //<gc> in vst textEditorReturnKeyPressed is not recognized, this is a trick
        if(owner.getAutosend())  owner.UDPsend();
    }
    
    void TableComponent::CustomComponent::textEditorReturnKeyPressed (TextEditor&) {
        value.setProperty(fieldName,textLabel.getText(),nullptr);
        if(owner.getAutosend())  owner.UDPsend();
    }
    
   TableComponent:: CustomComponent::~CustomComponent(){
        field =nullptr;
    }
    

TableComponent::DemoDataSorter::DemoDataSorter (const String& attributeToSortBy, bool forwards)
    : attributeToSort (attributeToSortBy),
    direction (forwards ? 1 : -1)
    {
    }
    
    int TableComponent::DemoDataSorter::compareElements (const ValueTree &first, const ValueTree &second)
    {
        int fs = first.getProperty (attributeToSort);
        int sd = second.getProperty (attributeToSort);
        
        int result = (fs < sd) ? -1 : ((sd < fs) ? 1 : 0);
        
        return direction * result;
    }


TableComponent::TableListBoxUDP::TableListBoxUDP(TableComponent& td):
    owner(td){};
    //        TableListBoxUDP::~TableListBoxUDP(){};
    
    //        void TableListBoxUDP::selectedRowsChanged (int row){
    //            owner.UDPsend();
    //        }
} // namespace metadataeditor
} // namespace dawplugins
} // namespace visr_production_suite

