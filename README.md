# Unix-Shell

A simple unix shell.<br />

**I.	Executing Child Process**<br />

Firstly the input command line is parsed into tokens *args[] array of strings. The args array is then passed to the execute() function which checks the pid of the process. If it is a child process, it executes the command. If it is a parent process and the & flag is set to zero, it waits till the child process finishes its execution, otherwise it doesn’t wait.
The & flag is set to one, only if the parse function finds an & char when parsing the char array. 

**II.	History Feature**<br />

After parsing the command line, if arg[0] equals  “!!”  , the history buffer flag is checked. If it is one, the command in the history buffer is executed. If if arg[0] doesn’t equals  “!!”  , the command in the history buffer is updated.
History flag is set to one if the command entered doesn’t equal the history command. 

**III.	Redirecting Input and Output** <br />

When parsing the command argument, if it contains redirecting input and output symbols, the redirect_ flag is set to the redirect character. Then to redirect output, the standard output is redirected to the file in the child process. Then, the output is redirected to the standard output before finishing the child process. The same case happens when directing input. 
