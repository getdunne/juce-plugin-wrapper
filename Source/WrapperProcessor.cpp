#include "WrapperProcessor.h"
#include "WrapperEditor.h"

#define PLUGIN_NAME "Hive"
#define PLUGIN_TYPE "VST3"
#define PLUGIN_MFR "u-he"

// This program does not maintain a known-plugins list of its own. Instead it "borrows" the one managed by the JUCE
// AudioPluginHost example program.
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    AudioPluginFormatManager pluginFormatManager;
    pluginFormatManager.addDefaultFormats();

    // Get AudioPluginHost's PluginList.xml
    PropertiesFile::Options options;
    options.applicationName = "Juce Audio Plugin Host";
    options.filenameSuffix = "settings";
    options.osxLibrarySubFolder = "Preferences";
    std::unique_ptr<PropertiesFile> knownPluginsFile;
    knownPluginsFile.reset(new PropertiesFile(options));

    std::unique_ptr<XmlElement> savedPluginList(knownPluginsFile->getXmlValue("pluginList"));
    if (savedPluginList)
    {
        KnownPluginList knownPluginList;
        knownPluginList.recreateFromXml(*savedPluginList);
        auto knownTypes = knownPluginList.getTypes();

        // Find the entry for your chosen plug-in in AudioPluginHost's list
        PluginDescription pid = knownTypes[0];
        for (auto& desc : knownTypes)
        {
            if (desc.pluginFormatName == PLUGIN_TYPE && desc.name.startsWith(PLUGIN_NAME) && desc.manufacturerName == PLUGIN_MFR)
            {
                pid = desc;
                break;
            }
        }

        String errorMessage;
        AudioPluginInstance* instance = pluginFormatManager.createPluginInstance(pid, 44100.0, 512, errorMessage).release();
        if (instance)
            return new WrapperProcessor(instance);

        DBG("Error instantiating " + pid.name);
        DBG(" ->" + errorMessage);
    }

    return nullptr;
}

//=======================================================================================================================

WrapperProcessor::WrapperProcessor(AudioPluginInstance* processorToUse)
    : AudioProcessor(getBusesPropertiesFromProcessor(processorToUse))
    , plugin(processorToUse)
{
}

WrapperProcessor::~WrapperProcessor()
{
    releaseResources();
}

AudioProcessor::BusesProperties WrapperProcessor::getBusesPropertiesFromProcessor(AudioProcessor* processor)
{
    BusesProperties retval;

    for (int dir = 0; dir < 2; ++dir)
    {
        const bool isInput = (dir == 0);
        const int n = processor->getBusCount(isInput);

        for (int i = 0; i < n; ++i)
            if (AudioProcessor::Bus* bus = processor->getBus(isInput, i))
                retval.addBus(isInput, bus->getName(), bus->getDefaultLayout(), bus->isEnabledByDefault());
    }

    return retval;
}

AudioProcessorEditor* WrapperProcessor::createEditor()
{
#if 0   // change to 1 to show the selected plug-in's own GUI window, if it has one
    if (plugin->hasEditor()) return plugin->createEditor();
    else
#else
    return new WrapperEditor(*this);
#endif
}

void WrapperProcessor::prepareToPlay(double sampleRate, int maximumExpectedSamplesPerBlock)
{
    plugin->releaseResources();
    plugin->setRateAndBufferSizeDetails(sampleRate, maximumExpectedSamplesPerBlock);

    // sync number of buses
    for (int dir = 0; dir < 2; ++dir)
    {
        const bool isInput = (dir == 0);
        int expectedNumBuses = getBusCount(isInput);
        int requiredNumBuses = plugin->getBusCount(isInput);

        for (; expectedNumBuses < requiredNumBuses; expectedNumBuses++)
            plugin->addBus(isInput);

        for (; requiredNumBuses < expectedNumBuses; requiredNumBuses++)
            plugin->removeBus(isInput);
    }

    plugin->setBusesLayout(getBusesLayout());
    plugin->prepareToPlay(sampleRate, maximumExpectedSamplesPerBlock);
}

void WrapperProcessor::releaseResources()
{
    plugin->releaseResources();
}

bool WrapperProcessor::canApplyBusCountChange(bool isInput, bool isAddingBuses,
                                              BusProperties& outNewBusProperties)
{
    if (isAddingBuses)
    {
        int busIdx = plugin->getBusCount(isInput);

        if (!plugin->addBus(isInput))
            return false;

        if (Bus* bus = plugin->getBus(isInput, busIdx))
        {
            outNewBusProperties.busName = bus->getName();
            outNewBusProperties.defaultLayout = bus->getDefaultLayout();
            outNewBusProperties.isActivatedByDefault = bus->isEnabledByDefault();

            return true;
        }
        else
        {
            jassertfalse;
            return false;
        }
    }
    else
        return plugin->removeBus(isInput);
}

void WrapperProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    plugin->processBlock(buffer, midiMessages);
}

size_t WrapperProcessor::getPluginState()
{
    plugin->getStateInformation(pluginState);

    FileChooser chooser("Save preset data file",
        File::getSpecialLocation(File::userDocumentsDirectory),
        "*.preset");
    if (chooser.browseForFileToSave(true))
    {
        File file = chooser.getResult();
        file.replaceWithData(pluginState.getData(), pluginState.getSize());
    }

    return pluginState.getSize();
}

void WrapperProcessor::setPluginState()
{
    FileChooser chooser("Choose a preset data file",
        File::getSpecialLocation(File::userDocumentsDirectory),
        "*.preset");
    if (chooser.browseForFileToOpen())
    {
        File file = chooser.getResult();
        file.loadFileAsData(pluginState);
    }

    int sizeInBytes = int(pluginState.getSize());
    void* data = pluginState.getData();
    plugin->setStateInformation(data, sizeInBytes);
}
