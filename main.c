#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

long int hex_to_dec(char*hex){
/*	unsigned long res=0;
	int i;
	for (i=a;i<strlen(hex);i++){
//		printf("res=%lu\n",res);
//		printf("hex = %c\n",hex[i]);
//		printf("pow = %d\n",strlen(hex)-1-i);
//		printf("str = %d, i=  %d\n",strlen(hex),i);
		switch (hex[i]) {
			case '1':
				res=res+pow(16, strlen(hex)-1-i);
				break;
			case '2':
				res=res+2*pow(16, strlen(hex)-1-i);
				break;
			case '3':
				res=res+3*pow(16, strlen(hex)-1-i);
				break;
			case '4':
				res=res+4*pow(16, strlen(hex)-1-i);
				break;
			case '5':
				res=res+5*pow(16, strlen(hex)-1-i);
				break;
			case '6':
				res=res+6*pow(16, strlen(hex)-1-i);
				break;
			case '7':
				res=res+7*pow(16, strlen(hex)-1-i);
				break;
			case '8':
				res=res+8*powl(16, strlen(hex)-1-i);
				break;
			case '9':
				res=res+9*pow(16, strlen(hex)-1-i);
				break;
			case 'a':
				res=res+10*pow(16, strlen(hex)-1-i);
				break;
			case 'A':
				res=res+10*pow(16, strlen(hex)-1-i);
				break;
			case 'b':
				res=res+11*pow(16, strlen(hex)-1-i);
				break;
			case 'B':
				res=res+11*pow(16, strlen(hex)-1-i);
				break;
			case 'c':
				res=res+12*pow(16, strlen(hex)-1-i);
				break;
			case 'C':
				res=res+12*pow(16, strlen(hex)-1-i);
				break;
			case 'd':
				res=res+13*pow(16, strlen(hex)-1-i);
				break;
			case 'D':
				res=res+13*pow(16, strlen(hex)-1-i);
				break;
			case 'e':
				res=res+14*pow(16, strlen(hex)-1-i);
				break;
			case 'E':
				res=res+14*pow(16, strlen(hex)-1-i);
				break;
			case 'f':
				res=res+15*pow(16, strlen(hex)-1-i);
				break;
			case 'F':
				res=res+15*pow(16, strlen(hex)-1-i);
				break;
			default:
				break;
		}
	}
	return res;*/
	long int number = (long int)strtol(hex, NULL, 0);
	return number;
}


main()
{
	char *nbHex = "0x80000000";
	int i = (int)hex_to_dec(nbHex);
	printf("%s en hexa vaut %d\n", nbHex, i);
}
