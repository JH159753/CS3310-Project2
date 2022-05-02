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

void mergeSort(int *array, int left, int right) {
    //split the array in the middle and then run both halves through mergesort again, then put them back together with merge
    if (right > left) {
        int middle = left + (right - left) / 2;
        mergeSort(array, left, middle);
        mergeSort(array, middle + 1, right);
        merge(array, left, middle, right);
    }
}

int findKSmallest(int* array, int k) {
    return array[k];
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
    int k = 0;

    //make array to sort, size dependent on user
    cout << "Enter a size for the array here, values will be random 0-99" << endl;
    cin >> size;

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

    auto algorithmOneStart = chrono::high_resolution_clock::now();
    mergeSort(listToSort, 0, size-1);
    cout << findKSmallest(listToSort, k) << endl;
    auto algorithmOneEnd = chrono::high_resolution_clock::now();
    
    cout << "Algorithm 1 took " << chrono::duration_cast<chrono::nanoseconds>(algorithmOneEnd - algorithmOneStart).count() << " nanoseconds" << endl;
    
        

    //deallocate memory 
    delete[] masterList;
    delete[] listToSort;
    return 0;
}