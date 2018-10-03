#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CSVLib.h>

int main (int argc, char *argv[])
{
    char **parsed=NULL;
	FILE *in,*out;
    int numberOfFields;
	char *line=NULL;
    int i;
	size_t len = 0;
    ssize_t read;
	char *buffer=NULL;

	/* Open the files */
    in = fopen (argv[1], "r");
	out = fopen (argv[2], "wb");

    if (in == NULL)
        exit(EXIT_FAILURE);

	if (out == NULL)
		exit(EXIT_FAILURE);

	/* Read each line */
	while ((read = getline (&line, &len, in)) != -1)
   	{
		/* Parse it! */
   		if (!(parsed = CSVParse (line,&numberOfFields)))
   		{
       		printf ("String parsing failed!\n");
       		return 1;
   		}

		/* Make Space */	
		buffer = malloc (numberOfFields+1);

		/* Fill it! */
   		for (i=0;i<numberOfFields;i++)
   		{
       		buffer[i] = (char) atoi (parsed[i]);
   		}

		/* Write it! */
		fwrite (buffer,numberOfFields,1,out);

		/* Clean up after ourselves */
		free (buffer);
		buffer = NULL;

   	   	cleanupStrings (parsed,numberOfFields);
	}

	/* Clean up and close out */
	free (line);
	line=NULL;

	fclose (in);
	fclose (out);


    /* And we are out of here! */
    return 0;
}
