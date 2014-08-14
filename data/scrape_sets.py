#!/usr/bin/python

import codecs
import re
import requests

from bs4 import BeautifulSoup

INPUT_FILE = 'set_input.txt'
OUTPUT_SETS = 'cardset.txt'
OUTPUT_CARDS = 'cardunique.txt'

CARD_SET_PATH = 'http://yugioh.wikia.com/wiki/Set_Card_Lists:'

def write_set(setfile, name, s, table_style):
    # the table styles are :
    # style 0: <set number> <name> <rarity>
    # style 1: <name> <jp name> <rarity>
    # style 2: <set number> <name> <jp name> <rarity>
    print name
    r = requests.get(s)
    data = r.text
    soup = BeautifulSoup(data)
    contents = soup.find_all("div",{"id":"mw-content-text"})

    setfile.write(name + '\n')
    for tag in contents:
        trTags = tag.find_all("tr")
        for trTag in trTags:
            tdTags = trTag.find_all("td")
            index = 0
            for tdTag in tdTags:
                if table_style == '0':
                    if index == 0:
                        setfile.write(tdTag.text.strip() + '\t')
                    elif index == 1:
                        setfile.write(tdTag.text.strip() + '\t')
                    elif index == 2:
                        setfile.write(tdTag.text.strip() + '\n')
                    else:
                        break
                elif table_style == '1':
                    if index == 0:
                        setfile.write('\t')
                        setfile.write(tdTag.text.strip() + '\t')
                    elif index == 1:
                        index = index + 1
                        continue
                    elif index == 2:
                        setfile.write(tdTag.text.strip() + '\n')
                    else:
                        break
                elif table_style == '2':
                    if index == 0:
                        setfile.write(tdTag.text.strip() + '\t')
                    elif index == 1:
                        setfile.write(tdTag.text.strip() + '\t')
                    elif index == 2:
                        index = index + 1
                        continue
                    elif index == 3:
                        setfile.write(tdTag.text.strip() + '\n')
                    else:
                        break
                index = index + 1

def generate_unique_cards():
    setfile = codecs.open(OUTPUT_SETS,'r','utf-8')
    names = []
    for line in setfile:
        if line.startswith('#'):
            continue
        items = re.split(r'\t+', line.rstrip())
        if len(items) == 1:
            # this is a set header, ignore
            continue
        # extract the name
        names.append(items[1])
    # remove duplicates
    names = list(set(names))
    # sort alphabetically
    names.sort()
    # write to an output file
    cardfile = codecs.open(OUTPUT_CARDS,'w','utf-8')
    for n in names:
        cardfile.write(n + '\n')


def read_scrape_input():
    with open(INPUT_FILE) as f:
        setfile = codecs.open(OUTPUT_SETS,'w','utf-8')
        for line in f:
            if line.startswith('#'):
                continue
            # reconstruct the input
            items = re.split(r'\t+', line.rstrip())
            name = items[0]
            ext = items[1]
            table_style = items[2]
            uri = CARD_SET_PATH + name.replace(' ','_')
            if ext != 'XXXX':
                uri = uri + '_' + ext.strip()
            write_set(setfile,name,uri,table_style)

read_scrape_input()
generate_unique_cards()
