/*
Development started on: 9/19/25 [v1.0.0.c]
Last (finished) update: 9/22/25 [v1.1.0.c]
Current (indev) update: 9/24/25 [v1.2.0.c]
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#ifdef _WIN32
    #include <direct.h>
    #include <winsock2.h>
	#include <windows.h>
#else
    #include <unistd.h>
    #include <sys/stat.h>
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <netinet/in.h>
#endif

bool prefix(const char *pre, const char *str){
    return strncmp(pre,str,strlen(pre))==0;
} //CHECK IF "str" STARTS WITH `pre`
int clrcns(void){
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    return 0;
}
//made sure to release memory on fail
char* cwd(char** buf , int MAX) {
    *buf = malloc(MAX);
    if (*buf == NULL) {
        perror("Memory allocation failed");
        return NULL;
    }

#ifdef _WIN32
    if (_getcwd(*buf, MAX) != NULL) {
        return *buf;
    } else {
        perror("Error in `cwd` [WIN32]");
        free(*buf);
        return NULL;
    }
#else
    if (getcwd(*buf, MAX) != NULL) {
        return *buf;
    } else {
        perror("Error in `cwd` [LINUX/OTHER]");
        free(*buf);
        return NULL;
    }
#endif
}
int SOCKTEST(){
    printf("Testing Sockets.");
    #ifdef _WIN32
        printf(" [WIN32]\n");
        WSADATA wsa;
        SOCKET s;
        printf("Initializing Winsock...");
        if (WSAStartup(MAKEWORD(2,2),&wsa) != 0){
            printf("Startup failed. Error Code: %d\n",WSAGetLastError());
            return 1;
        }
        printf("Initialized.\n");
        if((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET){
            printf("Could not create socket : %d", WSAGetLastError());
        }
	closesocket(s);
        printf("Socket created.\n");
        return 0;
    #else
        printf(" [LINUX/OTHER]\n");
        int status, valread, client_fd;
        struct sockaddr_in serv_addr;
        if((client_fd = socket(AF_INET,SOCK_STREAM,0))<0){
            perror("Socket creation error\nERROR");
            return -1;
        }
        close(client_fd);
        printf("Socket test:SUCCESSFUL.\n");
        return 0;
    #endif
}
int SOCKCONN(char* IP, int PORT){
    if (SOCKTEST()==0){
        #ifdef _WIN32
	IntPtr pSockAddr = IntPtr.Zero;
	int RIP = WSAStringToAddressW(IP,System.Net.Sockets.AdressFamily.InterNetwork,IntPtr.Zero);
        SOCKET s=socket(AF_INET, SOCK_STREAM, 0);
	connect(s,inet_addr(IP),strlen(inet_addr(IP)));
        #else
        int status, valread, client_fd;
        struct sockaddr_in serv_addr;
        char* buffer[1024] = { 0 };
        client_fd = socket(AF_INET, SOCK_STREAM,0);
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(PORT);
        if(inet_pton(AF_INET, IP, &serv_addr.sin_addr)<=0){
            printf("Invalid address/Address unsupported.\n");
            return -1;
        }
        return client_fd;
        #endif
        printf("Connected to \"%s\".\n",IP);
    }
}
int SOCKSEND(char* DATA,int CONN){
    #ifdef _WIN32
    printf("WIN32 Not implemented.");
    return 1;
    #else
    send(CONN,DATA,strlen(DATA),0);
    #endif
}
struct Version {
	int major;
	int minor;
	int patch;
};

int main(void){
    FILE* fptr;
    int running = 1;
    char* buf[100];
    struct Version ver;
    ver.major=1;
    ver.minor=2;
    ver.patch=0;
    int curcon;
    char *tok=malloc(50);
r:
    //General Commands
    if(running == 1) {
        printf("\x1B[1m\x1B[36m%s\x1B[0m> ",cwd(buf,150));
        char inp[50];
        char dest[50];
        const char *s = " ";
        fgets(inp, sizeof(inp), stdin);
        inp[strcspn(inp, "\n")] = 0;
        //BASIC COMMANDS
        if (strcmp("help",inp)==0){
            printf("QShell HELP:\nBASIC:\n'help': Displays this menu.\n'ver': Displays the current QShell version.\n'exit': Exits the shell.\n'clr': Clears the screen.\nFILES:\n'make [filename]': Creates a file named [filename]\n'rmv [filename]': Removes [filename]\n'cat [filename]': Displays the contents of [filename]\n");
        }
        else if (strcmp("ver",inp)==0){
            printf("QShell \"%d.%d.%d\"",ver.major,ver.minor,ver.patch);
        }
        else if (strcmp("exit",inp)==0){
            running = 0;
            printf("Exiting...");
        }
        else if (strcmp("clr",inp)==0){
            clrcns();
        }
        //FileSystem Commands
        if (prefix("make",inp)){ //MAKE NEW FILE
            strcpy(dest,inp);
            tok=realloc(tok,50);
            if (tok==NULL){
                perror("MEMORY ERROR WHEN REALLOCATING `tok` IN COMMAND `make`\nERROR");
            }
            tok = strtok(inp,s);
            int x = 0;
            while (tok != 0) {
                tok = strtok(0,s);
                x+=1;
                if (x==1){
                    fptr = fopen(tok,"w");
                    if (fptr == NULL){
                        perror("Error when making file\nERROR");
                    }
                    else{
                        fclose(fptr);
                        printf("Created file successfully.\n");
                    }
                    tok=0;
                }
            }
            free(tok);
        }
        else if (prefix("rmv",inp)){
            strcpy(dest,inp);
            tok=realloc(tok,50);
            if (tok==NULL){
                perror("MEMORY ERROR WHEN REALLOCATING `tok` IN COMMAND `rmv`\nERROR");
            }
            tok = strtok(inp,s);
            int x = 0;
            while (tok != 0) {
                tok = strtok(0,s);
                x+=1;
                if (x==1){
                    if (remove(tok)==0){
                        printf("File (%s) deleted successfully.\n",tok);
                    }
                    else {
                        perror("Error when deleting file.\nERROR");
                    }
                }
                tok=0;
            }
            free(tok);
        }
        else if (prefix("cat",inp)){
            strcpy(dest,inp);
            tok=realloc(tok,50);
            if (tok==NULL){
                perror("MEMORY ERROR WHEN REALLOCATING `tok` IN COMMAND `cat`\nERROR");
            }
            tok = strtok(inp,s);
            int x = 0;
            while (tok != 0){
                tok = strtok(0,s);
                x+=1;
                if (x==1){
                    fptr = fopen(tok,"r");
                    int tlen = 0;
                    char txt[10000];
                    while (fgets(txt,10000,fptr)){
                        printf("%s",txt);
                        tlen+=strlen(txt);
                    }
                    printf("\nTotal length: %d characters\n",tlen);
                }
                tok=0;
            }
            free(tok);
        }
        //Directory Commands
        else if (prefix("cd",inp)){ //Mostly just handling differences in OS...
            strcpy(dest,inp);
            tok = realloc(tok,50);
            if (tok==NULL){
                perror("MEMORY ERROR WHEN REALLOCATING `tok` IN COMMAND `cd`\nERROR");
            }
            tok = strtok(inp,s);
            int x = 0;
            while (tok!=0){
                tok=strtok(0,s);
                x+=1;
                if (x==1){
                    if (tok==NULL){
                        perror("tok is NULL.");
                    }
                    #ifdef _WIN32
						int z = _chdir(tok);
                        if (z==0){
                            printf("%s | %d\n",tok,x);
                            printf("Changed directory.\n");
                        }
                        else{
                            perror("An error has occured [WIN32]\nERROR");
                            printf("Returned:%d\n",chdir(tok));
                        }
                    #else
                        int z=chdir(tok);
                        if(z==0){
                            printf("%s | %d | %d\n",tok,x,z);
                            printf("Changed directory.\n");
                        }
                        else{
                            perror("An error has occured [LINUX/OTHER]\nERROR");
                            printf("Returned:%d\n",chdir(tok));
                        }
                    #endif
                    tok=0;
                    x=0;
                }
            }
            free(tok);
        }
        else if (prefix ("md",inp)){
            strcpy(dest,inp);
            tok=realloc(tok,50);
            if (tok==NULL){
                perror("MEMORY ERROR WHEN REALLOCATING `tok` IN COMMAND `md`\nERROR");
            }
            tok=strtok(inp,s);
            int x=0;
            while (tok!=0){
                tok=strtok(0,s);
                x+=1;
                if(x==1){
                    #ifdef _WIN32
                        if (_mkdir(tok)==0){
                            printf("Made directory %s\n",tok);
                        }
                        else{
                            perror("Could not make directory.\nERROR");
                        }
                    #else
                        if (mkdir(tok,0755)==0){
                            printf("Made directory %s\n",tok);
                        }
                        else{
                            perror("Could not make directory.\nERROR");
                        }
                    #endif
                    tok=0;
                    x=0;
                }
            }
            free(tok);
        }
        //HTTP Commands!
    else if (strcmp("httptest",inp)==0){
        SOCKTEST();
    }
    else if (prefix("httpconn",inp)){
        strcpy(dest,inp);
		tok=realloc(tok,50);
		if (tok==NULL){
			perror("MEMORY ERROR WHEN REALLOCATING `tok`\nERROR");
		}
		tok=strtok(inp,s);
		int x=0;
		while (tok!=0){
			tok=strtok(0,s);
			x+=1;
			if (x==1){
				curcon=SOCKCONN(tok,80);
			}
		}
    }
    //TO DO: FIX `EXIT CODE: 13`
    else if (prefix("httpsend",inp)){
        strcpy(dest,inp);
        tok=realloc(tok,50);
        if (tok==NULL){
            perror("MEMORY ERROR WHEN REALLOCATING `tok`\nERROR");
        }
        tok = strtok(inp,s);
        int x=0;
        while (tok!=0) {
            x+=1;
            if (x==1) {
                SOCKSEND(tok,curcon);
            }
        }
    }
    goto r;
    }
}
