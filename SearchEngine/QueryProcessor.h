// QueryProcessor Header File
//      Ben Grisz

#ifndef QUERYPROCESSOR_H
#define QUERYPROCESSOR_H


/** Query Processor

    Responsible for combing through queries, splitting them
    into coherent logic, organizing its data structures to
    prepare for the results of the search, and telling the
    Control class what information it needs to formulate an
    answer that best suits the query. It then provides the
    rankings of these results.
*/

#include "Vessel.h"
#include "porter2_stemmer.h"
using namespace std;

class QueryProcessor {

private:
    /** Keeps track of type of query: whether
        or not there was a prefix AND or OR,
        or a NOT and where it is in the query */
    struct Organizer
    {
        bool isAND;
        bool isOR;
        bool isNOT;
        int NOTposition;
        Organizer() : isAND(false), isOR(false), isNOT(false), NOTposition(0) {}
    };
    /** Instantiation of above struct */
    Organizer query_organizer;
    /** Holds the entire query initially */
    string query;
    /** The individual words of the query to look up */
    vector<string> queryWords;
    /** Stores a copy of each Vessel for each word
        in the query before NOT and not including
        AND or OR if one was included*/
    vector<Vessel> resultVector;
    /** Stores a copy of each Vessel for each word
        in the query after NOT if NOT was included */
    vector<Vessel> NOTresults;
    /** A Vessel with all relevant pages along with combined
        and adjusted TF_IDFs for rankings */
    Vessel finalResult;

    friend class Control;

public:
    // CONSTRUCTOR AND DESTRUCTOR
    QueryProcessor();
    ~QueryProcessor();

    // ACCESSORS

    // MUTATORS
    /** Calls setQuery, splitQuery, organizeQuery */
    void prepareSearch(string&);
    /** Stores the string into the titular string */
    void setQuery(string&);
    /** Stores the individual components of the query */
    void splitQuery();
    /** Uses  logic to deduce which wordsneed to be
        searched, how their results should be
        combined, etc. */
    void organizeQuery();
    /** Make sure ASCII characters. Stem. */
    void parseWord(string&);

    /** Rips information from carrier for search results */
    void storeSearchResult(Vessel&);
    /** Rips information from carrier for search results
        following the NOT expression */
    void storeNOTresult(Vessel&);
    /** Perform Boolean logic operations */
    void combineResults();

    /** Clears all query processor data */
    void clear();
};


#endif // QUERYPROCESSOR_H
