#include <stdio.h>
#include <stdlib.h>

#include "common.h"

int main(int argc, char **argv)
{
    int rc;
    HSTMT hstmt = SQL_NULL_HSTMT;
    HSTMT hstmt2 = SQL_NULL_HSTMT;
    HSTMT hstmts = SQL_NULL_HSTMT;
    char param1[20] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    SQLLEN cbParam1;
    SQLSMALLINT colcount;
    SQLSMALLINT dataType;
    SQLULEN paramSize;
    SQLSMALLINT decDigits;
    SQLSMALLINT nullable;
    SQLUSMALLINT supported;

    test_connect();
    // test_connect_ext("UseDeclareFetch=1;Fetch=1;");

    rc = SQLAllocStmt(conn, &hstmt);
    if (!SQL_SUCCEEDED(rc))
    {
        print_diag("failed to allocate stmt handle", SQL_HANDLE_DBC, conn);
        exit(1);
    }
    rc = SQLAllocStmt(conn, &hstmt2);
    if (!SQL_SUCCEEDED(rc))
    {
        print_diag("failed to allocate stmt handle", SQL_HANDLE_DBC, conn);
        exit(1); 
    }
    rc = SQLAllocStmt(conn, &hstmts);
    if (!SQL_SUCCEEDED(rc))
    {
        print_diag("failed to allocate stmt handle", SQL_HANDLE_DBC, conn);
        exit(1); 
    }
    
    /* Prepare a test table */
    rc = SQLExecDirect(hstmt, (SQLCHAR *) "DROP TABLE IF EXISTS  ORDERS; CREATE TABLE ORDERS (ORDER_NUM INT, AA INT, BB INT, ORDER_DATE DATE, QUANTITY INT); INSERT INTO ORDERS VALUES(5452,104,1001,'1999-01-23',1);INSERT INTO ORDERS VALUES(5453,104,1001,'1999-01-23',2);INSERT INTO ORDERS VALUES(5454,104,1001,'1999-01-23',3);INSERT INTO ORDERS VALUES(5455,104,1001,'1999-01-23',4);INSERT INTO ORDERS VALUES(5456,104,1001,'1999-01-23',5) ", SQL_NTS);
    CHECK_STMT_RESULT(rc, "CREATE SQLExecDirect failed", hstmt);    

    rc = SQLFreeStmt(hstmt, SQL_CLOSE);
    CHECK_STMT_RESULT(rc, "SQLFreeStmt failed", hstmt);


	// rc  = SQLSetStmtAttr(hstmt, SQL_ATTR_CONCURRENCY,
	// 					 (SQLPOINTER) SQL_CONCUR_ROWVER, 0);
	// CHECK_STMT_RESULT(rc, "SQLSetStmtAttr failed", hstmt);
	// rc = SQLSetStmtAttr(hstmt, SQL_ATTR_CURSOR_TYPE,
	// 					(SQLPOINTER) SQL_CURSOR_KEYSET_DRIVEN, 0);
	// CHECK_STMT_RESULT(rc, "SQLSetStmtAttr failed", hstmt);

    rc = SQLSetCursorName(hstmt, (SQLCHAR*)"C111", SQL_NTS);
    CHECK_STMT_RESULT(rc, "hstmt SQLSetCursorName failed", hstmt);

    rc = SQLSetCursorName(hstmt, (SQLCHAR*)"C1", SQL_NTS);
    CHECK_STMT_RESULT(rc, "hstmt SQLSetCursorName failed", hstmt);

    rc = SQLSetCursorName(hstmt, (SQLCHAR*)"C1", SQL_NTS);
    CHECK_STMT_RESULT(rc, "hstmt SQLSetCursorName failed", hstmt);

    rc = SQLSetCursorName(hstmt2, NULL, SQL_NTS);
    if (!SQL_SUCCEEDED(rc)) 
	{ 
		print_diag("hstmt2 SQLSetCursorName failed for NULL", SQL_HANDLE_STMT, hstmt2);									
    }

    rc = SQLSetCursorName(hstmt2, (SQLCHAR*)"C1", SQL_NTS);
    if (!SQL_SUCCEEDED(rc)) 
	{ 
		print_diag("hstmt2 SQLSetCursorName failed for C1", SQL_HANDLE_STMT, hstmt2);									
    }


    rc = SQLSetCursorName(hstmt2, (SQLCHAR*)"c1", SQL_NTS);
    if (!SQL_SUCCEEDED(rc)) 
	{ 
		print_diag("hstmt2 SQLSetCursorName failed for c1", SQL_HANDLE_STMT, hstmt2);									
    }

	char cursornamebuf[20];
	SQLSMALLINT cursornamelen;

    // 从这里开始对conn1进行操作
    char *dsn = "DSN=gaussdb";
    test_connect1(dsn);

    HSTMT conn1_htmt;
    rc = SQLAllocHandle(SQL_HANDLE_STMT, conn1, &conn1_htmt);
	CHECK_STMT_RESULT(rc, "SQLAllocHandle failed", conn1_htmt);

    rc = SQLSetCursorName(conn1_htmt, (SQLCHAR*)"C1", SQL_NTS);
    CHECK_STMT_RESULT(rc, "SQLSetCursorName failed", conn1_htmt);

    rc = SQLGetCursorName(conn1_htmt, (SQLCHAR*) cursornamebuf, sizeof(cursornamebuf), &cursornamelen);
	CHECK_STMT_RESULT(rc, "SQLGetCursorName failed", conn1_htmt);

    printf("conn1_htmt cusorname is %s\n", cursornamebuf);

    rc = SQLFreeHandle(SQL_HANDLE_STMT, conn1_htmt);
	CHECK_STMT_RESULT(rc, "SQLFreeHandle failed", conn1_htmt);

    test_disconnect1();

	rc = SQLGetCursorName(hstmt, (SQLCHAR*) cursornamebuf, sizeof(cursornamebuf), &cursornamelen);
	CHECK_STMT_RESULT(rc, "SQLGetCursorName failed", hstmt);

    printf("hstmt cusorname is %s\n", cursornamebuf);

    rc = SQLGetCursorName(hstmt2, (SQLCHAR*) cursornamebuf, sizeof(cursornamebuf), &cursornamelen);
	CHECK_STMT_RESULT(rc, "SQLGetCursorName failed", hstmt2);
    printf("hstmt2 cusorname is %s\n", cursornamebuf);

    rc = SQLExecDirect(hstmt, (SQLCHAR *) "SELECT * FROM ORDERS;", SQL_NTS);
    CHECK_STMT_RESULT(rc, "1 SQLExecDirect failed", hstmt);

    char buf[40];
    SQLLEN ind,ind2;
    rc = SQLBindCol(hstmt,1,SQL_C_CHAR, (SQLPOINTER)&buf,sizeof(buf),&ind);
    CHECK_STMT_RESULT(rc, "SQLFetch failed", hstmt);
    SQLINTEGER quantity;
    rc = SQLBindCol(hstmt,5,SQL_C_SHORT, (SQLPOINTER)&quantity,sizeof(quantity),&ind2);
    CHECK_STMT_RESULT(rc, "SQLFetch quantity failed", hstmt);
    /* Fetch until we find the row with orderNum=5454 */
    do {
        rc = SQLFetch(hstmt);
        CHECK_STMT_RESULT(rc, "SQLFetchScroll failed", hstmt);
        printf("current ORDERNUM=%s, QUANTITY=%d\n", buf, quantity);
        // rc = SQLGetData(hstmt, 1, SQL_C_CHAR, buf, sizeof(buf), &ind);
        // CHECK_STMT_RESULT(rc, "SQLGetData failed", hstmt);

    } while(strcmp(buf, "5454") != 0);

    rc = SQLExecDirect(hstmt2, (SQLCHAR *) "UPDATE ORDERS SET QUANTITY=1234567 WHERE CURRENT OF C1", SQL_NTS);
    if (!SQL_SUCCEEDED(rc)) 
	{ 
		print_diag("hstmt2 update WHERE CURRENT OF C1 failed", SQL_HANDLE_STMT, hstmt2);									
    }

	rc = SQLFreeStmt(hstmts, SQL_CLOSE);
	CHECK_STMT_RESULT(rc, "SQLFreeStmt failed", hstmts);

    rc = SQLExecDirect(hstmts, (SQLCHAR *) "select * from orders", SQL_NTS);
    if (!SQL_SUCCEEDED(rc)) 
	{ 
		print_diag("hstmt select * from orders failed", SQL_HANDLE_STMT, hstmts);									
    }
    print_result(hstmts);

    rc = SQLFetch(hstmt);
    CHECK_STMT_RESULT(rc, "SQLFetchScroll failed", hstmt);

HSTMT hstmt5 = SQL_NULL_HSTMT;
rc = SQLAllocStmt(conn, &hstmt5);
    rc = SQLExecDirect(hstmt5, (SQLCHAR *) "delete from orders WHERE CURRENT OF C1", SQL_NTS);
    if (!SQL_SUCCEEDED(rc)) 
	{ 
		print_diag("hstmt5 delete WHERE CURRENT OF C1 failed ", SQL_HANDLE_STMT, hstmt5);		
        printf("rc=%d\n", rc);							
    }



	rc = SQLFreeStmt(hstmts, SQL_CLOSE);
	CHECK_STMT_RESULT(rc, "SQLFreeStmt failed", hstmts);

    rc = SQLExecDirect(hstmts, (SQLCHAR *) "select * from orders", SQL_NTS);
    if (!SQL_SUCCEEDED(rc)) 
	{ 
		print_diag("hstmt select * from orders failed", SQL_HANDLE_STMT, hstmts);									
    }

    print_result(hstmts);


HSTMT hstmt3 = SQL_NULL_HSTMT;
HSTMT hstmt4 = SQL_NULL_HSTMT;
        rc = SQLAllocStmt(conn, &hstmt3);
    if (!SQL_SUCCEEDED(rc))
    {
        print_diag("failed to allocate stmt handle", SQL_HANDLE_DBC, conn);
        exit(1);
    }
    rc = SQLAllocStmt(conn, &hstmt4);
    if (!SQL_SUCCEEDED(rc))
    {
        print_diag("failed to allocate stmt handle", SQL_HANDLE_DBC, conn);
        exit(1); 
    }


    rc = SQLSetCursorName(hstmt3, (SQLCHAR*)"\"select C1\"", SQL_NTS);
    CHECK_STMT_RESULT(rc, "SQLSetCursorName failed", hstmt3);

    rc = SQLGetCursorName(hstmt3, (SQLCHAR*) cursornamebuf, sizeof(cursornamebuf), &cursornamelen);
	CHECK_STMT_RESULT(rc, "SQLGetCursorName failed", hstmt3);

    printf("hstmt3 cusorname is %s\n", cursornamebuf);

    rc = SQLSetCursorName(hstmt4, (SQLCHAR*)"\"SELECT C1\"", SQL_NTS);
    CHECK_STMT_RESULT(rc, "SQLSetCursorName failed", hstmt4);

    rc = SQLGetCursorName(hstmt4, (SQLCHAR*) cursornamebuf, sizeof(cursornamebuf), &cursornamelen);
	CHECK_STMT_RESULT(rc, "SQLGetCursorName failed", hstmt4);

    rc = SQLFreeStmt(hstmt, SQL_CLOSE);
    CHECK_STMT_RESULT(rc, "12 SQLFreeStmt failed", hstmt);
    rc = SQLFreeStmt(hstmt2, SQL_CLOSE);
    CHECK_STMT_RESULT(rc, "12 SQLFreeStmt failed", hstmt);
    /* Clean up */
    test_disconnect();
    return 0;
}
