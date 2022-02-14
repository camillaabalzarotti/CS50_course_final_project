import csv
import sys


def main():

    # TODO: Check for command-line usage first one cvs file, second file where dna
    if len(sys.argv) != 3:
        sys.exit("Error: expected filename filename")
# TODO: Check database for matching profiles
    tot = len(counts)
    
    person = {
    }
    people = []
    x = []
    # TODO: Read database file into a variable
    with open(sys.argv[1], "r") as database:
        reader = csv.DictReader(database)  #to read the file as a csv file
        for person in reader:
            people.append (person)
    x = person.keys()
    print(x[0])
main()