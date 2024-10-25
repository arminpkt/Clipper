#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p)
    : AudioProcessorEditor(&p), processorRef(p),
    thresholdAttachment(processorRef.params, "uThreshold",thresholdSlider),
    gainAttachment(processorRef.params, "uGain", gainSlider),
    tanhAttachment(processorRef.params, "uTanh", tanhSlider),
    exponentAttachment(processorRef.params, "uExponent", exponentSlider),
    modeAttachment(processorRef.params, "uMode", modeMenu) {
    juce::ignoreUnused(processorRef);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(400, 300);

    modeMenu.setSelectedId(*processorRef.params.getRawParameterValue("uMode") + 1);

    titleLabel.setFont(juce::Font(16.0f, juce::Font::bold));
    titleLabel.setColour (juce::Label::textColourId, juce::Colours::white);
    titleLabel.setJustificationType(juce::Justification::centred);
    titleLabel.setText ("Clipper", juce::dontSendNotification);
    addAndMakeVisible(&titleLabel);

    thresholdSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    thresholdSlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 90, 0);
    addAndMakeVisible (&thresholdSlider);

    thresholdLabel.setFont(juce::Font(14.0f, juce::Font::plain));
    thresholdLabel.setColour (juce::Label::textColourId, juce::Colours::white);
    thresholdLabel.setJustificationType(juce::Justification::centredTop);
    thresholdLabel.setText ("Threshold", juce::dontSendNotification);
    addAndMakeVisible (&thresholdLabel);

    gainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    gainSlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 90, 0);
    addAndMakeVisible (&gainSlider);

    gainLabel.setFont(juce::Font(14.0f, juce::Font::plain));
    gainLabel.setColour (juce::Label::textColourId, juce::Colours::white);
    gainLabel.setJustificationType(juce::Justification::centredTop);
    gainLabel.setText ("Gain", juce::dontSendNotification);
    addAndMakeVisible (&gainLabel);

    tanhSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    tanhSlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 90, 0);
    addAndMakeVisible (&tanhSlider);

    tanhLabel.setFont(juce::Font(14.0f, juce::Font::plain));
    tanhLabel.setColour (juce::Label::textColourId, juce::Colours::white);
    tanhLabel.setJustificationType(juce::Justification::centredTop);
    tanhLabel.setText ("Tanh", juce::dontSendNotification);
    addAndMakeVisible (&tanhLabel);

    exponentSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    exponentSlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 90, 0);
    addAndMakeVisible (&exponentSlider);

    exponentLabel.setFont(juce::Font(14.0f, juce::Font::plain));
    exponentLabel.setColour (juce::Label::textColourId, juce::Colours::white);
    exponentLabel.setJustificationType(juce::Justification::centredTop);
    exponentLabel.setText ("Exponent", juce::dontSendNotification);
    addAndMakeVisible (&exponentLabel);

    modeMenu.addItem("Tanh", ClipperMode::Tanh + 1);
    modeMenu.addItem("Sinusoidal", ClipperMode::Sinusoidal + 1);
    modeMenu.addItem("Exponential", ClipperMode::Exponential + 1);
    addAndMakeVisible (&modeMenu);

    modeLabel.setFont(juce::Font(14.0f, juce::Font::plain));
    modeLabel.setColour (juce::Label::textColourId, juce::Colours::white);
    modeLabel.setJustificationType(juce::Justification::centredTop);
    modeLabel.setText ("Mode", juce::dontSendNotification);
    addAndMakeVisible (&modeLabel);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    //g.setColour (juce::Colours::white);
    //g.setFont (15.0f);
    //g.drawFittedText ("Clipper", getLocalBounds(), juce::Justification::top, 1);
}

void AudioPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    float sliderLabelRatio = 0.7;

    auto bounds = getLocalBounds();

    titleLabel.setBounds(bounds.removeFromTop(bounds.getHeight() * 0.1));
    auto mainArea = bounds.removeFromTop (bounds.getHeight() * 0.6);
    auto sideArea = bounds;
    auto thresholdArea = mainArea.removeFromLeft(mainArea.getWidth()/2);
    auto gainArea = mainArea;
    auto modeArea = sideArea.removeFromLeft(sideArea.getWidth()/3);
    auto exponentArea = sideArea.removeFromLeft(sideArea.getWidth()/2);
    auto tanhArea = (sideArea);

    thresholdSlider.setBounds(thresholdArea.removeFromTop(thresholdArea.getHeight() * sliderLabelRatio));
    thresholdLabel.setBounds(thresholdArea);

    gainSlider.setBounds(gainArea.removeFromTop(gainArea.getHeight() * sliderLabelRatio));
    gainLabel.setBounds(gainArea);

    modeMenu.setBounds(modeArea.removeFromTop(modeArea.getHeight() * sliderLabelRatio));
    modeLabel.setBounds(modeArea);

    exponentSlider.setBounds(exponentArea.removeFromTop(exponentArea.getHeight() * sliderLabelRatio));
    exponentLabel.setBounds(exponentArea);

    tanhSlider.setBounds(tanhArea.removeFromTop(tanhArea.getHeight() * sliderLabelRatio));
    tanhLabel.setBounds(tanhArea);
}
