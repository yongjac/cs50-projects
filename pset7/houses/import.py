# TODO
import cs50
import csv
from sys import argv, exit

# check command-line arguments
if len(argv) != 2:
    print("Usage: python import.py characters.csv")
    exit(1)

# Create database
open(f"students.db", "w").close()
db = cs50.SQL("sqlite:///students.db")

#Create table called 'students', and specify the columns we want,
db.execute("CREATE TABLE students (first TEXT, middle TEXT, last TEXT, house TEXT, birth NUMERIC)")

# Open CSV file
with open(argv[1], "r") as input:

    #Create DictReader
    reader = csv.DictReader(input)

    #Iterate over CSV file
    for row in reader:

        #split name
        name = row["name"].split()
        first = None
        middle = None
        last = None
        if (len(name) == 2):
            first = name[0]
            last = name[1]
        else:
            first = name[0]
            middle = name[1]
            last = name[2]

        # Insert student
        db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES(?, ?, ?, ?, ?)",
                first, middle, last, row["house"], row["birth"])