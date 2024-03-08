//#pragma once
#ifndef CPP3_SMARTCALC_SRC_CONTROLLER_CONTROLLERCALCULATOR_H
#define CPP3_SMARTCALC_SRC_CONTROLLER_CONTROLLERCALCULATOR_H

#include "../Model/ModelCalculator.h"

namespace s21 {
class ControllerCalculator {
 public:
  ControllerCalculator(s21::ModelCalculator *m) : model(m) {}
  QString calculate(QString text);
  QString set_x(QString text, QString previous_x);

 private:
  ModelCalculator *model;
};
}  // namespace s21

#endif  // CPP3_SMARTCALC_SRC_CONTROLLER_CONTROLLERCALCULATOR_H
