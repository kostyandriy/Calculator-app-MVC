#include "ControllerGraph.h"

namespace s21 {
QString ControllerGraph::check(QString text) { return model->check(text); }

QString ControllerGraph::get_axis(QString previous, QString min_x,
                                  QString max_x, QString min_y, QString max_y) {
  return model->get_axis(previous, min_x, max_x, min_y, max_y);
}

void ControllerGraph::calculate(QString text) { model->calculate_graph(text); }

int ControllerGraph::get_min_x() { return model->get_min_x(); }

int ControllerGraph::get_max_x() { return model->get_max_x(); }

int ControllerGraph::get_min_y() { return model->get_min_y(); }

int ControllerGraph::get_max_y() { return model->get_max_y(); }

QVector<double> ControllerGraph::get_x_cords() { return model->x; }

QVector<double> ControllerGraph::get_y_cords() { return model->y; }
}  // namespace s21
