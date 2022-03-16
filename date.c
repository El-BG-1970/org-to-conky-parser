#include "date.h"
#define _BSD_SOURCE

enum month {JAN=1, FEB, MAR, APR, MAY, JUN, JUL, AUG, SEP, OCT, NOV, DEC} month;
enum weekday {SUN=0, MON, TUE, WED, THU, FRI, SAT} weekday;

Date extract_date(char *str) {
	Date ret;
	sscanf(str, "%d-%d-%d",
		   &ret.year,
		   &ret.month,
		   &ret.day);
	return ret;
}

Date adjust_date(Date date) {
	if (date.month == FEB && date.day > 28) {
		if (date.year % 4 == 0 && date.day > 29) {
			date.day -= 29; date.month++;
		} else {
			date.day -= 28; date.month++;
		}
	} else if (date.month == JAN || date.month == MAR || date.month == MAY || date.month == JUL ||
			   date.month == AUG || date.month == OCT || date.month == DEC) {
		if (date.day > 31 && date.month == DEC) {
			date.day -= 31; date.month = 1;
		} else if (date.day > 31) {
			date.day -= 31; date.month++;
		}
	} else if (date.month == APR || date.month == JUN || date.month == SEP || date.month == NOV) {
		if (date.day > 30) {
			date.day -= 30; date.month++;
		}
	}
	return date;
}

bool smaller(Date a, Date b) {
	if (zero(b)) return true;
	if (zero(a)) return false;
	return ((a.year < b.year) ||
			((a.year == b.year) && (a.month < b.month)) ||
			((a.year == b.year) && (a.month == b.month) && (a.day <= b.day)));
}

bool strictly_smaller(Date a, Date b) {
	if (zero(b)) return true;
	if (zero(a)) return false;
	return ((a.year < b.year) ||
			((a.year == b.year) && (a.month < b.month)) ||
			((a.year == b.year) && (a.month == b.month) && (a.day < b.day)));
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

Date tomorrow(Date td) {
	Date tm = { td.day+1, td.month, td.year };
    return adjust_date(tm);
}

Date nextweek(Date td) {
	Date nw = { td.day+7, td.month, td.year };
	return adjust_date(nw);
}

Date nextmonth(Date td) {
	Date nm = { td.day, td.month+1, td.year };
	return adjust_date(nm);
}
