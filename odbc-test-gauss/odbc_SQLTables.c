#include <stdio.h>
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <time.h>
SQLHENV       V_OD_Env;    // Handle ODBC environment
SQLHENV       V_OD_hstmt; 
SQLRETURN       rc;
long       V_OD_erg;    // result of functions
SQLHDBC       V_OD_hdbc;                      // Handle connection
char       V_OD_stat[10];    // Status SQL
SQLINTEGER     V_OD_err,V_OD_rowanz,V_OD_id,V_ID;
SQLSMALLINT     V_OD_mlen,V_OD_colanz;
char             V_OD_msg[200],V_OD_buffer[200],schema[200],table[200],database[200],remark[200],V_OD_buffer1[200];

SQLINTEGER maxlv,minlv,curschema;
SQLINTEGER m_min,m_max;
SQLHSTMT        hstmt;
char *buf = "Mike";
int value = 3;

int main(int argc,char *argv[])
{

  /**
	ODBC handle
	1) SQL_HANDLE_ENV 
	2) SQL_HANDLE_DBC
	3) SQL_HANDLE_STMT
  */
	
  // 1. test SQLAllocEnv
	V_OD_erg = SQLAllocEnv(&V_OD_Env);
	if ((V_OD_erg != SQL_SUCCESS) && (V_OD_erg != SQL_SUCCESS_WITH_INFO))
	{
		printf("Error AllocHandle\n");
	return -1;
	}
	V_OD_erg = SQLSetEnvAttr(V_OD_Env, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0); 
	if ((V_OD_erg != SQL_SUCCESS) && (V_OD_erg != SQL_SUCCESS_WITH_INFO))
	{
		printf("Error SetEnv\n");
		SQLFreeHandle(SQL_HANDLE_ENV, V_OD_Env);
		return -1;
	}
  
  // 2. allocate connection handle, set timeout
	V_OD_erg = SQLAllocConnect (V_OD_Env, &V_OD_hdbc); 
	if ((V_OD_erg != SQL_SUCCESS) && (V_OD_erg != SQL_SUCCESS_WITH_INFO))
	{
		printf("Error AllocHDB %d\n",V_OD_erg);
		SQLFreeHandle(SQL_HANDLE_ENV, V_OD_Env);
		return -1;
	}
  
  // 3. Connect to the datasource "gaussdb" 
	V_OD_erg = SQLConnect(V_OD_hdbc, (SQLCHAR*) "gaussdb", SQL_NTS,
                                     (SQLCHAR*) "", SQL_NTS,
                                     (SQLCHAR*) "", SQL_NTS);
	if ((V_OD_erg != SQL_SUCCESS) && (V_OD_erg != SQL_SUCCESS_WITH_INFO))
	{
		printf("Error SQLConnect %d\n",V_OD_erg);
		SQLGetDiagRec(SQL_HANDLE_DBC, V_OD_hdbc,1, 
                  V_OD_stat, &V_OD_err,V_OD_msg,100,&V_OD_mlen);
		printf("%s (%d)\n",V_OD_msg,V_OD_err);
		SQLFreeHandle(SQL_HANDLE_ENV, V_OD_Env);
		return -1;
	}
	printf("Connected Successfuly!\n");
	V_OD_erg = SQLAllocHandle(SQL_HANDLE_STMT, V_OD_hdbc, &hstmt);
	if ((V_OD_erg != SQL_SUCCESS) && (V_OD_erg != SQL_SUCCESS_WITH_INFO))
	{
		printf("Error allcoc hstmt %d\n",V_OD_erg);
		SQLGetDiagRec(SQL_HANDLE_STMT, hstmt,1, 
                  V_OD_stat, &V_OD_err,V_OD_msg,100,&V_OD_mlen);
		printf("%s (%d)\n",V_OD_msg,V_OD_err);
		SQLFreeHandle(SQL_HANDLE_ENV, V_OD_Env);
		return -1;
	}
	 V_OD_erg = SQLExecDirect(hstmt,"drop table IF EXISTS FVT_INTERFACE.odbc_SQLTables_001",SQL_NTS);
	if ((V_OD_erg != SQL_SUCCESS) && (V_OD_erg != SQL_SUCCESS_WITH_INFO))
	{
		printf("Error drop table %d\n",V_OD_erg);
		SQLGetDiagRec(SQL_HANDLE_STMT, hstmt,1, 
                  V_OD_stat, &V_OD_err,V_OD_msg,100,&V_OD_mlen);
		printf("%s (%d)\n",V_OD_msg,V_OD_err);
		SQLFreeHandle(SQL_HANDLE_ENV, V_OD_Env);
		return -1;
	}
	 V_OD_erg = SQLExecDirect(hstmt,"drop table IF EXISTS FVT_INTERFACE.odbc_SQLTables_002",SQL_NTS);
	if ((V_OD_erg != SQL_SUCCESS) && (V_OD_erg != SQL_SUCCESS_WITH_INFO))
	{
		printf("Error drop table %d\n",V_OD_erg);
		SQLGetDiagRec(SQL_HANDLE_STMT, hstmt,1, 
                  V_OD_stat, &V_OD_err,V_OD_msg,100,&V_OD_mlen);
		printf("%s (%d)\n",V_OD_msg,V_OD_err);
		SQLFreeHandle(SQL_HANDLE_ENV, V_OD_Env);
		return -1;
	}
	 V_OD_erg = SQLExecDirect(hstmt,"create table FVT_INTERFACE.odbc_SQLTables_001(id int,addr varchar(25),age float)",SQL_NTS);
	if ((V_OD_erg != SQL_SUCCESS) && (V_OD_erg != SQL_SUCCESS_WITH_INFO))
	{
		printf("Error drop table %d\n",V_OD_erg);
		SQLGetDiagRec(SQL_HANDLE_STMT, hstmt,1, 
                  V_OD_stat, &V_OD_err,V_OD_msg,100,&V_OD_mlen);
		printf("%s (%d)\n",V_OD_msg,V_OD_err);
		SQLFreeHandle(SQL_HANDLE_ENV, V_OD_Env);
		return -1;
	}
	 V_OD_erg = SQLExecDirect(hstmt,"create table FVT_INTERFACE.odbc_SQLTables_002(id int,addr varchar(25),age float)",SQL_NTS);
	if ((V_OD_erg != SQL_SUCCESS) && (V_OD_erg != SQL_SUCCESS_WITH_INFO))
	{
		printf("Error drop table %d\n",V_OD_erg);
		SQLGetDiagRec(SQL_HANDLE_STMT, hstmt,1, 
                  V_OD_stat, &V_OD_err,V_OD_msg,100,&V_OD_mlen);
		printf("%s (%d)\n",V_OD_msg,V_OD_err);
		SQLFreeHandle(SQL_HANDLE_ENV, V_OD_Env);
		return -1;
	}
	V_OD_erg=SQLTables(hstmt, NULL, 0, (SQLCHAR*)"fvt_interface", 200, (SQLCHAR*)"odbc_sqltables_002", 200, NULL,0);
	if ((V_OD_erg != SQL_SUCCESS) && (V_OD_erg != SQL_SUCCESS_WITH_INFO))
	{
		printf("Error SQLConnect %d\n",V_OD_erg);
		SQLGetDiagRec(SQL_HANDLE_DBC, V_OD_hdbc,1, 
                  V_OD_stat, &V_OD_err,V_OD_msg,100,&V_OD_mlen);
		printf("%s (%d)\n",V_OD_msg,V_OD_err);
		SQLFreeHandle(SQL_HANDLE_ENV, V_OD_Env);
		return -1;
	}
	SQLLEN schemaid,databaseid,tableid;
	rc = SQLBindCol(hstmt,3,SQL_C_CHAR, (SQLPOINTER)table,255,&tableid);
	rc = SQLBindCol(hstmt,2,SQL_C_CHAR, (SQLPOINTER)schema,255,&schemaid);
	rc = SQLBindCol(hstmt,1,SQL_C_CHAR, (SQLPOINTER)database,255,&databaseid);
	rc = SQLFetch(hstmt);  
	while(rc != SQL_NO_DATA)
	{   
		printf("database schema table        ----: %s %s %s\n",database,schema,table);
		rc = SQLFetch(hstmt);  
	};

    SQLDisconnect(V_OD_hdbc);
    SQLFreeHandle(SQL_HANDLE_DBC,V_OD_hdbc);
    SQLFreeHandle(SQL_HANDLE_ENV, V_OD_Env);
    return(0);
}



