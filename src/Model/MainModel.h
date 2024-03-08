// #pragma once
#ifndef CPP3_SMARTCALC_SRC_MODEL_MAINMODEL_H
#define CPP3_SMARTCALC_SRC_MODEL_MAINMODEL_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_SIZE_STRING 256

namespace s21 {
class MainModel {
 public:
  typedef enum type_t {
    Number = 1,
    var_x = 2,
    left_bracket = 3,
    right_bracket = 4,
    op_plus = 5,
    op_minus = 6,
    op_mul = 7,
    op_div = 8,
    op_mod = 9,
    op_power = 10,
    f_sin = 11,
    f_cos = 12,
    f_tan = 13,
    f_asin = 14,
    f_acos = 15,
    f_atan = 16,
    f_sqrt = 17,
    f_ln = 18,
    f_log = 19
  } my_type;

  typedef struct Stack {
    double value;
    int priority;
    my_type type;
    struct Stack *next;
  } Stack;

  int valid_input(char *input);
  void trim_input(char *input, char *result);
  int valid_x(char *input);
  int brackets_par(char *input);
  int brackets_valid_in(char *input, size_t i);
  int is_bracket(char symbol);
  int bracket_after_func(char *input);

  int is_operator(char *input, size_t *i, int offset);
  int valid_after_operator(char *input);
  int valid_after_mod(char *input);
  int valid_mul(char *input);
  int valid_simple_mul(char *input, size_t i);
  int valid_complex_mul(char *input, size_t *i);
  int valid_start(char *input);

  int funcs(char *input, size_t *i, int offset);
  int trigonometry(char *input, size_t *i, int offset);

  int is_dot(char *input, int i);
  int is_x(char symbol);
  int is_number(char symbol);
  int valid_number(char *input);

  void push_node(Stack **head, double value, int priority, my_type type);
  void pop_node(Stack **head);
  int peek_node(Stack *head);
  void remove_node(Stack **head);

  int get_priority(my_type type);
  int get_type_simple(char symbol);
  int get_type_complex(char *input, size_t *i);

  void stack_from_str(Stack **node, char *input, double x);
  void simple_symbols(Stack **node, char symbol, double x);
  void complex_symbols(Stack **node, char *input, size_t *i);
  void number_symbols(Stack **node, char *input, size_t *i);
  void inverse_stack(Stack **input, Stack **result);

  void first_part_notation(Stack **origin, Stack **result, Stack **support,
                           int *k);
  void second_part_notation(Stack **origin, Stack **result, Stack **support,
                            int *k);
  void notation_stack(Stack **origin, Stack **result, Stack **support);

  int calculate(Stack **ready, double *result);
  void calculate_1(Stack **ready, Stack **number, int *flag_error_math);
  void calculate_2(Stack **ready, Stack **number, int *flag_error_math);
  void calculate_3(Stack **ready, Stack **number, int *flag_error_math);
  void calculate_4(Stack **ready, Stack **number, int *flag_error_math);
  int final_func(char *input, double *calculated, double x);
};

}  // namespace s21

#endif  // CPP3_SMARTCALC_SRC_MODEL_MAINMODEL_H
