#ifndef CPP3_SMARTCALC_SRC_MODEL_MODELCALCULATOR_H
#define CPP3_SMARTCALC_SRC_MODEL_MODELCALCULATOR_H
#pragma once

#define MAX_SIZE_STRING 256
#include <QString>

#include "MainModel.h"

namespace s21 {

class ModelCalculator : public MainModel {
 public:
  QString calculate_value(QString text);
  QString set_x(QString x_text, QString previous_x);

 private:
  double x = 0;
};
}  // namespace s21
#endif  // CPP3_SMARTCALC_SRC_MODEL_MODELCALCULATOR_H
