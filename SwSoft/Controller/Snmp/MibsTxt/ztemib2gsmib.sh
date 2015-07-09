#!/bin/bash 
# Program: 
#       This program convert zte mib to gospell mib. 
# Version:
#	0.1 .
# History: 
# 	no.
# Author:
#	Qingsong Wang

# PATH
PATH=/bin:/sbin:/usr/bin:/usr/sbin:/usr/local/bin:/usr/local/sbin:~/bin 
export PATH 

# Main program
echo "convert zte mib to gospell mib \n"

#grep -n -A1 -B1 --color=auto 'zxDsl' GOSPELL-CMTS-SYSCTL-MIB.mib
#sed -i 's/zxDsl/gsCmts/g' GOSPELL-CMTS-SYSCTL-MIB.mib
sed -i 's/gsCmts/gsDsl/g' GOSPELL-DSL-SYSCTL-MIB.mib

#sed -i 's/ZxDsl/GsCmts/g' GOSPELL-CMTS-SYSCTL-MIB.mib
sed -i 's/GsCmts/GsDsl/g' GOSPELL-DSL-SYSCTL-MIB.mib
sed -i 's/zte/gos/g' GOSPELL-DSL-SYSCTL-MIB.mib
sed -i 's/Zte/Gos/g' GOSPELL-DSL-SYSCTL-MIB.mib
#sed -i 's/dsl/cmts/g' GOSPELL-CMTS-SYSCTL-MIB.mib

sed -i 's/ZxAn/GsAn/g' GOSPELL-AN-EOC-SYS-MIB.mib
sed -i 's/zxAn/gsAn/g' GOSPELL-AN-EOC-SYS-MIB.mib
sed -i 's/Zte/Gospell/g' GOSPELL-AN-EOC-SYS-MIB.mib
sed -i 's/zte/gospell/g' GOSPELL-AN-EOC-SYS-MIB.mib
sed -i 's/ZTE/GOSPELL/g' GOSPELL-AN-EOC-SYS-MIB.mib

sed -i 's/ZxAn/GsAn/g' GOSPELL-AN-EOC-CMC-MIB.mib
sed -i 's/zxAn/gsAn/g' GOSPELL-AN-EOC-CMC-MIB.mib
sed -i 's/zx/gs/g' GOSPELL-AN-EOC-CMC-MIB.mib
sed -i 's/Zte/Gospell/g' GOSPELL-AN-EOC-CMC-MIB.mib
sed -i 's/zte/gospell/g' GOSPELL-AN-EOC-CMC-MIB.mib
sed -i 's/ZTE/GOSPELL/g' GOSPELL-AN-EOC-CMC-MIB.mib
#sed changes .mib drectly ,Not stdout.

# Exit and Return
exit 0
