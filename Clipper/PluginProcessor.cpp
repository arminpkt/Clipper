#include "PluginProcessor.h"
#include "PluginEditor.h"
//#include <juce_dsp/juce_dsp.h>

//==============================================================================
AudioPluginAudioProcessor::AudioPluginAudioProcessor()
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), params(*this, nullptr, "Parameters", createParameterLayout())
{
    initializePropertiesFile();

    // Use the parameter ID to return a pointer to our parameter data
    clipperMode = params.getRawParameterValue("uMode");
    thresholdInDB = params.getRawParameterValue("uThreshold");
    tanhCoefficient = params.getRawParameterValue("uTanh");
    exponent = params.getRawParameterValue("uExponent");
    gainInDB = params.getRawParameterValue("uGain");

    // for each input channel emplace one filter
    for(auto i = 0; i < getBusesLayout().getNumChannels(true, 0); ++i){
        clippers.emplace_back();
    }
}

AudioPluginAudioProcessor::~AudioPluginAudioProcessor()
{
    propertiesFile->setValue(windowScaleKey, windowScale); // Save scale to properties file
    propertiesFile->saveIfNeeded();
}

//==============================================================================
const juce::String AudioPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AudioPluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AudioPluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AudioPluginAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AudioPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AudioPluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AudioPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AudioPluginAudioProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

const juce::String AudioPluginAudioProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

void AudioPluginAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

//==============================================================================
void AudioPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    juce::ignoreUnused (samplesPerBlock);
    for(auto & clipper : clippers){
        clipper.prepare(sampleRate);
    }
}

void AudioPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool AudioPluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}

void AudioPluginAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                              juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused (midiMessages);
    for(auto & clipper : clippers){
      clipper.setMode(static_cast<ClipperMode>(clipperMode->load()));
      clipper.setThreshold(juce::Decibels::decibelsToGain(thresholdInDB->load()));
      clipper.setTanhCoefficient(*tanhCoefficient);
      clipper.setExponent(*exponent);
      clipper.setGain(juce::Decibels::decibelsToGain(gainInDB->load()));
    }

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (auto channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* writePointer = buffer.getWritePointer (channel);
        auto* readPointer = buffer.getReadPointer (channel);
		for(auto sample = 0; sample < buffer.getNumSamples(); ++sample){
			//writePointer[sample] = juce::dsp::FastMathApproximations::tanh (5 * readPointer[sample]);
		    writePointer[sample] = static_cast<float>(clippers[static_cast<std::vector<Clipper>::size_type>(channel)].process(readPointer[sample]));
		}
    }
}

//==============================================================================
bool AudioPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* AudioPluginAudioProcessor::createEditor()
{
    return new AudioPluginAudioProcessorEditor (*this);
}

//==============================================================================
void AudioPluginAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    auto state = params.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void AudioPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (params.state.getType()))
            params.replaceState (juce::ValueTree::fromXml (*xmlState));
}

juce::AudioProcessorValueTreeState::ParameterLayout AudioPluginAudioProcessor::createParameterLayout() {
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { "uThreshold", 1},
        "Threshold", juce::NormalisableRange<float>(-51.8f, 0.f, 0.01f, 3.0f),
        -4.4, juce::String(), juce::AudioProcessorParameter::genericParameter,
        [](float value, int) { return juce::String(value, 2) + " dB"; }));
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { "uTanh", 1},
        "Tanh", 0.1, 10.0, 1.0));
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { "uExponent", 1},
        "Exp", 0.1, 50.0, 10));
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { "uGain", 1},
        "Gain", juce::NormalisableRange<float>(-70.0f, 6.0f, 0.01f, 2.5f),
        0.0, juce::String(), juce::AudioProcessorParameter::genericParameter,
        [](float value, int) {
            return value <= -70.f ? "-inf dB" : juce::String(value, 2) + " dB";
        }));
    juce::StringArray stringArray;
    stringArray.add ("Tanh");
    stringArray.add ("Sinusoidal");
    stringArray.add ("Exponential");
    layout.add(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID {"uMode", 1},
        "Mode", stringArray, 0));

    return layout;
}

void AudioPluginAudioProcessor::initializePropertiesFile()
{
    // Set up the options for the properties file
    juce::PropertiesFile::Options options;
    options.applicationName     = "Clipper";
    options.filenameSuffix      = ".settings";  // Use a custom suffix
    options.osxLibrarySubFolder = "Application Support"; // Save in Application Support folder on macOS
    options.folderName          = "Armin/Clipper"; // Folder structure in AppData or Application Support
    options.storageFormat       = juce::PropertiesFile::storeAsXML; // Store as XML (or use storeAsBinary)

    // Create or load the properties file
    propertiesFile = std::make_unique<juce::PropertiesFile>(options);

    // Load the initial scale from properties, if it exists
    windowScale = static_cast<float>(propertiesFile->getDoubleValue(windowScaleKey, 1.0f));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AudioPluginAudioProcessor();
}
