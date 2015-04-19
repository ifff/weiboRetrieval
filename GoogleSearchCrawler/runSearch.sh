#!/bin/bash
queryFile=../query/origin/total.query
resultDir=../query/web/raw/
restart=1
python startSearch.py $queryFile $resultDir $restart
