// QueryProcessor Source File




#include "QueryProcessor.h"




// CONSTRUCTORS AND DESTRUCTOR

QueryProcessor::QueryProcessor()
{
    query = "";
}


QueryProcessor::~QueryProcessor()
{
    clear();
}


void QueryProcessor::prepareSearch(string& theQuery)
{
    setQuery(theQuery);
    splitQuery();
    organizeQuery();
}

void QueryProcessor::setQuery(string& fullQuery)
{
    query = fullQuery;
}

void QueryProcessor::splitQuery()
{
    char splitter[] = " ";
    string stemBuffer;
    char* cQuery = new char[query.size() + 1];
    strcpy(cQuery, query.c_str());
    char* strToken;
    strToken = strtok(cQuery, splitter); // splits text up by spaces
    while (strToken != NULL)
    {
        queryWords.push_back(string(strToken));
        strToken = strtok(NULL, splitter); // continues where strtok left off
    }
    delete [] cQuery;
}


void QueryProcessor::organizeQuery()
{
    int searchSize = queryWords.size();
    int word_num = 0;
    if (queryWords[0] == "AND")
    {
        query_organizer.isAND = true;
        query_organizer.isOR = false;
        word_num = 1;
    }
    else if (queryWords[0] == "OR")
    {
        query_organizer.isAND = false;
        query_organizer.isOR = true;
        word_num = 1;
    }


    for (int Q = word_num; Q < searchSize; Q++)
    {
        if (queryWords[Q] == "NOT" && query_organizer.isNOT == false)
        {
            query_organizer.isNOT = true;
            query_organizer.NOTposition = Q;
            continue;
        }
        try
        {
            parseWord(queryWords[Q]);
        }
        catch (...)
        {
            throw "Invalid Search";
        }
    }

}


void QueryProcessor::parseWord(string& lookup_word)
{
    // CHECK IF IT IS A STOP WORD OR INCORRECT USAGE OF 'AND', 'OR', 'NOT'

    for (string::iterator it = lookup_word.begin(), itEnd = lookup_word.end(); it != itEnd; it++)
    {
        if (static_cast<unsigned int>(*it) < 65 || static_cast<unsigned int>(*it) > 122)
            throw "Invalid Search";
        if (static_cast<unsigned int>(*it) > 90 && static_cast<unsigned int>(*it) < 97)
            throw "Invalid Search";
        if (static_cast<unsigned int>(*it) > 64 && static_cast<unsigned int>(*it) < 91)
            (*it) += 32;
    }


    Porter2Stemmer::stem(lookup_word);

    return;
}


void QueryProcessor::storeSearchResult(Vessel& the_carrier)
{
    Vessel newVessel;
    newVessel.copy(the_carrier);
    resultVector.push_back(newVessel);
}

void QueryProcessor::storeNOTresult(Vessel& the_carrier)
{
    Vessel newVessel;
    newVessel.copy(the_carrier);
    NOTresults.push_back(newVessel);
}




void QueryProcessor::combineResults()
{
    int numResults = resultVector.size();
    int numNOT = NOTresults.size();
    if (query_organizer.isAND)
    {
        finalResult.copy(resultVector[0]); // always add all results for first word
        for (int r = 1; r < numResults; r++)
            finalResult.getIntersection(resultVector[r]);
    }
//    else if (query_organizer.isOR)
//    {
//        finalResult.copy(resultVector[0]);
//        for (int r = 1; r < numResults; r++)
//            finalResult.getUnion(resultVector[r]);
//    }
    else
    {
        finalResult.copy(resultVector[0]); // always add all results for first word
        for (int r = 1; r < numResults; r++)
            finalResult.getUnion(resultVector[r]);
    }

    if (query_organizer.isNOT)
    {
        for (int n = 0; n < numNOT; n++)
            finalResult.getDifference(NOTresults[n]);
    }
}

void QueryProcessor::clear()
{
    query_organizer.isAND = false;
    query_organizer.isOR = false;
    query_organizer.isNOT = false;
    query_organizer.NOTposition = 0;
    resultVector.clear();
    NOTresults.clear();
    queryWords.clear();
    finalResult.clearVessel();
    query = "";
}


