/*
Задача B-5. Парсер, логические выражения.
Time limit:	14 s
Memory limit:	64 M
Требуется написать программу, которая способна вычислять логическе выражения.
Допустимые выражения чувствительны к регистру и могут содержать:
1) знаки операций 'and' - и, 'or' - или, 'not' - отрицание, 'xor' - сложение по модулю 2
2) Скобки '(', ')'
3) Логические значения 'True', 'False'
4) Пустое выражение имеет значение "False"

Также требуется реализовать возможность задания переменных, которые могут состоять только из маленьких букв латинского алфавита (к примеру, 'x=True').
Объявление переменной имеет формат:
<имя>=<True|False>; // допустимы пробелы

Допускается несколько объявлений одной и той же переменной, учитывается последнее.
Все переменные, используемые в выражении, должны быть заданы заранее. Также запрещены имена, совпадающие с ключевыми словами (and, or и т.д.).

Необходимо учитывать приоритеты операций. Пробелы ничего не значат.

Если выражение или объявление не удовлетворяют описанным правилам, требуется вывести в стандартный поток вывода "[error]" (без кавычек).

run Id 1758
*/
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <assert.h>

#define SIZE_ARR_STRINGS 100
#define SIZE_DICTIONARY 100
#define SIZE_STRING 100
#define TOKEN_SIZE 20
#define NUMBER_SIZE 20
#define NUMBER_OPERATION 6

#define XOR "xor"
#define OR "or"
#define AND "and"
#define NOT "not"

typedef struct tree_node_t {
    char *token;
    struct tree_node_t *left;
    struct tree_node_t *right;

} tree_node;

typedef struct dict_s {
    char *name;
    char *val;
} dict;

char *read_token(char **input);
tree_node *read_simple_expression(char **input);
int get_priority(const char * binary_op);
tree_node *read_expression(int min_priority, char **input);
int resolve_tree(const tree_node *root);
bool check_name(const char *name);
int read_var(char *strings, dict *dictionary, int *k);
void free_tree(tree_node * node);
bool check_str(char *str);
void read_arr_strings(int *count_arr, char **arr_strings);


int main(void)
{
    setbuf(stdout,NULL);
    char **arr_strings = (char **)calloc(SIZE_ARR_STRINGS,sizeof(char*));
    if (!arr_strings) {
        printf("[error]");
        return 0;
    }
    dict *dictionary = (dict *)calloc(SIZE_DICTIONARY,sizeof(dict));
    if (!dictionary) {
        printf("[error]");
        free(arr_strings);
        return 0;
    }
    tree_node * root = NULL;
    char *main_str_copy = NULL;

    int count_arr = -1;

    // чтение
    read_arr_strings(&count_arr, arr_strings);

    if (count_arr == 0) {
        printf("[error]");
        free(dictionary);
        free(arr_strings);
        return 0;
    }

    int err = 0;
    int ind_dictionary = 0;
    // занесение в словарь
    for(int i = 0; i < count_arr - 1; i++) {
        err = read_var(arr_strings[i], dictionary, &ind_dictionary);
        if (err == -1)
            break;
    }
    if (err == -1) {
        printf("[error]");
    }
    else
    {
        char *input_string = arr_strings[count_arr - 1];
        char *main_str = (char* )malloc(SIZE_STRING * sizeof(char));
        if (!main_str) {
            printf("[error]");
            free(dictionary);
            for(int i = 0; i <= count_arr; i++) {
                free(arr_strings[i]);
            }
            free(arr_strings);
            return 0;
        }
        main_str_copy = main_str;
        int w = 0;
        // замена переменных
        while(strlen(input_string) != 0) {
            for(int i = 0; i < ind_dictionary; i++) {
                if (strncmp(input_string, dictionary[i].name, strlen(dictionary[i].name)) == 0) {
                    input_string += strlen(dictionary[i].name);
                    memcpy(main_str + w, dictionary[i].val, strlen(dictionary[i].val));
                    w+=strlen(dictionary[i].val);
                    break;
                }
            }
            main_str[w++] = *input_string++;
        }
        main_str[w] = 0;

        if (!check_str(main_str)) {
            printf("[error]");
        }
        else
        {
            root = read_expression(0, &main_str);
            if (root) {
                int res = resolve_tree(root);
                switch (res) {
                case 1:
                    printf("True");
                    break;
                case 0:
                    printf("False");
                    break;
                default :
                    printf("[error]");
                    break;
                }
            } else
                printf("[error]");
        }
    }

    //
    for(int i = 0; i <= count_arr; i++) {
        free(arr_strings[i]);
    }
    free(arr_strings);
    // очистка словаря
    for(int i = 0; i < ind_dictionary; i++) {
        free(dictionary[i].name);
        free(dictionary[i].val);
    }
    free(dictionary);
    free_tree(root);
    free(main_str_copy);

    return 0;
}


void read_arr_strings(int* count_arr, char** arr_strings)
{
    assert (arr_strings);
    if (!arr_strings) {
        *count_arr = 0;
        return;
    }
    do {
        (*count_arr) += 1;
        arr_strings[*count_arr] = (char* )calloc(SIZE_STRING, sizeof(char));
        gets(arr_strings[*count_arr]);

    } while(strlen(arr_strings[*count_arr]) != 0);
}

// чтение одного компонента выражения (операция или значение True или False)
char *read_token(char **input)
{
    if (!input)
        return "";
    while (isspace(**input)) ++*input;

    char *true_var = "True";
    char *false_var = "False";

    if (strncmp(*input, true_var, strlen(true_var)) == 0) {
        *input += strlen(true_var);
        return "1";
    }

    if (strncmp(*input, false_var, strlen(false_var)) == 0) {
        *input += strlen(false_var);
        return "0";
    }

    char *tokens[NUMBER_OPERATION] = { "and", "or", "not", "xor", "(", ")" };
    char *tmp = "";
    for (int i = 0; i < NUMBER_OPERATION; i++) {
        tmp = tokens[i];
        if (strncmp(*input, tmp, strlen(tmp)) == 0) {
            *input += strlen(tmp);
            break;
        }
    }

    return tmp;
}

// чтение одинарного выражения
tree_node *read_simple_expression(char **input)
{
    char *token = read_token(input);

    if (strlen(token) == 0) {
        return NULL;
    }

    if (strncmp(token, "(", 1) == 0) {
        tree_node *result = read_expression(0,input);
        if (strncmp(read_token(input), ")", 1) != 0) {
            free(result);
            return NULL;
        }
        return result;
    }
    tree_node * result = (tree_node* )malloc(sizeof(tree_node));
    if (!result)
        return NULL;
    result->token = token;
    if (isdigit(token[0])) {
        result->left = NULL;
        result->right = NULL;
    } else {
        result->right = NULL;
        result->left = read_simple_expression(input);
    }
    return result;
}


enum operation_priority {
    xor_v = 1,
    or_v = 1,
    and_v,
    not_v
};

// получение приоритета
int get_priority(const char * binary_op)
{
    enum operation_priority prior;
    if (strncmp(binary_op, XOR, 3) == 0) {
        return (prior = xor_v);
    }
    if (strncmp(binary_op, OR, 2) == 0) {
        return (prior = or_v);
    }
    if (strncmp(binary_op, AND, 3) == 0) {
        return (prior = and_v);
    }
    if (strncmp(binary_op, NOT, 3) == 0) {
        return (prior = not_v);
    }
    return 0;
}

// чтение бинарного выражения
tree_node *read_expression(int min_priority, char **input)
{
    tree_node* left_expr = read_simple_expression(input);

    if (!left_expr) {
        return NULL;
    }

    bool ans = false;

    while(!ans) {
        char *op = read_token(input);
        int priority = get_priority(op);
        if (priority <= min_priority) {
            *input -= strlen(op);
            ans = true;
        }
        else {
            tree_node *right_expr = read_expression(priority,input);
            tree_node *tmp_left = left_expr;
            left_expr = (tree_node* )malloc(sizeof(tree_node));
            left_expr->token = op;
            left_expr->left = tmp_left;
            left_expr->right = right_expr;
        }
    }
    return left_expr;
}

// решение выражения в дереве
int resolve_tree(const tree_node *root)
{
    if (strlen(root->token) == 0)
        return -1;
    if (root->left && root->right) {
        int a = resolve_tree(root->left);
        int b = resolve_tree(root->right);
        //printf("%d %d\n",a, b);
        if (strncmp(root->token, XOR, 3) == 0) {
            return a ^ b;
        }
        if (strncmp(root->token, OR, 2) == 0) {
            return a | b;
        }
        if (strncmp(root->token, AND, 3) == 0) {
            return a & b;
        }
    } else if (root->left) {
        int a = resolve_tree(root->left);
        if (strncmp(root->token, NOT, 3)== 0) {
            return !a;
        }
    } else {
        return strtod(root->token, NULL);
    }
    return 0;
}

bool is_letter(char a)
{
    if (a < 'a' || a > 'z')
        return 0;
    return 1;
}

// Проверка имени переменной согласно условиям
bool check_name(const char *name)
{
    char *tokens[NUMBER_OPERATION] = { "and", "or", "not", "xor", "(", ")" };
    for (int i = 0; i < NUMBER_OPERATION; i++) {
        char *tmp = tokens[i];
        if (strncmp(name, tmp, strlen(tmp)) == 0) {
            return false;
        }
    }
    for (int i = 0; i < strlen(name); i++) {
        if (!is_letter(name[i]))
            return false;
    }
    return true;
}



// Обработка объявления переменной и занесение в словарь
int read_var(char *strings, dict *dictionary, int *k)
{
    int err = -1;
    char *name = (char* )malloc(TOKEN_SIZE * sizeof(char));
    if (!name) {
        return err;
    }
    char *val = (char* )malloc(TOKEN_SIZE * sizeof(char));
    if (!val) {
        free(name);
        return err;
    }
    int i = 0;
    int j = 0;
    while (isspace(*strings)) ++strings;
    while (*strings != ' ' && *strings != '=') {
        name[i++] = *strings++;
    }
    name[i] = 0;

    if (!check_name(name)) {
        free(name);
        free(val);
        return err;
    }

    while (isspace(*strings)) ++strings;
    if (*strings != '=')
        return err;
    else
        ++strings;
    while (isspace(*strings)) ++strings;
    while (*strings != ' ' && *strings != ';' && *strings != 0) {
        val[j++] = *strings++;
    }
    val[j] = 0;

    if (strncmp(val, "True", 4) != 0 && strncmp(val, "False", 5) != 0) {
        return err;
    }

    while (isspace(*strings)) ++strings;
    //printf("%s   %s \n", name, val);
    //printf("%c\n",*strings );
    if (*strings != ';')
        return err;
    else {
        for(int w = 0; w < *k; w++) {
            if (strncmp(dictionary[w].name, name, strlen(name)) == 0) {
                dictionary[w].val = val;
                return 0;
            }
        }
        dictionary[*k].name = name;
        dictionary[*k].val = val;
        *k += 1;
    }
    return 0;
}


// Освобождение памяти из под дерева
void free_tree(tree_node * node)
{
    if (!node)
        return;
    if (node->left)   free_tree(node->left);
    if (node->right)  free_tree(node->right);
    //printf("%s\n",node->token);
    free(node);

    return;
}

// Проверка строки основного выражения на корректность
bool check_str(char *str)
{
    char *input = str;
    char *true_var = "True";
    char *false_var = "False";

    while(strlen(input) !=  0) {
        int len1 = strlen(input);
        while (isspace(*input)) ++input;
        if (strncmp(input, true_var, strlen(true_var)) == 0) {
            input += strlen(true_var);
        }

        if (strncmp(input, false_var, strlen(false_var)) == 0) {
            input += strlen(false_var);
        }

        char *tokens[NUMBER_OPERATION] = { "and", "or", "not", "xor", "(", ")" };
        for (int i = 0; i < NUMBER_OPERATION; i++) {
            char *tmp = tokens[i];
            if (strncmp(input, tmp, strlen(tmp)) == 0) {
                input += strlen(tmp);
            }
        }

        if  (strlen(input) == len1)
            return false;
    }

    return true;
}
