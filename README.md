# do-find

### Date: March 2021

## Task:

This C program finds a file/folder under any parent/upper-level directory.

## Criteria:

- Implementing a directory tree traversal in C.
- Implementing a recursive solution to perform this traversal.
- Printing the location, file modes (permissions), file size and link count.
- Error handling for edge cases.

### Requirements and Running it locally:

1. Run the script on a Bash (or even zsh) shell as certain commands may not be compatible with other shells.
2. The script takes in two command line arguments, the first argument for the root of the directory to search, and the second argument is the name of the file to be found.
3. On a successful traversal, the location, file modes (permissions), file size and link count are printed for that file.

## Aspects and Limitations:

In ```$ ./dofind search_dir find_file```, note the following:

- findfile can be a directory
- The exact location of searchdir is to be outputed as *search_dir/dir<sub>1</sub>/dir<sub>2</sub>/../dir<sub>n</sub>/findfile*
- If more than one find_files exist, then the one with the lowest link count should be returned
- If the permission of searchdir, dir<sub>i</sub>, or findfile don't allow the command to complete, then no output is shown and a subsequent "echo $?" after executing dofind should return *2*.
- If the find_file exists, a subsequent "echo $?" after executing dofind should return *0*.
- A searchdir or dir<sub>i</sub> along the search path is defined to be an ambiguous node if it has more than one child directories. If so, no matter whether find_file exists or not, there is no standard output and a subsequent "echo $?" after excecuting dofind returns 3. The program also prints out "ambiguous: search_dir/dir<sub>1</sub>/dir<sub>2</sub>/dir.../dir<sub>i</sub>" on the standard error output.

## Running the program (on MacOS):

- Install the command line-tools:

```
xcode-select --install
```

- Compile the dofind.c file by running clang like this in order to generate the dofind executable:

```
clang dofind.c -o dofind
```

- To run the executable generated for the dofind.c file, run the following:
```
$ ./dofind search_dir find_file
```

where search_dir is the directory to be searched in and find_file is the file to be found.


