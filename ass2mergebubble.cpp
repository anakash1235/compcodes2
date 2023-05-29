#include <iostream>
#include <cstdlib>
#include <ctime>
#include <omp.h>
using namespace std;
// Parallel Bubble Sort function
void parallelBubbleSort(int *array, int n) {
    int i, j;
    for (i = 0; i < n; i++) {
        int f=i%2;
        #pragma omp parallel for shared(array,f)
        for (j = f; j < n-1; j+=2) {
            if (array[j] > array[j+1]) {
                // Swap elements
                int temp = array[j];
                array[j] = array[j+1];
                array[j+1] = temp;
            }
        }
    }
}
// Parallel Merge Sort function
void merge(int *array, int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
    // Create temp arrays
    int *L = new int[n1];
    int *R = new int[n2];
    // Copy data to temp arrays L[] and R[]
    for (i = 0; i < n1; i++)
        L[i] = array[l + i];
    for (j = 0; j < n2; j++)
        R[j] = array[m + 1 + j];
    // Merge the temp arrays back into array[l..r]
    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            array[k] = L[i];
            i++;
        } else {
            array[k] = R[j];
            j++;
        }
        k++;
    }
    // Copy the remaining elements of L[], if there are any
    while (i < n1) {
        array[k] = L[i];
        i++;
        k++;
    }
    // Copy the remaining elements of R[], if there are any
    while (j < n2) {
        array[k] = R[j];
        j++;
        k++;
    }
    delete [] L;
    delete [] R;
}
void parallelMergeSort(int *array, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        #pragma omp parallel sections
        {
            #pragma omp section
            parallelMergeSort(array, l, m);
            #pragma omp section
            parallelMergeSort(array, m + 1, r);
        }
        merge(array, l, m, r);
    }
}
int main() {
    int n;
    cout << "Enter the size of the array: ";
    cin >> n;
    int *array = new int[n];
    srand(time(0));
    for (int i = 0; i < n; i++) {
        array[i] = rand() % 100;
    }
    cout << "Original Array: ";
    for (int i = 0; i < n; i++) {
        cout << array[i] << " ";
    }
    cout << endl;
    int choice;
    cout << "Enter 1 for Parallel Bubble Sort or 2 for Parallel Merge Sort: ";
    cin >> choice;
    if (choice == 1) {
        parallelBubbleSort(array, n);
    } else if (choice == 2) {
        parallelMergeSort(array, 0, n - 1);
    } else {
        cout << "Invalid choice. Exiting program." << endl;
        return 0;
    }
    cout << "Sorted Array: ";
    for (int i = 0; i < n; i++) {
        cout << array[i] << " ";
    }
    cout << endl;
    delete [] array;
    return 0;
}