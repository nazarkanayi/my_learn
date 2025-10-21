#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_ARG 10

struct cmd_process
{
    /* data */
    char cmd[100];
    void (*cmd_hdlr)(char **, int);
};

void add(char **args, int count)
{
    printf("add\n");
}

void test(char **args, int count)
{
    printf("test %d\n",count);
    for(int i = 0; i< count ;i++)
    {
        printf("arg %d: %s\n",i,args[i]);
    }
}
struct cmd_process commands[] =
    {
        {"add", add},
        {"test", test}};

int add_argumrnts(char **args, int *count, char *str)
{
    if (*count >= MAX_ARG)
    {
        printf("Max arg reached\n");
        return -1;
    }

    char *data = (char *)malloc(strlen(str) + 1);

    strcpy(data, str);

    args[*count] = data;

    (*count)++;

    return 0;
}

int main()
{
    char sentence[100];

    char *arglist[MAX_ARG];

    int argc = 0;

    char token[100] = "";

    printf("Enter a sentence: ");
    fgets(sentence, sizeof(sentence), stdin); // Read the entire line

    int len = strlen(sentence);

    if (len > 0 && sentence[len - 1] == '\n')
    {
        sentence[len - 1] = '\0'; // Remove the newline character

        len--;
    }

    int tk_len = 0;
    int k = 0;
    for (k = 0; k < len; k++)
    {
        if (sentence[k] != ' ')
        {
            token[tk_len++] = sentence[k];
        }
        else
        {

            token[tk_len] = '\0';
            tk_len = 0;
            printf("token detected: %s\n", token);
            add_argumrnts(arglist, &argc, token);
        }
    }

    token[tk_len] = '\0';
    if(tk_len !=0)
    {
      printf("token detected: %s\n", token);
      add_argumrnts(arglist, &argc, token);
    }
    

    bool status = false;
    for (int i = 0; i < sizeof(commands) / sizeof(struct cmd_process); i++)
    {
        if (strcmp(arglist[0], commands[i].cmd) == 0)
        {
            commands[i].cmd_hdlr(&arglist[1],argc-1);

            status = true;
        }
    }

    if (!status)
    {
        printf("\ncommand not found\n");
    }

    return 0;
}