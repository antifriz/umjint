#!/usr/bin/python

import sys
from sys import stdin
#
# 1. PARSING INPUT FILE
#

if len(sys.argv) != 2:
    print "No ES description file added"
    sys.exit(1)

import fileinput


def parse_input(file_name):
    attributes = {}
    rules = []
    rule = {'antecedent': {}, 'consequent': []}

    state = 0
    for line in fileinput.input(file_name):
        if len(line) <= 1:
            state = 1
            continue
        if state == 0:
            attribute, values = [x.strip() for x in line.split('=')]

            attributes[attribute] = [x.strip() for x in values.split(',')]
        elif state == 1:
            line = line[3:]
            amps = [x.strip() for x in line.split('&')]

            for amp in amps:
                attribute, values = [x.strip() for x in amp.split('=')]

                rule['antecedent'][attribute] = [x.strip() for x in values.split('|')]

            state = 2
        elif state == 2:
            line = line[5:]
            rule['consequent'] = [x.strip() for x in line.split('=')]

            state = 1
            rules += rule
            rule = {'antecedent': {}, 'consequent': []}
    return rules, attributes


rules, attributes = parse_input(sys.argv[1])

print attributes
print rules


#
# SHELL
#

def ask_user(query):
    print ">>", query
    return stdin.readline()


finite_goal_attribute = ask_user("Sto zelimo saznati?")

working_memory = []

stack = []

def get_rules_for_all_attribute_values(consequent_attribute):
    return [rule for rule in rules if rule['consequent'][0] == consequent_attribute].reverse()

def get_rules_for_certain_attribute_value(consequent):
    return [rule for rule in rules if rule['consequent']== consequent].reverse()

def add_rules_to_stack(consequent_attribute):
    stack.append(get_rules_for_all_attribute_values(consequent_attribute))

add_rules_to_stack(finite_goal_attribute)

while stack:
    current_rule = stack[-1]
    current_goal_attribute = current_rule['consequent'][0]
    current_goal_value = current_rule['consequent'][1]
    if all([any([value for value in values if [attribute, value] in working_memory]) for attribute,values in current_rule['antecedent'].iteritems()]) in working_memory:
        apply_rule()
        continue
    new_rules = get_rules_for_certain_attribute_value(current_rule['consequent'])
    if new_rules:
        continue

    ask_user()