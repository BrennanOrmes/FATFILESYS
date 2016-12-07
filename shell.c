#include "filesys.h"

// This is to sort the formating for the write disk function
// D Level
void cgs_D(){ // Format function

	format(); // Call format from shell.c
	writedisk("virtualdiskD3_D1");
}

int main(){
	cgs_D(); // Calls the format function (cgs_D)
	return 0;
}