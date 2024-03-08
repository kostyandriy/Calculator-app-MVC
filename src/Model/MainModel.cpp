#include "MainModel.h"

namespace s21 {
//----------------------------calculate
int MainModel::calculate(Stack **ready, double *result) {
  int res = 0;
  int flag_error_math = 0;
  Stack *number = NULL;
  while (*ready && !flag_error_math) {
    calculate_1(ready, &number, &flag_error_math);
    calculate_2(ready, &number, &flag_error_math);
    calculate_3(ready, &number, &flag_error_math);
    calculate_4(ready, &number, &flag_error_math);
  }
  if (number) {
    *result = number->value;
    pop_node(&number);
  } else
    flag_error_math = 1;
  if (!flag_error_math) res = 1;

  return res;
}

void MainModel::calculate_1(Stack **ready, Stack **number,
                            int *flag_error_math) {
  double tmp_1 = 0;
  double tmp_2 = 0;
  double tmp_res = 0;
  if (peek_node(*ready) == 1 && !(*flag_error_math)) {
    push_node(number, (*ready)->value, (*ready)->priority, (*ready)->type);
    pop_node(ready);
  }
  if (peek_node(*ready) == 2 && !(*flag_error_math)) {
    push_node(number, (*ready)->value, (*ready)->priority, (*ready)->type);
    pop_node(ready);
  }
  if (peek_node(*ready) == 5 && !(*flag_error_math)) {
    tmp_2 = (*number)->value;
    pop_node(number);
    tmp_1 = (*number)->value;
    pop_node(number);
    tmp_res = tmp_1 + tmp_2;
    push_node(number, tmp_res, get_priority(Number), Number);
    pop_node(ready);
  }
  if (peek_node(*ready) == 6 && !(*flag_error_math)) {
    tmp_2 = (*number)->value;
    pop_node(number);
    tmp_1 = (*number)->value;
    pop_node(number);
    tmp_res = tmp_1 - tmp_2;
    push_node(number, tmp_res, get_priority(Number), Number);
    pop_node(ready);
  }
  if (peek_node(*ready) == 7 && !(*flag_error_math)) {
    tmp_2 = (*number)->value;
    pop_node(number);
    tmp_1 = (*number)->value;
    pop_node(number);
    tmp_res = tmp_2 * tmp_1;
    push_node(number, tmp_res, get_priority(Number), Number);
    pop_node(ready);
  }
}

void MainModel::calculate_2(Stack **ready, Stack **number,
                            int *flag_error_math) {
  double tmp_1 = 0;
  double tmp_2 = 0;
  double tmp_res = 0;
  if (peek_node(*ready) == 8 && !(*flag_error_math)) {
    tmp_2 = (*number)->value;
    pop_node(number);
    if (tmp_2 != 0) {
      tmp_1 = (*number)->value;
      pop_node(number);
      tmp_res = tmp_1 / tmp_2;
      push_node(number, tmp_res, get_priority(Number), Number);
    } else
      *flag_error_math = 1;
    pop_node(ready);
  }
  if (peek_node(*ready) == 9 && !(*flag_error_math)) {
    if (number) {
      tmp_2 = (*number)->value;
      pop_node(number);
      if (tmp_2 != 0 && number) {
        tmp_1 = (*number)->value;
        pop_node(number);
        tmp_res = fmod(tmp_1, tmp_2);
        push_node(number, tmp_res, get_priority(Number), Number);
      } else
        *flag_error_math = 1;
    } else
      *flag_error_math = 1;
    pop_node(ready);
  }
  if (peek_node(*ready) == 10 && !(*flag_error_math)) {
    tmp_2 = (*number)->value;
    pop_node(number);
    tmp_1 = (*number)->value;
    pop_node(number);
    tmp_res = pow(tmp_1, tmp_2);
    push_node(number, tmp_res, get_priority(Number), Number);
    pop_node(ready);
  }
  if (peek_node(*ready) == 11 && !(*flag_error_math)) {
    tmp_1 = (*number)->value;
    pop_node(number);
    tmp_res = sin(tmp_1);
    push_node(number, tmp_res, get_priority(Number), Number);
    pop_node(ready);
  }
}

void MainModel::calculate_3(Stack **ready, Stack **number,
                            int *flag_error_math) {
  double tmp_1 = 0;
  double tmp_res = 0;
  if (peek_node(*ready) == 12 && !(*flag_error_math)) {
    tmp_1 = (*number)->value;
    pop_node(number);
    tmp_res = cos(tmp_1);
    push_node(number, tmp_res, get_priority(Number), Number);
    pop_node(ready);
  }
  if (peek_node(*ready) == 13 && !(*flag_error_math)) {
    tmp_1 = (*number)->value;
    pop_node(number);
    tmp_res = tan(tmp_1);
    push_node(number, tmp_res, get_priority(Number), Number);
    pop_node(ready);
  }
  if (peek_node(*ready) == 14 && !(*flag_error_math)) {
    tmp_1 = (*number)->value;
    if (tmp_1 >= -1 && tmp_1 <= 1) {
      pop_node(number);
      tmp_res = asin(tmp_1);
      push_node(number, tmp_res, get_priority(Number), Number);
    } else
      *flag_error_math = 1;
    pop_node(ready);
  }
  if (peek_node(*ready) == 15 && !(*flag_error_math)) {
    tmp_1 = (*number)->value;
    if (tmp_1 >= -1 && tmp_1 <= 1) {
      pop_node(number);
      tmp_res = acos(tmp_1);
      push_node(number, tmp_res, get_priority(Number), Number);
    } else
      *flag_error_math = 1;
    pop_node(ready);
  }
  if (peek_node(*ready) == 16 && !(*flag_error_math)) {
    tmp_1 = (*number)->value;
    if (fmod(tmp_1, M_PI / 2) > 1e-8 || fmod(tmp_1, M_PI / 2) < -1e-8) {
      pop_node(number);
      tmp_res = atan(tmp_1);
      push_node(number, tmp_res, get_priority(Number), Number);
    } else
      *flag_error_math = 1;
    pop_node(ready);
  }
}

void MainModel::calculate_4(Stack **ready, Stack **number,
                            int *flag_error_math) {
  double tmp_1 = 0;
  double tmp_res = 0;
  if (peek_node(*ready) == 17 && !(*flag_error_math)) {
    tmp_1 = (*number)->value;
    if (tmp_1 >= 0) {
      pop_node(number);
      tmp_res = sqrt(tmp_1);
      push_node(number, tmp_res, get_priority(Number), Number);
    } else
      *flag_error_math = 1;
    pop_node(ready);
  }
  if (peek_node(*ready) == 18 && !(*flag_error_math)) {
    tmp_1 = (*number)->value;
    if (tmp_1 > 0) {
      pop_node(number);
      tmp_res = log(tmp_1);
      push_node(number, tmp_res, get_priority(Number), Number);
    } else
      *flag_error_math = 1;
    pop_node(ready);
  }
  if (peek_node(*ready) == 19 && !(*flag_error_math)) {
    tmp_1 = (*number)->value;
    if (tmp_1 > 0) {
      pop_node(number);
      tmp_res = log10(tmp_1);
      push_node(number, tmp_res, get_priority(Number), Number);
    } else
      *flag_error_math = 1;
    pop_node(ready);
  }
}

int MainModel::final_func(char *input, double *calculated, double x) {
  int result = 0;
  char res[MAX_SIZE_STRING] = "";
  trim_input(input, res);
  double tmp = 0;
  if (valid_input(res)) {
    Stack *inverse_orig = NULL;
    Stack *orig = NULL;
    Stack *inverse_ready = NULL;
    Stack *support = NULL;
    Stack *ready = NULL;
    stack_from_str(&inverse_orig, res, x);
    inverse_stack(&inverse_orig, &orig);
    notation_stack(&orig, &inverse_ready, &support);
    inverse_stack(&inverse_ready, &ready);
    if (calculate(&ready, &tmp)) {
      *calculated = tmp;
      result = 1;
    } else {
      result = -1;
    }
    remove_node(&ready);
  } else {
    result = -2;
  }
  return result;
}

//-------------------------notation
void MainModel::notation_stack(Stack **origin, Stack **result,
                               Stack **support) {
  int k = 0;
  while (*origin) {
    first_part_notation(origin, result, support, &k);
    second_part_notation(origin, result, support, &k);
  }
  // ---------------ЗАКИДЫВАЕМ В READY ВСЕ ИЗ САППОРТА---------------------
  while (peek_node(*support) != 0) {
    push_node(result, (*support)->value, (*support)->priority,
              (*support)->type);
    pop_node(support);
  }
}

void MainModel::first_part_notation(Stack **origin, Stack **result,
                                    Stack **support, int *k) {
  // -------------ЧИСЛО------------------------
  if (peek_node(*origin) == 1) {
    push_node(result, (*origin)->value, (*origin)->priority, (*origin)->type);
    pop_node(origin);
    *k += 1;
  }
  // ---------------ИКС---------------------
  if (peek_node(*origin) == 2) {
    push_node(result, (*origin)->value, (*origin)->priority, (*origin)->type);
    pop_node(origin);
    *k += 1;
  }
  // // ---------------ЛЕВАЯ---------------------
  if (peek_node(*origin) == 3) {
    if ((*origin)->next->type == 5 || (*origin)->next->type == 6) {
      push_node(result, 0, get_priority(Number), MainModel::my_type(1));
    }
    push_node(support, (*origin)->value, (*origin)->priority, (*origin)->type);
    pop_node(origin);
    *k += 1;
  }
  // // ---------------ПРАВАЯ---------------------
  if (peek_node(*origin) == 4) {
    while (peek_node(*support) != 3) {
      push_node(result, (*support)->value, (*support)->priority,
                (*support)->type);
      pop_node(support);
    }
    if (peek_node(*support) == 3) pop_node(support);
    pop_node(origin);
    *k += 1;
  }
  // // ---------------ПЛЮС---------------------
  if (peek_node(*origin) == 5) {
    if (*k == 0) {
      push_node(result, 0, get_priority(Number), MainModel::my_type(1));
    }
    while (peek_node(*support) != 0 && (*support)->priority >= 1) {
      push_node(result, (*support)->value, (*support)->priority,
                (*support)->type);
      pop_node(support);
    }
    push_node(support, (*origin)->value, (*origin)->priority, (*origin)->type);
    pop_node(origin);
    *k += 1;
  }
}

void MainModel::second_part_notation(Stack **origin, Stack **result,
                                     Stack **support, int *k) {
  // // ---------------МИНУС---------------------
  if (peek_node(*origin) == 6) {
    if (*k == 0) {
      push_node(result, 0, get_priority(Number), MainModel::my_type(1));
    }
    while (peek_node(*support) != 0 && (*support)->priority >= 1) {
      push_node(result, (*support)->value, (*support)->priority,
                (*support)->type);
      pop_node(support);
    }
    push_node(support, (*origin)->value, (*origin)->priority, (*origin)->type);
    pop_node(origin);
    *k += 1;
  }
  // // ---------------УМНОЖИТЬ И ДЕЛИТЬ И MOD---------------------
  if (peek_node(*origin) >= 7 && peek_node(*origin) <= 9) {
    while (peek_node(*support) != 0 &&
           (*support)->priority >= get_priority(MainModel::my_type(7))) {
      push_node(result, (*support)->value, (*support)->priority,
                (*support)->type);
      pop_node(support);
    }
    push_node(support, (*origin)->value, (*origin)->priority, (*origin)->type);
    pop_node(origin);
    *k += 1;
  }
  // ---------------СТЕПЕНЬ---------------------
  if (peek_node(*origin) == 10) {
    while (peek_node(*support) != 0 &&
           (*support)->priority > get_priority(op_power)) {
      push_node(result, (*support)->value, (*support)->priority,
                (*support)->type);
      pop_node(support);
    }
    push_node(support, (*origin)->value, (*origin)->priority, (*origin)->type);
    pop_node(origin);
    *k += 1;
  }
  // ---------------ФУНКЦИИ---------------------
  if (peek_node(*origin) >= 11 && peek_node(*origin) <= 19) {
    while (peek_node(*support) != 0 &&
           (*support)->priority >= get_priority(MainModel::my_type(11))) {
      push_node(result, (*support)->value, (*support)->priority,
                (*support)->type);
      pop_node(support);
    }
    push_node(support, (*origin)->value, (*origin)->priority, (*origin)->type);
    pop_node(origin);
    *k += 1;
  }
}

//-----------------------parse
void MainModel::stack_from_str(Stack **node, char *input, double x) {
  for (size_t i = 0; input[i] != '\0'; i++) {
    simple_symbols(node, input[i], x);
    complex_symbols(node, input, &i);
    number_symbols(node, input, &i);
  }
}

int MainModel::get_priority(my_type type) {
  int res = -2;
  if (type >= 1 && type <= 2) res = 0;
  if (type >= 3 && type <= 4) res = -1;
  if (type >= 5 && type <= 6) res = 1;
  if (type >= 7 && type <= 9) res = 2;
  if (type == 10) res = 3;
  if (type >= 11 && type <= 19) res = 4;
  return res;
}

int MainModel::get_type_simple(char symbol) {
  int res = 0;
  if (is_x(symbol)) res = 2;
  if (is_bracket(symbol) == 1) res = 3;
  if (is_bracket(symbol) == 2) res = 4;
  if (symbol == '+') res = 5;
  if (symbol == '-') res = 6;
  if (symbol == '*') res = 7;
  if (symbol == '/') res = 8;
  if (symbol == '^') res = 10;
  return res;
}

int MainModel::get_type_complex(char *input, size_t *i) {
  int res = 0;
  int func = funcs(input, i, 1);
  if (is_operator(input, i, 1) == 1) res = 9;
  if (func == 1) res = 11;
  if (func == 2) res = 12;
  if (func == 3) res = 13;
  if (func == 4) res = 14;
  if (func == 5) res = 15;
  if (func == 6) res = 16;
  if (func == 7) res = 17;
  if (func == 8) res = 18;
  if (func == 9) res = 19;
  return res;
}

void MainModel::simple_symbols(Stack **node, char symbol, double x) {
  int type = get_type_simple(symbol);
  if (type != 0 && type != 2)
    push_node(node, 0, get_priority(MainModel::my_type(type)),
              MainModel::my_type(type));
  if (type == 2)
    push_node(node, x, get_priority(MainModel::my_type(type)),
              MainModel::my_type(type));
}

void MainModel::complex_symbols(Stack **node, char *input, size_t *i) {
  int type = get_type_complex(input, i);
  if (type != 0)
    push_node(node, 0, get_priority(MainModel::my_type(type)),
              MainModel::my_type(type));
}

void MainModel::number_symbols(Stack **node, char *input, size_t *i) {
  double res = 0;
  if (is_number(input[*i]) || is_dot(input, *i)) {
    res = atof(input + *i);
    while (input[*i] != '\0' && (is_number(input[*i]) || is_dot(input, *i))) {
      *i += 1;
    }
    *i -= 1;
    push_node(node, res, 0, Number);
  }
}

void MainModel::inverse_stack(Stack **input, Stack **result) {
  while (*input) {
    push_node(result, (*input)->value, (*input)->priority, (*input)->type);
    pop_node(input);
  }
}

//--------------stack
void MainModel::push_node(Stack **head, double value, int priority,
                          MainModel::my_type type) {
  Stack *tmp = (Stack *)malloc(sizeof(Stack));
  if (tmp != NULL) {
    tmp->value = value;
    tmp->priority = priority;
    tmp->type = type;
    tmp->next = *head;
    *head = tmp;
  }
}

void MainModel::pop_node(Stack **head) {
  if (*head != NULL) {
    Stack *out = *head;
    *head = (*head)->next;
    free(out);
  }
}

int MainModel::peek_node(Stack *head) {
  int res = 0;
  if (head != NULL) {
    res = head->type;
  }
  return res;
}

void MainModel::remove_node(Stack **head) {
  while (*head) {
    Stack *tmp = *head;
    *head = (*head)->next;
    free(tmp);
  }
}

//--------------valid
//#include "Calc.h"

// -------------------------------пробелы----------------------------------
void MainModel::trim_input(char *input, char *result) {
  int count = 0;
  int len = strlen(input);
  for (int i = 0, j = 0; i < len; i++) {
    if (input[i] != ' ') {
      result[j] = input[i];
      j++;
    } else {
      count++;
    }
  }
  if (count > 0)
    result[strlen(input) - count] = '\0';
  else
    result[strlen(input)] = '\0';
}

//-------------------------------Проверка x----------------------------
int MainModel::valid_x(char *input) {
  int res = 0;
  int len = strlen(input);
  int flag_not_valid_symb = 0;

  for (int i = 0; i < len && !flag_not_valid_symb; i++) {
    if (i == 0 && input[i] == '-') i++;
    if (!is_number(input[i]) && !is_dot(input, i)) flag_not_valid_symb = 1;
  }

  if (!flag_not_valid_symb && valid_number(input)) res = 1;
  return res;
}

//----------------------------------Логика скобок-----------------------------
int MainModel::brackets_par(char *input) {
  int res = 0;
  int flag_er = 0;
  int flag_nonvalid = 0;
  int len = strlen(input);
  char temp[MAX_SIZE_STRING] = "";
  for (int i = 0; i < len && !flag_er; i++) {
    if (is_bracket(input[i]) == 1) {
      temp[strlen(temp)] = '(';
      if (!brackets_valid_in(input, i)) flag_nonvalid = 1;
    }
    if (is_bracket(input[i]) == 2) {
      if (temp[strlen(temp) - 1] == '(') {
        temp[strlen(temp) - 1] = '\0';
      } else {
        flag_er = 1;
      }
    }
  }
  if (!flag_er && strlen(temp) == 0 && !flag_nonvalid) {
    res = 1;
  }
  return res;
}

int MainModel::brackets_valid_in(char *input, size_t i) {
  int res = 0;
  i += 1;
  if (input[i] != '\0' &&
      (input[i] == '(' || input[i] == '+' || input[i] == '-' ||
       is_number(input[i]) || funcs(input, &i, 0) || input[i] == 'x' ||
       is_dot(input, i)))
    res = 1;
  return res;
}

int MainModel::is_bracket(char symbol) {
  int res = 0;
  if (symbol == '(') res = 1;
  if (symbol == ')') res = 2;
  return res;
}

// -------------------------------операторы----------------------------------
int MainModel::is_operator(char *input, size_t *i, int offset) {
  int res = 0;
  int exist_mod = 0;
  int exist_operator = 0;
  if (input[*i] == 'm' && strlen(input) - *i - 1 >= 2) {
    if (input[*i + 1] == 'o' && input[*i + 2] == 'd') {
      exist_mod = 1;
      *i += 3 - offset;
    }
  }
  if ((input[*i] == '+') || (input[*i] == '-') || (input[*i] == '*') ||
      (input[*i] == '/') || (input[*i] == '^')) {
    exist_operator = 1;
    *i += 1 - offset;
  }
  if (exist_mod) res = 1;
  if (exist_operator) res = 2;
  return res;
}

int MainModel::valid_after_operator(char *input) {
  int res = 0;
  int flag_er = 0;
  size_t len = strlen(input);
  for (size_t i = 0; i < len && !flag_er; i++) {
    if (is_operator(input, &i, 0) == 2) {
      if (!(input[i] != '\0' &&
            (is_number(input[i]) || input[i] == '(' || funcs(input, &i, 0) ||
             input[i] == 'x' || is_dot(input, i)))) {
        flag_er = 1;
      }
    }
  }
  if (!flag_er) res = 1;
  return res;
}

int MainModel::valid_after_mod(char *input) {
  int res = 0;
  int flag_er = 0;
  for (size_t i = 0; i < strlen(input); i++) {
    if (is_operator(input, &i, 0) == 1) {
      if (!(input[i] != '\0' && input[i] != '-' &&
            (is_number(input[i]) || input[i] == '(' || funcs(input, &i, 0) ||
             input[i] == 'x' || is_dot(input, i))))
        flag_er = 1;
    }
  }
  if (!flag_er) res = 1;
  return res;
}

int MainModel::valid_mul(char *input) {
  int res = 0;
  size_t len = (strlen(input));
  int flag_simple = 0;
  int flag_complex = 0;
  for (size_t i = 0; i < len && !flag_simple && !flag_complex; i++) {
    if (!valid_simple_mul(input, i)) flag_simple = 1;

    if (!valid_complex_mul(input, &i)) flag_complex = 1;
  }
  if (!flag_simple && !flag_complex) res = 1;

  return res;
}

int MainModel::valid_simple_mul(char *input, size_t i) {
  int res = 0;
  int flag_er = 0;
  if (is_x(input[i])) {
    if (i != 0) {
      if (is_number(input[i - 1]) || is_dot(input, i - 1) ||
          is_bracket(input[i - 1] == 2))
        flag_er = 1;
    }
    if (is_number(input[i + 1]) || is_dot(input, i + 1) ||
        is_bracket(input[i + 1]) == 1)
      flag_er = 1;
  }
  if (i != 0 && is_bracket(input[i]) == 1 &&
      (is_number(input[i - 1]) || is_dot(input, i - 1)))
    flag_er = 1;
  if (is_bracket(input[i]) == 2 &&
      (is_number(input[i + 1]) || is_dot(input, i + 1)))
    flag_er = 1;

  if (!flag_er) res = 1;
  return res;
}

int MainModel::valid_complex_mul(char *input, size_t *i) {
  int res = 0;
  int flag_er = 0;
  size_t k = *i;
  if (funcs(input, i, 1)) {
    if (k != 0) {
      if (is_number(input[k - 1]) || is_dot(input, k - 1) ||
          is_bracket(input[k - 1] == 2) || is_x(input[k - 1]))
        flag_er = 1;
    }
  }
  if (!flag_er) res = 1;

  return res;
}

int MainModel::valid_start(char *input) {
  int res = 0;
  int flag_er = 0;
  size_t i = 0;
  if (input[i] == '^' || input[i] == '*' || input[i] == '/') flag_er = 1;
  if (is_operator(input, &i, 0) == 1) flag_er = 1;
  if (!flag_er) res = 1;
  return res;
}

//------------------------------------числа------------------------------------
int MainModel::is_number(char symbol) {
  return (symbol >= '0' && symbol <= '9');
}

int MainModel::valid_number(char *input) {
  int res = 0;
  int len = strlen(input);
  int flag_dot = 0;
  for (int i = 0; i < len && !flag_dot; i++) {
    int amount_dot = 0;
    while (input[i] != '\0' && (is_number(input[i]) || is_dot(input, i))) {
      if (is_dot(input, i)) amount_dot++;
      if (amount_dot > 1) flag_dot = 1;
      i++;
    }
  }
  if (!flag_dot) res = 1;
  return res;
}

//--------------------------------точка----------------------------------
int MainModel::is_dot(char *input, int i) {
  int res = 0;
  if (input[i] == '.' && i != 0) {
    if (is_number(input[i - 1]) || is_number(input[i + 1])) res = 1;
  }
  return res;
}

//----------------------------------x----------------------------
int MainModel::is_x(char symbol) { return (symbol == 'x'); }

//----------------------------функции-------------------------------------
int MainModel::trigonometry(char *input, size_t *i, int offset) {
  int res = 0;
  if (input[*i] == 's' && strlen(input) - *i - 1 >= 2) {
    if (input[*i + 1] == 'i' && input[*i + 2] == 'n') {
      res = 1;
      *i += 3 - offset;
    }
  }
  if (input[*i] == 'c' && strlen(input) - *i - 1 >= 2) {
    if (input[*i + 1] == 'o' && input[*i + 2] == 's') {
      res = 2;
      *i += 3 - offset;
    }
  }
  if (input[*i] == 't' && strlen(input) - *i - 1 >= 2) {
    if (input[*i + 1] == 'a' && input[*i + 2] == 'n') {
      res = 3;
      *i += 3 - offset;
    }
  }
  return res;
}

int MainModel::funcs(char *input, size_t *i, int offset) {
  int res = 0;
  int tmp = 0;
  if (input[*i] == 'l') {
    if (strlen(input) - *i - 1 >= 1) {
      if (input[*i + 1] == 'n') {
        res = 8;
        *i += 2 - offset;
      }
    }
    if (strlen(input) - *i - 1 >= 2) {
      if (input[*i + 1] == 'o' && input[*i + 2] == 'g') {
        res = 9;
        *i += 3 - offset;
      }
    }
  }
  if (input[*i] == 's' && strlen(input) - *i - 1 >= 3) {
    if (input[*i + 1] == 'q' && input[*i + 2] == 'r' && input[*i + 3] == 't') {
      res = 7;
      *i += 4 - offset;
    }
  }
  tmp = trigonometry(input, i, offset);
  if (input[*i] == 'a' && strlen(input) - *i - 1 >= 3) {
    *i += 1;
    tmp = trigonometry(input, i, offset) + 3;
  }
  if (tmp != 0) res = tmp;
  return res;
}

//----------------------------Скобка после функции---------------------------
int MainModel::bracket_after_func(char *input) {
  int res = 0;
  int flag_non_bracket = 0;
  size_t len = strlen(input);
  for (size_t i = 0; i < len; i++) {
    if (funcs(input, &i, 0) != 0) {
      if (input[i] != '(') {
        flag_non_bracket = 1;
      }
    }
  }
  if (!flag_non_bracket) {
    res = 1;
  }
  return res;
}

//-----------------------------Общая проверка------------------------------
int MainModel::valid_input(char *input) {
  int res = 0;
  int flag_symb = 0;
  int flag_brackets = 0;
  int flag_operators = 0;
  int flag_brackets_after_func = 0;
  int flag_valid_number = 0;
  int flag_valid_mul = 0;
  int flag_valid_start = 0;
  int flag_after_mod = 0;
  size_t len = strlen(input);

  for (size_t i = 0;
       i < len && !flag_symb && !(input[i] == '\n' || input[i] == '\0'); i++) {
    if (!(is_number(input[i]) || funcs(input, &i, 1) != 0 || is_dot(input, i) ||
          is_x(input[i]) || is_operator(input, &i, 1) != 0 ||
          is_bracket(input[i]))) {
      flag_symb = 1;
    }
  }

  if (!brackets_par(input)) flag_brackets = 1;

  if (!valid_after_operator(input)) flag_operators = 1;

  if (!bracket_after_func(input)) flag_brackets_after_func = 1;

  if (!valid_number(input)) flag_valid_number = 1;

  if (!valid_mul(input)) flag_valid_mul = 1;

  if (!valid_start(input)) flag_valid_start = 1;

  if (!valid_after_mod(input)) flag_after_mod = 1;

  if (!flag_symb && !flag_brackets && !flag_operators &&
      !flag_brackets_after_func && !flag_valid_number && !flag_valid_mul &&
      !flag_valid_start && !flag_after_mod)
    res = 1;

  return res;
}

}  // namespace s21
