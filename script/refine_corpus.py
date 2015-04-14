# Author:         Fan Feifan
# Email:          fanff.pku@gmail.com
# Filename:       refine_corpus.py
# Last modified:  2015-04-13 18:30
# Description: filter non-english tweet 

import sys
import re
if len(sys.argv) < 3:
    print 'argv[1]: originCorpus'
    print 'argv[2]: filteredCorpus'
    exit()


