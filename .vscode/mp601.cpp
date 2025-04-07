#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

// Function to print the initial array
void printInitialArray(const vector<int>& arr) {
    cout << "Initial array: ";
    for (int num : arr) {
        cout << num << " ";
    }
    cout << endl;
}

// Function to print the result of the search
void printResult(int index, int key) {
    if (index != -1) {
        cout << "Element " << key << " found at index: " << index << endl;
    } else {
        cout << "Element " << key << " not found in the array." << endl;
    }
}

// Function to perform Bubble Sort
void bubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                // Swap arr[j] and arr[j + 1]
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

// Function to check if the array is sorted
bool isSorted(const vector<int>& arr) {
    for (size_t i = 1; i < arr.size(); i++) {
        if (arr[i] < arr[i - 1]) {
            return false; // Array is not sorted
        }
    }
    return true; // Array is sorted
}

// Linear Search
int linearSearch(const vector<int>& arr, int target, int& comparisons) {
    comparisons = 0;
    for (size_t i = 0; i < arr.size(); i++) {
        comparisons++;
        if (arr[i] == target) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

// Binary Search
int binarySearch(const vector<int>& arr, int target, int& comparisons) {
    int left = 0;
    int right = arr.size() - 1;
    comparisons = 0;

    while (left <= right) {
        int mid = left + (right - left) / 2;
        comparisons++;
        if (arr[mid] == target) {
            return mid;
        } else if (arr[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return -1;
}

// Jump Search
int jumpSearch(const vector<int>& arr, int target, int& comparisons) {
    int n = arr.size();
    int step = static_cast<int>(sqrt(n));
    int prev = 0;
    comparisons = 0;

    while (prev < n && arr[min(step, n) - 1] < target) {
        comparisons++;
        prev = step;
        step += static_cast<int>(sqrt(n));
        if (prev >= n) {
            return -1;
        }
    }

    for (int i = prev; i < min(step, n); i++) {
        comparisons++;
        if (arr[i] == target) {
            return i;
        }
    }
    return -1;
}

// Interpolation Search
int interpolationSearch(const vector<int>& arr, int target, int& comparisons) {
    int low = 0, high = arr.size() - 1;
    comparisons = 0;

    while (low <= high && target >= arr[low] && target <= arr[high]) {
        comparisons++;
        if (low == high) {
            if (arr[low] == target) return low;
            return -1;
        }

        int pos = low + ((double)(high - low) / (arr[high] - arr[low]) * (target - arr[low]));
        comparisons++;
        if (arr[pos] == target) {
            return pos;
        } else if (arr[pos] < target) {
            low = pos + 1;
        } else {
            high = pos - 1;
        }
    }
    return -1;
}

int main() {
    int N, target;

    // Loop until a sorted array is provided
    while (true) {
        // Input the number of elements
        cout << "Enter the number of elements: ";
        cin >> N;

        vector<int> arr(N);
        cout << "Enter " << N << " elements (unsorted):\n";
        for (int i = 0; i < N; i++) {
            cin >> arr[i];  // Take array elements as input
        }

        // Check if the array is sorted
        if (isSorted(arr)) {
            cout << "The array is sorted." << endl;
            break; // Exit the loop if the array is sorted
        } else {
            cout << "The array is not sorted. Please enter the elements again." << endl;
        }
    }

    // Print the initial array
    printInitialArray(arr);

    // Input the target value to search
    cout << "Enter number to search: ";
    cin >> target;

    // Perform searches and display results
    int comparisons;

    int linIndex = linearSearch(arr, target, comparisons);
    printResult(linIndex, target);
    cout << "Linear Search Comparisons: " << comparisons << endl;

    int binIndex = binarySearch(arr, target, comparisons);
    printResult(binIndex, target);
    cout << "Binary Search Comparisons: " << comparisons << endl;

    int jumpIndex = jumpSearch(arr, target, comparisons);
    printResult(jumpIndex, target);
    cout << "Jump Search Comparisons: " << comparisons << endl;

    int interIndex = interpolationSearch(arr, target, comparisons);
    printResult(interIndex, target);
    cout << "Interpolation Search Comparisons: " << comparisons << endl;

    return 0;
}