# Simple Linux Shell Implementation in C

This project implements a basic Linux shell in C. The shell operates in a loop, performing the following actions:

1. Prints a custom prompt
2. Reads a command in sh syntax (excluding internal commands or functions)
3. Executes the sh command using fork(), execv(), and waits for it to complete using wait()
4. Exits the program if the entered command is "leave"
