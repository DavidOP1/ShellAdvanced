# ShellAdvanced
Shell
*Running the program :

make all , will run the program 

make clean , cleans the files created by make

3.Prompt task:
 
 You must type : prompt = something , prompt= something , or prompt =something wont work , amount of spaces between the strings doesn't matter.
 
 4.echo $? task:
 
 Prints 0 on success , 1 on failure.
 
 6.Last command task : !!
 
 If the last command was !! , and you type !! again it will print an error that the command !! doesn't exist.
 
 8.Control C task:
 
 After you press type C , the required string will be printed but you need to press ENTER afterwards to continue using the shell.
 
10.Adding variables task:
 
 If you add the variable $person = David , and then you type again : $person = Tal , it will interptate the string : $David = Tal , so it will print and error , 
 since there is no commmand $tal.
 
 11.read command : 
 
 You must type read var_name , press enter , and then enter the value of the variable name , overriding of the variable value here is possible.
 Example can be seen in screenshot.
 
 12.Arrow task:
 
 Works the same as in a real shell. If your page up too much it will print that youre out of commands so page down.
 
 It only prints the command , then the prompt will appear , if you keep the command input empty(you can enter spaces) and hit enter it will enter the command
 
 printed from the arrow history. See example below:
 
 ![arrow-shell](https://user-images.githubusercontent.com/54214707/232021224-50fa4fbc-1c59-4ea9-aba1-6206feafdbd4.png)

 
13.IF/ELSE task:
 
 once you entered , if ... no matter if its wrong or not , you need to enter fi , becuase the shell wont run correctly , so you must enter fi at the end.
 
Example: more example will be seen in the picture.

devz : if ....

 enter
 
devz: then

devz: command

enter

devz: else

enter

devz : command

enter

fi

enter

Example:

![shell-seond](https://user-images.githubusercontent.com/54214707/232021506-0812bad7-5384-4a77-a516-9f5f82b619c4.png)

