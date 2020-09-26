import csv
from sys import argv, exit


def main():

    argc = len(argv)
    if argc != 3:
        print("Usage: python dna.py data.csv sequence.txt")
        exit(1)

    with open(argv[2], "r") as dna:
        dnainfo = dna.read()

    persondna = []

    with open(argv[1]) as database:
        read = csv.reader(database)
        for row in read:
            # Gain the sequence's dna type and put it in persondna
            if row[0] == "name":
                for value in row:
                    if value != "name":
                        persondna.append(dnaSequence(dnainfo, value))
            else:
                currentdna = []
                for i in range(1, len(row), 1):
                    currentdna.append(int(row[i]))
                # print(rowinfo)
                #print(f"persondna: {persondna}, currentdna: {currentdna}")
                if currentdna == persondna:
                    print(row[0])
                    exit(0)

    print("No match")


def dnaSequence(rawdna, sequence):
    # Returns the number that the sequence repeates from dnainfo

    amounts = []

    #print(f"rawdna: {rawdna}, sequence: {sequence}")

    i = 0

    counter = 0
    foundDNA = False
    while i < len(rawdna) - len(sequence):
        if rawdna[i:i+len(sequence)] == sequence:
            # We found some DNA
            if foundDNA:
                # If we've already found DNA before
                counter += 1
            else:
                foundDNA = True
                counter += 1
            i += len(sequence)
        else:
            # We didn't find any DNA
            if foundDNA:
                foundDNA = False
                amounts.append(counter)
                counter = 0
            i += 1
    if len(amounts) > 0:
        return max(amounts)
    else:
        return 0


main()