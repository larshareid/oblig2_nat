#include <arpa/inet.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <limits.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/sendfile.h>
#include <time.h>
#include <netdb.h>
#include <sys/queue.h>
#define LOKAL_PORT 80
#define BAK_LOGG 10 // StÃƒÂ¸rrelse pÃƒÂ¥ for kÃƒÂ¸ ventende forespÃƒÂ¸rsler 

//function declaration
int getContentLength(FILE* input_file);

//function
int getContentLength(FILE* input_file)
{
    int count = 0;
    int ch;

    while (1)
    {
        ch = fgetc(input_file);
        if (ch == EOF)
            break;
        count++;
    }

    return (count);
}

char* timeStamp()
{
        time_t currentTime;
        char *timeString;

        currentTime = time(NULL);
        timeString = ctime(&currentTime);
        timeString[strcspn(timeString, "\n")] = 0;

        return timeString;
}

char* timeStamp();

char* TypeEnd[]={"php", "html", "plain", "png", "svg", "xml", "xslt+xml", "css", "json", NULL};
char* TypeHead[]={ "text/php", "text/html", "text/plain", "image/png", "image/svg", "application/xml", "application/xslt+xml", "text/css", "application/json", NULL};


void printHeader(char ending[] , int filValid, int fileExist){
char pHolder [200];
int i = 0;
while(TypeEnd[i]){
    if(strcmp(TypeEnd[i],ending) == 0){
      strcpy(pHolder, TypeHead[i]);
      break;
      }
      i++;
    }
  if(filValid == 1 && fileExist == 1){
    printf("HTTP/1.1 200 OK\nContent-Type: %s\nContent-Length: 270000\n\n", pHolder);
}else if(fileExist == 0 && filValid == 1){
  printf("HTTP/1.1 404 Not Found\nContent-Type: text/plain\nContent-Length: 18\n\n");
  printf("404 File not found");
  }else if(filValid == 0){
  printf("HTTP/1.1 415 Unsupported Media Type\nContent-Type: %s\nContent-Length: 270000\n\n", pHolder);
  printf("415 Unsupported Media Type");
  }
}

int main(void) {
char cwd[PATH_MAX];
char buf[2048];
int fdimg;
char get[2048];

        /* Our process ID and Session ID */
        pid_t pid, sid;

	struct sockaddr_in  lok_adr;
 	int sd, ny_sd;

	printf("Main PID: %d , SID: %d \n", getpid(), getsid(0));
        /* Fork off the parent process */
        pid = fork();
        if (pid < 0) {
                exit(EXIT_FAILURE);
        }
        /* If we got a good PID, then
           we can exit the parent process. */
        if (pid > 0) {
                signal(SIGCHLD,SIG_IGN); 
                exit(EXIT_SUCCESS);
        }

        /* Change the file mode mask */
        umask(0);
                
        /* Open any logs here */        
                
        /* Create a new SID for the child process */
        sid = setsid();
        if (sid < 0) {
                /* Log the failure */
                exit(EXIT_FAILURE);
        }
	printf("Child PID: %d , SID: %d \n", getpid(), getsid(0));


	if ((chdir("/var/www/")) < 0) {
		exit(EXIT_FAILURE);
	}


	chroot("/var/www/");

   if (getcwd(cwd, sizeof(cwd)) != NULL) {
       printf("Current working dir: %s\n", cwd);
   } else {
       perror("getcwd() error");
   }

	// Setter opp socket-strukturen
  sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  // For at operativsystemet ikke skal holde porten reservert etter tjenerens dÃƒÂ¸d
  setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int));

  // Initierer lokal adresse
  lok_adr.sin_family      = AF_INET;
  lok_adr.sin_port        = htons((u_short)LOKAL_PORT);
  lok_adr.sin_addr.s_addr = htonl(         INADDR_ANY);

  // Kobler sammen socket og lokal adresse
  if ( 0==bind(sd, (struct sockaddr *)&lok_adr, sizeof(lok_adr)) )
    fprintf(stderr, "%s : Prosess %d er knyttet til port %d.\n", timeStamp(), getpid(), LOKAL_PORT);
  else
    exit(1);

	gid_t gid = 65534;
	uid_t uid = 65534;

	setgid(gid);
	setuid(uid);

	close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);

  // Venter pÃƒÂ¥ forespÃƒÂ¸rsel om forbindelse
  listen(sd, BAK_LOGG);
  while(1){

    // Aksepterer mottatt forespÃƒÂ¸rsel
    ny_sd = accept(sd, NULL, NULL);

    if(0==fork()) {

      dup2(ny_sd, 1); // redirigerer socket til standard utgang

	memset(buf, 0, 2048);
	read(ny_sd, buf, 2047);
	//printf("%s\n", buf);

int i, v;
char singleLine [200];
char fileEnding [200];
char placeHolder [200];
char *pch;
char mainPage[200] = "index.html";

//Lagrer singleLineen sendt fra klienten
pch = strtok(buf," ");

while(pch != NULL)
  {
    if(i==1)
      strcpy(singleLine, pch);

    pch = strtok(NULL, " ");
    i++;
  }

//Lagrer filtypen fra singleLineen fra klienten
strcpy(placeHolder, singleLine);

pch = strtok(placeHolder, " .");
while(pch != NULL)
  {
    if(v==1){
	strcpy(fileEnding, pch);}

   pch = strtok(NULL, " .");
  v++;
}

//printf("\nFileEnding is %s, singleLine is %s\n", fileEnding, singleLine);

//Sjekker om filtypen stÃƒÂ¸ttes, og setter variabel til 1 om den er stÃƒÂ¸ttet
char* filesAccept[]={ "asis", "html", "png", "txt", "svg", "css", "json", "plain", "xml", "xsly+xml", NULL};
int filValid = 0;
i = 0;
while(filesAccept[i]) {
	if(strcmp(filesAccept[i], fileEnding) == 0){
		filValid = 1;}
	i++;
	}

int fileExist = 1;
//printf("Variable filValid is %i\n", filValid);
if(filValid == 1 || strcmp(singleLine, "/") == 0){
   if(strcmp(singleLine, "/") == 0){
	fdimg = open(mainPage, O_RDONLY);
 }else{
 fdimg = open(singleLine, O_RDONLY);}
}

if(fdimg == -1 ){
  fileExist = 0;}
  
char* pptr = &singleLine;
FILE * fileptr;
fileptr=fopen(pptr, "r");
int count = getContentLength(fileptr);
fclose(pptr);

  
printHeader(fileEnding, filValid, fileExist);

printf("fdimg is : %d", count);


//Sjekker om fil eksisterer, og dermed ÃƒÂ¥pner og sender. Gir riktig feilmeldinger
//om filtype ikke stÃƒÂ¸ttes eller ikke finnes.
//printf("HTTP/1.1 200 OK\nContent-Type: text/html\n\n");

  sendfile(ny_sd, fdimg, NULL, 270000);
  close(fdimg);
  printf("\n");

      fflush(stdout);

      // SÃƒÂ¸rger for ÃƒÂ¥ stenge socket for skriving og lesing
      // NB! FrigjÃƒÂ¸r ingen plass i fildeskriptortabellen
      shutdown(ny_sd, SHUT_RDWR);
      exit(0);

    }

else {
//Fjerner zombiene som socketene kan danne.
      signal(SIGCHLD,SIG_IGN);
      close(ny_sd);
    }
  }
  return 0;
}

