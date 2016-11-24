//
//  heapsort.c
//  JBasic
//
//  Created by Juan López Rubio on 8/5/16.
//  Copyright © 2016 Juan López Rubio. All rights reserved.
//

#include "heapsort.h"

void testHeapSort()
{
    int arr[10] = { 11, 2, 9, 13, 57, 25, 17, 1, 90, 3 } ;
    int i ;
    
    printf( "Heap Sort.\n" ) ;
    
    printf ( "\nBefore Heap:\n" ) ;
    for ( i = 0 ; i <= 9 ; i++ )
        printf ( "%d\t", arr[i] ) ;
    
    makeheap_( arr, 10 ) ;
    
    printf ( "\nBefore Sorting:\n" ) ;
    for ( i = 0 ; i <= 9 ; i++ )
        printf ( "%d\t", arr[i] ) ;
    
    heapsort_ ( arr, 10 ) ;
    
    printf ( "\nAfter Sorting:\n" ) ;
    for ( i = 0 ; i <= 9 ; i++ )
        printf ( "%d\t", arr[i] ) ;
    
    printf("\r\n");
}

void makeheap_ ( int x[ ], int n )
{
    int i, val, s, f ;
    for ( i = 1 ; i < n ; i++ )
    {
        val = x[i] ;
        s = i ;
        f = ( s - 1 ) / 2 ;
        while ( s > 0 && x[f] < val )
        {
            x[s] = x[f] ;
            s = f ;
            f = ( s - 1 ) / 2 ;
        }
        x[s] = val ;
    }
}

void heapsort_ ( int x[ ], int n )
{
    int i, s, f, ivalue ;
    for ( i = n - 1 ; i > 0 ; i-- )
    {
        ivalue = x[i] ;
        x[i] = x[0] ;
        f = 0 ;
        
        if ( i == 1 )
            s = -1 ;
        else
            s = 1 ;
        
        if ( i > 2 && x[2] > x[1] )
            s = 2 ;
        
        while ( s >= 0 && ivalue < x[s] )
        {
            x[f] = x[s] ;
            f = s ;
            s = 2 * f + 1 ;
            
            if ( s + 1 <= i - 1 && x[s] < x[s + 1] )
                s++ ;
            if ( s > i - 1 )
                s = -1 ;
        }
        x[f] = ivalue ;
    }
}
