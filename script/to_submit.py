# Author:         Fan Feifan
# Email:          fanff.pku@gmail.com
# Filename:       to_submit.py
# Last modified:  2015-04-17 09:56
# Description: change result format to submit 

import sys

if len(sys.argv) < 2:
    print 'argv[1]: resultdir'
    exit()
submit_file = open(sys.argv[1]+'/submit.txt','w')
for index in range(1, 61):
    res_file = open(sys.argv[1]+'/'+str(index)+'.res','r')
    number = ''
    if index < 50:
        number = 'MB0' + str(index + 50)
    else:
        number = 'MB' + str(index + 50)
    for line in res_file:
        content = line.strip().split()
        out_str = number + ' ' + content[2] + ' ' + content[4]+' ' + 'test' + '\n'
        submit_file.write(out_str)
    res_file.close()
submit_file.close()

