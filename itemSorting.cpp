#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <vector>
#include <numeric>

using namespace std;
using namespace std::chrono;

template <typename T>
class SortTester
{
private:
    T *arr;
    int size;

public:
    SortTester() : arr(nullptr), size(0) {}

    void generateRandomArray(int sz)
    {
        size = sz;
        arr = new T[size];
        srand(time(0));
        for (int i = 0; i < size; ++i)
        {
            arr[i] = static_cast<T>(-5000 + rand() % 10000);
        }
    }

    void printArray()
    {
        for (int i = 0; i < size; ++i)
        {
            cout << "| " << arr[i] << " ";
        }
        cout << endl
             << "---------------------------------------------------------------------------------------------------------------------------------------------------------------"
             << endl;
    }

    void readArrayFromFile(const string &filename)
    {
        ifstream file(filename);
        if (!file)
        {
            cerr << "Unable to open file " << filename << endl;
            exit(1);
        }

        size = 0;
        T temp;
        while (file >> temp)
        {
            size++;
        }

        file.clear();
        file.seekg(0, ios::beg);

        arr = new T[size];

        for (int i = 0; i < size; i++)
        {
            file >> arr[i];
        }
        file.close();
    }

    bool isSorted()
    {
        for (int i = 0; i < size - 1; ++i)
        {
            if (arr[i] > arr[i + 1])
                return false;
        }
        return true;
    }

    void insertionSort()
    {
        for (int i = 1; i < size; ++i)
        {
            T key = arr[i];
            int j = i - 1;
            while (j >= 0 && arr[j] > key)
            {
                arr[j + 1] = arr[j];
                j--;
            }
            arr[j + 1] = key;

            // cout << "Step " << i << ": ";
            // printArray();
        }
    }

    void heapify(int n, int i)
    {
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < n && arr[left] > arr[largest])
            largest = left;

        if (right < n && arr[right] > arr[largest])
            largest = right;

        if (largest != i)
        {
            swap(arr[i], arr[largest]);
            heapify(n, largest);
        }
    }

    void heapSort()
    {
        for (int i = size / 2 - 1; i >= 0; --i)
            heapify(size, i);

        for (int i = size - 1; i > 0; --i)
        {
            swap(arr[0], arr[i]);
            heapify(i, 0);

            // cout << "Step " << size - i << ": ";
            // printArray();
        }
    }

    void shellSort(int gap)
    {
        int iteration = 0;
        for (int g = size / 2; g > 0; g /= 2)
        {
            for (int i = g; i < size; ++i)
            {
                T temp = arr[i];
                int j;
                for (j = i; j >= g && arr[j - g] > temp; j -= g)
                    arr[j] = arr[j - g];
                arr[j] = temp;
                // cout << "Iteration: " << iteration << ": ";
                // printArray();
            }
            iteration++;
        }
    }

    void quickSort(int left, int right)
    {
        if (left < right)
        {
            int pivotIndex = rand() % (right - left + 1) + left;
            swap(arr[pivotIndex], arr[right]);

            pivotIndex = partition(left, right);
            quickSort(left, pivotIndex - 1);
            quickSort(pivotIndex + 1, right);
        }
    }

    int partition(int left, int right)
    {
        T pivot = arr[right];
        int i = left - 1;
        for (int j = left; j <= right - 1; j++)
        {
            if (arr[j] < pivot)
            {
                i++;
                swap(arr[i], arr[j]);
            }
        }
        swap(arr[i + 1], arr[right]);

        // cout << "Step: " << right - left + 1 << ": ";
        // printArray();

        return i + 1;
    }

    long long measureTime(void (SortTester::*sortMethod)(), int repetitions)
    {
        auto start = high_resolution_clock::now();
        for (int i = 0; i < repetitions; i++)
        {
            (this->*sortMethod)();
        }
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop - start);
        return duration.count();
    }

    long long measureTime(void (SortTester::*sortMethod)(int, int), int left, int right, int repetitions)
    {
        auto start = high_resolution_clock::now();
        for (int i = 0; i < repetitions; i++)
        {
            (this->*sortMethod)(left, right);
        }
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop - start);
        return duration.count();
    }

    long long measureTime(void (SortTester::*sortMethod)(int), int gap, int repetitions)
    {
        auto start = high_resolution_clock::now();
        for (int i = 0; i < repetitions; i++)
        {
            (this->*sortMethod)(gap);
        }
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop - start);
        return duration.count();
    }

    int getSize() const { return size; }

    void runTests(int choice, int repetitions)
    {
        vector<int> arraySizes = {20000, 40000, 50000, 80000, 100000, 200000, 300000};
        vector<long long> averageTimes;

        for (int size : arraySizes)
        {
            cout << "Sorting array of size: " << size << endl;
            generateRandomArray(size);
            long long totalTime = 0;
            switch (choice)
            {
            case 1:
                totalTime = measureTime(&SortTester::insertionSort, repetitions);
                break;
            case 2:
                totalTime = measureTime(&SortTester::heapSort, repetitions);
                break;
            case 3:
                totalTime = measureTime(&SortTester::shellSort, sizeof(arr) / sizeof(arr[0]), repetitions);
                break;
            case 4:
                totalTime = measureTime(&SortTester::quickSort, 0, size - 1, repetitions);
                break;
            default:
                cout << "Invalid choice" << endl;
                return;
            }
            cout << "Average time taken for size " << size << ": " << totalTime << " milliseconds" << endl;
            averageTimes.push_back(totalTime / repetitions);
        }

        cout << "Average times for different array sizes:" << endl;
        for (size_t i = 0; i < arraySizes.size(); ++i)
        {
            cout << "Size: " << arraySizes[i] << " | Average Time: " << averageTimes[i] << " milliseconds" << endl;
        }
    }

    ~SortTester()
    {
        delete[] arr;
    }
};

int main()
{
    int choice;
    int size;
    int repetitions;
    SortTester<int> intTester;

    do
    {
        cout << "Choose sorting method:" << endl;
        cout << "1. Insertion Sort" << endl;
        cout << "2. Heap Sort" << endl;
        cout << "3. Shell Sort" << endl;
        cout << "4. Quick Sort" << endl;
        cout << "Enter your choice (1-4): ";
        cin >> choice;
    } while (choice < 1 || choice > 4);

    int choice2;
    cout << "Enter 1 to generate random array or 2 to read array from file: ";
    cin >> choice2;

    if (choice2 == 1)
    {
        int choice3;
        cout << "Enter 1 to sort the array or 2 to run tests: ";
        cin >> choice3;
        if (choice3 == 1)
        {
            cout << "Enter the size of the array: ";
            cin >> size;
            intTester.generateRandomArray(size);
            cout << "Unsorted array:" << endl;
            intTester.printArray();
            switch (choice)
            {
            case 1:
                intTester.insertionSort();
                break;
            case 2:
                intTester.heapSort();
                break;
            case 3:
                intTester.shellSort(size / 2);
                break;
            case 4:
                intTester.quickSort(0, size - 1);
                break;
            default:
                cout << "Invalid choice" << endl;
                return 1;
            }
            cout << "Sorted array:" << endl;
            intTester.printArray();
        }
        else if (choice3 == 2)
        {
            cout << "Enter the number of repetitions for each array size: ";
            cin >> repetitions;
            intTester.runTests(choice, repetitions);
        }
        else
        {
            cout << "Invalid choice" << endl;
            return 1;
        }
    }
    else if (choice2 == 2)
    {
        string filename;
        cout << "Enter the filename: ";
        cin >> filename;
        intTester.readArrayFromFile(filename);
        cout << "Unsorted array from file:" << endl;
        intTester.printArray();
        switch (choice)
        {
        case 1:
            intTester.insertionSort();
            break;
        case 2:
            intTester.heapSort();
            break;
        case 3:
            intTester.shellSort(intTester.getSize() / 2);
            break;
        case 4:
            intTester.quickSort(0, intTester.getSize() - 1);
            break;
        default:
            cout << "Invalid choice" << endl;
            return 1;
        }
        cout << "Sorted array from file:" << endl;
        intTester.printArray();
    }
    else
    {
        cout << "Invalid choice" << endl;
        return 1;
    }

    return 0;
}
