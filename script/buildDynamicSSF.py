# Author:         Fan Feifan
# Email:          fanff.pku@gmail.com
# Filename:       buildDynamicSSF.py
# Last modified:  2015-04-14 22:03
# Description: build ssf files for dynamic corpus


import os
query_num = 60
home_dir = os.environ['HOME']
base_dir = home_dir + '/study/weiboRetrieval'

for index in range(1, 61):
    
    # write parameter
    para_file_name = "%s/parameter/dynamicSSF/%d.para" % (base_dir, index)
    para_file = open(para_file_name, 'w')
    ssf_file_name = "%s/dynamicSSF/%d.supp" % (base_dir, index)
    index_file_name = "%s/dynamicIndex/index%d/base.key" % (base_dir, index)

    out_str = "<parameters>\n"
    out_str += "\t<index>%s</index>\n" % index_file_name 
    out_str += "\t<smoothSupportFile>%s</smoothSupportFile>\n" % ssf_file_name
    out_str += "</parameters>\n"
    para_file.write(out_str)
    para_file.close()
    if index % 10 == 0:
        os.system('GenerateSmoothSupport ' + para_file_name)
    else:
        os.system('GenerateSmoothSupport ' + para_file_name + ' & ')

