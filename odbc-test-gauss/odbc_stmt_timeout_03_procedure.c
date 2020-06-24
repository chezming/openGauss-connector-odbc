/*
测试odbc支持语句执行超时连接设置
test: SQLSetStmtAttr()函数设定5秒超时,SQLprepare和SQLExecute执行调用存储过程（超过5秒），会被终止，再用提示返回信息ERROR: canceling statement due to user request (110)
*/
#include <stdlib.h>
#include <stdio.h>
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <time.h>
SQLHENV       V_OD_Env;    // Handle ODBC environment
SQLHENV       V_OD_hstmt;  // Handle statement
SQLHDBC       V_OD_hdbc;   // Handle connection
SQLRETURN     rc;
SQLLEN	      V_OD_rowanz;
SQLINTEGER    V_OD_err,V_OD_id,ID,IDLEN,typelen;
SQLSMALLINT   V_OD_mlen,V_OD_colanz,col_type,col_num;
long          V_OD_erg;
char          V_OD_stat[10];    // Status SQL
char          V_OD_msg[200],V_OD_buffer[200],type[200],name[200],
 	      typename[200],col_name[200], numattr[200];
int i;
char *buf = "zhuyunlong";
int value = 25;

int main(int argc,char *argv[])
{
    // 1. allocate Environment handle and register version 
    V_OD_erg = SQLAllocHandle(SQL_HANDLE_ENV,SQL_NULL_HANDLE,&V_OD_Env);
    if ((V_OD_erg != SQL_SUCCESS) && (V_OD_erg != SQL_SUCCESS_WITH_INFO))
    {
        printf("Error AllocHandle\n");
        exit(-1);
    }
    SQLSetEnvAttr(V_OD_Env, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0); 
    // 2. allocate connection handle, set timeout
    V_OD_erg = SQLAllocHandle(SQL_HANDLE_DBC, V_OD_Env, &V_OD_hdbc); 
    if ((V_OD_erg != SQL_SUCCESS) && (V_OD_erg != SQL_SUCCESS_WITH_INFO))
    {
        printf("Error AllocHDB %d\n",V_OD_erg);
        SQLFreeHandle(SQL_HANDLE_ENV, V_OD_Env);
        exit(-1);
    }
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
        exit(-1);
    }
    printf("Connected !\n");

    V_OD_erg=SQLAllocHandle(SQL_HANDLE_STMT, V_OD_hdbc, &V_OD_hstmt);
    if ((V_OD_erg != SQL_SUCCESS) && (V_OD_erg != SQL_SUCCESS_WITH_INFO))
    {
        printf("Fehler im AllocStatement %d\n",V_OD_erg);
        SQLGetDiagRec(SQL_HANDLE_DBC, V_OD_hdbc,1, V_OD_stat,&V_OD_err,V_OD_msg,100,&V_OD_mlen);
        printf("%s (%d)\n",V_OD_msg,V_OD_err);
        SQLFreeHandle(SQL_HANDLE_ENV, V_OD_Env);
        exit(-1);
    }
    // set the wairt time is 5 seconds.	
    SQLSetStmtAttr(V_OD_hstmt,SQL_ATTR_QUERY_TIMEOUT,(SQLPOINTER *)5,0);            //set the max timeout 5!

    V_OD_erg = SQLExecDirect(V_OD_hstmt,"create or replace function odbc_sleep(tm int) returns char as $$ BEGIN RETURN  PG_SLEEP(TM);END;$$ LANGUAGE PLPGSQL;",SQL_NTS);         //create function  
    if ((V_OD_erg != SQL_SUCCESS) && (V_OD_erg != SQL_SUCCESS_WITH_INFO))
    {
        SQLGetDiagRec(SQL_HANDLE_DBC, V_OD_hdbc,1, V_OD_stat,&V_OD_err,V_OD_msg,100,&V_OD_mlen);
        printf("%s (%d)\n",V_OD_msg,V_OD_err);
        exit(-1);
    }

    SQLUINTEGER   PartID=10;
    SQLLEN        PartIDInd = 0;		
    SQLBindParameter(V_OD_hstmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0,&PartID, 0, &PartIDInd);
    //prepare
    V_OD_erg = SQLPrepare(V_OD_hstmt,"{call odbc_sleep(?)}",SQL_NTS);		
    V_OD_erg = SQLExecute(V_OD_hstmt);         //  
    if (V_OD_erg != SQL_ERROR)
    {
        printf("Expected: SQL_ERROR with timeout, but not.");
        exit(-1);
    }
    
    SQLGetDiagRec(SQL_HANDLE_DBC, V_OD_hdbc,1, V_OD_stat,&V_OD_err,V_OD_msg,100,&V_OD_mlen);
    printf("%s (%d)\n",V_OD_msg,V_OD_err);
    printf("SQLRowCount ----- Number of Rows %d\n",V_OD_rowanz);

    SQLFreeHandle(SQL_HANDLE_STMT,V_OD_hstmt);
    SQLDisconnect(V_OD_hdbc);
    SQLFreeHandle(SQL_HANDLE_DBC,V_OD_hdbc);
    SQLFreeHandle(SQL_HANDLE_ENV, V_OD_Env);

    printf("GO ON!\n");                                                             
    return(0);
}
