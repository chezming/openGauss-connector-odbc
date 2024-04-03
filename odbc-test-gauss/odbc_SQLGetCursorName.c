#include <stdio.h>
#include <stdlib.h>

#include "common.h"

int main(int argc, char **argv)
{
    int rc;
    HSTMT hstmt = SQL_NULL_HSTMT;
    HSTMT hstmt2 = SQL_NULL_HSTMT;
    char param1[20] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    SQLLEN cbParam1;
    SQLSMALLINT colcount;
    SQLSMALLINT dataType;
    SQLULEN paramSize;
    SQLSMALLINT decDigits;
    SQLSMALLINT nullable;
    SQLUSMALLINT supported;

    test_connect();

    rc = SQLAllocStmt(conn, &hstmt);
    if (!SQL_SUCCEEDED(rc))
    {
        print_diag("failed to allocate stmt handle", SQL_HANDLE_DBC, conn);
        exit(1);
    }

    rc = SQLSetCursorName(hstmt, (SQLCHAR*)"C1", SQL_NTS);
    CHECK_STMT_RESULT(rc, "hstmt SQLSetCursorName failed", hstmt);

    char cursornamebuf[100];
    SQLSMALLINT cursornamelen;

    rc = SQLGetCursorName(hstmt, cursornamebuf, 100, &cursornamelen);
    CHECK_STMT_RESULT(rc, "SQLGetCursorName failed", hstmt);
    printf("current cursorname is %s, len is %d\n", cursornamebuf, cursornamelen);


    /* 自动生成SQL_CUR开头的游标名 */
    HSTMT hstmt3 = SQL_NULL_HSTMT;
    rc = SQLAllocStmt(conn, &hstmt3);
    if (!SQL_SUCCEEDED(rc))
    {
        print_diag("failed to allocate stmt handle", SQL_HANDLE_DBC, conn);
        exit(1);
    }

    rc = SQLExecDirect(hstmt3, (SQLCHAR *) "select * from pg_amop", SQL_NTS);
    CHECK_STMT_RESULT(rc, "SQLExecDirect failed", hstmt3);

    rc = SQLGetCursorName(hstmt3, cursornamebuf, 100, &cursornamelen);
    CHECK_STMT_RESULT(rc, "SQLGetCursorName failed", hstmt3);
    printf("current 2 cursorname is %s, len is %d\n", cursornamebuf, cursornamelen);

    /* get长度截断 */
    HSTMT hstmt4 = SQL_NULL_HSTMT;
    rc = SQLAllocStmt(conn, &hstmt4);
    if (!SQL_SUCCEEDED(rc))
    {
        print_diag("failed to allocate stmt handle", SQL_HANDLE_DBC, conn);
        exit(1);
    }

    
    rc = SQLSetCursorName(hstmt4, (SQLCHAR*)"A1B2C3D4E5", SQL_NTS);
    CHECK_STMT_RESULT(rc, "hstmt SQLSetCursorName failed", hstmt);

    rc = SQLGetCursorName(hstmt4, cursornamebuf, 10, &cursornamelen);
    CHECK_STMT_RESULT(rc, "SQLGetCursorName failed", hstmt4);
    printf("current 3 cursorname is %s, len is %d\n", cursornamebuf, cursornamelen);


    /* 错误用例 */
    rc = SQLGetCursorName(hstmt, cursornamebuf, -1, &cursornamelen);
    PRINT_STMT_RESULT(rc, "SQLGetCursorName failed", hstmt);

    memset(cursornamebuf, 0, sizeof(cursornamebuf));
    rc = SQLGetCursorName(NULL, cursornamebuf, 100, &cursornamelen);
    printf("rc need to be -2: %d\n", rc);

    memset(cursornamebuf, 0, sizeof(cursornamebuf));
    rc = SQLGetCursorName(hstmt, NULL, 100, &cursornamelen);
    if (!SQL_SUCCEEDED(rc))
    {
        print_diag("failed to SQLGetCursorName", SQL_HANDLE_STMT, hstmt);
        printf("rc=%d\n", rc);
    }

    memset(cursornamebuf, 0, sizeof(cursornamebuf));
    cursornamelen = 0;
    rc = SQLGetCursorName(hstmt, cursornamebuf, 100, NULL);
    PRINT_STMT_RESULT(rc, "SQLGetCursorName failed", hstmt);
    printf("rc %d current  cursorname is %s, len is %d\n", rc, cursornamebuf, cursornamelen);

    rc = SQLFreeStmt(hstmt, SQL_CLOSE);
    CHECK_STMT_RESULT(rc, "12 SQLFreeStmt failed", hstmt);

    /* Clean up */
    test_disconnect();
    return 0;
}
