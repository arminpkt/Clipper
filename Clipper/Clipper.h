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
  //TSQ, //two stage quadratic
  //Cubic
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

    switch (mode) {
      case Tanh:
        output = juce::dsp::FastMathApproximations::tanh (tanhCoefficient.getNextValue() * input);
        break;

      case Sinusoidal:
        if (abs(input) > threshold.getNextValue()) {
          output = sgn(input);
        } else {
          output = sin(((1/(2 * threshold.getNextValue())) * pi * input));
        }
        output *= threshold.getNextValue() * gain.getNextValue();
        break;

      case Exponential:
        if (abs(input) > threshold.getNextValue()) {
          output = sgn(input);
        } else {
          output = sgn(input) * (1 - pow(abs((1/threshold.getNextValue()) * input - sgn(input)), exponent.getNextValue()));
        }
        output *= threshold.getNextValue() * gain.getNextValue();
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
  juce::SmoothedValue<double> tanhCoefficient { 5.0 };
  juce::SmoothedValue<double> exponent { 2.0 };
  juce::SmoothedValue<double> gain { 1.0 };

  // signum function
  static double sgn (double x) {
    return (0 < x) - (x < 0);
  }

};