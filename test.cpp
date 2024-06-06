#include <iostream>
#include <vector>
#include <map>
#include <unistd.h>
#include <sys/wait.h>

int main(int ac, char **av, char **env)
{
    std::string form_type;
    std::vector<std::string> pure_data;
    pure_data.push_back("eyubech");

    form_type = "login";

    pure_data.push_back("eyubech");
    pure_data.push_back("test");

    std::string user = "eyubech";
    std::string pass = "test";

    const char *username = user.c_str();
    const char *password = pass.c_str();

    char *const command[] = {"/usr/local/bin/python3", "test.py", "1", (char *const)username, 
        (char *const)password, NULL};
    pid_t pid = fork();

    if (pid == 0) 
    {
        execve("/usr/local/bin/python3", command, env);
        perror("execve failed");
        return 1;
    } 
    else 
    {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            std::cout << "Child process exited with status " << WEXITSTATUS(status) << std::endl;
        } else {
            std::cout << "Child process did not terminate normally" << std::endl;
        }
    }

    return 0;
}
