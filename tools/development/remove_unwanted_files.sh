#! /bin/sh

# Use this command to delete all files with the extension
# "*.~undo-tree~"

#find . -name "*.~undo-tree~" -type f -delete

# If you prefer to list the files that will be removed use the same
# command without the -delete flag
find . -name "*.~undo-tree~" -type f

# If you want to list where the bin folders are you can do it with
find . -name "bin" -type d
