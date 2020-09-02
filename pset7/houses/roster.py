# TODO
import cs50
from sys import argv, exit

# check command-line arguments
if len(argv) != 2:
    print("Usage: python roster.py house")
    exit(1)

# Open database
db = cs50.SQL("sqlite:///students.db")

# Query database for all students in house sorted alphabetically by last name, then first name
rows = db.execute("SELECT * from  students WHERE house = ? ORDER BY last, first", argv[1])

# Print out each student's full name and birth year
for row in rows:
    print(row["first"], end=" ")

    if row["middle"] != None:
        print(row["middle"], end=" ")

    print(row["last"] + ", born " + str(row["birth"]))