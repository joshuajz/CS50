# Imports
import csv
import cs50
from sys import argv, exit

# How many commandline arguments
argc = len(argv)

# Checks the amount of commandline arguments
if argc != 2:
    print("Invalid Commandline Arguments Usage: ./import {name of csv}")
    exit(0)

# Opens the database
database = cs50.SQL("sqlite:///students.db")

# Opens the .csv file
with open(argv[1], "r") as file:
    # Creates a reader
    reader = csv.DictReader(file)

    # Iterates over each row
    for row in reader:
        # Splits the name part of the input
        fullname = row['name'].split(" ")
        if len(fullname) == 2:
            # First + Last Name
            fname = fullname[0]
            lname = fullname[1]
            database.execute("INSERT INTO students (first, last, house, birth) VALUES (?, ?, ?, ?)",
                             fname, lname, row['house'], row['birth'])

        elif len(fullname) == 3:
            # First, Middle + Last Name
            fname = fullname[0]
            mname = fullname[1]
            lname = fullname[2]
            database.execute("INSERT INTO students (first, middle, last, house, birth) VALUES (?, ?, ?, ?, ?)",
                             fname, mname, lname, row['house'], row['birth'])
