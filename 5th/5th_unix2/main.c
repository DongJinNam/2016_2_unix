#include <stdio.h>
#include "copyfile.h"

int main(int argc, char *argv[]) {
	if (argc!=3) {
		printf("invalid arguments\n");
		return -1;
	}
	copyfile(argv[1],argv[2]);
	return 0;
}
