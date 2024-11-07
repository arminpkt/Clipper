#pragma once

#include "PluginProcessor.h"
#include "CustomLookAndFeel.h"

//==============================================================================
class AudioPluginAudioProcessorEditor final : public juce::AudioProcessorEditor, public juce::ComboBox::Listener
{
public:
    explicit AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor&);
    ~AudioPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void comboBoxChanged (juce::ComboBox* comboBox) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AudioPluginAudioProcessor& processorRef;

    juce::Label titleLabel, thresholdLabel, gainLabel, tanhLabel, exponentLabel, modeLabel;

    juce::Slider thresholdSlider, gainSlider, tanhSlider, exponentSlider;

    juce::ComboBox modeMenu;

    juce::Image backgroundImage1, backgroundImage2, knobImage1, knobImage2;

    std::unique_ptr<CustomSliderLookAndFeel> sliderLookAndFeel1, sliderLookAndFeel2;
    CustomComboBoxLookAndFeel comboBoxLookAndFeel;

    juce::AudioProcessorValueTreeState::SliderAttachment thresholdAttachment, gainAttachment, tanhAttachment, exponentAttachment;
    juce::AudioProcessorValueTreeState::ComboBoxAttachment modeAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessorEditor)
};
