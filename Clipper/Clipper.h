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

  double process (double input) {
    double output = input;

    switch (mode) {
      case Tanh:
        output = juce::dsp::FastMathApproximations::tanh (tanhCoefficient * input);
        break;

      case Sinusoidal:
        if (abs(input) > threshold) {
          output = sgn(input);
        } else {
          output = sin(((1/(2 * threshold)) * pi * input));
        }
        output *= threshold * gain;
        break;

      case Exponential:
        if (abs(input) > threshold) {
          output = sgn(input);
        } else {
          output = sgn(input) * (1 - pow(abs((1/threshold) * input - sgn(input)), exponent));
        }
        output *= threshold * gain;
        break;
    }

    return output;
  }

  void setMode (ClipperMode m) {
    this->mode = m;
  }

  void setThreshold (double thresh) {
    threshold = thresh;
  }

  void setTanhCoefficient (double coefficient) {
    tanhCoefficient = coefficient;
  }

  void setExponent (double exp) {
    exponent = exp;
  }

  void setGain (double g) {
    gain = g;
  }

private:
  double pi = acos(-1);

  ClipperMode mode {ClipperMode::Sinusoidal};
  double threshold { 0.66 };
  double tanhCoefficient { 5.0 };
  double exponent { 2.0 };
  double gain { 1.0 };

  // signum function
  double sgn (double x) {
    return (0 < x) - (x < 0);
  }

};