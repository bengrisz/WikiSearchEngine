// XML Parser Header File
//      Ben Grisz

#ifndef XMLPARSER_H
#define XMLPARSER_H


/** XML Parser

    This is the class that uses the tinyxml2 parser
    to load XML files, read through them, and extract
    the elements from their nodes, either to put that
    info into the index, or to display it when we are
    searching for words.
*/


#include <iostream>
#include <string>
using namespace std;
#include "tinyxml2.h"
using namespace tinyxml2;


class XMLparser {

private:

    XMLDocument* xmlDoc;
    XMLNode* rootElement;
    XMLElement* pageElement;
    XMLElement* titleElement;
    XMLElement* revisionElement;

public:

    XMLparser();
    ~XMLparser();

    /** Loads an XML file using its file name */
    void load(string&);

    /** Stores the title of a page into a string */
    void getTitle(string&);
    /** Stores the username of the page into a string */
    void getUsername(string&);
    /** Stores the text into a string */
    void getText(string&);
    /** Stores the date in a string */
    void getDate(string&);

    /** Advances to the next document/page in the file */
    bool nextDoc();

};


#endif // XMLPARSER_H
