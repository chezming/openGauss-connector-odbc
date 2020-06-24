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
	
	rc = SQLExecDirect(hstmt,"drop table IF EXISTS FVT_INTERFACE.odbc_SQLDescribeParam",SQL_NTS);
	if ((!SQL_SUCCEEDED(rc)))
	{
		printf("Error  SQLExecDirect %d\n",rc);
		SQLGetDiagRec(SQL_HANDLE_STMT, hstmt,1, 
                  V_OD_stat, &V_OD_err,V_OD_msg,100,&V_OD_mlen);
		printf("%s (%d)\n",V_OD_msg,V_OD_err);
		SQLFreeHandle(SQL_HANDLE_ENV, V_OD_Env);
		return -1;
	}
	
   rc = SQLExecDirect(hstmt,"create table FVT_INTERFACE.odbc_SQLDescribeParam(tm time)",SQL_NTS);
	if ((!SQL_SUCCEEDED(rc)))
	{
		printf("Error  SQLExecDirect %d\n",rc);
		SQLGetDiagRec(SQL_HANDLE_STMT, hstmt,1, 
                  V_OD_stat, &V_OD_err,V_OD_msg,100,&V_OD_mlen);
		printf("%s (%d)\n",V_OD_msg,V_OD_err);
		SQLFreeHandle(SQL_HANDLE_ENV, V_OD_Env);
		return -1;
	}

  SQLPrepare(hstmt,"insert into FVT_INTERFACE.odbc_SQLDescribeParam values(?)",SQL_NTS);
  SQLSMALLINT spe,scal;
  SQLULEN  pre;
  rc = SQLDescribeParam (hstmt,1,&spe,(SQLULEN*)pre,&scal,SQL_NO_NULLS);
  if ((!SQL_SUCCEEDED(rc)))
	{
		printf("Error  SQLDescribeParam %d\n",rc);
		SQLGetDiagRec(SQL_HANDLE_STMT, hstmt,1, 
                  V_OD_stat, &V_OD_err,V_OD_msg,100,&V_OD_mlen);
		printf("%s (%d)\n",V_OD_msg,V_OD_err);
		SQLFreeHandle(SQL_HANDLE_ENV, V_OD_Env);
		return -1;
	}
	printf("spe----: %d\n",spe);					

    SQLDisconnect(V_OD_hdbc);
    SQLFreeHandle(SQL_HANDLE_DBC,V_OD_hdbc);
    SQLFreeHandle(SQL_HANDLE_ENV, V_OD_Env);
    return(0);
}



