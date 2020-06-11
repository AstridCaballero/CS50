# TODO
import csv
from sys import argv
from cs50 import SQL

# Check csv file has been entered by the user
if len(argv) != 2:
    print(f"usage:{argv[0]} infile")

# Create a db by opening and closing an empty file first
open(f"students.db", "w").close()
db = SQL("sqlite:///students.db")

# Create table called 'students' and specify the columns we want and their type
db.execute("CREATE TABLE students (first TEXT NOT NULL, middle TEXT, last TEXT NOT NULL, house TEXT NOT NULL, birth NUMERIC NOT NULL, PRIMARY KEY(first, middle, last, birth))")

# Open csv file
with open(argv[1], "r") as csv_file:

    # Create DictReader
    reader = csv.DictReader(csv_file, delimiter=',')

    # Iterate over csv file
    for row in reader:
        # parse name according to https://stackoverflow.com/questions/59809474/parsing-just-a-column-of-a-csv-file-with-multiple-columns
        list_names = row["name"].split()
        if len(list_names) == 2:
            middle = None
            first, last = list_names
        else:
            first, middle, last = list_names

        # Insert information into table 'students' from csv
        db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES(?, ?, ?, ?, ?)", first, middle, last, row["house"],
                   row["birth"])