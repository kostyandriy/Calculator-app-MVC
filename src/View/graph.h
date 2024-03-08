#ifndef CPP3_SMARTCALC_SRC_VIEW_GRAPH_H
#define CPP3_SMARTCALC_SRC_VIEW_GRAPH_H

#include <QVector>
#include <QWidget>

#include "../Controller/ControllerGraph.h"
#include "../Model/MainModel.h"
#include "qcustomplot.h"

namespace Ui {
class Graph;
}

class Graph : public QWidget {
  Q_OBJECT

 public:
  explicit Graph(QWidget *parent = nullptr);
  explicit Graph(QWidget *parent = nullptr, s21::ControllerGraph *c = nullptr);
  ~Graph();

 private slots:
  void on_pushButton_graph_clicked();

 private:
  Ui::Graph *ui;
  s21::ControllerGraph *controller;
};

#endif  // GRAPH_H
