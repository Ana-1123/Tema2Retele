#include <sys/socket.h>
#include<sys/types.h>
#include <netinet/in.h> 
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <sqlite3.h>
#include<signal.h>
#include <assert.h>
extern int errno;

int port;
char ms[1500];
int nr_utiliz;
char numecurent[100];
char numeUtiliz[100][100];
void  SIGQUIT_handler(int signal)
  {
        exit(0);
  }
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
    printf("V-ati autentificat cu succes!\nMesajele necitite:\n");
    int st;
    sqlite3_stmt *stmt;
    char *query = NULL;
    bzero(ms,1500);
    asprintf(&query, "SELECT * FROM Mesaje_necitite WHERE Destinatar='%s';",nume);  
    sqlite3_prepare_v2(db, query, strlen(query), &stmt, NULL);
    while ( (st = sqlite3_step(stmt)) == SQLITE_ROW) 
    {
     printf("Expeditor    Data si ora  \n");
     sprintf(ms,"%s  %s ",sqlite3_column_text(stmt, 0),sqlite3_column_text(stmt, 3));
     printf("%s ",ms);
     bzero(ms,1500);
     printf("Mesajul: ");
     sprintf(ms,"%s",sqlite3_column_text(stmt, 2));
     printf("%s \n",ms);
     bzero(ms,1500);
    }//terminare while
   
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
{ port=2728;
  int sd;
  struct sockaddr_in server;
  char msg_primit[1000];
  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror (" Eroare la socket().\n");
      return errno;
    }
  

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr("127.0.0.1");
  server.sin_port = htons (port);

  if (connect (sd, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1)
    {
      perror ("Eroare la connect().\n");
      return errno;
    }
 printf("Aplicatia Offline Messenger, pentru mai multe detalii tastati Ajutor\n ");
  fflush (stdout);

  int pid;

  pid=fork();

  if(pid==0)
  {//procesul copil

    char msg_trimis[1000];
    while(1)
      {
      bzero(msg_trimis,1000);
      scanf ("%[^\n]%*c", msg_trimis);

    if(strlen(msg_trimis)>0)
      {if (write (sd, msg_trimis, 1000) <= 0)
          {
              perror ("Eroare la write() spre server.\n");
              return errno;
            }}

      }

    }
  else
    {

    while(1)
      {
      bzero(msg_primit,1000);
        if (read (sd, msg_primit, 1000) < 0)
          {
              perror ("Eroare la read() de la server.\n");
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
       if(strstr(msg_primit,"Deconectat")!=NULL)
                {printf("Deconectat cu succes!\n");
                  close(sd);
                  kill(pid,SIGQUIT); exit(1);
                  }
      else
        if(strlen(msg_primit)>0)
        printf ("-->: %s\n", msg_primit);}
     }     
   }
    }
