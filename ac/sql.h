

#define CREATE_PERSON_TABLE  "CREATE TABLE IF NOT EXISTS test_table("  \
                "id INTEGER PRIMARY KEY," \
                "name TEXT NOT NULL," \
                "age INTEGER NOT NULL);"

# define FORMAT_INSERT_STUDENT  "INSERT INTO test_table (name, age) VALUES ('%s', %d);"             