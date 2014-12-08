// Main Source File


// REMEMBER TO COMPILE USING -std=c++11


#include "Control.h"



int main(int argc, char** argv) // argc = arg count, argv = arg vector
{
    Control* user_interface;
    if (argc == 2)
    {
        string index_type = argv[1];
        user_interface = new Control(index_type); // designates hash or b+ tree from command line tag
    }
    else
        user_interface = new Control;

    user_interface->loadPersistentIndex();
    user_interface->MainMenu();


    delete user_interface;
    return 0;
}

