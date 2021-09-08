#pragma once
#include "JuceHeader.h"
#include "WrapperProcessor.h"
#include "PluginWindow.h"

class WrapperEditor  : public AudioProcessorEditor, public ChangeListener
{
public:
    WrapperEditor (WrapperProcessor&);
    virtual ~WrapperEditor() = default;

    // Component
    void paint (Graphics&) override;
    void resized() override;

    // ChangeListener
    void changeListenerCallback(ChangeBroadcaster*) override;

protected:
    void handleMenuButton();

private:
    WrapperProcessor& processor;
    std::unique_ptr<PluginWindow> pluginWindow;
    
    TextButton openPluginGuiButton;
    TextButton getPluginStateButton;
    TextButton setPluginStateButton;

    TextButton menuButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WrapperEditor)
};
