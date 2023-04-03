#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <termios.h>

int check_not_shell(){
    return 0;
}
void mem_res(){}
volatile int flag = 0;

void c_handler(int signum){
    printf("\nYou typed Control-C!\n");
    flag = 1;
}
char swap_command[1024];
char*  swap_var(char * command,int var_index,char var_list[][100]){
    int var = 0;
    memset(swap_command,0,1024);
    char * token = strtok(command , " ");

    while(token!=NULL){
        printf("token : %s\n",token);
        var=0;
        if (token[0]=='$')
        {
            //Here check if the variable exists.
            for(int i =0 ; i < var_index; i+=2){
                if(var_list[i]&&var_list[i+1]){
                if(!strcmp(token,var_list[i])) {
                    printf("hello\n"); strcat(swap_command , var_list[i+1]); var=1;  break;}
                }
            }

        }
        if(!var){
        strcat(swap_command,token);}
        strcat(swap_command," ");
        token=strtok(NULL," ");
    }

    swap_command[strlen(swap_command)-1]='\0';
    printf("swaped final : %s\n",swap_command);
    return swap_command;

}

int main(){
    char command[1024] , command_temp[1024], not_shell[1024],prompt[1024] , last_command[1024],command_temp_2[1024]
    ,command_new[1024],var_list[40][100],command_list[20][1024];//format : index i : variable , index i+1 variable value.

    memset(var_list,0,sizeof(var_list));
    strcpy(prompt,"devz");
    memset(last_command,0,1024);
    signal(SIGINT,c_handler);
    int not_shell_command = 0 , counter = 0 , status = 0 , error = 0 , var_index=2;
    
    while(1){
        error = 0;
        fflush(stdout);
        printf("%s: " , prompt);
        memset(command_temp_2,0,1024);
        memset(command ,0 ,1024);
        memset(command_temp,0 ,1024);
        memset(not_shell ,0 ,1024);
        memset(command_new ,0 ,1024);

        fgets(command,1024,stdin);
        /********swap var*********/



        if(strlen(command)>0){
            command[strlen(command)-1]='\0';
        }
        // char * res = swap_var(command,var_index,var_list);
        // printf("swaped final : %s\n",res);



        strcpy(command,swap_var(command,var_index,var_list));
        printf("main command : %s\n",command);

        strcpy(command_temp,command);
        strcpy(command_temp_2,command);
        strcpy(command_new,command);
        char *token2 = strtok(command_temp_2," ");

        if(!strcmp("!!",token2) || !strcmp("quit",token2)){
            printf("entered\n");
            if(!strtok(NULL," ")&&!strcmp("!!",token2)){
                printf("last command : %s , %ld\n",last_command,strlen(last_command));
                strcpy(command_temp,last_command);
                strcpy(command,last_command);
            }else if(!strtok(NULL," ")&&!strcmp("quit",token2)){
                exit(0);
            }
            else
                error = 1;
        }

        char * token  = strtok(command_temp , " ");
        not_shell_command = 0;
        counter = 0;

        //Checking for non shell command.
        while(token!=NULL&&!error){
            strcat(not_shell,token);
            if(!strcmp(not_shell ,"prompt=")&&counter>=1){
                token =  strtok(NULL," ");
                strcpy(prompt,token);
                not_shell_command = 1;
                status = 0;
                break;
            }
            else if(token[0]=='$'&&!counter){

                if(strtok(NULL," ")[0]=='='){
                strcpy(var_list[var_index],token);
                while(token!=NULL){
                        strcat(var_list[var_index+1],token);
                        strcat(var_list[var_index+1], " ");
                        token = strtok(NULL," ");
                    }
                var_list[var_index+1][strlen(var_list[var_index+1])-1] = '\0';
                var_index+=2;
                not_shell_command=1;
                }else{
                    break;
                }
                break;
            }
            else if(!strcmp(not_shell,"echo$?")&&counter>=1){
                not_shell_command=1;
                status = 0;
                break;
            }else if(!strcmp(not_shell,"cd")){
                token =  strtok(NULL," "); 
                status = chdir(token);
                not_shell_command = 1;
                if(status){status=1;}
                break;
            }else if((!counter && (strcmp(not_shell,"prompt") && strcmp(not_shell,"echo"))) || counter>=1){
                break;
            }counter+=1;
            token = strtok(NULL," ");
        }

        if(!not_shell_command){
         status = system(command);
         if(status){
            status = 1;
         }

        }
        strcpy(last_command,command_new);
    }
}