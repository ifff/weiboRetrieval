# Author:         Fan Feifan
# Email:          fanff.pku@gmail.com
# Filename:       gen_dynamic_corpus.py
# Last modified:  2015-04-14 13:08
# Description: generate dynamic corpus 



query_file = open('../query/origin/mb2012.topic', 'r')
corpus_file_name = '../corpus/refine.docEx.corpus'
dynamic_doc_dir = '../corpus/dynamicCorpus/'
info_dict = []
doc_array = []

# read query id
query_num = 1
for line in query_file:
    dynamic_doc_name = "%s/%d.corpus" % (dynamic_doc_dir, query_num)
    doc_array.append(open(dynamic_doc_name, 'w'))
    info_dict.append(long(line.split('\t')[0]))
    query_num += 1

query_file.close()

count = 0
corpus_file = open(corpus_file_name, 'r')
for line in corpus_file:
    count += 1
    if count % 10000 == 0:
        print '%i tweets have processed...' % count
    tweet_info = line.split('\t')
    timestamp = long(tweet_info[0])
    tweet_content = tweet_info[1].replace('\n','')
    out_str = "<DOC>\n<DOCNO>%s</DOCNO>\n<TEXT>\n%s\n</TEXT>\n</DOC>\n" % (tweet_info[0], tweet_content)

    for index in range(60):
        if timestamp <= info_dict[index]:
            doc_array[index].write(out_str)

corpus_file.close()
for index in range(60):
    doc_array[index].close()
