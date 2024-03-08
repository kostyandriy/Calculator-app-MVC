#ifndef CPP3_SMARTCALC_SRC_MODEL_MODELCREDIT_H
#define CPP3_SMARTCALC_SRC_MODEL_MODELCREDIT_H
#include <iostream>

#include "MainModel.h"

namespace s21 {
class ModelCredit {
 public:
  std::string check(std::string sum, std::string time, std::string precent,
                    std::string type);
  void calculate();

  std::string get_payment();
  std::string get_overpayment();
  std::string get_sum_total();

 private:
  int allow = 0;

  double sum_credit = 0;
  double percent = 0;
  int time = 0;
  std::string type = "";

  double ann_payment = 0;
  double diff_payment_first = 0;
  double diff_payment_last = 0;
  double sum = 0;
  double diff = 0;

  bool valid_sum(std::string text);
  bool valid_time(std::string text);
  bool valid_precent(std::string text);

  bool valid_int(std::string text);
  bool valid_double(std::string text);

  void per_month_ann();
  void per_month_diff();
};
}  // namespace s21

#endif  // CPP3_SMARTCALC_SRC_MODEL_MODELCREDIT_H
