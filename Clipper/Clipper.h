//
// Created by Armin Peukert on 30.09.24.
//

#pragma once

#include <cmath>
#include <juce_dsp/juce_dsp.h>

enum ClipperMode {
  Sinusoidal,
  Tanh
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
        break;
    }

    return output;
  }

  void setMode (ClipperMode mode) {
    this->mode = mode;
  }

  void setThreshold (double thresh) {
    threshold = thresh;
  }

  void setTanhCoefficient (double coefficient) {
    tanhCoefficient = coefficient;
  }

private:
  double pi = acos(-1);

  ClipperMode mode {ClipperMode::Sinusoidal};
  double threshold { 0.66 };
  double tanhCoefficient { 5.0 };

  // signum function
  double sgn (double x) {
    return (0 < x) - (x < 0);
  }

};