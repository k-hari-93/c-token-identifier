#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<regex.h>

int main(int argc, char **argv)
{
    if(argc!=3)
    {
        printf("Error:Usage is '%s <inFile> <outFile>'\n",argv[0]);
        return 1;
    }
    FILE *fp_i = fopen(argv[1],"r"), *fp_o = fopen(argv[2],"w");
    regex_t intg,flt,expnz;
    regmatch_t _intg[1],_flt[1], _expn[1];
    char *token = NULL, *buf = NULL, *_match = NULL, c;
    int i = 0, j, w = 0, begin, end, size, integer, fl, exp;
    size_t len = 0;

    regcomp(&intg, "([[:digit:]]+)", REG_EXTENDED|REG_NEWLINE);
    regcomp(&flt, "([[:digit:]]+.[[:digit:]]+)", REG_EXTENDED|REG_NEWLINE);
    regcomp(&expnz, "([[:digit:]]+.[[:digit:]]+[Ee][+-]?[[:digit:]]+)", REG_EXTENDED|REG_NEWLINE);

    while((getline(&token, &len, fp_i))!=-1)
    {
        i = 0;
        while((c = token[i++]) != '\n')
        {
            switch(c)
            {
            case '+':
                fprintf(fp_o,"<operator,addition>");
                break;
            case '-':
                fprintf(fp_o,"<operator,subtraction>");
                break;
            case '/':
                fprintf(fp_o,"<operator,division>");
                break;
            case '*':
                fprintf(fp_o,"<operator,multiplication>");
                break;
            case '=':
                if(token[i] == '=') {fprintf(fp_o,"<re-operator,isEqualTo>");++i;}
                else fprintf(fp_o,"<operator,assignment>");
                break;
            case '<':
                if(token[i] == '=') {fprintf(fp_o,"<re-operator,isLessThanOrEqualTo>");++i;}
                else fprintf(fp_o,"<re-operator,isLessThan>");
                break;
            case '>':
                if(token[i] == '=') {fprintf(fp_o,"<re-operator,isGreaterThanOrEqualTo>");++i;}
                else fprintf(fp_o,"<re-operator,isGreaterThan>");
                break;
            case '!':
                fprintf(fp_o,"<re-operator,isNotEqual>");
                ++i;
                break;
            case '^':
                fprintf(fp_o,"<operator,xor>");
                break;
            case '|':
                if(token[i] == '|') {fprintf(fp_o,"<lo-operator,or>");++i;}
                else fprintf(fp_o,"<lo-operator,bitwiseOr>");
                break;
            case '&':
                if(token[i] == '&') {fprintf(fp_o,"<lo-operator,and>");++i;}
                else fprintf(fp_o,"<lo-operator,bitwiseAnd>");
                break;
            case ' ':
                continue;
            default:
                if(isalpha(c) || c == '_')
                {
                    fprintf(fp_o,"<identifier,");
                    while(isalnum(c) || c == '_')
                    {
                        fprintf(fp_o,"%c",c);
                        c = token[i++];
                    }
                    fprintf(fp_o,">");
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
                            fprintf(fp_o,"<exponential-constant,%s>",_match);
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
                            fprintf(fp_o,"<floating-constant,%s>",_match);
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
                            fprintf(fp_o,"<integer-constant,%s>",_match);
                            break;
                        }
                    }
                }

            }
        }
        fprintf(fp_o,"\n");
    }
    return 0;
}
