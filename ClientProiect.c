#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <sqlite3.h>
#include <fcntl.h> 
#include <assert.h>
#include <signal.h>
extern int errno;
/*void  SIGQUIT_handler(int sig)
  {
        exit(10);
  }*/
int port=2024;
char ms[1500];
int nr_utiliz;
char numecurent[100];
char numeUtiliz[100][100];
void all_nume_utilizatori(char numeUtiliz[100][100],char numecurent[100])
{
sqlite3 *db;
  int openBD;
   openBD=sqlite3_open("Offline_Messenger.db", &db);
   int st;
  int k=0;
  sqlite3_stmt *stmt;
    char *query = NULL;
      asprintf(&query, "SELECT Nume FROM Utilizatori WHERE Nume NOT LIKE '%s';",numecurent);   	    
    sqlite3_prepare_v2(db, query,-1, &stmt, NULL);
while ( (st = sqlite3_step(stmt)) == SQLITE_ROW) 
       { strcpy(numeUtiliz[k],sqlite3_column_text(stmt,0));k++;}
  nr_utiliz=k;
	sqlite3_finalize(stmt);
	free(query); 
}
void afisare_istorie(char *comandaistorie)
{   
 int i=2;
  int k=0;
  //aflam numele utilizatorului ce cere istoria cu toti ceilalti utilizatori
  while(comandaistorie[i]!='\0')
  {numecurent[k]=comandaistorie[i];k++;i++;}
  numecurent[k]='\0';
  all_nume_utilizatori(numeUtiliz,numecurent);
   sqlite3 *db;
  int openBD;
   openBD=sqlite3_open("Offline_Messenger.db", &db);
  for(i=0;i<nr_utiliz;i++)
  { 
    printf("Istoria conversatiei dintre %s si %s \n",numecurent,numeUtiliz[i]);
    int st;
    sqlite3_stmt *stmt;
    char *query = NULL;
    //bzero(msg_trimis,1000);
    bzero(ms,1500); int gasit=0;
    asprintf(&query, "SELECT * FROM Istorie WHERE (Expeditor='%s'AND Destinatar='%s') OR (Expeditor='%s'AND Destinatar='%s');",numecurent,numeUtiliz[i],numeUtiliz[i],numecurent);  
    sqlite3_prepare_v2(db, query, strlen(query), &stmt, NULL);
    while ( (st = sqlite3_step(stmt)) == SQLITE_ROW) 
    {gasit++;
     if(gasit==1)
     {printf("Expeditor    Destinatar    Data si ora          Mesajul \n");}
     sprintf(ms,"%s  %s  %s: %s ",sqlite3_column_text(stmt, 0),sqlite3_column_text(stmt, 1),sqlite3_column_text(stmt, 2),sqlite3_column_text(stmt, 3));
     printf("%s \n",ms);
     bzero(ms,1500);
    }//terminare while
    if(gasit==0)
      printf("Nu exista mesaje!");
    printf("\n");
    sqlite3_finalize(stmt);
    free(query);
   }//terminare 'for'
}//terminare afisare_istorie

void afisare_istorie_cu_utilizator(char *comandaistorie)
{int i=2,k=0;
 char nume1[100],nume2[100];
 while(comandaistorie[i]!=' ')
 {nume1[k]=comandaistorie[i];k++;i++;}
nume1[k]='\0';
 k=0;i++;
  while(comandaistorie[i]!='\0')
 {nume2[k]=comandaistorie[i];k++;i++;}
  nume2[k]='\0';
 sqlite3 *db;
  int openBD;
   openBD=sqlite3_open("Offline_Messenger.db", &db);
    printf("Istoria conversatiei dintre %s si %s \n",nume1,nume2);
    int st;
    sqlite3_stmt *stmt;
    char *query = NULL;
    //bzero(msg_trimis,1000);
    bzero(ms,1500); int gasit=0;
    asprintf(&query, "SELECT * FROM Istorie WHERE (Expeditor='%s'AND Destinatar='%s') OR (Expeditor='%s'AND Destinatar='%s');",nume1,nume2,nume2,nume1);  
    sqlite3_prepare_v2(db, query, strlen(query), &stmt, NULL);
    while ( (st = sqlite3_step(stmt)) == SQLITE_ROW) 
    {gasit++;
     if(gasit==1)
     {printf("Expeditor    Destinatar    Data si ora          Mesajul \n");}
     sprintf(ms,"%s  %s  %s : %s ",sqlite3_column_text(stmt, 0),sqlite3_column_text(stmt, 1),sqlite3_column_text(stmt, 2),sqlite3_column_text(stmt, 3));
     printf("%s \n",ms);
     bzero(ms,1500);
    }//terminare while
    if(gasit==0)
      printf("Nu exista mesaje!");
    printf("\n");
    sqlite3_finalize(stmt);
    free(query);
}//terminare afisare_istorie_cu_utilizator

void afisare_mesaje_necitite(char* comandanecitite)
{int i=1,k=0;
 char nume[100];
 while(comandanecitite[i]!='\0')
 {nume[k]=comandanecitite[i];k++;i++;}
nume[k]='\0';
 sqlite3 *db;
  int openBD;
   openBD=sqlite3_open("Offline_Messenger.db", &db);
    printf("Mesajele necitite\n");
    int st;
    sqlite3_stmt *stmt;
    char *query = NULL;
    //bzero(msg_trimis,1000);
    bzero(ms,1500); int gasit=0;
    asprintf(&query, "SELECT * FROM Mesaje_necitite WHERE Destinatar='%s';",nume);  
    sqlite3_prepare_v2(db, query, strlen(query), &stmt, NULL);
    while ( (st = sqlite3_step(stmt)) == SQLITE_ROW) 
    {//gasit++;
     printf("Expeditor    Data si ora  \n");
     sprintf(ms,"%s  %s ",sqlite3_column_text(stmt, 0),sqlite3_column_text(stmt, 3));
     printf("%s \n",ms);
     bzero(ms,1500);
     printf("Mesajul: ");
     sprintf(ms,"%s",sqlite3_column_text(stmt, 2));
     printf("%s \n",ms);
     bzero(ms,1500);
    }//terminare while
    //if(gasit==0)
      //printf("Nu exista mesaje necitite\n!");
    char *querydelete=NULL;
    sqlite3_stmt *stmtdelete;
    asprintf(&querydelete,"DELETE FROM Mesaje_necitite WHERE Destinatar='%s';",nume);
    sqlite3_prepare_v2(db, querydelete, strlen(querydelete), &stmtdelete, NULL);
    int stdelete=sqlite3_step(stmtdelete);
    if(stdelete!=SQLITE_DONE)
    {
      printf("Eroare la stergerea mesajelor necitite: %s \n",sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);
    sqlite3_finalize(stmtdelete);
    free(query);
    free(querydelete);

}//terminare afisare_mesaje_necitite


int main (int argc, char *argv[])
{
  int sd;			
  struct sockaddr_in server;
  char msg_trimis[1000];
  char msg_primit[1000];
  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("Error at socket().\n");return errno;
    }

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr("127.0.0.1");
  server.sin_port = htons (port);
  if (connect (sd, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1)
    {
      perror ("-->Error at connect().\n");return errno;
    }

  printf("Aplicatia Offline Messenger, pentru mai multe detalii tastati Ajutor\n ");
  fflush (stdout); 

  //set_nonblock(sd);
  while(1) 
  {
    bzero (msg_primit, 1000);
    bzero(msg_trimis,1000);
    fflush(stdout);
    //set_nonblock(0);

    if (read(0,msg_trimis,1000) < 0) /*{
    if (EAGAIN == errno) {
        sleep(1);
    } else */
    {perror ("-->Error at read() from console.\n");
      return errno;}
//} 
    msg_trimis[strlen(msg_trimis)-1]=0;
     if (write (sd, msg_trimis,sizeof(msg_trimis)) <= 0)
        /*if (EAGAIN == errno) 
        sleep(1);
     else */
      {perror ("-->Error at write() to server.\n");
      return errno;
      }
      if(strcmp(msg_trimis,"EXIT")==0) return 0;
      if (read (sd, msg_primit, sizeof(msg_primit)) < 0)
         /*{if (EAGAIN == errno) 
        sleep(1);
     else */
      {perror ("-->Error at read() from server.\n");
      return errno;
      }//}
     else
     {if(strstr(msg_primit,"!1")!=0&&msg_primit[0]=='!')
          afisare_istorie(msg_primit);
       else
       if(strstr(msg_primit,"!2")!=0&&msg_primit[0]=='!')
          afisare_istorie_cu_utilizator(msg_primit);
        else
          if(msg_primit[0]=='+')
            afisare_mesaje_necitite(msg_primit);
      else
        printf ("-->: %s\n", msg_primit);
     }
  }
  close (sd);
  return 0;
}
  /*
  char msg_trimis[1000];
  char msg_primit[1000];
  int sd;
  struct sockaddr_in server;
  //char msg[1000];
  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("[client] Eroare la socket().\n");
      return errno;
    }
  

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr("127.0.0.1");
  server.sin_port = htons (port);

  if (connect (sd, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1)
    {
      perror ("[client]Eroare la connect().\n");
      return errno;
    }

  int fiu;
  ,pip[2];
  pipe(pip);
  int flags=fcntl(pip[0],F_GETFL,0);
  flags=flags|O_NONBLOCK;
  fcntl(pip[0],F_SETFL,flags);

  if(fiu=fork())
  {
    //close(pip[1]);
    //char mesaj[1000],destinatari[1000],numeDest[20],comanda[20],comanda2[20];
    while(1)
      {
      //bzero(destinatari,1000);
      bzero(msg_trimis,1000);
      scanf ("%s", msg_trimis);

      //read(pip[0],destinatari,1000);
    
    if(strlen(msg_trimis)>0)
      if (write (sd,msg_trimis, 1000) <= 0)
          {
              perror ("[client]Eroare la write() spre server.\n");
              return errno;
            }
      if(strstr(msg_trimis,"quit")!=NULL)
        {
        close(sd);
        kill(fiu,SIGQUIT);
        exit(1);
        }

      }

    }
  else
    {
    //close(pip[0]);

    while(1)
      {
      bzero(msg_primit,1000);
        if (read (sd, msg_primit, 1000) < 0)
          {
              perror ("[client]Eroare la read() de la server.\n");
              return errno;
            }
            else
               {if(strstr(msg_primit,"!1")!=0&&msg_primit[0]=='!')
          afisare_istorie(msg_primit);
       else
       if(strstr(msg_primit,"!2")!=0&&msg_primit[0]=='!')
          afisare_istorie_cu_utilizator(msg_primit);
        else
          if(msg_primit[0]=='+')
            afisare_mesaje_necitite(msg_primit);
      else
        printf ("-->: %s\n", msg_primit);
     }
        //printf ("%s\n", msg_primit);
      }
    }
  close (sd);
}*/
