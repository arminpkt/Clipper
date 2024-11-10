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

    /*
    backgroundImage1 = juce::ImageFileFormat::loadFrom(BinaryData::clipperbg_png, BinaryData::clipperbg_pngSize);
    backgroundImage2 = juce::ImageFileFormat::loadFrom(BinaryData::clipperbg2_png, BinaryData::clipperbg2_pngSize);
    knobImage1 = juce::ImageFileFormat::loadFrom(BinaryData::clipperknob1_png, BinaryData::clipperknob1_pngSize);
    knobImage2 = juce::ImageFileFormat::loadFrom(BinaryData::clipperknob2_png, BinaryData::clipperknob2_pngSize);
    */

    backgroundDrawable1 = juce::Drawable::createFromImageData(BinaryData::clipperbg_svg, BinaryData::clipperbg_svgSize);
    backgroundDrawable2 = juce::Drawable::createFromImageData(BinaryData::clipperbg2_svg, BinaryData::clipperbg2_svgSize);
    knobDrawable1 = juce::Drawable::createFromImageData(BinaryData::clipperknob1_svg, BinaryData::clipperknob1_svgSize);
    knobDrawable2 = juce::Drawable::createFromImageData(BinaryData::clipperknob2_svg, BinaryData::clipperknob2_svgSize);

    //setSize(backgroundImage1.getWidth(), backgroundImage1.getHeight());
    setSize(400, 290);
    setResizable(true, true); // Allow resizing
    setResizeLimits(400, 290, 800, 580); // Minimum and maximum sizes maintaining aspect ratio
    getConstrainer()->setFixedAspectRatio(400.0 / 290.0); // Keep aspect ratio of 400x290 (original size)


    sliderLookAndFeel1 = std::make_unique<CustomSliderLookAndFeel>(knobDrawable1->createCopy());
    sliderLookAndFeel2 = std::make_unique<CustomSliderLookAndFeel>(knobDrawable2->createCopy());


    juce::FontOptions clipperFont = {juce::Typeface::createSystemTypefaceFor(BinaryData::PlusJakartaSans_Medium_ttf, BinaryData::PlusJakartaSans_Medium_ttfSize)};
   //float fontSize = 18.f;

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
    juce::Rectangle<float> bounds = getLocalBounds().toFloat();

    if (modeMenu.getSelectedId() - 1 == ClipperMode::Sinusoidal) {
        if (backgroundDrawable2 != nullptr) {
            backgroundDrawable2->setTransformToFit(bounds, juce::RectanglePlacement::stretchToFit);
            backgroundDrawable2->draw(g, 1.0f);
        }
    } else {
        if (backgroundDrawable1 != nullptr) {
            backgroundDrawable1->setTransformToFit(bounds, juce::RectanglePlacement::stretchToFit);
            backgroundDrawable1->draw(g, 1.0f);
        }
    }
}

void AudioPluginAudioProcessorEditor::resized()
{
    float scale = (float) getWidth() / 400.0f; // Scale factor based on original width

    // Scale each component's position and size
    thresholdSlider.setBounds(juce::Rectangle<int>(26, 68, 128, 128).toFloat().transformedBy(juce::AffineTransform::scale(scale)).toNearestInt());
    thresholdLabel.setBounds(juce::Rectangle<int>(48, 201, 84, 20).toFloat().transformedBy(juce::AffineTransform::scale(scale)).toNearestInt());

    gainSlider.setBounds(juce::Rectangle<int>(245, 68, 128, 128).toFloat().transformedBy(juce::AffineTransform::scale(scale)).toNearestInt());
    gainLabel.setBounds(juce::Rectangle<int>(267, 201, 84, 20).toFloat().transformedBy(juce::AffineTransform::scale(scale)).toNearestInt());

    exponentSlider.setBounds(juce::Rectangle<int>(165, 126, 70, 70).toFloat().transformedBy(juce::AffineTransform::scale(scale)).toNearestInt());
    exponentLabel.setBounds(juce::Rectangle<int>(158, 201, 84, 20).toFloat().transformedBy(juce::AffineTransform::scale(scale)).toNearestInt());

    tanhSlider.setBounds(juce::Rectangle<int>(165, 126, 70, 70).toFloat().transformedBy(juce::AffineTransform::scale(scale)).toNearestInt());
    tanhLabel.setBounds(juce::Rectangle<int>(158, 201, 84, 20).toFloat().transformedBy(juce::AffineTransform::scale(scale)).toNearestInt());

    modeMenu.setBounds(juce::Rectangle<int>(178, 246, 113, 31).toFloat().transformedBy(juce::AffineTransform::scale(scale)).toNearestInt());
    modeLabel.setBounds(juce::Rectangle<int>(105, 250, 84, 20).toFloat().transformedBy(juce::AffineTransform::scale(scale)).toNearestInt());

    float scaledFontSize = fontSize * scale;
    thresholdLabel.setFont(thresholdLabel.getFont().withHeight(scaledFontSize));
    gainLabel.setFont(gainLabel.getFont().withHeight(scaledFontSize));
    tanhLabel.setFont(tanhLabel.getFont().withHeight(scaledFontSize));
    exponentLabel.setFont(exponentLabel.getFont().withHeight(scaledFontSize));
    modeLabel.setFont(modeLabel.getFont().withHeight(scaledFontSize));
    comboBoxLookAndFeel.setFontSize(scaledFontSize);
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

