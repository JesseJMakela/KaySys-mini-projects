/*
 * wish - Wisconsin Shell
 * A simple Unix shell with support for built-in commands,
 * redirection, and parallel command execution.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_PATHS 100
#define MAX_ARGS 100
#define MAX_COMMANDS 100

// Error message types
char error_message[30] = "An error has occurred\n";
char error_not_found[30] = "wish: command not found\n";
char error_cd_args[40] = "wish: cd requires one argument\n";
char error_exit_args[40] = "wish: exit takes no arguments\n";
char error_redirect[30] = "wish: redirection error\n";

// Path management
char *search_paths[MAX_PATHS];
int num_paths = 0;

// Print standard error message to stderr
void print_error()
{
    write(STDERR_FILENO, error_message, strlen(error_message));
}

// Print specific error message to stderr
void print_error_msg(char *msg)
{
    write(STDERR_FILENO, msg, strlen(msg));
}

// Initialize default search path to /bin
void initialize_paths()
{
    search_paths[0] = strdup("/bin");
    num_paths = 1;
}

// Free all allocated path strings
void free_paths()
{
    for (int i = 0; i < num_paths; i++)
    {
        free(search_paths[i]);
    }
    num_paths = 0;
}

// Search for executable in path directories
char *find_executable(char *cmd)
{
    static char full_path[512];

    for (int i = 0; i < num_paths; i++)
    {
        snprintf(full_path, sizeof(full_path), "%s/%s", search_paths[i], cmd);
        if (access(full_path, X_OK) == 0)
        {
            return full_path;
        }
    }
    return NULL;
}

// Check if command is a built-in
int is_builtin(char *cmd)
{
    if (cmd == NULL)
        return 0;
    return (strcmp(cmd, "exit") == 0 ||
            strcmp(cmd, "cd") == 0 ||
            strcmp(cmd, "path") == 0);
}

// Execute built-in commands (exit, cd, path)
int execute_builtin(char **args, int num_args)
{
    if (strcmp(args[0], "exit") == 0)
    {
        if (num_args != 1)
        {
            print_error_msg(error_exit_args);
            return -1;
        }
        exit(0);
    }
    else if (strcmp(args[0], "cd") == 0)
    {
        if (num_args != 2)
        {
            print_error_msg(error_cd_args);
            return -1;
        }
        if (chdir(args[1]) != 0)
        {
            print_error();
            return -1;
        }
        return 0;
    }
    else if (strcmp(args[0], "path") == 0)
    {
        free_paths();
        for (int i = 1; i < num_args; i++)
        {
            search_paths[num_paths] = strdup(args[i]);
            num_paths++;
        }
        return 0;
    }
    return -1;
}

// Remove leading and trailing whitespace from string
char *trim_whitespace(char *str)
{
    while (*str == ' ' || *str == '\t')
    {
        str++;
    }

    char *end = str + strlen(str) - 1;
    while (end > str && (*end == ' ' || *end == '\t' || *end == '\n' || *end == '\r'))
    {
        end--;
    }
    *(end + 1) = '\0';

    return str;
}

// Parse command line and execute commands (supports parallel execution and redirection)
void parse_and_execute(char *line)
{
    char line_copy[4096];
    strncpy(line_copy, line, sizeof(line_copy) - 1);
    line_copy[sizeof(line_copy) - 1] = '\0';

    // Split by '&' for parallel commands
    char *commands[MAX_COMMANDS];
    int num_commands = 0;

    char *cmd = strtok(line_copy, "&");
    while (cmd != NULL && num_commands < MAX_COMMANDS)
    {
        commands[num_commands] = trim_whitespace(cmd);
        if (strlen(commands[num_commands]) > 0)
        {
            num_commands++;
        }
        cmd = strtok(NULL, "&");
    }

    if (num_commands == 0)
    {
        return;
    }

    pid_t pids[MAX_COMMANDS];
    int num_pids = 0;

    // Execute each command
    for (int i = 0; i < num_commands; i++)
    {
        char command_copy[4096];
        strncpy(command_copy, commands[i], sizeof(command_copy) - 1);
        command_copy[sizeof(command_copy) - 1] = '\0';

        char *command_line = command_copy;

        // Count '>' symbols first
        int redirect_count = 0;
        char *temp = command_line;
        while ((temp = strchr(temp, '>')) != NULL)
        {
            redirect_count++;
            temp++;
        }

        if (redirect_count > 1)
        {
            print_error_msg(error_redirect);
            continue;
        }

        // Check for redirection
        char *redirect_pos = strchr(command_line, '>');
        char *output_file = NULL;

        if (redirect_pos != NULL)
        {
            *redirect_pos = '\0';
            redirect_pos++;

            redirect_pos = trim_whitespace(redirect_pos);

            // Parse output file
            char redirect_copy[1024];
            strncpy(redirect_copy, redirect_pos, sizeof(redirect_copy) - 1);
            redirect_copy[sizeof(redirect_copy) - 1] = '\0';

            char *output_tokens[10];
            int num_output_tokens = 0;
            char *output_token = strtok(redirect_copy, " \t");
            while (output_token != NULL && num_output_tokens < 10)
            {
                output_tokens[num_output_tokens++] = output_token;
                output_token = strtok(NULL, " \t");
            }

            if (num_output_tokens == 0)
            {
                print_error_msg(error_redirect);
                continue;
            }

            if (num_output_tokens > 1)
            {
                print_error_msg(error_redirect);
                continue;
            }

            output_file = strdup(output_tokens[0]);
        }

        // Parse arguments
        char *args[MAX_ARGS];
        int num_args = 0;

        char *token = strtok(command_line, " \t");
        while (token != NULL && num_args < MAX_ARGS - 1)
        {
            args[num_args] = token;
            num_args++;
            token = strtok(NULL, " \t");
        }
        args[num_args] = NULL;

        if (num_args == 0)
        {
            if (output_file != NULL)
                free(output_file);
            continue;
        }

        // Check if it's a built-in command
        if (is_builtin(args[0]))
        {
            execute_builtin(args, num_args);
            if (output_file != NULL)
                free(output_file);
            continue;
        }

        // Find the executable
        char *executable = find_executable(args[0]);
        if (executable == NULL)
        {
            print_error_msg(error_not_found);
            if (output_file != NULL)
                free(output_file);
            continue;
        }

        pid_t pid = fork();
        if (pid < 0)
        {
            print_error();
            if (output_file != NULL)
                free(output_file);
            continue;
        }
        else if (pid == 0)
        {
            // Child process
            if (output_file != NULL)
            {
                int fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd < 0)
                {
                    print_error();
                    exit(1);
                }
                dup2(fd, STDOUT_FILENO);
                dup2(fd, STDERR_FILENO);
                close(fd);
            }

            execv(executable, args);
            print_error();
            exit(1);
        }
        else
        {
            // Parent process - save pid
            pids[num_pids] = pid;
            num_pids++;
            if (output_file != NULL)
                free(output_file);
        }
    }

    // Wait for all child processes
    for (int i = 0; i < num_pids; i++)
    {
        waitpid(pids[i], NULL, 0);
    }
}

int main(int argc, char *argv[])
{
    FILE *input = stdin;
    int interactive = 1;

    // Check arguments
    if (argc > 2)
    {
        print_error();
        exit(1);
    }

    if (argc == 2)
    {
        input = fopen(argv[1], "r");
        if (input == NULL)
        {
            print_error();
            exit(1);
        }
        interactive = 0;
    }

    // Initialize search paths
    initialize_paths();

    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    while (1)
    {
        // Print prompt in interactive mode
        if (interactive)
        {
            printf("wish> ");
            fflush(stdout);
        }

        // Read line
        read = getline(&line, &len, input);

        // Check for EOF
        if (read == -1)
        {
            break;
        }

        // Remove newline
        if (line[read - 1] == '\n')
        {
            line[read - 1] = '\0';
        }

        // Skip empty lines
        char *trimmed = trim_whitespace(line);
        if (strlen(trimmed) == 0)
        {
            continue;
        }

        // Parse and execute
        parse_and_execute(line);
    }

    // Cleanup
    free(line);
    free_paths();

    if (!interactive)
    {
        fclose(input);
    }

    return 0;
}
