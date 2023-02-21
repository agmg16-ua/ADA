//Alejandro Guillén Merino      DNI:48790456G
/*
ADA. 2022-23
Práctica 2:"Empirical analysis by means of program-steps account of two sorting algorithms: Middle-Quicksort and Heapsort."
*/
#include <unistd.h>
#include <iostream>
#include <math.h>
#include <algorithm>

using namespace std;

double steps = 0;

//--------------------------------------------------------------
// Middle Quick Sort
void middle_QuickSort(int * v, long left, long right){
    steps++; //incrementar
	if (left<right){
	    long i=left; 
        long j=right;
		int pivot=v[(i+j)/2];
		do {
            steps++; //incrementar
            while (v[i]<pivot) { i++; steps++; } //incrementar
            while (v[j]>pivot) { j--; steps++; } //incrementar
            if (i<=j){ 
                swap(v[i], v[j]);
                i++; j--;
            }
        } while (i<=j);
        if (left < j)  middle_QuickSort(v,left,j);
        if (i < right) middle_QuickSort(v,i,right);
    }
    steps++;
}

//--------------------------------------------------------------                
//HEAPSORT
// Procedure sink used by Heapsort algorith
// Sink an element (indexed by i) in a tree to keep the heap property. n is the size of the heap. 

void sink(int *v, size_t n, size_t i) {
    steps++;
    do {
        steps++;
        size_t largest = i;  // Initialize largest as root
        size_t l = 2*i + 1;  // left = 2*i + 1
        size_t r = 2*i + 2;  // right = 2*i + 2
 
    // Is left child larger than root?
        if (l < n && v[l] > v[largest])
            largest = l;
 
    // Is right child larger than largest so far
        if (r < n && v[r] > v[largest])
            largest = r;
 
    // If largest is still root then the process is done
        if (largest == i) break;
    
	// If not, swap new largest with current node i and repeat the process with childs. 
        swap(v[i], v[largest]);
        i=largest;  
     } while (true);
}
 
//--------------------------------------------------------------                
// Heapsort algorithm (ascending sorting)
void heapSort(int *v, size_t n) {
    steps ++;
    if( n < 2 ) // nothing to do
        return;

    // Build a MAX-HEAP with the input array
    for (size_t i = n / 2 - 1; true; i--) {
        steps++;
        sink(v, n, i);
        if (i==0) break; //as size_t is unsigned type
	}	

  	// A this point we have a HEAP on the input array, let's take advantage of this to sort the array:
    // One by swap the first element, which is the largest (max-heap), with the last element of the vector and rebuild heap by sinking the new placed element on the begin of vector.  
    for (size_t i=n-1; i>0; i--) {
        steps++;
		// Move current root to the end.
        swap(v[0], v[i]);
 		// Now largest element is at the end but do not know if the element just moved to the beginning is well placed, so a sink process is required.
		// Note that, at the same time, the HEAP size is reduced one element by one.
        sink(v, i, 0);
		// The procedure ends when HEAP has only one element, the lower of the input array
    }
}

int main(void){

    srand(0);

    cout << "# QUICKSORT VERSUS HEAPSORT" << endl;
    cout << "#Average processing Msteps (million of program steps)" << endl;
    cout << "#Number of samples (arrays of integer): 30" << endl;
    cout << endl;
    cout << "#\t             RANDOM ARARYS                    SORTED ARRAYS              REVERSE SORTED ARRAYS" << endl;
    cout << "#\t       -------------------------        -------------------------       ------------------------" << endl;
    cout << "# Size         QuickSort        HeapSort        QuickSort        HeapSort       QuickSort       HeapSort " << endl;
    cout << "#";
    for(int i=0; i<=103; i++) { cout << '='; };
    cout << endl;


    for (int exp = 15; exp <= 20; exp++){
        size_t size = size_t( pow(2,exp) );

        int* vRan = new int [size];
        int* vSort = new int [size];
        int* vRever = new int [size];

        if (!vRan){
            cerr << "Error, not enough memory!" << endl;
            exit (EXIT_FAILURE);  
        }

        if (!vSort){
            cerr << "Error, not enough memory!" << endl;
            exit (EXIT_FAILURE);  
        }

        if (!vRever){
            cerr << "Error, not enough memory!" << endl;
            exit (EXIT_FAILURE);  
        }

        cout << size << "\t\t" << std::flush;

        //RANDOM QUICKSORT
        for(int i=0; i<30; i++) {
            for (size_t j = 0; j < size; j++) {
                vRan[j] = rand(); 
            }
            middle_QuickSort(vRan, 0, size-1);
        }
        
        cout << steps/30000000 << "  \t" << std::flush;

        //RANDOM HEAPSORT
        steps = 0;
        for(int i=0; i<30; i++) {
            for (size_t j = 0; j < size; j++) {
                vRan[j] = rand(); 
            }
            heapSort(vRan, size);
        }

        cout << steps/30000000 << "  \t" << std::flush;
        
        //SORTED QUICKSORT
        for(size_t i=0; i<size; i++) {
            vSort[i] = i;
        }

        steps = 0; 
        for(int i=0; i<30; i++) {
            middle_QuickSort(vSort, 0, size-1);
        }

        cout << steps/30000000 << "  \t " << std::flush;

        //SORTED HEAPSORT
        steps = 0;
        for(int i=0; i<30; i++) {
            heapSort(vSort, size);
        }

        cout << steps/30000000 << "  \t" << std::flush;

        for(size_t i=0; i<size; i++) {
            vRever[i] = (size-1) - i;
        }

        //REVERSE QUICKSORT
        steps = 0;
        for(int i=0; i<30; i++) {
            middle_QuickSort(vRever, 0, size-1);
        }

        cout << steps/30000000 << "     \t" << std::flush;

        //REVERSE HEAPSORT
        steps = 0;
        for(int i=0; i<30; i++) {
            heapSort(vRever, size);
        }

        cout << steps/30000000 << endl;

        
		for (size_t i = 1; i < size; i++){
			if (vRan[i] < vRan[i-1]){ 
				cerr << "Panic, array not sorted! RAN " << i << " " << size << endl; 
				exit(EXIT_FAILURE);            
			}
        }
        delete[] vRan; 

		for (size_t i = 1; i < size; i++) {
			if (vSort[i] < vSort[i-1]){ 
				cerr << "Panic, array not sorted! SORT" << i << endl; 
				exit(EXIT_FAILURE);            
			}
        }
        delete[] vSort; 

		for (size_t i = 1; i < size; i++) {
			if (vRever[i] < vRever[i-1]){ 
				cerr << "Panic, array not sorted! REVER" << i << endl; 
				exit(EXIT_FAILURE);            
			}
        }
        delete[] vRever;   
    }
}

