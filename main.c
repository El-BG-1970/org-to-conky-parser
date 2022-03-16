#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "agenda_entry.h"
#define BS 1024 //blocksize for buffers

void destroy_entry_array(entry *array, int elements) {
    for (int i = 0; i < elements; i++)
        destroy_entry(array[i]);
    free(array);
}

char *read_file_to_buffer(char *filename) {
    int fd = open(filename, O_RDONLY);
	if (fd < 0) return NULL; // if error opening file, return NULLptr
    int bufsize = BS,
        bufread = 0;
    char *buf = (char *)malloc(bufsize);
    char *newbuf;

    while (read(fd, buf + bufread, BS) == BS) {
        newbuf = (char *)malloc(bufsize + BS);
        strncpy(newbuf, buf, bufsize);
        free(buf);
        buf = newbuf;
        bufsize += BS;
        bufread += BS;
    }
	close(fd);
    return buf;
}

entry *read_entries_to_array(char *buffer, int *entries) {
    int numentries = 5,
        idx = 0;
    entry *ret = (entry *)malloc(numentries * sizeof(entry));
    char *cursor = buffer;
    while (cursor) {
        if (idx == numentries) {
            entry *newret = (entry *)malloc((numentries + 5) * sizeof(entry));
            memcpy(newret, ret, (idx)*sizeof(entry));
            free(ret);
            ret = newret;
            numentries += 5;
        }
        ret[idx++] = read_agenda_entry(cursor);
        cursor = next_entry(cursor);
    }
    *entries = idx;
    return ret;
}

int main(int argc, char **argv) {
    // unbuffering stdout in order to prevent leaks
    // has the added benefit of reducing memory usage too
    setvbuf(stdout, NULL, _IONBF, 0);
    if (argc <= 1) {
        printf("no agenda file supplied!\nUsage: otc <agendafile>\n");
        return -1;
    }

    // read file into buf 
    char *buf = read_file_to_buffer(argv[1]);
	if (!buf){ // abort if file could not be opened
	  fprintf(stderr, "Error: file \"%s\" does not exist!\n", argv[1]);
	  return -1;
	}
    if (strlen(buf) == 0) return -1; // don't do empty buffers

    // read entries into array
    int idx = 0;
    entry *agenda = read_entries_to_array(buf, &idx);
    free(buf);

    // sort entries
    sort_entry_array(agenda, idx);

    // format nicely
    char *e;
    Date td = today();
    Date tm = tomorrow(td);
    Date end = nextmonth(td);
    Date last_date = (struct Date){ -1, -1, -1 };
    for (int i = 0; i < idx; i++) {
        if (!eql(agenda[i].date, last_date)) {
            last_date = agenda[i].date;
            if (!zero(last_date)) {
                e = print_date_to_string(last_date);
                if (strictly_smaller(last_date, td))
                    printf("\n%s [OUTATIME]:\n", e);
                else if (eql(last_date, td))
                    printf("\n%s [TODAY]:\n", e);
                else if (eql(last_date, tm))
                    printf("\n%s [TOMORROW]:\n", e);
                else if (smaller(last_date, end))
                    printf("\n%s:\n", e);
                free(e);
            } else {
                printf("\nUnscheduled:\n");
            }
        }
        e = format_entry(agenda[i]);
        if (zero(agenda[i].date) || smaller(agenda[i].date, end)) puts(e);
        free(e);
    }

    // free up the memory
    fclose(stdout);
    destroy_entry_array(agenda, idx);
    return 0;
}
