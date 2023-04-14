#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <termios.h>
#include <sys/wait.h>

int check_not_shell(){
    return 0;
}
void mem_res(){}
volatile int flag_c = 0;

/// @brief //////////////////////////SIGNAL FUNCTION TO CATCH THE CONTROL C SIGNAL////////////////
/// @param signum /
void c_handler(int signum){
    printf("\nYou typed Control-C!\n");
    flag_c = 1;
}
char swap_command[1024];

////////////////////////////////////////////EXECUTING THE COMMAND/////////////////////////////////
int run(char* command) {
    pid_t p_id = fork();
    int status = 0;
    if (p_id == 0) {
        // Child process
        char* args[] = {"/bin/sh", "-c", (char*) command, NULL};
        status = execv("/bin/sh", args);
        exit(1);
    } else if (p_id < 0) {
        // if the fork failed
        return 1;
    } else {
        //main processs wait for execv to finish.
        waitpid(p_id, &status, 0);

        //Got some help from stack overflow , since I had some problem of getting the status.
        if (WIFEXITED(status)) {
            return WEXITSTATUS(status);
        } else {
            return 1;
        }
    }
}



////////////////////////////////////////////swap variables in command if exist////////////////////
char*  swap_var(char * command,int var_index,char var_list[][100]){
    int var = 0;
    memset(swap_command,0,1024);
    char * token = strtok(command , " ");

    while(token!=NULL){
        var=0;
        if (token[0]=='$')
        {
            //Here check if the variable exists.
            for(int i =0 ; i < var_index; i+=2){
                if(var_list[i]&&var_list[i+1]){
                if(!strcmp(token,var_list[i])) {
                    strcat(swap_command , var_list[i+1]); var=1;  break;}
                }
            }

        }
        
        if(!var){
            
        strcat(swap_command,token);}
        strcat(swap_command," ");
        token=strtok(NULL," ");
    }

    swap_command[strlen(swap_command)-1]='\0';
    return swap_command;

}

//////////////////////////////////////////////CREATING A LINKED LIST TO KEEP THE 20 LAST COMMANDS//////////////////////////
struct Node {
    char data[1024];
    struct Node * next;
    struct Node * prev;
};

typedef struct Node Node;
void add_node(Node ** head , Node ** tail, char command[]) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    strcpy(newNode->data, command);
    newNode->prev = NULL;

    if (*head == NULL) {
        newNode->next = NULL;
        *head = newNode;
        *tail = newNode;
    } else {
        newNode->next = *head;
        (*head)->prev = newNode;
        *head = newNode;
    }
}


///////////////////////////////////////////////FREEING THE LIST WHEN QUITTING FOR THE SHELL////////////////////
void free_list(Node * head , Node * tail) {
    Node* curr = head;
    while (curr != NULL) {
        Node* temp = curr;
        curr = curr->next;
        free(temp);
    }
    head = NULL;
    tail = NULL;
}

int main(){
    /////////////////////////////////////////////////INITIALIZING VARIABLES////////////////////////////////////
    char command[1024] , command_temp[1024], not_shell[1024],prompt[1024] , last_command[1024],command_temp_2[1024]
    ,command_new[1024],var_list[40][100],condition[1024],arrow_command[1024]
    ,command_temp_3[1024];//format : index i : variable , index i+1 variable value.
    memset(arrow_command,0,1024);
    memset(var_list,0,sizeof(var_list));
    strcpy(prompt,"devz");
    memset(last_command,0,1024);
    signal(SIGINT,c_handler);
    int not_shell_command = 0 , counter = 0 , status = 0 , error = 0 , var_index=0;
    int command_index=0,up_index=0,down_index=0,read=0,cond=0;
    Node * head  = NULL;
    Node * tail = NULL;
    Node * current_command = NULL;
    memset(condition,0,1024);


    /////////////////////////////////////////////////MAIN LOOP////////////////////////////////////////
    while(1){
        error = 0;
        fflush(stdout);
        flag_c=0;
        printf("%s: " , prompt);
        memset(command_temp_2,0,1024);
        memset(command ,0 ,1024);
        memset(command_temp,0 ,1024);
        memset(not_shell ,0 ,1024);
        memset(command_new ,0 ,1024);
        memset(command_temp_3,0,1024);

        fgets(command,1024,stdin);
        /********swap var*********/


        if(!flag_c){
        not_shell_command = 0;
        if(strlen(command)>0){
            command[strlen(command)-1]='\0';
        }

    
         strcpy(command_temp_3,command);
         char * token3 = strtok(command_temp_3," ");

        if (token3==NULL)
        {
            if(!command_index&&arrow_command){
                strcpy(command , arrow_command);
            }else{
            continue;
            }
        }

        strcpy(command_temp_2,command);
        char *token2 = strtok(command_temp_2," ");
        

        strcpy(command_new,command);


        //////////////////////////////////////////////IF ARROW KEY IS PRESSED/////////////////////////////////////
        //////////////////////////////////////////////ARROW UP///////////////////////////////////////////////////
        if(!strcmp("\x1b[A",command)){
            if(command_index&&current_command){
                printf("command : %s\n",current_command->data);
                strcpy(arrow_command,current_command->data);
                command_index =0;
              }
            else if(current_command->next&&!command_index){
                current_command = current_command -> next;
                printf("command : %s\n",current_command->data);
                strcpy(arrow_command,current_command->data);
            }else{
                printf("out of commands! PAGE DOWN\n"); 
            }
            continue;
            }
        
        /////////////////////////////////////////ARROW DOWN////////////////////////////////////////////////////
        else if(!strcmp("\x1b[B",command)){
             if(current_command->prev){
                current_command = current_command ->prev;
                strcpy(arrow_command,current_command->data);
                printf("command : %s\n",current_command ->data);
            }else{
                printf("out of commands! PAGE UP\n"); 
            }
        
            continue;
            }

        add_node(&head,&tail,command);
        current_command = head;
        command_index =1;
        ////////////////////////////////////////////////////IF/ELSE/////////////////////////////////////////
        if(!strcmp("if",token2) || cond){

            if((!strcmp("else",token2)||!strcmp("then",token2))){strcat(condition,command); strcat(condition," "); continue;}
            else if(!strcmp("fi",token2)){strcat(condition,command); system(condition); memset(condition,0,1024); cond=0; continue;}
            else {strcat(condition,command); strcat(condition,";"); cond=1 ;continue;}
        }
        //////////////////////////////////////////////////////READ VARIABLES////////////////////////////////
        else if(!strcmp("read",token2)||read){
            char var[1024];
            memset(var,0,1024);
            if(read){
                    int index=var_index;
                    //CHECKING IF VARIABLE ALREADY EXISTS.
                    for(int i = 0 ; i <var_index; i+=2){
                        if(var_list[i]&&var_list[i+1]){
                        if(!strcmp(var_list[i],last_command)){
                            index = i;
                            break;
                        }}
                    }

                    strcpy(var_list[index],last_command);

                    while(token2!=NULL){
                        strcat(var , token2);
                        strcat(var," ");
                        token2=strtok(NULL," ");
                    }
                    var[strlen(var)-1]='\0';
                    strcpy(var_list[index+1],var);


                    read=0;
                    var_index+=2;
                    not_shell_command = 1;
            }
            else if((token2=strtok(NULL," "))&&(!strtok(NULL," "))){

                strcat(var,"$");
                strcat(var,token2);
                strcpy(last_command,var);
                not_shell_command=1;
                read=1;
            }

        }
        /////////////////////////////////////////////////!! execute last command///////////////////////////////////
        else if(!strcmp("!!",token2) || !strcmp("quit",token2)){
            if(!strtok(NULL," ")&&!strcmp("!!",token2)){
                strcpy(command,last_command);
        ///////////////////////////////////////////////quit shell/////////////////////////////////////////////
            }else if(!strtok(NULL," ")&&!strcmp("quit",token2)){
                free_list(head,tail);
                exit(0);
            }
            else
                error = 1;
        }
        
        
        
        strcpy(command,swap_var(command,var_index,var_list));
        
        strcpy(command_temp,command);


        char * token  = strtok(command_temp , " ");
        
        counter = 0;

        //Checking for non shell command.
        while(token!=NULL&&!error){
            strcat(not_shell,token);
            /////////////////////////////////////////////////prompt/////////////////////////////////////
            if(!strcmp(not_shell ,"prompt=")&&counter>=1){
                token =  strtok(NULL," ");
                memset(prompt,0 ,1024);
                while(token!=NULL){
                strcat(prompt , token);
                strcat(prompt, " ");
                token = strtok(NULL , " ");
                }
                prompt[strlen(prompt)-1] = '\0';
                not_shell_command = 1;
                status = 0;
                break;
            }
            ////////////////////////////////////////////////regular variable////////////////////////////
            else if(token[0]=='$'&&!counter){
                if(strtok(NULL," ")[0]=='='){
                int flag =0;
                if(strcmp(token , var_list[var_index])){flag = 1;}
                strcpy(var_list[var_index],token);
                token = strtok(NULL," ");
                while(token!=NULL){
                        strcat(var_list[var_index+1],token);
                        strcat(var_list[var_index+1], " ");
                        token = strtok(NULL," ");
                    }
                var_list[var_index+1][strlen(var_list[var_index+1])-1] = '\0';
                not_shell_command=1;
                if(flag){var_index+=2;}
                }else{
                    break;
                }
                break;

            }
            //////////////////////////////////////////////print status///////////////////////////////////
            else if(!strcmp(not_shell,"echo$?")&&counter>=1){
                not_shell_command=1;
                printf("%d\n",status);
                status = 0;
                break;
            }
            ////////////////////////////////////////////////cd///////////////////////////////////////////
            else if(!strcmp(not_shell,"cd")){
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
        
        /////////////////////////////////////////////RUNNING THE COMMAND/////////////////////////////////

        if(!not_shell_command){
         status = run(command);
         if(status){
            status = 1;
         }

        }

        if(!read){
        strcpy(last_command,command_new);
        }
    }
    }
}