#!/usr/bin/python

import re
import sqlite3
from sqlite3 import OperationalError


def executeSql(filename):
    # Open and read the file as a single buffer
    fd = open(filename, 'r')
    sqlFile = fd.read()
    fd.close()

    # all SQL commands (split on ';')
    sqlCommands = sqlFile.split(';')

    # Execute every command from the input file
    for command in sqlCommands:
        # This will skip and report errors
        # For example, if the tables do not yet exist, this will skip over
        # the DROP TABLE commands
        try:
            c.execute(command)
        except OperationalError, msg:
            print "Command skipped: ", msg

db = sqlite3.connect('card.db')
db.text_factory = str
c = db.cursor()

# create all the databases
executeSql('schema.sql')

# load up all the card data
with open('card.txt') as f:
   all_cards = []
   for line in f:
       items = re.split(r'\t+', line.rstrip())
       items.insert(0,None)
       all_cards.append(items)

   c.executemany('INSERT INTO card VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?)', \
             all_cards)

with open('formats.txt') as f:
    all_formats = []
    for line in f:
        items = re.split(r'\t+', line.rstrip())
        items.insert(0,None)
        all_formats.append(items)

    c.executemany('INSERT INTO formats VALUES (?,?,?,?)', all_formats)

db.commit()

c.close()
db.close()

