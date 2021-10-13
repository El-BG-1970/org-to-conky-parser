#include "date.h"
#define _BSD_SOURCE

enum month {JAN=1, FEB, MAR, APR, MAY, JUN, JUL, AUG, SEP, OCT, NOV, DEC} month;

Date extract_date(char *str) {
	Date ret;
	sscanf(str, "%d-%d-%d",
		   &ret.year,
		   &ret.month,
		   &ret.day);
	return ret;
}

bool smaller(Date a, Date b) {
	if (zero(b)) return true;
	if (zero(a)) return false;
	return ((a.year < b.year) ||
			((a.year == b.year) && (a.month < b.month)) ||
			((a.year == b.year) && (a.month == b.month) && (a.day <= b.day)));
}

bool eql(Date a, Date b) {
	return (a.day == b.day) &&
		(a.month == b.month) &&
		(a.year == b.year);
}

bool zero(Date a) {
	return ((a.year == 0) ||
			(a.month == 0) ||
			(a.day == 0));
}

void print_date(Date date) {
	printf("%i/%i/%i\n", date.month, date.day, date.year);
}

char *print_date_to_string(Date date) {
	char *ret = (char *)calloc(16, 1);
	sprintf(ret, "%i/%i/%i", date.month, date.day, date.year);
	return ret;
}

Date today() {
	FILE *pipe = popen("date \"+%Y-%m-%d\"", "r");
	char *td = (char *)malloc(12);
	td = fgets(td, 12, pipe);
	Date ret = extract_date(td);

	pclose(pipe);
	free(td);
	return ret;
}

Date nextweek(Date td) {
	Date nw = { td.day+7, td.month, td.year };
	if (nw.month == FEB && nw.day > 28) {
		if (nw.year % 4 == 0 && nw.day > 29) {
			nw.day -= 29; nw.month++;
		} else {
			nw.day -= 28; nw.month++;
		}
	} else if (nw.month == JAN || nw.month == MAR || nw.month == MAY || nw.month == JUL ||
			   nw.month == AUG || nw.month == OCT || nw.month == DEC) {
		if (nw.day > 31 && nw.month == DEC) {
			nw.day -= 31; nw.month = 1;
		} else if (nw.day > 31) {
			nw.day -= 31; nw.month++;
		}
	} else if (nw.month == APR || nw.month == JUN || nw.month == SEP || nw.month == NOV) {
		if (nw.day > 30) {
			nw.day -= 30; nw.month++;
		}
	}
	return nw;
}
