#include "PluginProcessor.h"
#include "MetadataTable.h"
#include <libefl/cartesian_spherical_conversion.hpp>
#include <libefl/degree_radian_conversion.hpp>

using namespace juce;
using namespace visr::objectmodel;

namespace visr_production_suite
{
namespace dawplugins
{
namespace scene_master
{
using namespace visr;
    
SceneTableModel::SceneTableModel(const Processor& p, XmlElement& model)   :
numRows(0),
processor(p),
properties(p.parameters.state),
tableStructure(model),
font (14.0f)
{
    sceneTable = new TableListBox();
    addAndMakeVisible (sceneTable);
    sceneTable->setModel (this);
    
    // give it a border and a background
    sceneTable->setColour (ListBox::outlineColourId, Colours::black.withAlpha(0.7f));
    sceneTable->setColour (ListBox::backgroundColourId, Colours::transparentBlack);
    sceneTable->getHeader().setColour (TableHeaderComponent::outlineColourId, Colours::black.withAlpha(0.7f));
    sceneTable->getHeader().setColour (TableHeaderComponent::backgroundColourId, Colour(20,40,80).brighter().withAlpha(0.4f));
    sceneTable->setOutlineThickness (1);
    sceneTable->setMultipleSelectionEnabled (true);
    
    // Add some columns to the sceneTable header, based on the column list in our database..
    forEachXmlChildElement (tableStructure, columnXml)
    {
        sceneTable->getHeader().addColumn (columnXml->getStringAttribute ("name"),
                                           columnXml->getIntAttribute ("columnId"),
                                           columnXml->getIntAttribute ("width"),
                                           50, 400,TableHeaderComponent::defaultFlags);
    }
}

SceneTableModel::~SceneTableModel(){
    deleteSocket();
}

void SceneTableModel::update(){
    try
    {
        scene = processor.getScene();
        while(scene.size()>getNumRows()) addRow();
        while(scene.size()<getNumRows()) deleteRow();
        
        // This makes the table update its content via refreshComponentForCell()
        sceneTable->updateContent();
    }
    catch( std::exception const & ex )
    {
        printf("ERROR\n");
        std::cerr << "Error while updating the scene table" << ex.what() << std::endl;
    }
}

juce::Component* SceneTableModel::refreshComponentForCell (int rowNumber, int columnId, bool /*isRowSelected*/,
                                                           Component* existingComponentToUpdate)
{
    CustomComponent* objectMetadata = static_cast<CustomComponent*> (existingComponentToUpdate);
    if (objectMetadata == nullptr)
        objectMetadata = new CustomComponent (*this,rowNumber,*tableStructure.getChildElement(columnId-1));
    
    ObjectVector::iterator iter (scene.begin());
    for(int i = 0; i < rowNumber; i++) iter++;
    Object & obj(*iter);

    objectMetadata->refresh(obj);    
    return objectMetadata;
}

// this method is called when an object field has changed via mouse interaction
void SceneTableModel::UDPsend(int objid ){
    String newTargetHost = properties.getProperty("objects_ip","127.0.0.1");
    int newTargetPort = properties.getProperty("objects_port",60000);
    //    std::cout<<"connected : "<<newTargetHost<<" "<<newTargetPort<<std::endl;
    createSocket();
    String jsonMsg = toJSON(objid);
    outStream.write( jsonMsg.getCharPointer(), jsonMsg.length() );
    sendOutputStream (newTargetHost, newTargetPort+objid+1);
    outStream.reset();
}

bool SceneTableModel::createSocket()
{
    if (! deleteSocket()) return false;
    socket = new DatagramSocket (true);
    
    if (socket->bindToPort (0)) // 0 = use any local port assigned by the OS.
        return true;
    
    socket = nullptr;
    return false;
}

bool SceneTableModel::deleteSocket()
{
    socket = nullptr;
    return true;
}

bool SceneTableModel::sendOutputStream (const juce::String& newTargetHost, int newTargetPort)
{
    if (socket != nullptr)
    {
        const int streamSize = (int) outStream.getDataSize();
        const int bytesWritten = socket->write (newTargetHost, newTargetPort, outStream.getData(), streamSize);
        return bytesWritten == streamSize;
    }
    return false;
}

String SceneTableModel::toJSON(int objid){
    Object & obj( scene.at(objid) );
    visr::objectmodel::ObjectVector wrap;
    
    //TODO: retrieve object type, and isntantiate the specific object, otherwise no bidirectionality with PointSourceExtent and PointSourceWithReverb
    if(obj.type() == visr::objectmodel::ObjectTypeId::PointSource){
        visr::objectmodel::PointSource * mPs = dynamic_cast< visr::objectmodel::PointSource *>(&(obj));
        wrap.clear();
        wrap.insert( *mPs );
    }else if(obj.type() == visr::objectmodel::ObjectTypeId::PointSourceWithReverb){
        visr::objectmodel::PointSourceWithReverb * mPsWr = dynamic_cast< visr::objectmodel::PointSourceWithReverb *>(&(obj));
        wrap.clear();
        wrap.insert( *mPsWr );
    }
    
    std::ostringstream jsonStream;
    visr::objectmodel::ObjectVectorParser::encodeObjectVector(wrap, jsonStream);
    String jsonMsg = ""+jsonStream.str() + "";
    std::string s = jsonStream.str();
    return s;
}

// This is overloaded from TableListBoxModel, and should fill in the background of the whole row
void SceneTableModel::paintRowBackground (Graphics& g, int rowNumber, int /*width*/, int /*height*/, bool rowIsSelected)
{
    //    const Colour alternateColour (getLookAndFeel().findColour (ListBox::backgroundColourId)
    //                                  .interpolatedWith (getLookAndFeel().findColour (ListBox::textColourId), 0.03f));
    if (rowIsSelected)
        g.fillAll ( Colour(131,168,46).withAlpha(0.5f));
    else if (rowNumber % 2)
        g.fillAll (Colours::darkgrey.withAlpha(0.4f));
    else
        g.fillAll (Colours::darkgrey.withAlpha(0.8f));
}

// This is overloaded from TableListBoxModel, and must paint any cells that aren't using custom
// components.
void SceneTableModel::paintCell (Graphics& g, int rowNumber, int columnId,
                                 int width, int height, bool /*rowIsSelected*/)
{
    g.setColour (getLookAndFeel().findColour (ListBox::textColourId));
    g.setFont (font);
    if (const XmlElement* rowElement = tableStructure.getChildElement(rowNumber))
    {
        const String text (rowElement->getStringAttribute (getAttributeNameForColumnId (columnId)));
        g.drawText (text, 2, 0, width - 4, height, Justification::centredLeft, true);
    }
    g.setColour (getLookAndFeel().findColour (ListBox::backgroundColourId));
    g.fillRect (width - 1, 0, 1, height);
}

// This is overloaded from TableListBoxModel, and should choose the best width for the specified
// column.
int SceneTableModel::getColumnAutoSizeWidth (int columnId)
{
    if (columnId == 5)
        return 100; // (this is the ratings column, containing a custom combobox component)
    int widest = 32;

    // find the widest bit of text in this column..
    for (int i = getNumRows(); --i >= 0;)
    {
        if (const XmlElement* rowElement = tableStructure.getChildElement(i))
        {
            const String text (rowElement->getStringAttribute (getAttributeNameForColumnId (columnId)));
            widest = jmax (widest, font.getStringWidth (text));
        }
    }
    return widest + 8;
}

String SceneTableModel::getAttributeNameForColumnId (const int columnId) const
{
    forEachXmlChildElement (tableStructure, columnXml)
    {
        if (columnXml->getIntAttribute ("columnId") == columnId)
            return columnXml->getStringAttribute ("name");
    }
    return {};
}

void SceneTableModel::resized()
{
    // position our table with a gap around its edge
    sceneTable->setBoundsInset (BorderSize<int> (8));
}


//==============================================================================
SceneTableModel::CustomComponent::CustomComponent (SceneTableModel& td, int rownumber, XmlElement& column)  :
owner (td),
field(column),
rowNum(rownumber)
{
    fieldName = field.getStringAttribute("name");
    
    if(field.getStringAttribute("Type")=="Int" || field.getStringAttribute("Type")=="Double" ){
        StringArray tokens;
        tokens.addTokens (field.getStringAttribute("Range"),  ":", "\"");
        addAndMakeVisible (numSlider);
        
        if(field.getStringAttribute("Type")=="Int"){
            int min, max;
            if(tokens[0] == "-inf") min = -INT_MAX;
            else min = stoi(tokens[0].toStdString());
            
            if(tokens[1] == "inf") max = INT_MAX;
            else max = stoi(tokens[1].toStdString());
            
            numSlider.setRange (min, max, 1);
        }
        
        else if(field.getStringAttribute("Type")=="Double"){
            float min,max;
            if(tokens[0] == "-inf") min = -FLT_MAX;
            else min = stof(tokens[0].toStdString());
            
            if(tokens[1] == "inf") max = FLT_MAX;
            else max = stof(tokens[1].toStdString());
            
            numSlider.setRange (min, max, 0.01);
        }
        numSlider.setTooltip(field.getStringAttribute("Description"));
        numSlider.addMouseListener(this, true);
        numSlider.setSliderStyle (Slider::IncDecButtons);
        numSlider.setColour(Slider::ColourIds::textBoxOutlineColourId, Colours::black.withAlpha(0.7f));
        numSlider.setColour(Slider::ColourIds::textBoxTextColourId, Colours::black.withAlpha(0.7f));
        
        numSlider.setIncDecButtonsMode (Slider::incDecButtonsDraggable_Vertical);
        numSlider.setScrollWheelEnabled(false);
        if(!field.getStringAttribute("isEditable").isEmpty() && field.getStringAttribute("isEditable") == "false" )
            numSlider.setTextBoxStyle (Slider::TextBoxLeft, true, 50, 20);
        else
            numSlider.setTextBoxStyle (Slider::TextBoxLeft, false, 50, 20);
        numSlider.addListener(this);
    }else if(field.getStringAttribute("Type")=="Text"){
        addAndMakeVisible (textLabel);
        //                textLabel.setText(value.getProperty(fieldName), dontSendNotification);
        textLabel.setTooltip(field.getStringAttribute("Description"));
        
        //        textLabel.setColour(TextEditor::ColourIds::textColourId, Colours::black.withAlpha(0.7f));
        
        if(!field.getStringAttribute("isEditable").isEmpty() && field.getStringAttribute("isEditable") == "false" )
            textLabel.setReadOnly(true);
        
        textLabel.addMouseListener(this, true);
        textLabel.addListener(this);
    }
    
}

SceneTableModel::CustomComponent::~CustomComponent(){
    value=nullptr;
}
    
void SceneTableModel::CustomComponent::refresh(Object& object){
    value = &object;
    //    std::cout<<"slider refreshed: "<<field.getStringAttribute("name")<<std::endl;
    
    //TODO: retrieve object type, and isntantiate the specific object, otherwise no bidirectionality with PointSourceExtent and PointSourceWithReverb
    if(object.type() == visr::objectmodel::ObjectTypeId::PointSource || object.type() == visr::objectmodel::ObjectTypeId::PointSourceWithReverb){
        visr::objectmodel::PointSource * mPs = dynamic_cast< visr::objectmodel::PointSource *>(&(object));
        fieldName = field.getStringAttribute("name");
        
        if(fieldName=="objID"){
            textLabel.setText(std::to_string(mPs->id()+1),dontSendNotification);
        }else if(fieldName=="azimuth"){
            objectmodel::Object::Coordinate az, el, radius;
            std::tie( az, el, radius ) = efl::cartesian2spherical(mPs->x(), mPs->y(), mPs->z());
            numSlider.setValue(std::round(efl::radian2degree(az)),dontSendNotification);
        }else if(fieldName=="elevation"){
            objectmodel::Object::Coordinate az, el, radius;
            std::tie( az, el, radius ) = efl::cartesian2spherical(mPs->x(), mPs->y(), mPs->z());
            numSlider.setValue(std::round(efl::radian2degree(el)),dontSendNotification);
        }else if(fieldName=="radius"){
            objectmodel::Object::Coordinate az, el, radius;
            std::tie( az, el, radius ) = efl::cartesian2spherical(mPs->x(), mPs->y(), mPs->z());
            numSlider.setValue(radius,dontSendNotification);
        }
    }
}

void SceneTableModel::CustomComponent::resized()
{
    numSlider.setBoundsInset (BorderSize<int> (2));
    textLabel.setBoundsInset (BorderSize<int> (2));
}

void SceneTableModel::CustomComponent::mouseDown (const MouseEvent& e) {
    owner.sceneTable->selectRow(rowNum);
}

void SceneTableModel::CustomComponent::sliderValueChanged (Slider* sliderThatWasMoved)
{
//    std::cout<<"slider changed: "<<fieldName<<std::endl;
    if(value->type() == visr::objectmodel::ObjectTypeId::PointSource  || value->type() == visr::objectmodel::ObjectTypeId::PointSourceWithReverb){
        visr::objectmodel::PointSource * mPs = dynamic_cast< visr::objectmodel::PointSource *>(value);
        objectmodel::Object::Coordinate xPos, yPos, zPos;
        objectmodel::Object::Coordinate az, el, radius;
        std::tie( az, el, radius ) = efl::cartesian2spherical(mPs->x(), mPs->y(), mPs->z());
//        printf("az %f el %f radius %f, x %f y %f z %f \n", efl::radian2degree(az), efl::radian2degree(el), radius, mPs->x(), mPs->y(), mPs->z());
        if(fieldName=="azimuth"){
            az = efl::degree2radian(numSlider.getValue());
        }
        if(fieldName=="elevation"){
            el = efl::degree2radian(numSlider.getValue());
        }
        if(fieldName=="radius"){
            radius = numSlider.getValue();
        }
        std::tie( xPos, yPos, zPos ) = efl::spherical2cartesian( az, el, radius );
        mPs->setX(xPos); mPs->setY(yPos); mPs->setZ(zPos);
//        printf("az %f el %f radius %f, x %f y %f z %f \n", efl::radian2degree(az), efl::radian2degree(el), radius, mPs->x(), mPs->y(), mPs->z());
    }
    owner.UDPsend(value->id());
}

void SceneTableModel::CustomComponent::textEditorFocusLost(TextEditor&) {
    textEditorReturnKeyPressed(textLabel);
    owner.UDPsend(value->id());
}

void SceneTableModel::CustomComponent::textEditorReturnKeyPressed (TextEditor&) {
    owner.UDPsend(value->id());
}
    
} // namespace scene_master
} // namespace dawplugins
} // namespace visr_production_suite
