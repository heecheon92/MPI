/*
 *
 *  File Name: mat_sendrecv.c
 *  
 *  Developer: Heecheon Park
 *
 *  Date: July 15, 2019
 *
 */

# include <mpi.h>
# include <stdio.h>
# include <stdlib.h>

// Print a matrix
void print_mat ( double *mat, int row, int col ) ;
void merge(double * arr, int firstIndex, int midIndex, int lastIndex);
void mergeSort(double * arr, int firstIndex, int lastIndex);

int main ( int argc, char *argv[] )
{

  int my_id ;       // rank
  int num_procs ;   // size

  int master = 0 ;

  double *a ;       // matrix A
  double *a_row ;
  double *a_merged;

  int m = 4 ;       // A is a m*m matrix
  double ans ;
  
  int i, j, k ;
  int dest, tag ;
  MPI_Status status ;
  
  MPI_Init ( &argc, &argv ) ;

  MPI_Comm_rank ( MPI_COMM_WORLD, &my_id ) ;
  MPI_Comm_size ( MPI_COMM_WORLD, &num_procs ) ;

  //printf ( "I am %d of %d\n", my_id+1, num_procs ) ;

  if ( my_id == master ) {

    printf( "The number of row is     %d.\n",  m) ;
    printf( "The number of columns is %d.\n", m ) ;
  } else {

  }

  // Initialize a and a_row
  if ( my_id == master ) {

    a = ( double * ) malloc ( m * 2 * sizeof ( double ) ) ;
    a_merged = ( double * ) malloc (m * m * sizeof (double) );

    // Initializing A with random numbers between 0-100.
    for ( i = 0 ; i <= m*m-1 ; i++ )
      a[i] = rand() % 100 ;
    
    
    // Print A
    printf ( "________________________\n" ) ;
    
    print_mat ( a, m, m ) ;

    printf ( "________________________\n" ) ;

  } else { 

    // Initialize a_row in slave processors
    a_row = ( double * ) malloc ( m * 2 * sizeof ( double ) ) ;
  }

  // Send A to processors
  if ( my_id == master ) {

    MPI_Send ( a, m*2, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD ) ;
    MPI_Send ( a+8, m*2, MPI_DOUBLE, 2, 0, MPI_COMM_WORLD ) ;
    
    free ( a ) ;

  } else {


    MPI_Recv ( a_row, m*2, MPI_DOUBLE, master, MPI_ANY_TAG, MPI_COMM_WORLD, &status );

    printf ( "************************\n" ) ;
    printf ( "I am %d of %d\n", my_id+1, num_procs ) ;
    printf ( "************************\n" ) ;
    print_mat ( a_row, m, 2 ) ;
    printf ( "************************\n" ) ;
  }

  if (my_id != master)
  { 
        mergeSort(a_row, 0, m*2-1);
	if (my_id == 1)
    		MPI_Send(a_row, m*2, MPI_DOUBLE, master, 0, MPI_COMM_WORLD);
	else if (my_id == 2)
		MPI_Send(a_row, m*2, MPI_DOUBLE, master, 0, MPI_COMM_WORLD);
    	free ( a_row ) ;
  }

  if ( my_id == master)
  {
	printf ("-------------------------------------------------------------\n");
  	//MPI_Recv(a_merged, m * 2, MPI_DOUBLE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
	
	MPI_Recv(a_merged, m * 2, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, &status);
  	MPI_Recv(a_merged+8, m * 2, MPI_DOUBLE, 2, 0, MPI_COMM_WORLD, &status);
  	
	mergeSort(a_merged, 0, m*m-1);
	printf("This is the final result from processor %d.\n", my_id);
	print_mat ( a_merged, m, m ) ;
  }
  
  MPI_Finalize ( ) ;
}

void print_mat ( double *mat, int row, int col ) {

  int i, k ;

  k = 0 ;
  for ( i = 1 ; i <= row*col ; i++ ) {

    printf( "%5.1f", mat[k] ) ;
    if ( ( i % row ) == 0 || i == ( row * col ) ) printf( "\n" ) ;
    k++ ;
  }
}

/*
 * Merge sort algorithm.
 * MergeSort(arr[], l,  r)
     If r > l
     1. Find the middle point to divide the array into two halves:
             middle m = (l+r)/2
     2. Call mergeSort for first half:
             Call mergeSort(arr, l, m)
     3. Call mergeSort for second half:
             Call mergeSort(arr, m+1, r)
     4. Merge the two halves sorted in step 2 and 3:
             Call merge(arr, l, m, r)
 * */

void merge(double * arr, int firstIndex, int midIndex, int lastIndex)
{
	// static size of left array and right array
	size_t left_size = midIndex - firstIndex + 1;
	size_t right_size = lastIndex - midIndex;
	
	// Temporary arrays to hold values from arr parameter.
	double L[left_size], R[right_size];

	// Copy values over to temporary arrays.
	for (int i = 0; i < left_size; i++)
		L[i] = arr[firstIndex + i];
	for (int j = 0; j < right_size; j++)
		R[j] = arr[midIndex + 1 + j];

	int left_firstIndex, right_firstIndex, merged_firstIndex;
	left_firstIndex = 0;
	right_firstIndex = 0;
	merged_firstIndex = firstIndex;

	while (left_firstIndex < left_size && right_firstIndex < right_size)
	{
		if (L[left_firstIndex] <= R[right_firstIndex])
		{
			arr[merged_firstIndex] = L[left_firstIndex];
			left_firstIndex++;
		}	
		else
		{
			arr[merged_firstIndex] = R[right_firstIndex];
			right_firstIndex++;
		}
		merged_firstIndex++;
	}

	while (left_firstIndex < left_size)
	{
		arr[merged_firstIndex] = L[left_firstIndex];
		left_firstIndex++;
		merged_firstIndex++;
	}

	while (right_firstIndex < right_size)
	{
		arr[merged_firstIndex] = R[right_firstIndex];
		right_firstIndex++;
		merged_firstIndex++;
	}

}
void mergeSort(double * arr, int firstIndex, int lastIndex)
{
	if (firstIndex < lastIndex)
	{
		// Same as (first+last)/2 but prevents overflow
		int midIndex = firstIndex + (lastIndex - firstIndex) / 2;

		mergeSort(arr, firstIndex, midIndex);
		mergeSort(arr, midIndex+1, lastIndex);

		merge(arr, firstIndex, midIndex, lastIndex);	
	}
}
