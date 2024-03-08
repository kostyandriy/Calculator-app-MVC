#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  s21::ModelCalculator model_calc;
  s21::ControllerCalculator controller_calc(&model_calc);

  s21::ModelCredit model_credit;
  s21::ControllerCredit controller_credit(&model_credit);

  s21::ModelGraph model_graph;
  s21::ControllerGraph controller_graph(&model_graph);

  MainWindow w(nullptr, &controller_calc, &controller_credit,
               &controller_graph);
  w.show();
  return a.exec();
}
