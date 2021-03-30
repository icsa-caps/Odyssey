#!/usr/bin/env bash

source ./cluster.sh

LOCAL_HOST=`hostname`
EXECUTABLES=($1)
SCRIPTS=($2)
MAKE_FOLDER=$3
SCRIPT_FOLDER=$4


DEST_FOLDER=$5

cd $MAKE_FOLDER
#make clean
make $1
rm -rf core
cd -

for EXEC in "${EXECUTABLES[@]}"
do
	#echo "${EXEC} copied to {${HOSTS[@]/$LOCAL_HOST}}"
	parallel --will-cite scp $MAKE_FOLDER/${EXEC} {}:${DEST_FOLDER}/${EXEC} ::: $(echo ${HOSTS[@]/$LOCAL_HOST})
	echo "${EXEC} copied to {${HOSTS[@]/$LOCAL_HOST}}"
done

for SCRIPT in "${SCRIPTS[@]}"
do
	#echo "${EXEC} copied to {${HOSTS[@]/$LOCAL_HOST}}"
	parallel --will-cite scp $SCRIPT_FOLDER/${SCRIPT} {}:${DEST_FOLDER}/${SCRIPT} ::: $(echo ${HOSTS[@]/$LOCAL_HOST})
	echo "${SCRIPT} copied to {${HOSTS[@]/$LOCAL_HOST}}"
done