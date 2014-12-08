// BTree Index Header File
//      Ben Grisz

#ifndef BTREEINDEX_H
#define BTREEINDEX_H


/** B Tree Index Class

  Used for storing the index in memory. One of two index structures.
  Has 4 overwritten virtual functions derived from abstract base class Index.
*/

#include "Index.h"
#include "btree_map.h"
using namespace btree;


class BTreeIndex : public Index {

private:
    /** the btree_map that holds index in memory*/
    btree_map<string, Vessel> B_tree;

public:

    BTreeIndex();
    ~BTreeIndex();

    /** adds a word as the key, and a Vessel with
        document and term frequency info*/
    void addDataToIndex(string&, string&, double&);
    /** takes vessel from search, gives it relevant
        info about word in index*/
    void fillVessel(string&, Vessel&);
    /** writes index to CSV file*/
    void writePersistentFile(ofstream&);
    /** clears the entire index*/
    void clear();

};


#endif // BTREEINDEX_H
