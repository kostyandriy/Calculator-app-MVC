#include "ModelCalculator.h"

namespace s21 {
QString ModelCalculator::calculate_value(QString text) {
  QString result_out;
  int flag_empty = 0;
  int flag_large = 0;

  if (text.length() == 0) {
    flag_empty = 1;
  }
  if (text.length() > MAX_SIZE_STRING) {
    flag_large = 1;
  }

  if (!flag_empty && !flag_large) {
    char *input = text.toUtf8().data();
    double result = 0;
    int flag = final_func(input, &result, this->x);
    if (flag == 1) {
      result_out = QString::number(result, 'f', 8);
    }
    if (flag == -1) {
      result_out = "Error in calculation";
    }
    if (flag == -2) {
      result_out = "Error in input";
    }
  }
  if (flag_empty) {
    result_out = "Empty input";
  }
  if (flag_large) {
    result_out = "Too large input";
  }
  return result_out;
}

QString ModelCalculator::set_x(QString x_text, QString previous_x) {
  QString result_out = previous_x;
  int flag_x_empty = 0;
  int flag_x_large = 0;

  if (x_text.length() == 0) {
    flag_x_empty = 1;
  }
  if (x_text.length() > MAX_SIZE_STRING) {
    flag_x_large = 1;
  }

  if (!flag_x_empty && !flag_x_large) {
    char *x_input = x_text.toUtf8().data();
    if (valid_x(x_input) == 1) {
      result_out = x_text;
      this->x = x_text.toDouble();
    }
  }
  return result_out;
}

}  // namespace s21
