# do-find

### Date: March 2021

## Task:

This C program finds a file under a directory.

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
$ ./dofind searchdir findfile
```



