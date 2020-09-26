# Imports
import cs50
from sys import exit, argv

# How many commandline Arugmnets
argc = len(argv)

# Checks
if argc != 2:
    print("Invalid Commandline Arguments Usage: ./roster {house}")
    exit(0)

# Opens database
database = cs50.SQL("sqlite:///students.db")

# Opens the amount of rows
rows = database.execute(
    "SELECT first, middle, last, house, birth FROM students WHERE house = :house ORDER BY last, first", house=argv[1])

# Iterates over the rows
for row in rows:
    if row["middle"] == None:
        # We have no middle Name
        print(f"{row['first']} {row['last']}, born {row['birth']}")
    else:
        # We have a middle name
        print(f"{row['first']} {row['middle']} {row['last']}, born {row['birth']}")
