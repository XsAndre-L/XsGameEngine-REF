#include "process.h"

#if defined(__linux__)

string getProcessID()
{
    pid_t pid = getpid();
    return toString(pid);
}

bool runProcess(const char* cmd)
{
    int status;
    pid_t pid = fork();

    if (pid == 0) // This is the child process
    {
        execl("/bin/sh", "sh", "-c", cmd, NULL);
        exit(EXIT_FAILURE); // execl only returns on error
    } else if (pid < 0) // Failed to fork
    {
        return false; // Return false if fork failed
    } else // This is the parent process
    {
        waitpid(pid, &status, 0); // Wait for the child to finish
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            return true; // Return true if child process exited normally with status 0
        } else {
            return false; // Return false if child process failed
        }
    }
}

#endif