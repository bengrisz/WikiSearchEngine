// Vessel Source File





#include "Vessel.h"


// CONSTRUCTORS AND DESTRUCTORS
Vessel::Vessel()
{


}

Vessel::Vessel(string& docID, double& some_tf)
{
    DocData newData(docID, some_tf);
    WordData.push_back(newData);
}

Vessel::Vessel(const Vessel& otherVessel)
{
    WordData = otherVessel.WordData;
}

Vessel::~Vessel()
{
    WordData.clear();
}




// ACCESSORS
int Vessel::getNumDocs()
{
    int num_docs = WordData.size();
    return num_docs;
}

void Vessel::printVessel()
{
    int size = WordData.size();
    for (int v = 0; v < size; v++)
    {
        cout << '\t' << "PageID: " << WordData[v].docID << endl;
        cout << '\t' << "Term F: " << WordData[v].term_freq << endl;
    }
}

void Vessel::getDocID(string& in_here, int doc_number)
{
    in_here = WordData[doc_number].docID;
}

bool Vessel::is_in_DocData(string& some_docID) // unnecessary?
{
    int size = WordData.size();
    for (int v = 0; v < size; v++)
    {
        if (some_docID == WordData[v].docID)
            return true;
    }
    return false;
}


void Vessel::writeCSV(ofstream& fout)
{
    int size = WordData.size();
    for (int v = 0; v < size; v++)
    {
        fout << WordData[v].docID << ","
             << WordData[v].term_freq << ",";
    }
}


// MUTATORS
void Vessel::addDocData(string& documentID, double& some_termF)
{
    DocData newData(documentID, some_termF);
    WordData.push_back(newData);
}



void Vessel::clearVessel()
{
    WordData.clear();
}



void Vessel::copy(Vessel& otherVessel)
{
    WordData = otherVessel.WordData;
}


// FOR MANIPULATING RESULTS IN QUERY PROCESSOR
void Vessel::getIntersection(Vessel& otherVessel)
{
    int thisSize = WordData.size();
    int otherSize = otherVessel.WordData.size();
    if (thisSize == 0 || otherSize == 0)
    {
        WordData.clear();
        return;
    }
    vector<int> RemovePositions;
    bool found;
    for (int t = 0; t < thisSize; t++)
    {
        found = false;
        for (int o = 0; o < otherSize; o++)
        {
            if (otherVessel.WordData[o].docID == WordData[t].docID)
            {
                found = true;
                WordData[t].term_freq += otherVessel.WordData[o].term_freq;
                break;
            }
        }
        if (!found)
            RemovePositions.push_back(t);
    }
    if (RemovePositions.size() == 0)
        return;
    int position;
    for (int RP = RemovePositions.size() - 1; RP >= 0; RP--)
    {
        position = RemovePositions[RP];
        WordData.erase(WordData.begin() + position);
    }
}

void Vessel::getUnion(Vessel& otherVessel)
{
    int thisSize = WordData.size();
    int otherSize = otherVessel.WordData.size();

    bool found;
    for (int t = 0; t < thisSize; t++)
    {
        for (int o = 0; o < otherSize; o++)
        {
            if (otherVessel.WordData[o].docID == WordData[t].docID)
                WordData[t].term_freq += otherVessel.WordData[o].term_freq;
        }
    }

    for (int o = 0; o < otherSize; o++)
    {
        found = false;
        for (int t = 0; t < thisSize; t++)
        {
            if (otherVessel.WordData[o].docID == WordData[t].docID)
                found = true;
        }
        if (!found)
            addDocData(otherVessel.WordData[o].docID, otherVessel.WordData[o].term_freq);
    }

}

void Vessel::getDifference(Vessel& otherVessel)
{
    vector<int> RemovePositions;
    int thisSize = WordData.size();
    int otherSize = otherVessel.WordData.size();
    if (otherSize == 0)
        return;
    for (int o = 0; o < otherSize; o++)
    {
        for (int t = 0; t < thisSize; t++)
        {
            if (otherVessel.WordData[o].docID == WordData[t].docID)
            {
                RemovePositions.push_back(t);
                break;
            }
        }
    }
    if (RemovePositions.size() == 0)
        return;
    int position;
    for (int RP = RemovePositions.size() - 1; RP >= 0; RP--)
    {
        position = RemovePositions[RP];
        WordData.erase(WordData.begin() + position);
    }
}

void Vessel::sort()
{
    std::sort(WordData.begin(), WordData.end(), CompareOp());
}


void Vessel::calc_TF_IDF(double& word_idf)
{
    int WDsize = WordData.size();
    for (int z = 0; z < WDsize; z++)
        WordData[z].term_freq = (WordData[z].term_freq)*word_idf;
}
