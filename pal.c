#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define N 2000
#define lN 4

// minima [x][y] memoizes the minimum for substring beginning at y and ending
// at x (both inclusive):
unsigned int minima[N][N] = {{0}};

unsigned char is_pal ( const char* str, unsigned int end, unsigned int begin)
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

  for ( y = 0  ;  y < size  ;  ++ y )
  {
    for ( x = 0  ;  x < size  ;  ++ x )
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
    for ( y = 0  ;  y < (size - diagonal)  ;  ++ y )
    {
      x = diagonal + y;
      if ( is_pal ( str, x, y ) )
      {
        minima [x][y] = 1;
      }
      else
      {
        minima [x][y] = x - y + 1; //maximum

        for ( i = 0  ;  i < (x - y)  ;  ++ i )
        {
          tmp_min = minima [y + i] [y]  +  minima [x] [y + i + 1];
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
  return minima [size - 1][0];
}

int main()
{
  int n, test_num = 1, overflow = 0, key;
  char line[N + 2];

  fgets( line, N + 2, stdin );

  // limits n to N
  while ((  n =  atoi(line) >= N  ?  N  :  atoi(line)  ))
  {
    if ( line[strlen(line) - 1] != '\n' )
    {
      overflow = 1;
    }

    fgets( line, N + 2, stdin );

    while ( overflow && ( key = getc ( stdin ) ) != EOF && key != '\n' );
    
    overflow = 0;

    printf( "Teste %d\n", test_num++ );
    printf( "%d\n", min_pal(line, n) );
    puts("");

    fgets( line, lN + 2, stdin );
  }

  return 0;
}
