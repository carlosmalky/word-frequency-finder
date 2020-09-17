#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

class WordCount
{
public:
    WordCount(string w, int c);
    WordCount();
    string word;
    int count;
    void print();
};

WordCount::WordCount()
{
    word = "";
    count = -1;
}

WordCount::WordCount(string w, int c)
{
    word = w;
    count = c;
}

void WordCount::print()
{
    cout << word << ": " << count << endl;
}

void printVector(vector<string> data)
{
    for (int i = 0; i < data.size(); i++)
        cout << data[i] << ", ";
    cout << endl;
}

void printArrayOfVectors(vector<string> data[], int arraySize)
{
    for (int i = 0; i < arraySize; i++)
    {
        cout << (char)(i + 97) << ": ";
        printVector(data[i]);
    }
}

//------------------------------------------------------------------
// Mergesort implementation with copy array
//------------------------------------------------------------------
void merge_sort(vector<string> &data, vector<string> &copy, int low, int high)
{
    // Check terminating condition
    int range = high - low + 1;
    if (range > 1)
    {
        // Divide the array and sort both halves
        int mid = low + (high - low) / 2;
        merge_sort(data, copy, low, mid);
        merge_sort(data, copy, mid + 1, high);

        // Initialize array indices
        int index1 = low;
        int index2 = mid + 1;
        int index = 0;

        // Merge smallest data elements into copy array
        while (index1 <= mid && index2 <= high)
        {
            if (data[index1] < data[index2])
                copy[index++] = data[index1++];
            else
                copy[index++] = data[index2++];
        }

        // Copy any remaining entries from the first half
        while (index1 <= mid)
            copy[index++] = data[index1++];

        // Copy any remaining entries from the second half
        while (index2 <= high)
            copy[index++] = data[index2++];

        // Copy data back from the temporary array
        for (index = 0; index < range; index++)
            data[low + index] = copy[index];
    }
}

//------------------------------------------------------------------
// Mergesort implementation without copy array
// Best case O(NlogN) - random data
// Worst case O(NlogN) - random data
// Average case O(NlogN) - random data
//------------------------------------------------------------------
void merge_sort_WordCounts_frequency(vector<WordCount> &data, vector<WordCount> &copy, int low, int high)
{
    // Check terminating condition
    int range = high - low + 1;
    if (range > 1)
    {
        // Divide the array and sort both halves
        int mid = low + (high - low) / 2;
        merge_sort_WordCounts_frequency(data, copy, low, mid);
        merge_sort_WordCounts_frequency(data, copy, mid + 1, high);

        // Initialize array indices
        int index1 = low;
        int index2 = mid + 1;
        int index = 0;

        // Merge smallest data elements into copy array
        while (index1 <= mid && index2 <= high)
        {
            if (data[index1].count >= data[index2].count)
                copy[index++] = data[index1++];
            else
                copy[index++] = data[index2++];
        }

        // Copy any remaining entries from the first half
        while (index1 <= mid)
            copy[index++] = data[index1++];

        // Copy any remaining entries from the second half
        while (index2 <= high)
            copy[index++] = data[index2++];

        // Copy data back from the temporary array
        for (index = 0; index < range; index++)
            data[low + index] = copy[index];
    }
}

int main()
{
    //-------------------------------------------------------------------
    // Open input file
    string filename;
    cout << "Please enter a TXT file to analyze: " << endl;
    cin >> filename;
    string outputfilename;
    ifstream din;
    vector<string> vectorWords;
    int pos = 0;
    string str;

    din.open(filename.c_str());
    if (din.fail())
    {
        cout << "Error: Could not open " << filename << endl;
    }
    //din >> str;

    while (din >> str)
    {
        //Puts words in lower case and removes punctuation
        for (int i = 0; i < str.length(); i++)
        {
            str[i] = tolower(str[i]);
            if (ispunct(str[i]))
            {
                str.erase(i--, 1);
            }
        }
        //Add words to vector
        vectorWords.push_back(str);
    }
    din.close();

    //-------------------------------------------------------------------
    // Hybrid Sorting
    //-------------------------------------------------------------------

    //Create array of 26 vectors of strings
    int arraySize = 26;
    vector<string> arrayOfVectors[arraySize];

    // insert empty vectors into array of vectors
    for (int i = 0; i < arraySize; i++)
    {
        vector<string> temp;
        arrayOfVectors[i] = temp;
    }

    // insert words into array of vectors
    for (int i = 0; i < vectorWords.size(); i++)
    {
        string word = vectorWords[i];
        if (word != "")
        {
            int index = word[0] - 97;
            arrayOfVectors[index].push_back(word);
        }
    }

    cout << endl
         << endl
         << "Before merge sort: " << filename << endl
         << endl;

    cout << endl
         << endl
         << "Start of merge sort: " << filename << endl
         << endl;

    //-------------------------------------------------
    // Using Merge Sort
    //-------------------------------------------------

    int low = 0;
    int high = vectorWords.size();
    int range = high - low + 1;
    vector<string> copy(range);
    cout << "Size of vectorWords: " << vectorWords.size() << ", size of copy: " << copy.size() << endl;
    merge_sort(vectorWords, copy, low, high - 1);
    int vectorWordsSize = vectorWords.size();

    string currentWord;

    int wordCounter = 1;
    currentWord = vectorWords[0];

    // new vector for WordCounts to easily sort them
    vector<WordCount> wordCounts;

    //Sort Alphabetically order
    for (int i = 1; i < vectorWordsSize; i++)
    {
        if (currentWord != vectorWords[i])
        {
            // create new WordCount object
            WordCount currentWC(currentWord, wordCounter);

            // add WordCount object to wordCounts vector
            wordCounts.push_back(currentWC);

            // reset counter and current word
            wordCounter = 0;
            currentWord = vectorWords[i];
        }
        wordCounter++;
    }

    // print WordCount vector, sorted alphabetically
    cout << endl
         << endl
         << "Words sorted alphabetically: " << filename << endl;

    string alphabeticaloutputfilename = filename + " - alphabetical.txt";
    ofstream alphabeticalOutput(alphabeticaloutputfilename.c_str());
    for (int i = 0; i < wordCounts.size(); i++)
        alphabeticalOutput << wordCounts[i].word << " - " << wordCounts[i].count << endl;
    alphabeticalOutput.close();

    // Sort by frequency
    cout << endl
         << endl
         << "Words sorted by frequency: " << filename << endl;

    low = 0;
    high = wordCounts.size();
    range = high - low + 1;
    vector<WordCount> wordCountsCopy(range);
    merge_sort_WordCounts_frequency(wordCounts, wordCountsCopy, low, high - 1);

    //-------------------------------------------------
    // Open output file
    //-------------------------------------------------

    string frequencyoutputfilename = filename + " - frequency.txt";
    ofstream frequencyOutput(frequencyoutputfilename.c_str());
    for (int i = 0; i < wordCounts.size(); i++)
        frequencyOutput << wordCounts[i].word << " - " << wordCounts[i].count << endl;
    frequencyOutput.close();

    return 0;
}
