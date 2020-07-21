#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>
#include"libpq-fe.h"
//ESTABLISHING A CONNECTION WITH THE DATABASE
void do_exit(PGconn *conn,PGresult *res)
{
    PQclear(res);
    PQfinish(conn);
    exit(1);
}

int main()
{
    int num;
    int result=0;
    PGconn *conn=PQconnectdb("user=postgres password=root dbname=madewithc");
    if(PQstatus(conn)==CONNECTION_BAD)
    {
        printf("Connect to database failed: %s\n", PQerrorMessage(conn));
       PQfinish(conn);
    }
    char *user=PQuser(conn);
   char *db_name=PQdb(conn);
    char *pswd=PQpass(conn);
    
    
    printf("\n***************You Are Now Connected******************\n");
    printf("User: %s \n",user);
    printf("Database: %s \n",db_name);
    printf("Password: %s \n",pswd);
    printf("\n Enter an integer for creating its mathematical table\n\t");
    scanf("%d",&num);
    
    PGresult *res = PQexec(conn, "DROP TABLE IF EXISTS tablec");
    
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        do_exit(conn, res);
    }
    
    PQclear(res);

    res = PQexec(conn, "CREATE TABLE tablec(Multiplicand INTEGER , Multiplier INTEGER , Result INTEGER)");
        
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        do_exit(conn, res); 
    }
    PQclear(res);
    char insert[50];
    for(int i=1;i<=50;i++)
    {
    sprintf(insert,"INSERT INTO tablec VALUES(%d,%d,%d)",num,i,(num*i)); 
    res = PQexec(conn, insert);
        
    if (PQresultStatus(res) != PGRES_COMMAND_OK) 
        do_exit(conn, res);     
    
    PQclear(res); 
     }
     printf("\n TABLE CREATED CHECK PSQLSHELL");
     


    PQfinish(conn);
    return 0;
} 