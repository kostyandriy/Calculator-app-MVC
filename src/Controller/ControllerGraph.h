#ifndef CPP3_SMARTCALC_SRC_CONTROLLER_CONTROLLERGRAPH_H
#define CPP3_SMARTCALC_SRC_CONTROLLER_CONTROLLERGRAPH_H
#include <QString>
#include <QVector>

#include "../Model/ModelGraph.h"

namespace s21 {
class ControllerGraph {
 public:
  ControllerGraph(s21::ModelGraph *m) : model(m) {}

  QString check(QString text);
  QString get_axis(QString previous, QString min_x, QString max_x,
                   QString min_y, QString max_y);
  void calculate(QString text);

  int get_min_x();
  int get_max_x();
  int get_min_y();
  int get_max_y();

  QVector<double> get_x_cords();
  QVector<double> get_y_cords();

 private:
  ModelGraph *model;
};
}  // namespace s21
#endif  // CPP3_SMARTCALC_SRC_CONTROLLER_CONTROLLERGRAPH_H
