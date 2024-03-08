#include "credit.h"

#include "ui_credit.h"

Credit::Credit(QWidget *parent) : QWidget(parent), ui(new Ui::Credit) {
  ui->setupUi(this);
}

Credit::Credit(QWidget *parent, s21::ControllerCredit *c)
    : QWidget(parent), ui(new Ui::Credit), controller(c) {
  ui->setupUi(this);
}

Credit::~Credit() { delete ui; }

void Credit::on_pushButton_calculate_clicked() {
  ui->label_error->setText(QString::fromStdString(
      controller->check(ui->lineEdit_sum_val->text().toStdString(),
                        ui->lineEdit_date_val->text().toStdString(),
                        ui->lineEdit_procent_val->text().toStdString(),
                        ui->comboBox_type->currentText().toStdString())));
  controller->calculate();
  ui->label_payment_val->setText(
      QString::fromStdString(controller->get_payment()));
  ui->label_overpayment_val->setText(
      QString::fromStdString(controller->get_overpayment()));
  ui->label_payment_sum_val->setText(
      QString::fromStdString(controller->get_sum_total()));
}
