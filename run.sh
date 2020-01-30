#!/bin/sh

for bench in `ls testcases\ -\ release2/bench/`
do
	for config in `ls testcases\ -\ release2/config/`
	do
		echo ./project $config $bench index.rpt
		./project testcases\ -\ release2/config/$config ./testcases\ -\ release2/bench/$bench index.rpt
		#echo ./verify $config $bench index.rpt
		././verifier/verify   testcases\ -\ release2/config/$config ./testcases\ -\ release2/bench/$bench index.rpt
		echo ===============================================
	done
done
