# include "stdio.h"
# include "stdlib.h"
# include "string.h"
# include "math.h"

int ret_number(char c)
{
    if (c < 58 && c > 47)
        return c - '0';
    if (c > 90)
        c -= 32; 
    return c - 'A' + 10; 
}

char ret_char(int n)
{
    if (n < 10)
    {
        return '0' + n;
    }
    else 
    {
        return 'A' + (n - 10);
    }
}

void reverse_string(char *s)
{
    int len = strlen(s);
	int mid = len / 2;
	int i = 0;
    while (i < mid)
	{
		char tmp = s[i];
		s[i] = s[len - 1 - i];
		s[len - 1 - i] = tmp;
        i++;
	}
}

int correct_input(int p, char *s)
{
    int len = strlen(s);
    int err = 1;
    for(int i = 0; i < len; i++)
    {
        if (ret_number(s[i]) >= p || ret_number(s[i]) < 0)
        {
            err = 0;
            break;
        }
    }
    return err;
}

long long int converter_to_dec(int p, char *s)
{
    int len_str = strlen(s);
    long long int dec_ss = 0;
    for (int i = 0; i < len_str; i++)
    {
        dec_ss += ret_number(s[len_str - 1 - i]) * pow(p, i);
    }
    return dec_ss;
}

char *converter(int p, int q, char *s)
{
    if (q < 2 || q >= p ||
        p > 36 || !correct_input(p, s))
    {
        return NULL;
    }

    long long int dec_ss = converter_to_dec(p, s);

    char *ans = (char *)malloc(100);
	int i = 0;
	do
	{
		ans[i++] = ret_char(dec_ss % q);
		dec_ss = dec_ss / q;
	} while (dec_ss != 0);
	ans[i] = '\0';
    
    reverse_string(ans);
    
    return ans;
}

int main(void)
{
    int p;
    int q;
    char *s = (char *)malloc(20);
    if (scanf("%d %d %s", &p, &q, s) == 3) // from p to q
    {
        char *ans = converter(p, q, s);
        if (ans)
            printf("%s", ans);
        else
            printf("[error]");
        free(ans);
    }
    else
        printf("[error]");
    free(s);
    
    return 0;
}