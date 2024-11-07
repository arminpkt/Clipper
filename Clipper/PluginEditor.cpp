#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "BinaryData.h"

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

    backgroundImage1 = juce::ImageFileFormat::loadFrom(BinaryData::clipperbg_png, BinaryData::clipperbg_pngSize);
    backgroundImage2 = juce::ImageFileFormat::loadFrom(BinaryData::clipperbg2_png, BinaryData::clipperbg2_pngSize);
    knobImage1 = juce::ImageFileFormat::loadFrom(BinaryData::clipperknob1_png, BinaryData::clipperknob1_pngSize);
    knobImage2 = juce::ImageFileFormat::loadFrom(BinaryData::clipperknob2_png, BinaryData::clipperknob2_pngSize);

    setSize(backgroundImage1.getWidth(), backgroundImage1.getHeight());
    //setSize(400, 290);

    sliderLookAndFeel1 = std::make_unique<CustomSliderLookAndFeel>(knobImage1);
    sliderLookAndFeel2 = std::make_unique<CustomSliderLookAndFeel>(knobImage2);

    juce::FontOptions clipperFont = {juce::Typeface::createSystemTypefaceFor(BinaryData::PlusJakartaSans_Medium_ttf, BinaryData::PlusJakartaSans_Medium_ttfSize)};
    float fontSize = 18.f;

    thresholdSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    thresholdSlider.setLookAndFeel(sliderLookAndFeel1.get());
    thresholdSlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 90, 0);
    thresholdSlider.setPopupDisplayEnabled (true, true, this);
    addAndMakeVisible (&thresholdSlider);

    thresholdLabel.setFont(clipperFont.withHeight(fontSize));
    thresholdLabel.setColour (juce::Label::textColourId, juce::Colour(126, 126, 126));
    thresholdLabel.setJustificationType(juce::Justification::centred);
    thresholdLabel.setText ("Threshold", juce::dontSendNotification);
    addAndMakeVisible (&thresholdLabel);

    gainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    gainSlider.setLookAndFeel(sliderLookAndFeel1.get());
    gainSlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 90, 0);
    gainSlider.setPopupDisplayEnabled (true, true, this);
    addAndMakeVisible (&gainSlider);

    gainLabel.setFont(clipperFont.withHeight(fontSize));
    gainLabel.setColour (juce::Label::textColourId, juce::Colour(126, 126, 126));
    gainLabel.setJustificationType(juce::Justification::centred);
    gainLabel.setText ("Gain", juce::dontSendNotification);
    addAndMakeVisible (&gainLabel);

    tanhSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    tanhSlider.setLookAndFeel(sliderLookAndFeel2.get());
    tanhSlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 90, 0);
    tanhSlider.setPopupDisplayEnabled (true, true, this);
    addAndMakeVisible (&tanhSlider);

    tanhLabel.setFont(clipperFont.withHeight(fontSize));
    tanhLabel.setColour (juce::Label::textColourId, juce::Colour(126, 126, 126));
    tanhLabel.setJustificationType(juce::Justification::centred);
    tanhLabel.setText ("Tanh", juce::dontSendNotification);
    addAndMakeVisible (&tanhLabel);

    exponentSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    exponentSlider.setLookAndFeel(sliderLookAndFeel2.get());
    exponentSlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 90, 0);
    exponentSlider.setPopupDisplayEnabled (true, true, this);
    addAndMakeVisible (&exponentSlider);

    exponentLabel.setFont(clipperFont.withHeight(fontSize));
    exponentLabel.setColour (juce::Label::textColourId, juce::Colour(126, 126, 126));
    exponentLabel.setJustificationType(juce::Justification::centred);
    exponentLabel.setText ("Exponent", juce::dontSendNotification);
    addAndMakeVisible (&exponentLabel);

    modeMenu.setLookAndFeel(&comboBoxLookAndFeel);
    modeMenu.setColour(juce::ComboBox::ColourIds::backgroundColourId, juce::Colour(220, 220, 220));
    modeMenu.setColour(juce::ComboBox::ColourIds::outlineColourId, juce::Colour(220, 220, 220));
    modeMenu.setColour(juce::ComboBox::ColourIds::textColourId, juce::Colour(126, 126, 126));
    modeMenu.setColour(juce::PopupMenu::ColourIds::backgroundColourId, juce::Colour::fromFloatRGBA(220, 220, 220, 0.0f));
    modeMenu.addItem("Tanh", ClipperMode::Tanh + 1);
    modeMenu.addItem("Sinusoidal", ClipperMode::Sinusoidal + 1);
    modeMenu.addItem("Exponential", ClipperMode::Exponential + 1);
    modeMenu.addListener(this);
    modeMenu.setSelectedId(*processorRef.params.getRawParameterValue("uMode") + 1);
    addAndMakeVisible (&modeMenu);

    modeLabel.setFont(clipperFont.withHeight(fontSize));
    modeLabel.setColour (juce::Label::textColourId, juce::Colour(126, 126, 126));
    modeLabel.setJustificationType(juce::Justification::centred);
    modeLabel.setText ("Mode:", juce::dontSendNotification);
    addAndMakeVisible (&modeLabel);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
    thresholdSlider.setLookAndFeel(nullptr);
    gainSlider.setLookAndFeel(nullptr);
    tanhSlider.setLookAndFeel(nullptr);
    exponentSlider.setLookAndFeel(nullptr);
    modeLabel.setLookAndFeel(nullptr);
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    if (modeMenu.getSelectedId() - 1 == ClipperMode::Sinusoidal)
        g.drawImage(backgroundImage2, getLocalBounds().toFloat());
    else
        g.drawImage(backgroundImage1, getLocalBounds().toFloat());
}

void AudioPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    thresholdSlider.setBounds(26, 68, 128, 128);
    thresholdLabel.setBounds(48, 201, 84, 20);
    gainSlider.setBounds(245, 68, 128, 128);
    gainLabel.setBounds(267, 201, 84, 20);
    exponentSlider.setBounds(165, 126, 70, 70);
    exponentLabel.setBounds(158, 201, 84, 20);
    tanhSlider.setBounds(165, 126, 70, 70);
    tanhLabel.setBounds(158, 201, 84, 20);
    modeMenu.setBounds(178, 246, 113, 31);
    modeLabel.setBounds(105, 250, 84, 20);
}

void AudioPluginAudioProcessorEditor::comboBoxChanged(juce::ComboBox *comboBox) {
    if (comboBox == &modeMenu) {
        thresholdSlider.setVisible(false);
        thresholdLabel.setVisible(false);
        tanhSlider.setVisible(false);
        tanhLabel.setVisible(false);
        exponentSlider.setVisible(false);
        exponentLabel.setVisible(false);
        gainSlider.setVisible(false);
        gainLabel.setVisible(false);

        switch (modeMenu.getSelectedId()-1) {
            case ClipperMode::Tanh:
                thresholdSlider.setVisible(true);
                thresholdLabel.setVisible(true);
                gainSlider.setVisible(true);
                gainLabel.setVisible(true);
                tanhSlider.setVisible(true);
                tanhLabel.setVisible(true);
                break;
            case ClipperMode::Sinusoidal:
                thresholdSlider.setVisible(true);
                thresholdLabel.setVisible(true);
                gainSlider.setVisible(true);
                gainLabel.setVisible(true);
                break;
            case ClipperMode::Exponential:
                thresholdSlider.setVisible(true);
                thresholdLabel.setVisible(true);
                exponentSlider.setVisible(true);
                exponentLabel.setVisible(true);
                gainSlider.setVisible(true);
                gainLabel.setVisible(true);
                break;
            default:
                break;
        }

        repaint();
        resized();
    }
}

