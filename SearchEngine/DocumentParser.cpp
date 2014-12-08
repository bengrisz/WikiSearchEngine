// DocumentParser Source File




#include "DocumentParser.h"
#include "porter2_stemmer.h"


// CONSTRUCTORS AND DESTRUCTOR

DocumentParser::DocumentParser()
{
    xmlParse = new XMLparser;
    document_number = 0;
}

DocumentParser::~DocumentParser()
{
    delete xmlParse;
    for (int y = 0; y < stopSize; y++)
        delete stopWords[y];
    delete [] stopWords;
}


// ACCESSORS




// MUTATORS

void DocumentParser::loadStopWords()
{
    string filename = "../StopWords2.txt";
    string buffer;
    vector<string> stop_words;
    ifstream fin;
    fin.open(filename);
    while (getline(fin, buffer))
        stop_words.push_back(buffer);
    fin.close();
    stopSize = stop_words.size();
    stopWords = new char*[stopSize];
    for (int w = 0; w < stopSize; w++)
    {
        stopWords[w] = new char[stop_words[w].size() + 1];
        strcpy(stopWords[w], stop_words[w].c_str());
    }

    return;
}


void DocumentParser::loadXMLfile(string& fileName)
{
    document_number = 0;
    try
    {
        xmlParse->load(fileName);
    }
    catch (...)
    {
        cout << "Error loading file " << fileName << endl;
        throw "No file";
    }
}



void DocumentParser::getTokens()
{
    stripNonASCII(text_string);
    int reserveNum = 1;
    int helpcount = 0;
    bool stopBool;
    string stemBuffer;
    tokens.reserve(reserveNum*10);
    char* textCstring = new char[text_string.size() + 1];
    strcpy(textCstring, text_string.c_str());
    char* strToken;
    strToken = strtok(textCstring, " "); // splits text up by commas (everything else already stripped)
    while (strToken != NULL)
    {
        stopBool = false;
        for (int y = 0; y < stopSize; y++)
        {
            if (strcmp(strToken, stopWords[y]) == 0)
            {
                stopBool = true;
                break;
            }
        }
        if (stopBool == false)
        {
            stemBuffer = string(strToken); // could make faster using >> now that it's all whitespace
            Porter2Stemmer::stem(stemBuffer);
            tokens.push_back(stemBuffer);
            helpcount++;
            if (helpcount == 10)
            {
                reserveNum++;
                tokens.reserve(reserveNum*10);
                helpcount = 0;
            }
        }
        strToken = strtok(NULL, " "); // continues where strtok left off
    }
    delete [] textCstring;
    return;
}


void DocumentParser::stripNonASCII(string& some_token)
{
    for (string::iterator it = some_token.begin(), itEnd = some_token.end(); it != itEnd; it++)
    {
        if (static_cast<unsigned int>(*it) < 65 || static_cast<unsigned int>(*it) > 122)
            (*it) = ' ';
        else if (static_cast<unsigned int>(*it) > 90 && static_cast<unsigned int>(*it) < 97)
            (*it) = ' ';
        else if (static_cast<unsigned int>(*it) > 64 && static_cast<unsigned int>(*it) < 91)
            (*it) += 32;
    }
    return;
}


void DocumentParser::getDocElements()
{
    try
    {
        xmlParse->getTitle(title_string);
        xmlParse->getDate(date_string);
        xmlParse->getUsername(username_string); // not every doc has a username
        xmlParse->getText(text_string);
    }
    catch (...)
    {
        throw "Could not get Doc Elements";
    }
    return;
}


bool DocumentParser::nextDocument()
{
    if (xmlParse->nextDoc())
    {
        document_number++; // for a particular file
        return true;
    }
    else
        return false;
}

void DocumentParser::consolidateTokens()
{
    int num_words_in_doc = tokens.size();
    if (num_words_in_doc == 0)
    {
        throw "No tokens";
        return;
    }
    docWordData.reserve(num_words_in_doc);
    int docDataSize;
    int counter;
    for (int t = 0; t < num_words_in_doc; t++)
    {
        docDataSize = docWordData.size(); // size will change each time new word is added
        counter = 0;
        for (int d = 0; d < docDataSize; d++)
        {
            if (tokens[t] == docWordData[d].theWord) // if word has already been added to docWordData
            {
                docWordData[d].theCount += 1;
                break;
            }
            else
                counter++; // count number of words gone through with no match
        }
        if (counter == docDataSize) // no match
        {
            WordCountPair addWord(tokens[t]); // constructor
            docWordData.push_back(addWord);
        }
    }
}


void DocumentParser::clear()
{
    tokens.clear();
    docWordData.clear();
    text_string = "";
    title_string = "";
    date_string = "";
    username_string = "";
}


void DocumentParser::generateDocID(string& doc_ID)
{

    if (document_number < 10)
        doc_ID += "00" + to_string(document_number);
    else if (document_number < 100)
        doc_ID += "0" + to_string(document_number);
    else
        doc_ID += to_string(document_number);
}


