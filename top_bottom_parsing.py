
'''

S -> NP VP
NP -> DT NN | DT NN PP
VP -> VBZ NP | VBZ NP PP
PP -> IN NP
DT -> 'the'
NN -> 'cat' | 'mat' | 'mouse' | 'table'
VBZ -> 'chases'
IN -> 'on' | 'under'

the cat on the mat chases the mouse under the table

'''

import nltk
from nltk import CFG
import sys
from nltk.parse import RecursiveDescentParser, ChartParser
from collections import defaultdict

sys.setrecursionlimit(10000)

print("Top-Down and Bottom-Up Parsing\n")

print("Enter CFG grammar:")
rules = []
while True:
    line = input()
    if line.strip() == "":
        break
    rules.append(line)
grammar_text = "\n".join(rules)

try:
    grammar = CFG.fromstring(grammar_text)
except Exception as e:
    print("\nInvalid grammar format:", e)
    sys.exit()

sentence = input("Enter the sentence or corpus: ").strip().lower().split()

print("\nParsing Results")
print("Sentence:", " ".join(sentence))
print("Tokens:", sentence)

def get_subtrees_by_level(tree, level=0, result=None):
    if result is None:
        result = defaultdict(list)
    result[level].append(tree)
    for child in tree:
        if isinstance(child, nltk.Tree):
            get_subtrees_by_level(child, level + 1, result)
    return result


print("\nTop-Down Parsing")
rd_parser = RecursiveDescentParser(grammar)
found = False
for tree in rd_parser.parse(sentence):
    found = True
    print("\nTop-Down Parse Tree:")
    print(tree)
    tree.pretty_print()
    tree.draw()

   
    print("\nSubtrees by Levels :")
    level_subtrees = get_subtrees_by_level(tree)
    for level in sorted(level_subtrees.keys()):
        print(f"\nLevel {level}:")
        for subtree in level_subtrees[level]:
            print(subtree.pformat(margin=100))  
            subtree.pretty_print()
            subtree.draw()

if not found:
    print("No parse tree found")

print("\nBottom-Up Parsing")
chart_parser = ChartParser(grammar)
found = False
for tree in chart_parser.parse(sentence):
    found = True
    print("\nBottom-Up Parse Tree:")
    print(tree)
    tree.pretty_print()
    tree.draw()
if not found:
    print("No parse tree found")
