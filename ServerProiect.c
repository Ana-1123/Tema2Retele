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
#define PORT 2024
 char nume_dest[100],nume_exp[100],mesaj[1000];
 int destd;
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
  if(gasit==1)
     return 1;
return 0;
}

int actualizare_stare_conectat(char* nume_utilizator, int cd)
{ sqlite3 *db;
  int openBD;
   openBD=sqlite3_open("Offline_Messenger.db", &db);
   int st;  
int gasit=0;
  sqlite3_stmt *stmt;
    char *query = NULL;
      asprintf(&query, "UPDATE Utilizatori SET Stare=1, Descriptor='%d' WHERE Nume='%s';",cd,nume_utilizator);           
    sqlite3_prepare_v2(db, query,-1, &stmt, NULL);
      st = sqlite3_step(stmt);
       if (st != SQLITE_DONE) {
             printf("ERROR UPDATE: %s\n", sqlite3_errmsg(db)); return 0;
              }
        sqlite3_finalize(stmt);
    free(query); 
     return 1;
}
int actualizare_stare_deconectat(int cd)
{ sqlite3 *db;
  int openBD;
   openBD=sqlite3_open("Offline_Messenger.db", &db);
   int st;  
int gasit=0;
  sqlite3_stmt *stmt;
    char *query = NULL;
      asprintf(&query, "UPDATE Utilizatori SET Stare=0, Descriptor=NULL WHERE Descriptor='%d';",cd);           
    sqlite3_prepare_v2(db, query,-1, &stmt, NULL);
      st = sqlite3_step(stmt);
       if (st != SQLITE_DONE) {
             printf("ERROR UPDATE: %s\n", sqlite3_errmsg(db)); return 0;
              }
        sqlite3_finalize(stmt);
    free(query); 
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
     {printf("ERROR inserting data: %s\n", sqlite3_errmsg(db));
     return 0;} 
     sqlite3_finalize(stmt);
        free(query); 
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
        return 1; 
}

void trimitere(char* comanda/*,char* nume_dest,char* mesaj*/)
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
int aflare_nume_expeditor(int cd,char nume_exp[100])
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
  sqlite3_finalize(stmt);
  free(query); 
  if(gasit==1)
     return 1;
return 0;
}
int aflare_descriptor_dest(char nume_dest[100],int destd)
{
  sqlite3 *db;
  int openBD;
   openBD=sqlite3_open("Offline_Messenger.db", &db);
   int st;
int gasit=0;
  sqlite3_stmt *stmt;
    char *query = NULL;
      asprintf(&query, "SELECT Descriptor FROM Utilizatori WHERE Nume='%s';",nume_dest);         
    sqlite3_prepare_v2(db, query,-1, &stmt, NULL);
while ( (st = sqlite3_step(stmt)) == SQLITE_ROW) 
       { gasit++;destd=sqlite3_column_int(stmt,3);}
     printf("Descriptor destinatar %d \n",destd);
  sqlite3_finalize(stmt);
  free(query); 
  if(gasit==1)
     return 1;
return 0;
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
/*void afisare_istorie(char *nume_exp)
{   
   sqlite3 *db;
  int openBD;
   openBD=sqlite3_open("Offline_Messenger.db", &db);
   int st;
    sqlite3_stmt *stmt;
     char *query = NULL;
           bzero(msg_trimis,1000);
           bzero(ms,1000); int gasit=0;
          asprintf(&query, "SELECT * FROM Istorie WHERE Expeditor='%s'OR Destinatar='%s';",Id_mel);  
           sqlite3_prepare_v2(db, query, strlen(query), &stmt, NULL);
                 while ( (st = sqlite3_step(stmt)) == SQLITE_ROW) 
                     { gasit++;}
                 if(gasit==0)
                {
                   strcpy(msg_trimis,"Wrong song Id .Try again See comments for a song.");
                 }
                 else{

           bzero(ms,1000);  char *sql= NULL;
          asprintf(&sql, "SELECT * FROM Comentarii WHERE Id_mel='%d';",Id_mel); 
           st=sqlite3_prepare_v2(db,sql,-1,&rez,0);
             sprintf(ms," \n Id_mel     Nume_melodie     Comentariu        Nume_utilizator \n");
               strcpy(msg_trimis,ms); gasit=0;
           while((step=sqlite3_step(rez))==SQLITE_ROW)
            { 
               gasit++;
              sprintf(ms,"%d | %s | %s | %s \n\n",sqlite3_column_int(rez, 1),sqlite3_column_text(rez, 2),sqlite3_column_text(rez, 3),sqlite3_column_text(rez, 4)); 
              strcat(msg_trimis,ms);
            }
                 }
          if(gasit==0)
          {
            bzero(msg_trimis,1000);strcpy(msg_trimis,"Nu exista comentarii la melodia cu Id-ul respectiv.");
          }
              sqlite3_finalize(rez);
              printf("%s",msg_trimis);
            sqlite3_close(db);
}//terminare afisare_istorie

void afisare_istorie_cu_utilizator(char *nume_exp,char *nume_dest)
{
}*/
int main()
{
  struct sockaddr_in server,client_addr;
  char msg_primit[1000];
  char msg_trimis[1000];
  int st,sd,step;
  char ms[1000];
   char *query = NULL;
   char *query1=NULL;
  sqlite3_stmt *rez,*stmt,*stmt1;
  char username[20];

//deschidem baza de date
    sqlite3 *db;
   int openBD;

   openBD=sqlite3_open("Offline_Messenger.db", &db);

   if(openBD) 
      fprintf(stderr, "Can not open database: %s\n", sqlite3_errmsg(db));
 else 
      fprintf(stderr, "Opened database successfully\n");

  if((sd=socket(AF_INET,SOCK_STREAM,0))==-1)
   {
    	perror ("***Error at socket().\n");
    	return errno;
   }
     bzero (&server, sizeof (server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl (INADDR_ANY);
    server.sin_port = htons (PORT);

     if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
    {
    	perror ("***Error at bind().\n");
    	return errno;
    }

    if(listen(sd,10)==-1)
    {
        perror("*** Error at listen() .\n");
        return errno;
    }

    bzero(msg_primit,1000);
    printf ("***WAITING AT PORT %d...\n",PORT);
    fflush (stdout);
    while(1)
    {
        int cd;//descriptor client
        int lungime=sizeof(client_addr);
    	cd = accept (sd,(struct sockaddr *) &client_addr, &lungime);
    	if (cd < 0)
    	{
    		perror ("***Error at accept().\n");
    		exit(1);
    	}
       printf("[server] S-a conectat clientul cu descriptorul %d\n",cd);
      int pid=fork();
      if (pid == -1) 
      {
    		close(cd);
    		continue;
    	}
        /*else
        if(pid>0)
        {//parinte
         close(cd);
         while(waitpid(-1,NULL,WNOHANG));
         continue;
        }*/
        else
        if(pid==0)
       { //copil
         close(sd);
      int login=0;
      int tm=0;
        while(1)
        {
          bzero(msg_primit,1000);
            if (read (cd, msg_primit, sizeof (msg_primit)) <= 0)
            {
                    perror ("***Error read () from client.\n");
                    close (cd);	
                    continue;
            }
         
        printf ("*** Command received...\n");int comandacorecta=0;
        //Autentificare
        if(strcmp(msg_primit,"Autentificare")==0)
        { bzero(msg_trimis,1000); comandacorecta=1;
          strcat(msg_trimis,"Introduceti numele de utilizator");
          if (write (cd, msg_trimis, 1000) <= 0)
    		{
    			perror ("***Error at write()  to client ..\n");
    			//continue;
            }
    		   else
    			printf ("***Send message successfuly.\n");  
            bzero(msg_primit,1000);
          if (read (cd, msg_primit,sizeof(msg_primit))<= 0)	
            {
                    perror ("***Error read () from client.\n");
                    close (cd);	
                    continue;
            }
        printf ("***Username received... %s \n",msg_primit);
       bzero(username,20);
      strcpy(username,msg_primit);
        bzero(msg_trimis,1000);
       int gasit=vf_nume_utilizator(username);
       if(gasit==1)
       {    
         bzero(msg_trimis,1000); 
          strcat(msg_trimis,"Introduceti parola");
          write (cd, msg_trimis, 1000);
              bzero(msg_primit,1000);
          if (read (cd, msg_primit,sizeof(msg_primit))<= 0)	
            {
                    perror ("***Error read () from client.\n");
                    close (cd);	
                    continue;
            }
            if(vf_parola(msg_primit,username)==1)
           { login=1;bzero(msg_trimis,1000);
             if(actualizare_stare_conectat(username,cd)==0)
                strcpy(msg_trimis,"V-ati autentificat, dar nu a fost actualizata starea");
            else
            strcpy(msg_trimis,"V-ati autentificat cu succes");
           }
           else
           {
             bzero(msg_trimis,1000);
            strcpy(msg_trimis,"Parola gresita Incercati din nou Autentificare");
           }
       }
         else if(gasit==0)
         {
             strcpy(msg_trimis,"Numele de utilizator nu exista. Incercati sa va autentificati din nou sau inregistrati-va\n");
         }         
         }
        
      //Inregistrare
    if(strcmp(msg_primit,"Inregistrare")==0)
   {      
         bzero(msg_trimis,1000);comandacorecta=1;
            strcat(msg_trimis,"Introduceti numele de utilizator");
         if (write (cd, msg_trimis, 1000) <= 0)
    		{
    			perror ("***Error at write()  to client ..\n");
    			continue;
            }
    		   else
    			printf ("***Send message successfuly.\n");  
                bzero(msg_primit,1000);
          if (read (cd, msg_primit, sizeof (msg_primit)) <= 0)	
            {
                    perror ("***Error read () from client.\n");
                    close (cd);	
                    continue;
            }
      printf ("*** Username received... %s\n",msg_primit);
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
                    perror ("***Error read () from client.\n");
                    close (cd);	
                    continue;
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

        //Utilizatori
            if((login==1)&&strcmp(msg_primit,"Utilizatori")==0)
        {
            int openBD=sqlite3_open("Offline_Messenger.db", &db);
           bzero(msg_trimis,1000);bzero(ms,1000); comandacorecta=1;
           char *sql="SELECT Nume FROM Utilizatori;";
           st=sqlite3_prepare_v2(db,sql,-1,&rez,0);
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

        //Utilizatori deconectati
                if((login==1)&&strcmp(msg_primit,"Utilizatori deconectati")==0)
        {
            int openBD=sqlite3_open("Offline_Messenger.db", &db);
           bzero(msg_trimis,1000);bzero(ms,1000); comandacorecta=1;
           char *sql="SELECT Nume FROM Utilizatori WHERE Stare=0;";
           st=sqlite3_prepare_v2(db,sql,-1,&rez,0);
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

         //Utilizatori conectati
                if((login==1)&&strcmp(msg_primit,"Utilizatori conectati")==0)
        {
            int openBD=sqlite3_open("Offline_Messenger.db", &db);
           bzero(msg_trimis,1000);bzero(ms,1000); comandacorecta=1;
           char *sql="SELECT Nume FROM Utilizatori WHERE Stare=1;";
           st=sqlite3_prepare_v2(db,sql,-1,&rez,0);
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

       //Ajutor
        if(strcmp(msg_primit,"Ajutor")==0)
   {    bzero(msg_trimis,1000); comandacorecta=1;
   strcpy(msg_trimis,"Comenzi :\n");
   strcat(msg_trimis,"Ajutor - detalii despre comenzile disponibile\n");
   strcat(msg_trimis,"Autentificare\n");
   strcat(msg_trimis,"Inregistrare\n");
   strcat(msg_trimis,"Trimitere <nume_destinatar> <text mesaj>\n");
   strcat(msg_trimis,"Istorie - Istoria conversatiilor dintre utilizatorul curent si toti ceilalti utilizatori\n");
   strcat(msg_trimis,"Istorie <nume> - istoria conversatiilor dintre utilizatorul curent si utilizatorul <nume>\n");
   strcat(msg_trimis,"Utilizatori\n");
   strcat(msg_trimis,"Utilizatori conectati\n");
   strcat(msg_trimis,"Utilizatori deconectati\n");
   strcat(msg_trimis,"Citire - Pentru citirea mesajelor primite in perioada in care utilizatorul nu a fost conectat\n");
   strcat(msg_trimis,"RA - Raspuns automat pentru ultimul mesaj, daca acest face parte din categoria mesajelor specifice\n");
   strcat(msg_trimis,"Deconectare\n");
   }

     //Deconectare
    if((login==1)&&strcmp(msg_primit,"Deconectare")==0)
   {    bzero(msg_trimis,1000); comandacorecta=1;
          login=0;
          if(actualizare_stare_deconectat(cd)==0)
            strcpy(msg_trimis,"Deconectat cu succes, fara a actualiza starea");
            else
         strcpy(msg_trimis,"Deconectat cu succes");
   }

    //Trimitere
    if((login==1)&&(strstr(msg_primit,"Trimitere")!=0))
    {bzero(msg_trimis,1000);comandacorecta=1;
      bzero(nume_dest,100);
      bzero(mesaj,1000);
      printf("Avem comanda trimitere");
      trimitere(msg_primit/*,nume_dest,mesaj*/);
      if(vf_nume_utilizator(nume_dest)==0)

          strcpy(msg_trimis,"Nu exista utilizator cu numele specificat! \n");
       else
       {
      
        if(aflare_nume_expeditor(cd,nume_exp)==0)
          
          strcpy(msg_trimis,"Eroare la aflare nume expeditor\n");
        else
            {if(adaugare_Istorie(nume_exp,nume_dest,mesaj)==0)

          strcpy(msg_trimis,"Eroare la adaugare in istorie\n");
         else
        if(aflare_descriptor_dest(nume_dest,destd)==0)
          strcpy(msg_trimis,"Eroare la aflare descriptorului destinatarului\n");
        else
       {
        strcpy(msg_trimis,nume_exp);strcat(msg_trimis,": "); strcat(msg_trimis,mesaj);tm=1;}
       }
     }
 }

   //Istorie
   if((login==1)&&(strcmp(msg_primit,"Istorie")==0))
   {  bzero(msg_trimis,1000);comandacorecta=1;
      bzero(nume_dest,100);bzero(nume_exp,100);
      bzero(mesaj,1000);
      if(aflare_nume_expeditor(cd,nume_exp)==0)
          strcpy(msg_trimis,"Eroare la aflare nume expeditor\n");
      else
      {strcpy(msg_trimis,"!1");strcat(msg_trimis,nume_exp);}
     // afisare_istorie(nume_exp);
    }//se termina Istorie

  //Istorie <nume>
  if((login==1)&&(strstr(msg_primit,"Istorie ")!=0)&&strcmp(msg_primit,"Istorie")>0)
   {  bzero(msg_trimis,1000);comandacorecta=1;int rezultat=1;
      bzero(nume_dest,100);bzero(nume_exp,100);
      bzero(mesaj,1000);
      if(aflare_nume_expeditor(cd,nume_exp)==0)
      {strcpy(msg_trimis,"Eroare la aflare nume expeditor\n");rezultat=0;}
     istorie_nume2(msg_primit);//aflam in nume_dest <nume>
    if(vf_nume_utilizator(nume_dest)==0)
    {strcpy(msg_trimis,"Nu exista utilizator cu numele specificat!\n");rezultat=0;}
    if(rezultat==1)
    {strcpy(msg_trimis,"!2");strcat(msg_trimis,nume_exp);strcat(msg_trimis," ");strcat(msg_trimis,nume_dest);}
     //afisare_istorie_cu_utilizator(nume_exp,nume_dest);
    }//se termina Istorie <nume>


  if(comandacorecta==0)
   {
     bzero(msg_trimis,1000);
     strcpy(msg_trimis,"Command is wrong try again ! \n");
   }
   
   if(tm==0)
    {if (write (cd, msg_trimis, 1000) <= 0)
    		{
    			perror ("***Error at write()  to client ..\n");
    			continue;
            }
    		   else
    			printf ("***Send message successfuly.\n");}
        else
        {if (write (destd, msg_trimis, 1000) <= 0)
        {
          perror ("***Error at write()  to client ..\n");
          continue;
            }
           else
          printf ("***Send message successfuly.\n");

        }
       
        /*if(strstr(msg_primit,"EXIT")!=0)
        {
           close (cd);
                    break;
        }*/

      }
      exit(0);

        }
         }
    return 0;
}
























