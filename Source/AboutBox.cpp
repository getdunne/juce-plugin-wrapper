#include "AboutBox.h"

AboutBox::AboutBox()
{
    copyrightLine.setColour(Label::textColourId, Colours::white);
    copyrightLine.setJustificationType(Justification::horizontallyCentred);
    copyrightLine.setText("(c)2019 The Very Big Corporation of America", NotificationType::dontSendNotification);
    addAndMakeVisible(copyrightLine);

    vstLine.setText("VST PlugIn Technology by Steinberg Media Technologies", NotificationType::dontSendNotification);
    vstLine.setJustificationType(Justification::horizontallyCentred);
    addAndMakeVisible(vstLine);
}

void AboutBox::paint (Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
}

void AboutBox::resized()
{
    auto bounds = getLocalBounds().reduced(10);
    copyrightLine.setBounds(bounds.removeFromTop(24));
    vstLine.setBounds(bounds.removeFromTop(24));
}

void AboutBox::launch()
{
    DialogWindow::LaunchOptions options;
    options.dialogTitle = "About this program";
    AboutBox* view = new AboutBox;
    view->setBounds(0, 0, 420, 80);
    options.content.set(view, true);
    options.useNativeTitleBar = false;
    options.launchAsync();
}
