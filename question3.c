// OS assignment 1 : question 3
// grep line number extractor

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    char cmd[2000];

    // extract search term and file names from argv
    char *searchTerm = argv[1];
    char fileNames[1024];
    int pos = 0;
    for (int i = 2; i < argc; i++) {
        int n = strlen(argv[i]);
        for (int j = 0; j < n; j++) fileNames[pos++] = argv[i][j];
        fileNames[pos++] = ' ';
    }

    // build the unix command
    // search for searchTerm in fileNames, cut after : to get numbers and sort numbers to keep unique only
    snprintf(cmd, 2000, "grep -n %s %s | cut -d: -f2 | sort -u -n", searchTerm, fileNames);
    
    // execute
    execlp("sh", "sh", "-c", cmd, NULL);

    return 0;
}
