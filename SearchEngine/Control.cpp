// Control Source File


#include "Control.h"



    // CONSTRUCTORS AND DESTRUCTOR

Control::Control()
{
    index_handler = new IndexHandler;
    document_parser = new DocumentParser;
    document_parser->loadStopWords();
    query_processor = new QueryProcessor;
}

Control::Control(string& indexType)
{
    index_handler = new IndexHandler(indexType);
    document_parser = new DocumentParser;
    document_parser->loadStopWords();
    query_processor = new QueryProcessor;
}

Control::~Control()
{
    delete index_handler;
    delete document_parser;
    delete query_processor;
}



    // OPERATION

void Control::MainMenu()
{
    cout << endl << "\t\t\tMAIN MENU" << endl << endl;
    bool running = true;
    int choice = 0;
    string buffer;
    do
    {
        cout << "Select one of the following options:" << endl;
        cout << "(1) Maintenance" << endl;
        cout << "(2) Interactive" << endl;
        cout << "(3) Stress Test" << endl;
        cout << "(4) Exit Program" << endl << endl;

        getline(cin, buffer);
        try
        {
            choice = stoi(buffer);
        }
        catch (...)
        {
            cout << endl << "Invalid entry. Please try again." << endl << endl;
            cin.sync();
            continue;
        }
        if (choice < 1 || choice > 4)
        {
            cout << endl << "Invalid entry. Please try again." << endl << endl;
            continue;
        }
        if (choice == 4)
            return;
        else if (choice == 1)
            running = Maintenance();
        else if (choice == 2)
            running = Interactive();
        else if (choice == 3)
            running = StressTest();


    } while (running);

    return;
}

bool Control::Maintenance()
{
    cout << "\tMaintenance Mode" << endl;
    int choice = 0;
    string cin_buffer;
    bool maintain = true;
    int f_num;
    // 1) Add a file to the index
    // 2) Clear the index
    // 3) Return to Main
    do
    {
        cout << endl << "Select an option below:" << endl;
        cout << "(1) Add new file to index" << endl;
        cout << "(2) Clear the index" << endl;
        cout << "(3) Return to Main Menu" << endl << endl;
        getline(cin, cin_buffer);
        try
        {
            choice = stoi(cin_buffer);
            cin.sync();
        }
        catch (...)
        {
            cout << endl << "Invalid entry. Please try again." << endl << endl;
            cin.clear();
            cin.sync();
            continue;
        }
        if (choice == 1)
        {
            // only let index file of format ../WikiDump/WikiDumpPart###.xml ?
            cout << endl << "Enter number for WikiDumpPart###.xml: ";
            getline(cin, cin_buffer);
            try
            {   // or check to see if valid file name, then convert to min value not in 'files_indexed'
                f_num = stoi(cin_buffer);
            }
            catch (...)
            {
                cout << endl << "Invalid entry." << endl << endl;
                cin.sync();
                return true;
            }
            try
            {
                indexFile(f_num);
            }
            catch (...)
            {
                continue;
            }

        }
        else if (choice == 2)
        {
            files_indexed.clear();
            index_handler->clearIndex();
            DocMeta.clear();
        }
        else if (choice == 3)
            break;
        else
            return false;
    } while (maintain);
    cout << endl << "\t\t\tMAIN MENU" << endl << endl;
    return true;
}
//void Control::addNewFile(string& fileName)
//{
//    // check to see largest file_number?
//}


bool Control::Interactive()
{
    cout << "\tInteractive Mode" << endl << endl;
    bool search_engine = true;
    int choice;
    string IDXtype;
    string buffer;
    // 1) Conduct search
    // 2) Select Index Type
    cout << "Please select an option below:" << endl;
    cout << "(1) Search with Boolean query" << endl;
    cout << "(2) Select Index Type" << endl;
    cout << "(3) Return to Main Menu" << endl << endl;

    getline(cin, buffer);
    try
    {
        choice = stoi(buffer);
    }
    catch (...)
    {
        cout << endl << "Invalid entry." << endl << endl;
        cin.sync();
        return true;
    }
    if (choice == 1)
    {
        while (search_engine)
            search_engine = tempSearch();
    }
    else if (choice == 2)
    {
        cout << endl << "Choose Index Type: " << endl;
        cout << "(1) Hash Table Index" << endl;
        cout << "(2) B Tree Index" << endl << endl;

        getline(cin, buffer);
        try
        {
            choice = stoi(buffer);
        }
        catch (...)
        {
            cout << endl << "Invalid entry." << endl << endl;
            cin.sync();
            return true;
        }
        writeIndexToCSV();

        delete index_handler;
        if (choice == 1)
        {
            IDXtype = "hash";
            index_handler = new IndexHandler(IDXtype);
            loadPersistentIndex();
        }
        else if (choice == 2)
        {
            IDXtype = "btree";
            index_handler = new IndexHandler(IDXtype);
            loadPersistentIndex();
        }
        else
        {
            cout << "Bad entry. Hash Index loaded." << endl;
            index_handler = new IndexHandler;
            loadPersistentIndex();
        }
    }


    cout << endl << "\t\t\tMAIN MENU" << endl << endl;
    return true;
}

bool Control::StressTest()
{
    int lineCounter = 1;
    string buffer;
    ifstream f_in;
    ofstream fout;
    int choice = 0;
    cout << "\tStress Test Mode" << endl << endl;
    // Load file of commands and run

    cout << "Please select an option:" << endl;
    cout << "(1) Load commands from file" << endl;
    cout << "(2) Return to Main Menu" << endl << endl;

    getline(cin, buffer);
    try
    {
        choice = stoi(buffer);
    }
    catch (...)
    {
        cout << endl << "Invalid entry." << endl << endl;
        cin.sync();
        return true;
    }

    if (choice == 2)
        return true;
    else if (choice == 1)
    {
        cout << endl << "Enter path for command file: ";
        getline(cin, buffer);
        cin.sync();
        f_in.open(buffer);
        fout.open("StressTestOutput.txt");
        if (!f_in.is_open() || !fout.is_open())
        {
            cout << endl << "A filepath failed to open." << endl;
            return true;
        }
        cin.sync();

        std::chrono::time_point<std::chrono::system_clock> start, end;
        start = std::chrono::system_clock::now();

        while (!f_in.eof())
        {
            getline(f_in, buffer);
            STmethod(buffer, fout, lineCounter);
            lineCounter++;
        }

        end = std::chrono::system_clock::now();
        unsigned int milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
        double seconds = milliseconds/1000.0;
        fout << endl << "Completion Time: " << seconds << " seconds" << endl;

        f_in.close();
        fout.close();
    }

    cout << endl << "\t\t\tMAIN MENU" << endl << endl;
    return true;
}


void Control::STmethod(string& command, ofstream& file_out, int line_count)
{                   // carries out one line from command file
    string full;
    string buffer;
    stringstream ss;
    DocumentInfo meta_info;
    int f_num;
    ss << command;
    ss >> buffer;
    if (buffer == "BOOLQ")
    {
        ss >> buffer;
        full += buffer;
        while (ss >> buffer)
        {
            full += " " + buffer;
        }
        query_processor->clear();
        try
        {
            query_processor->prepareSearch(full);
        }
        catch (...)
        {
            cout << endl << "Invalid query. Valid searches are of the form:" << endl << endl
                 << "(AND/OR) word1 word2 ... (NOT) word1 word2 ..." << endl << endl;
            throw "bad search";
        }
        retrieveResults();
        query_processor->combineResults();
        query_processor->finalResult.sort();
        query_processor->finalResult.getDocID(buffer, 1);
        file_out << line_count << ". Top Search Result: ";
        meta_info = DocMeta[buffer];
        file_out  << "\t\"" << meta_info.title << "\" : " << buffer << endl;
        return;
    }
    else if (buffer == "ADD_F")
    {
        ss >> buffer;
        try
        {   // or check to see if valid file name, then convert to min value not in 'files_indexed'
            f_num = stoi(buffer);
        }
        catch (...)
        {
            file_out << line_count << ". Invalid entry." << endl;
            cin.sync();
            return;
        }
        try
        {
            indexFile(f_num);
        }
        catch (...)
        {
            file_out << line_count << ".\t\tFile already indexed or file doesn't exist" << endl;
            return;
        }
        file_out << line_count << ".\t\tFile successfully indexed" << endl;
    }
    else if (buffer == "HASHX")
    {
        writeIndexToCSV();
        delete index_handler;
        buffer = "hash";
        index_handler = new IndexHandler(buffer);
        loadPersistentIndex();
        file_out << line_count << ".\t\tHash Table Idx successfully loaded" << endl;
    }
    else if (buffer == "BTIDX")
    {
        writeIndexToCSV();
        delete index_handler;
        buffer = "btree";
        index_handler = new IndexHandler(buffer);
        loadPersistentIndex();
        file_out << line_count << ".\t\tB Tree Idx successfully loaded" << endl;
    }
    else if (buffer == "LOADP")
    {
        index_handler->clearIndex();
        files_indexed.clear();
        DocMeta.clear();
        loadPersistentIndex();
        file_out << line_count << ".\t\tIndex reset to Persistent Index" << endl;
    }
    else if (buffer == "CLEAR")
    {
        index_handler->clearIndex();
        files_indexed.clear();
        DocMeta.clear();
        file_out << line_count << ".\t\tIndex successfully cleared" << endl;
    }
    else if (buffer == "WRITE")
    {
        writeIndexToCSV();
        file_out << line_count << ".\t\tIndex successfully stored" << endl;
    }

    else
        throw "Invalid form";

}




    // DOCUMENT PARSING & INDEXING

void Control::indexWordData()
{
    int numWords = document_parser->docWordData.size();
    string some_word;
    string docID;
    string title = document_parser->title_string;
    string username = document_parser->username_string;
    string date = document_parser->date_string;
    document_parser->generateDocID(docID);
    if (current_file < 10)
        docID = "00" + to_string(current_file) + docID;
    else if (current_file < 100)
        docID = "0" + to_string(current_file) + docID;
    else
        docID = to_string(current_file) + docID;
    storeDocMeta(docID, title, username, date);
    int some_count;
    int max_count = 0;
    double termFreq;
    for (int i = 0; i < numWords; i++)
    {
        some_count = document_parser->docWordData[i].theCount;
        if (some_count > max_count)
            max_count = some_count; // get the maximum number of times a word appears
    }
    for (int i = 0; i < numWords; i++)
    {
        some_word = document_parser->docWordData[i].theWord;
        some_count = document_parser->docWordData[i].theCount;
        termFreq = .5 + .5*((double)some_count/(double)(max_count)); // cast to doubles
        index_handler->addToIndex(some_word, docID, termFreq);
    }
}


void Control::loadXMLtoParse(string& file_name)
{
    cout << "Loading file: " << file_name << endl;
    try
    {
        document_parser->loadXMLfile(file_name); // pageElement automatically points to first doc
    }
    catch (...)
    {
        throw "No file";
    }
}


void Control::indexFile(int file_number)
{
    int numFiles = files_indexed.size();
    for (int c = 0; c < numFiles; c++)
    {
        if (file_number == files_indexed[c])
        {
            cout << endl << "File has already been indexed." << endl;
            return;
        }
    }
    current_file = file_number;
    string file_string = "../WikiDump/WikiDumpPart";
    if (file_number < 10)
        file_string += "00" + to_string(file_number) + ".xml";
    else if (file_number < 100)
        file_string += "0" + to_string(file_number) + ".xml";
    else
        file_string += to_string(file_number) + ".xml";

    try
    {
        loadXMLtoParse(file_string);
    }
    catch (...)
    {
        throw "No file.";
    }

    do
    {
        document_parser->clear();
        try
        {
            document_parser->getDocElements();
        }
        catch (...)
        {
            continue;
        }
        document_parser->getTokens();
        try
        {
            document_parser->consolidateTokens();
        }
        catch (...)
        {
            continue;
        }
        indexWordData();
    } while (document_parser->nextDocument());
    files_indexed.push_back(current_file);
    return;
}


void Control::storeDocMeta(string& some_docID, string& some_title, string& some_user, string& some_date)
{
    DocumentInfo newDocInfo(some_title, some_user, some_date);
    DocMeta[some_docID] = newDocInfo;
}






    // QUERY PROCESSING

bool Control::tempSearch() // maybe make function for individual search; separate function for while loop
{
    string some_query;
    string buffer;
    int numFinalResults;
    int selection;
    int choice = 1;
    query_processor->clear();
    cout << endl << "Please enter a search: "; // or enter _____ to return to main menu?
    getline(cin, some_query);
    try
    {
        query_processor->prepareSearch(some_query);
    }
    catch (...)
    {
        cout << endl << "Invalid query. Valid searches are of the form:" << endl << endl
             << "(AND/OR) word1 word2 ... (NOT) word1 word2 ..." << endl << endl;
        return false;
    }
    retrieveResults();
    query_processor->combineResults();
    query_processor->finalResult.sort();
    numFinalResults = query_processor->finalResult.getNumDocs();
    if (numFinalResults == 0)
    {
        cout << endl << endl << "No results found." << endl << endl;

        cout << "What would you like to do?" << endl;
        cout << "(1) Enter another query" << endl;
        cout << "(2) Return to Main Menu" << endl << endl;
        getline(cin, buffer);
        try
        {
            choice = stoi(buffer);
        }
        catch (...)
        {
            cout << endl << "Invalid entry." << endl << endl;
            cin.sync();
            return false;
        }
        if (choice == 1)
            return true;
        return false;
    }

    while (choice == 1)
    {
        for (int j = 1; j <= numFinalResults && j <= 15; j++)
            printSearchResult(j);

        cout << endl << endl;
        cout << "What would you like to do?" << endl;
        cout << "(1) Select a page from above" << endl;
        cout << "(2) Enter another query" << endl;
        cout << "(3) Return to Main Menu" << endl << endl;
        getline(cin, buffer);
        try
        {
            choice = stoi(buffer);
        }
        catch (...)
        {
            cout << endl << "Invalid entry." << endl << endl;
            cin.sync();
            return false;
        }

        if (choice == 1)
        {
            cout << endl << "Select document to view: ";
            cin >> selection;
            cin.ignore();
            if (selection < 1 || selection > 15 || selection > numFinalResults)
            {
                cout << "Invalid selection range." << endl;
                cin.clear();
                cin.sync();
                return true;
            }
            else
            {
                displaySelection(selection);
                cout << endl << "What would you like to do?" << endl;
                cout << "(1) View search results again" << endl;
                cout << "(2) Enter another query" << endl;
                cout << "(3) Return to Main Menu" << endl << endl;
                getline(cin, buffer);
                try
                {
                    choice = stoi(buffer);
                }
                catch (...)
                {
                    cout << endl << "Invalid entry." << endl << endl;
                    cin.sync();
                    return false;
                }
                if (choice == 1)
                {
                    continue;
                }
                else if (choice == 2)
                {
                    return true;
                }
//                else if (choice == 3)
//                {
//                    return false;
//                }
                else
                    return false;
            }
        }
        else if (choice == 2)
            return true;
        else
            return false;
    }
    return true;
}

void Control::lookup(string& searchWord)
{
    carrier.clearVessel();
    index_handler->getResults(searchWord, carrier);

    int num_documents = carrier.getNumDocs();
    if (num_documents == 0)
        return;

    int total_docs = DocMeta.size();
    double idf = (double)total_docs/(double)num_documents;
    idf = log(idf); // take the natural logarithm for scaling purposes
    carrier.calc_TF_IDF(idf);
}


void Control::retrieveResults()
{
    int num_query_words = query_processor->queryWords.size();
    int start = 0;
    if (query_processor->query_organizer.isAND || query_processor->query_organizer.isOR)
        start = 1;
    if (query_processor->query_organizer.isNOT)
    {
        for (int y = start; y < query_processor->query_organizer.NOTposition; y++)
            search_and_store(y);
        for (int z = query_processor->query_organizer.NOTposition + 1; z < num_query_words; z++)
            search_store_NOT(z);
    }
    else
    {
        for (int y = start; y < num_query_words; y++)
            search_and_store(y);
    }
    return;
}

void Control::search_and_store(int i)
{
    string word = query_processor->queryWords[i];
    lookup(word);
    query_processor->storeSearchResult(carrier);
}

void Control::search_store_NOT(int i)
{
    string word = query_processor->queryWords[i];
    lookup(word);
    query_processor->storeNOTresult(carrier);
}



void Control::printSearchResult(int rank)
{
    string find_docID;
    DocumentInfo meta_info;
    query_processor->finalResult.getDocID(find_docID, rank - 1);
    cout << endl << rank << ".   ";
    meta_info = DocMeta[find_docID];
    cout  << "\t\"" << meta_info.title << "\"" << endl << "\t\t";
    cout << "User: " << meta_info.username << " ; " << meta_info.date_submitted
         << " ; #" << find_docID << endl;

}

void Control::displaySelection(int selectNum)
{
    string find_docID;
    string load_file;
    int doc_number;
    query_processor->finalResult.getDocID(find_docID, selectNum - 1);
    load_file = "../WikiDump/WikiDumpPart" + find_docID.substr(0,3) + ".xml";
    doc_number = stoi(find_docID.substr(3)); // (3,3)
    loadXMLtoParse(load_file);
    for (int d = 0; d < doc_number; d++)
        document_parser->nextDocument();
    try
    {
        document_parser->getDocElements();
    }
    catch (...)
    {
        cout << "Couldn't get doc elements for viewing text." << endl;
        return;
    }
    cout << "TEXT:" << endl;
    cout << document_parser->text_string << endl;
}




    // OTHER

// from jtshaw on http://www.linuxquestions.org/questions/programming-9/c-list-files-in-directory-379323/
void Control::getDirectory(string directory, vector<string>& files)
{
    DIR *DIR_ptr;
    struct dirent *dirent_ptr;
    DIR_ptr = opendir(directory.c_str());
    if (DIR_ptr == NULL)
    {
        cout << "Failed to open directory: " << directory << endl << endl;
        return;
    }
    dirent_ptr = readdir(DIR_ptr);
    while (dirent_ptr != NULL)
    {
        files.push_back(string(dirent_ptr->d_name));
        dirent_ptr = readdir(DIR_ptr);
    }
    files.erase(files.begin(), files.begin() + 3);

    closedir(DIR_ptr);
    return;
}
//    vector<string> files;
//    user_interface->getDirectory("../WikiDump", files);
//
// COULD CHECK NAMES IN DIRECTORY AGAINST VECTOR OF FILENAMES INDEXED INSTEAD
// BUT WOULD STILL NEED A NUMBER; VECTOR.SIZE() + 1 or MIN VALUE OF FILES_INDEXED
//              THEN RENAME TO WIKIDUMPSPART[WHATEVER_NUMBER].XML


void Control::writeIndexToCSV()
{
    index_handler->writeIndexToFile();
    ofstream fout("PersistentDocMeta.txt");
    for (auto& x: DocMeta)
    {
        fout << x.first << endl << x.second.title << endl
             << x.second.username << endl
             << x.second.date_submitted << endl;
    }
}

void Control::loadPersistentIndex()
{
    index_handler->clearIndex();
    files_indexed.clear();
    DocMeta.clear();
    string index_word;
    string documentID;
    double termFreq;
    string someTitle;
    string someUsername;
    string someDate;
    ifstream fin("PersistentIndex.txt");
    if (!fin.is_open())
    {
        cout << "Failed to open Persistent Index file.";
        exit(1);
    }
    char segment[256];
    while (!fin.eof())
    {
        fin.getline(segment, 256, ',');
        if (segment[0] == '<')
        {
            index_word = string(segment + 1); // should ignore '<'
            index_word.pop_back();
        }
        else
        {
            documentID = segment;
            fin.getline(segment, 256, ',');
            termFreq = atof(segment);
            index_handler->addToIndex(index_word, documentID, termFreq);
        }
    }
    fin.close();
    fin.open("PersistentDocMeta.txt");
    if (!fin.is_open())
    {
        cout << "Failed to open Persistent Doc Meta file.";
        exit(1);
    }
    int file_number;
    while (!fin.eof())
    {
        fin.getline(segment, 256, '\n');
        documentID = segment;
        fin.getline(segment, 256, '\n');
        someTitle = segment;
        fin.getline(segment, 256, '\n');
        someUsername = segment;
        fin.getline(segment, 256, '\n');
        someDate = segment;
        if (documentID!=""&&someTitle!=""&&someUsername!=""&&someDate!="")
        {
            storeDocMeta(documentID, someTitle, someUsername, someDate);
            file_number = stoi(documentID.substr(0, 3));
            if (find(files_indexed.begin(), files_indexed.end(), file_number) == files_indexed.end())
                files_indexed.push_back(file_number);
        }
    }

    // other info like document_parser->file_number (for parsing a new file)...
}

