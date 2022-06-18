/**
 * Simple shell interface program.
 *
 * Operating System Concepts - Tenth Edition
 * Copyright John Wiley & Sons - 2018
 */

# include <stdio.h>
# include <fcntl.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/wait.h>
# include <sys/types.h>
#define MAX_LINE		80 /* 80 chars per line, per command */
#define REDIRECT_INPUT_FAIL 1
#define READ_END 0
#define WRITE_END 1

char **init_args(char* Instruction,char** args,char * last_parameter)
{
	//DeInstruction
	int Index_Instruction=0;
	int Index_args=0;
	char tmp=Instruction[Index_Instruction];
	int precious_is_space=0;
	
	while(tmp!='\n'&&tmp!=EOF)
	{
		if(Index_Instruction==0)
		{
			free(args[0]);
			args[0]=(char *) malloc(sizeof(char)*MAX_LINE);
			memset(args[0], 0, sizeof(args[0]));

		}
		if(tmp==' ')
		{
			if(precious_is_space==0)
			{
				Index_args++;
				args[Index_args]=(char *)malloc(sizeof(char)*MAX_LINE);
				precious_is_space=1;
			}
		}
		else
		{
			strncat(args[Index_args],&tmp,1);
			
			precious_is_space=0;
		}
		
		Index_Instruction++;
		tmp=Instruction[Index_Instruction];	
	}

	if(args[Index_args]!=NULL)
		strcpy(last_parameter,args[Index_args]);
	if(strcmp(last_parameter,"&")==0)
		args[Index_args]=NULL;
}


char **init_history(char** history,char** args,char* last_parameter)
{	
	if(args[0]==NULL || strcmp(args[0],"!!")==0)
		return NULL;
	int i=0;
	for(i=0;i<MAX_LINE/2+1;i++)
	{
		if(args[i]==NULL)
			break;
		history[i]=(char*) malloc(MAX_LINE*sizeof(char));
		strcpy(history[i],args[i]);
	}
	if(strcmp(last_parameter,"&")==0)
	{
		history[i]=(char*) malloc(MAX_LINE*sizeof(char));
		history[i]="&";
	}
	
}

void redirect_output(char** args)
{
	int i=0;
	while(i<MAX_LINE/2+1)
	{
		if(args[i]!=NULL && strcmp(args[i],">")==0)// > exists
		{
			char file_name[MAX_LINE];
			strcpy(file_name,args[i+1]);
			
			int fd=open(file_name,O_RDWR | O_NOCTTY | O_NDELAY);
			dup2(fd,STDOUT_FILENO);

			free(args[i]);
			free(args[i+1]);
			args[i]=NULL;
			args[i+1]=NULL;

			close(fd);
		}
		i++;
	}
}

void redirect_input(char** args,char* last_parameter)
{
	int i=0;
	while(i<MAX_LINE/2+1)
	{
		if(args[i]!=NULL && strcmp(args[i],"<")==0)// < exists
		{
			char file_name[MAX_LINE];
			strcpy(file_name,args[i+1]);
			
			int fd=open(file_name,O_RDWR | O_NOCTTY | O_NDELAY);
			
			free(args[i]);
			free(args[i+1]);
			args[i]=NULL;
			args[i+1]=NULL;
			dup2(fd,STDIN_FILENO);
			char Instruction[MAX_LINE*sizeof(char)];
			fgets(Instruction,MAX_LINE,stdin);
			init_args(Instruction,args+i,last_parameter);

			close(fd);
		}
		i++;
	}
}

int find_pipe(char** args)
{
	int i=0;
	while(args[i]!=NULL&&i<MAX_LINE/2 + 1)
	{
	if(strcmp(args[i],"|")==0) return i;
	i++;
	}
	return 0;
	
}
int main(void)
{
	char *args[MAX_LINE/2 + 1]={NULL};	/* command line (of 80) has max of 40 arguments */
	char *history[MAX_LINE/2 + 1]={NULL};
	int should_run = 1;
		
	while (should_run){   
		for(int i=0;i<MAX_LINE/2+1;++i)
			args[i]=NULL;

		printf("osh>");
		fflush(stdout);
		
	/*
	Read instructions and preprocess
	*/
		char * Instruction;
		char * last_parameter;
		Instruction=(char*) malloc(sizeof(char)*MAX_LINE);
		last_parameter=(char *) malloc (sizeof(char)*MAX_LINE);

		fgets(Instruction,MAX_LINE,stdin);
		init_args(Instruction,args,last_parameter);//store the seperate blocks into args[i], && is eliminated and stored in last_parameter, redirection is not done
		init_history(history,args,last_parameter);//reset the history according to args
		free(Instruction);
	/*
	args already set.
	files are rediected to STDIN/OUT
	*/
	//special order
	
		if(args[0]!=NULL)
		{
		
			if(strcmp(args[0],"!!")==0)//!!
			{
			free(args[0]);
			args[0]=NULL;

			if(history[0]==NULL)
			{
				printf("No history!\n");
				continue;
			}
			else
			{
			        int i=0;
			        while(history[i]!=NULL&&i<MAX_LINE/2 + 1)
			        {
			            args[i]=(char*)malloc(MAX_LINE*sizeof(char));
				    strcpy(args[i],history[i]);
				    printf("%s ",history[i]);
				    i++;
			        }
				printf("\n");
			}
			}
			
			if(strcmp(args[0],"exit")==0)//exit
			{
			should_run=0;
			continue;
			}
		}
		else
		{
		    printf("No command!\n");
		    continue;
		}

	
	//pipe
		int pipe_position=find_pipe(args);
		if(pipe_position==0)// no pipe
		{
			pid_t pid;
			pid=fork();

			if (pid==0) //child process
			{
				/*
				redirection output >
				*/
				redirect_output(args);
				redirect_input(args,last_parameter);//problem: if && in in.txt, it will not be operated	
				execvp(args[0],args);
				should_run=0;
			}
			else //parent process
			{
				if(strcmp(last_parameter,"&")!=0)
					wait(NULL);
			}
		}
		else
		{
			pid_t pid;

			pid=fork();

			if(pid==0)//child process
			{
				int fd[2];
				pid_t pid;
				
				/*create a pipe*/
				if(pipe(fd)==-1)
				{
					fprintf(stderr,"Pipe failed");
					return 1;
				}

				pid=fork();

				if(pid>0)//parent process(actually child process of the initial one)
				{

					int i=pipe_position;
					while(args[i]!=NULL&&i<MAX_LINE/2 + 1)
					{
					free(args[i]);
					args[i]=NULL;
					i++;
					}
					close(fd[READ_END]);

					dup2(fd[WRITE_END],STDOUT_FILENO);

					execvp(args[0],args);
					
				}
				else if(pid==0)//grandson process
				{
					strcpy(args[0],args[pipe_position+1]);
					int i=1;
					while(args[i]!=NULL&&i<MAX_LINE/2 + 1)
					{
					free(args[i]);
					args[i]=NULL;
					i++;
					}
					close(fd[WRITE_END]);

					dup2(fd[READ_END],STDIN_FILENO);

					execvp(args[0],args);
				}
			}
			else // father process
			{
				wait(NULL);
			}
		}
		
	}
 		
	
	for(int i=0;i<MAX_LINE/2+1;i++)
		free(args[i]);
	for(int i=0;i<MAX_LINE/2+1;i++)
		free(history[i]);
	
	return 0;
}


