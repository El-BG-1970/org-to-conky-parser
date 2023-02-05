#pragma once
#include <stdlib.h>
#include <string.h>
#include "date.h"

typedef struct entry {
	//struct entry *parent;
	//struct entry *children;
	//int8_t level;// nesting level of the entry
	//char *todo;  // TODO keyword
	Date date;  // scheduled or deadline date
	char *tag;   // tag keyword
	char *title; // the actual entry
	//char *text;  // description (if there is one)
} entry;

char *next_word(char *str);
char *next_line(char *str);
char *next_entry(char *str);

entry read_agenda_entry(char *agenda);
char *format_entry(entry e);

void merge_entry_array(entry *arr, int halfway, int n);
void insertion_sort_entry_array(entry *arr, int n);
void merge_sort_entry_array(entry *arr, int n);
void sort_entry_array(entry *arr, int n);
void destroy_entry_array(entry *arr, int n);

void print_entry(entry e);
void destroy_entry(entry e);
