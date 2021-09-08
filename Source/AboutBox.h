#pragma once
#include <JuceHeader.h>

class AboutBox    : public Component
{
public:
    AboutBox();

    void paint (Graphics&) override;
    void resized() override;

    static void launch();

private:
    Label copyrightLine;
    Label vstLine;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AboutBox)
};
