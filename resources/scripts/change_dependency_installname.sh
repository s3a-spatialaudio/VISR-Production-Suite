#fix install names in a 2nd level dependency.

loadlibname=$(otool -l $2 | grep -F "$1." | awk -F"name "  '{print $2}' | awk -F\(  '{print $1}')
install_name_tool -change $loadlibname @rpath/$1.dylib $2
