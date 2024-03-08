#ifndef CPP3_SMARTCALC_SRC_VIEW_CREDIT_H
#define CPP3_SMARTCALC_SRC_VIEW_CREDIT_H

#include <QMainWindow>

#include "../Controller/ControllerCredit.h"
#include "QWidget"

namespace Ui {
class Credit;
}

class Credit : public QWidget {
  Q_OBJECT

 public:
  explicit Credit(QWidget *parent = nullptr);
  explicit Credit(QWidget *parent = nullptr,
                  s21::ControllerCredit *c = nullptr);
  ~Credit();

 private slots:
  void on_pushButton_calculate_clicked();

 private:
  Ui::Credit *ui;
  s21::ControllerCredit *controller;
};

#endif  // CPP3_SMARTCALC_SRC_VIEW_CREDIT_H
