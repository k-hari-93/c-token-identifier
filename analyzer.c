#include<stdio.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>


int main(int argc, char **argv)
{
    FILE *fp = fopen(argv[1],"r");
    char *token = NULL , c;
    int i = 0;
    size_t len = 0;

    while((getline(&token, &len, fp))!=-1)
    {
        printf("%s",token);
    }

    return 0;
}
