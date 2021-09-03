#include "WrapperProcessor.h"
#include "WrapperEditor.h"

WrapperEditor::WrapperEditor (WrapperProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    openPluginGuiButton.setButtonText("Open plugin GUI");
    openPluginGuiButton.onClick = [this]()
    {
        if (pluginWindow) return;
        pluginWindow.reset(new PluginWindow(processor.getPlugin()));
        pluginWindow->addChangeListener(this);
    };
    addAndMakeVisible(&openPluginGuiButton);

    getPluginStateButton.setButtonText("Save plugin state");
    getPluginStateButton.onClick = [this]()
    {
        processor.getPluginState();
        repaint();
    };
    addAndMakeVisible(&getPluginStateButton);

    setPluginStateButton.setButtonText("Load saved state");
    setPluginStateButton.onClick = [this]()
    {
        processor.setPluginState();
        repaint();
    };
    addAndMakeVisible(&setPluginStateButton);

    setSize(400, 300);
}

WrapperEditor::~WrapperEditor()
{
}

void WrapperEditor::paint (Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void WrapperEditor::resized()
{
    auto area = getLocalBounds().reduced(40);
    openPluginGuiButton.setBounds(area.removeFromTop(30));
    area.removeFromTop(20);
    getPluginStateButton.setBounds(area.removeFromTop(30));
    area.removeFromTop(20);
    setPluginStateButton.setBounds(area.removeFromTop(30));
}

void WrapperEditor::changeListenerCallback(ChangeBroadcaster*)
{
    pluginWindow.reset(nullptr);
}
