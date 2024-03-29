#! /bin/sh

#====================================================================
# Variables
#====================================================================
build_dir=build
# The name of the library
lib_name=SciCell++
folder_name=scicellxx

#====================================================================
# The building script
#====================================================================

echo " "
echo "============================================================= "
echo "        "$lib_name" make clean distro script" 
echo "============================================================= "
echo " "

echo ""
echo "============================================================= "
echo ""
echo "I am going to create a clean distro and package it!"
echo ""
echo "============================================================= "
echo ""

#====================================================================
# Get working folder
#====================================================================
working_folder=$(pwd)

#====================================================================
# Clean for binaries
#====================================================================
echo ""
echo "============================================================= "
echo ""
echo "Cleaning distro for binaries"
echo ""
echo "============================================================= "
echo ""
cd $working_folder/$build_dir
make clean
cd $working_folder

#====================================================================
# Remove previous distros packages
#====================================================================
echo ""
echo "============================================================= "
echo ""
echo "Removing any previous old compressed versions"
echo ""
echo "============================================================= "
echo ""
rm -i $lib_name.tar.gz

#====================================================================
# Make a temporal directory
#====================================================================
echo ""
echo "============================================================= "
echo ""
echo "Create a temporary folder"
echo ""
echo "============================================================= "
echo ""
tmp_dir=$(mktemp -d -t scicellxx-XXXXXXXXXX)
echo ""
echo $tmp_dir
echo ""

#====================================================================
# Making a copy
#====================================================================
echo "============================================================= "
echo "Copying the library into the '" $tmp_dir"' folder ..."
echo "============================================================= "
if ! cp -r $working_folder $tmp_dir ; then
    echo ""
    echo ""
    echo ""
    echo "========================================================= "
    echo "[FAIL] copy"
    echo "========================================================= "
    echo ""
    exit 1
fi
echo ""
echo "[DONE] copy"
echo ""

#====================================================================
# Deleting .git folder
#====================================================================
echo "============================================================= "
echo "I am going to delete the .git folder and all .git* files"
echo "============================================================= "
if ! rm -rf $tmp_dir/$folder_name/.git* ; then
    echo ""
    echo ""
    echo ""
    echo "========================================================= "
    echo "[FAIL] Delete .git folder and all .git* files"
    echo "========================================================= "
    echo ""
    exit 1
fi
echo ""
echo "[DONE] Delete .git folder and all .git* files"
echo ""

#====================================================================
# Deleting build folder
#====================================================================
echo "============================================================= "
echo "I am going to delete the '$build_dir' folder"
echo "============================================================= "
if ! rm -rf $tmp_dir/$folder_name/$build_dir; then
    echo ""
    echo ""
    echo ""
    echo "========================================================= "
    echo "[FAIL] Delete the '$build_dir' folder"
    echo "========================================================= "
    echo ""
    exit 1
fi
echo ""
echo "[DONE] Delete the '$build_dir' folder"
echo ""

#====================================================================
# Deleting dat png in folders
#====================================================================
echo "============================================================= "
echo "I am going to delete"
echo "[dat,png,pdf,jpg,jpeg,tar.gz,tar.xz,bin,vtu,ubx,eps,fig,gp,m,rar] files,"
echo "ignoring those in [demos, external_src, tools] folders"
echo "============================================================= "
echo ""
if ! $tmp_dir/$folder_name/tools/development/clean_distro.py --root_folder $tmp_dir/$folder_name --ext dat png pdf jpg jpeg tar.gz tar.xz fig bin rar vtu ubx gp m eps --ignore_in_path demos docs external_src private tools ; then
    echo ""
    echo ""
    echo ""
    echo "========================================================= "
    echo "[FAIL] Delete [dat,png,pdf,jpg,jpeg,tar.gz,tar.xz,bin,vtu,ubx,eps,fig,gp,m,rar] files"
    echo "========================================================= "
    echo ""
    exit 1
fi
echo ""
echo "[DONE] Delete [dat,png,pdf,jpg,jpeg,tar.gz,tar.xz,bin,vtu,ubx,eps,fig,gp,m,rar] files"
echo ""

#====================================================================
# Create the package of the new distribution
#====================================================================
echo "============================================================= "
echo "I am going to package the new clean distribution"
echo "============================================================= "
echo ""
if ! tar cvfz $lib_name.tar.gz $tmp_dir/$folder_name ; then
    echo ""
    echo ""
    echo ""
    echo "========================================================= "
    echo "[FAIL] Package creation (.tar.gz)"
    echo "========================================================= "
    echo ""
    exit 1
fi
echo "[DONE] Package creation (.tar.gz)"
#====================================================================
# Delete temporal directory
#====================================================================
echo "============================================================= "
echo "I am going to delete the temporal folder"
echo "============================================================= "
echo ""
if ! rm -rf $tmp_dir ; then
    echo ""
    echo ""
    echo ""
    echo "========================================================= "
    echo "[FAIL] Delete temporal folder"
    echo "========================================================= "
    echo ""
    exit 1
fi
echo ""
echo "[DONE] Delete temporal folder"
echo ""

# Done
exit 0
