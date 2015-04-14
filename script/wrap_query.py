# Author:         Fan Feifan
# Email:          fanff.pku@gmail.com
# Filename:       wrap_query.py
# Last modified:  2015-04-14 13:00
# Description: wrap_query TREC format

import sys
import os
if len(sys.argv) < 3:
    print 'argv[1]: originQuery'
    print 'argv[2]: TREC format query dir'
    exit()

origin = open(sys.argv[1], 'r')
line_count = 0
while True:
    line = origin.readline()
    if not line:break
    line_count += 1
    queryFile =  sys.argv[2] + '/' + str(line_count) + '.origin'
    trec = open(queryFile, 'w')
    query_content = line.split('\t')[1]
    output_str = "<DOC>\n<DOCNO>%s</DOCNO>\n<TEXT>\n%s\n</TEXT>\n</DOC>\n" % (line_count, query_content)
    trec.write(output_str)
    trec.close()

    # parse query 
    paraFile = '../parameter/ParseFile.para'
    query_para = open(paraFile, 'w')
    line =                                                                                        \
    '<parameter>\n<docFormat>trec</docFormat>\n<outputFile>%s</outputFile>\
            \n<stemmer>Porter</stemmer>\n<stopWords>../corpus/stopword</stopWords>\n</parameter>' \
                 % ('../query/'+str(line_count)+'.query')
    query_para.write(line)
    query_para.close()
    os.system('ParseToFile ' + paraFile + ' ' + queryFile)

origin.close()

