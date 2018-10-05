/*
Задача A-2. Задача о системах счисления
Time limit:    14 s
Memory limit:    64 M
Составить программу для перевода чисел из системы счисления по основанию P в
систему счисления по основанию Q, где 2 меньше или равно Q меньше P меньше или
равно 36.
Значения P, Q и строковое представление S числа в исходной системе счисления
вводятся с клавиатуры в следующем порядке:
P Q S
S не выходит за границы size_t.
Процедура перевода не может использовать соответствующей функции системной
библиотеки и должна быть оформлена в виде отдельной функции, на вход которой
подаются значения P, Q и S, размещенные в динамической памяти.
На выход функция должна возвращать строковое представление S1 числа в целевой
системе счисления.
runId 332
*/

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"
#include "assert.h"
#include "stdbool.h"

#define STRING_SIZE 50
#define DEC_BASE 10
#define LOW_BORDER_BASE 2
#define HIGH_BORDER_BASE 36

#define OK 0
#define ERROR -1

int get_dec_digit_from_symbol(char symbol);

char get_symbol_from_digit(const int number);

void reverse_string(char *string);

bool correct_input(const int base, const char *string);

size_t converter_to_dec(const int base, const char *string);

char *converter(const int base_from, const int base_to, char *string);

int main(void) {
    int base_from = 0;
    int base_to = 0;
    char *string = (char *)calloc(STRING_SIZE, sizeof(char));
    if (!string) {
        printf("[error]");
        return 0;
    }
    int res_code = OK;
    char *ans = NULL;
    if (scanf("%d %d %s", &base_from, &base_to, string) == 3) {  // from p to q
        ans = converter(base_from, base_to, string);
        if (!ans) {
            res_code = ERROR;
        }
    } else {
        res_code = ERROR;
    }

    if (res_code == OK) {
        printf("%s", ans);
    } else {
        printf("[error]");
    }

    free(string);
    free(ans);
    return 0;
}

int get_dec_digit_from_symbol(char symbol) {
    if (symbol <= '9' && symbol >= '0') {
        return symbol - '0';
    }
    if (symbol >= 'a') {  // Перевод маленьких букв в большие
        symbol -= ('a' - 'A');
    }
    return (symbol - 'A' + DEC_BASE);
}

char get_symbol_from_digit(const int number) {
    if ((number >= DEC_BASE) || (number < 0)) {
        return ' ';
    }

    char symbol = ' ';
    if (number < DEC_BASE) {
        symbol = '0' + number;
    } else {
        symbol = 'A' + (number - DEC_BASE);
    }
    return symbol;
}

void reverse_string(char *string) {
    if (!string) {
        return;
    }

    size_t len = strlen(string);
    size_t mid = len / 2;
    size_t i = 0;
    char tmp = ' ';
    while (i < mid) {
        tmp = string[i];
        string[i] = string[len - 1 - i];
        string[len - 1 - i] = tmp;
        i++;
    }
}

bool correct_input(const int base, const char *string) {
    if ((!string) || (base < 0)) {
        return false;
    }

    size_t len = strlen(string);
    bool result = true;
    for (size_t i = 0; i < len; i++) {
        if (get_dec_digit_from_symbol(string[i]) >= base ||
                get_dec_digit_from_symbol(string[i]) < 0) {
            result = false;
            break;
        }
    }
    return result;
}

size_t converter_to_dec(const int base, const char *string) {
    assert(base >= LOW_BORDER_BASE);

    if ((!string) || (base < LOW_BORDER_BASE)) {
        return ERROR;
    }

    size_t len_str = strlen(string);
    size_t answer = 0;
    for (size_t i = 0; i < len_str; i++) {
        answer += get_dec_digit_from_symbol(string[len_str - 1 - i]) * pow(base, i);
    }
    return answer;
}

char *converter(const int base_from, const int base_to, char *string) {
    if (base_to < LOW_BORDER_BASE || base_to >= base_from ||
            base_from > HIGH_BORDER_BASE || !correct_input(base_from, string)) {
        return NULL;
    }

    size_t dec_ss = converter_to_dec(base_from, string);

    char *ans = (char *)calloc(STRING_SIZE, sizeof(char));
    if (!ans) {
        return NULL;
    }

    size_t i = 0;
    int result = OK;
    do {
        ans[i] = get_symbol_from_digit(dec_ss % base_to);
        if (ans[i++] == ' ') {
            result = ERROR;
            break;
        }
        dec_ss = dec_ss / base_to;
    } while (dec_ss != 0);

    if (result == ERROR) {
        free(ans);
        return NULL;
    }

    ans[i] = '\0';

    reverse_string(ans);

    return ans;
}