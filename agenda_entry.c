#include "utils.h"
#include "agenda_entry.h"

char *next_word(char *str) { return strstr(str, " ")+1; }
char *next_line(char *str) { return strstr(str, "\n")+1; }
char *next_entry(char *str) {
    char *tmp = strstr(str, "\n*");
    if (tmp) tmp += 1;
    return tmp; 
}

entry read_agenda_entry(char *agenda) {
    entry agenda_entry;
    char *cursor = agenda;
    char *lookahead, *tmp;
    char *nextentry = next_entry(cursor);
    if (nextentry == NULL)
        nextentry = agenda + strlen(agenda) - 1;

    while ((*cursor == '\n') ||
            (*cursor == '\t') ||
            (*cursor == ' '))
        cursor++;
    while (*(cursor++) == '*'); // trim the * out

    //find out if there is a tag, and extract it
    lookahead = strstr(cursor, ":\n");
    if (lookahead && (lookahead < nextentry)) {
        tmp = rstrchr(cursor, lookahead-1, ':');
        agenda_entry.tag = (char *)malloc(lookahead - tmp);
        strncpy(agenda_entry.tag, tmp+1, lookahead - tmp - 1);
        agenda_entry.tag[lookahead - tmp - 1] = '\0'; // here we're actually modifying lookahead-tmp cause the array is zero indexed
        lookahead = tmp;
    } else {
        lookahead = strstr(cursor, "\n");
        agenda_entry.tag = NULL;
    }
    agenda_entry.title = (char *)malloc(lookahead - cursor + 1);
    strncpy(agenda_entry.title, cursor, lookahead - cursor);
    agenda_entry.title[lookahead - cursor] = '\0';  // here we're actually modifying lookahead-cursor cause the array is zero indexed

    //find out if there is a deadline and extract it
    cursor = next_line(cursor);
    if (((lookahead = strstr(cursor, "DEADLINE:")) &&
                (lookahead < nextentry)) ||
            ((lookahead = strstr(cursor, "SCHEDULED:")) &&
             (lookahead < nextentry))) {
        //there is a deadline or event is scheduled
        tmp = strstr(lookahead, ">");
        lookahead = strstr(lookahead, "<");
        char *date = (char *)malloc(tmp - lookahead + 1);
        strncpy(date, lookahead+1, tmp - lookahead - 1);
        agenda_entry.date = extract_date(date);
        free(date);
    } else {
        agenda_entry.date = (Date){ 0, 0, 0 };
    }

    return agenda_entry;
}

char *format_entry(entry e) {
    char *ret = calloc(125, 1);
    if (e.tag)
        snprintf(ret, 124, "   * %s \t:%s:", e.title, e.tag);
    else
        snprintf(ret, 124, "   * %s", e.title);
    return ret;
}

void sort_entry_array(entry *arr, int n) {
    merge_sort_entry_array(arr, n);
}

void merge_sort_entry_array(entry *arr, int n) {
    if (n == 1) return;

    int halfway = n/2;
    merge_sort_entry_array(arr, halfway);
    merge_sort_entry_array(arr+halfway, n - halfway);

    merge_entry_array(arr, halfway, n);
}

void merge_entry_array(entry *arr, int halfway, int n) {
    int i = 0,
        j = halfway;
    entry *tmp;
    entry sw;
    while ((i < j) && (j < n)) {
        if (smaller(arr[i].date, arr[j].date))
            i++;
        else {
            tmp = arr + j - 1;
            while (tmp >= arr + i) {
                sw = tmp[1];
                tmp[1] = tmp[0];
                tmp[0] = sw;
                tmp--;
            }
            i++;
            j++;
        }
    }
}

void print_entry(entry e) {
    char *tmp = print_date_to_string(e.date);
    printf("%s\n%s\n>%s\n",
            e.title,
            e.tag,
            tmp);
    free(tmp);
}

void destroy_entry(entry e) {
    free(e.tag);
    free(e.title);
}
