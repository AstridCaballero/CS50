# TODO
from sys import argv
from cs50 import SQL

# Check user entered correct number of command-line arguments
if len(argv) != 2:
    print(f"usage: {argv[0]} house_name")

house_name = argv[1]

# call db
db = SQL("sqlite:///students.db")

# execute selection
rows = db.execute("SELECT first, middle, last, birth FROM students WHERE house = ? ORDER BY last ASC, first ASC", house_name)
for col in rows:
    if col["middle"] == None:
        print(col["first"], col["last"]+",born", col["birth"])
    else:
        print(col["first"], col["middle"], col["last"]+", born", col["birth"])