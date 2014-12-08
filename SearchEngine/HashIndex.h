// HashIndex Header File
//      Ben Grisz


#ifndef HASHINDEX_H
#define HASHINDEX_H

/** Hash Table Index Class

  Used for storing the index in memory. One of two index structures.
  Has 4 overwritten virtual functions derived from abstract base class Index.
*/

#include "Index.h"
#include <unordered_map>
#include <functional> // use function 'hash' to get key values?

class HashIndex : public Index {

private:
    /** the unordered_map that holds index in memory*/
    unordered_map<string, Vessel> hash_map;

public:

    HashIndex();
    ~HashIndex();

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




#endif // HASHINDEX_H
