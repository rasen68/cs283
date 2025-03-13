1. In this assignment I suggested you use `fgets()` to get user input in the main while loop. Why is `fgets()` a good choice for this application?

    > **Answer**:  fgets() can read specifically until we find a newline, which tells us that a command has been entered and we should begin to process it. 

2. You needed to use `malloc()` to allocte memory for `cmd_buff` in `dsh_cli.c`. Can you explain why you needed to do that, instead of allocating a fixed-size array?

    > **Answer**: We need to send the allocated cmd_buff memory as a pointer to our build_cmd_list function, which only works because it was returned by malloc() and not a statically defined fixed-size array.

3. In `dshlib.c`, the function `build_cmd_list(`)` must trim leading and trailing spaces from each command before storing it. Why is this necessary? If we didn't trim spaces, what kind of issues might arise when executing commands in our shell?

    > **Answer**:  Without trimming spaces, when testing a function for equality with one of our default functions, we may get a false negative if a command was inputted with extra spaces.

4. For this question you need to do some research on STDIN, STDOUT, and STDERR in Linux. We've learned this week that shells are "robust brokers of input and output". Google _"linux shell stdin stdout stderr explained"_ to get started.

- One topic you should have found information on is "redirection". Please provide at least 3 redirection examples that we should implement in our custom shell, and explain what challenges we might have implementing them.

    > **Answer**:  Redirection can be used to pass the contents of a file into a command, to send the output of a command into a file, or to append the otuput of a command to the bottom of a file (>>).

- You should have also learned about "pipes". Redirection and piping both involve controlling input and output in the shell, but they serve different purposes. Explain the key differences between redirection and piping.

    > **Answer**:  The key difference is that redirection redirects i/o to files in the system, possibly ones that don't yet exist as is often the case with using >, while piping redirects commands into each other, always starting and ending with real functions to create a larger composite function.

- STDERR is often used for error messages, while STDOUT is for regular output. Why is it important to keep these separate in a shell?

    > **Answer**:  STDOUT gets redirected or piped as usual, while STDERR is still often printed directly to the user to tell them that something went wrong. By separating these two streams we can more easily differentiate between debugging and error handling messages and intended output.

- How should our custom shell handle errors from commands that fail? Consider cases where a command outputs both STDOUT and STDERR. Should we provide a way to merge them, and if so, how?

    > **Answer**:  With bash, we can merge STEDERR and STDOUT and redirect both to a file by using &>, or redirect STDERR to STDOUT by using 2>&1. When implementing redirection, this could be a helpful tool if STDERR needs to be redirected to a pipe or file just like STDOUT, so we could provide a similar syntax. Because STDERR is a stream just like STDOUT, this shouldn't be any harder than implementing normal redirection. 
