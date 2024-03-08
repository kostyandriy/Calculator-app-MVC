#include "mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent, s21::ControllerCalculator *c_ca,
                       s21::ControllerCredit *c_cr, s21::ControllerGraph *c_gr)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      controller_calc(c_ca),
      controller_credit(c_cr),
      controller_graph(c_gr) {
  ui->setupUi(this);

  credit = new Credit(nullptr, controller_credit);

  graph = new Graph(nullptr, controller_graph);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_pushButton_1_clicked() {
  QString symbolToAdd = "1";
  ui->lineEdit_expression->setText(ui->lineEdit_expression->text() +
                                   symbolToAdd);
}

void MainWindow::on_pushButton_2_clicked() {
  QString symbolToAdd = "2";
  ui->lineEdit_expression->setText(ui->lineEdit_expression->text() +
                                   symbolToAdd);
}

void MainWindow::on_pushButton_3_clicked() {
  QString symbolToAdd = "3";
  ui->lineEdit_expression->setText(ui->lineEdit_expression->text() +
                                   symbolToAdd);
}

void MainWindow::on_pushButton_4_clicked() {
  QString symbolToAdd = "4";
  ui->lineEdit_expression->setText(ui->lineEdit_expression->text() +
                                   symbolToAdd);
}

void MainWindow::on_pushButton_5_clicked() {
  QString symbolToAdd = "5";
  ui->lineEdit_expression->setText(ui->lineEdit_expression->text() +
                                   symbolToAdd);
}

void MainWindow::on_pushButton_6_clicked() {
  QString symbolToAdd = "6";
  ui->lineEdit_expression->setText(ui->lineEdit_expression->text() +
                                   symbolToAdd);
}

void MainWindow::on_pushButton_7_clicked() {
  QString symbolToAdd = "7";
  ui->lineEdit_expression->setText(ui->lineEdit_expression->text() +
                                   symbolToAdd);
}

void MainWindow::on_pushButton_8_clicked() {
  QString symbolToAdd = "8";
  ui->lineEdit_expression->setText(ui->lineEdit_expression->text() +
                                   symbolToAdd);
}

void MainWindow::on_pushButton_9_clicked() {
  QString symbolToAdd = "9";
  ui->lineEdit_expression->setText(ui->lineEdit_expression->text() +
                                   symbolToAdd);
}

void MainWindow::on_pushButton_0_clicked() {
  QString symbolToAdd = "0";
  ui->lineEdit_expression->setText(ui->lineEdit_expression->text() +
                                   symbolToAdd);
}

void MainWindow::on_pushButton_bracket_open_clicked() {
  QString symbolToAdd = "(";
  ui->lineEdit_expression->setText(ui->lineEdit_expression->text() +
                                   symbolToAdd);
}

void MainWindow::on_pushButton_bracket_close_clicked() {
  QString symbolToAdd = ")";
  ui->lineEdit_expression->setText(ui->lineEdit_expression->text() +
                                   symbolToAdd);
}

void MainWindow::on_pushButton_multiply_clicked() {
  QString symbolToAdd = "*";
  ui->lineEdit_expression->setText(ui->lineEdit_expression->text() +
                                   symbolToAdd);
}

void MainWindow::on_pushButton_divide_clicked() {
  QString symbolToAdd = "/";
  ui->lineEdit_expression->setText(ui->lineEdit_expression->text() +
                                   symbolToAdd);
}

void MainWindow::on_pushButton_minus_clicked() {
  QString symbolToAdd = "-";
  ui->lineEdit_expression->setText(ui->lineEdit_expression->text() +
                                   symbolToAdd);
}

void MainWindow::on_pushButton_plus_clicked() {
  QString symbolToAdd = "+";
  ui->lineEdit_expression->setText(ui->lineEdit_expression->text() +
                                   symbolToAdd);
}

void MainWindow::on_pushButton_x_clicked() {
  QString symbolToAdd = "x";
  ui->lineEdit_expression->setText(ui->lineEdit_expression->text() +
                                   symbolToAdd);
}

void MainWindow::on_pushButton_power_clicked() {
  QString symbolToAdd = "^";
  ui->lineEdit_expression->setText(ui->lineEdit_expression->text() +
                                   symbolToAdd);
}

void MainWindow::on_pushButton_sin_clicked() {
  QString textToAdd = "sin";
  ui->lineEdit_expression->setText(ui->lineEdit_expression->text() + textToAdd);
}

void MainWindow::on_pushButton_cos_clicked() {
  QString textToAdd = "cos";
  ui->lineEdit_expression->setText(ui->lineEdit_expression->text() + textToAdd);
}

void MainWindow::on_pushButton_tan_clicked() {
  QString textToAdd = "tan";
  ui->lineEdit_expression->setText(ui->lineEdit_expression->text() + textToAdd);
}

void MainWindow::on_pushButton_asin_clicked() {
  QString textToAdd = "asin";
  ui->lineEdit_expression->setText(ui->lineEdit_expression->text() + textToAdd);
}

void MainWindow::on_pushButton_acos_clicked() {
  QString textToAdd = "acos";
  ui->lineEdit_expression->setText(ui->lineEdit_expression->text() + textToAdd);
}

void MainWindow::on_pushButton_atan_clicked() {
  QString textToAdd = "atan";
  ui->lineEdit_expression->setText(ui->lineEdit_expression->text() + textToAdd);
}

void MainWindow::on_pushButton_ln_clicked() {
  QString textToAdd = "ln";
  ui->lineEdit_expression->setText(ui->lineEdit_expression->text() + textToAdd);
}

void MainWindow::on_pushButton_log_clicked() {
  QString textToAdd = "log";
  ui->lineEdit_expression->setText(ui->lineEdit_expression->text() + textToAdd);
}

void MainWindow::on_pushButton_sqrt_clicked() {
  QString textToAdd = "sqrt";
  ui->lineEdit_expression->setText(ui->lineEdit_expression->text() + textToAdd);
}

void MainWindow::on_pushButton_mod_clicked() {
  QString textToAdd = "mod";
  ui->lineEdit_expression->setText(ui->lineEdit_expression->text() + textToAdd);
}

void MainWindow::on_pushButton_dot_clicked() {
  QString textToAdd = ".";
  ui->lineEdit_expression->setText(ui->lineEdit_expression->text() + textToAdd);
}

void MainWindow::on_pushButton_delete_clicked() {
  ui->lineEdit_expression->backspace();
}

void MainWindow::on_pushButton_delete_all_clicked() {
  ui->lineEdit_expression->clear();
}

void MainWindow::on_pushButton_calculate_clicked() {
  ui->label_result->setText(
      controller_calc->calculate(ui->lineEdit_expression->text()));
}

void MainWindow::on_pushButton_x_set_clicked() {
  ui->label_x_val->setText(controller_calc->set_x(
      ui->lineEdit_x_expression->text(), ui->label_x_val->text()));
}

void MainWindow::on_pushButton_graph_clicked() { graph->show(); }

void MainWindow::on_pushButton_credit_clicked() { credit->show(); }
