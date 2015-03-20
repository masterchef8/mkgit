//
// Created by gaugendre on 19/03/15.
//

#include "test.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <git2.h>
#include <string.h>

#define false 0

int main(int argc, char** argv) {
    int success_create = 0;
    git_repository *repo = NULL;
    int error = 0;
    FILE *readme = NULL;

    if (argc < 2 || argc > 3) {
        perror("Usage : mkdir <directory> [\"<Readme message>\"]");
        return 2;
    }

    success_create = mkdir(argv[1], 0775);

    if (success_create != 0) {
        perror("Error while creating folder");
        return 1;
    }

    error = git_repository_init(&repo, argv[1], false);

    if (argc == 3) {
        char *readme_path = malloc(sizeof(char) * 1024);
        char *line = malloc(sizeof(char) * 1024);
        strcpy(readme_path, argv[1]);
        strcat(readme_path, "/README.md");
        readme = fopen(readme_path, "w+");

        if (readme == NULL) {
            perror("Error while opening README file");
            return 4;
        }

        strcpy(line, "# ");
        strcat(line, argv[1]);
        strcat(line, "\n");
        error = fputs(line, readme);
        if (error == EOF) {
            perror("Error while writing README file");
            return 3;
        }

        strcpy(line, argv[2]);
        strcat(line, "\n");
        error = fputs(line, readme);
        if (error == EOF) {
            perror("Error while writing README file");
            return 3;
        }

        fclose(readme);
    }

    return 0;
}