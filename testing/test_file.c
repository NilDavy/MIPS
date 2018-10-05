#include <stdio.h>

int main ( int argc, char *argv[] ){
	
	char *file 	= NULL;
	if ( argc <2 ) {
		print_usage(argv[0]);
		exit( EXIT_FAILURE );
	}
	
	
	file  	= argv[argc-1];
	
	
	if ( NULL == file ) {
		fprintf( stderr, "Missing ASM source file, aborting.\n" );
		exit( EXIT_FAILURE );
	}
	
	
}
