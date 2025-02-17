#include <iostream>
#include <fstream>
#include <ncurses.h>
#include <string>
#include "database/database.h"
#include "commands.h"

int main()
{
    // Set random seed to time
    srand(time(NULL));

    // Initialize ncurses
    WINDOW *win = initscr();
    if (win == nullptr)
    {
        std::cerr << "Error initializing ncurses" << std::endl;
        return 1;
    }

    cbreak();             // Disable line buffering
    noecho();             // Don't echo input
    keypad(stdscr, TRUE); // Enable keypad input

    Database db;
    db.init("data");

    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    while (true)
    {
        clear();
        mvprintw(0, 0, "Enter a command (type 'help' for a list of commands): ");
        refresh();

        char input[256];
        echo(); // Enable echo for command input
        getstr(input);
        noecho(); // Disable echo after input

        string command(input);

        // Clear the screen for command output
        clear();

        if (command == "get")
        {
            command_get(db);
        }
        else if (command == "get_by_id")
        {
            command_get_by_id(db);
        }
        else if (command == "get_joined")
        {
            command_get_joined(db);
        }
        else if (command == "insert")
        {
            command_insert(db);
        }
        else if (command == "update")
        {
            command_update(db);
        }
        else if (command == "delete")
        {
            command_delete(db);
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
        else if (command == "get_table_column_names")
        {
            command_get_table_column_names(db);
        }
        else if (command == "clear_table")
        {
            command_clear_table(db);
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
            mvprintw(1, 0, "Invalid command");
        }

        // Clear last line for text
        move(max_y - 1, 0);
        clrtoeol();

        // Wait for user input before clearing screen
        mvprintw(max_y - 1, 0, "Press any key to continue...");
        refresh();
        getch();
    }

    // Clean up ncurses
    endwin();
    return 0;
}