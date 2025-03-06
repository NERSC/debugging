#include <stdio.h> 
#include "array.h"

int main(int argc, char **argv)
{ 
    /************* command line args ***/ 
    { 
        char command_line_string[80];
        if (argc > 1) 
        { 
            strcpy(command_line_string, argv[1]); 
            printf("arg_2=%s\n", command_line_string); 
        } 
    }
    
    array();

    array();

    {

        char input[80];
        scanf( "%s", input );
        printf( "You entered: %s\n", input );
        scanf( "%s", input );
        printf( "Now you entered: %s\n", input );

    }

    return 0;
}
