// Vessel Header File
//      Ben Grisz

#ifndef VESSEL_H
#define VESSEL_H


/** Vessel

    This is the versatile worker-bee class. Its
    instantiated objects store specific information
    about a word, in a series of paired strings and
    doubles that represent which document it can be
    found in and how frequently compared to other
    words. It is the map value for each index structure
    in which the words are the keys.

    It conducts all of the logical
    operations: union (OR), intersection (AND), and
    difference (NOT) for the Query Processor.

    The carrier instantiation, a member of the Control
    class, is what gets sent to the index and back to
    retrieve search results, and it gives them to the
    Query Processor, which then stores the information
    in its own member vectors.

    Lastly, it also sorts itself, which is how the
    rankings are found in the final results.

*/


#include <vector>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;


class Vessel {

private:
    /** Holds the crucial information for the index and
        for the search engine: Document ID and term frequency. */
    struct DocData
    {
        string docID;
        double term_freq;
        DocData(string& documentID, double num): docID(documentID), term_freq(num) {}
    };
    /** Inline member struct function that compares the two
        doubles. This is used for sorting the results after
        tf has been converted to tf_idf. */
    struct CompareOp
    {
        bool inline operator() (const DocData& doc_data1, const DocData& doc_data2)
        {
            return (doc_data1.term_freq > doc_data2.term_freq);
        }
    };
    /** The container of all the documents a word appears in
        together with the frequency it appears in it. */
    vector<DocData> WordData;

public:
    // CONSTRUCTORS AND DESTRUCTOR
    Vessel();
    Vessel(string&, double&);
    Vessel(const Vessel&);
    ~Vessel();

    // GETTERS
    /** Returns the number of string/double pairs stored */
    int getNumDocs();
    /** Gets Document ID from a particular address in the vector */
    void getDocID(string&, int);
    /** Returns whether or not a document ID is in the vessel */
    bool is_in_DocData(string&); // possibly for Maintenance Mode

    // SETTERS
    /** Adds a string/double pair by pushing back the vector */
    void addDocData(string&, double&); // adds docID and count to Vessel
    /** Copies all information from another Vessel */
    void copy(Vessel&);
    /** Clears data in Vessel */
    void clearVessel();

    // RESULT OPERATIONS FOR QUERY PROCESSOR
    /** Multiplies TF by IDF for final result rankings */
    void calc_TF_IDF(double&); // converts tf to tf-idf for ranking search results
    /** Stores union of 'this' and the other Vessel in 'this' */
    void getUnion(Vessel&);
    /** Stores intersection of 'this' and the other Vessel in 'this' */
    void getIntersection(Vessel&); // LOGICAL OPERATORS for Query Processor
    /** Removes from 'this' the documents that are in the other Vessel */
    void getDifference(Vessel&);
    /** Sorts entire vector; highest ranking is best */
    void sort(); // sorts WordData putting highest TF(TF_IDF) first, descending order

    // OTHER
    /** Writes contents of the Vessel to an output */
    void writeCSV(ofstream&);
    /** Prints contents to the console */
    void printVessel(); // test for checking contents

};




#endif // VESSEL_H
