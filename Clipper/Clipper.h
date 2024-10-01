//
// Created by Armin Peukert on 30.09.24.
//

#pragma once

#include <juce_dsp/juce_dsp.h>

struct Clipper {
  Clipper() {}

  double process (double input) {
    return juce::dsp::FastMathApproximations::tanh (tanhCoefficient * input);
  }

  void setTanhCoefficient (double coefficient) {
    tanhCoefficient = coefficient;
  }

private:
  double tanhCoefficient { 5.0 };

};