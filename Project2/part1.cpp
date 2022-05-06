#include <stdio.h>
#include <iostream>
#include <chrono>

using namespace std;

void merge(int* array, int left, int middle, int right) {
    //This is +1 because it's the size of the array, example left 0 mid 7 means size should be 8 not 7
    int leftArraySize = middle + 1 - left;
    //this is not +1 because 15 - 7 for example is 8 as it should be, so the size is already correct and doesn't need adjustment
    int rightArraySize = right - middle;

    //allocating memory for our new arrays; don't forget to delete when done
    int* leftArray = new int[leftArraySize];
    int* rightArray = new int[rightArraySize];

    //copy the values into our new arrays
    for (int i = 0; i < leftArraySize; i++) {
        leftArray[i] = array[left + i];
    }

    for (int i = 0; i < rightArraySize; i++) {
        rightArray[i] = array[middle + 1 + i];
    }

    //indexes need to start at 0 here because we want to run through our whole new arrays
    int indexLeft = 0;
    int indexRight = 0;

    //index for our merged array starts from left though
    int indexMergedArray = left;

    //while our indexes haven't run through either array all the way, do this.
    while (indexLeft < leftArraySize && indexRight < rightArraySize) {
        //if the left one's smaller, add it to our array at the specified position, then increment indexLeft
        if (leftArray[indexLeft] < rightArray[indexRight]) {
            array[indexMergedArray] = leftArray[indexLeft];
            indexLeft++;
        }
        //if the right one is smaller, add it to our array at the specified position, then increment indexRight
        else {
            array[indexMergedArray] = rightArray[indexRight];
            indexRight++;
        }
        //regardless of which one happens, increment this
        indexMergedArray++;
    }

    //Whichever one hasn't finished running through all the way, finish copying it
 
    while (indexLeft < leftArraySize) {
        array[indexMergedArray] = leftArray[indexLeft];
        indexLeft++;
        indexMergedArray++;
    }

    while (indexRight < rightArraySize) {
        array[indexMergedArray] = rightArray[indexRight];
        indexRight++;
        indexMergedArray++;
    }
    


    //When done, deallocate memory to stop leaks
    delete[] leftArray;
    delete[] rightArray;

}

void mergeSort(int* array, int left, int right) {
    //split the array in the middle and then run both halves through mergesort again, then put them back together with merge
    if (right > left) {
        int middle = left + (right - left) / 2;
        mergeSort(array, left, middle);
        mergeSort(array, middle + 1, right);
        merge(array, left, middle, right);
    }
}

int findKSmallestFromSorted(int* array, int k) {
    return array[k];
}

//partition function for quicksort
int partition(int* array, int size, int low, int high) {
    int swapStorage = 0;
    int pivotValue = array[high];
    //cout << "end of array " << pivotValue << endl;
    //pivotPosition indicates where the pivot will belong in the end
    int pivotPosition = low;
    //go through every value in the array except the pivot
    for (int i = low; i < high; i++) {
        //if the value we're looking at is less than pivot value, do this
        if (array[i] < pivotValue) {
            //swap values, then increment pivotPosition
            swapStorage = array[pivotPosition];
            array[pivotPosition] = array[i];
            array[i] = swapStorage;
            pivotPosition++;
        }
    }

    array[high] = array[pivotPosition];
    array[pivotPosition] = pivotValue;

    return pivotPosition;
}

int findKSmallestUsingIterativePartition(int* array, int size, int k) {

    int low = 0;
    int high = size - 1;
    int pivotPosition = partition(array, size, low, high);
    //cout << "Pivot is at " << pivotPosition << endl;

    while (pivotPosition != k) {
        //if pivotposition is less than k, then we should try again with anything between pivotPosition and high
        if (pivotPosition < k) {
            low = pivotPosition;
        }
        //if pivotposition is greater than k, try again with anything between low and pivotPosition
        else {
            high = pivotPosition - 1;
        }
        pivotPosition = partition(array, size, low, high);
        //cout << "Pivot is at " << pivotPosition << endl;
    }
    return array[k];
}

int findKSmallestUsingRecursivePartition(int* array, int size, int k, int low, int high) {
    int pivotPosition = partition(array, size, low, high);
    //cout << "Pivot is at " << pivotPosition << endl;
    
    if (pivotPosition == k) {
        return array[k];
    }
    else {
        //if pivotPosition is less than k, try again with anything between pivotPosition and high
        if (pivotPosition < k) {
            low = pivotPosition;
        }
        //if it is greater, then try again with anything between low and pivotposition
        else {
            high = pivotPosition - 1;
        }
        findKSmallestUsingRecursivePartition(array, size, k, low, high);
    }
    


}

//we cheat by only sorting half the list and returning median
int modifiedSelectionSortForMedian(int* array, int size) {
    int minimum = 0;
    int storage = 0;
    for (int i = 0; i <= (size / 2); i++) {
        minimum = array[i];
        for (int j = i + 1; j < size; j++) {
            if (array[j] < minimum) {

                //swap minimum and whatevers in array j
                storage = minimum;
                minimum = array[j];
                array[j] = storage;

            }
        }
    }
    return array[size / 2];
}

int findKSmallestUsingMedians(int* array, int size, int k, int low, int high) {
    size = high - low + 1;
    //if size < 5, should just be able to sort and return k element
    if (size < 5) {
        mergeSort(array, 0, size - 1);
        return findKSmallestFromSorted(array, k);
    }

    //make a new array with size / 5
    int* arrayOfMedians = new int[(size / 5)];
    int sizeMediansArray = (size / 5);

    
    
    int* groupOfFive = new int[5];
    //groupsize stays 5
    int groupSize = 5;
    //this is so we know how many values in arrayOfMedians
    int counter = 0;
    
    for (int i = low; i < high; i++) {
        //use the modulo operator to know when to reset
        groupOfFive[(i - low)%5] = array[i];

        //if modulo i returns 4, calculate median of the group
        if ((i - low) % 5 == 4) {
            groupSize = 5;
            arrayOfMedians[counter] = modifiedSelectionSortForMedian(groupOfFive, groupSize);
            counter++;
        }
        

    }

    //after filling ArrayOfMedians, find the actual median and use it as the pivot for partitioning
    int pivotPosition = findKSmallestUsingIterativePartition(arrayOfMedians, counter, counter/2);
    
    //since my partition uses pivot is last, let's swap them
    int pivotStorage = array[pivotPosition];
    array[pivotPosition] = array[high];
    array[high] = pivotStorage;

    //use partition to find where the pivot ended up so we can cut off stuff
    pivotPosition = partition(array, size, low, high);

    if (pivotPosition == k) {
        return array[k];
    }
    else {
        if (pivotPosition < k) {
            low = pivotPosition;
        }
        else {
            high = pivotPosition - 1;
        }
        findKSmallestUsingMedians(array, size, k, low, high);
    }


    //deallocate memory after we don't need it
    

}


void printArray(int* array, int size) {
    for (int i = 0; i < size; i++) {
        cout << array[i] << " ";
    }
    cout << endl;
}


int main() {
    //initialize random (not using time, let's just have the seed always be the same for now)
    srand(0);

    int size = 0;

    //make array to sort, size dependent on user
    cout << "Enter a size for the array here, values will be random 0-99" << endl;
    cin >> size;

    int k = size / 4;

    //pointer masterList points to array of size the user gives it
    int* masterList = new int[size];

    //set masterList's values to random values from 0 to 99
    for (int i = 0; i < size; i++) {
        masterList[i] = rand() % 100;
    }

    //Make a new list for the sorting portion, and copy values in
    int* listToSort = new int[size];
    for (int i = 0; i < size; i++) {
        listToSort[i] = masterList[i];
    }

    //run and keep track of time
    auto algorithmOneStart = chrono::high_resolution_clock::now();
    mergeSort(listToSort, 0, size-1);
    cout << "Kth smallest element from algorithm 1 is " << findKSmallestFromSorted(listToSort, k) << endl;
    auto algorithmOneEnd = chrono::high_resolution_clock::now();
    cout << "Algorithm 1 took " << chrono::duration_cast<chrono::nanoseconds>(algorithmOneEnd - algorithmOneStart).count() << " nanoseconds" << endl;

    //Make a new list for the iterative partition algorithm and copy values
    int* iterativePartitionList = new int[size];
    for (int i = 0; i < size; i++) {
        iterativePartitionList[i] = masterList[i];
    }
    
    //run and keep track of time
    auto algorithmTwoStart = chrono::high_resolution_clock::now();
    cout << "Kth smallest element from algorithm 2 is " << findKSmallestUsingIterativePartition(iterativePartitionList, size, k) << endl;
    auto algorithmTwoEnd = chrono::high_resolution_clock::now();
    cout << "Algorithm 2 took " << chrono::duration_cast<chrono::nanoseconds>(algorithmTwoEnd - algorithmTwoStart).count() << " nanoseconds" << endl;

    //make new list for recursive partition algorithm
    int* recursivePartitionList = new int[size];
    for (int i = 0; i < size; i++) {
        recursivePartitionList[i] = masterList[i];
    }

    //run and keep track of time
    auto algorithmThreeStart = chrono::high_resolution_clock::now();
    cout << "Kth smallest element from algorithm 3 is " << findKSmallestUsingRecursivePartition(recursivePartitionList, size, k, 0, size - 1) << endl;
    auto algorithmThreeEnd = chrono::high_resolution_clock::now();
    cout << "Algorithm 3 took " << chrono::duration_cast<chrono::nanoseconds>(algorithmThreeEnd - algorithmThreeStart).count() << " nanoseconds" << endl;

    //run and keep track of time, and we are using masterlist because it's gonna be deleted anyway so whatever
    auto algorithmFourStart = chrono::high_resolution_clock::now();
    cout << "Kth smallest element from algorithm 4 is " << findKSmallestUsingMedians(masterList, size, k, 0, size - 1) << endl;
    auto algorithmFourEnd = chrono::high_resolution_clock::now();
    cout << "Algorithm 4 took " << chrono::duration_cast<chrono::nanoseconds>(algorithmFourEnd - algorithmFourStart).count() << " nanoseconds" << endl;

    //This is here so we can see the sorted list in the end, so we can try to verify our results
    cout << "sorted array:" << endl;
    printArray(listToSort, size);

    //let's look at the other ones just out of curiosity 
    cout << "results of iterative" << endl;
    printArray(iterativePartitionList, size);
    cout << "results of recursive" << endl;
    printArray(recursivePartitionList, size);
    cout << "results of MM method" << endl;
    printArray(masterList, size);

    //deallocate memory 
    delete[] masterList;
    delete[] listToSort;
    delete[] iterativePartitionList;
    delete[] recursivePartitionList;
    return 0;
}