// DocumentParser Header File
//      Ben Grisz


#ifndef DOCUMENTPARSER_H
#define DOCUMENTPARSER_H


#include "XMLparser.h"
#include <fstream>
#include <iostream>
#include <cstring>
#include <vector>
#include "porter2_stemmer.h"

class DocumentParser {

private:
    /** The XML Parser using tinyxml2 */
    XMLparser* xmlParse;
    /** Holds the stop words for parsing */
    char** stopWords;
    /** Keeps track of how many stop words */
    int stopSize;
    /** Gets titles by going into XML parser
       by reference */
    string title_string;
    /** Gets usernames by going into XML
       parser by reference */
    string username_string;
    /** Gets the text from pages by going
       into XML parser by reference */
    string text_string;
    /** Gets dates by going into XML parser
       by reference */
    string date_string;
    /** The number of the current document
       being parsed for a certain file */
    int document_number;
    /** A container for splitting up the
       individual words from the text */
    vector<string> tokens;
    /** Packages word and term frequency into
        bundles to send off to index with the
        document ID */
    struct WordCountPair // stores word in a string and its count for current document being indexed
    {
        string theWord;
        int theCount;
        WordCountPair(string& someWord) : theWord(someWord), theCount(1) {}
    };
    /** Basically like vector<string> tokens but
        consolidated with multiplicities rather
        than all individual words */
    vector<WordCountPair> docWordData;

    friend class Control;

public:
    // CONSTRUCTORS AND DESTRUCTOR
    DocumentParser();
    ~DocumentParser();
    /** Loading stop words into stopWords */
    void loadStopWords();

    // ACCESSORS


    // MUTATORS
    /** loads XML file into XMLparser, set to first Document */
    void loadXMLfile(string&); // loads XML file into XMLparser, set to first Document
    /** for current document, get title/username/text/date */
    void getDocElements(); // for current document, get title/username/text/date
    /** converts text string into vector of all words */
    void getTokens(); // converts text string into vector of all words
    /** Turns every non-alpha character into delimiter for removal */
    void stripNonASCII(string&);
    /** counts words and creates word-count pair container */
    void consolidateTokens(); // counts words and creates word-count pair container
    /** set to next Document in File */
    bool nextDocument(); // set to next Document in File
    /** converts doc number into string for indexing purposes */
    void generateDocID(string&);
    /** clears the entire Document Parser */
    void clear();

};



#endif // DOCUMENTPARSER_H
