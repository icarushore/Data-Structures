#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <vector>
#include <utility>
#include "bst.h"
#include "hashtable.h"
using namespace std;

struct DocumentItem {
    string documentName;
    int count;

    // Default constructor
    DocumentItem() : documentName(""), count(0) {}

    // Constructor with parameters
    DocumentItem(const string& name, int c) : documentName(name), count(c) {}

    // Comparison operator for sorting
    bool operator<(const DocumentItem& other) const {
        // You might need to adjust the logic based on how you want to compare DocumentItem instances
        return count < other.count;
    }
};


//lowers the input
string to_lower(string word){
    string lowerWord = "";

    for(int i = 0; i < word.length();i++){
        char c = word[i];
        lowerWord += tolower(c);

    }
    return lowerWord;
}
//to be able to get the words from the query
vector<string> queryToWord(const string& query) {
    vector<string> wordVector;
    string word;
    for (int i = 0; i < query.length(); i++) {
        char c = query[i];
        if (c == ' ') { // we came to another word
            wordVector.push_back(word);
            word = "";
        } else {
            word += c;
        }
    }
    // Add the last word to the vector after the loop
    wordVector.push_back(word);

    return wordVector;
}
//count how many words
int countWords(string filename,vector<DocumentItem>* searchVector){
    int countOfWord = 0;
    for(int i = 0; i < searchVector->size();i++){
        if(filename == searchVector->at(i).documentName){
            countOfWord = searchVector->at(i).count;
            break;
        }
    }
    return countOfWord;
}
int countWords(string filename,vector<pair<string,DocumentItem>> searchVector){
    int countOfWord = 0;
    for(int i = 0; i < searchVector.size();i++){
        string documentName = searchVector.at(i).second.documentName;
        if(filename == documentName){
            countOfWord = searchVector.at(i).second.count;;
            break;
        }
    }
    return countOfWord;
}
void BSTOperation(BST<string,vector<DocumentItem>* >* bst, string word,string docName){
    if (bst->find(word) == nullptr) { // if the word does not already exist, insert it
        vector<DocumentItem> *docItemVector = new vector<DocumentItem>;
        DocumentItem docItem(docName, 1);
        docItemVector->push_back(docItem);
        bst->insert(word, docItemVector);
    } else { // if the word already exists in the tree
        bool found = false;

        vector<DocumentItem> *docItemVector = bst->find(word);
        for (int j = 0; j < docItemVector->size(); j++) {
            if (docItemVector->at(j).documentName == docName) {
                // Found the document in the vector, update the count
                docItemVector->at(j).count++;
                found = true; // Set the flag to true since we found and updated the document
                break;
            }
        }

        // If the document is not found in the vector, add a new DocumentItem
        if (!found) {
            DocumentItem docItem(docName, 1);
            docItemVector->push_back(docItem);
        }
    }
}

void HASHOperation(HashTable<string, vector<DocumentItem>*> *hashtable,string word,string docName){
    if (hashtable->find(word) == nullptr) { // if the word does not already exist, insert it
        vector<DocumentItem> *docItemVector = new vector<DocumentItem>;
        DocumentItem docItem(docName, 1);
        docItemVector->push_back(docItem);
        hashtable->insert(word, docItemVector);
    } else { // if the word already exists in the tree
        bool found = false;
        vector<DocumentItem> *docItemVector = hashtable->find(word);
        for (int j = 0; j < docItemVector->size(); j++) {
            if (docItemVector->at(j).documentName == docName) {
                // Found the document in the vector, update the count
                docItemVector->at(j).count++;
                found = true; // Set the flag to true since we found and updated the document
                break;
            }
        }
        // If the document is not found in the vector, add a new DocumentItem
        if (!found) {
            DocumentItem docItem(docName, 1);
            docItemVector->push_back(docItem);
        }
    }
}
void BSTFindOperation(BST<string,vector<DocumentItem>* >* bst,vector<string> waitingWord,vector<string> docNames, bool print){
    string resultBST;
    bool somethingMissing = false;

    for (int j = 0; j < docNames.size(); j++) {

        string doc = docNames[j];

        for (int n = 0; n < waitingWord.size(); n++) {
            string wordQ = waitingWord.at(n);

            vector<DocumentItem> *infoBST = bst->find(wordQ); // info points to the document item vector of our word
            // QueryDocuments(with BST);
            if (infoBST != nullptr) { // if the word is in the tree
                int count = countWords(doc, infoBST); // find the count

                if (count == 0) { // this word is not in this document
                    resultBST = "";
                }
                else {
                    resultBST += wordQ + " found " + to_string(count) + " times ";
                }

            } else { //if the word is not in the tree -> no document would have it
                if(print){
                    somethingMissing = true;
                    cout << "No document contains the given query" << endl;
                    return;
                }
            }
        }
        if (resultBST != "") {
            if(print && !somethingMissing){
                cout << "in Document " << doc;
                cout << ", " << resultBST;
                cout << "." << endl;
                resultBST = "";
            }
        }
    }
}
void HASHFindOperation(HashTable<string, vector<DocumentItem>*> *hashtable,vector<string> waitingWord,vector<string> docNames, bool print){
    string resultHash;
    bool somethingMissing = false;

    for (int j = 0; j < docNames.size(); j++) {
        string doc = docNames[j];
        for (int n = 0; n < waitingWord.size(); n++) {
            string wordQ = waitingWord.at(n);

            vector<DocumentItem> *infoHash = hashtable->find(wordQ);
            // QueryDocuments (with hashtable);
            if (infoHash != nullptr) {

                int count = countWords(doc, infoHash); // find the count

                if (count == 0) { // this word is not in this document
                    resultHash = "";
                }
                else {
                    resultHash += wordQ + " found " + to_string(count) + " times ";
                }
            }
            else {
                if (print){
                    somethingMissing = true;
                    cout << "No document contains the given query" << endl;
                    return;
                }
            }
        }

        if (resultHash != "") {
            if(print && ! somethingMissing){
                cout << "in Document " << doc;
                cout << ", " << resultHash;
                cout << "." <<endl;
                resultHash = "";
            }

        }
    }
}
void InsertToVector(vector<pair<string, vector<DocumentItem>>>& givenVector, string docName, string word) {
    bool found = false;
    string tempDoc = "";

    // Iterate through the vector to check if the word already exists
    for (int i = 0; i < givenVector.size(); i++) {
        string tempWord = givenVector.at(i).first;

        if (tempWord == word) {
            found = true;
            // Check if the document already exists for this word
            for (int j = 0; j < givenVector.at(i).second.size(); j++) {
                tempDoc = givenVector.at(i).second.at(j).documentName;

                if (tempDoc == docName) {
                    // Found the document in the vector, update the count
                    givenVector.at(i).second.at(j).count++;
                }
            }
            // If the document is not found in the vector for this word, add a new DocumentItem
            DocumentItem docItem(docName, 1);
            givenVector.at(i).second.push_back(docItem);
        }
    }

    // If the word does not already exist, insert it with a new DocumentItem vector
    if (!found) {
        vector<DocumentItem> docItemVector;
        DocumentItem docItem(docName, 1);
        docItemVector.push_back(docItem);
        givenVector.push_back(make_pair(word, docItemVector));
    }
}



//FUNCTIONS FOR INSERTION SORTING
template <class First, class Second>
void insertionSort(vector<pair<First, Second>>& a) {
    int j;
    // loop over the passes
    for (int p = 1; p < a.size(); p++) {
        pair<First, Second> tmp = a[p];
        // loop over the elements
        for (j = p; j > 0 && tmp.first < a[j - 1].first; j--) {
            a[j] = a[j - 1];
        }
        a[j] = tmp;
    }
}


//FUNCTIONS FOR HEAP SORTING
int leftChild(int i) {
    return 2 * i + 1;
}

template <class First, class Second>
void percDown(std::vector<std::pair<First, Second>>& a, int i, int n) {
    int child;
    First tmp;

    for (tmp = a[i].first; leftChild(i) < n; i = child) {
        child = leftChild(i);
        if (child != n - 1 && a[child].first < a[child + 1].first) {
            child++;
        }
        if (a[child].first > tmp) {
            a[i] = a[child];
        } else {
            break;
        }
    }
    a[i].first = tmp;
}

template <class First, class Second>
void heapsort(vector<pair<First, Second>>& a) {
    // buildHeap
    for (int i = a.size() / 2; i >= 0; i--) {
        percDown(a, i, a.size());
    }

    // sort
    for (int j = a.size() - 1; j > 0; j--) {
        swap(a[0], a[j]);    // swap max to the last pos.
        percDown(a, 0, j); // re-form the heap
    }
}

//FUNCTIONS FOR MERGE SORTING
template <class First, class Second>
void merge(vector<pair<First, Second>>& a,
           vector<pair<First, Second>>& tmpArray,
           int leftPos, int rightPos, int rightEnd) {
    int leftEnd = rightPos - 1;
    int tmpPos = leftPos;
    int numElements = rightEnd - leftPos + 1;

    // Copy entire pairs during merge
    while (leftPos <= leftEnd && rightPos <= rightEnd) {
        if (a[leftPos].first <= a[rightPos].first) {
            tmpArray[tmpPos++] = a[leftPos++];
        } else {
            tmpArray[tmpPos++] = a[rightPos++];
        }
    }

    // Copy remaining pairs
    while (leftPos <= leftEnd) {
        tmpArray[tmpPos++] = a[leftPos++];
    }

    while (rightPos <= rightEnd) {
        tmpArray[tmpPos++] = a[rightPos++];
    }

    // Copy tmpArray back
    for (int i = 0; i < numElements; ++i, --rightEnd) {
        a[rightEnd] = tmpArray[rightEnd];
    }
}


template <class Comparable>
void mergeSort( vector<Comparable> & a,
                vector<Comparable> & tmpArray, int left, int right )
{
    if ( left < right )
    {
        int center = ( left + right ) / 2;
        mergeSort( a, tmpArray, left, center );
        mergeSort( a, tmpArray, center + 1, right );
        merge( a, tmpArray, left, center + 1, right );
    }
}

/**
      * Mergesort algorithm (driver).
      */
template <class First,class Second>
void mergeSort( vector<pair<First,Second>> & a )
{
    vector<pair<First,Second>> tmpArray( a.size( ) );

    mergeSort( a, tmpArray, 0, a.size( ) - 1 );
}
//FUNCTION FOR QUICK SORT
//FIRST
template <class First, class Second>
int partitionFirst(vector<pair<First, Second>>& a, int low, int high) {
    pair<First, Second> pivot = a[low];
    int left = low + 1;
    int right = high;

    while (true) {
        while (left <= right && a[left].first <= pivot.first) {
            left++;
        }

        while (right >= left && a[right].first >= pivot.first) {
            right--;
        }

        if (left > right) {
            break;
        }

        swap(a[left], a[right]);
    }

    swap(a[low], a[right]);
    return right;
}
template <class First, class Second>
void quicksortFirst(vector<pair<First, Second>>& a, int low, int high) {
    if (low < high) {
        int pivotIndex = partitionFirst(a, low, high);
        quicksortFirst(a, low, pivotIndex - 1);
        quicksortFirst(a, pivotIndex + 1, high);
    }
}
/**
 * Quicksort algorithm (driver).
*/
template <class First,class Second>
void quicksortFirst( vector<pair<First,Second>> & a )
{
    quicksortFirst( a, 0, a.size( ) - 1 );
}


//RANDOM
template <class First, class Second>
int partitionRandom(vector<pair<First, Second>>& a, int low, int high) {
    int pivotIndex = low + rand() % (high - low + 1);
    swap(a[low], a[pivotIndex]);
    pair<First, Second> pivot = a[low];

    int left = low + 1;
    int right = high;

    while (true) {
        while (left <= right && a[left].first <= pivot.first) {
            left++;
        }

        while (right >= left && a[right].first >= pivot.first) {
            right--;
        }

        if (left > right) {
            break;
        }

        swap(a[left], a[right]);
    }

    swap(a[low], a[right]);
    return right;
}

template <class First, class Second>
void quicksortRandom(vector<pair<First, Second>>& a, int low, int high) {
    if (low < high) {
        int pivotIndex = partitionRandom(a, low, high);
        quicksortRandom(a, low, pivotIndex - 1);
        quicksortRandom(a, pivotIndex + 1, high);
    }
}
template <class First, class Second>
void quicksortRandom(vector<pair<First, Second>>& a) {
    quicksortRandom( a, 0, a.size( ) - 1 );
}
//MEDIAN
template <class First, class Second>
int partitionMedianOfThree(vector<pair<First, Second>>& a, int low, int high) {
    int mid = low + (high - low) / 2;

    // Find the median of the first, middle, and last elements
    if (a[mid].first < a[low].first) {
        swap(a[low], a[mid]);
    }
    if (a[high].first < a[low].first) {
        swap(a[low], a[high]);
    }
    if (a[mid].first < a[high].first) {
        swap(a[mid], a[high]);
    }

    pair<First, Second> pivot = a[high - 1]; // Choose the last element as pivot
    int left = low;
    int right = high - 2;

    while (true) {
        while (a[left].first < pivot.first) {
            left++;
        }

        while (a[right].first > pivot.first) {
            right--;
        }

        if (left >= right) {
            break;
        }

        swap(a[left], a[right]);
        left++;
        right--;
    }

    swap(a[left], a[high - 1]);
    return left;
}
template <class First, class Second>
void quicksortMedian(vector<pair<First, Second>>& a, int low, int high) {
    if (low < high) {
        int pivotIndex = partitionMedianOfThree(a, low, high);
        quicksortMedian(a, low, pivotIndex - 1);
        quicksortMedian(a, pivotIndex + 1, high);
    }
}
template <class First, class Second>
void quicksortMedian(vector<pair<First, Second>>& a) {
    quicksortMedian( a, 0, a.size( ) - 1 );
}
template <class First, class Second>
int binarySearch(const vector<pair<First, Second>>& a, const First& target) {
    int low = 0;
    int high = a.size() - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;

        if (a[mid].first == target) {
            return mid; // Found the target
        } else if (a[mid].first < target) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    return -1; // Target not found
}
void BinaryFindOperation(const vector<pair<string, vector<DocumentItem>>>& a, vector<string> waitingWord, vector<string> docNames, bool print) {
    string result;
    bool somethingMissing = false;

    for (int j = 0; j < docNames.size(); j++) {
        string doc = docNames[j];
        for (int n = 0; n < waitingWord.size(); n++) {
            string wordQ = waitingWord.at(n);
            int index = binarySearch(a, wordQ);

            if (index != -1) {
                // Word found, you can access the count or other information from 'a[index].second'
                const vector<DocumentItem>& docItems = a[index].second;
                int count = 0;

                for(int k = 0; k < docItems.size();k++){
                    string docName = docItems.at(k).documentName;

                    if(docName == doc){
                        count = docItems.at(k).count;
                        break;
                    }
                }
                if (count == 0) { // this word is not in this document
                    result = "";
                } else {
                    result += wordQ + " found " + to_string(count) + " times ";
                }
            } else {
                if (print) {
                    somethingMissing = true;
                    cout << "No document contains the given query" << endl;
                    return;
                }
            }
        }

        if (result != "") {
            if (print && !somethingMissing) {
                cout << "in Document " << doc;
                cout << ", " << result;
                cout << "." << endl;
                result = "";
            }
        }
    }
}


int main() {
    int numDoc; // number of documents that are going to be preprocessed
    string sNumDoc;
    cout << "Enter number of input files: ";
    getline(cin, sNumDoc); //get the inputs using getline
    numDoc = stoi(sNumDoc); //turn the string into an int
    vector<pair<string,vector<DocumentItem>>> quickFirst, quickRandom,quickMedian,insertion,merge,heap,temp;

    vector<string> docNames; //stores document names
    for (int i = 0; i < numDoc; i++) {
        string docName;
        cout << "Enter " + to_string(i + 1) + ". file name: ";
        cin >> docName; //get the document names from the user
        docNames.push_back(docName); //push them to the vector
    }


    // PREPROCESS

    // creating a tree & hash
    BST<string, vector<DocumentItem>*> *bst = new BST<string, vector<DocumentItem>*>(NULL);
    HashTable<string, vector<DocumentItem>*> *hashtable = new HashTable<string, vector<DocumentItem>*>(NULL, 53);

    for (int i = 0; i < numDoc; i++) { // loop through all your documents

        string docName = docNames[i];

        ifstream docFile;

        docFile.open(docName);

        if (docFile.is_open()) { // check if the file is successfully opened

            string word;
            vector<string> waitingWord;

            while (docFile >> word) { // read the word from the file
                word = to_lower(word);

                //push back to the vector we will sort
                for (int j = 0; j < word.length(); j++) {
                    char ch = word[j];
                    if (!isalpha(ch)) {
                        string str = "";
                        for (int k = 0; k < j; k++) {
                            if (isalpha(word[k])) {
                                str += word[k];
                            }
                        }
                        if (!str.empty()) {
                            waitingWord.push_back(str);
                        }

                        str = "";
                        for (int n = j + 1; n < word.length(); n++) {
                            if (isalpha(word[n])) {
                                str += word[n];
                            }
                        }
                        if (!str.empty()) {
                            waitingWord.push_back(str);
                        }
                    }
                }
                if (!waitingWord.empty()) {

                    for(int n = 0; n < waitingWord.size();n++){
                        word = waitingWord.at(n);
                        BSTOperation(bst, word, docName);
                        HASHOperation(hashtable, word, docName);
                        InsertToVector(quickFirst,docName,word);
                        InsertToVector(quickRandom,docName,word);
                        InsertToVector(quickMedian,docName,word);
                        InsertToVector(insertion,docName,word);
                        InsertToVector(merge,docName,word);
                        InsertToVector(heap,docName,word);
                        InsertToVector(temp,docName,word);
                    }
                    while(!waitingWord.empty()){
                        waitingWord.pop_back();
                    }

                } else {
                    BSTOperation(bst, word, docName);
                    HASHOperation(hashtable, word, docName);
                    InsertToVector(quickFirst,docName,word);
                    InsertToVector(quickRandom,docName,word);
                    InsertToVector(quickMedian,docName,word);
                    InsertToVector(insertion,docName,word);
                    InsertToVector(merge,docName,word);
                    InsertToVector(heap,docName,word);
                    InsertToVector(temp,docName,word);
                }

            }
            docFile.close();
        }
    }

    hashtable->printPreprocess();
    string wordQuery;
    cin.ignore(); // Ignore newline character from previous input

    //SORT ONE OF THEM
    insertionSort(temp);

    cout << endl;
    string wordQ;
    cout << "Enter queried words in one line: ";
    getline(cin, wordQuery); //get the query from the user
    vector<string> wordVector = queryToWord(wordQuery);
    vector<string> waitingWord;


    // this is for the last word to be counted correctly
    int size = wordVector.size();
    wordVector[size - 1] += "-";

    for (int i = 0; i < wordVector.size(); i++) { //iterate for all the words
        wordQ = to_lower(wordVector.at(i));

        for (int j = 0; j < wordQ.length(); j++) {
            char ch = wordQ[j];
            if (!isalpha(ch)) { // if it is a non alpha character
                string str = "";
                for (int k = 0; k < j; k++) {
                    if (isalpha(wordQ[k])) {
                        str += wordQ[k];
                    } // str is now the word until that non alpha character
                }
                if (!str.empty()) { // if there is a word before it
                    waitingWord.push_back(str);
                    str = "";
                }

                // str is now an empty string
                // now for the other word after the non alpha character a:b-
                for (int n = j + 1; n < wordQ.length(); n++, j++) {

                    if (isalpha(wordQ[n])) {
                        str += wordQ[n];
                    }
                }
                if (!str.empty()) {
                    waitingWord.push_back(str);
                }
            }

        }

    }
    bool print = true;

    int k = 100;
    clock_t begin_time = clock();
    for (int i = 0; i < k; i++){
        // QueryDocuments with Binary Search Tree
        BSTFindOperation(bst, waitingWord,docNames, print);
        print = false;
    }
    print = true;

    begin_time = clock();
    for (int i = 0; i < k; i++){
        // QueryDocuments with Hash Table
        HASHFindOperation(hashtable,waitingWord,docNames, print);
        print = false;

    }
    print = true;
    begin_time = clock();
    for (int i = 0; i < k; i++){
        // QueryDocuments with Binary Search
        BinaryFindOperation(temp, waitingWord,docNames,print);
        print = false;
    }
    double endBS = float(clock() - begin_time);
    double endBST = float(clock() - begin_time);
    double endHT = float(clock() - begin_time);
    cout << "\nBinary Search Tree Time: " << endBST / k;
    cout << "\nHash Table Time: " << endHT / k;
    cout << "\nBinary Search Time: " << endBS / k;

    //SORTING TIME FOR VECTORS
    int j = 100;
    clock_t begin_time2 = clock();

    begin_time2 = clock();
    quicksortMedian(quickMedian);
    double medianSort = float(clock()-begin_time2);

    begin_time2 = clock();
    quicksortRandom(quickRandom);
    double randomSort = float(clock()-begin_time2);

    begin_time2 = clock();
    quicksortFirst(quickFirst);
    double firstSort = float(clock()-begin_time2);

    begin_time2 = clock();
    insertionSort(insertion);
    double insertionsort = float(clock()-begin_time2);

    begin_time2 = clock();
    mergeSort(merge);
    double mergesort = float(clock()-begin_time2);

    begin_time2 = clock();
    heapsort(heap);
    double heapSort = float(clock()-begin_time2);

    cout << endl << endl;
    cout << "Quick Sort(median) Time: " << to_string(medianSort) << endl;
    cout << "Quick Sort(random) Time: " << to_string(randomSort) << endl;
    cout << "Quick Sort(first) Time: " << to_string(firstSort) << endl;
    cout << "Merge Sort Time: " << to_string(mergesort) << endl;
    cout << "Heap Sort Time: " << to_string(heapSort) << endl;
    cout << "Insertion Sort Time: " << to_string(insertionsort) << endl;
    cout << "Speed Up BST/HST: " << to_string(float(endBST/endHT)) << endl;
    cout << "Speed Up Merge Sort/Quick Sort(Median): " << to_string(float(mergesort/medianSort)) << endl;
    cout << "Speed Up Heap Sort/Quick Sort(Median): " << to_string(float (heapSort/medianSort)) << endl;
    cout << "Speed Up Insertion Sort/Quick Sort(Median): " << to_string(float(insertionsort/medianSort)) << endl;
    cout << endl;
    cout << "Speed Up Binary Search Tree Time / Binary Search: " << to_string(float(endBST/endBS)) << endl;
    cout << "Speed Up Binary Search / Hash Table Time: " << to_string(float(endBS/endHT)) << endl;

    return 0;
}
