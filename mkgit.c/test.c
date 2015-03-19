//
// Created by gaugendre on 19/03/15.
//

#include "test.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>

int main(int argc, char** argv) {
    int success_create = 0;

    if (argc < 2 || argc > 3) {
        perror("Usage : mkdir <directory> [\"<Readme message>\"]");
        return 1;
    }

    success_create = mkdir(argv[1], 0775);
    if (success_create == 0 && argc == 2)
        return 0;

    if (success_create != 0) {
        perror("Error while creating folder.");
        return -1;
    }

    return 0;
}