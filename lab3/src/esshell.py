#!/usr/bin/python

import sys
from sys import stdin
#
# 1. PARSING INPUT FILE
#

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

#
# SHELL
#

def ask_user(query):
    print ">>", query
    return stdin.readline().strip()

rules = []
attributes = {}


def print_rule(rule,padding):
    full_str = "IF "
    antecedent_str_list = []
    for antecedent, values in rule['antecedents'].iteritems():
        str = antecedent + " = "
        str += ' | '.join(values)
        antecedent_str_list += [str]
    full_str += " & ".join(antecedent_str_list)

    full_str += " THEN " + rule['consequent'][0] + " = " + rule['consequent'][1]
    print " "*padding*4 + full_str

def decide(finite_goal_attribute):
    working_memory = {}

    def printp(string,padding):
        print " "*padding*4 + string

    def printpit(string,padding):
        printp(string * (80-padding),padding)

    def backward_chaining(attribute,padding):
        printpit("=", padding)
        printpit("=", padding)
        printp("Zakljucujem za: " + attribute, padding)
        printpit("=", padding)

        conflicts = [rule for rule in rules if rule['consequent'][0] == attribute]

        printp("KONFLIKTNI SKUP",padding)
        printpit("-", padding)
        for c in conflicts:
            print_rule(c,padding)
        printpit("-", padding)

        printp("RADNA MEMORIJA",padding)
        printpit("-", padding)
        for attribute, value in working_memory.iteritems():
            printp(attribute+ " := "+ value,padding)
        printpit("-", padding)

        for rule in conflicts:
            printp("Ispitujem pravilo:",padding)
            print_rule(rule,padding)
            dokazane_premise = 0

            for antecedent, values in rule['antecedents'].iteritems():
                printp("Provjeravam premisu: " + antecedent,padding)

                if antecedent in working_memory:
                    if working_memory[antecedent] in values:
                        #print "RAM ATRIBUT OK"
                        dokazane_premise += 1
                        continue
                    else:
                        #print "RAM ATRIBUT NOT OK"
                        break
                else:
                    #print "U RAM-u NEMA ATRIBUTA"

                    if [r1 for r1 in rules if r1['consequent'][0] == antecedent]:

                        #print "*" * 80
                        #print "IZVEDIMO ATRIBUT: " + antecedent

                        if backward_chaining(antecedent,padding+1):
                            #print "*" * 80
                            if working_memory[antecedent] in values:
                                printp("Premisa odgovara izvedenoj vrijednosti", padding)
                                dokazane_premise += 1
                            else:
                                printp("Premisa NE odgovara izvedenoj vrijednosti", padding)
                                break
                        else:
                            #print "*" * 80
                            #print "CILJ JE NEIZVEDIV " + antecedent
                            break
                    else:
                        #print "NEMA PRAVILA ZA IZVODENJE " + antecedent

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
                #print "sve premise su zadovoljene, dodajem konzekvens u radnu memoriju"
                working_memory[rule['consequent'][0]] = rule['consequent'][1]
                return True

        printp("Cilj "+ attribute+ " je nemoguce izvesti",padding)
        return False

    backward_chaining(finite_goal_attribute,0)

    if finite_goal_attribute in working_memory:
        print "ZAKLJUCAK: ",working_memory[finite_goal_attribute]

while True:
    while True:
        try:
            file_name = ask_user("Baza znanja?")
            if file_name in ["quit", "q"]:
                sys.exit(0)
            rules, attributes = parse_input(file_name)
            break
        except IOError:
            continue
    while True:
        finite_goal_attribute = ask_user("Sto zelimo saznati? (" + ",".join(attributes) + ")")
        if finite_goal_attribute in ["quit", "q"]: break
        if finite_goal_attribute not in attributes: continue
        decide(finite_goal_attribute)

