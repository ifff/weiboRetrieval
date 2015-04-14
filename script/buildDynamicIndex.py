# Author:         Fan Feifan
# Email:          fanff.pku@gmail.com
# Filename:       buildDynamicIndex.py
# Last modified:  2015-04-14 21:08
# Description: build index for dynamic corpus

import os
query_num = 60
home_dir = os.environ['HOME']
base_dir = home_dir + '/study/weiboRetrieval'

for index in range(query_num):
    # write corpus list 
    filelist_name = "%s/parameter/filelist/%d.filelist"  % (base_dir, index)
    filelist = open(filelist_name, 'w')
    out_str = "%s/corpus/dynamicCorpus/%d.corpus" % (base_dir, index)
    filelist.write(out_str)
    filelist.close()
    # write parameter
    para_file_name = "%s/parameter/dynamicIndex/%d.para" % (base_dir, index)
    para_file = open(para_file_name, 'w')
    # create index dir
    index_dir = "%s/dynamicIndex/index%d/" % (base_dir, index)
    if not os.path.exists(index_dir):
        os.system('mkdir ' + index_dir)

    out_str = "<parameters>\n"
    out_str += "\t<dataFiles>%s</dataFiles>\n" % filelist_name
    out_str += "\t<docFormat>trec</docFormat>\n"
    out_str += "\t<index>%sbase</index>\n" % index_dir 
    out_str += "\t<indexType>key</indexType>\n"
    out_str += "\t<memory>2G</memory>\n"
    out_str += "\t<stemmer>Porter</stemmer>\n"
    out_str += "\t<countStopWords>True</countStopWords>\n"
    out_str += "\t<stopWords>%s/corpus/stopword</stopWords>\n" % base_dir
    out_str += "</parameters>\n"
    para_file.write(out_str)
    para_file.close()
    if index % 10 == 0:
        os.system('BuildIndex ' + para_file_name)
    else:
        os.system('BuildIndex ' + para_file_name + ' & ')
