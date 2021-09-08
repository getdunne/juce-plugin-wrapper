#include "WrapperProcessor.h"
#include "WrapperEditor.h"
#include "AboutBox.h"

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
    };
    addAndMakeVisible(&getPluginStateButton);

    setPluginStateButton.setButtonText("Load saved state");
    setPluginStateButton.onClick = [this]()
    {
        processor.setPluginState();
    };
    addAndMakeVisible(&setPluginStateButton);

    menuButton.setButtonText("Popup Menu Test");
    menuButton.onClick = [this]()
    {
        handleMenuButton();
    };
    addAndMakeVisible(&menuButton);

    setSize(360, 220);
}

void WrapperEditor::paint (Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void WrapperEditor::resized()
{
    auto area = getLocalBounds().reduced(20);
    openPluginGuiButton.setBounds(area.removeFromTop(30));
    area.removeFromTop(20);
    getPluginStateButton.setBounds(area.removeFromTop(30));
    area.removeFromTop(20);
    setPluginStateButton.setBounds(area.removeFromTop(30));
    area.removeFromTop(20);
    menuButton.setBounds(area.removeFromTop(30));
}

void WrapperEditor::changeListenerCallback(ChangeBroadcaster*)
{
    pluginWindow.reset(nullptr);
}

void WrapperEditor::handleMenuButton()
{
    PopupMenu menu;
    menu.addItem(1, "About...");
    int sel = menu.show();
    if (sel)
    {
        AboutBox::launch();
    }
}
