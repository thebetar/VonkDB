#include <iostream>
#include <fstream>
#include "database.h"
#include "commands.h"

int main()
{
    Database db;

    db.init("data");

    while (true)
    {
        cout << "Enter a command (type 'help' for a list of commands): ";
        string command;
        cin >> command;

        if (command == "get")
        {
            command_get(db);
        }
        else if (command == "get_by_id")
        {
            command_get_by_id(db);
        }
        else if (command == "insert")
        {
            command_insert(db);
        }
        else if (command == "update")
        {
            command_update(db);
        }
        else if (command == "remove")
        {
            command_remove(db);
        }
        else if (command == "create_table")
        {
            command_create_table(db);
        }
        else if (command == "drop_table")
        {
            command_drop_table(db);
        }
        else if (command == "get_table_names")
        {
            command_get_table_names(db);
        }
        else if (command == "get_table_columns")
        {
            command_get_table_columns(db);
        }
        else if (command == "clear")
        {
            command_clear(db);
        }
        else if (command == "help")
        {
            command_help();
        }
        else if (command == "exit")
        {
            break;
        }
        else
        {
            cout << "Invalid command\n";
        }
    }

    return 0;
}