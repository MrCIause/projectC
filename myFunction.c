#include "myFunction.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void welcome()
{
    printf("                         ______                     \n");
    printf(" _________        .---\"\"\"      \"\"\"---.              \n");
    printf(":______.-':      :  .--------------.  :             \n");
    printf("| ______  |      | :                : |             \n");
    printf("|:______B:|      | |  Little Error: | |             \n");
    printf("|:______B:|      | |                | |             \n");
    printf("|:______B:|      | |  Power not     | |             \n");
    printf("|         |      | |  found.        | |             \n");
    printf("|:_____:  |      | |                | |             \n");
    printf("|    ==   |      | :                : |             \n");
    printf("|       O |      :  '--------------'  :             \n");
    printf("|       o |      :'---...______...---'              \n");
    printf("|       o |-._.-i___/'             \\._              \n");
    printf("|'-.____o_|   '-.   '-...______...-'  `-._          \n");
    printf(":_________:      .____________________   -.___.-. \n");
    printf("                 .'.eeeeeeeeeeeeeeeeee.'.      :___:\n");
    printf("    fsc        .'.eeeeeeeeeeeeeeeeeeeeee.'.         \n");
    printf("              :____________________________:\n\n");
    printf("Welcome to my cShell project ~AdiShcherbakov\n");
}

char *getInputFromUser()
{
    char ch;
    int size = 1;
    int index = 0;

    char *str = (char *)malloc(size* sizeof(char));
    puts("Enter strings:");
    while((ch = getchar()) != '\n')
    {
        *(str+index) = ch;
        size++;
        index++;
        str = (char *)realloc(str, size * sizeof(char));
    }
    *(str+index) = '\0';
    return str;
}

void blue()
{

    printf("\033[0;34m");
}

void green() 
{
    printf("\033[0;32m");
}

void reset()
{

     printf("\033[0m");
}

char **splitArgument(char *str)
{
    // Check if the input string is NULL or empty
    if (str == NULL || str[0] == '\0')
    {
        printf("Empty command.\n");
        return NULL;
    }

    int size = 1;                                     // Initialize size to 1 to account for the NULL terminator at the end
    char **arguments = malloc(size * sizeof(char *)); // Allocate memory for the array of strings
    if (arguments == NULL)
    {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    char *token = strtok(str, " "); // Tokenize the input string using space as the delimiter
    while (token != NULL)
    {
        arguments[size - 1] = token; // Assign the token to the current position in the arguments array
        size++;
        arguments = realloc(arguments, size * sizeof(char *)); // Resize the arguments array to accommodate the next token
        if (arguments == NULL)
        {
            printf("Memory allocation failed.\n");
            exit(1);
        }
        token = strtok(NULL, " "); // Move to the next token
    }
    arguments[size - 1] = NULL; // Set the last element of the arguments array to NULL to mark the end

    return arguments;
}

void getLocation()
{
    char location[BUFF_SIZE];

    if (getcwd(location, BUFF_SIZE) == NULL)
    {
        puts("Error");
    }
    else
    {
        blue();
        puts(location);
        reset();
    }
}

void logout(char *str)
{
    fflush(stdout);
    sleep(1);
    printf("\n");
    printf(">{}< bye!\n");
    printf("(づ ᴗ _ᴗ)づ♡\n");

    printf("\n");
    printf("Exit success!!!\n");
    printf("GOOD BYE!\n");
    exit(EXIT_SUCCESS);
}

void echo(char **arg)
{
    while (*(++arg)) // this while starts with index 1, after echo string and runs till NULL printing the echo string.
    {
        printf("%s ",*arg);
    }
    puts("");

    // int i = 1;  // anothe example of while, it takes more resources because of int i.
    // while(arg[i]!=NULL)
    //     printf("%s ",arg[i++]);
    
}

void getPcUsername()
{
    char* username = getenv("USER");
    if(!username)
    {
        username = getenv("LOGNAME");
    }

    if (username) 
    {
        green();
        printf("%s@%s", username,username);
    } 
    else 
    {
        printf("Username could not be determined.\n");
    }
}

void cd(char **args)
{
    char newPath[1024] = {0}; // Assume the path will not exceed 1024 characters

    // If only cd, without arguments, or argument starts with '~', go to home directory
    if (args[1] == NULL || strcmp(args[1], "~") == 0)
    {
        const char *home = getenv("HOME");
        if (home != NULL)
        {
            strcpy(newPath, home);
        }
        else
        {
            fprintf(stderr, "cd: HOME not set\n");
            return;
        }
    }
    else if (args[1][0] == '\"' || args[1][0] == '\'')
    { // Handling path in quotes
        // Concatenate all parts of the path into one string, remove quotes
        for (int i = 1; args[i] != NULL; ++i)
        {
            // Add spaces between parts, except the first one
            if (i > 1)
                strcat(newPath, " ");
            // Remove quotes from arguments and append to path
            strncat(newPath, args[i] + (args[i][0] == '\"' || args[i][0] == '\''), strlen(args[i]) - (args[i][0] == '\"' || args[i][0] == '\'' ? 2 : 0));
            // Check if the current argument ends with a quote
            if (args[i][strlen(args[i]) - 1] == '\"' || args[i][strlen(args[i]) - 1] == '\'')
            {
                break;
            }
        }
    }
    else
    { // Simple path without quotes
        strcpy(newPath, args[1]);
    }

    if (chdir(newPath) != 0)
    {
        perror("cd: Error changing directory");
    }
}

void cp(char **args)
{
    if (args[1] == NULL || args[2] == NULL) {
        printf("Error: Missing source or destination path.\n");
        return;
    }

    char *srcPath = args[1];
    char *destPath = args[2];
    FILE *src, *dest;
    char ch;
// Open the source file
    src = fopen(srcPath, "r");
    if (src == NULL)
    {
        perror("Error opening source file");
        return;
    }
    dest = fopen(destPath, "w");
    if (dest == NULL)
    {
        perror("Error opening destination file");
        fclose(src);
        return;
    }
// Copy the contents of the source file to the destination file
    while ((ch = fgetc(src)) != EOF)
    {
        fputc(ch, dest);
    }

    fclose(src);
    fclose(dest);
    printf("File copied successfully from %s to %s\n", srcPath, destPath);
}

void delete(char **args) {
    if (args[1] == NULL) {
        printf("Error: Missing file path to delete.\n");
        return;
    }
// Delete the file
    char *filePath = args[1];
    if (unlink(filePath) != 0) {
        perror("Error deleting file");
    } else {
        printf("File deleted successfully: %s\n", filePath);
    }
}

void systemCall(char **arg)
{
    pid_t pid = fork();
    if(pid == -1)
    {
        printf("fork err\n");
        return;
    }
    if(pid == 0 && execvp(arg[0],arg) == -1)
    {
        exit(1);
    }
}

void mypipe(char **argv1, char **argv2) {
    if (argv2 == NULL) {
        if (fork() == 0) {
            execvp(argv1[0], argv1);
            perror("Command execution error");
            exit(EXIT_FAILURE);
        }
    } else {
        int fildes[2];
        if (pipe(fildes) == -1) {
            perror("Error creating pipe");
            return;
        }

        if (fork() == 0) {
            close(fildes[0]);
            dup2(fildes[1], STDOUT_FILENO);
            close(fildes[1]);
            execvp(argv1[0], argv1);
            perror("first Command execution error");
            exit(EXIT_FAILURE);
        }

        if (fork() == 0) {
            close(fildes[1]);
            dup2(fildes[0], STDIN_FILENO);
            close(fildes[0]);
            execvp(argv2[0], argv2);
            perror("seond Command execution error");
            exit(EXIT_FAILURE);
        }

        close(fildes[0]);
        close(fildes[1]);
        wait(NULL);
        wait(NULL);
    }
}

void move(char **args) {
    if (args[1] == NULL || args[2] == NULL) {
        printf("Error: Missing source or destination path.\n");
        return;
    }

    char *srcPath = args[1];
    char *destPath = args[2];
    char newPath[BUFF_SIZE];

    if (destPath[0] == '.' && destPath[1] == '.') {
        char *base = basename(srcPath);
        snprintf(newPath, sizeof(newPath), "%s/%s", destPath, base);
    } else {
        snprintf(newPath, sizeof(newPath), "%s/%s", destPath, basename(srcPath));
    }

    if (rename(srcPath, newPath) == 0) {
        printf("File moved successfully to %s\n", newPath);
    } else {
        perror("Faild to move file");
        return;
    }

}

void echoppend(char **args) {
    char *message = NULL;
    char *filePath = NULL;

    // Check if enough arguments are provided
    if (args[0] == NULL || args[1] == NULL || args[2] == NULL) {
        fprintf(stderr, "Usage: echoppend <message> >> <path_to_file>\n");
        return;
    }

    // Search for ">>" arrows and the file path in the arguments
    for (int i = 0; args[i] != NULL; i++) {
        if (args[i][0] == '>' && args[i][1] == '>' && args[i][2] == '\0' && args[i + 1] != NULL) {
            filePath = args[i + 1]; 
            break;
        }
    }

    // Check for the presence of the file path
    if (filePath == NULL) {
        fprintf(stderr, "Usage: echoppend <message> >> <path_to_file>\n");
        return;
    }

    // Extract the message from the arguments
    message = args[0];

    // Open (or create) the file for appending data
    int fd = open(filePath, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd == -1) {
        perror("Error opening file");
        return;
    }

    // Write the message to the file
    ssize_t write_len = write(fd, message, strlen(message));
    if (write_len == -1) {
        perror("Error writing to file");
    } else {
        // Add a new line after the message for readability
        write(fd, "\n", 1);
    }

    // Close the file
    close(fd);
}

void echorite(char **args) {
    // Check if enough arguments are provided
    if (args[1] == NULL || args[2] == NULL || args[3] == NULL) {
        fprintf(stderr, "Usage: <command> <message> > <path_to_file>\n");
        return;
    }

    // The path to the file is in the fourth element, considering the command as the first element
    char *path = args[3];
    // The message to be written is in the second element
    char *message = args[1];

    // Open the file for overwriting
    FILE *file = fopen(path, "w");
    if (!file) {
        perror("Error opening file");
        return;
    }

    // Write the message
    if (fputs(message, file) == EOF) {
        perror("Error writing to file");
    }

    // Close the file
    fclose(file);
}

void readFile(char **args) {
    // Check if the file path is provided
    if (args[1] == NULL) {
        fprintf(stderr, "Usage: rf <path_to_file>\n");
        return;
    }

    // Try to open the specified file
    FILE *file = fopen(args[1], "r");
    if (!file) {
        // If the file cannot be opened, assume it does not exist and do not print an error
        return;
    }

    // Read and print the contents of the file with a visually appealing format
    printf("\n=== Contents of the file: %s ===\n\n", args[1]);
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        printf("  %s", buffer);
    }
    printf("\n=== End of file ===\n\n");

    // Close the file
    fclose(file);
}

void wordCount(char **args) {
    if (args[1] == NULL || args[2] == NULL) {
        fprintf(stderr, "Usage: wc <options> <path_to_file>\nOptions:\n -l for line count\n -w for word count\n");
        return;
    }

    char *option = args[1];
    char *path = args[2];

    FILE *file = fopen(path, "r");
    if (!file) {
        fprintf(stderr, "File %s cannot be opened.\n", path);
        return;
    }

    int lines = 0, words = 0;
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        if (strcmp(option, "-l") == 0) {
            lines++;
        } else if (strcmp(option, "-w") == 0) {
            char *token = strtok(buffer, " \t\n");
            while (token != NULL) {
                words++;
                token = strtok(NULL, " \t\n");
            }
        }
    }

    if (strcmp(option, "-l") == 0) {
        printf("Lines: %d\n", lines);
    } else if (strcmp(option, "-w") == 0) {
        printf("Words: %d\n", words);
    }

    fclose(file);
}

