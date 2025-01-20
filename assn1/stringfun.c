#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define BUFFER_SZ 50

//prototypes
void usage(char *);
void print_buff(char *, int);
int  setup_buff(char *, char *, int);

//prototypes for functions to handle required functionality
int  count_words(char *, int, int);
void reverse_string(char *, int, int);
void word_print(char *, int, int);

int setup_buff(char *buff, char *user_str, int len){
	int i, j = 0;
	while (*(user_str+j) == ' ' || *(user_str+j) == '\t') {
		j++;
	}

	while (*(user_str+j) != '\0') {
		if (i >= len) { return -1; }

		if (*(user_str+j) == ' ' || *(user_str+j) == '\t') {
			buff[i] = ' ';
			while (*(user_str+j) == ' ' || *(user_str+j) == '\t') { j++; }
		} else {
			*(buff+i) = *(user_str+j);
			j++;
		}

		i++;
	}

	if (*(buff+i-1) == ' ') {
		i--;
	}

	for (int k = i; k < BUFFER_SZ; k++) { *(buff+k) = '.'; }
		
   return i; 
}

void print_buff(char *buff, int len){
    printf("Buffer:  [");
    for (int i = 0; i < len; i++) { putchar(*(buff+i)); }
	 putchar(']');
    putchar('\n');
}

void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);
}

int count_words(char *buff, int len, int str_len) {
	if (str_len > len) { exit(3); }

	int i = 0, cnt = 0;

	if (*buff == ' ') {
		i++;
	}

	while (i < str_len) {
		cnt++;
		while (*(buff+i) != ' ' && i < str_len) { i++; } 
		i++;
	}

   return cnt;
}

void reverse_string(char *buff, int len, int str_len) {
	if (str_len > len) { exit(3); }

	char* reverse_buff = malloc(len);

	for (int i = 0; i < str_len; i++) {
		*(reverse_buff+i) = *(buff+str_len-i-1);
	}

	for (int i = 0; i < str_len; i++) {
		*(buff+i) = *(reverse_buff+i);
	}
}

void word_print(char *buff, int len, int str_len) {
	if (str_len > len) { exit(3); }

	printf("Word Print\n");
	printf("----------\n");

	int i = 0, cnt = 0, letter_cnt = 0;
	if (*buff == ' ') {
		i++;
	}
	while (i < str_len) {
		cnt++;
		printf("%d. ", cnt);
		letter_cnt = 0;
		while (*(buff+i) != ' ' && i < str_len) { 
			printf("%c", *(buff+i));
			letter_cnt++;
			i++; 
		} 

		// The directions readme indicated there should be a space between the
		// word and the number of letters, but that failed test.sh, so I
		// defaultd to the code that passed the tests.
		printf("(%d)\n", letter_cnt);
		i++;
	}

	printf("\nNumber of words returned: %d\n", cnt);
}	

int main(int argc, char *argv[]){

    char *buff;             //placeholder for the internal buffer
    char *input_string;     //holds the string provided by the user on cmd line
    char opt;               //used to capture user option from cmd line
    int  rc;                //used for return codes
    int  user_str_len;      //length of user supplied string

    //   #1. WHY IS THIS SAFE, aka what if arv[1] does not exist?
    //       This is safe because if argv[1] does not exist, the boolean
	 //       expression would have already been short-circuited by argc < 2
	 //       being true, and so the second part will never be evaluated. 
    if ((argc < 2) || (*argv[1] != '-')){
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1]+1);   //get the option flag

    //handle the help flag and then exit normally
    if (opt == 'h'){
        usage(argv[0]);
        exit(0);
    }

    //WE NOW WILL HANDLE THE REQUIRED OPERATIONS

    //   #2. Document the purpose of the if statement below
    //       argc would be less than 3 if less than 2 arguments were given.
	 //       We need at least two arguments to run the program, so if less
	 //       were given, we can assume the user didn't know what they were
	 //       doing, print help, and exit. 
    if (argc < 3){
        usage(argv[0]);
        exit(1);
    }

    input_string = argv[2]; //capture the user input string

	 buff = malloc(BUFFER_SZ);
	 if (buff == NULL) { exit(99); }

    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);     //see todos
    if (user_str_len < 0){
        printf("Error setting up buffer, error = %d", user_str_len);
        exit(2);
    }

    switch (opt){
        case 'c':
            rc = count_words(buff, BUFFER_SZ, user_str_len);  
            if (rc < 0){
                printf("Error counting words, rc = %d", rc);
                exit(2);
            }
            printf("Word Count: %d\n", rc);
    			print_buff(buff,BUFFER_SZ);
            break;
			case 'r':
				reverse_string(buff, BUFFER_SZ, user_str_len);
    			print_buff(buff,BUFFER_SZ);
				break;
			case 'w':
				word_print(buff, BUFFER_SZ, user_str_len);
    			print_buff(buff,BUFFER_SZ);
				break;
			case 'x':
				printf("Not Implemented!\n");
    			if (argc < 5){
        			usage(argv[0]);
        			exit(1);
				}
				break;
        default:
            usage(argv[0]);
            exit(1);
    }

	 free(buff);
    exit(0);
}

//     #7.  Notice all of the helper functions provided in the 
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that 
//          the buff variable will have exactly 50 bytes?
//  
//          I think this is a good practice, since this allows us more
//          flexibility if we were to implement variable buffer sizes or
//          different use cases for each of these functions. Obviously this
//          program is quite one-note, but a more complex application may 
//          need more information like this.
