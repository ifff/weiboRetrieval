#! /usr/bin/python
# -*- coding: utf-8 -*-

#@ Author:          Liang Feng
#@ Mail:            liangfeng1987@gmail.com
#@ FileName:        refine_title_corpus.py
#@ Create Time:     2012-12-01 16:53
#@ Description:    

import sys
import re

# filter non-english term; RT term; and @username
def non_ascii_term(term):
    ascii_reg = re.compile(r'^[\x00-\x7F]+$')
    url_reg = re.compile(r'(https?://)?(\w+\.)+\w+(/\w+)*/*')

    if ascii_reg.match(term) and not url_reg.search(term):
        return True
    else:
        return False

try:
    corpus_in = open(sys.argv[1], 'r')
    corpus_out = open(sys.argv[2], 'w')
    corpus_err = open(sys.argv[3], 'w')
except:
    sys.stderr.write('Usage: ' + sys.argv[0] + ' corpus_in corpus_out corpus_err\n')
    exit(-1)

space_reg = re.compile('\s+')

corpus_lines = corpus_in.readlines()

titles = {}
terms = {}
for line in corpus_lines:
    line = line.replace('\n', '')
    tweet_id, tweet_content = line.split('\t')

    if len(tweet_content) == 1:
        # print 'empty tweet....'
        continue

    # calculate content stats
    if not tweet_content in titles:
        titles[tweet_content] = 0
    else:
        titles[tweet_content] += 1

    term_list = space_reg.split(tweet_content)
    for term in term_list:
        term = term.lower()
        if term in terms:
            terms[term] += 1
        else:
            terms[term] = 0

# rank according to frequence
top_ranked_title = sorted(titles.items(), key=lambda d:d[1], reverse=True)[0:79]
high_fre_tweets = {}
for pair in top_ranked_title:
    #print pair[0], pair[1]
    high_fre_tweets[pair[0]] = pair[1]

for line in corpus_lines:
    line = line.replace('\n', '')
    tweet_id, tweet_content = line.split('\t')

    if tweet_content in high_fre_tweets or len(tweet_content) == 1:
        # print tweet_content
        corpus_err.write("%s\n" % line)
        continue
        
    term_list = space_reg.split(tweet_content)

    term_list = filter(non_ascii_term, term_list)

    for index in range(len(term_list)):
        # eliminate hash tag
        term = term_list[index]
        if term[0] == '#':
            term_list[index] = term.replace('#', '')

    if len(term_list) == 0:
        corpus_err.write("%s\n" % line)
        continue

    out_content = ' '.join(term_list)
    out_str = "%s\t%s\n" % (tweet_id, out_content)
    corpus_out.write(out_str)

corpus_in.close()
corpus_out.close()
corpus_err.close()
