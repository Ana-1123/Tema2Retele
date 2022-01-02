#include <sys/time.h>
#include <arpa/inet.h>
#include<ctype.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>
#define PORT 2728
 struct RaspAut
{
  char ultimmesaj[1000];
  char numexp[100];
  int dexpeditor;//descriptorul celui care a primit mesaj si vrea sa trimita raspuns automat
  int ddestinatar;
}utilizatoriRA[100];
int nrutilizatoriRA,nrlogati;
 char nume_dest[100],nume_exp[100],mesaj[1000],raspRA[1000];
 int destd,logati[100];


   int vf_nume_utilizator(char* nume)
{

sqlite3 *db;
  int openBD;
   openBD=sqlite3_open("Offline_Messenger.db", &db);
   int st;
int gasit=0;
  sqlite3_stmt *stmt;
    char *query = NULL;
      asprintf(&query, "SELECT Nume FROM Utilizatori WHERE Nume='%s';",nume);         
    sqlite3_prepare_v2(db, query,-1, &stmt, NULL);
while ( (st = sqlite3_step(stmt)) == SQLITE_ROW) 
       { gasit++;}
  sqlite3_finalize(stmt);
  free(query); 
  sqlite3_close(db);
  if(gasit==1)
     return 1;
return 0;
}

   int vf_parola(char* parola,char* nume_utilizator)    
{
  sqlite3 *db;
  int openBD;
   openBD=sqlite3_open("Offline_Messenger.db", &db);
   int st;  
int gasit=0;
  sqlite3_stmt *stmt;
    char *query = NULL;
      asprintf(&query, "SELECT Parola FROM Utilizatori WHERE Parola='%s' AND Nume='%s';",parola,nume_utilizator);         
    sqlite3_prepare_v2(db, query,-1, &stmt, NULL);
while ( (st = sqlite3_step(stmt)) == SQLITE_ROW) 
       { gasit++;}
  sqlite3_finalize(stmt);
  free(query); 
  sqlite3_close(db);
  if(gasit==1)
     return 1;
return 0;
}

int actualizare_stare_conectat(char* nume_utilizator, int cd)
{ sqlite3 *db;
  int openBD;
   openBD=sqlite3_open("Offline_Messenger.db", &db);
   int st;  
  sqlite3_stmt *stmt;
    char *query = NULL;
      asprintf(&query, "UPDATE Utilizatori SET Stare=1, Descriptor='%d' WHERE Nume='%s';",cd,nume_utilizator);           
    sqlite3_prepare_v2(db, query,-1, &stmt, NULL);
      st = sqlite3_step(stmt);
       if (st != SQLITE_DONE) {
             printf("Eroare la actualizare: %s\n", sqlite3_errmsg(db)); return 0;
              }
        sqlite3_finalize(stmt);
    free(query); 
    sqlite3_close(db);
     return 1;
}
int actualizare_stare_deconectat(int cd)
{ sqlite3 *db;
  int openBD;
   openBD=sqlite3_open("Offline_Messenger.db", &db);
   int st;  
  sqlite3_stmt *stmt;
    char *query = NULL;
      asprintf(&query, "UPDATE Utilizatori SET Stare=0, Descriptor=NULL WHERE Descriptor='%d';",cd);           
    sqlite3_prepare_v2(db, query,-1, &stmt, NULL);
      st = sqlite3_step(stmt);
       if (st != SQLITE_DONE) {
             printf("Eroare la actualizare: %s\n", sqlite3_errmsg(db)); return 0;
              }
        sqlite3_finalize(stmt);
    free(query); 
    sqlite3_close(db);
     return 1;
}

  int inregistrare(char* nume,char* parola)
{
sqlite3 *db;
   int openBD;
   openBD=sqlite3_open("Offline_Messenger.db", &db);
      int st;
  sqlite3_stmt *stmt;
    char *query = NULL; 
    int stare=0;
       asprintf(&query, "INSERT INTO Utilizatori (Nume,Parola,Stare) VALUES('%s','%s','%d');",nume,parola,stare);         
    sqlite3_prepare_v2(db, query,-1, &stmt, NULL);
 if( (st = sqlite3_step(stmt)) != SQLITE_DONE) 
     {printf("Eroare la inserarea datelor in baza de date: %s\n", sqlite3_errmsg(db));
     return 0;} 
     sqlite3_finalize(stmt);
        free(query); 
        sqlite3_close(db);
return 1;
}

  int adaugare_Istorie(char* NumeExpeditor,char* NumeDestinatar,char* text)
{   sqlite3 *db;
     char *query = NULL;
char *data_time;
time_t t;
time(&t);
data_time=ctime(&t);
     int rc;
      sqlite3_stmt *stmt;
  int openBD;
   openBD=sqlite3_open("Offline_Messenger.db", &db);
    asprintf(&query, "INSERT INTO Istorie (Expeditor,Destinatar,Data_time,Text) VALUES ('%s','%s','%s','%s');",NumeExpeditor,NumeDestinatar,data_time,text);  
     sqlite3_prepare_v2(db, query, strlen(query), &stmt, NULL);
      rc = sqlite3_step(stmt);
         if (rc != SQLITE_DONE) {
             printf("Eroare la inserarea datelor in baza de date: %s\n", sqlite3_errmsg(db)); return 0;
              }
        sqlite3_finalize(stmt);
        free(query); 
        sqlite3_close(db);
        return 1; 
}

void trimitere(char* comanda)
{int i,k=0;
  if(comanda[9]==' ')
  {i=10;
    while(comanda[i]!=' ')
    {
        nume_dest[k]=comanda[i];k++;i++;
    }
    nume_dest[k]='\0';
    printf("Nume dest %s",nume_dest);
    k=0;
    for(int j=i+1;j<strlen(comanda);j++)
    {
    mesaj[k]=comanda[j];k++;
    }  
    mesaj[k]='\0';
  }
}
int aflare_nume_expeditor(int cd)
{
  sqlite3 *db;
  int openBD;
   openBD=sqlite3_open("Offline_Messenger.db", &db);
   int st;
int gasit=0;
  sqlite3_stmt *stmt;
    char *query = NULL;
      asprintf(&query, "SELECT Nume FROM Utilizatori WHERE Descriptor='%d';",cd);         
    sqlite3_prepare_v2(db, query,-1, &stmt, NULL);
while ( (st = sqlite3_step(stmt)) == SQLITE_ROW) 
       { gasit++;strcpy(nume_exp,sqlite3_column_text(stmt,0));}
     printf("Nume expeditor %s \n", nume_exp);
  sqlite3_finalize(stmt);
  free(query); 
  sqlite3_close(db);
  if(gasit==1)
     return 1;
return 0;
}
int aflare_descriptor_dest(char nume_dest[100])
{
  sqlite3 *db;
  int openBD;
   openBD=sqlite3_open("Offline_Messenger.db", &db);
   int st;
int gasit=0;
  sqlite3_stmt *stmt;
    char *query = NULL;
      asprintf(&query, "SELECT * FROM Utilizatori WHERE Nume='%s';",nume_dest);         
    sqlite3_prepare_v2(db, query,-1, &stmt, NULL);
while ( (st = sqlite3_step(stmt)) == SQLITE_ROW) 
       { gasit++;
          destd=(sqlite3_column_int(stmt,3)); 
         //printf("Descriptor dest %d\n",sqlite3_column_int(stmt,3));
       }
       if(st!=SQLITE_DONE)
        printf("Eroare la aflare descriptor dest");
     printf("Descriptor destinatar %d \n",destd);
  sqlite3_finalize(stmt);
  free(query);
  sqlite3_close(db); 
  if(gasit==1)
     return 1;
return 0;
}
int aflare_stare_dest(char nume_dest[100])
{  sqlite3 *db;
  int openBD;
   openBD=sqlite3_open("Offline_Messenger.db", &db);
   int st;
int stare;
  sqlite3_stmt *stmt;
    char *query = NULL;
      asprintf(&query, "SELECT * FROM Utilizatori WHERE Nume='%s';",nume_dest);         
    sqlite3_prepare_v2(db, query,-1, &stmt, NULL);
while ( (st = sqlite3_step(stmt)) == SQLITE_ROW) 
       { stare=(sqlite3_column_int(stmt,2)); }
     printf("Stare destinatar %d \n",stare);
  sqlite3_finalize(stmt);
  free(query); 
  sqlite3_close(db);
  if(stare==1)
     return 1;
return 0;

}
int salvare_mesaje_necitite(char* NumeExpeditor,char* NumeDestinatar,char* text)
{ char *query = NULL;
  char *data_time;
  sqlite3 *db;
time_t t;
time(&t);
data_time=ctime(&t);
     int rc;
      sqlite3_stmt *stmt;
  int openBD;
   openBD=sqlite3_open("Offline_Messenger.db", &db);
    asprintf(&query, "INSERT INTO Mesaje_necitite (Expeditor,Destinatar,Continut,Data_time) VALUES ('%s','%s','%s','%s');",NumeExpeditor,NumeDestinatar,text,data_time);  
     sqlite3_prepare_v2(db, query, strlen(query), &stmt, NULL);
      rc = sqlite3_step(stmt);
         if (rc != SQLITE_DONE) {
             printf("Eroare la inserarea datelor in baza de date: %s\n", sqlite3_errmsg(db)); return 0;
              }
        sqlite3_finalize(stmt);
        free(query); 
        sqlite3_close(db);
        return 1; 
}
void istorie_nume2(char *comanda)
{int i,k=0;
  if(comanda[7]==' ')
  {i=8;
    while(comanda[i]!='\0')
    {
        nume_dest[k]=comanda[i];k++;i++;
    }
    nume_dest[k]='\0';
    printf("Nume dest istorie %s",nume_dest);
  }
}
int exista_mesaje_necitite(char NumeDestinatar[100])
{
  sqlite3 *db;
  int openBD;
   openBD=sqlite3_open("Offline_Messenger.db", &db);
   int st;
int gasit=0;
  sqlite3_stmt *stmt;
    char *query = NULL;
      asprintf(&query, "SELECT * FROM Mesaje_necitite WHERE Destinatar='%s';",NumeDestinatar);         
    sqlite3_prepare_v2(db, query,-1, &stmt, NULL);
while ( (st = sqlite3_step(stmt)) == SQLITE_ROW) 
       { gasit++;}
  sqlite3_finalize(stmt);
  free(query); 
  sqlite3_close(db);
  if(gasit>=1)
     return 1;
return 0;
}
int vf_mesaj_specific(char* text)
{
   sqlite3 *db;
  int openBD;
   openBD=sqlite3_open("Offline_Messenger.db", &db);
   int st;
int gasit=0;
  sqlite3_stmt *stmt;
    char *query = NULL;
      asprintf(&query, "SELECT * FROM Mesaje_specifice WHERE Text_mesaj='%s';",text);         
    sqlite3_prepare_v2(db, query,-1, &stmt, NULL);
while ( (st = sqlite3_step(stmt)) == SQLITE_ROW) 
       { gasit++;strcpy(raspRA,sqlite3_column_text(stmt,2));}
     printf("Mesaj specific %s %d",raspRA,gasit);
  sqlite3_finalize(stmt);
  free(query); 
  sqlite3_close(db);
  if(gasit==1)
     return 1;
return 0;
}
int vf_logare(int cd)
{
  for(int i=1;i<=nrlogati;i++)
    if(logati[i]==cd)
      {printf("Este logat\n");return 1;}
   return 0;
}
/* functie de convertire a adresei IP a clientului in sir de caractere */
char * conv_addr (struct sockaddr_in address)
{
  static char str[25];
  char port[7];

  /* adresa IP a clientului */
  strcpy (str, inet_ntoa (address.sin_addr)); 
  /* portul utilizat de client */
  bzero (port, 7);
  sprintf (port, ":%d", ntohs (address.sin_port));  
  strcat (str, port);
  return (str);
}

int main ()
{//srand(time(NULL));
  nrlogati=0;nrutilizatoriRA=0;
  struct sockaddr_in server;  /* structurile pentru server si clienti */
  struct sockaddr_in from;
  fd_set readfds;   /* multimea descriptorilor de citire */
  fd_set actfds;    /* multimea descriptorilor activi */
  struct timeval tv;    /* structura de timp pentru select() */
  int sd, client;   /* descriptori de socket */
  int optval=1;       /* optiune folosita pentru setsockopt()*/ 
  int fd;     /* descriptor folosit pentru 
           parcurgerea listelor de descriptori */
  int nfds;     /* numarul maxim de descriptori */
  int len;      /* lungimea structurii sockaddr_in */

  /* creare socket */
  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("Eroare la socket().\n");
      return errno;
    }

  /*setam pentru socket optiunea SO_REUSEADDR */ 
  setsockopt(sd, SOL_SOCKET, SO_REUSEADDR,&optval,sizeof(optval));

  /* pregatim structurile de date */
  bzero (&server, sizeof (server));

  /* umplem structura folosita de server */
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = htonl (INADDR_ANY);
  server.sin_port = htons (PORT);

  /* atasam socketul */
  if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
    {
      perror ("[server] Eroare la bind().\n");
      return errno;
    }

  /* punem serverul sa asculte daca vin clienti sa se conecteze */
  if (listen (sd, 5) == -1)
    {
      perror ("[server] Eroare la listen().\n");
      return errno;
    }
  
  /* completam multimea de descriptori de citire */
  FD_ZERO (&actfds);    /* initial, multimea este vida */
  FD_SET (sd, &actfds);   /* includem in multime socketul creat */

  tv.tv_sec = 1;    /* se va astepta un timp de 1 sec. */
  tv.tv_usec = 0;
  
  /* valoarea maxima a descriptorilor folositi */
  nfds = sd;

  printf ("Asteptam clienti la portul %d...\n", PORT);
  fflush (stdout);
        
  /* servim in mod concurent clientii... */
  while (1)
    {
      /* ajustam multimea descriptorilor activi (efectiv utilizati) */
      bcopy ((char *) &actfds, (char *) &readfds, sizeof (readfds));

      /* apelul select() */
      if (select (nfds+1, &readfds, NULL, NULL, &tv) < 0)
  {
    perror ("Eroare la select().\n");
    return errno;
  }
      /* vedem daca e pregatit socketul pentru a-i accepta pe clienti */
      if (FD_ISSET (sd, &readfds))
  {
    /* pregatirea structurii client */
    len = sizeof (from);
    bzero (&from, sizeof (from));

    /* a venit un client, acceptam conexiunea */
    client = accept (sd, (struct sockaddr *) &from, &len);

    /* eroare la acceptarea conexiunii de la un client */
    if (client < 0)
      {
        perror ("Eroare la accept().\n");
        continue;
      }

          if (nfds < client) /* ajusteaza valoarea maximului */
            nfds = client;
            
    /* includem in lista de descriptori activi si acest socket */
    FD_SET (client, &actfds);

    printf("S-a conectat clientul cu descriptorul %d, de la adresa %s.\n",client, conv_addr (from));
    fflush (stdout);
  }
      /* vedem daca e pregatit vreun socket client pentru a trimite raspunsul */
      for (fd = 0; fd <= nfds; fd++)  /* parcurgem multimea de descriptori */
  {
    /* este un socket de citire pregatit? */
    if (fd != sd && FD_ISSET (fd, &readfds))
      { 
        if (chat(fd)==0)
    {
      printf ("S-a deconectat clientul cu descriptorul %d.\n",fd);
      fflush (stdout);
      close (fd);   /* inchidem conexiunea cu clientul */
      FD_CLR (fd, &actfds);/* scoatem si din multime */
      
    }
      }
  }     /* for */
    }       /* while */
}       /* main */

/* realizeaza primirea si retrimiterea unui mesaj unui client */
int chat(int cd)
{
    char msg_primit[1000];
  char msg_trimis[1000];
  int step;
  char ms[1000];
  sqlite3_stmt *rez;
  char username[20];
int logare=vf_logare(cd);
//deschidem baza de date
    sqlite3 *db;
   int openBD;

   openBD=sqlite3_open("Offline_Messenger.db", &db);

   if(openBD) 
      fprintf(stderr, "Baza de date nu poate fi deschisa: %s\n", sqlite3_errmsg(db));
 else 
      fprintf(stderr, "Baza de date a fost deschisa cu succes\n");
        
       int tm=0;
            if (read (cd, msg_primit, sizeof (msg_primit)) <= 0)
            {
                    perror ("*Eroare la read () din client.\n");
                   return 0;
            }
         
        printf ("*Comanda a fost primita*\n");int comandacorecta=0;
        //Autentificare
        if(strcmp(msg_primit,"Autentificare")==0)
        { bzero(msg_trimis,1000); comandacorecta=1;
             if(logare==0){
          strcat(msg_trimis,"Introduceti numele de utilizator");
          if (write (cd, msg_trimis, 1000) <= 0)
        {
          perror ("*Eroare write() catre client* \n");
          //continue;
            }
           else
          printf ("*Mesajul a fost trimis cu succes*\n");  
            bzero(msg_primit,1000);
          if (read (cd, msg_primit,sizeof(msg_primit))<= 0) 
            {
                    perror ("*Eroare la read () dinspre client.\n");return 0;
            }
        printf ("*Nume de utilizator primit* %s \n",msg_primit);
       bzero(username,20);
      strcpy(username,msg_primit);
       int gasit=vf_nume_utilizator(username);
       if(gasit==1)
       {    
         bzero(msg_trimis,1000); 
          strcat(msg_trimis,"Introduceti parola");
          write (cd, msg_trimis, 1000);
              bzero(msg_primit,1000);
          if (read (cd, msg_primit,sizeof(msg_primit))<= 0) 
            {
                    perror ("*Eroare la read () dinspre client.\n");
                    return 0;
            }
            if(vf_parola(msg_primit,username)==1)
           { bzero(msg_trimis,1000);
             if(actualizare_stare_conectat(username,cd)==0)
                {strcpy(msg_trimis,"V-ati autentificat, dar nu a fost actualizata starea");}
            else
            {
             bzero(nume_dest,100);bzero(nume_exp,100);
               if(aflare_nume_expeditor(cd)==0)
                  printf("Eroare la aflare nume expeditor\n");
                 else
              {
              if(exista_mesaje_necitite(nume_exp)==0)
                strcpy(msg_trimis,"V-ati autentificat cu succes!");
              else
                  {{strcpy(msg_trimis,"+");strcat(msg_trimis,nume_exp);} }
                nrlogati=nrlogati+1;logati[nrlogati]=cd;
              }  
                        }
           }
           else
           {
             bzero(msg_trimis,1000);
            strcpy(msg_trimis,"Parola gresita Incercati din nou Autentificare");
           }
       }
         else if(gasit==0)
         {
             strcpy(msg_trimis,"Numele de utilizator nu exista. Incercati sa va autentificati din nou sau inregistrati-va");
         }
         }
         else
         {strcpy(msg_trimis,"Sunteti deja autentificat!");}         
         }// terminare autentificare
        else
      //Inregistrare
    if(strcmp(msg_primit,"Inregistrare")==0)
   {      
         bzero(msg_trimis,1000);comandacorecta=1;
            strcat(msg_trimis,"Introduceti numele de utilizator");
         if (write (cd, msg_trimis, 1000) <= 0)
        {
          perror ("*Eroare la write() catre client*\n");
          return 0;
            }
           else
          printf ("*Mesajul a fost trimis cu succes*\n");  
                bzero(msg_primit,1000);
          if (read (cd, msg_primit, sizeof (msg_primit)) <= 0)  
            {
                    perror ("*Eroare la read () dinspre client.\n");return 0;
            }
      printf ("*Nume de utilizator primit* %s\n",msg_primit);
       bzero(username,20);
      strcpy(username,msg_primit);
        bzero(msg_trimis,1000);
         if(vf_nume_utilizator(username)==0)
       {  
           strcat(msg_trimis,"Introduceti parola");
           write (cd, msg_trimis, 1000);
              bzero(msg_primit,1000);bzero(msg_trimis,1000);
          if (read (cd, msg_primit,sizeof(msg_primit))<= 0) 
            {
                    perror ("*Eroare la read () dinspre client.*\n");return 0;
            }
              if(inregistrare(username,msg_primit)==0)
              {  strcpy(msg_trimis,"Ceva nu a mers bine. Incercati din nou Inregistrare.");}
              else
            strcpy(msg_trimis,"V-ati inregistrat cu succes, pentru autentificare tastati Autentificare \n ");
       }
         else
         {
             strcpy(msg_trimis,"Nume de utilizator folosit. Incercati Inregistrare cu un alt nume de utilizator\n");
         }
          }
        else
        //Utilizatori
            if((logare==1)&&strcmp(msg_primit,"Utilizatori")==0)
        {
            //int openBD=sqlite3_open("Offline_Messenger.db", &db);
           bzero(msg_trimis,1000);bzero(ms,1000); comandacorecta=1;
           char *sql="SELECT Nume FROM Utilizatori;";
            sqlite3_prepare_v2(db,sql,-1,&rez,0);
             sprintf(ms,"Nume \n");
               strcpy(msg_trimis,ms);
           while((step=sqlite3_step(rez))==SQLITE_ROW)
            {

              sprintf(ms," %s \n",sqlite3_column_text(rez, 0)); 
              strcat(msg_trimis,ms);
            }
              sqlite3_finalize(rez);
               sqlite3_close(db);
        }
         else
        //Utilizatori deconectati
                if(logare==1&&strcmp(msg_primit,"Utilizatori deconectati")==0)
        {
            //int openBD=sqlite3_open("Offline_Messenger.db", &db);
           bzero(msg_trimis,1000);bzero(ms,1000); comandacorecta=1;
           char *sql="SELECT Nume FROM Utilizatori WHERE Stare=0;";
           sqlite3_prepare_v2(db,sql,-1,&rez,0);
             sprintf(ms,"Nume \n");
               strcpy(msg_trimis,ms);
           while((step=sqlite3_step(rez))==SQLITE_ROW)
            {

              sprintf(ms," %s \n",sqlite3_column_text(rez, 0)); 
              strcat(msg_trimis,ms);
            }
              sqlite3_finalize(rez);
               sqlite3_close(db);
        }
else
         //Utilizatori conectati
                if(logare==1&&strcmp(msg_primit,"Utilizatori conectati")==0)
        {
            //int openBD=sqlite3_open("Offline_Messenger.db", &db);
           bzero(msg_trimis,1000);bzero(ms,1000); comandacorecta=1;
           char *sql="SELECT Nume FROM Utilizatori WHERE Stare=1;";
           sqlite3_prepare_v2(db,sql,-1,&rez,0);
             sprintf(ms,"Nume \n");
               strcpy(msg_trimis,ms);
           while((step=sqlite3_step(rez))==SQLITE_ROW)
            {

              sprintf(ms," %s \n",sqlite3_column_text(rez, 0)); 
              strcat(msg_trimis,ms);
            }
              sqlite3_finalize(rez);
               sqlite3_close(db);
        }
else
       //Ajutor
        if(strcmp(msg_primit,"Ajutor")==0)
   {    bzero(msg_trimis,1000); comandacorecta=1;
   strcpy(msg_trimis,"Comenzi :\n");
   strcat(msg_trimis,"Ajutor - detalii despre comenzile disponibile\n");
   strcat(msg_trimis,"Autentificare\n");
   strcat(msg_trimis,"Inregistrare\n");
   strcat(msg_trimis,"Trimitere <nume_destinatar> <text mesaj> - trimiterea unui mesaj catre un utilizator\n");
   strcat(msg_trimis,"Istorie - Istoria conversatiilor dintre utilizatorul curent si toti ceilalti utilizatori\n");
   strcat(msg_trimis,"Istorie <nume> - istoria conversatiilor dintre utilizatorul curent si utilizatorul <nume>\n");
   strcat(msg_trimis,"Utilizatori\n");
   strcat(msg_trimis,"Utilizatori conectati\n");
   strcat(msg_trimis,"Utilizatori deconectati\n");
   strcat(msg_trimis,"RA - Raspuns automat pentru ultimul mesaj, daca acest face parte din categoria mesajelor specifice\n");
   strcat(msg_trimis,"Deconectare\n");
   }
else
     //Deconectare
    if((logare==1)&&strcmp(msg_primit,"Deconectare")==0)
   {    bzero(msg_trimis,1000); comandacorecta=1;int p,i;
          for(i=1;i<=nrlogati;i++)
            if(logati[i]==cd)
              {p=i;break;}
          for(i=p+1;i<=nrlogati;i++)
              {logati[i-1]=logati[i];}
          nrlogati--;
          if(actualizare_stare_deconectat(cd)==0)
            strcpy(msg_trimis,"Deconectat cu succes, fara a actualiza starea si descriptorul");
            else
         strcpy(msg_trimis,"Deconectat cu succes");
   }
else
    //Trimitere
    if((logare==1)&&(strstr(msg_primit,"Trimitere")!=0))
    {bzero(msg_trimis,1000);comandacorecta=1;
      bzero(nume_dest,100);bzero(nume_exp,100);
      bzero(mesaj,1000);
      printf("Avem comanda trimitere");
      trimitere(msg_primit);
      if(vf_nume_utilizator(nume_dest)==0)

          strcpy(msg_trimis,"Nu exista utilizator cu numele specificat! \n");
       else
       {
      
        if(aflare_nume_expeditor(cd)==0)
          
          strcpy(msg_trimis,"Eroare la aflare nume expeditor\n");
        else
            {if(adaugare_Istorie(nume_exp,nume_dest,mesaj)==0)

          strcpy(msg_trimis,"Eroare la adaugare in istorie\n");
         else
        if(aflare_descriptor_dest(nume_dest)==0)
          strcpy(msg_trimis,"Eroare la aflare descriptorului destinatarului\n");
        else
       {
        if(aflare_stare_dest(nume_dest)==0)
        {if(salvare_mesaje_necitite(nume_exp,nume_dest,mesaj)==0)
           {strcpy(msg_trimis,"Eroare la salvarea in mesaje necitite\n");tm=2;}}
        else
        {strcpy(msg_trimis,nume_exp);strcat(msg_trimis,": "); strcat(msg_trimis,mesaj);tm=1;
        int i,gasit=0,p;
           for(i=1;i<=nrutilizatoriRA;i++)
             if(utilizatoriRA[i].dexpeditor==cd)
                {gasit=1;p=i;break;}
  
              if(gasit==1)
                {strcpy(utilizatoriRA[p].numexp,nume_dest);
          strcpy(utilizatoriRA[p].ultimmesaj,mesaj); utilizatoriRA[p].dexpeditor=destd;
          utilizatoriRA[p].ddestinatar=cd;}
                else
          {nrutilizatoriRA++;strcpy(utilizatoriRA[nrutilizatoriRA].numexp,nume_dest);
          strcpy(utilizatoriRA[nrutilizatoriRA].ultimmesaj,mesaj); utilizatoriRA[nrutilizatoriRA].dexpeditor=destd;
          utilizatoriRA[nrutilizatoriRA].ddestinatar=cd;}
              }
       }
     }
 }}
else
   //Istorie
   if((logare==1)&&(strcmp(msg_primit,"Istorie")==0))
   {  bzero(msg_trimis,1000);comandacorecta=1;
      bzero(nume_dest,100);bzero(nume_exp,100);
      bzero(mesaj,1000);
      if(aflare_nume_expeditor(cd)==0)
          strcpy(msg_trimis,"Eroare la aflare nume expeditor\n");
      else
      {strcpy(msg_trimis,"!1");strcat(msg_trimis,nume_exp);}
    }//se termina Istorie

  //Istorie <nume>
  if((logare==1)&&(strstr(msg_primit,"Istorie ")!=0)&&strcmp(msg_primit,"Istorie")>0)
   {  bzero(msg_trimis,1000);comandacorecta=1;int rezultat=1;
      bzero(nume_dest,100);bzero(nume_exp,100);
      bzero(mesaj,1000);
      if(aflare_nume_expeditor(cd)==0)
      {strcpy(msg_trimis,"Eroare la aflare nume expeditor\n");rezultat=0;}
     istorie_nume2(msg_primit);//aflam in nume_dest <nume>
    if(vf_nume_utilizator(nume_dest)==0)
    {strcpy(msg_trimis,"Nu exista utilizator cu numele specificat!\n");rezultat=0;}
    if(rezultat==1)
    {strcpy(msg_trimis,"!2");strcat(msg_trimis,nume_exp);strcat(msg_trimis," ");strcat(msg_trimis,nume_dest);}

    }//se termina Istorie <nume>
else
//RA(Raspuns Automat)
  if((logare==1)&&(strcmp(msg_primit,"RA")==0))
  {int gasit=0,i,p;
    bzero(msg_trimis,1000);comandacorecta=1;
    for(i=1;i<=nrutilizatoriRA;i++)
      if(utilizatoriRA[i].dexpeditor==cd)
            {gasit=1;p=i;break;}
       
    if(gasit==0)
      strcpy(msg_trimis,"Optiunea RA nu este disponibila, deoarece nu aveti nici un ultim mesaj.");
    else
    {//verificam daca mesajul face parte din mesaje specifice
      if(vf_mesaj_specific(utilizatoriRA[p].ultimmesaj)==1)
      {strcpy(msg_trimis,utilizatoriRA[p].numexp);strcat(msg_trimis,": ");strcat(msg_trimis,raspRA);
       if (write (utilizatoriRA[p].ddestinatar, msg_trimis, 1000) <= 0)
        {
          perror ("*Eroare la write() RA catre client ..\n");
            }
           else
          printf ("*Mesajul a fost trimis cu succes.*\n");
        tm=3;
      }
       else
        strcpy(msg_trimis,"Optiunea RA nu este disponibila, deoarece mesajul nu face parte din mesaje specifice.");
    }


  }//se termina RA
else
  if(comandacorecta==0)
   {
     bzero(msg_trimis,1000);
     strcpy(msg_trimis,"Comanda este gresita! Incearca din nou!\n");
   }
   
   if(tm==0)
    {if (write (cd, msg_trimis, 1000) <= 0)
        {
          perror ("*Eroare la write() catre client*\n");
            }
           else
          printf ("*Mesajul a fost trimis cu succes*\n");}
        else
          if(tm==1)
        {
          if (write (destd, msg_trimis, 1000) <= 0)
        {
          perror ("*Eroare la write() catre client ..\n");
          printf("Eroare la scriere catre client cu descriptorul %d",destd);
            }
           else
          printf ("*Mesajul a fost trimis cu succes*\n");

        }
 bzero(msg_primit,1000);
          bzero(msg_trimis,1000);
      return 1;
}
