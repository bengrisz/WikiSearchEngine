// HashIndex Source File




#include "HashIndex.h"



HashIndex::HashIndex()
{

}


HashIndex::~HashIndex()
{
    hash_map.clear();
}



void HashIndex::addDataToIndex(string& key, string& documentID, double& tf)
{
    unordered_map<string, Vessel>::const_iterator got = hash_map.find(key);
    if (got == hash_map.end())
    {
        Vessel newVessel(documentID, tf);
        hash_map[key] = newVessel;
    }
    else
        hash_map[key].addDocData(documentID, tf); // adds to mapped Vessel if word is found
    return;
}


void HashIndex::clear()
{
    hash_map.clear();
}


void HashIndex::fillVessel(string& lookup_word, Vessel& queryVessel)
{
    unordered_map<string, Vessel>::const_iterator got = hash_map.find(lookup_word);
    if (got == hash_map.end())
        return;
    else
    {
        queryVessel.copy(hash_map[lookup_word]);
        return;
    }
}



void HashIndex::writePersistentFile(ofstream& file_out)
{
    string left = "<";
    string right_comma = ">,";
    for (auto& x: hash_map)
    {
        file_out << left << x.first << right_comma;
        x.second.writeCSV(file_out);
    }
}

