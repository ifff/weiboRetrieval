# Author:         Fan Feifan
# Email:          fanff.pku@gmail.com
# Filename:       reteval.py
# Last modified:  2015-04-14 22:45
# Description:  retrieval static index


import os
import sys
home_dir = os.environ['HOME']
base_dir = home_dir + '/study/weiboRetrieval'
if len(sys.argv[1]) < 2:
    print 'sys.argv[1]: result_dir'
    exit()
result_dir = sys.argv[1] 
os.system('mkdir ' + sys.argv[1])

for index in range(1,61):
    # write parameter
    para_file_name = "%s/parameter/reteval/%d.para" % (base_dir, index)
    para_file = open(para_file_name, 'w')
    # create index dir
    #index_dir = "%s/index/" % (base_dir, index)
    #if not os.path.exists(index_dir):
    #    os.system('mkdir ' + index_dir)
    index_file = "%s/contentIndex/base.key" % base_dir
    query_file = "%s/query/origin/%d.query" % (base_dir, index)
    result_file = "%s/%d.res" % (result_dir, index)

    out_str = "<parameters>\n"
    out_str += "\t<index>%s</index>\n" % index_file
    out_str += "\t<retModel>0</retModel>\n"
    #out_str += "\t<smoothSupportFile>%s/ssf/base.supp</smoothSupportFile>\n" % base_dir
    #out_str += "\t<smoothMethod>1</smoothMethod>\n"
    #out_str += "\t<smoothStrategy>1</smoothStrategy>\n"
    #out_str += "\t<DirichletPrior>100</DirichletPrior>\n"
    #out_str += "\t<feedbackDocCount>5</feedbackDocCount>\n"
    #out_str += "\t<feedbackTermCount>7</feedbackTermCount>\n"
    #out_str += "\t<feedbackCoefficient>0.6</feedbackCoefficient>\n"
    #out_str += "\t<feedbackMixtureNoise>0.5</feedbackMixtureNoise>\n"
    out_str += "\t<textQuery>%s</textQuery>\n" % query_file
    out_str += "\t<memory>2G</memory>\n"
    out_str += "\t<count>1000</count>\n"
    out_str += "\t<resultFile>%s</resultFile>\n" % result_file
    out_str += "\t<runID>test</runID>\n"
    out_str += "\t<trecFormat>true</trecFormat>\n"
    out_str += "</parameters>\n"
    para_file.write(out_str)
    para_file.close()
    if index % 10 == 0:
        os.system('RetEval ' + para_file_name)
    else:
        os.system('RetEval ' + para_file_name + ' & ')
