#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "BinaryData.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p)
    : AudioProcessorEditor(&p), processorRef(p), thresholdLabel("Threshold"), gainLabel("Gain"), tanhLabel("Tanh"), exponentLabel("Exponent"),
      thresholdAttachment(processorRef.params, "uThreshold", thresholdSlider),
      gainAttachment(processorRef.params, "uGain", gainSlider),
      tanhAttachment(processorRef.params, "uTanh", tanhSlider),
      exponentAttachment(processorRef.params, "uExponent", exponentSlider),
      modeAttachment(processorRef.params, "uMode", modeMenu) {
    juce::ignoreUnused(processorRef);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setLookAndFeel(&customLookAndFeel);

    backgroundImage1 = juce::ImageFileFormat::loadFrom(BinaryData::clipperbg_png, BinaryData::clipperbg_pngSize);
    backgroundImage2 = juce::ImageFileFormat::loadFrom(BinaryData::clipperbg2_png, BinaryData::clipperbg2_pngSize);

    knobDrawable1 = juce::Drawable::createFromImageData(BinaryData::clipperknob1_svg, BinaryData::clipperknob1_svgSize);
    knobDrawable2 = juce::Drawable::createFromImageData(BinaryData::clipperknob2_svg, BinaryData::clipperknob2_svgSize);

    sliderLookAndFeel1 = std::make_unique<CustomSliderLookAndFeel>(knobDrawable1->createCopy());
    sliderLookAndFeel2 = std::make_unique<CustomSliderLookAndFeel>(knobDrawable2->createCopy());

    clipperFont = {
        juce::Typeface::createSystemTypefaceFor(BinaryData::PlusJakartaSans_Medium_ttf,
                                                BinaryData::PlusJakartaSans_Medium_ttfSize)
    };

    sliderLabelMap[&thresholdSlider] = &thresholdLabel;
    sliderLabelMap[&gainSlider] = &gainLabel;
    sliderLabelMap[&tanhSlider] = &tanhLabel;
    sliderLabelMap[&exponentSlider] = &exponentLabel;

    setupSlider(thresholdSlider, sliderLookAndFeel1);
    setupSlider(gainSlider, sliderLookAndFeel1);
    setupSlider(tanhSlider, sliderLookAndFeel2);
    setupSlider(exponentSlider, sliderLookAndFeel2);

    modeMenu.setLookAndFeel(&comboBoxLookAndFeel);
    modeMenu.setColour(juce::ComboBox::ColourIds::backgroundColourId, juce::Colour(220, 220, 220));
    modeMenu.setColour(juce::ComboBox::ColourIds::outlineColourId, juce::Colour(220, 220, 220));
    modeMenu.setColour(juce::ComboBox::ColourIds::textColourId, juce::Colour(126, 126, 126));
    modeMenu.setColour(juce::PopupMenu::ColourIds::backgroundColourId,
                       juce::Colour::fromFloatRGBA(220, 220, 220, 0.0f));
    modeMenu.addItem("Tanh", ClipperMode::Tanh + 1);
    modeMenu.addItem("Sinusoidal", ClipperMode::Sinusoidal + 1);
    modeMenu.addItem("Exponential", ClipperMode::Exponential + 1);
    modeMenu.addListener(this);
    modeMenu.setSelectedId(*processorRef.params.getRawParameterValue("uMode") + 1);
    addAndMakeVisible(&modeMenu);

    modeLabel.setFont(clipperFont.withHeight(fontSize));
    modeLabel.setColour(juce::Label::textColourId, juce::Colour(126, 126, 126));
    modeLabel.setJustificationType(juce::Justification::centred);
    modeLabel.setText("Mode:", juce::dontSendNotification);
    addAndMakeVisible(&modeLabel);

    scale = processorRef.windowScale;
    setSize(400 * scale, 290 * scale);
    setResizable(true, true); // Allow resizing
    setResizeLimits(400, 290, 800, 580); // Minimum and maximum sizes maintaining aspect ratio
    getConstrainer()->setFixedAspectRatio(400.0 / 290.0); // Keep aspect ratio of 400x290 (original size)
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
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
        if (!backgroundImage2.isNull())
            g.drawImage(backgroundImage2, bounds, juce::RectanglePlacement::stretchToFit);
    } else {
        if (!backgroundImage1.isNull())
            g.drawImage(backgroundImage1, bounds, juce::RectanglePlacement::stretchToFit);
    }
}

void AudioPluginAudioProcessorEditor::resized()
{
    scale = static_cast<float>(getWidth()) / 400.0f;
    processorRef.windowScale = scale;

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

void AudioPluginAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    if (auto* clipperSlider = dynamic_cast<ClipperSlider*>(slider))
    {
        // Check if the change is due to automation by seeing if the slider is dragged
        if (clipperSlider->isMouseButtonDown()) {
            // Perform any actions when the slider is being dragged by the user
        } else {
            // Start the timer for the specific ClipperSlider
            sliderTimers[clipperSlider]->startTimer(600);
        }
        updateSliderLabel(clipperSlider);
    }
}

void AudioPluginAudioProcessorEditor::sliderDragStarted(juce::Slider* slider)
{
    updateSliderLabel(slider);
}

void AudioPluginAudioProcessorEditor::sliderDragEnded(juce::Slider* slider)
{
    if (auto clipperSlider = dynamic_cast<ClipperSlider*>(slider))
        if (!clipperSlider->isHovered())
            sliderLabelMap[clipperSlider]->resetText();
}

void AudioPluginAudioProcessorEditor::mouseEnter(const juce::MouseEvent& event)
{
    if (auto* slider = dynamic_cast<ClipperSlider*>(event.eventComponent))
    {
        slider->setHovered(true);
        updateSliderLabel(slider);
    }
}

void AudioPluginAudioProcessorEditor::mouseExit(const juce::MouseEvent& event)
{
    if (auto* slider = dynamic_cast<ClipperSlider*>(event.eventComponent))
    {
        slider->setHovered(false);
        sliderLabelMap[slider]->resetText();
    }
}

void AudioPluginAudioProcessorEditor::setupSlider(ClipperSlider& slider, const std::unique_ptr<CustomSliderLookAndFeel> &sliderLookAndFeel)
{
    slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    slider.setLookAndFeel(sliderLookAndFeel.get());
    slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    slider.addListener(this);
    slider.setInterceptsMouseClicks(true, false);
    slider.addMouseListener(this, false);
    addChildComponent(&slider);
    sliderTimers[&slider] = std::make_unique<SliderLabelTimer>(&slider, sliderLabelMap[&slider]);

    auto label = sliderLabelMap[&slider];
    label->setFont(clipperFont.withHeight(fontSize));
    label->setColour(juce::Label::textColourId, juce::Colour(126, 126, 126));
    label->setJustificationType(juce::Justification::centred);
    addChildComponent(label);
}

// Helper function to set label text to parameter value
void AudioPluginAudioProcessorEditor::updateSliderLabel(juce::Slider* slider)
{
    juce::Label* targetLabel = nullptr;
    juce::String valueText;

    if (slider == &thresholdSlider)
    {
        targetLabel = &thresholdLabel;
        if (auto* param = processorRef.params.getParameter("uThreshold"))
            valueText = param->getCurrentValueAsText();
    }
    else if (slider == &gainSlider)
    {
        targetLabel = &gainLabel;
        if (auto* param = processorRef.params.getParameter("uGain"))
            valueText = param->getCurrentValueAsText();
    }
    else if (slider == &tanhSlider)
    {
        targetLabel = &tanhLabel;
        if (auto* param = processorRef.params.getParameter("uTanh"))
            valueText = param->getCurrentValueAsText();
    }
    else if (slider == &exponentSlider)
    {
        targetLabel = &exponentLabel;
        if (auto* param = processorRef.params.getParameter("uExponent"))
            valueText = param->getCurrentValueAsText();
    }

    if (targetLabel != nullptr)
    {
        targetLabel->setText(valueText, juce::dontSendNotification);
    }
}