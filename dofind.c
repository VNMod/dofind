// Project 2 - Intro to Unix by Varun Nimish Modak

#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// MACRO definitions:
#define EXIT_PERMISSION_DENIED 2
#define EXIT_AMBIGUOUS_NODE 3

// FUNCTION DECLARATIONS:
int traverse(char *folder, char *find_file, char *path, int linkcount[], int foundfiledepth[]);
int ambiguity(char *folder, char *find_file, char *path, int linkctr[], int foundfiledepth[]);

// stores the stats of a file.
struct foundfile
{
    int filesize;       // size of the file or folder
    int linkcount;      // number of links it takes to get to the file
    char filemodes[11]; // permissions for particular file or directory
    char *location;     // path of the file with respect to the search directory
};

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("--------------------------------------------------------------------------------");
        printf("\nOnly 3 arguments are allowed in the terminal. No more and no less. \n");
        printf("--------------------------------------------------------------------------------\n");
        return EXIT_FAILURE;
    }

    // Command Line Arguments are entered in the following format:
    // $ dofind search_dir find_file

    int EXIT_STATUS;

    char *search_dir = argv[1];
    char *find_file = argv[2];

    // check if the directory (to be searched) exists:
    int ret = access(search_dir, F_OK);

    if (ret == -1)
    {
        printf("--------------------------------------------------------------------------------");
        printf("\nError occured. The directory to be searched does not exist. \n");
        printf("--------------------------------------------------------------------------------\n");
        return EXIT_FAILURE;
    }

    // checking file permissions for search_dir:
    int Rok = access(search_dir, R_OK);
    int Wok = access(search_dir, W_OK);
    int Xok = access(search_dir, X_OK);

    if (Rok == -1 || Wok == -1 || Xok == -1)
    {
        return EXIT_PERMISSION_DENIED;
    }

    int success;             // flag for search success
    char *path = search_dir; //note: root directory for the path is search_dir

    int linkcount[1]; // denotes the number of links between the search directory and the file to be found.
    linkcount[0] = 0;

    int linkctr[1]; // denotes the number of links between the search directory and find file to be found.
    linkctr[0] = 0;

    // used to track the depth of the file to be found. Ideally, we'd like the shallowest match.
    // the ambiguity() function ensures that we find the shallowest copy.
    int foundfiledepth[1];
    foundfiledepth[0] = 100;

    printf("\nPROJECT 2 -- RUNNING SCRIPT by Varun Nimish Modak\n");
    printf("Syntax: ./dofind search_dir find_file\n");
    // calls a recursive solution to find if there are any ambiguous nodes,
    // and also searches for the shallowest match if multiples matches are found (if the file to be found is a file and not directory)
    int ambiguity_flag = ambiguity(search_dir, find_file, path, linkctr, foundfiledepth);

    // if no ambiguous node exists, then traverse again to find the file
    if (ambiguity_flag == EXIT_SUCCESS)
    {
        // calls a recursive solution to traverse the search directory and find the file/folder to be found:
        success = traverse(search_dir, find_file, path, linkcount, foundfiledepth);
    }
    else
    {
        success = ambiguity_flag;
    }

    printf("\n--------------------------------------------------------------------------------");
    printf("\nSTATUS: ");
    switch (success)
    {
    case EXIT_SUCCESS:
        printf("\n--------------------------------------------------------------------------------\n");
        printf("File or folder was found. Exit code (0)");
        printf("\n--------------------------------------------------------------------------------\n");
        break;
    case EXIT_FAILURE:
        printf("\n--------------------------------------------------------------------------------\n");
        printf("File or folder wasn't found. Exit code (1)");
        printf("\n--------------------------------------------------------------------------------\n");
        break;
    case EXIT_PERMISSION_DENIED:
        printf("\n--------------------------------------------------------------------------------\n");
        printf("Due to insufficient permissions, the traversal was stopped. Exit code (2)");
        printf("\n--------------------------------------------------------------------------------\n");
        break;
    case EXIT_AMBIGUOUS_NODE:
        printf("\n--------------------------------------------------------------------------------\n");
        printf("Ambiguous nodes were encountered. Exit code (3)");
        printf("\n--------------------------------------------------------------------------------\n");
        break;
    default:
        break;
    }

    printf("\n\n");

    return success;
}

// Method:
// a recursive function to traverse a given folder until:
// 1. The file is found
// 2. An ambiguous node is encountered when more than one child directory -- subdirectory fork
// Returns true if the file is found, else false.

int traverse(char *folder, char *find_file, char *path, int linkcount[], int foundfiledepth[])
{
    int success = EXIT_FAILURE;

    struct dirent *de; // a pointer for the entries within a directory

    DIR *dr;

    dr = opendir(path); // open the passed folder

    if (dr == NULL) // if you could not open that folder
    {
        printf("\nCould not open the current directory - %s.\n", folder);
        return EXIT_PERMISSION_DENIED;
    }

    int linkc = linkcount[0] + 1;
    int oldlinkc = linkcount[0]; // keeps count of the linkcount before it is incremented

    // read all the sub-directories within that directory:

    while ((de = readdir(dr)) != NULL)
    {
        // if it is a directory (that is not essentially a relative path of . or ..),
        // 1. check if that folder matches the folder to be found
        // 2. if not, then traverse it.
        // 3. if the folder/file is found: print the detailed information for the file with:
        // the location, file modes, file size and link count.
        if (de->d_type == DT_DIR && strcmp(de->d_name, ".") && strcmp(de->d_name, ".."))
        {
            linkcount[0] = linkcount[0] + 1; //increment the linkcount

            // Step 1.

            if (!strcmp(de->d_name, find_file))
            {
                // Step 3.

                // creating the file path for the found folder:
                char filepath[300] = "";
                strcat(filepath, path);
                strcat(filepath, "/");
                strcat(filepath, de->d_name);

                struct stat st;
                stat(filepath, &st);

                int fsize = st.st_size; // size of the folder in bytes
                char filemodes[11] = "";

                // PERMISSIONS:

                //owner/user:
                filemodes[0] = 'd';
                filemodes[1] = (st.st_mode & S_IRUSR) ? 'r' : '-';
                filemodes[2] = (st.st_mode & S_IWUSR) ? 'w' : '-';
                filemodes[3] = (st.st_mode & S_IXUSR) ? 'x' : '-';
                //group:
                filemodes[4] = (st.st_mode & S_IRGRP) ? 'r' : '-';
                filemodes[5] = (st.st_mode & S_IWGRP) ? 'w' : '-';
                filemodes[6] = (st.st_mode & S_IXGRP) ? 'x' : '-';
                //others
                filemodes[7] = (st.st_mode & S_IROTH) ? 'r' : '-';
                filemodes[8] = (st.st_mode & S_IWOTH) ? 'w' : '-';
                filemodes[9] = (st.st_mode & S_IWOTH) ? 'x' : '-';
                filemodes[10] = '\0';

                printf("\n---------------------------------------------------------------------------------\n");
                printf("%-30s %-15s %-10s %-15s", "Location: ", "File Modes: ", "File Size: ", "Link Count:");
                printf("\n---------------------------------------------------------------------------------\n");
                printf("%-30s %-15s %-11d %-15i\n", filepath, filemodes, fsize, linkcount[0]);
                printf("---------------------------------------------------------------------------------\n");

                // return the status for the file search: 0 (indicates that no error occured)
                return EXIT_SUCCESS;
            }

            char newpath[300] = "";
            strcat(newpath, path);
            strcat(newpath, "/");
            strcat(newpath, de->d_name);

            // Step 2.
            success = traverse(de->d_name, find_file, newpath, linkcount, foundfiledepth);
        }
        // else, if it is a file or symbolic link,
        // check if it matches the file to be found
        else if (de->d_type == DT_REG)
        {
            // if the filename matches and the current linkcount matches the file depth
            // (as per the shallowest match found in the ambiguity function)
            if (!strcmp(de->d_name, find_file) && linkc == *foundfiledepth)
            {
                // Step 3.

                // creating the file path for the found file:
                char filepath[300] = "";
                strcat(filepath, path);
                strcat(filepath, "/");
                strcat(filepath, de->d_name);

                struct stat st;
                stat(filepath, &st);

                int fsize = st.st_size; // size of the folder in bytes
                char filemodes[11] = "";

                // PERMISSIONS:

                //owner/user:
                filemodes[0] = '-';
                filemodes[1] = (st.st_mode & S_IRUSR) ? 'r' : '-';
                filemodes[2] = (st.st_mode & S_IWUSR) ? 'w' : '-';
                filemodes[3] = (st.st_mode & S_IXUSR) ? 'x' : '-';
                //group:
                filemodes[4] = (st.st_mode & S_IRGRP) ? 'r' : '-';
                filemodes[5] = (st.st_mode & S_IWGRP) ? 'w' : '-';
                filemodes[6] = (st.st_mode & S_IXGRP) ? 'x' : '-';
                //others
                filemodes[7] = (st.st_mode & S_IROTH) ? 'r' : '-';
                filemodes[8] = (st.st_mode & S_IWOTH) ? 'w' : '-';
                filemodes[9] = (st.st_mode & S_IWOTH) ? 'x' : '-';
                filemodes[10] = '\0';

                printf("\n---------------------------------------------------------------------------------\n");
                printf("%-30s %-15s %-10s %-15s", "Location: ", "File Modes: ", "File Size: ", "Link Count:");
                printf("\n---------------------------------------------------------------------------------\n");
                printf("\n%-30s %-15s %-11d %-15i\n", filepath, filemodes, fsize, oldlinkc);
                printf("---------------------------------------------------------------------------------\n");

                return EXIT_SUCCESS;
            }
        }
    }

    return success; // reaches here if the file/folder wasn't found.
}

// Method:
// a recursive function that traverses all the subdirectories in order to check for ambiguous nodes.
// this function also finds the specific linkcount at which a file is found (very helpful in the case of duplicates)

int ambiguity(char *folder, char *find_file, char *path, int linkctr[], int foundfiledepth[])
{
    int success = EXIT_SUCCESS;
    struct dirent *de; // a pointer for the entries within a directory

    DIR *dr;

    dr = opendir(path); // open the passed folder

    if (dr == NULL) // if you could not open that folder due to incorrect permissions
    {
        printf("\n\nCould not open the current directory - %s.\n\n", folder);
        return EXIT_PERMISSION_DENIED;
    }

    int num_sub_direc = 0;

    int linkc = linkctr[0] + 1;

    while ((de = readdir(dr)) != NULL)
    {
        // if it is a directory (that is not essentially a relative path of . or ..),
        // 1. check if that folder matches the folder to be found
        // 2. if not, then traverse it.
        // 3. also if the number of subdirectories within a directory are greater than 1:
        // then you've encountered an ambiguous node and exit code of value 3 is returned.
        if (de->d_type == DT_DIR && strcmp(de->d_name, ".") && strcmp(de->d_name, ".."))
        {
            num_sub_direc = num_sub_direc + 1;

            linkctr[0] = linkctr[0] + 1; //increment the linkcount

            if (num_sub_direc > 1)
            {
                char newpath[300] = "";
                strcat(newpath, path);
                strcat(newpath, "/");
                strcat(newpath, de->d_name);

                printf("\n---------------------------------------------------------------------------------");
                printf("\nPATH: ");
                printf("\n---------------------------------------------------------------------------------\n");
                fprintf(stderr, "ambiguous: %s", newpath);
                printf("\n---------------------------------------------------------------------------------\n");
                return EXIT_AMBIGUOUS_NODE;
            }

            char newpath[300] = "";
            strcat(newpath, path);
            strcat(newpath, "/");
            strcat(newpath, de->d_name);

            success = ambiguity(de->d_name, find_file, newpath, linkctr, foundfiledepth);
        }
        // else, if it is a file or symbolic link,
        // check if it matches the file to be found
        // also, check for duplicates by updating the value of foundfiledepth based on the linkcount of the shallowest match.
        else if (de->d_type == DT_REG)
        {
            if (!strcmp(de->d_name, find_file) && linkc < *foundfiledepth)
            {
                *foundfiledepth = linkc;
            }
        }
    }

    return success;
}