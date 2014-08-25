#!/usr/bin/python

import codecs
import re
import requests

from bs4 import BeautifulSoup

OUTPUT_FILE = 'formats.txt'

LIMITED_CHART_URI = 'http://yugioh.wikia.com/wiki/Historic_Forbidden/Limited_Chart'
ILLEGAL_URI = 'http://yugioh.wikia.com/wiki/Illegal'

# find the ban list headers
r = requests.get(LIMITED_CHART_URI)
data = r.text
soup = BeautifulSoup(data)

formats = []
cards = []
contents = soup.find_all("th")
for th in contents:
    if th.has_attr('title') and th.has_attr('scope'):
        formats.append(th['title'])
    elif not th.has_attr('style') and \
        th.has_attr('scope') and th['scope'] == 'row':
            name = th.text.strip()
            name = re.sub(r"\(.*\)","",name)
            cards.append(name)

# the headers are present twice on the page
formats = formats[:len(formats)/2]

# find out all the ban info:
format_count = len(formats)
counter = 0
index = 0

# write out this data
cardout = codecs.open(OUTPUT_FILE,'w','utf-8')

tds = soup.find_all("td")
for td in tds:
    if td.has_attr('title') and td.has_attr('style'):
        if counter == format_count:
            # we move on to the next card
            counter = 0
            index = index + 1
        limit = td['title']
        if limit == 'Semi-limited':
            cardout.write(formats[counter] + '\t' + cards[index] + '\tSemi-limited\n')
        elif limit == 'Limited':
            cardout.write(formats[counter] + '\t' + cards[index] + '\tLimited\n')
        elif limit == 'Forbidden':
            cardout.write(formats[counter] + '\t' + cards[index] + '\tForbidden\n')
        counter = counter + 1

# get all the illegal card names
r = requests.get(ILLEGAL_URI)
data = r.text
soup = BeautifulSoup(data)

trs = soup.find_all("tr")
illegals = []
for tr in trs:
    if tr.has_attr('class'):
        tds = tr.find_all("td")
        for td in tds:
            name = td.text.strip()
            name = re.sub(r"\(.*\)","",name)
            illegals.append(name)
            break

illegals = list(set(illegals))
illegals.sort()
for i in illegals:
    cardout.write("Illegal\t" + i + "\tIllegal\n")

