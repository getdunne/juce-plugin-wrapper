#pragma once
#include "JuceHeader.h"
#include "WrapperProcessor.h"
#include "PluginWindow.h"

class WrapperEditor  : public AudioProcessorEditor, public ChangeListener
{
public:
    WrapperEditor (WrapperProcessor&);
    ~WrapperEditor();

    // Component
    void paint (Graphics&) override;
    void resized() override;

    // ChangeListener
    void changeListenerCallback(ChangeBroadcaster*) override;

private:
    WrapperProcessor& processor;
    std::unique_ptr<PluginWindow> pluginWindow;
    
    TextButton openPluginGuiButton;
    TextButton getPluginStateButton;
    TextButton setPluginStateButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WrapperEditor)
};
