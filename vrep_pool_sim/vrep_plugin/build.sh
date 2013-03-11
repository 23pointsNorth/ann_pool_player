#! /bin/sh
make
FILE_PATH='/home/daniel/Programs/V-REP_PRO_EDU_V3_0_1_Linux/libv_repExtPoolSim.so'
cp -u -f ./lib/libvrep_plugin.so $FILE_PATH
echo "Copyed library to $FILE_PATH"
