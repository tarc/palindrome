#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define N 2000
#define lN 4

// minima [x][y] memoizes the minimum for the substring beginning at x and
// ending at y (both inclusive). Such requirement implies minima is upper
// triangular - if the first index x is considered to be the row index and y
// the column one:
unsigned int minima[N][N] = {{0}};

unsigned char is_pal ( const char* str, unsigned int begin, unsigned int end)
{
  for ( ; begin < end ; begin++, end--)
  {
    if ( str[begin] != str[end] )
    {
      return 0;
    }
  }

  return 1;
}

void pp_mat ( const char* str, unsigned int mat[][N], size_t size )
{
  unsigned int x, y;
  for ( x = 0  ;  x < size  ;  ++ x )
  {
    printf ( "%c " , str [x] );
  }
  puts ("");

  for ( x = 0  ;  x < size  ;  ++ x )
  {
    printf ( "--" );
  }
  puts ("");

  for ( x = 0  ;  x < size  ;  ++ x )
  {
    for ( y = 0  ;  y < size  ;  ++ y )
    {
      printf ( "%u " , mat [x] [y] );
    }
    puts("");
  }
}

unsigned int min_pal( const char* str, size_t size )
{
  unsigned int diagonal, x, y, tmp_min, i;

  for ( diagonal = 0;  diagonal < size;  diagonal++)
  {
    for ( x = 0  ;  x < (size - diagonal)  ;  ++ x )
    {
      y = diagonal + x; // from  ( y - x )  ==  diagonal
      if ( is_pal ( str, x, y ) )
      {
        minima [x][y] = 1;
      }
      else
      {
        minima [x][y] = y - x + 1; // maximum (one palindrome for each letter)

        for ( i = 0  ;  i < (y - x)  ;  ++ i )
        {
          tmp_min = minima [x] [x + i]  +  minima [x + i + 1] [y];
          if ( tmp_min < minima [x] [y] )
          {
            minima [x] [y] = tmp_min;
          }
        }
      }
    }
  }

#if 0
  pp_mat ( str, minima , size );
#endif
  return minima [0] [size - 1];
}

int main()
{
  int n, test_num = 1;
  char line[N + 2];
#if 0
  int overflow = 0, key;
#endif

  fgets( line, N + 2, stdin );

  // limits n to N
  while ((  n =  atoi(line) >= N  ?  N  :  atoi(line)  ))
  {
#if 0
    if ( line[strlen(line) - 1] != '\n' )
    {
      overflow = 1;
    }
#endif

    fgets( line, N + 2, stdin );

#if 0
    while ( overflow && ( key = getc ( stdin ) ) != EOF && key != '\n' );
    
    overflow = 0;
#endif

    printf( "Teste %d\n", test_num++ );
    printf( "%d\n", min_pal(line, n) );
    puts("");

    fgets( line, lN + 2, stdin );
  }

  return 0;
}
