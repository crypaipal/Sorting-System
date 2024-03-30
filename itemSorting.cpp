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
// Klasa SortTester do testowania różnych metod sortowania
class SortTester
{
private:
    T *arr;                    // Wskaźnik do tablicy przechowującej elementy do posortowania
    int size;                  // Rozmiar tablicy
    static int dataTypeChoice; // Statyczne pole przechowujące wybór typu danych

public:
    // Konstruktor
    SortTester() : arr(nullptr), size(0) {}

    // Metoda zwracająca wybór typu danych
    static int getDataTypeChoice()
    {
        return dataTypeChoice;
    }

    // Metoda ustawiająca wybór typu danych
    static void setDataTypeChoice(int choice)
    {
        dataTypeChoice = choice;
    }

    // Metoda generująca losową tablicę
    void generateRandomArray(int sz)
    {
        size = sz;
        arr = new T[size];
        srand(time(0));
        for (int i = 0; i < size; ++i)
        {
            if (dataTypeChoice == 1) // generowanie tablicy typu int
            {
                arr[i] = static_cast<T>(rand() % 15001 - 5000); // zakres generowania [-5000;10000]
            }
            else // generowanie tablicy typu float
            {
                float lowerLimit = -5000.0f;
                float upperLimit = 10000.0f;
                float range = upperLimit - lowerLimit;
                float random = static_cast<float>(rand()) / RAND_MAX;
                arr[i] = (random * range) + lowerLimit;
            }
        }
    }

    // Metoda wyświetlająca tablicę
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

    // Metoda wczytująca tablicę z pliku
    // &filename - nazwa pliku, z którego wczytywana jest tablica.
    void readArrayFromFile(const string &filename)
    {
        // Otwarcie pliku
        ifstream file(filename);
        // Sprawdzenie, czy plik został poprawnie otwarty
        if (!file)
        {
            // Wyświetlenie komunikatu o błędzie i zakończenie programu
            cerr << "Unable to open file " << filename << endl;
            exit(1);
        }

        // Inicjalizacja licznika elementów tablicy
        size = 0;
        // Zmienna pomocnicza do wczytywania danych z pliku
        T temp;
        // Pętla wczytująca elementy z pliku i zliczająca liczbę elementów
        while (file >> temp)
        {
            size++;
        }

        // Wyczyszczenie flagi błędu i ustawienie wskaźnika pliku na początek
        // metoda (seekg) ustawia wskaźnik odczytu (get pointer) na określonej pozycji w strumieniu
        // ios::beg oznacza początek pliku
        file.clear();
        file.seekg(0, ios::beg);

        // Inicjalizacja dynamicznej tablicy na podstawie liczby elementów wczytanych z pliku
        arr = new T[size];

        for (int i = 0; i < size; i++)
        {
            file >> arr[i];
        }
        file.close();
    }

    // Metoda sprawdzająca, czy tablica jest posortowana
    bool isSorted()
    {
        for (int i = 0; i < size - 1; ++i)
        {
            if (arr[i] > arr[i + 1])
                return false;
        }
        return true;
    }

    // Implementacja algorytma przez wstawianie (Insertion Sort)
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

    // Implementacja algorytma przez kopcowanie (Heap Sort)
    // Metoda pomocnicza
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

    // Główna metoda algorytma przez kopcowanie
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

    // Implementacja algorytma Shell'a (Shell Sort)
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

    // Implementacja algorytma szybkiego (Quick Sort)
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

    // Metoda pomocnicza do sortowania szybkiego
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

    // Metoda mierząca czas wykonania dla wybranej metody sortowania
    // *sortMethod - wskaźnik do metody sortowania w klasie SortTester
    // repetitions - liczba powtórzeń, dla których mierzony jest czas
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

    // Metoda mierząca czas wykonania dla sortowania szybkiego
    // *sortMethod - wskaźnik do metody sortowania w klasie SortTester
    // repetitions - liczba powtórzeń, dla których mierzony jest czas
    // left - wartość lewego indeksu w algorytmie QuickSort
    // right - wartość prawego indeksu w algorytmie QuickSort
    long long measureTime(void (SortTester::*sortMethod)(int, int), int left, int right, int repetitions)
    {
        auto start = high_resolution_clock::now();
        for (int i = 0; i < repetitions; i++)
        {
            (this->*sortMethod)(left, right); // Wywołanie metody sortowania z określonymi lewym i prawym indeksem
        }
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop - start);
        return duration.count();
    }

    // Metoda mierząca czas wykonania dla sortowania Shell'a
    // gap - wartość odstępu używana w sortowaniu Shell'a
    // *sortMethod - wskaźnik do metody sortowania w klasie SortTester
    // repetitions - liczba powtórzeń, dla których mierzony jest czas
    long long measureTime(void (SortTester::*sortMethod)(int), int gap, int repetitions)
    {
        auto start = high_resolution_clock::now();
        for (int i = 0; i < repetitions; i++)
        {
            (this->*sortMethod)(gap); // Wywołanie metody sortowania z określonym odstępem
        }
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop - start);
        return duration.count();
    }

    // Metoda zwracająca rozmiar tablicy
    int getSize() const { return size; }

    // Metoda uruchamiająca testy sortowania
    void runTests(int choice, int repetitions, const vector<int> &arraySizes)
    {
        vector<long long> averageTimes;

        for (int size : arraySizes)
        {
            cout << "Sorting array of size: " << size << endl;
            generateRandomArray(size);

            // Mierzenie czasu dla wybranej metody sortowania dla danego rozmiaru tablicy
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

            // Wyświetlanie czasu dla wybranej metody sortowania dla danego rozmiaru tablicy
            cout << "Total time taken for size " << size << " and " << repetitions << " repetitions "
                 << ": " << totalTime << " milliseconds" << endl;
            averageTimes.push_back(totalTime / repetitions);
        }

        // Wyświetlanie średnich czasów dla różnych rozmiarów tablicy
        cout << "Average times for different array sizes:" << endl;
        for (size_t i = 0; i < arraySizes.size(); i++)
        {
            cout << "Size: " << arraySizes[i] << " | Average Time: " << averageTimes[i] << " milliseconds" << endl;
        }
    }

    // Destruktor
    // Usuwa zaalokowaną pamięć dla tablicy
    ~SortTester()
    {
        delete[] arr;
    }
};

// Inicjalizacja statycznego pola dataTypeChoice
template <typename T>
int SortTester<T>::dataTypeChoice = 0;

int main()
{
    vector<int> arraySizes = {20000, 40000, 50000, 80000, 100000, 200000, 300000};
    int answer; // Wybór typu danych: int lub float
    do
    {
        cout << "Choose data type:" << endl;
        cout << "1. Integer" << endl;
        cout << "2. Float" << endl;
        cout << "Enter your choice (1-2): ";
        cin >> answer;
    } while (answer < 1 || answer > 2);

    // Ustawienie wyboru typu danych
    SortTester<int>::setDataTypeChoice(answer);

    // Wybór metody sortowania i operacji na tablicy
    int choice;
    int size;
    int repetitions;
    // wywołanie statycznej metody getDataTypeChoice() klasy SortTester zparametryzowanej typem int
    if (SortTester<int>::getDataTypeChoice() == 1)
    {
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

        if (choice2 == 1) // Generowanie losowej tablicy
        {
            // Operacja sortowania lub testowania
            int choice3;
            cout << "Enter 1 to sort the array or 2 to run tests: ";
            cin >> choice3;
            if (choice3 == 1) // Sortowanie tablicy
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
            else if (choice3 == 2) // Testowanie czasu metody sortowania
            {
                cout << "Enter the number of repetitions for each array size: ";
                cin >> repetitions;
                intTester.runTests(choice, repetitions, arraySizes);
            }
            else
            {
                cout << "Invalid choice" << endl;
                return 1;
            }
        }
        else if (choice2 == 2) // Wczytywanie tablicy z pliku
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
    }
    // wywołanie statycznej metody getDataTypeChoice() klasy SortTester zparametryzowanej typem int
    else if (SortTester<int>::getDataTypeChoice() == 2)
    {
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
            int choice3;
            cout << "Enter 1 to sort the array or 2 to run tests: ";
            cin >> choice3;
            if (choice3 == 1)
            {
                cout << "Enter the size of the array: ";
                cin >> size;
                floatTester.generateRandomArray(size);
                cout << "Unsorted array:" << endl;
                floatTester.printArray();
                switch (choice)
                {
                case 1:
                    floatTester.insertionSort();
                    break;
                case 2:
                    floatTester.heapSort();
                    break;
                case 3:
                    floatTester.shellSort(size / 2);
                    break;
                case 4:
                    floatTester.quickSort(0, size - 1);
                    break;
                default:
                    cout << "Invalid choice" << endl;
                    return 1;
                }
                cout << "Sorted array:" << endl;
                floatTester.printArray();
            }
            else if (choice3 == 2)
            {
                cout << "Enter the number of repetitions for each array size: ";
                cin >> repetitions;
                floatTester.runTests(choice, repetitions, arraySizes);
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
            floatTester.readArrayFromFile(filename);
            cout << "Unsorted array from file:" << endl;
            floatTester.printArray();
            switch (choice)
            {
            case 1:
                floatTester.insertionSort();
                break;
            case 2:
                floatTester.heapSort();
                break;
            case 3:
                floatTester.shellSort(floatTester.getSize() / 2);
                break;
            case 4:
                floatTester.quickSort(0, floatTester.getSize() - 1);
                break;
            default:
                cout << "Invalid choice" << endl;
                return 1;
            }
            cout << "Sorted array from file:" << endl;
            floatTester.printArray();
        }
        else
        {
            cout << "Invalid choice" << endl;
            return 1;
        }
    }
    return 0;
}
