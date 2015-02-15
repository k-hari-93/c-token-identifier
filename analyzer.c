#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>

int main(int argc, char **argv)
{
    FILE *fp = fopen(argv[1],"r");
    char *token = NULL , c;
    int i = 0;
    size_t len = 0;

    while((getline(&token, &len, fp))!=-1)
    {
        i = 0;
        while((c = token[i++]) != '\n')
        {
            switch(c)
            {
            case '+':
                printf("<operator,addition>");
                break;
            case '-':
                printf("<operator,subtraction>");
                break;
            case '/':
                printf("<operator,division>");
                break;
            case '*':
                printf("<operator,multiplication>");
                break;
            case '=':
                if(token[i] == '=') {printf("<operator,isEqualTo>");++i;}
                else printf("<operator,assignment>");
                break;
            case '<':
                if(token[i] == '=') {printf("<operator,isLessThanOrEqualTo>");++i;}
                else printf("<operator,isLessThan>");
                break;
            case '>':
                if(token[i] == '=') {printf("<operator,isGreaterThanOrEqualTo>");++i;}
                else printf("<operator,isGreaterThan>");
                break;
            case '!':
                printf("<operator,isNotEqual>");
                ++i;
                break;
            case '^':
                printf("<operator,xor>");
                break;
            case ' ':
                continue;
            default:
                if(isalpha(c))
                {
                    printf("<identifier,");
                    while(isalnum(c) || c == '_')
                    {
                        printf("%c",c);
                        c = token[i++];
                    }
                    printf(">");
                    i--;
                }
                else
                {
                    if(isdigit(c))
                    {
                        printf("<number,");
                        while(isdigit(c))
                        {
                            printf("%c",c);
                            c = token[i++];
                        }
                        printf(">");
                        --i;
                    }
                }
            }
        }
        printf("\n");
    }

    return 0;
}
