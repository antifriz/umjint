#!/usr/bin/python

import fileinput

atributi = {}
pravila = []
pravilo = [{}, {}]
state = 0

for line in fileinput.input():
    if len(line) <= 1:
        state = 1
        continue
    if state == 0:
        atribut, vrijednosti = [x.strip() for x in line.split('=')]

        atributi[atribut] = [x.strip() for x in vrijednosti.split(',')]
    elif state == 1:
        line = line[3:]
        amps = [x.strip() for x in line.split('&')]

        for amp in amps:
            atribut, vrijednosti = [x.strip() for x in amp.split('=')]

            pravilo[0][atribut] = [x.strip() for x in vrijednosti.split('|')]

        state = 2
    elif state == 2:
        line = line[5:]
        amps = [x.strip() for x in line.split('&')]

        for amp in amps:
            atribut, vrijednosti = [x.strip() for x in amp.split('=')]

            pravilo[1][atribut] = [x.strip() for x in vrijednosti.split('|')]
        state = 1
        pravila += pravilo
        pravilo = [{}, {}]


print atributi
print pravila
