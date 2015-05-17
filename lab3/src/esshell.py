#!/usr/bin/python

import sys
from sys import stdin
#
# 1. PARSING INPUT FILE
#

if len(sys.argv) != 2:
    print "No ES description file added"
    sys.exit(1)

from collections import OrderedDict
import fileinput


def parse_input(file_name):
    attributes = OrderedDict()
    rules = []
    rule = {'antecedents': OrderedDict(), 'consequent': []}

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

                rule['antecedents'][attribute] = [x.strip() for x in values.split('|')]

            state = 2
        elif state == 2:
            line = line[5:]
            rule['consequent'] = [x.strip() for x in line.split('=')]

            state = 1
            rules += [rule]
            rule = {'antecedents': OrderedDict(), 'consequent': []}
    return rules, attributes


rules, attributes = parse_input(sys.argv[1])

print attributes
print rules


#
# SHELL
#

def ask_user(query):
    print ">>", query
    return stdin.readline().strip()


finite_goal_attribute = ""
while True:
    finite_goal_attribute = ask_user("Sto zelimo saznati? (" + ",".join(attributes) + ")")
    if finite_goal_attribute in attributes: break

working_memory = {}

# def every_rule_with_consequent_matching(attribute,value):
#     print rules
#     return [rule for rule in rules if rule['consequent'] == [attribute,value]]
#
# def backward_chaining(attribute,val):
#    if attribute in working_memory:
#        if working_memory[attribute] == val:
#            return True
#    if not every_rule_with_consequent_matching(attribute, val):
#         while True:
#             resp = ask_user("De unesi "+attribute + " ("+",".join(attributes[attribute])+ "):")
#             if resp in attributes[attribute]:
#                 working_memory[attribute] = resp
#                 break
#
#     for rule in every_rule_with_consequent_matching(attribute, val):
#         print rule
#         if all([any([antecedent in working_memory and  working_memory[antecedent] == v for v in vs]) for antecedent,vs in rule['antecedents'].iteritems()]):
#             return True
#     return False
#
#
#
# for value in attributes[finite_goal_attribute]:
#     if backward_chaining(finite_goal_attribute, value):
#         break
#
# print working_memory[finite_goal_attribute]

#
# for value in set([rule['consequent'][1] for rule in rules if rule['consequent'][0] == finite_goal_attribute]):
#
#
#
#


def all_premises_satisfied(rule):
    return all(
        [any([antecedent in working_memory and working_memory[antecedent] == v for v in vs]) for antecedent, vs in
         rule['antecedents'].iteritems()])


def premise_not_satisfied(rule):
    return any(
        [all([antecedent in working_memory and working_memory[antecedent] != v for v in vs]) for antecedent, vs in
         rule['antecedents'].iteritems()])


def premises_not_in_working_memory(rule):
    return [antecedent for antecedent in rule['antecedents'] if antecedent not in working_memory]


def print_rule(rule):
    full_str = "IF "
    antecedent_str_list = []
    for antecedent, values in rule['antecedents'].iteritems():
        str = antecedent + " = "
        str += ' | '.join(values)
        antecedent_str_list += [str]
    full_str += " & ".join(antecedent_str_list)

    full_str += " THEN " + rule['consequent'][0] + " = " + rule['consequent'][1]
    print full_str

# 1)oblikuj stog sastavljen od najvaznijih hipoteza
# todo
stack = [finite_goal_attribute]
# 2) na vrhu je ono sto trebamo dokazati, ako je stog prazan onda kraj
#
#
def backward_chaining(attribute):
    print "=" * 80
    print "=" * 80
    print "ZAKLJUCIVANJE ZA ATRIBUT " + attribute
    print "-" * 80

    conflicts = [rule for rule in rules if rule['consequent'][0] == attribute]

    print "KONFLIKTNI SKUP"
    print "-" * 80
    for c in conflicts:
        print_rule(c)
    print "-" * 80

    print "RADNA MEMORIJA"
    print "-" * 80
    for attribute, value in working_memory.iteritems():
        print attribute, " = ", value
    else:
        print "<PRAZNA>"
    print "-" * 80

    for rule in conflicts:
        print "ISPITUJEM PRAVILO"
        print_rule(rule)
        dokazane_premise = 0

        for antecedent, values in rule['antecedents'].iteritems():
            print "Provjeravam premisu: " + antecedent

            if antecedent in working_memory:
                print "Atribut se nalazi u radnoj memoriji"
                if working_memory[antecedent] in values:
                    print "Ispravna vrijednost se nalazi u radnoj memoriji"
                    dokazane_premise += 1
                    continue
                else:
                    print "Neispravna vrijednost se nalazi u radnoj memoriji"
                    break
            else:
                print "Atribut se ne nalazi u radnoj memoriji"

                postoji_pravilo = False
                rezultat = False
                for r1 in [r1 for r1 in rules if r1['consequent'][0] == antecedent]:
                    print "Postoji pravilo koje izvodi atribut " + attribute

                    rezultat = backward_chaining(antecedent)
                    postoji_pravilo = True
                    break

                if postoji_pravilo:
                    if rezultat:
                        if working_memory[antecedent] in values:
                            print "Izvedena vrijednost se poklapa s onim u premisi"
                            dokazane_premise += 1
                        else:
                            print "Izvedena vrijednost se NE poklapa s onim u premisi"
                            break
                    else:
                        print "Cilj je neodreden " + attribute
                        break

                else:
                    print "ne postoji pravilo koje izvodi atribut"

                    value = ''
                    while True:
                        value = ask_user("De unesi " + antecedent + " (" + ",".join(attributes[antecedent]) + "):")
                        if value in attributes[antecedent]:
                            working_memory[antecedent] = value
                            break

                    if value in values:
                        dokazane_premise += 1
                        continue
                    else:
                        break

        if dokazane_premise == len(rule['antecedents']):
            print "sve premise su zadovoljene, dodajem konzekvens u radnu memoriju"
            working_memory[rule['consequent'][0]] = rule['consequent'][1]
            return True

    print "Nijedno pravilo ne moze izvesti cilj " + attribute
    return False


backward_chaining(finite_goal_attribute)

#
#
# while True:
#     current_goal_attribute = stack.pop()
#
#     print "="*80
#     print current_goal_attribute
#
#     conflicts = [rule for rule in rules if rule['consequent'][0] == current_goal_attribute]
#     if not conflicts:
#         print "no new rules"
#         while True:
#             resp = ask_user("De unesi "+current_goal_attribute + " ("+",".join(attributes[current_goal_attribute])+ "):")
#             if resp in attributes[current_goal_attribute]:
#                 working_memory[current_goal_attribute] = resp
#                 break
#
#     print "found new rules"
#     for conflict in conflicts:
#         print conflict
#     print "-"*80
#
#     contin = False
#     for rule in conflicts:
#         print "-" * 80
#         print rule
#         print "-" * 80
#         if all_premises_satisfied(rule):
#             working_memory[current_goal_attribute] = rule['consequent'][1]
#             print "all premises satisfied"
#             contin = True
#             stack.pop()
#             break
#         if premise_not_satisfied(rule):
#             print "at least one premise not satisfied"
#             continue
#         print "there are premises not in working memory"
#
#         not_in_memory = premises_not_in_working_memory(rule)
#         if not not_in_memory:
#             raise Exception("This shouldn't happen")
#
#         not_in_memory.reverse()
#
#         print not_in_memory
#
#         stack.extend(not_in_memory)
#         contin = True
#         break
#
#     if contin: continue
#
#     print "unsatisfiable rule"
#     stack.pop()
#
print working_memory[finite_goal_attribute]
#
#

#   if current_goal_attribute in working_memory:
#         if working_memory[current_goal_attribute] ==
#
#     stack_size = len(stack)
#     all_flag = True
#     for attribute, values in current_rule['antecedent'].iteritems():
#         any_list = []
#         any_flag = False
#         for value in values:
#             if value not in working_memory:
#                 any_list += [attribute, value]
#             else:
#                 any_flag =True
#
#         if not any_flag:
#             rules = []
#             for any_item in any_list:
#                 rules = get_rules_for_certain_attribute_value(any_item)
#                 if not rules:
#                     continue
#
#             if not rules:
#                 ask_user_for(any_list[0][0])
#         """
#         mozemo i ostale dodat al ajmo za pocetak sam jedan
#         """
#         all_flag = all_flag and any_flag
#
#
#     if all_flag:
#         fire_rule()
#         continue
#
#
#
#
#     ask_user()
