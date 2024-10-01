//
// Created by Armin Peukert on 30.09.24.
//

#pragma once

#include <cmath>
#include <juce_dsp/juce_dsp.h>

enum ClipperMode {
  Tanh,
  Sinusoidal
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
        if (abs(input) > 2/3) {
          output = sin(input);
        } else {
          output = sin((sineCoefficientA * pi * input) / sineCoefficientB);
        }
        break;
    }

    return output;
  }

  void setMode (ClipperMode mode) {
    this->mode = mode;
  }

  void setTanhCoefficient (double coefficient) {
    tanhCoefficient = coefficient;
  }

  void setSineCoefficients (double sineA, double sineB) {
    sineCoefficientA = sineA;
    sineCoefficientB = sineB;
  }

private:
  double pi = acos(-1);

  double tanhCoefficient { 5.0 };
  double sineCoefficientA { 3.0 };
  double sineCoefficientB { 4.0 };
  ClipperMode mode {ClipperMode::Sinusoidal};

};