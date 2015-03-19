/*testPhilippe*/


#include "test.h"
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char** argv) {
    

	if (argc <=2)
	{
		mkdir(argv[1] ,0777);
	}



    return 0;
}