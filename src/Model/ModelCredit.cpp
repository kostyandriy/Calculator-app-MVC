#include "ModelCredit.h"

namespace s21 {

std::string ModelCredit::check(std::string sum, std::string time,
                               std::string precent, std::string type) {
  std::string res_out = "";
  if (valid_precent(precent) && valid_sum(sum) && valid_time(time)) {
    this->allow = true;
    this->sum_credit = std::stod(sum);
    this->percent = std::stod(precent) / 1200;
    this->time = std::stoi(time);
    this->type = type;
  } else {
    this->allow = false;
    res_out = "Incorrect input";
  }
  return res_out;
}

void ModelCredit::calculate() {
  if (this->allow) {
    sum = 0;
    diff = 0;

    if (type == "Annuitentnie") {
      per_month_ann();
    }
    if (type == "Differentials") {
      per_month_diff();
    }
  }
}

std::string ModelCredit::get_payment() {
  std::string res_out = "";
  if (allow) {
    if (type == "Annuitentnie") {
      res_out = std::to_string(ann_payment);
    }
    if (type == "Differentials") {
      res_out = std::to_string(diff_payment_first) + ".." +
                std::to_string(diff_payment_last);
    }
  }
  return res_out;
}

std::string ModelCredit::get_overpayment() {
  std::string res_out = "";
  if (allow) {
    res_out = std::to_string(diff);
  }
  return res_out;
}

std::string ModelCredit::get_sum_total() {
  std::string res_out = "";
  if (allow) {
    res_out = std::to_string(sum);
  }
  return res_out;
}

void ModelCredit::per_month_ann() {
  ann_payment = sum_credit * ((percent * pow(1 + percent, time)) /
                              (pow(1 + percent, time) - 1));
  sum = ann_payment * time;
  diff = sum - sum_credit;
}

void ModelCredit::per_month_diff() {
  double per_month = sum_credit / time;
  for (int i = 0; i < time; i++) {
    diff_payment_last = per_month + (sum_credit - per_month * i) * percent;
    if (i == 0) {
      diff_payment_first = diff_payment_last;
    }
    sum += diff_payment_last;
  }
  diff = sum - sum_credit;
}

bool ModelCredit::valid_time(std::string text) {
  bool res = false;
  unsigned int tmp = 0;
  int flag_empty = 0, flag_large = 0;
  if (text.length() == 0) {
    flag_empty = 1;
  }
  if (text.length() > 4) {
    flag_large = 1;
  }
  if (!flag_empty && !flag_large) {
    if (valid_int(text)) {
      tmp = std::stoul(text);
      if (tmp > 0 && tmp <= 1200) {
        res = true;
      }
    }
  }
  return res;
}

bool ModelCredit::valid_precent(std::string text) {
  bool res = 0;
  double tmp = 0;
  int flag_empty = 0, flag_large = 0;
  if (text.length() == 0) {
    flag_empty = 1;
  }
  if (text.length() > 100) {
    flag_large = 1;
  }
  if (!flag_empty && !flag_large) {
    if (valid_double(text)) {
      tmp = std::stod(text);
      if (tmp >= 0 && tmp <= 100) {
        res = true;
      }
    }
  }
  return res;
}

bool ModelCredit::valid_sum(std::string text) {
  bool res = 0;
  double tmp = 0;
  int flag_empty = 0, flag_large = 0;
  if (text.length() == 0) {
    flag_empty = 1;
  }
  if (text.length() > 15) {
    flag_large = 1;
  }
  if (!flag_empty && !flag_large) {
    if (valid_double(text)) {
      tmp = std::stod(text);
      if (tmp > 0 && tmp <= 100000000) {
        res = true;
      }
    }
  }
  return res;
}

bool ModelCredit::valid_int(std::string text) {
  bool res = false;
  bool flag_er = false;
  long long unsigned int len = text.length();
  for (long long unsigned int i = 0; i < len; i++) {
    if (text[i] < '0' || text[i] > '9') {
      flag_er = true;
    }
  }
  if (!flag_er) res = true;
  return res;
}

bool ModelCredit::valid_double(std::string text) {
  bool res = false;
  bool flag_er = false;
  int dot = 0;
  long long unsigned int len = text.length();
  for (long long unsigned int i = 0; i < len; i++) {
    if ((text[i] < '0' || text[i] > '9')) {
      if (text[i] == '.') {
        dot++;
      } else {
        flag_er = true;
      }
    }
  }
  if (!flag_er && dot <= 1) res = true;
  return res;
}

}  // namespace s21
