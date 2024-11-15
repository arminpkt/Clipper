//
// Created by Armin Peukert on 30.09.24.
//

#pragma once

#include <cmath>
#include <juce_dsp/juce_dsp.h>

enum ClipperMode {
  Tanh,
  Sinusoidal,
  Exponential
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