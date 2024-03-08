#include "ControllerCredit.h"

namespace s21 {

std::string ControllerCredit::check(std::string sum, std::string date,
                                    std::string precent, std::string type) {
  return model->check(sum, date, precent, type);
}

void ControllerCredit::calculate() { model->calculate(); }

std::string ControllerCredit::get_payment() { return model->get_payment(); }

std::string ControllerCredit::get_overpayment() {
  return model->get_overpayment();
}

std::string ControllerCredit::get_sum_total() { return model->get_sum_total(); }

}  // namespace s21