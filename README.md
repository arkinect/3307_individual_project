# Project Overview
A file explorer with basic interaction. Built for linux on wsl. 

# Running 
make to build
./FileManager to run

# Note on comments throughout
I added comments for functions to the .cpp files as the functions are the same between the .h and .cpp files, it seems irrelevant to add to both

# Note on Opening Files
ngl I have no idea how this will run outside of the context of wsl. 
As of right now, I've built in functionality so that it attempts to open files with ubuntu defaults as specified by the widget package, 
and if it cant attempts to open with windows defaults.

# Credit 
Mathew Lane
251230373
Feb 2, 2026

# List of bugs I have found
renaming a folder to something the same as an existing folder combines the two into one (this onoly applies if the one not being renamed is empty)
