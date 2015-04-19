# Author:         Fan Feifan
# Email:          fanff.pku@gmail.com
# Filename:       filter_result.py
# Last modified:  2015-04-15 08:50
# Description: filter retrieval results by upbound id
import sys
import os
if len(sys.argv) < 3:
    print 'argv[1]: raw results dir'
    print 'argv[2]: filtered results dir'
    exit()
os.system('mkdir ' + sys.argv[2])
info_file = open('../query/origin/mb2012.topic', 'r')
info_dict = []
for line in info_file:
    info_dict.append(long(line.split('\t')[0]))

info_file.close()

for index in range(1, 61):
    raw_result = open(sys.argv[1]+'/'+str(index)+'.res','r')
    filtered_result = open(sys.argv[2]+'/'+str(index)+'.res','w')
    valid_count = 0
    for line in raw_result:
        tweet_info = line.strip().split()
        tweet_id = long(tweet_info[2])
        if tweet_id < info_dict[index-1]:
            valid_count += 1
            filtered_result.write(line)
            if valid_count >= 1000:break
    raw_result.close()
    filtered_result.close()


