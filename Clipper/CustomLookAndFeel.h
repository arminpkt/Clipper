//
// Created by Armin Peukert on 05.11.24.
//
#pragma once

#include "BinaryData.h"

class CustomSliderLookAndFeel : public juce::LookAndFeel_V4
{
public:
    CustomSliderLookAndFeel(std::unique_ptr<juce::Drawable> kD) : knobDrawable(std::move(kD)) {}

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                          float sliderPos, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override
    {
        if (knobDrawable != nullptr) {
            const float rotation = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
            const float centerX = x + width * 0.5f;
            const float centerY = y + height * 0.5f;

            // Calculate scale factor based on slider size
            float scaleX = (float) width / knobDrawable->getWidth();
            float scaleY = (float) height / knobDrawable->getHeight();

            // Apply both scaling and rotation transformations
            juce::AffineTransform transform = juce::AffineTransform::scale(scaleX, scaleY)
                                              .rotated(rotation, centerX, centerY);

            knobDrawable->setTransform(transform);

            // Draw the drawable using its own bounds
            knobDrawable->draw(g, 1.0f);
        }
    }

private:
    std::unique_ptr<juce::Drawable> knobDrawable;
};

class CustomComboBoxLookAndFeel : public juce::LookAndFeel_V4
{
public:
    CustomComboBoxLookAndFeel() {
        setColour(juce::PopupMenu::backgroundColourId, juce::Colour(226, 226, 226).withAlpha (0.99f));
        setColour(juce::PopupMenu::highlightedBackgroundColourId, juce::Colour(220, 220, 220));
        setColour(juce::PopupMenu:: textColourId, juce::Colour(126, 126, 126));
        setColour(juce::PopupMenu:: highlightedTextColourId, juce::Colour(126, 126, 126));

    }

    juce::FontOptions clipperFont = {juce::Typeface::createSystemTypefaceFor(BinaryData::PlusJakartaSans_Medium_ttf, BinaryData::PlusJakartaSans_Medium_ttfSize)};

    float fontSize = 18.f;

    void setFontSize(float newSize) {
        fontSize = newSize;
    }

    juce::Font getComboBoxFont (juce::ComboBox& box)
    {
        return withDefaultMetrics (clipperFont).withHeight(fontSize);
    }

    void drawComboBox (juce::Graphics& g, int width, int height, bool isButtonDown,
                          int buttonX, int buttonY, int buttonW, int buttonH,
                          juce::ComboBox& box) override
    {
        //auto cornerSize = box.findParentComponentOfClass<juce::ChoicePropertyComponent>() != nullptr ? 0.0f : 3.0f;
        //setColour(juce::PopupMenu::backgroundColourId, juce::Colour(226, 226, 226).withAlpha (0.99f));
        auto cornerSize = 7.0f;
        juce::Rectangle<int> boxBounds (0, 0, width, height);

        g.setColour (box.findColour (juce::ComboBox::backgroundColourId));
        g.fillRoundedRectangle (boxBounds.toFloat(), cornerSize);

        g.setColour (box.findColour (juce::ComboBox::outlineColourId));
        g.drawRoundedRectangle (boxBounds.toFloat().reduced (0.5f, 0.5f), cornerSize, 1.0f);
    }

    void positionComboBoxText (juce::ComboBox& box, juce::Label& label)
    {
        label.setBounds (1, 1,
                         box.getWidth(),
                         box.getHeight()*0.9f);

        label.setFont (getComboBoxFont (box));
        label.setJustificationType (juce::Justification::centred);
    }

    juce::Font getPopupMenuFont()
    {
        return withDefaultMetrics (clipperFont.withHeight(fontSize));
    }

    /*void getPopupMenuWindowSize (int numItems, int width, int height, int& newWidth, int& newHeight) override
    {
        newWidth = width;  // Set the popup menu width to match the ComboBox width
        newHeight = height; // Keep the height as per default or adjust if necessary
    }*/

    // Draw the popup menu when the ComboBox is clicked
    void drawPopupMenuBackground(juce::Graphics& g, int width, int height) override
    {
        // Set a rounded rectangle background for the menu
        //setColour(juce::PopupMenu::backgroundColourId, juce::Colour(0, 0, 0).withAlpha (0.5f));
        juce::Rectangle<float> backgroundRect(0, 0, (float)width, (float)height);
        auto cornerSize = 7.0f; // Radius for rounded edges
        g.setColour(findColour (juce::PopupMenu::backgroundColourId).withAlpha(1.f)); // Background color of the menu
        g.fillRoundedRectangle(backgroundRect, cornerSize);

    }

    void drawPopupMenuItem (juce::Graphics& g, const juce::Rectangle<int>& area,
                                        const bool isSeparator, const bool isActive,
                                        const bool isHighlighted, const bool isTicked,
                                        const bool hasSubMenu, const juce::String& text,
                                        const juce::String& shortcutKeyText,
                                        const juce::Drawable* icon, const juce::Colour* const textColourToUse)
    {
        if (isSeparator)
        {
            auto r  = area.reduced (5, 0);
            r.removeFromTop (juce::roundToInt (((float) r.getHeight() * 0.5f) - 0.5f));

            g.setColour (findColour (juce::PopupMenu::textColourId).withAlpha (0.3f));
            g.fillRect (r.removeFromTop (1));
        }
        else
        {
            auto textColour = (textColourToUse == nullptr ? findColour (juce::PopupMenu::textColourId)
                                                          : *textColourToUse);

            auto r  = area.reduced (1);

            if (isHighlighted && isActive)
            {
                g.setColour (findColour (juce::PopupMenu::highlightedBackgroundColourId));
                g.fillRoundedRectangle(r.toFloat(), 7.0f);

                g.setColour (findColour (juce::PopupMenu::highlightedTextColourId));
            }
            else
            {
                g.setColour (textColour.withMultipliedAlpha (isActive ? 1.0f : 0.5f));
            }

            r.reduce (juce::jmin (5, area.getWidth() / 20), 0);

            auto font = getPopupMenuFont();

            auto maxFontHeight = (float) r.getHeight() / 1.3f;

            if (font.getHeight() > maxFontHeight)
                font.setHeight (maxFontHeight);

            g.setFont (font);

            r.removeFromRight (3);
            g.drawFittedText (text, r, juce::Justification::centred, 1);

            if (shortcutKeyText.isNotEmpty())
            {
                auto f2 = font;
                f2.setHeight (f2.getHeight() * 0.75f);
                f2.setHorizontalScale (0.95f);
                g.setFont (f2);

                g.drawText (shortcutKeyText, r, juce::Justification::centredRight, true);
            }
        }
    }

};
