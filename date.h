#ifndef __DATE_H__
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Date {
	int day;
	int month;
	int year;
} Date;

Date extract_date(char *str);
Date adjust_date(Date date);
bool smaller(Date a, Date b);
bool strictly_smaller(Date a, Date b);
bool eql(Date a, Date b);
bool zero(Date a);

void print_date(Date date);
char *print_date_to_string(Date date);

Date today();
Date tomorrow(Date td);
Date nextweek(Date td);
Date nextmonth(Date td);

#define __DATE_H__
#endif
