
Aditya Srivastava	-	201425112



Information and Directions:


The main program lies in the file "shell_main.c" which calls all other headers required by the program.

To compile the program, run the "make" command. Then launch "shell" from the terminal.

To exit the program, terminate using [ctrl + C].



Header File Descriptions:

1. background.c		-	Checks status of all background processes held in the list.
				{check_bg_proc()}

2. bg_pid_list.c	-	Linked list for holding backgrounded processes and functions for its
				manipulation.
				{insert(), delete(), show()}

3. call.c		-	Functions which call the appropriate builtins and shell executables as
				required.
				{call(), call_pwd(), call_echo(), call_cd(), call_pinfo(), call_exec()}

4. colors.c		-	Defines color constants for the shell.

5. parse.c		-	Holds command and query parsing statements which return lists of
				appropriately tokenized command statements and queries.
				{parser(), multi()}

6. prompt.c		-	Prints the command prompt.
				{prompt()}



Program Flow:

1.	Print the command prompt (prompt.c => prompt()).

2.	Scan user input (shell_main.c).

3.	Check if input conatins multiple ";" separated commands. If yes, split (parse.c => multi()) 
	and run one by one individually (call.c => call()), else run normally.

4.	Tokenize the command (parse.c => parser()) and then using the first keyword check if the command
	is built in or otherwise and and pass the command and all it's flags accordingly (call.c => call_*()).

5.	If the command statement contained "&", fork and add it's PID to a list (bg_pid_list.c => insert())
	and don't wait for it. Else, wait for the child process to end.

6.	Go through all the PIDs stored in the list (background.c => check_bg_proc()) and remove the ones
	that have completed (bg_pid_pist.c => delete()) after printing status.

7.	Repeat from (1).



Caveats:

1.	To run a command in the  background, "&" should be inserted at the end separated by a [\SPACE] from the rest of the command statement.

2.	"echo" command prints `"` along with string.

