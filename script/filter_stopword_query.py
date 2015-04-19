# Author:         Fan Feifan
# Email:          fanff.pku@gmail.com
# Filename:       filter_stopword_query.py
# Last modified:  2015-04-19 15:42
# Description:  filter stopword in query, why ParseToFile do not work!!!

import sys

if len(sys.argv) < 2:
    print 'argv[1]: query dir'
    exit()
# load stopword
stopwordDict = {}
stopWordFile = open('../corpus/stopword','r')
for line in stopWordFile:
    stopwordDict[line.strip()] = True  
stopWordFile.close()

for index in range(1, 61):
    rawQuery = open(sys.argv[1]+'/'+str(index)+'.raw','r')
    issueQuery = open(sys.argv[1]+'/'+str(index)+'.query','w')
    for line in rawQuery:
        if not stopwordDict.has_key(line.strip()) and not line.strip().isdigit():
            issueQuery.write(line)
    rawQuery.close()
    issueQuery.close()
