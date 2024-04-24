#include "myShell.h"
#include "myFunction.h"


int main()
{
    welcome();
    while (1) {
        getPcUsername();
        printf(": ");
        getLocation(); // Get current location
        fflush(stdout);
        char *input = getInputFromUser(); // Get user input
        if (input == NULL) {
            printf("Error reading input.\n");
            continue;
        }
        char **arguments = splitArgument(input); // Split input into arguments

        char **part1 = NULL;
        char **part2 = NULL;
        int foundPipe = 0; // Flag for detecting '|'
        for (int i = 0; arguments[i] != NULL; ++i) {
            if (strcmp(arguments[i], "|") == 0) {
                arguments[i] = NULL; // Split arguments
                part1 = arguments;
                part2 = &arguments[i + 1];
                foundPipe = 1;
                break; // Assume only one '|' character
            }
        }

        if (foundPipe) {
            mypipe(part1, part2);
        } else {
            char *command = arguments[0];
            if (strcmp(command, "exit") == 0) {
                logout(input);
                break;
            } else if (strcmp(command, "cp") == 0) {
                cp(arguments);
            } else if (strcmp(command, "cd") == 0) {
                cd(arguments);
            } else if (strcmp(command, "rm") == 0) {
                if (arguments[1] == NULL) {
                    printf("Usage: delete <file>\n");
                } else {
                    delete(arguments[1]);
                }
            } else if (strcmp(command, "move") == 0) {
                move(arguments);
            } else if (strcmp(command, "echo") == 0) {
                echoppend(&arguments[1]);
            } else if (strcmp(command, "echor") == 0) {
                echorite(arguments);
            } else if (strcmp(command, "rf") == 0) {
                readFile(arguments);
            }else if (strcmp(command, "wc") == 0) {
                wordCount(arguments);
            } else {
                printf("Command not recognized: %s\n", command);
            }
        }
        free(input);
        free(arguments);
    }
    
    
    return 0;
}
