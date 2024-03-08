#include "ModelGraph.h"

namespace s21 {

QString ModelGraph::check(QString text) {
  QString res_out = "";
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
    char res[MAX_SIZE_STRING] = "";
    this->trim_input(input, res);
    if (this->valid_input(res)) {
      this->allow = true;
    } else {
      res_out = "Incorrect input";
    }
  }
  if (flag_empty) {
    res_out = "Empty input";
  }
  if (flag_large) {
    res_out = "Too large input";
  }
  return res_out;
}

QString ModelGraph::get_axis(QString previous, QString x_min_text,
                             QString x_max_text, QString y_min_text,
                             QString y_max_text) {
  QString res_out = previous;
  bool flag_valid_cord = false;
  if (valid_string(x_min_text) && valid_string(x_max_text) &&
      valid_string(y_min_text) && valid_string(y_max_text)) {
    if (valid_cord(x_min_text.toInt(), x_max_text.toInt()) &&
        valid_cord(y_min_text.toInt(), y_max_text.toInt()))
      flag_valid_cord = true;
  }
  if (!flag_valid_cord) {
    res_out = "Invalid cords";
    this->allow = false;
  } else {
    this->min_x = x_min_text.toInt();
    this->max_x = x_max_text.toInt();
    this->min_y = y_min_text.toInt();
    this->max_y = y_max_text.toInt();
  }
  return res_out;
}

bool ModelGraph::valid_string(QString input) {
  bool res = false;
  int flag_empty = 0;
  int flag_large = 0;

  if (input.length() == 0) {
    flag_empty = 1;
  }
  if (input.length() > 9) {
    flag_large = 1;
  }

  if (!flag_empty && !flag_large) {
    if (valid_int(input) == 1) {
      res = true;
    }
  }

  return res;
}

bool ModelGraph::valid_cord(int min, int max) {
  bool res = false;
  if (min < max)
    if (max >= -1000000 && max <= 1000000)
      if (min >= -1000000 && min <= 1000000) res = true;
  return res;
}

bool ModelGraph::valid_int(QString text) {
  bool res = false;
  text.toInt(&res);
  return res;
}

void ModelGraph::calculate_graph(QString text) {
  if (allow) {
    if (max_x - min_x >= 1) h = 0.01;

    if (max_x - min_x >= 20) h = 0.1;

    if (max_x - min_x >= 200) h = 1;

    if (max_x - min_x >= 10000) h = 2;

    if (max_x - min_x >= 100000) h = 4;

    if (max_x - min_x >= 200000) h = 8;

    x.clear();
    y.clear();
    double X = 0, Y = 0;
    for (X = min_x; X < max_x; X += h) {
      char *input_str = text.toUtf8().data();
      int flag_ok = final_func(input_str, &Y, X);
      if (flag_ok == 1) {
        x.push_back(X);
        y.push_back(Y);
      }
    }
  }
}

int ModelGraph::get_min_x() { return min_x; }

int ModelGraph::get_max_x() { return max_x; }

int ModelGraph::get_min_y() { return min_y; }

int ModelGraph::get_max_y() { return max_y; }

}  // namespace s21
