# Author:         Fan Feifan
# Email:          fanff.pku@gmail.com
# Filename:       gen_issue_query.py
# Last modified:  2015-04-19 13:12
# Description:  generate issue query from retrieval results


import sys
import os
if len(sys.argv) < 4:
    print 'argv[1]: retrieval result dir'
    print 'argv[2]: gen issue query dir'
    print 'argv[3]: corpus file'
    exit()

if not os.path.exists(sys.argv[2]):
    os.system('mkdir ' + sys.argv[2])
# load corpus file 
corpus_dict = {}
corpusFile = open(sys.argv[3], 'r')
line_count = 0
for line in corpusFile:
    line_count += 1
    if line_count % 10000 == 0:
        print 'load corpus %d lines...' % line_count
    tweet_id = line.split('\t')[0]
    tweet_content = ''
    if len(line.split('\t')) > 1:
        tweet_content = line.split('\t')[1]

    tweet_content = tweet_content.replace('\n','')
    corpus_dict[tweet_id] = tweet_content

# load result file 
for index in range(1, 61):
    resultFile = open(sys.argv[1]+'/'+str(index)+'.res', 'r')
    queryFile = open(sys.argv[2]+'/'+str(index)+'.origin','w')
    issue_line = resultFile.readline()
    issue_id = issue_line.strip().split()[2]
    issue_content = corpus_dict[issue_id]
    output_str = "<DOC>\n<DOCNO>%s</DOCNO>\n<TEXT>\n%s\n</TEXT>\n</DOC>\n" % (index, issue_content)
    queryFile.write(output_str)
    queryFile.close()
    # parse query
    paraFile = '../parameter/ParseFile.para'
    query_para = open(paraFile, 'w')
    line =                                                                                        \
    '<parameters>\n<docFormat>trec</docFormat>\n<outputFile>%s</outputFile>\
            \n<stemmer>Porter</stemmer>\n<stopWords>/home/fanff/study/weiboRetrieval/corpus/stopword</stopWords>\n</parameters>' \
                 % (sys.argv[2]+'/'+str(index)+'.raw')
    query_para.write(line)
    query_para.close()
    os.system('ParseToFile ' + paraFile + ' ' + sys.argv[2]+'/'+str(index)+'.origin')
    
