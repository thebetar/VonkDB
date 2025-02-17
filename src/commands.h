#pragma once

#include "database/database.h"

void command_get(Database &db);
void command_get_by_id(Database &db);
void command_get_joined(Database &db);
void command_insert(Database &db);
void command_update(Database &db);
void command_delete(Database &db);
void command_create_table(Database &db);
void command_drop_table(Database &db);
void command_get_table_names(Database &db);
void command_get_table_column_names(Database &db);
void command_clear_table(Database &db);
void command_help();