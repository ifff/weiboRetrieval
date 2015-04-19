# Author:         Fan Feifan
# Email:          fanff.pku@gmail.com
# Filename:       wrap_corpus.py
# Last modified:  2015-04-14 13:00
# Description: wrap_corpus TREC format

import sys
if len(sys.argv) < 3:
    print 'argv[1]: originCorpus'
    print 'argv[2]: TREC format corpus'
    exit()

origin = open(sys.argv[1], 'r')
trec = open(sys.argv[2], 'w')
line_count = 0
while True:
    line = origin.readline()
    if not line:break
    line_count += 1
    if line_count % 1000 == 0:
        print '%s lines processed...' % (line_count)
    tweet_num = line.split('\t')[0]
    tweet_content = ''
    if len(line.split('\t')) == 2:
        tweet_content = line.split('\t')[1]
    output_str = "<DOC>\n<DOCNO>%s</DOCNO>\n<TEXT>\n%s\n</TEXT>\n</DOC>\n" % (tweet_num,\
                                                                              tweet_content.lower())
    trec.write(output_str)

origin.close()
trec.close()

