#ifndef CPP3_SMARTCALC_SRC_CONTROLLER_CONTROLLERCREDIT_H
#define CPP3_SMARTCALC_SRC_CONTROLLER_CONTROLLERCREDIT_H
#include "../Model/ModelCredit.h"

namespace s21 {
class ControllerCredit {
 public:
  ControllerCredit(s21::ModelCredit *m) : model(m) {}

  std::string check(std::string sum, std::string date, std::string precent,
                    std::string type);
  void calculate();
  std::string get_payment();
  std::string get_overpayment();
  std::string get_sum_total();

 private:
  ModelCredit *model;
};
}  // namespace s21
#endif  // CPP3_SMARTCALC_SRC_CONTROLLER_CONTROLLERCREDIT_H
