#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>

typedef char* String;

enum CMD {
	CMD_protegepracaramba,
	CMD_liberageral,
	CMD_rodeveja,
	CMD_rode,
  CMD_sai,
	CMD_error,
  CMD_EOF
};

enum STATE {
  STATE_running,
  STATE_quit
} shell_state = STATE_running;

String* getInputArray(void);

void freeStringArray(String*);

void mainLoop(void);

void protegePraCaramba(String);

void changePermission(String, int);

void liberaGeral(String);

void rode(String);

void shout(void);

void rodeVeja(String);

enum CMD matchCMD(String);

String literalCMDS[6] = {"protegepracaramba", "liberageral", "rodeveja", "rode", "sai", ""};

int main(int argc, char ** argv){
	mainLoop();
	return 0;
}


void mainLoop(void){
	String* input;
	enum CMD cmd;

	begin:
    printf("(^_^) ");
		input = getInputArray();
    
    if(shell_state == STATE_quit) {
      puts("Sayonara!");
      return;
    }

		cmd = matchCMD(input[0]);
		switch(cmd) {
			case CMD_protegepracaramba:
				protegePraCaramba(input[1]);
				break;
			case CMD_liberageral:
				liberaGeral(input[1]);
				break;
			case CMD_rodeveja:
				rodeVeja(input[1]);
				break;
			case CMD_rode:
				rode(input[1]);
				break;
      case CMD_sai:
        puts("Sayonara!");
        return;
			case CMD_error:
				shout();

		}

		freeStringArray(input);
	goto begin;
}

enum ReadInputState {
	READ_FILLING,
	READ_DONE,
	READ_END
};


String* getInputArray(void){
	char buffer[1024];
	String *out = malloc(sizeof(String) * 50);
	enum ReadInputState state = READ_DONE;
	int currentChar;
	int index = 0; /* Always points to the next one free */
	int charIndex = 0;

begin:
	currentChar = getchar();
  if(currentChar == EOF) shell_state = STATE_quit;
noread:
	switch(state) {
		case READ_FILLING:
			if(currentChar == ' ' || currentChar == EOF || currentChar == '\n') {
				out[index][charIndex] = '\0';
				charIndex = 0;
				index++; /* points to the next one free */

				/* check if it has ended */
				if(currentChar == EOF || currentChar == '\n') {
					state = READ_END;
          goto noread;
					break;
				} else {
					state = READ_DONE;
					break;
				}
			}

			out[index][charIndex++] = (char) currentChar;
			break;

		case READ_DONE:
			/* eats the spaces */
			if(currentChar == ' ') break;
			/* check if it has ended */
			if(currentChar == EOF || currentChar == '\n') {
				state = READ_END;
				break;
			}
			state = READ_FILLING;
			out[index] = malloc(sizeof(char) * 1024);
			out[index][charIndex++] = (char) currentChar;
			break;

		case READ_END:
			out[index] = NULL;
			goto end;
			break;
	}

	goto begin;

end:
	return out;
}

void freeStringArray(String* a){
  String *olda = a;
	while(*a != NULL) {
		free(*a);
		a += 1;
	}
	free(olda);
}

enum CMD matchCMD(String cmd){
  int i;
	for(i = 0; literalCMDS[i][0] != '\0'; i++){
		if(strcmp(cmd, literalCMDS[i]) == 0) return i;
	}
	return CMD_error;
}

void protegePraCaramba(String arg){
	changePermission(arg,000);
}

void changePermission(String arg, int mode){
	int s = chmod(arg, mode);
	printf("Retornou %d\n", s);
}

void liberaGeral(String arg){
	changePermission(arg, 0777);
}

void rode(String arg) {
  int input;
	pid_t ret;

  ret = fork();
  input = dup(0);

  close(0);

	if(ret == 0) {
		execve(arg, NULL, NULL);
	} else {
    dup(input);
  }
}

void shout(void) {
	puts("Cracatua quando voa se esparrama pelo chão, digita algo certo pois assim eu não entendo não!");
}

void rodeVeja(String arg) {
	pid_t ret = fork();
  int status = 0;

	if(ret == 0) {
		status = execve(arg, NULL, NULL);
    if(status == -1) exit(1); /* Exit if program does not exist */
	} else {
		wait(&status);
		printf("=> programa '%s' retornou %d\n", arg, status>>8); /*Exit status is on high byte of status */
	}
}