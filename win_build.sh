# Zex2005 Windows Build - Rob Probin 5 Oct 2005
cd "windows_build"
sh cross-make.sh

# used to create cross build directory ... not any more
# create directory if it does not exist
#myDir="cross_build"
#
#if ( !( test -d "$myDir" ) )
#then
#  echo creating directory "$myDir"
#  mkdir "$myDir" ;
#fi
#  
# copy the make file into that directory
#cp Makefile "$myDir"
#
# now move into that directory to build
#cd "$myDir"
#
# now run the make  
#sh ../linux_control/cross-make.sh
#
# delete the duplicate of the make file
#rm Makefile
