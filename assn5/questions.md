1. Your shell forks multiple child processes when executing piped commands. How does your implementation ensure that all child processes complete before the shell continues accepting user input? What would happen if you forgot to call waitpid() on all child processes?

The shell uses waitpid() to wait on all child processes in order, making sure that none of them can finish before the previous ones do and give them the piped input. It also uses a supervisor that the main waits on to ensure the entire process finishes before the main shell can progress. If waitpid() isn't called, undefined behaviour may progress due to multiple processes continuing at the same time with null piped inputs.

2. The dup2() function is used to redirect input and output file descriptors. Explain why it is necessary to close unused pipe ends after calling dup2(). What could go wrong if you leave pipes open?

Because the system only has so many file descriptors, not closing pipes can cause issues since we might run out of pipes, which can cause blocking and unexpected behaviour. This can also cause file descriptor memory leaks, which results in memory risks.

3. Your shell recognizes built-in commands (cd, exit, dragon). Unlike external commands, built-in commands do not require execvp(). Why is cd implemented as a built-in rather than an external command? What challenges would arise if cd were implemented as an external process?

The cd command works with the shell application itself, and is not something that can be pushed off to another program, because we need to change what the shell itself is working with. External commands run some other process possibly using resources and information from the shell, but don't change what's going on in the shell itself. If cd was an external process, we would fork it into a child process, which would then end and do nothing to the main process.

4. Currently, your shell supports a fixed number of piped commands (CMD_MAX). How would you modify your implementation to allow an arbitrary number of piped commands while still handling memory allocation efficiently? What trade-offs would you need to consider?

We could use heap allocation (malloc()) to dynamically create memory for pipes and arrays. We would also have to figure out how to close pipes after they are used so that we don't run out of file descriptors and avoid memory leaks.