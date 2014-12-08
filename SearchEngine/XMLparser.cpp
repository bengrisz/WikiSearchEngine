// XMLparser Source File


#include "XMLparser.h"


// CONSTRUCTORS AND DESTRUCTOR

XMLparser::XMLparser()
{
    xmlDoc = NULL;
    rootElement = NULL;
    pageElement = NULL;
}

XMLparser::~XMLparser()
{
    if (xmlDoc != NULL)
        delete xmlDoc;
}



// ACCESSORS







// MUTATORS

void XMLparser::load(string& fileName)
{
    const char* xmlFile = fileName.c_str();
    if (xmlDoc != NULL)
        delete xmlDoc;
    xmlDoc = new XMLDocument;
    xmlDoc->LoadFile(xmlFile);
    if (xmlDoc->ErrorID() != 0)
        throw "Could not load XML file";
    try
    {
        rootElement = xmlDoc->RootElement();
        pageElement = rootElement->FirstChildElement("page");
        titleElement = pageElement->FirstChildElement("title");
        revisionElement = pageElement->FirstChildElement("revision");
    }
    catch (...)
    {
        throw "Error loading first elements";
    }
    return;
}



void XMLparser::getTitle(string& titleString)
{
    if (titleElement->GetText() == NULL)
        throw "No title text";
    else
        titleString = titleElement->GetText();
    return;
}


void XMLparser::getUsername(string& usernameString)
{
    XMLElement* contributorElement = revisionElement->FirstChildElement("contributor");
    XMLElement* usernameElement = contributorElement->FirstChildElement("username");
    if (usernameElement == NULL)
        throw "No username element";
    if (usernameElement->GetText() == NULL)
        throw "No username text";
    else
        usernameString = usernameElement->GetText();
    return;
}


void XMLparser::getText(string& textString)
{
    XMLElement* textElement = revisionElement->FirstChildElement("text");
//    if (textElement == NULL)
//        throw "No text element";
    if (textElement->GetText() == NULL)
        throw "No text";
    else
        textString = textElement->GetText();
    return;
}


void XMLparser::getDate(string& dateString)
{
    XMLElement* dateElement = revisionElement->FirstChildElement("timestamp");
    if (dateElement->GetText() == NULL)
        throw "No date";
    else
    {
        string buffer = dateElement->GetText();
        dateString = buffer.substr(0, 10); // get date but not time
    }
}



bool XMLparser::nextDoc()
{
    pageElement = pageElement->NextSiblingElement("page");
    if (pageElement == NULL)
        return false;
    else
    {
        titleElement = pageElement->FirstChildElement("title");
        revisionElement = pageElement->FirstChildElement("revision");
    }
        return true;
}
