#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>

#define	CODE_SEPERATOR  "::="
#define CODE_OUTPUT     '~'
#define CODE_INPUT      ":::"
#define CODE_COMMENT    "#"
#define CODE_EXECUTABLE "#!/"

char	* get_line	(FILE * infile);

struct rule
{
  char	lhs[64];
  char	rhs[64];
} rulebase[128];

int	ruleidx = 0,
	debug = 0;
char	*dataspace,
	*tempspace;

typedef enum {
  SYNTAX_ERROR,
  NO_INPUT_ERROR,
  ENDING_ERROR,
  FILE_EOF_ERROR,
  NO_ARGUMENTS_ERROR,
} ThueError;

char* say_error(ThueError e) {
  switch (e) {
  case NO_INPUT_ERROR:return "You need to pass an input file.";
  case SYNTAX_ERROR:return "Syntax Error. Perhaps a misplaced seperator?";
  case ENDING_ERROR:return "Ended file improperly. Make sure to add a seperator and operating string?";
  case FILE_EOF_ERROR:return "File does not exist";
  case NO_ARGUMENTS_ERROR:return "You have to pass an argument.";
  }
}

void exit_with_error(ThueError e) {
  fprintf(stderr, "%s\n", say_error(e));
  exit(1); //not sure if this is memory safe
}

void output_file() {
  FILE* fptr = fopen("a.out", "wb");
  if (fptr) {
    fputs(dataspace, fptr);
    fclose(fptr);
  } else {
    fprintf(stderr, "Error opening output file. Try running without -o?");
  }
} 

char * get_line (FILE * infile)
{
  static	char	 buffer[256];
	char	*s;

  memset (buffer, 0, sizeof (buffer));
  /* Get next line from file */
  s = fgets (buffer, sizeof (buffer), infile);
  if (s == NULL)
    return (NULL);
  buffer[strlen (buffer) - 1] = '\0';
  /* Return pointer to string */
  return (buffer);
}

int main (int argc, char *argv[])
{
   char	*line,
    *c,
    *tmp,
    *target[64],
    tempstr[64];
  int	 state,
    flagstate,
    i, j, k,
    order,
    temp,
    rnum[64];
  FILE	*infile = NULL;

  //memory management
  //forgive me
  srand(time(0));
  dataspace = malloc(16384);
  tempspace = malloc(16384);
  if (dataspace == NULL || tempspace == NULL) {
    fprintf (stderr, "Could not allocate sufficient working memory\n");
    exit(1);
  }
  target[0] = dataspace;
  memset (rulebase, 0, sizeof (rulebase));
  memset (dataspace, 0, sizeof (dataspace));
  //argument processing
  order = 0;
  int opt;
  while ((opt=getopt(argc,argv, "rldo")) != -1) {
    switch (opt) {
    case 'r': //right to left processing
      order = 2;
      break;
    case 'l': //left to right processing
      order = 1;
      break;
    case 'd': //debug
      debug = 1;
      break;
    case 'o': //output
      atexit(output_file);
      break;
    }
  }

  if (argc > 1) {
    infile = fopen (argv[argc-1], "r");
  } else {
    exit_with_error(NO_ARGUMENTS_ERROR);
  }
  if (infile == NULL) {
    fprintf(stderr, "Could not open file: %s\n", argv[argc-1]);
    return 1;
  }

  /* Get input file */
  state = 0;
  while (!feof (infile))
    {
      line = get_line (infile);
      if (state == 0)
        {
          if (line != NULL && !strlen (line))
            continue;
          c = strstr (line, CODE_SEPERATOR);
          char* executable_line = strstr(line, CODE_EXECUTABLE);
          char* comment_line    = strstr(line, CODE_COMMENT);
          if (executable_line != NULL || comment_line != NULL) {
            continue;
          } 
          if (c == NULL)
            fprintf (stderr, "Malformed production: \"%s\"!\n", line);
          else if (c == line)
            state = 1;
          else	{
            flagstate = 0;
            for (tmp=line;tmp!=c;tmp++)
              if (!isspace (*tmp))
                flagstate = 1;
            if (flagstate)
              {
                *c = '\000';
                c += strlen (CODE_SEPERATOR);
                strcpy (rulebase[ruleidx].lhs, line);
                strcpy (rulebase[ruleidx].rhs, c);
                ++ruleidx;
              }
            else	state = 1;
          }
        }
      else if (line != NULL)
        strcat (dataspace, line);
    }

  if (debug)
    printf ("Initial:  \"%s\"\n", dataspace);

  /* Apply rules */
  state = 1;
  while (state)
    {
      /* Get all valid LHSs */
      j = 1;
      k = 0;
      c = dataspace;
      for (i=0;i<ruleidx;i++)
        do
          {
            k = j;
            target[k] = strstr (c, rulebase[i].lhs);
            rnum[k] = i;
            if (target[k] != NULL)
              {
                ++j;
                c = target[k] + 1;
              }
            else
              c = dataspace;
          }
        while (target[k]);

      if (j == 1)
        {
          state = 0;
          continue;
        }

      /* Sort the LHS list - Just a bubble sort */
      for (i=1;i<j;i++)
        for (k=1;k<i;k++)
          if (target[i] < target[k])
            {
              c = target[i];		temp = rnum[i];
              target[i] = target[k];	rnum[i] = rnum[k];
              target[k] = c;		rnum[k] = temp;
            }

      /* Choose rule to apply */
      switch (order)
        {
        case 2:
          i = j - 1;
          break;
        case 1:
          i = 1;
          break;
        default:
          i = rand() % (j - 1) + 1;
          break;
        }
      line = target[i];
      temp = rnum[i];
      c = line + strlen (rulebase[temp].lhs);
      *line = '\000';

      /* Check for I/O */
      i = rulebase[temp].rhs[0] == CODE_OUTPUT;
      if (i)
        {
          puts (&rulebase[temp].rhs[1]);
          memset (tempstr, 0, sizeof (tempstr));
        }
      else
        {
          i = !strcmp (rulebase[temp].rhs, CODE_INPUT);
          if (i)
            {
              fgets (tempstr, sizeof (tempstr), stdin);
              tempstr[strlen (tempstr) - 1] = '\0';
            }
        }

      /* Apply the rule */
      sprintf (tempspace, "%s%s%s", dataspace, i?tempstr:rulebase[temp].rhs, c);
      strcpy (dataspace, tempspace);
      
      if (debug) {
        puts(dataspace);
      }
    }
    
  if (debug) {
    printf ("Final:  \"%s\"\n", dataspace);
  }
  fclose(infile);
  //why was this so horribly written?
  //it can't be that hard to be a decent c programmer.
  
  free(dataspace);
  free(tempspace);
  return (0);
}
