// B Tree Index Source File



#include "BTreeIndex.h"

/*
    typedef btree::btree_map<int, MyObject*> MyMap;

    MyMap* BuildMap() {
       MyMap *obj_map = new MyMap;

      for (...) {
        int id = ...;
        MyMap::const_iterator lookup = obj_map->find(id);
        if (lookup != obj_map->end()) {
          // "id" already exists.
          continue;
        }
        MyObject *obj = ...;
        obj_map->insert(std::make_pair(id, obj));
      }
      return obj_map;
    }
*/

BTreeIndex::BTreeIndex()
{



}

BTreeIndex::~BTreeIndex()
{
    B_tree.clear();
}




void BTreeIndex::addDataToIndex(string& key, string& some_docID, double& term_frequency)
{
    btree_map<string, Vessel>::const_iterator got = B_tree.find(key);
    if (got == B_tree.end())
    {
        Vessel newVessel(some_docID, term_frequency);
        B_tree[key] = newVessel;
    }
    else
        B_tree[key].addDocData(some_docID, term_frequency); // adds to mapped Vessel if word is found
    return;
}

void BTreeIndex::fillVessel(string& lookup_word, Vessel& queryVessel)
{
    btree_map<string, Vessel>::const_iterator got = B_tree.find(lookup_word);
    if (got == B_tree.end())
        return;
    else
    {
        queryVessel.copy(B_tree[lookup_word]);
        return;
    }
}

void BTreeIndex::writePersistentFile(ofstream& file_out)
{
    string left = "<";
    string right_comma = ">,";
    for (auto& x: B_tree)
    {
        file_out << left << x.first << right_comma;
        x.second.writeCSV(file_out);
    }
}

void BTreeIndex::clear() // print
{
    B_tree.clear();
}
