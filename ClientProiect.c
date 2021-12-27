#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
extern int errno;
int port=2024;
char ms[1500];
void all_nume_utilizatori(char numeUtiliz[100][100],char numecurent[100],nr_utiliz)
{
sqlite3 *db;
  int openBD;
   openBD=sqlite3_open("Offline_Messenger.db", &db);
   int st;
  nr_utiliz=0;
  sqlite3_stmt *stmt;
    char *query = NULL;
      asprintf(&query, "SELECT Nume FROM Utilizatori WHERE Nume NOT LIKE '%s';",numecurent);   	    
    sqlite3_prepare_v2(db, query,-1, &stmt, NULL);
while ( (st = sqlite3_step(stmt)) == SQLITE_ROW) 
       { strcpy(numeUtiliz[nr_utiliz],sqlite3_column_text(stmt,0));nr_utiliz++;}
  --nr_utiliz;
	sqlite3_finalize(stmt);
	free(query); 
}
void afisare_istorie(char *comandaistorie)
{   
 int i=2;
  int k=0,nr_utiliz;
  char numecurent[100];
  char numeUtiliz[100][100];
  //aflam numele utilizatorului ce cere istoria cu toti ceilalti utilizatori
  while(comandaistorie[i]!='\0')
  {numecurent[k]=comandaistorie[i];k++;i++;}
  numecurent[k]='\0';
  all_nume_utilizatori(numeUtiliz,numecurent,nr_utiliz);
   sqlite3 *db;
  int openBD;
   openBD=sqlite3_open("Offline_Messenger.db", &db);
  for(int i=0;i<=nr_utiliz;i++)
  { 
    printf("Istoria conversatiei dintre %s si %s \n",numecurent,numeUtiliz[i]);
    int st;
    sqlite3_stmt *stmt;
    char *query = NULL;
    bzero(msg_trimis,1000);
    bzero(ms,1500); int gasit=0;
    asprintf(&query, "SELECT * FROM Istorie WHERE (Expeditor='%s'AND Destinatar='%s') OR (Expeditor='%s'AND Destinatar='%s');",numecurent,numeUtiliz[i],numeUtiliz[i],numecurent);  
    sqlite3_prepare_v2(db, query, strlen(query), &stmt, NULL);
    while ( (st = sqlite3_step(stmt)) == SQLITE_ROW) 
    {gasit++;
     if(gasit==1)
     {printf("Expeditor    Destinatar    Data si ora          Mesajul \n");}
     sprintf(ms,"%s  %s  %s : %s \n",sqlite3_column_text(rez, 0),sqlite3_column_text(rez, 1),sqlite3_column_text(rez, 2),sqlite3_column_text(rez, 3));
     printf("%s",ms);
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
 char numecurent[100],nume2[100];
 while(comandaistorie[i]!=' ')
 {numecurent[k]=comandaistorie[i];k++;i++;}
numecurent[k]='\0';
 k=0;i++;
  while(comandaistorie[i]!='\0')
 {nume2[k]=comandaistorie[i];k++;i++;}
  nume2[k]='\0';
 sqlite3 *db;
  int openBD;
   openBD=sqlite3_open("Offline_Messenger.db", &db);
    printf("Istoria conversatiei dintre %s si %s \n",numecurent,nume2);
    int st;
    sqlite3_stmt *stmt;
    char *query = NULL;
    bzero(msg_trimis,1000);
    bzero(ms,1500); int gasit=0;
    asprintf(&query, "SELECT * FROM Istorie WHERE (Expeditor='%s'AND Destinatar='%s') OR (Expeditor='%s'AND Destinatar='%s');",numecurent,nume2,nume2,numecurent);  
    sqlite3_prepare_v2(db, query, strlen(query), &stmt, NULL);
    while ( (st = sqlite3_step(stmt)) == SQLITE_ROW) 
    {gasit++;
     if(gasit==1)
     {printf("Expeditor    Destinatar    Data si ora          Mesajul \n");}
     sprintf(ms,"%s  %s  %s : %s \n",sqlite3_column_text(rez, 0),sqlite3_column_text(rez, 1),sqlite3_column_text(rez, 2),sqlite3_column_text(rez, 3));
     printf("%s",ms);
     bzero(ms,1500);
    }//terminare while
    if(gasit==0)
      printf("Nu exista mesaje!");
    printf("\n");
    sqlite3_finalize(stmt);
    free(query);
}//terminare afisare_istorie_cu_utilizator
int main (int argc, char *argv[])
{
  int sd;			
  struct sockaddr_in server;
  char msg_trimis[1000];
  char msg_primit[1000];
  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("Error at socket().\n");
      return errno;
    }
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr("127.0.0.1");
  server.sin_port = htons (port);
  if (connect (sd, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1)
    {
      perror ("-->Error at connect().\n");
      return errno;
    }

  printf("Aplicatia Offline Messenger, pentru mai multe detalii tastati Ajutor\n ");
  fflush (stdout); 
  while(1) 
  {
    bzero (msg_primit, 1000);
    bzero(msg_trimis,1000);
    fflush(stdout);
    read(0,msg_trimis,1000);    
    msg_trimis[strlen(msg_trimis)-1]=0;
     if (write (sd, msg_trimis,sizeof(msg_trimis)) <= 0)
      {
      perror ("-->Error at write() to server.\n");
      return errno;
      }
      if(strcmp(msg_trimis,"EXIT")==0) return 0;
      if (read (sd, msg_primit, sizeof(msg_primit)) < 0)
      {
       perror ("-->Error at read() from server.\n");
      return errno;
      }
     else
     {if(strstr(msg_primit,"!1")!=0&&msg_primit[0]==!)
          afisare_istorie(msg_primit);
       else
       if(strstr(msg_primit,"!2")!=0&&msg_primit[0]==!)
          afisare_istorie_cu_utilizator(msg_primit);
      else
        printf ("-->: %s\n", msg_primit);
     }
  }
  close (sd);
  return 0;
}
