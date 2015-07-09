# /usr/bin/FindRootPath.sh, Initialization script for bash and sh

# find the root path where a Release.Readme exist.
if [ $# != 1 ]
then
    exit 1
fi

cd $1
while [ ! -e Release.Readme ]
do
	if [ "$(pwd)" = "/" ]
	then
	   exit 1
	fi 
	cd ..
done
pwd
exit 0

