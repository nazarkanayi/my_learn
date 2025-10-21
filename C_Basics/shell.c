#include <stdio.h>
#include <string.h>
#include <stdbool.h>

struct cmd_process
{
    /* data */
    char cmd[100];
    void (*cmd_hdlr)(void);
};

void add(void)
{
    printf("add\n");
}


void test(void)
{
    printf("test\n");
}
struct cmd_process commands[] =
{
   {"add",add},
   {"test",test}
};

int main()
{
    char sentence[100];
        printf("Enter a sentence: ");
        fgets(sentence, sizeof(sentence), stdin); // Read the entire line

        int len = strlen(sentence);
        
        if (len > 0 && sentence[len - 1] == '\n') {
            sentence[len - 1] = '\0'; // Remove the newline character
        }
        bool status =false;
        for(int i =0; i < sizeof(commands)/sizeof(struct cmd_process); i++)
        {
            if(strcmp(sentence,commands[i].cmd) == 0)
            {
                  commands[i].cmd_hdlr();

                  status = true;
            }
        }

        if(!status)
        {
            printf("\ncommand not found\n");
        }

        return 0;
}