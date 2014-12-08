// Control Header File
//      Ben Grisz

#ifndef CONTROL_H
#define CONTROL_H

/** Control Class, the class that puts everything together

    Owns the DocumentParser, IndexHandler (which handles whichever index
    implementation there is), and the QueryProcessor. It also has the ongoing
    while loops that run the program and create the interactive, maintenance
    and stress test modes.

*/

#include "IndexHandler.h"
#include "DocumentParser.h"
#include "QueryProcessor.h"
#include <dirent.h>
#include <sstream>


class Control {

private:
    /** This is the Index Handler, which owns the Index,
        either HashIndex or BTreeIndex */
    IndexHandler* index_handler;
    /** This is the Document Parser, which owns the XML Parser */
    DocumentParser* document_parser;
    /** This is the Query Processor, which organizes searches and
        ranks and displays the results */
    QueryProcessor* query_processor;
    /** This is the carrier Vessel, which retrieves the relevant
        document ID and term frequency from the index for a certain
        word */
    Vessel carrier;

    int current_file;
    /** Keeps track of the files that have already been parsed and
        added to the index */
    vector<int> files_indexed;
    /** A struct that holds all relevant metadata about a particular
        document for displaying search results */
    struct DocumentInfo
    {
        string title;
        string username;
        string date_submitted;

        DocumentInfo() : title(""), username(""), date_submitted("") {}
        DocumentInfo(string& Title, string& UsrName, string& Date) :
            title(Title), username(UsrName), date_submitted(Date) {}
    };
    /** A hash table that stores all this DocumentInfo metadata */
    unordered_map<string, DocumentInfo> DocMeta;

public:

    Control();
    Control(string&);
    ~Control();

    // OPERATION
    /** Continuous while loop that is the basis for the program. For
        normal operation (once parsing has all been done), this acts
        as the multilevel user interface and makes calls to all
        relevant functions after providing simple instruction options
        for the user */
    void MainMenu();
    /** Sub while loop. User interface for adding new files and
        clearing the index.*/
    bool Maintenance();
    /** Sub while loop. User interface for searching for a boolean
        query and switching index structures. */
    bool Interactive();
    /** Sub while loop. Takes a set of predefined commands from a file
        provided by the user and executes them; has all capabilities of
        Maintenance and Interactive plus more.

        search query:       BOOLQ (AND/OR) <word1> <word2> ... (NOT) <word1> <word2> ...
        add file to index:  ADD_F <file_name>
        load hash index:    HASHX
        load btree index:   BTIDX
        load idx from disk: LOADP
        clear the index:    CLEAR
        write idx to disk:  WRITE

        Times itself for list of commands. Ouput goes to file in same directory. */
    bool StressTest();
    /** Helper function for Stress Test.*/
    void STmethod(string&, ofstream&, int);

    // DOCUMENT PARSING & INDEXING
    /** Tells the Document Parser to load an XML file into the XML Parser.*/
    void loadXMLtoParse(string&); // File --- Document Parser
    /** Loads file of the form <../WikiDump/WikiDumpPart###.xml> into the
        parser, extracts info and puts into index. */
    void indexFile(int); // File --- Document Parser --- Index Handler
    /** Dissects document text and adds individual word info to index. */
    void indexWordData(); // Document Parser --- Index Handler
    /** Stores metadata about docs for search results. */
    void storeDocMeta(string&, string&, string&, string&);

    // QUERY PROCESSING
    /** Sends 'carrier' Vessel to Index and retreives data for a word. */
    void lookup(string&); // Index Handler --> carrier
    /** Splits up the query into individual strings */
    void parseQuery(string&); // Query Processor
    /** Gets all relevant results needed for the query */
    void retrieveResults(); // Query Processor --- Index Handler
    /** Searches for word from order of query and stores result */
    void search_and_store(int);
    /** Searches for word from (NOT) part of query and stores result */
    void search_store_NOT(int);
    /** The individual search function for a single query */
    bool tempSearch(); // getting words from the command line
    /** Prints ranked results to the console */
    void printSearchResult(int);
    /** Displays original text when user selects to view it */
    void displaySelection(int);

    // OTHER
    /** Can get the names of all files in a directory, ignoring { . , .. , .DS_store } */
    void getDirectory(string, vector<string>&); // gets all file names from directory
    /** Load index info from storage into memory */
    void loadPersistentIndex();
    /** Write index info from memory into storage */
    void writeIndexToCSV();


};

#endif // CONTROL_H
