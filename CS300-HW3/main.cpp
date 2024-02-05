#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <vector>
#include "bst.h"
#include "hashtable.h"

using namespace std;

struct DocumentItem{
    string documentName;
    int count;
    DocumentItem(const string& name, int c) : documentName(name), count(c) {}
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

int main() {
    int numDoc; // number of documents that are going to be preprocessed
    string sNumDoc;
    cout << "Enter number of input files: ";
    getline(cin, sNumDoc); //get the inputs using getline
    numDoc = stoi(sNumDoc); //turn the string into an int

    //cin.ignore();

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
                    }
                    while(!waitingWord.empty()){
                        waitingWord.pop_back();
                    }

                } else {
                    BSTOperation(bst, word, docName);
                    HASHOperation(hashtable, word, docName);
                }

            }
            docFile.close();
        }
    }

    hashtable->printPreprocess();
    string wordQuery;
    cin.ignore(); // Ignore newline character from previous input


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
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < k; i++)
    {
        // QueryDocuments(with BST);
        BSTFindOperation(bst, waitingWord,docNames, print);
        print = false;
    }
    auto BSTTime = std::chrono::duration_cast<std::chrono::nanoseconds>
            (std::chrono::high_resolution_clock::now() - start);

    print = true;

    int j = 100;
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < j; i++)
    {
        // QueryDocuments (with hashtable);
        HASHFindOperation(hashtable,waitingWord,docNames, print);
        print = false;
    }
    auto HTTime = std::chrono::duration_cast<std::chrono::nanoseconds>
            (std::chrono::high_resolution_clock::now() - start);

    cout << "\nTime: " << BSTTime.count() / k << "\n";
    cout << "\nTime: " << HTTime.count() / k << "\n";
    double speed = (double) BSTTime.count() / HTTime.count();
    cout << "Speed Up: " << speed << endl; // bak!!

    return 0;
}
