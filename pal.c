#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define N 2000
#define log_N 4

#define DISCARDS_EXCEEDING_INPUT 0
#define PRINTS_MEMO_MATRIX 0
#define LINEAR_SPACE 1

#if LINEAR_SPACE
  #define LINES 1
#else
  #define LINES N
#endif

// minima [x][y] memoizes the minimum for the substring beginning at x and
// ending at y (both inclusive). Such requirement implies minima is upper
// triangular - if the first index x is considered to be the row index and y
// the column one:
unsigned int minima[LINES][N] = {{0}};

// type of possible minimization strategies
typedef unsigned int ( *  minimization_algorithm  ) \
  ( const char*, size_t, unsigned int [][N] ) ;

// runs minimization algorithm strategy for a given instance str of size
// str_size using memoization_matrix to hold results of already computed
// subproblems
unsigned int run
  ( minimization_algorithm strategy,
    const char * str,
    size_t str_size,
    unsigned int memoization_matrix [] [N]
  )
{
  unsigned int i, j;

  // memoization matrix clean up
#if !LINEAR_SPACE
  for ( i = 0 ; i < str_size ; ++ i )
#else
  i = 0;
#endif
  {
    for ( j = 0 ; j < str_size ; ++ j )
    {
      memoization_matrix [i] [j] = 0;
    }
  }

  return ( * strategy ) ( str, str_size, memoization_matrix );
}

// Check if str's sub-array beginning at begin and and at end is a palindrome
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

// Pretty prints memoization matrix
void pp_mat ( const char* str, size_t size, unsigned int mat[][N] )
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

#if !LINEAR_SPACE
  for ( x = 0  ;  x < size  ;  ++ x )
#else
  x = 0;
#endif
  {
    for ( y = 0  ;  y < size  ;  ++ y )
    {
      printf ( "%u " , mat [x] [y] );
    }
    puts("");
  }
}

// Bottom-up implementation
unsigned int botton_up_strategy( const char* str, size_t size, unsigned int memo_matrix[][N] )
{
  unsigned int diagonal, x, y, tmp_min, i;

  for ( diagonal = 0;  diagonal < size;  diagonal++)
  {
    for ( x = 0  ;  x < (size - diagonal)  ;  ++ x )
    {
      y = diagonal + x; // from  ( y - x )  ==  diagonal
      if ( is_pal ( str, x, y ) )
      {
        memo_matrix [x][y] = 1;
      }
      else
      {
        memo_matrix [x][y] = y - x + 1; // maximum (one palindrome for each letter)

        for ( i = 0  ;  i < (y - x)  ;  ++ i )
        {
          tmp_min = memo_matrix [x] [x + i]  +  memo_matrix [x + i + 1] [y];
          if ( tmp_min < memo_matrix [x] [y] )
          {
            memo_matrix [x] [y] = tmp_min;
          }
        }
      }
    }
  }

  return memo_matrix [0] [size - 1];
}

// Top-down implementation
unsigned int top_down_strategy (
    const char* str, size_t size,
    unsigned int memo_matrix[][N]
    )
{
  unsigned int suffix_start;

  if ( memo_matrix [0] [size - 1] != 0 )
  {
    return memo_matrix [0] [size - 1];
  }

  if ( is_pal ( str, 0, size - 1 ) )
  {
    memo_matrix [0] [size - 1] = 1;
    return 1;
  }

  memo_matrix [0] [size - 1] = size;
  for ( suffix_start = 1  ;  suffix_start < size  ;  ++ suffix_start )
  {
    if ( is_pal ( str, suffix_start, size - 1 ) )
    {
#if !LINEAR_SPACE
      memo_matrix [suffix_start] [size - 1] = 1;
#endif

      top_down_strategy ( str, suffix_start, memo_matrix );

      if ( memo_matrix [0] [size - 1] > memo_matrix [0] [suffix_start - 1] + 1 )
      {
        memo_matrix [0] [size - 1] = memo_matrix [0] [suffix_start - 1] + 1;
      }
    }
  }

  return memo_matrix [0] [size - 1];
}


int main()
{
  int n, test_num = 1;
  char line[N + 2];
#if DISCARDS_EXCEEDING_INPUT
  int overflow = 0, key;
#endif

  fgets( line, N + 2, stdin );

  // limits n to N
  while ((  n =  atoi(line) >= N  ?  N  :  atoi(line)  ))
  {
#if DISCARDS_EXCEEDING_INPUT
    if ( line[strlen(line) - 1] != '\n' )
    {
      overflow = 1;
    }
#endif

    fgets( line, N + 2, stdin );

#if DISCARDS_EXCEEDING_INPUT
    while ( overflow && ( key = getc ( stdin ) ) != EOF && key != '\n' );
    
    overflow = 0;
#endif

    printf( "Teste %d\n", test_num++ );
    printf( "%d\n", run ( & top_down_strategy, line, n, minima ) );

#if PRINTS_MEMO_MATRIX
  pp_mat ( line, n, minima );
#endif

    puts("");

    fgets( line, log_N + 2, stdin );
  }

  return 0;
}
