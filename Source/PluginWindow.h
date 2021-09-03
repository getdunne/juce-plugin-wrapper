#pragma once
#include "JuceHeader.h"

class PluginWindow : public DocumentWindow, public ChangeBroadcaster
{
public:
    PluginWindow(AudioProcessor* thePlugin);
    ~PluginWindow();

    void closeButtonPressed() override;

private:
    AudioProcessor* plugin;
    std::unique_ptr<AudioProcessorEditor> editor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginWindow)
};
