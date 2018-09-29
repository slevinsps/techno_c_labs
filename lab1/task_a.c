# include "stdio.h"
# include "stdlib.h"
# include "string.h"
# include "math.h"
# include "assert.h"

#define STRING_SIZE 50
#define ASCII_NUMBER_FIRST 47
#define ASCII_NUMBER_LAST 58
#define ASCII_BIG_LETTER_FIRST 65
#define ASCII_BIG_LETTER_LAST 90
#define ASCII_DIST_BETWEEN_BIG_SMALL_LETTER 32
#define MAIN_BASE 10
#define LOW_BORDER_BASE 2
#define HIGH_BORDER_BASE 36

#define OK 0
#define ERROR_SCANF -1
#define ERROR_CONVERTER -2

int ret_number(char c) {
    //assert((c <= 'Z' && c >= 'A') || (c <= 'z' && c >= 'a') || (c <= '9' && c >= '0'));
    if (c < ASCII_NUMBER_LAST && c > ASCII_NUMBER_FIRST)
        return c - '0';
    if (c > ASCII_BIG_LETTER_LAST)
        c -= ASCII_DIST_BETWEEN_BIG_SMALL_LETTER;
    return (c - 'A' + MAIN_BASE);
}

char ret_char(int n) {
    assert(n < MAIN_BASE && n >= 0);
    char c = ' ';
    if (n < MAIN_BASE) {
        c = '0' + n;
    } else {
        c = 'A' + (n - MAIN_BASE);
    }
    return c;
}

void reverse_string(char *s) {
    int len = (int)strlen(s);
    int mid = len / 2;
    int i = 0;
    while (i < mid) {
        char tmp = s[i];
        s[i] = s[len - 1 - i];
        s[len - 1 - i] = tmp;
        i++;
    }
}

int correct_input(int p, char *s) {
    int len = (int)strlen(s);
    int err = OK;
    for(int i = 0; i < len; i++) {
        if (ret_number(s[i]) >= p || ret_number(s[i]) < 0) {
            err = ERROR_CONVERTER;
            break;
        }
    }
    return err;
}

long long int converter_to_dec(int p, char *s) {
    assert(p >= 2);
    int len_str = strlen(s);
    long long int dec_ss = 0;
    for (int i = 0; i < len_str; i++) {
        dec_ss += ret_number(s[len_str - 1 - i]) * pow(p, i);
    }
    return dec_ss;
}

char *converter(int p, int q, char *s) {
    if (q < LOW_BORDER_BASE || q >= p ||
        p > HIGH_BORDER_BASE || correct_input(p, s) == ERROR_CONVERTER) {
        return NULL;
    }

    long long int dec_ss = converter_to_dec(p, s);

    char *ans = (char *)calloc(STRING_SIZE, sizeof(char));;
    int i = 0;
    do {
        ans[i++] = ret_char(dec_ss % q);
        dec_ss = dec_ss / q;
    } while (dec_ss != 0);
    ans[i] = '\0';

    reverse_string(ans);

    return ans;
}

int main(void) {
    int p;
    int q;
    char *s = (char *)calloc(STRING_SIZE, sizeof(char));
    int err = OK;
    char *ans = NULL;
    if (scanf("%d %d %s", &p, &q, s) == 3) { // from p to q
        ans = converter(p, q, s);
        if (!ans)
            err = ERROR_CONVERTER;
    } else
        err = ERROR_SCANF;

    if (err == OK)
    {
        printf("%s", ans);
    }
    else
        printf("[error]");

    free(s);
    free(ans);
    return 0;
}
