//
// Created by gaugendre on 19/03/15.
//

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <git2.h>
#include <string.h>

#define false 0

int main(int argc, char** argv) {
    int success_create = 0;
    int error = 0;
    git_repository *repo = NULL;
    FILE *readme = NULL;

    git_threads_init();

    /* Check if we have the right number of arguments */
    if (argc < 2 || argc > 3) {
        perror("Usage : mkgit <directory> [\"<Readme message>\"]");
        return 2;
    }

    /* Creating folder and check if no error. */
    success_create = mkdir(argv[1], 0775);

    if (success_create != 0) {
        perror("Error while creating folder");
        return 1;
    }

    /* Initializing git repository in newly created folder.
    * NO IMPLEMENTED ERROR CHECKING HERE. */
    error = git_repository_init(&repo, argv[1], false);

    /* If we have a README message, we have to create a file and
    * fill it with some text. */
    if (argc == 3) {
        char *readme_path = malloc(sizeof(char) * 1024);
        char *line = malloc(sizeof(char) * 1024);

        /* Building string for README filename,
        * creating file and checking errors. */
        strcpy(readme_path, argv[1]);
        strcat(readme_path, "/README.md");
        readme = fopen(readme_path, "w+");

        if (readme == NULL) {
            perror("Error while opening README file");
            return 4;
        }

        /* Building string for README title (first line)
        * and checking errors. */
        strcpy(line, "# ");
        strcat(line, argv[1]);
        strcat(line, "\n");
        error = fputs(line, readme);
        if (error == EOF) {
            perror("Error while writing README file");
            return 3;
        }

        /* Building string for README message (second line)
        * and checking errors. */
        strcpy(line, argv[2]);
        strcat(line, "\n");
        error = fputs(line, readme);
        if (error == EOF) {
            perror("Error while writing README file");
            return 3;
        }

        /* Closing file. */
        fclose(readme);
    }

    git_threads_shutdown();

    return 0;
}