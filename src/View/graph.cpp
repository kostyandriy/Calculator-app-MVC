#include "graph.h"

#include "mainwindow.h"
#include "ui_graph.h"

Graph::Graph(QWidget *parent) : QWidget(parent), ui(new Ui::Graph) {
  ui->setupUi(this);
}

Graph::Graph(QWidget *parent, s21::ControllerGraph *c)
    : QWidget(parent), ui(new Ui::Graph), controller(c) {
  ui->setupUi(this);
}

Graph::~Graph() { delete ui; }

void Graph::on_pushButton_graph_clicked() {
  ui->label_error->setText(
      controller->check(ui->lineEdit_func_expression->text()));

  ui->label_error->setText(controller->get_axis(
      ui->label_error->text(), ui->lineEdit_min_x_val->text(),
      ui->lineEdit_max_x_val->text(), ui->lineEdit_min_y_val->text(),
      ui->lineEdit_max_y_val->text()));

  controller->calculate(ui->lineEdit_func_expression->text());

  ui->widget->xAxis->setRange(controller->get_min_x(), controller->get_max_x());
  ui->widget->yAxis->setRange(controller->get_min_y(), controller->get_max_y());
  ui->widget->clearGraphs();
  ui->widget->addGraph();
  ui->widget->graph(0)->addData(controller->get_x_cords(),
                                controller->get_y_cords());
  ui->widget->replot();
}
