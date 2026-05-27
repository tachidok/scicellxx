#!/usr/bin/env python3

import os
import sys
import shutil
import argparse

def scan_for_files_of_a_type(root_folder, ext, ignore_in_path):
    # A counter for the number of files to be deleted
    sum_ext_files = 0
    
    # A list with the full names of the files found to delete
    files_to_delete = list()
    
    # Get the list of all files in directory tree at given path
    list_of_all_files = list()
    for (dir_path, dir_names, file_names) in os.walk(root_folder):
        list_of_all_files += [os.path.join(dir_path, file) for file in file_names]
    
    for entry in list_of_all_files:
        # Flag to check whether to ignore or not the file
        ignore_file = False
        for item in ignore_in_path:
            if entry.find(item) >= 0:
                ignore_file = True
                break

        # If file not ignored then check it it ends with any given extension
        if not ignore_file:
            for extension in ext:
                if entry.endswith(extension):
                    #print(f"File ending with {type_file} : {entry}")
                    files_to_delete.append(entry)
                    sum_ext_files = sum_ext_files + 1
                    break;
    
    return sum_ext_files, files_to_delete

def main():
    if len(sys.argv) < 2:
        print("Usage: clean_distro.py [--root_folder] [--ext] [-- ignore_in_path]")
        sys.exit()
    
    # Create the parser to deal with the arguments
    parser = argparse.ArgumentParser("Creates a clean distribution to move into another machine")
    
    # Set the positional arguments
    parser.add_argument("--root_folder", type=str, help="The root folder to search for given extensions", required=True)
    parser.add_argument("--ext", type=str, nargs='+', help="The list of extensions to look for and delete", required=True, choices=['dat', 'png', 'jpg', 'jpeg', 'pdf', 'fig', 'vtu', 'ubx', 'bin', 'rar', 'm', 'gp', 'tar.gz', 'tar.xz', 'eps'])
    parser.add_argument("--ignore_in_path", type=str, nargs='*', help="A list of strings that may occur in the path, therefore these files should be ignored for deletion", required=False)
        
    # parse args
    args = parser.parse_args()

    # --------------------------------------------------------------
    # Information
    # --------------------------------------------------------------
    print(f"Searching for '{args.ext}' files in '{args.root_folder}' directory and all subdirectories ...\n")
    
    for item in args.ignore_in_path:
        print(f"Ignoring filenames with: {item} string")
    
    print(f"Scanning ...")
    
    #n_type_files, files_found = scan_for_files_of_a_type(root_folder, type_file, ignore_in_pathlist_ignore_has_in_filename)
    n_type_files, files_found = scan_for_files_of_a_type(args.root_folder, args.ext, args.ignore_in_path)
    #n_type_files, files_found = scan_for_files_of_a_type()

    # --------------------------------------------------------------
    # Output results
    # --------------------------------------------------------------
    if n_type_files > 0:
        print(f"List of files '{args.ext}':")
        for item in files_found:
            print(item)

        print(f"\nNumber of {args.ext} files: {n_type_files}\n")
        user_input = input("Really delete? [y/n]: ")
        if user_input.lower() == 'y':
            for item in files_found:
                print(f"Deleting {item}")
                os.unlink(item)
        else:
            print("No files deleted\n")
    else:
        print(f"No '{args.ext}' files found")
        
    print("[Done]\n")    

'''
Usage:
  ./clean_distro.py root_folder extension_type list of strings in filename to ignore
'''
if __name__ == '__main__':
    # Run the main function
    main()
    

