// IndexHandler Source File

#include "IndexHandler.h"


    // CONSTRUCTORS AND DESTRUCTOR
IndexHandler::IndexHandler()
{
    theIndexStructure = new HashIndex;
//    theIndexStructure = new BpTreeIndex;
}


IndexHandler::IndexHandler(string& indexType)
{
    if (indexType == "btree")
    {
        theIndexStructure = new BTreeIndex;
    }
    else if (indexType == "hash")
    {
        theIndexStructure = new HashIndex;
    }
    else
    {
        theIndexStructure = new BTreeIndex;
    }
}

IndexHandler::~IndexHandler()
{
    delete theIndexStructure;
}


    // GETTERS
void IndexHandler::getResults(string& lookupWord,Vessel& theCarrier)
{
    theIndexStructure->fillVessel(lookupWord, theCarrier);
}



    // SETTERS
void IndexHandler::addToIndex(string& some_word, string& doc_ID, double& some_tf)
{
    theIndexStructure->addDataToIndex(some_word, doc_ID, some_tf);
}


void IndexHandler::writeIndexToFile()
{
    string persistent_index("PersistentIndex.txt");
    ofstream fout(persistent_index);
    theIndexStructure->writePersistentFile(fout);
    fout.close();
}

void IndexHandler::clearIndex()
{
    theIndexStructure->clear();
}
