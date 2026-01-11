//
// Created by Armin Peukert on 30.09.24.
//

#pragma once

#include <cmath>
#include <juce_dsp/juce_dsp.h>

enum ClipperMode {
  Tanh,
  Sinusoidal,
  Exponential,
  SoftClipPro
};

struct Clipper {
  Clipper() {}

  void prepare (double sampleRate) {
    constexpr double rampLength = 0.05;
    threshold.reset(sampleRate, rampLength);
    tanhCoefficient.reset(sampleRate, rampLength);
    exponent.reset(sampleRate, rampLength);
    gain.reset(sampleRate, rampLength);
  }

  double process (double input) {
    double output = input;
    double thresh = threshold.getNextValue();
    double nextGain = gain.getNextValue();
    double sig = sgn(input);
    switch (mode) {
      case Tanh:
        // old: output = juce::dsp::FastMathApproximations::tanh (tanhCoefficient.getNextValue() * input) / juce::dsp::FastMathApproximations::tanh (tanhCoefficient.getNextValue());
        if (abs(input) > thresh) {
          output = sig * (thresh + juce::dsp::FastMathApproximations::tanh (tanhCoefficient.getNextValue() * (abs(input) - thresh)));
        }
        output *= nextGain;
        break;

      case Sinusoidal:
        /*if (abs(input) > thresh) {
          output = sgn(input);
        } else {
          output = sin(((1/(2 * thresh)) * pi * input));
        }
        output *= thresh * nextGain;*/
        if (abs(input) > thresh)
          output = sig * (thresh + 0.6 * sin(0.5 * pi * abs(input - sig * thresh)));
        output *= thresh * nextGain;
        break;

      case Exponential:
        /*if (abs(input) > thresh) {
          output = sgn(input);
        } else {
          output = sgn(input) * (1 - pow(abs((1/thresh) * input - sgn(input)), exponent.getNextValue()));
        }
        output *= thresh * nextGain;*/

        if (abs(input) > thresh)
          output = sig * (thresh + (1 - pow(abs(0.1 * (input - sig * thresh) - sig ), exponent.getNextValue())));
        output *= nextGain;
        break;

case SoftClipPro:
{
  // StandardCLIP "Soft Clip Pro" behaviour (manual):
  // - Dynamic integrity below threshold
  // - 2:1 compression above threshold (in dB domain)
  // - Fixed 25% amount -> compress last 6 dB into 3 dB, then add +3 dB
  // This implementation uses the *existing* threshold parameter as the saturation start point.
  //
  // NOTE: If threshold is set to -6 dB (≈ 0.501187), this matches the 25% start line described in the manual.

  const double v = std::abs(input);

  if (v > thresh)
  {
    // Core 2:1 above threshold: |y| = sqrt(thresh * |x|)
    const double core = std::sqrt(thresh * v);

    // Fixed +3 dB makeup gain, but faded in smoothly to avoid a kink at the threshold.
    const double makeup = std::pow(10.0, 3.0 / 20.0); // +3 dB ≈ 1.4125

    // smoothstep weight from threshold..1
    const double denom = juce::jmax(1e-12, 1.0 - thresh);
    double u = (v - thresh) / denom;
    u = juce::jlimit(0.0, 1.0, u);
    const double w = u * u * (3.0 - 2.0 * u);

    const double g = 1.0 + w * (makeup - 1.0);

    output = sig * core * g;

    // Hard safety clamp (StandardCLIP will never exceed full-scale here for the 25% default threshold).
    output = juce::jlimit(-1.0, 1.0, output);
  }

  output *= nextGain;
  break;
}
    }

    return output;
  }

  void setMode (ClipperMode m) {
    this->mode = m;
  }

  void setThreshold (double thresh) {
    threshold.setTargetValue(thresh);
  }

  void setTanhCoefficient (double coefficient) {
    tanhCoefficient.setTargetValue(coefficient);
  }

  void setExponent (double exp) {
    exponent.setTargetValue(exp);
  }

  void setGain (double g) {
    gain.setTargetValue(g);
  }

private:
  double pi = acos(-1);

  ClipperMode mode {ClipperMode::Sinusoidal};
  juce::SmoothedValue<double> threshold { 0.66 };
  juce::SmoothedValue<double> tanhCoefficient { 1.0 };
  juce::SmoothedValue<double> exponent { 2.0 };
  juce::SmoothedValue<double> gain { 1.0 };

  // signum function
  double sgn (double x) {
    return (0 < x) - (x < 0);
  }

};