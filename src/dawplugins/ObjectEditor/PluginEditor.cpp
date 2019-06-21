#include "PluginEditor.h"
#include <libcore/PluginBinaryData.h>

namespace visr_production_suite
{
namespace dawplugins
{
namespace object_editor
{

using namespace juce;

Editor::Editor (Processor* opan)
: PluginEditorObject(opan)
{
    plugtitle = new ImageButton ("ImageButton");
    juceImage2 = ImageCache::getFromMemory (PluginBinaryData::s3aObjectEditor_png, PluginBinaryData::s3aObjectEditor_pngSize);
    plugtitle->setImages (true, true, true,
                          juceImage2, 1.0f, Colours::transparentBlack,
                          juceImage2, 1.0f, Colours::transparentBlack,
                          juceImage2, 1.0f, Colours::transparentBlack,
                          0.5f);
    addAndMakeVisible(plugtitle);
    resized();
}

Editor::~Editor(){}

void Editor::resized()
{
    plugtitle->setBoundsToFit(juce::Rectangle<int>(105, 12,500,25), Justification::centredLeft,false);
}

} // namespace object_editor
} // namespace dawplugins
} // namespace visr_production_suite

