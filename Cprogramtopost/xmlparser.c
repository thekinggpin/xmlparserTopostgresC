	#include<stdio.h>
	#include<malloc.h>
	#include<string.h>
	#include<stdlib.h>
	#include"libpq-fe.h"
/* THIS HAS DATABASE END PARSING RATHER THAN SCRIPT END
    1) Read from file(.XML) into buffer(String)
	2) Create a table into postgres with (XML type data) by using XMLPARSE predefined in postgres
	3) Parse the data as needed

  author: Ram Kartikeya
*/
	void do_exit(PGconn *conn,PGresult *res)
	{
	PQclear(res);
	PQfinish(conn);
	exit(1);
	}

	int main()
	{
	char file_content[25000];
	FILE *fp;
	char ch;
	int i=0;
	fp=fopen("GponSdDb.xml","r");
	if(fp==NULL)
	printf("File not opened");
	else
	{
	while((ch=getc(fp))!=EOF)
	{
	strncat(file_content,&ch,1);//STRCAT
	}
	}
    fflush(stdin);
	fclose(fp);
	fflush(fp);
	PGconn *conn=PQconnectdb("user=postgres password=root dbname=madewithc");
	if(PQstatus(conn)==CONNECTION_BAD)
	{
	printf("Connect to database failed: %s\n", PQerrorMessage(conn));
	PQfinish(conn);
	}
	char *user=PQuser(conn);
	char *db_name=PQdb(conn);
	char *pswd=PQpass(conn);
	//printf("\n..%s",file_content);
    
	printf("\n***************You Are Now Connected******************\n");
	printf("User: %s \n",user);
	printf("Database: %s \n",db_name);
	printf("Password: %s \n",pswd); 
	fflush(stdout);
  
	PGresult *res = PQexec(conn,"DROP TABLE IF EXISTS datastore");
	if (PQresultStatus(res) != PGRES_COMMAND_OK) {
	do_exit(conn, res);
	}
	PQclear(res);

	res = PQexec(conn, "CREATE TABLE datastore(data xml)");
	if (PQresultStatus(res) != PGRES_COMMAND_OK) {
	do_exit(conn, res);
	}
	PQclear(res);

	char *databuff=malloc(30000 *sizeof(char));
    
	sprintf(databuff, "INSERT INTO datastore VALUES (XMLPARSE (DOCUMENT '%s'))",file_content);

	//printf("\n..%s",databuff);
	res = PQexec(conn,databuff);
	                                                                  
	if (PQresultStatus(res) != PGRES_COMMAND_OK) {
	do_exit(conn, res);
	}
	PQclear(res);

	free(databuff);
	res = PQexec(conn, "DROP TABLE IF EXISTS finalxml");
	if (PQresultStatus(res) != PGRES_COMMAND_OK) {
	do_exit(conn, res);
	}
	PQclear(res);
	char *databuff2=malloc(3000 *sizeof(char));
	databuff2="SELECT xmltable.* INTO finalxml FROM datastore,XMLTABLE('//database/objects/SwImageInfo' PASSING data COLUMNS CardID INTEGER PATH 'CardId',ActiveImage INTEGER PATH 'ActiveImage',CommitImage INTEGER PATH 'CommitImage',IChecksum INTEGER PATH 'IChecksum',IMd5sum TEXT PATH 'IMd5sum',IVersionName TEXT PATH 'IVersionName') ORDER BY CardID";

	res = PQexec(conn,databuff2);
    
	if (PQresultStatus(res) != PGRES_COMMAND_OK) {
	do_exit(conn, res);
	}
	PQclear(res);
	
	//free(databuff2);
	// USE \n for printf debug
       //printf("AAAAAAAA Faulty AAAAAA\n");
	   char *databuff3=malloc(3000 *sizeof(char));
	databuff3="SELECT xmltable.* INTO finalxml2 FROM datastore,XMLTABLE('//database/objects/SwImageInfo' PASSING data COLUMNS CardID INTEGER PATH 'CardId',RChecksum INTEGER PATH 'RChecksum',RMd5sum TEXT PATH 'RMd5sum',RVersionName TEXT PATH 'RVersionName')";

	res = PQexec(conn,databuff3);
    
	if (PQresultStatus(res) != PGRES_COMMAND_OK) {
	do_exit(conn, res);
	}
	PQclear(res);
	
	PQfinish(conn);
	return 0;
	}



    
