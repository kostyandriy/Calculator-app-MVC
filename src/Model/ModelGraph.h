#ifndef CPP3_SMARTCALC_SRC_MODEL_MODELGRAPH_H
#define CPP3_SMARTCALC_SRC_MODEL_MODELGRAPH_H
#include <QString>
#include <QVector>

#include "MainModel.h"

namespace s21 {
class ModelGraph : public MainModel {
 public:
  QString check(QString text);
  QString get_axis(QString previous, QString min_x, QString max_x,
                   QString min_y, QString max_y);
  void calculate_graph(QString text);
  QVector<double> x, y;

  int get_min_x();
  int get_max_x();
  int get_min_y();
  int get_max_y();

 private:
  bool allow = false;

  int min_x = -10;
  int max_x = 10;
  int min_y = -10;
  int max_y = 10;
  double h = 0;

  bool valid_int(QString text);
  bool valid_string(QString input);
  bool valid_cord(int min, int max);
};
}  // namespace s21
#endif  // CPP3_SMARTCALC_SRC_MODEL_MODELGRAPH_H
