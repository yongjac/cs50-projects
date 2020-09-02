from sys import argv, exit
import csv
import re

if len(argv) != 3:
    print("Usage: python dna.py data.csv sequence.txt")
    exit(1)

txt_file = open(argv[2], "r")
dna = txt_file.read()
txt_file.close()
name = None

with open(argv[1], mode = 'r') as csv_file:
    reader = csv.DictReader(csv_file)
    line_count = 0
    STRs = []
    STR_count = []
    for row in reader:
        if line_count == 0:
            for k, v in row.items():
                if k == 'name':
                    pass
                else:
                    STRs.append(k)
                    pattern = k
                    pattern_count = 0
                    while re.search(pattern, dna):
                        pattern += k
                        pattern_count += 1
                    STR_count.append(pattern_count)
                    #print(pattern_count)

        line_count += 1
        num_seq = 0
        for i in range(len(STRs)):
            if STR_count[i] == int(row[STRs[i]]):
                num_seq += 1
        if num_seq == len(STRs):
            name = row["name"]

if name == None:
    print("No match")
else:
    print(name)

