#!/bin/bash
queryFile=../queryFile
resultDir=../websource/result/
restart=10000
python startSearch.py $queryFile $resultDir $restart
