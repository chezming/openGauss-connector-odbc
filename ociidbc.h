#ifndef __OCIODBC_H__
#define __OCIODBC_H__

#define SQL_OCI_DATA -97
#define SQL_OCI_DATETIME -96

typedef signed char sb1;
typedef unsigned char ub1;

typedef signed short sb2;
typedef unsigned int ub4;

struct OCITime
{
	ub1 OCITimeHH;                     /* hours; range is 0 <= hours <=23 */
	ub1 OCITimeMI;                     /* minutes; range is 0 <= minutes <= 59 */
	ub1 OCITimeSS;                     /* seconds; range is 0 <= seconds <= 59 */
};
typedef struct OCITime OCITime;

/*
 * OCITime - OCI TiMe portion of date
 *
 * This structure should be treated as an opaque structure as the format
 * of this structure may change. Use OCIDateGetTime/OCIDateSetTime
 * to manipulate time portion of OCIDate.
 */

struct OCIDate
{
	sb2 OCIDateYYYY;         /* gregorian year; range is -4712 <= year <= 9999 */
	ub1 OCIDateMM;                          /* month; range is 1 <= month < 12 */
	ub1 OCIDateDD;                             /* day; range is 1 <= day <= 31 */
	OCITime OCIDateTime;                                               /* time */
};
typedef struct OCIDate OCIDate;

typedef struct Node
{
	ub4             type;
} Node;

typedef unsigned char oratext;
struct OCIDateTime
{
	Node type;           /* 描述符类型 */

	sb2 YYYY;         /* gregorian year; range is -4712 <= year <= 9999 */
	ub1 MM;                          /* month; range is 1 <= month < 12 */
	ub1 DD;                             /* day; range is 1 <= day <= 31 */
	ub1 HH;                          /* hours; range is 0 <= hours <=23 */
	ub1 MI;                     /* minutes; range is 0 <= minutes <= 59 */
	ub1 SS;                     /* seconds; range is 0 <= seconds <= 59 */
	ub4 fraction;
	oratext zone[20];
};
typedef struct OCIDateTime OCIDateTime;

#endif /* OCI_UTILS_H */