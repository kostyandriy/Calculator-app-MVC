#include "ControllerCalculator.h"

namespace s21 {
QString ControllerCalculator::calculate(QString text) {
  return model->calculate_value(text);
}
QString ControllerCalculator::set_x(QString text, QString previous_x) {
  return model->set_x(text, previous_x);
}

}  // namespace s21
