import csv
import sys


def main():

    # TODO: Check for command-line usage first one cvs file, second file where dna
    if len(sys.argv) != 3:
        sys.exit("Error: expected filename filename")

    person = {
    }
    people = []
    # TODO: Read database file into a variable
    with open(sys.argv[1], "r") as database:
        reader = csv.DictReader(database)  # to read the file as a csv file
        for person in reader:
            people.append(person)

    # TODO: Read DNA sequence file into a variable
    with open(sys.argv[2], "r") as dnafile:
        content = dnafile.read()
    counts = []
    # TODO: Find longest match of each STR in DNA sequence
    for key in person:
        if (key != "name"):
            count = longest_match(content, key)
            counts.append(count)

    # for every person in people see if there is a match
    ris = False
    for person in people:
        sequence = []
        for key in person:
            if (key != "name"):
                con = int(person[key])
                sequence.append(con)
        ris = thereismatch(counts, sequence)
        if ris == True:
            print(person["name"])
            break
    if ris == False:
        print("No match")
    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


def thereismatch(s1, s2):
    i = 0
    tot = len(s1)
    for i in range(tot):
        if s1[i] != s2[i]:
            return False
    return True


main()

