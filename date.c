#include "date.h"
#define _BSD_SOURCE

enum month {JAN=1, FEB, MAR, APR, MAY, JUN, JUL, AUG, SEP, OCT, NOV, DEC} month;
enum weekday {SUN=0, MON, TUE, WED, THU, FRI, SAT} weekday;

int get_weekday_from_string(char *str) {
	if (!strcmp(str, "Mon")) return MON;
	else if (!strcmp(str, "Tue")) return TUE;
	else if (!strcmp(str, "Wed")) return WED;
	else if (!strcmp(str, "Thu")) return THU;
	else if (!strcmp(str, "Fri")) return FRI;
	else if (!strcmp(str, "Sat")) return SAT;
	else return SUN;
}

char *weekday_to_string(int weekday) {
	switch (weekday) {
	case SUN: return "Sun";
	case MON: return "Mon";
	case TUE: return "Tue";
	case WED: return "Wed";
	case THU: return "Thu";
	case FRI: return "Fri";
	case SAT: return "Sat";
	default: return "";
	}
}

Date extract_date(struct tm *localtm) {
	Date ret;
	ret.year = 1900 + localtm->tm_year;
	ret.month = 1 + localtm->tm_mon;
	ret.day = localtm->tm_mday;
	ret.weekday = localtm->tm_wday;
	return ret;
}

Date extract_date_from_string(char *str) {
	Date ret;
	sscanf(str, "%d-%d-%d",
		   &ret.year,
		   &ret.month,
		   &ret.day);
	ret.weekday = get_weekday_from_string(&str[11]);
	return ret;
}

Date adjust_date(Date date) {
	if (date.month == FEB && date.day > 28) {
		if (date.year % 4 == 0 && date.day > 29) {
			date.day -= 29; date.month++; date.weekday += 29;
		} else {
			date.day -= 28; date.month++; date.weekday += 29;
		}
	} else if (date.month == JAN || date.month == MAR || date.month == MAY || date.month == JUL ||
			   date.month == AUG || date.month == OCT || date.month == DEC) {
		if (date.day > 31 && date.month == DEC) {
			date.day -= 31; date.month = 1; date.weekday += 31;
		} else if (date.day > 31) {
			date.day -= 31; date.month++; date.weekday += 31;
		}
	} else if (date.month == APR || date.month == JUN || date.month == SEP || date.month == NOV) {
		if (date.day > 30) {
			date.day -= 30; date.month++; date.weekday += 30;
		}
	}
	date.weekday = date.weekday % 7;
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
	char *wd = weekday_to_string(date.weekday);
	printf("%s %i/%i/%i\n",
		   wd,
		   date.month, date.day, date.year);
}

char *print_date_to_string(Date date) {
	char *ret = (char *)calloc(16, 1);
	char *wd = weekday_to_string(date.weekday);
	sprintf(ret, "%s %i/%i/%i",
			wd,
			date.month, date.day, date.year);
	return ret;
}

char *short_date_to_string(Date date) {
	char *ret = (char *)calloc(16, 1);
	sprintf(ret, "%i/%i/%i",
			date.month, date.day, date.year);
	return ret;
}

Date today() {
	Date ret;
	time_t tm;
	struct tm localtm;
	struct tm *ltm = &localtm;

	time(&tm);
	ltm = localtime(&tm);

	ret = extract_date(ltm);
	return ret;
}

Date tomorrow(Date td) {
	Date tm = { td.day+1, td.month, td.year, td.weekday+1 };
    return adjust_date(tm);
}

Date nextweek(Date td) {
	Date nw = { td.day+7, td.month, td.year, td.weekday };
	return adjust_date(nw);
}

Date nextmonth(Date td) {
	Date nm = { td.day, td.month+1, td.year, td.weekday };
	return adjust_date(nm);
}

Date nextmonday(Date td) {
	if (td.weekday == MON) return nextweek(td);
	else if (td.weekday == SUN) return adjust_date((Date){ td.day+1, td.month, td.year, MON });
	else return adjust_date((Date){ td.day+(7-td.weekday)+1, td.month, td.year, MON });
}
