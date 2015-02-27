#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<regex.h>

int main(int argc, char **argv)
{
    regex_t intg,flt,expnz;
    regmatch_t _intg[1],_flt[1], _expn[1];
    FILE *fp = fopen(argv[1],"r");
    char *token = NULL, *buf = NULL, *_match = NULL, c;
    int i = 0, j, w = 0, begin, end, size, integer, fl, exp;
    size_t len = 0;

    regcomp(&intg, "([[:digit:]]+)", REG_EXTENDED|REG_NEWLINE);
    regcomp(&flt, "([[:digit:]]+.[[:digit:]]+)", REG_EXTENDED|REG_NEWLINE);
    regcomp(&expnz, "([[:digit:]]+.[[:digit:]]+[Ee][+-]?[[:digit:]]+)", REG_EXTENDED|REG_NEWLINE);

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
                if(token[i] == '=') {printf("<re-operator,isEqualTo>");++i;}
                else printf("<operator,assignment>");
                break;
            case '<':
                if(token[i] == '=') {printf("<re-operator,isLessThanOrEqualTo>");++i;}
                else printf("<re-operator,isLessThan>");
                break;
            case '>':
                if(token[i] == '=') {printf("<re-operator,isGreaterThanOrEqualTo>");++i;}
                else printf("<re-operator,isGreaterThan>");
                break;
            case '!':
                printf("<re-operator,isNotEqual>");
                ++i;
                break;
            case '^':
                printf("<operator,xor>");
                break;
            case '|':
                if(token[i] == '|') {printf("<lo-operator,or>");++i;}
                else printf("<lo-operator,bitwiseOr>");
                break;
            case '&':
                if(token[i] == '&') {printf("<lo-operator,and>");++i;}
                else printf("<lo-operator,bitwiseAnd>");
                break;
            case ' ':
                continue;
            default:
                if(isalpha(c) || c == '_')
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
                        exp = regexec(&expnz, &token[i-1], 1, _expn, 0);
                        if(!exp)
                        {
                            begin =  (int)_expn[0].rm_so;
                            end = (int)_expn[0].rm_eo;
                            size = end - begin;
                            _match = malloc(size+1);
                            for(j=begin;j<end;++j)
                            {
                                _match[w++] = token[i-1+j];
                            }
                            i = i + size-1;
                            _match[w] = '\0';
                            w = 0;
                            printf("<exponential-constant,%s>",_match);
                            break;
                        }
                        fl = regexec(&flt, &token[i-1], 1, _flt, 0);
                        if(!fl)
                        {
                            begin =  (int)_flt[0].rm_so;
                            end = (int)_flt[0].rm_eo;
                            size = end - begin;
                            _match = malloc(size+1);
                            for(j=begin;j<end;++j)
                            {
                                _match[w++] = token[i-1+j];
                            }
                            i = i + size-1;
                            _match[w] = '\0';
                            w = 0;
                            printf("<floating-constant,%s>",_match);
                            break;
                        }
                        integer = regexec(&intg, &token[i-1], 1, _intg, 0);
                        if(!integer)
                        {
                            begin = (int)_intg[0].rm_so;
                            end = (int)_intg[0].rm_eo;
                            size = end - begin;
                            _match = malloc(size+1);
                            for(j=begin;j<end;++j)
                            {
                                _match[w++] = token[i-1+j];
                            }
                            i = i + size-1;
                            _match[w] = '\0';
                            w = 0;
                            printf("<integer-constant,%s>",_match);
                            break;
                        }
                    }
                }

            }
        }
        printf("\n");
    }
    return 0;
}
