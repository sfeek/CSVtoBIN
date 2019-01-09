#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*  Simple and fast CSV parser that is memory efficient and handles escaped characters and quotes correctly.
    Written by Shane Feek 04/20/2016  Email: shane.feek@gmail.com
*/
extern char **CSVParse (char *str, int *numberOfFields)
{
	char *newStr = NULL;
	char currentCharacter;
	char **strArray = NULL;
	int quote = 0;
	int csvLength = strlen (str);
	int maxFieldCount = 2; /* Start with two fields as MAX */
	int *commaPositions = NULL;
	int currentField = 0;
	int cleanStringPosition = 0;
	int i;
	int startPosition = 0;
	int fieldLength;

	/* Allocate memory for the comma position array */
	if (! (commaPositions = calloc (1, sizeof (int) * maxFieldCount)))
	{
		printf ("Error Allocating Memory!\n");
		return NULL;
	}

	/* Allocate memory for "cleaned up" string the same size as the original string to guarantee that it is big enough */
	if (! (newStr = calloc (1, sizeof (char) * csvLength)))
	{
		printf ("Error Allocating Memory!\n");
		return NULL;
	}

	/* First pass through to record the correct comma positions */
	for (i = 0; i < csvLength; i++)
	{
		/* Get a single character and skip any control or garbage characters */
		if ((currentCharacter = str[i]) < 32)
			continue;

		/* Handle quotes, escapes and commas */
		switch (currentCharacter)
		{
			/* Check for escape character not inside quotes */
			case 92:
				{
					if (quote == 0)
					{
						/* Move ahead one character */
						i++;
						/* Keep the next good character and move to the next good character position*/
						newStr[cleanStringPosition++] = str[i];
						/* Move on to the next new character */
						continue;
					}

					break;
				}

			/* Check for quote and keep track of pairs */
			case 34:
				{
					/* Toggle the pair indicator */
					quote = 1 - quote;
					/* Skip the quote */
					continue;
				}

			/* Check for comma that is NOT inside quotes */
			case 44:
				{
					if (quote == 0)
					{
						/* Check to see if we need to grow our comma position array */
						if (currentField == maxFieldCount - 1)
						{
							/* Double in size each time */
							maxFieldCount *= 2;

							/* Allocate more memory for the array*/
							if (! (commaPositions = realloc (commaPositions, sizeof (int) * maxFieldCount)))
							{
								printf ("Error Expanding Array!\n");
								return NULL;
							}
						}

						/* Keep track of the comma positions and move to the next field*/
						commaPositions[currentField++] = cleanStringPosition;
					}
				}
		}

		/* Keep the good characters and move to the next good character position  */
		newStr[cleanStringPosition++] = currentCharacter;
	}

	/* Make sure that clean string gets NULL terminator */
	newStr[cleanStringPosition] = 0;
	/* Make sure to mark the end of the string as a "comma" position so that the last field gets included in the array and include the last field */
	commaPositions[currentField++] = cleanStringPosition;
	/* Record the Total number of fields to return to the calling function */
	*numberOfFields = currentField;
	/* Allocate an array of pointers to chars, not actually allocating any strings themselves */
	strArray = malloc (sizeof (char *) * currentField );

	/* Copy the strings to the new string array */
	for (i = 0; i < currentField; i++)
	{
		/* Calculate length of the current field plus the Null Terminator*/
		fieldLength = commaPositions[i] - startPosition + 1;
		/* Replace the comma with a Null terminator */
		newStr[commaPositions[i]] = 0;
		/* Allocate memory for the current field */
		strArray[i] = calloc (1, sizeof (char) * fieldLength);
		/* Copy the string into the new array */
		memcpy (strArray[i], newStr + startPosition, fieldLength);
		/* Move our start position to the next field */
		startPosition = commaPositions[i] + 1;
	}

	/* Clean up the dynamic arrays */
	free (commaPositions);
	commaPositions = NULL;
	free (newStr);
	newStr = NULL;
	/* Return the new array back to the calling function */
	return strArray;
}

/*  Clean up an array of strings
    Input:  Array of strings
            Number of strings
    Return: none
*/
extern void cleanupStrings (char **strArray, int numberOfStrings)
{
	int i;

	/* Free the individual strings */
	for (i = 0; i < numberOfStrings; i++)
	{
		free (strArray[i]);
		strArray[i] = NULL;
	}

	/* Once the strings themselves are freed, free the actual array itself */
	free (strArray);
	strArray = NULL;
}


