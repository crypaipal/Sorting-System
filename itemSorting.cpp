#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <chrono>

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
            arr[i] = static_cast<T>(rand());
        }
    }

    void printArray()
    {
        for (int i = 0; i < size; ++i)
        {
            cout << arr[i] << " ";
        }
        cout << endl
             << "---------------------------------------------------------------------------------------------------" << endl;
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

            cout << "Step " << i << ": ";
            printArray();
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

            cout << "Step " << size - i << ": ";
            printArray();
        }
    }

    void shellSort(int gap)
    {
        int iteration = 0;
        for (int gap = size / 2; gap > 0; gap /= 2)
        {
            for (int i = gap; i < size; ++i)
            {
                T temp = arr[i];
                int j;
                for (j = i; j >= gap && arr[j - gap] > temp; j -= gap)
                    arr[j] = arr[j - gap];
                arr[j] = temp;

                cout << "Iteration: " << iteration << ": ";
                printArray();
            }
            iteration++;
        }
    }

    void quickSort(int left, int right)
    {
        if (left < right)
        {
            int pivotIndex = partition(left, right);
            quickSort(left, pivotIndex - 1);
            quickSort(pivotIndex + 1, right);
        }
    }

    int partition(int left, int right)
    {
        int iteration = 0;
        T pivot = arr[right];
        int i = left - 1;
        for (int j = left; j <= right - 1; ++j)
        {
            if (arr[j] < pivot)
            {
                i++;
                swap(arr[i], arr[j]);
            }
        }
        swap(arr[i + 1], arr[right]);

        cout << "Iteration: " << iteration << " Step: " << right - left + 1 << ": ";
        printArray();

        return i + 1;
    }

    void runTests(int choice)
    {
        cout << "Initial array: ";
        printArray();

        auto start = high_resolution_clock::now();

        switch (choice)
        {
        case 1:
            insertionSort();
            break;
        case 2:
            heapSort();
            break;
        case 3:
            shellSort(size / 2);
            break;
        case 4:
            quickSort(0, size - 1);
            break;
        default:
            cout << "Invalid choice" << endl;
            return;
        }

        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop - start);
        cout << "Time taken: " << duration.count() << " milliseconds" << endl;

        if (!isSorted())
            cout << "Array is not sorted!" << endl;
        else
            cout << "Array is sorted." << endl;
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
    SortTester<int> intTester;
    SortTester<float> floatTester;

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
        cout << "Enter the size of the array: ";
        cin >> size;
        intTester.generateRandomArray(size);
    }
    else if (choice2 == 2)
    {
        string filename;
        cout << "Enter the filename: ";
        cin >> filename;
        intTester.readArrayFromFile(filename);
    }
    else
    {
        cout << "Invalid choice" << endl;
        return 1;
    }

    intTester.runTests(choice);

    return 0;
}
