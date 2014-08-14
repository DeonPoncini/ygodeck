#!/usr/bin/python

import logging
import rdflib
import codecs

from urllib2 import quote

logging.basicConfig()

INPUT_FILE = 'cardunique.txt'
ERROR_FILE = 'carderrors.txt'
OUTPUT_FILE = 'card.txt'

# yugioh wiki paths
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
RANK = PROPERTY_PATH + 'Rank'
ATK = PROPERTY_PATH + 'ATK'
ATKSTR = PROPERTY_PATH + 'ATK_string'
DEF = PROPERTY_PATH + 'DEF'
DEFSTR = PROPERTY_PATH + 'DEF_string'
PENDULUM = PROPERTY_PATH + 'Pendulum_Scale'
IMAGE = PROPERTY_PATH + 'Card_Image'
TEXT = PROPERTY_PATH + 'Lore'

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
    g = rdflib.Graph()
    uri = LOAD_PATH + cardname
    g.load(LOAD_PATH + quote(cardname.encode('utf-8')))

    # load up the data dictionary
    data = {}
    type_count = 0

    for s,p,o in g:
        try:
            k = str(p)
            if k == CARD_NAME:
                data['name'] = o.encode('utf-8')
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
            elif k == RANK:
                data['level'] = int(float(o))
            elif k == ATK:
                data['attack'] = int(float(o))
            elif k == ATKSTR:
                if o.isnumeric():
                    data['attack'] = int(float(o))
                else:
                    data['attack'] = str(o)
            elif k == DEF:
                data['defense'] = int(float(o))
            elif k == DEFSTR:
                if o.isnumeric():
                    data['defense'] = int(float(o))
                else:
                    data['defense'] = str(o)
            elif k == PENDULUM:
                data['lpendulum'] = int(float(o))
                data['rpendulum'] = int(float(o))
        except ValueError:
            continue

    # process the output
    try:
        print data
        if "cardType" in data and "Monster" in data['cardType']:
            print_output(parse_monster(data))
        elif "attribute" in data and "Spell" in data['attribute']:
            print_output(parse_spell(data))
        elif "attribute" in data and "Trap" in data['attribute']:
            print_output(parse_trap(data))
        else:
            log_error(origcardname)
    except:
            log_error(origcardname)


def parse_cards():
    for line in cardin:
        if line.startswith('#'):
            continue
        line = line.strip()
        parse_card(line)

parse_cards()
