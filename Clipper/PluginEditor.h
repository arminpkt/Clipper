#pragma once

#include "PluginProcessor.h"
#include "CustomLookAndFeel.h"
#include <map>

//==============================================================================
class AudioPluginAudioProcessorEditor final : public juce::AudioProcessorEditor, public juce::ComboBox::Listener,
                                                public juce::Slider::Listener {
public:
    explicit AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor&);
    ~AudioPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void comboBoxChanged (juce::ComboBox* comboBox) override;

    void sliderValueChanged(juce::Slider *slider) override;
    void sliderDragStarted(juce::Slider *slider) override;
    void sliderDragEnded(juce::Slider *slider) override;
    void mouseEnter(const juce::MouseEvent &event) override;
    void mouseExit(const juce::MouseEvent &event) override;

    void updateSliderLabel(juce::Slider *slider);
    void resetSliderLabel(juce::Slider *slider);
    void setupSlider(ClipperSlider &slider, const std::unique_ptr<CustomSliderLookAndFeel> &sliderLookAndFeel);

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AudioPluginAudioProcessor& processorRef;

    CustomLookAndFeel customLookAndFeel;

    SliderLabel thresholdLabel, gainLabel, tanhLabel, exponentLabel;
    juce::Label modeLabel;

    ClipperSlider thresholdSlider, gainSlider, tanhSlider, exponentSlider;
    std::map<juce::Slider*, SliderLabel*> sliderLabelMap;
    std::map<ClipperSlider*, std::unique_ptr<SliderLabelTimer>> sliderTimers;

    juce::ComboBox modeMenu;

    juce::Image backgroundImage1, backgroundImage2;
    std::unique_ptr<juce::Drawable> knobDrawable1, knobDrawable2;

    std::unique_ptr<CustomSliderLookAndFeel> sliderLookAndFeel1, sliderLookAndFeel2;
    CustomComboBoxLookAndFeel comboBoxLookAndFeel;

    juce::AudioProcessorValueTreeState::SliderAttachment thresholdAttachment, gainAttachment, tanhAttachment, exponentAttachment;
    juce::AudioProcessorValueTreeState::ComboBoxAttachment modeAttachment;

    juce::FontOptions clipperFont;
    float scale;
    float fontSize = {18.0f};
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessorEditor)
};