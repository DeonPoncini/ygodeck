#!/usr/bin/python

import requests
import codecs
import re

from urllib2 import quote
from bs4 import BeautifulSoup

INPUT_FILE = 'cardunique.txt'
ERROR_FILE = 'carderrors.txt'
OUTPUT_FILE = 'card.txt'

# yugioh wiki paths
LOAD_PATH = 'http://yugioh.wikia.com/wiki/'

cardin = codecs.open(INPUT_FILE,'r','utf-8')
cardout = codecs.open(OUTPUT_FILE,'w','utf-8')
errorfile = codecs.open(ERROR_FILE,'w','utf-8')

def log_error(name):
    errorfile.write(name + '\n')

def print_output(data):
    cardout.write(data['name'] + '\t' + \
          data['cardType'] + '\t' + \
          data['attribute'] + '\t' + \
          data['monsterType'] + '\t' + \
          data['monsterAbility'] + '\t' + \
          data['type'] + '\t' + \
          str(data['level']) + '\t' + \
          str(data['attack']) + '\t' + \
          str(data['defense']) + '\t' + \
          str(data['lpendulum']) + '\t' + \
          str(data['rpendulum']) + '\t' + \
          data['spellType'] + '\t' + \
          data['trapType'] + '\n')

def parse_monster(data):
    output = {}
    output['name'] = data['name']
    output['cardType'] = 'Monster'
    output['attribute'] = data['attribute']
    output['monsterType'] = data['monsterType'].split()[0]
    output['type'] = data['type']
    output['level'] = data['level']
    output['attack'] = data['attack']
    output['defense'] = data['defense']
    if 'lpendulum' in data:
        output['lpendulum'] = data['lpendulum']
    else:
        output['lpendulum'] = '0'
    if 'rpendulum' in data:
        output['rpendulum'] = data['rpendulum']
    else:
        output['rpendulum'] = '0'
    output['spellType'] = 'None'
    output['trapType'] = 'None'

    if not 'monsterAbility' in data:
        output['monsterAbility'] = 'None'
    else:
        output['monsterAbility'] = data['monsterAbility']

    return output

def parse_spell(data):
    # fill out the output data structure
    output = {}
    output['name'] = data['name']
    output['cardType'] = 'Spell'
    output['attribute'] = 'None'
    output['monsterType'] = 'None'
    output['monsterAbility'] = 'None'
    output['type'] = 'None'
    output['level'] = '0'
    output['attack'] = '0'
    output['defense'] = '0'
    output['lpendulum'] = '0'
    output['rpendulum'] = '0'
    output['spellType'] = data['type0']
    output['trapType'] = 'None'
    return output

def parse_trap(data):
    # fill out the output data structure
    output = {}
    output['name'] = data['name']
    output['cardType'] = 'Trap'
    output['attribute'] = 'None'
    output['monsterType'] = 'None'
    output['monsterAbility'] = 'None'
    output['type'] = 'None'
    output['level'] = '0'
    output['attack'] = '0'
    output['defense'] = '0'
    output['lpendulum'] = '0'
    output['rpendulum'] = '0'
    output['spellType'] = 'None'
    output['trapType'] = data['type0']
    return output

def parse_card(cardname):
    origcardname = cardname;
    cardname = cardname.replace(' ','_').replace('#','').strip()
    uri = LOAD_PATH + quote(cardname.encode('utf-8'))
    print uri

    r = requests.get(uri)
    html = r.text
    soup = BeautifulSoup(html)
    contents = soup.find_all("table",{"class":"cardtable"})

    data = {}
    data['name'] = origcardname

    if len(contents) == 0:
        # do a retry with _(card) disambiguation suffix
        uri = uri + '_(card)'
        r = requests.get(uri)
        html = r.text
        soup = BeautifulSoup(html)
        contents = soup.find_all("table",{"class":"cardtable"})

    if len(contents) == 0:
        log_error(origcardname)
        return # we have truly failed

    for tag in contents:
        trTags = tag.find_all("tr")
        for trTag in trTags:
            thTags = trTag.find_all("th")
            tdTags = trTag.find_all("td")
            headers = []
            info = []
            for thTag in thTags:
                headers.append(thTag.text.strip())
            for tdTag in tdTags:
                info.append(tdTag.text.strip())
            if len(headers) == 1 and len(info) == 1:
                if headers[0].strip() == 'Attribute':
                    data['cardType'] = 'Monster'
                    data['attribute'] = info[0].strip()
                elif headers[0].strip() == 'Type':
                    if info[0].strip() == 'Spell Card':
                        data['cardType'] = 'Spell'
                    elif info[0].strip() == 'Trap Card':
                        data['cardType'] = 'Trap'
                    else:
                        data['type'] = info[0].strip()
                        data['monsterType'] = 'Normal'
                elif headers[0].strip() == 'Types':
                    types = re.split(r'/', info[0].strip())
                    data['type'] = types[0]
                    data['monsterType'] = types[1]
                    if len(types) == 3:
                        data['monsterAbility'] = types[2]
                elif headers[0].strip() == 'Level':
                    data['level'] = int(float(info[0]))
                elif headers[0].strip() == 'Rank':
                    data['level'] = int(float(info[0]))
                elif headers[0].strip() == 'Property':
                    data['type0'] = info[0].strip()
                elif headers[0].strip() == 'ATK/DEF':
                    points = re.split(r'/', info[0].strip())
                    if points[0].isnumeric():
                        data['attack'] = int(float(points[0]))
                    else:
                        data['attack'] = points[0]
                    if points[1].isnumeric():
                        data['defense'] = int(float(points[1]))
                    else:
                        data['defense'] = points[1]
                elif headers[0].strip() == 'Pendulum Scale':
                    data['lpendulum'] = info[0].strip()
                    data['rpendulum'] = info[0].strip()

    # process the output
    if 'cardType' in data:
        if data['cardType'] == 'Monster':
            print_output(parse_monster(data))
        elif data['cardType'] == 'Spell':
            print_output(parse_spell(data))
        elif data['cardType'] == 'Trap':
            print_output(parse_trap(data))
        else:
            log_error(origcardname)
    else:
        log_error(origcardname)


def parse_cards():
    for line in cardin:
        if line.startswith('#'):
            continue
        line = line.strip()
        parse_card(line)

parse_cards()
