# juce-wrapper
This is a JUCE based plug-in that loads and wraps a single VST, VST3, or AU plug-in (in this case an instrument plug-in, which requires MIDI in and stereo audio-out), based on [this code example provided by Fabian Renn-Giles in the JUCE forum](https://forum.juce.com/t/host-a-vst-in-an-audio-plugin/23791).

It serves as a simple starting point for experiments with the JUCE hosting code.

You will need your own copy of the [JUCE Framework](https://juce.com/) to work with this code. This code was developed using JUCE version 6.0.7.

## Known plug-ins list

One tricky aspect of building a JUCE-based host program is the need to create a "known plug-ins list" (class juce::KnownPluginList), and register various plug-ins into it. This code skirts that obstacle by "borrowing" the known plug-ins list managed by the JUCE [AudioPluginHost](https://github.com/juce-framework/JUCE/tree/master/extras/AudioPluginHost) example program. You will need to compile and run that one, and register at least one of your plug-ins (I used [u-he's Hive 2](https://u-he.com/products/hive/) in VST3 format).

## Memory leak issue

If you run this program in a debugger, and set up to load a VST3 plug-in, it will most likely stop at a *jassert* line and report a couple of memory leaks. You can just continue execution until it shuts down normally. This appears to be a JUCE issue, which I don't have time to track down at the moment.

