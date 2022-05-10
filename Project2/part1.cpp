#include <stdio.h>
#include <iostream>
#include <chrono>
#include <time.h>

using namespace std;

void printArray(int* array, int size) {
    for (int i = 0; i < size; i++) {
        cout << array[i] << " ";
    }
    cout << endl;
}

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

//partition function for quicksort
int partition(int* array, int size, int low, int high) {
    //cout << "high is " << high << endl;
    //cout << "low is " << low << endl;

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

    while (pivotPosition != k) {
        //if pivotposition is less than k, then we should try again with anything between pivotPosition and high
        if (pivotPosition < k) {
            low = pivotPosition + 1;
        }
        //if pivotposition is greater than k, try again with anything between low and pivotPosition
        else {
            high = pivotPosition - 1;
        }
        pivotPosition = partition(array, size, low, high);
        //cout << "k is" << k << endl;
        //cout << "Pivot is at " << pivotPosition << endl;
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
            low = pivotPosition + 1;
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

//make sure this returns the index, not the value!
int findMedianOfMedians(int* array, int size, int k) {
    //if the size of the array we're trying to get the median of is not 5, do this
    if (size < 5) {
        mergeSort(array, 0, size - 1);
        return k;
    }
    //If the size is not less than 5, do this
    else {
        //make a new array with size / 5
        int* arrayOfMedians = new int[(size / 5)];
        int sizeMediansArray = (size / 5);

        int* groupOfFive = new int[5];
        //groupsize stays 5
        int groupSize = 5;
        int tracker = 0;
        
        for (int i = 0; i < sizeMediansArray; i++) {
            //for loop to fill our group of 5
            for (int j = 0; j < 5; j++) {
                groupOfFive[j] = array[(5 * i) + j];
            }
            
            //printArray(groupOfFive, 5);
            //calculate median and put it into arrayOfMedians
            //mergeSort(groupOfFive, 0, 4);
            //arrayOfMedians[i] = groupOfFive[2];
            arrayOfMedians[i] = modifiedSelectionSortForMedian(groupOfFive, 5);
            

        }

        //after you have all the medians, find the median of that 
        
        //mergeSort(arrayOfMedians, 0, sizeMediansArray-1);
        
        //int pivotValue = arrayOfMedians[sizeMediansArray/2];

        int pivotValue = findKSmallestUsingRecursivePartition(arrayOfMedians, sizeMediansArray, sizeMediansArray / 2, 0, sizeMediansArray - 1);

        //int pivotValue = findMedianOfMedians(arrayOfMedians, sizeMediansArray, sizeMediansArray / 2);
        
        //don't need these anymore
        delete[] arrayOfMedians;
        delete[] groupOfFive;

        for (int i = 0; i < size; i++) {
            if (array[i] == pivotValue) {
                return i;
            }
        }


        


    }
    

}


int findKSmallestUsingMedians(int* array, int size, int k, int low, int high) {
    
    //int low = 0;
    //int high = size - 1;

    //cout << "k is " << k << endl;

    int pivotPosition = low + findMedianOfMedians(array + low, size, k - low);
    //cout << "This is pivot position " << pivotPosition << endl;
    //cout << "this is low " << low << endl;
    //cout << "this is high " << high << endl;

    //cout << endl;
    //cout << "before partition " << endl;
    //printArray(array + low, high - low + 1);

    //swap pivotPosition with high
    int storage = array[pivotPosition];
    array[pivotPosition] = array[high];
    array[high] = storage;
    

    pivotPosition = partition(array, size, low, high);
    //cout << "after partition " << endl;
    //printArray(array + low, high - low + 1);

    //cout << "Pivot is at " << pivotPosition << " and pivot value is " << array[pivotPosition] << endl;
    if (pivotPosition == k) {
        return array[k];
    }
    else {
        //if pivotposition is less than k, then we should try again with anything between pivotPosition and high
        if (pivotPosition < k) {
            low = pivotPosition + 1;
        }
        //if pivotposition is greater than k, try again with anything between low and pivotPosition
        else {
            high = pivotPosition - 1;
        }
 
        findKSmallestUsingMedians(array, high - low + 1, k, low, high);


        
    }
    
    
}


void testAlgorithms(int size, int iterationsWanted, int k) {

    //slightly cheating by default value to size - 1 if weird inputs
    if (k == 1) {
        k = 0;
    }
    else if (k == 2) {
        k = size / 4;
    }
    else if (k == 3) {
        k = size / 2;
    }
    else if (k == 4) {
        k = (size / 4) * 3;
    }
    else {
        k = size - 1;
    }

    //pointers point to arrays with the given size
    int* masterList = new int[size];
    int* listToSort = new int[size];
    int* iterativePartitionList = new int[size];
    int* recursivePartitionList = new int[size];

    //make variables for keeping track of the time
    std::chrono::nanoseconds algorithmOneTime = std::chrono::duration<long, std::nano>::zero();
    std::chrono::nanoseconds algorithmTwoTime = std::chrono::duration<long, std::nano>::zero();
    std::chrono::nanoseconds algorithmThreeTime = std::chrono::duration<long, std::nano>::zero();
    std::chrono::nanoseconds algorithmFourTime = std::chrono::duration<long, std::nano>::zero();



    for (int iterationsRun = 0; iterationsRun < iterationsWanted; iterationsRun++) {
        //set masterList's values to random values from 0 to 1 billion
        for (int i = 0; i < size; i++) {
            masterList[i] = ((rand() % 32767) * 32767) + rand() % 32767;
        }

        //Then copy masterList's values into all three other arrays

        for (int i = 0; i < size; i++) {
            listToSort[i] = masterList[i];
        }

        for (int i = 0; i < size; i++) {
            iterativePartitionList[i] = masterList[i];
        }

        for (int i = 0; i < size; i++) {
            recursivePartitionList[i] = masterList[i];
        }

        //run and keep track of time
        auto algorithmOneStart = chrono::high_resolution_clock::now();
        mergeSort(listToSort, 0, size - 1);
        int result = listToSort[k];
        auto algorithmOneEnd = chrono::high_resolution_clock::now();
        auto oneElapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(algorithmOneEnd - algorithmOneStart);
        algorithmOneTime = algorithmOneTime + oneElapsed;

        //run and keep track of time
        auto algorithmTwoStart = chrono::high_resolution_clock::now();
        findKSmallestUsingIterativePartition(iterativePartitionList, size, k);
        auto algorithmTwoEnd = chrono::high_resolution_clock::now();
        auto twoElapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(algorithmTwoEnd - algorithmTwoStart);
        algorithmTwoTime = algorithmTwoTime + twoElapsed;

        //run and keep track of time
        auto algorithmThreeStart = chrono::high_resolution_clock::now();
        findKSmallestUsingRecursivePartition(recursivePartitionList, size, k, 0, size - 1);
        auto algorithmThreeEnd = chrono::high_resolution_clock::now();
        auto threeElapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(algorithmThreeEnd - algorithmThreeStart);
        algorithmThreeTime = algorithmThreeTime + threeElapsed;

        //run and keep track of time, and we are using masterlist because it's gonna be deleted anyway so whatever
        auto algorithmFourStart = chrono::high_resolution_clock::now();
        findKSmallestUsingMedians(masterList, size, k, 0, size - 1);
        auto algorithmFourEnd = chrono::high_resolution_clock::now();
        auto fourElapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(algorithmFourEnd - algorithmFourStart);
        algorithmFourTime = algorithmFourTime + fourElapsed;
    }


    
    cout << "Algorithm 1 took " << algorithmOneTime.count() / iterationsWanted << " nanoseconds on average" << endl;
    cout << "Algorithm 2 took " << algorithmTwoTime.count() / iterationsWanted << " nanoseconds on average" << endl;
    cout << "Algorithm 3 took " << algorithmThreeTime.count() / iterationsWanted << " nanoseconds on average" << endl;
    cout << "Algorithm 4 took " << algorithmFourTime.count() / iterationsWanted << " nanoseconds on average" << endl;

    //deallocate memory 
    delete[] masterList;
    delete[] listToSort;
    delete[] iterativePartitionList;
    delete[] recursivePartitionList;

}


int main() {
    //initialize random (not using time, let's just have the seed always be the same for now)
    srand(time(NULL));

    //These are hardcoded purely for example purposes
    int size = 10;

    int k = size / 2;

    //pointer masterList points to array of size the user gives it
    int* masterList = new int[size];

    //set masterList's values to random values from 0 to about but not exactly 1 billion
    //32767 is the minimum for rand_max, so I am using it like this so it's consistent even if rand_max is larger
    for (int i = 0; i < size; i++) {
        masterList[i] = ((rand() % 32767) * 32767) + rand() % 32767;
    }

    //Make a new list for the sorting portion, and copy values in
    int* listToSort = new int[size];
    for (int i = 0; i < size; i++) {
        listToSort[i] = masterList[i];
    }
    
    //run and keep track of time
    auto algorithmOneStart = chrono::high_resolution_clock::now();
    mergeSort(listToSort, 0, size-1);
    cout << "Kth smallest element from algorithm 1 is " << listToSort[k] << endl;
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

    //deallocate memory 
    delete[] masterList;
    delete[] listToSort;
    delete[] iterativePartitionList;
    delete[] recursivePartitionList;

    
    cout << endl;
    //size, iterations, k
    testAlgorithms(10, 1000000, 2);

    cout << endl;
    //size, iterations, k
    testAlgorithms(50, 1000000, 2);

    cout << endl;
    //size, iterations, k
    testAlgorithms(100, 100000, 2);

    cout << endl;
    //size, iterations, k
    testAlgorithms(500, 100000, 2);

    cout << endl;
    //size, iterations, k
    testAlgorithms(1000, 10000, 2);

    cout << endl;
    //size, iterations, k
    testAlgorithms(10, 1000000, 3);

    cout << endl;
    //size, iterations, k
    testAlgorithms(50, 1000000, 3);

    cout << endl;
    //size, iterations, k
    testAlgorithms(100, 100000, 3);

    cout << endl;
    //size, iterations, k
    testAlgorithms(500, 100000, 3);

    cout << endl;
    //size, iterations, k
    testAlgorithms(1000, 10000, 3);

    cout << endl;
    //size, iterations, k
    testAlgorithms(10, 1000000, 4);

    cout << endl;
    //size, iterations, k
    testAlgorithms(50, 1000000, 4);

    cout << endl;
    //size, iterations, k
    testAlgorithms(100, 100000, 4);

    cout << endl;
    //size, iterations, k
    testAlgorithms(500, 100000, 4);

    cout << endl;
    //size, iterations, k
    testAlgorithms(1000, 10000, 4);

    cout << endl;
    //size, iterations, k
    testAlgorithms(10, 1000000, 5);

    cout << endl;
    //size, iterations, k
    testAlgorithms(50, 1000000, 5);

    cout << endl;
    //size, iterations, k
    testAlgorithms(100, 100000, 5);

    cout << endl;
    //size, iterations, k
    testAlgorithms(500, 100000, 5);

    cout << endl;
    //size, iterations, k
    testAlgorithms(1000, 10000, 5);


    return 0;
}