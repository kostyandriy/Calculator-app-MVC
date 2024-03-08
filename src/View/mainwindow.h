//#pragma once
#ifndef CPP3_SMARTCALC_SRC_VIEW_MAINWINDOW_H
#define CPP3_SMARTCALC_SRC_VIEW_MAINWINDOW_H

#include <QMainWindow>

#include "../Controller/ControllerCalculator.h"
#include "../Controller/ControllerCredit.h"
#include "../Model/ModelCredit.h"
#include "credit.h"
#include "graph.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  MainWindow(QWidget *parent = nullptr,
             s21::ControllerCalculator *c_c = nullptr,
             s21::ControllerCredit *c_cr = nullptr,
             s21::ControllerGraph *c_gr = nullptr);

  ~MainWindow();

 private slots:
  void on_pushButton_1_clicked();

  void on_pushButton_2_clicked();

  void on_pushButton_sin_clicked();

  void on_pushButton_asin_clicked();

  void on_pushButton_mod_clicked();

  void on_pushButton_bracket_open_clicked();

  void on_pushButton_bracket_close_clicked();

  void on_pushButton_delete_clicked();

  void on_pushButton_cos_clicked();

  void on_pushButton_acos_clicked();

  void on_pushButton_7_clicked();

  void on_pushButton_8_clicked();

  void on_pushButton_9_clicked();

  void on_pushButton_multiply_clicked();

  void on_pushButton_delete_all_clicked();

  void on_pushButton_tan_clicked();

  void on_pushButton_atan_clicked();

  void on_pushButton_4_clicked();

  void on_pushButton_5_clicked();

  void on_pushButton_6_clicked();

  void on_pushButton_divide_clicked();

  void on_pushButton_ln_clicked();

  void on_pushButton_log_clicked();

  void on_pushButton_3_clicked();

  void on_pushButton_minus_clicked();

  void on_pushButton_sqrt_clicked();

  void on_pushButton_power_clicked();

  void on_pushButton_0_clicked();

  void on_pushButton_dot_clicked();

  void on_pushButton_calculate_clicked();

  void on_pushButton_plus_clicked();

  void on_pushButton_x_clicked();

  void on_pushButton_x_set_clicked();

  void on_pushButton_graph_clicked();

  void on_pushButton_credit_clicked();

 private:
  Ui::MainWindow *ui;

  s21::ControllerCalculator *controller_calc;
  s21::ControllerCredit *controller_credit;
  s21::ControllerGraph *controller_graph;

  Graph *graph;
  Credit *credit;
};
#endif  // MAINWINDOW_H
