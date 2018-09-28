# include "stdio.h"
# include "stdlib.h"
# include "string.h"
# include "math.h"
# include "ctype.h"

# define TOKEN_SIZE 20
# define NUMBER_SIZE 20
# define NUMBER_OPERATION 6

typedef struct tree_node_s
{
    char *token;
    struct tree_node_s *left;
    struct tree_node_s *right;
    
} tree_node;

typedef struct dict_s
{
    char *name;
    char *val;
    
} dict;

tree_node *parse_binary_expression(int min_priority, char **input);



char *parse_token(char **input) 
{
    while (isspace(**input)) ++*input;
    
    char *true = "True";
    char *false = "False";
    
    if (strncmp(*input, true, strlen(true)) == 0)
    {
        *input += strlen(true);
        return "1";
    }
    
    if (strncmp(*input, false, strlen(false)) == 0)
    {
        *input += strlen(false);
        return "0";
    }

    char *tokens[NUMBER_OPERATION] = { "and", "or", "not", "xor", "(", ")" };
    for (int i = 0; i < NUMBER_OPERATION; i++) 
    {
        char *tmp = tokens[i];
        if (strncmp(*input, tmp, strlen(tmp)) == 0) 
        {
            *input += strlen(tmp);
            return tmp;
        }
    }

    return "";
}

tree_node *parse_simple_expression(char **input) {
    
    char *token = parse_token(input);
    
    if (strlen(token) == 0)
    {
        return NULL;
    }

    if (strncmp(token, "(", 1) == 0)
    {
        tree_node *result = parse_binary_expression(0,input);
        if (strncmp(parse_token(input), ")", 1) != 0) 
        {
            return NULL;
        }
        return result;
    }
    tree_node * result = (tree_node* )malloc(sizeof(tree_node));
    result->token = token;
    if (isdigit(token[0]))
    { 
        result->left = NULL;
        result->right = NULL;
    }
    else
    {
        result->right = NULL;
        result->left = parse_simple_expression(input);
    }
    return result;
}

int get_priority(char * binary_op) 
{
    if (strncmp(binary_op, "xor", 1) == 0) return 1;
    if (strncmp(binary_op, "or", 1) == 0) return 1;
    if (strncmp(binary_op, "and", 3) == 0) return 2;
    if (strncmp(binary_op, "not", 3) == 0) return 3;
    return 0;
}

tree_node *parse_binary_expression(int min_priority, char **input) 
{
    
    tree_node * left_expr = parse_simple_expression(input);
    
    if (!left_expr)
    {
        
        return NULL;
    }

    while(1)
    {
        //printf("%s\n", *input);
        char *op = parse_token(input);
        int priority = get_priority(op);
        if (priority <= min_priority) {
            *input -= strlen(op);
            return left_expr;
        }

        tree_node *right_expr = parse_binary_expression(priority,input);
        tree_node *tmp_left = left_expr;
        left_expr = (tree_node* )calloc(1, sizeof(tree_node));
    
        //printf("%s\n", left_expr->token);
        left_expr->left = tmp_left;
        left_expr->right = right_expr;
    }
}

int eval(tree_node *root) 
{
    if (root->left && root->right)
    {
        int a = eval(root->left);
        int b = eval(root->right);
        //printf("%d %d\n",a, b);
        if (strncmp(root->token, "xor", 3) == 0) return a ^ b;
        if (strncmp(root->token, "or", 2) == 0) return a | b;
        if (strncmp(root->token, "and", 3) == 0) return a & b;
    }
    else if (root->left)
    {
        int a = eval(root->left);
        if (strncmp(root->token, "not", 3)== 0) return !a;
    }
    else
        return strtod(root->token, NULL);
    return 0;
}



int read_var(char *strings, dict *dictionary, int *k)
{
    char *name = (char* )malloc(20 * sizeof(char));
    char *val = (char* )malloc(20 * sizeof(char));
    int i = 0; 
    int j = 0;
    while (isspace(*strings)) ++strings;
    while (*strings != ' ' && *strings != '=')
    {
        name[i++] = *strings++;
    }
    name[i] = 0;
    while (isspace(*strings)) ++strings;
    if (*strings != '=')
        return -1;
    else
        ++strings;
    while (isspace(*strings)) ++strings;
    while (*strings != ' ' && *strings != ';' && *strings != 0)
    {
        val[j++] = *strings++;
    }
    val[j] = 0;
    
    if (strncmp(val, "True", 4) != 0 && strncmp(val, "False", 5) != 0)
    {
        return -1;
    }
    
    while (isspace(*strings)) ++strings;
    //printf("%s   %s \n", name, val);
    //printf("%c\n",*strings );
    if (*strings != ';')
        return -1;
    else
    {
        for(int w = 0; w < *k; w++)
        {
            if (strncmp(dictionary[w].name, name, strlen(name)) == 0)
            {
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

int check_str(char *str)
{
    char *input = str;    
    char *true = "True";
    char *false = "False";
    
    while(strlen(input) !=  0)
    {
        int len1 = strlen(input);
        while (isspace(*input)) ++input;
        if (strncmp(input, true, strlen(true)) == 0)
        {
            input += strlen(true);
        }
        
        if (strncmp(input, false, strlen(false)) == 0)
        {
            input += strlen(false);
        }
    
        char *tokens[NUMBER_OPERATION] = { "and", "or", "not", "xor", "(", ")" };
        for (int i = 0; i < NUMBER_OPERATION; i++) 
        {
            char *tmp = tokens[i];
            if (strncmp(input, tmp, strlen(tmp)) == 0) 
            {
                input += strlen(tmp);
            }
        }
        
        if  (strlen(input) == len1)
            return -1;
    }

    return 0;
}


int main(void)
{
    setbuf(stdout,NULL);
    //char *buf = (char* )malloc(100  * sizeof(char));
    char *arr_strings[100];
    dict *dictionary = (dict *)calloc(100,sizeof(dict));
    int count_arr = -1;
    //char buf[100];
    do // чтение 
    {
        count_arr += 1;
        arr_strings[count_arr] = (char* )calloc(100, sizeof(char));
        gets(arr_strings[count_arr]);
        /*if (strlen(buf) != 0)
        {
            
            memmove(arr_strings[count_arr++], buf, sizeof(char)*(strlen(buf) + 1));
        }*/
        
    }while(strlen(arr_strings[count_arr]) != 0);
    //count_arr--;
    if (count_arr == 0)
    {
        printf("[error]");
        //free(buf);
        free(dictionary);
        return 0;
    }
    
    /*for(int i = 0; i < count_arr; i++)
    {
        printf("%s\n", arr_strings[i]);
    }*/
    
    
    /*char* str1 = "     iscat    = True ;\n";
    char* str2 = "     isdog=False;\n";
    
    arr[0] = str1;
    arr[1] = str2;*/
    
    
    int err = 0;
    int ind = 0;
    for(int i = 0; i < count_arr - 1; i++) // занесение в словарь
    {
        err = read_var(arr_strings[i], dictionary, &ind);
        if (err == -1)
            break;
    }
    if (err == -1)
    {
        printf("[error]");
        //free(buf);
        for(int i = 0; i < count_arr; i++) // занесение в словарь
        {
            free(arr_strings[i]);
        }
        for(int i = 0; i < ind; i++) // занесение в словарь
        {
            free(dictionary[i].name);
            free(dictionary[i].val);
        }
        free(dictionary);
        return 0;
    }
   
    //for(int i = 0; i < ind; i++)
    //{
    //    printf("%s   %s \n", dictionary[i].name, dictionary[i].val);
    //}

    char *input = arr_strings[count_arr - 1];//"isdog or (not iscat and isdog) or True xor isdog";
    
    char *main_str_copy;
    char *main_str = (char* )malloc(100 * sizeof(char));
    main_str_copy = main_str;
    int w = 0;
    while(strlen(input) != 0) // замена переменных
    {
        for(int i = 0; i < ind; i++)
        {
            if (strncmp(input, dictionary[i].name, strlen(dictionary[i].name)) == 0) 
            { 
                input += strlen(dictionary[i].name);
                memcpy(main_str + w, dictionary[i].val, strlen(dictionary[i].val));
                w+=strlen(dictionary[i].val);
                break;
            }
        }
        main_str[w++] = *input++;
    }
    main_str[w] = 0;
    //printf("%s\n", main_str);
    
    /*if (check_str(main_str) == -1)
    {
        printf("[error]");
        for(int i = 0; i <= count_arr; i++) // занесение в словарь
        {
            free(arr_strings[i]);
        }
        for(int i = 0; i < ind; i++) // занесение в словарь
        {
            free(dictionary[i].name);
            free(dictionary[i].val);
        }
        free(dictionary);
        free(main_str_copy);
        return 0;
    }*/
    
    
    tree_node * root = parse_binary_expression(0, &main_str);
    int res = eval(root);
    if (res == 1)
        printf("True");
    else
        printf("False");  
    
    
    
    
    //free(buf);
    for(int i = 0; i <= count_arr; i++) // занесение в словарь
    {
        free(arr_strings[i]);
    }
    for(int i = 0; i < ind; i++) // занесение в словарь
    {
        free(dictionary[i].name);
        free(dictionary[i].val);
    }
    free(dictionary);
    free_tree(root);
    free(main_str_copy);   
    
    return 0;
}