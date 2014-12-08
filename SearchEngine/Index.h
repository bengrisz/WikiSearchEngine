// Index Header File
//      Ben Grisz

#ifndef INDEX_H
#define INDEX_H


/** Abstract Base Class

    Parent class of BTreeIndex and HashIndex
*/


#include "Vessel.h"
using namespace std;

class Index {

private:

public:
    Index();
    virtual ~Index() = 0;

    virtual void fillVessel(string&, Vessel&) = 0;
    virtual void addDataToIndex(string&, string&, double&) = 0;
    virtual void writePersistentFile(ofstream&) = 0;
    virtual void clear() = 0;
};

#endif // INDEX_H
