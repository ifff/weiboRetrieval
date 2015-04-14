# Author:         Fan Feifan
# Email:          fanff.pku@gmail.com
# Filename:       preProcessCorpus.py
# Last modified:  2015-04-13 18:18
# Description: merge content corpus and title corpus   

import sys

if len(sys.argv) < 3:
   print 'argv[1]: contentCorpus'
   print 'argv[2]: titleCorpus'
   print 'argv[3]: docExCorpus'
   exit()

titleDic = {}
# read title file 
titleFile = open(sys.argv[2], 'r')
line_count = 0
while True:
    line = titleFile.readline()
    if not line:break
    line_count += 1
    if line_count % 1000 == 0:
        print 'title corpus ' + str(line_count) + ' lines processed...'
    title = line.strip().split('\t')
    titleDic[title[0]] = title[1]
# open docEx corpus and read contentCorpus
docExFile = open(sys.argv[3], 'w')
contentFile = open(sys.argv[1], 'r')
line_count = 0
while True:
    line = contentFile.readline()
    if not line:break
    line_count += 1
    if line_count % 1000 == 0:
        print 'content corpus ' + str(line_count) + ' lines processed...'
    content = line.strip().split('\t')
    if titleDic.has_key(content[0]):
        docLine = content[0] + '\t' + content[1] + ' ' + titleDic[content[0]] + '\n'
        docExFile.write(docLine)
    else:
        docExFile.write(line)

docExFile.close()
contentFile.close()
titleFile.close()
