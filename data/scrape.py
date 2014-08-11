#!/usr/bin/python

import sys
import logging
import rdflib
import codecs
import requests
from bs4 import BeautifulSoup

logging.basicConfig()

# yugioh wiki rdf paths
URI_PATH = 'http://yugioh.wikia.com/wiki/Special:URIResolver/'
PROPERTY_PATH = URI_PATH + 'Property-3A'
LOAD_PATH = 'http://yugioh.wikia.com/wiki/Special:ExportRDF/'

# attribute maps:
CARD_NAME = PROPERTY_PATH + 'English_name'
CARD_TYPE = PROPERTY_PATH + 'Card_category_Text'
ATTRIBUTE = PROPERTY_PATH + 'Attribute_Text'
MONSTER_TYPE = PROPERTY_PATH + 'Card_type_Text'
TYPE = PROPERTY_PATH + 'Types'
LEVEL = PROPERTY_PATH + 'Level'
ATK = PROPERTY_PATH + 'ATK'
DEF = PROPERTY_PATH + 'DEF'
PENDULUM = PROPERTY_PATH + 'Pendulum_Scale'
IMAGE = PROPERTY_PATH + 'Card_Image'
TEXT = PROPERTY_PATH + 'Lore'

SCRAPE_SETS = [
        # Booster packs
        # Series 1-3
        'http://yugioh.wikia.com/wiki/Set_Card_Lists:Legend_of_Blue_Eyes_White_Dragon_(TCG-EN)'
        ]

errors = []

def log_error(name):
    errors.append(name)

def report_errors():
    print "The following cards failed to parse: "
    for error in errors:
        print error

def print_output(data):
    print data['name'] + '\t' + \
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
          data['trapType']

def check_type(t, monsterType):
    # we don't know what type is present
    if t == monsterType:
        return ('monsterType', monsterType)
    elif t == 'Effect':
        return ('monsterAbility', t)
    elif t == 'Flip':
        return ('monsterAbility', t)
    elif t == 'Toon':
        return ('monsterAbility', t)
    elif t == 'Spirit':
        return ('monsterAbility', t)
    elif t == 'Union':
        return ('monsterAbility', t)
    elif t == 'Gemini':
        return ('monsterAbility', t)
    elif t == 'Tuner':
        return ('monsterAbility', t)
    else:
        # its a type
        return ('type', t)

def parse_monster(data):
    output = {}
    output['name'] = data['name']
    output['cardType'] = 'Monster'
    output['attribute'] = data['attribute']

    output['monsterType'] = data['monsterType'].split()[0]

    if 'type0' in data:
        (index, value) = check_type(data['type0'],output['monsterType'])
        output[index] = value
    if 'type1' in data:
        (index, value) = check_type(data['type1'],output['monsterType'])
        output[index] = value
    if 'type2' in data:
        (index, value) = check_type(data['type2'],output['monsterType'])
        output[index] = value

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

    print_output(output)

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
    print_output(output)

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
    print_output(output)

def write_to_file(g):
    f = codecs.open('g.txt','w','utf-8')
    for s,p,o in g:
        f.write(p)
        f.write(' ')
        f.write(o)
        f.write('\n')

def parse_card(cardname):
    origcardname = cardname;
    cardname = cardname.replace(' ','_').replace('#','').strip()
    g = rdflib.Graph()
    g.load(LOAD_PATH + cardname)
    #write_to_file(g)

    # load up the data dictionary
    data = {}
    type_count = 0

    for s,p,o in g:
        k = str(p)
        if k == CARD_NAME:
            data['name'] = str(o)
        elif k == CARD_TYPE:
            data['cardType'] = str(o)
        elif k == ATTRIBUTE:
            data['attribute'] = str(o)
        elif k == MONSTER_TYPE:
            data['monsterType'] = str(o)
        elif k == TYPE:
            data['type' + str(type_count)] = str(o)
            type_count = type_count + 1
        elif k == LEVEL:
            data['level'] = int(float(o))
        elif k == ATK:
            data['attack'] = int(float(o))
        elif k == DEF:
            data['defense'] = int(float(o))
        elif k == PENDULUM:
            data['lpendulum'] = int(float(o))
            data['rpendulum'] = int(float(o))

    # process the output
    if "cardType" in data and "Monster" in data['cardType']:
        parse_monster(data)
    elif "attribute" in data and "Spell" in data['attribute']:
        parse_spell(data)
    elif "attribute" in data and "Trap" in data['attribute']:
        parse_trap(data)
    else:
        log_error(origcardname)

def parse_sets():
    for s in SCRAPE_SETS:
        r = requests.get(s)
        data = r.text
        soup = BeautifulSoup(data)
        contents = soup.find_all("div",{"id":"mw-content-text"})

        for tag in contents:
            trTags = tag.find_all("tr")
            for trTag in trTags:
                tdTags = trTag.find_all("td")
                counter = 0;
                for tdTag in tdTags:
                    if counter == 1:
                        parse_card(tdTag.text.strip())
                        break
                    # we only want the first entry
                    counter = counter + 1

parse_sets()
report_errors()
