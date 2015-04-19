f1 = open('mb2012.topic','r')
f2 = open('total.query','w')
for line in f1:
    content = line.strip().split('\t')
    newline = content[1] + '\n'
    f2.write(newline)
