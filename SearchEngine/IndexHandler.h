// IndexHandler Header File
//      Ben  Grisz

#ifndef INDEXHANDLER_H
#define INDEXHANDLER_H


/** IndexHandler: takes care of the index structure, and
    thus is the interface bewteen the actual index and the
    Control class, and thereby the rest of the program.
    Mostly and intermediary so that different Index
    implementations can be loaded at any time.
*/


#include "HashIndex.h"
#include "BTreeIndex.h"

class IndexHandler {

private:
    /** Base class pointer to either of the implementations */
    Index* theIndexStructure;

public:

    // CONSTRUCTORS AND DESTRUCTOR
    IndexHandler();
    IndexHandler(string&);
    ~IndexHandler();

    // MUTATORS
    /** Tells the index structure to add data to index */
    void addToIndex(string&, string&, double&);
    /** Clears the index */
    void clearIndex();

    // ACCESSORS
    /** Tells index to write itself to storage */
    void writeIndexToFile();
    /** Searches for word from query and sends result back to Control */
    void getResults(string&, Vessel&); // lookup word and Vessel from Query Processor
};

#endif // INDEXHANDLER_H
