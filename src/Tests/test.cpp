#include <gtest/gtest.h>

#include "../Model/MainModel.h"
#include "../Model/ModelCredit.h"

TEST(Model_calculator, Test1) {
  s21::MainModel model;
  double res = 0;
  char input[] = "1 + 2 -   3";
  model.final_func(input, &res, 0);
  EXPECT_DOUBLE_EQ(1 + 2 - 3, res);
}

TEST(Model_calculator, Test2) {
  s21::MainModel model;
  double res = 0;
  char input[] = "1-2*12-3^2";
  model.final_func(input, &res, 0);
  EXPECT_DOUBLE_EQ(-32, res);
}

TEST(Model_calculator, Test3) {
  s21::MainModel model;
  double res = 0;
  char input[] = "sin(12)-3";
  model.final_func(input, &res, 0);
  EXPECT_DOUBLE_EQ(sin(12) - 3, res);
}

TEST(Model_calculator, Test4) {
  s21::MainModel model;
  double res = 0;
  char input[] = "ln(12)^3-1*(-12+5)";
  model.final_func(input, &res, 0);
  EXPECT_DOUBLE_EQ(pow(log(12), 3.0) - 1 * (-12 + 5), res);
}

TEST(Model_calculator, Test5) {
  s21::MainModel model;
  double res = 0;
  char input[] = "(cos((-5)))+ln((10/(5*7))^2)-(tan(sin(-3mod2))-5mod3*4/5/7)";
  model.final_func(input, &res, 0);
  EXPECT_NEAR(-0.874353, res, 7);
}

TEST(Model_calculator, Test6) {
  s21::MainModel model;
  double res = 0;
  char input[] = "cos(cos(12))-3-5";
  model.final_func(input, &res, 0);
  EXPECT_DOUBLE_EQ(cos(cos(12)) - 3 - 5, res);
}

TEST(Model_calculator, Test7) {
  s21::MainModel model;
  double res = 0;
  char input[] = "sin(3^3-ln(12))-12+(-11+20)";
  model.final_func(input, &res, 0);
  EXPECT_DOUBLE_EQ(sin(27 - log(12)) - 12 + (-11 + 20), res);
}

TEST(Model_calculator, Test8) {
  s21::MainModel model;
  double res = 0;
  char input[] = "sqrt(atan(12))";
  model.final_func(input, &res, 0);
  EXPECT_DOUBLE_EQ(sqrt(atan(12)), res);
}

TEST(Model_calculator, Test9) {
  s21::MainModel model;
  double res = 0;
  char input[] = "asin(1)";
  model.final_func(input, &res, 0);
  EXPECT_DOUBLE_EQ(asin(1), res);
}

TEST(Model_calculator, Test10) {
  s21::MainModel model;
  double res = 0;
  char input[] = "log(10)";
  model.final_func(input, &res, 0);
  EXPECT_DOUBLE_EQ(log10(10), res);
}

TEST(Model_calculator, Test11) {
  s21::MainModel model;
  double res = 0;
  char input[] = "sqrt(4)";
  model.final_func(input, &res, 0);
  EXPECT_DOUBLE_EQ(sqrt(4), res);
}

TEST(Model_calculator, Test12) {
  s21::MainModel model;
  double res = 0;
  char input[] = "ln(2)";
  model.final_func(input, &res, 0);
  EXPECT_DOUBLE_EQ(log(2), res);
}

TEST(Model_calculator, Test13) {
  s21::MainModel model;
  double res = 0;
  char input[] = "tan(2)";
  model.final_func(input, &res, 0);
  EXPECT_DOUBLE_EQ(tan(2), res);
}

TEST(Model_calculator, Test14) {
  s21::MainModel model;
  double res = 0;
  char input[] = "atan(2)";
  model.final_func(input, &res, 0);
  EXPECT_DOUBLE_EQ(atan(2), res);
}

TEST(Model_calculator, Test15) {
  s21::MainModel model;
  double res = 0;
  char input[] = "(()";
  model.final_func(input, &res, 0);
  EXPECT_EQ(-2, model.final_func(input, &res, 0));
}

TEST(Model_calculator, Test16) {
  s21::MainModel model;
  double res = 0;
  char input[] = "1/0";
  model.final_func(input, &res, 0);
  EXPECT_EQ(-1, model.final_func(input, &res, 0));
}

TEST(Model_credit, Test1) {
  s21::ModelCredit model;
  model.check("100000", "12", "13", "Annuitentnie");
  model.calculate();
  EXPECT_EQ(model.get_payment(), "8931.727571");
  EXPECT_EQ(model.get_overpayment(), "7180.730854");
  EXPECT_EQ(model.get_sum_total(), "107180.730854");
}

TEST(Model_credit, Test2) {
  s21::ModelCredit model;
  std::string res = model.check("100000", "12.", "13", "Annuitentnie");
  EXPECT_EQ(res, "Incorrect input");
}

TEST(Model_credit, Test3) {
  s21::ModelCredit model;
  model.check("100000", "12", "13", "Differentials");
  model.calculate();
  EXPECT_EQ(model.get_payment(), "9416.666667..8423.611111");
  EXPECT_EQ(model.get_overpayment(), "7041.666667");
  EXPECT_EQ(model.get_sum_total(), "107041.666667");
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
