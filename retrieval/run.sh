#!
# This is a sample run script
# Please copy this file for your own run

year=2012
index=../dynamicIndex/
ssf=../dynamicSSF/
query=../query/origin/
expand=../query/issue/
#expand=test_query12/
#expand=/index12/query/freebase/
#expand=/index12/query/origin/
#expand=/index12/query/issue/
alpha=0.4
beta=0.5
doc=5
term=7
for doc in 3 4 5 6 7 8 9; do
for term in 3 4 5 6 7 8 9 10; do
#for alpha in 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9; do
##for beta in 0.5; do
    output=../result/dynamic/RTRM/issue_alpha${alpha}_beta${beta}_doc${doc}_term${term}/
    #output=result2012/QEFB
    #output=result2012/knowledgeTerm 
    mkdir $output
    total=5
    for((remain=0; remain<total; remain++)); do 
        ./main -index $index -ssf $ssf -query $query -expand $expand -output $output -count 60 -year $year -term $term -doc $doc -alpha $alpha -beta $beta -remain $remain -total $total & 
    #    ./main -index $index -ssf $ssf -query $query -expand $expand -output $output -count 60 -year $year -term $term -doc $doc -alpha $alpha -beta $beta -remain $remain -total $total&
    done;
done;
done; 
