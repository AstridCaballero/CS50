import csv
from sys import argv


def main():
    # Check if user entered the correct number of command-line arguments
    if len(argv) != 3:
        print(f"usage: {argv[0]} infile")

    # get the headers names
    with open(argv[1], "r") as csv_file_h:
        csv_header = csv.reader(csv_file_h, delimiter=',')
        for row in csv_header:
            # get header to create a data structure with the csv file
            header = list(row)
            # Get headers to read the .txt file
            STR_header = list(row)
            STR_header.pop(0)
            break

    # Read the csv file and store it in memory
    with open(argv[1], "r") as csv_file:
        csv_reader = csv.DictReader(csv_file, fieldnames=header, delimiter=',')
        new_dict = {}
        # skip first row (headers)
        next(csv_reader)
        for lines in csv_reader:
            # prepare to make 'name' a value (v) and the rest of fields/columns/STRs will be the key (the key will be a string)
            v = None
            STR_counts = ""
            # iterate over each key to rearrange the new_dict keys and values
            for key in lines.keys():
                if key == 'name':
                    v = lines[key]
                else:
                    # concatenate the columns != name
                    STR_counts += lines[key]
            # define key:value order for each dictionary. Here the code rearrange the k:v and store it in a new_dict
            new_dict[STR_counts] = v

    # read .txt file and count the STRs sequences
    with open(argv[2], "r") as sequence_file:
        sequence_reader = sequence_file.read()
        # prepare to count
        counter_fin = ""
        for n in range(len(STR_header)):
            i = 0
            counter = 0
            longest_counter = 0
            while i < len(sequence_reader):
                if STR_header[n] == sequence_reader[i:(len(STR_header[n]) + i)]:
                    counter += 1
                    i += len(STR_header[n])
                else:
                    if counter > longest_counter:
                        longest_counter = counter
                    counter = 0
                    i += 1
            # concatenate the final counts
            counter_fin += str(longest_counter)

    # iterate over new_dict and compare each key (string) from the csv file with counter_fin string
    count_key = 0
    for k in new_dict.keys():
        if k == counter_fin:
            # Print name if matched
            print(new_dict[k])
            break
        else:
            count_key += 1
    if count_key == len(new_dict.keys()):
        print("No match")


main()