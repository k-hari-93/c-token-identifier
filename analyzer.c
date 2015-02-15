#include<stdio.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>


int main(int argc, char **argv)
{
    FILE *fp = fopen(argv[1],"r");
    char line, c;
    int i = 0;

    while((line = (char)fgetc(fp))!=EOF)
    {
        printf("%c",line);
    }
    printf("\n");
    fclose(fp);

    return 0;
}
