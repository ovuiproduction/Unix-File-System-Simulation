#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NAME 256
#define MAX_CONTENT 1024
#define MAX_PATH_LENGTH 4096
#define MAX_QUEUE 1000

typedef enum
{
    TYPE_FILE = 0,
    TYPE_FOLDER = 1
} NodeType;

typedef struct FileNode
{
    char fileName[MAX_NAME];
    char fileContent[MAX_CONTENT];
    NodeType type;
    time_t createdTime;
    time_t modifiedTime;
    struct FileNode *parent;
    struct FileNode *fChild;
    struct FileNode *nSibling;
} FileNode;

// Function prototypes
FileNode *createNode(const char *name, const char *content, NodeType type);
FileNode *findNode(FileNode *root, const char *name);
FileNode *findParent(FileNode *parentFolder, const char *node);
int insertNode(FileNode *parent, FileNode *newNode);
int deleteNode(FileNode *root, FileNode *parent, const char *name);
void freeTree(FileNode *node);
void listDirectory(FileNode *node, int showDetails);
void printPath(FileNode *node);
void displayHelp(void);
char *getCurrentTime(time_t t);
int isValidName(const char *name);

// Create a new file/folder node
FileNode *createNode(const char *name, const char *content, NodeType type)
{
    if (!isValidName(name))
    {
        printf("Error: Invalid name '%s'\n", name);
        return NULL;
    }

    FileNode *node = (FileNode *)malloc(sizeof(FileNode));
    if (!node)
    {
        printf("Error: Memory allocation failed\n");
        return NULL;
    }

    strncpy(node->fileName, name, MAX_NAME - 1);
    node->fileName[MAX_NAME - 1] = '\0';

    if (content)
    {
        strncpy(node->fileContent, content, MAX_CONTENT - 1);
        node->fileContent[MAX_CONTENT - 1] = '\0';
    }
    else
    {
        node->fileContent[0] = '\0';
    }

    node->type = type;
    node->createdTime = time(NULL);
    node->modifiedTime = node->createdTime;
    node->parent = NULL;
    node->fChild = NULL;
    node->nSibling = NULL;

    return node;
}

// Validate filename
int isValidName(const char *name)
{
    if (!name || strlen(name) == 0 || strlen(name) >= MAX_NAME)
    {
        return 0;
    }

    // Check for invalid characters
    const char *invalid = "/\\:*?\"<>|";
    for (int i = 0; name[i]; i++)
    {
        if (strchr(invalid, name[i]))
        {
            return 0;
        }
    }

    // Check for reserved names
    if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
    {
        return 0;
    }

    return 1;
}

// Find node by name using BFS
FileNode *findNode(FileNode *root, const char *name)
{
    if (!root || !name)
        return NULL;

    FileNode *queue[MAX_QUEUE];
    int front = 0, rear = 0;

    queue[rear++] = root;

    while (front < rear)
    {
        FileNode *curr = queue[front++];

        if (strcmp(curr->fileName, name) == 0)
        {
            return curr;
        }

        // Add first child
        if (curr->fChild)
        {
            if (rear < MAX_QUEUE)
                queue[rear++] = curr->fChild;
        }

        // Add all siblings
        FileNode *sibling = curr->nSibling;
        while (sibling)
        {
            if (rear < MAX_QUEUE)
                queue[rear++] = sibling;
            sibling = sibling->nSibling;
        }
    }

    return NULL;
}

// Find parent of a specific node
FileNode *findParent(FileNode *parentFolder, const char *name)
{
    if (!parentFolder || !name)
        return NULL;

    // Check direct children
    FileNode *child = parentFolder->fChild;
    FileNode *prev = NULL;

    while (child)
    {
        if (strcmp(child->fileName, name) == 0)
        {
            return prev ? prev : parentFolder;
        }
        prev = child;
        child = child->nSibling;
    }

    return NULL;
}

// Insert node into parent directory
int insertNode(FileNode *parent, FileNode *newNode)
{
    if (!parent || !newNode)
        return 0;

    if (parent->type != TYPE_FOLDER)
    {
        printf("Error: '%s' is not a directory\n", parent->fileName);
        return 0;
    }

    // Check for duplicate names
    FileNode *child = parent->fChild;
    while (child)
    {
        if (strcmp(child->fileName, newNode->fileName) == 0)
        {
            printf("Error: '%s' already exists\n", newNode->fileName);
            return 0;
        }
        child = child->nSibling;
    }

    newNode->parent = parent;

    if (!parent->fChild)
    {
        parent->fChild = newNode;
    }
    else
    {
        FileNode *temp = parent->fChild;
        while (temp->nSibling)
        {
            temp = temp->nSibling;
        }
        temp->nSibling = newNode;
    }

    parent->modifiedTime = time(NULL);
    return 1;
}

// Delete node from tree
int deleteNode(FileNode *root, FileNode *parent, const char *name)
{
    if (!parent || !name)
        return 0;

    FileNode *child = parent->fChild;
    FileNode *prev = NULL;

    while (child)
    {
        if (strcmp(child->fileName, name) == 0)
        {
            // Check if directory is empty
            if (child->type == TYPE_FOLDER && child->fChild)
            {
                printf("Error: Directory '%s' is not empty\n", name);
                return 0;
            }

            // Remove from linked list
            if (prev)
            {
                prev->nSibling = child->nSibling;
            }
            else
            {
                parent->fChild = child->nSibling;
            }

            free(child);
            parent->modifiedTime = time(NULL);
            return 1;
        }
        prev = child;
        child = child->nSibling;
    }

    printf("Error: '%s' not found\n", name);
    return 0;
}

// Free entire tree
void freeTree(FileNode *node)
{
    if (!node)
        return;

    // Free all children recursively
    FileNode *child = node->fChild;
    while (child)
    {
        FileNode *next = child->nSibling;
        freeTree(child);
        child = next;
    }

    free(node);
}

// List directory contents
void listDirectory(FileNode *node, int showDetails)
{
    if (!node)
        return;

    FileNode *child = node->fChild;

    if (!child)
    {
        return;
    }

    while (child)
    {
        if (showDetails)
        {
            char typeChar = (child->type == TYPE_FOLDER) ? 'd' : '-';
            char *modTime = getCurrentTime(child->modifiedTime);
            printf("%c  %s  %s\n", typeChar, modTime, child->fileName);
            free(modTime);
        }
        else
        {
            if (child->type == TYPE_FOLDER)
            {
                printf("%s/\n", child->fileName);
            }
            else
            {
                printf("%s\n", child->fileName);
            }
        }
        child = child->nSibling;
    }
}

// Print full path from root
void printPath(FileNode *node)
{
    if (!node)
        return;

    char path[MAX_PATH_LENGTH] = "";
    FileNode *stack[100];
    int top = -1;

    // Build stack of nodes from current to root
    FileNode *temp = node;
    while (temp)
    {
        stack[++top] = temp;
        temp = temp->parent;
    }

    // Print path from root to current
    while (top >= 0)
    {
        strcat(path, stack[top]->fileName);
        if (top > 0)
            strcat(path, "/");
        top--;
    }

    printf("%s\n", path);
}

// Get formatted time string
char *getCurrentTime(time_t t)
{
    char *timeStr = (char *)malloc(20);
    struct tm *tm_info = localtime(&t);
    strftime(timeStr, 20, "%b %d %H:%M", tm_info);
    return timeStr;
}

// Display help information
void displayHelp(void)
{
    printf("\n=== File System Commands ===\n");
    printf("  man              - Display this help message\n");
    printf("  ls [-l]          - List directory contents (-l for details)\n");
    printf("  pwd              - Print working directory\n");
    printf("  cd <dir>         - Change directory\n");
    printf("  mkdir <name>     - Create directory\n");
    printf("  touch <name>     - Create file\n");
    printf("  rm <name>        - Remove file/empty directory\n");
    printf("  cat <file>       - Display file content\n");
    printf("  echo > <file>    - Write to file\n");
    printf("  cp <src> <dst>   - Copy file\n");
    printf("  mv <src> <dst>   - Move file/directory\n");
    printf("  rename <old> <new> - Rename file/directory\n");
    printf("  find <name>      - Find file/directory path\n");
    printf("  tree             - Display directory tree\n");
    printf("  clear            - Clear screen\n");
    printf("  exit             - Exit program\n");
    printf("============================\n\n");
}

// Display tree structure
void displayTree(FileNode *node, int depth)
{
    if (!node)
        return;

    for (int i = 0; i < depth; i++)
    {
        printf("  ");
    }

    if (node->type == TYPE_FOLDER)
    {
        printf("[DIR] %s/\n", node->fileName);
    }
    else
    {
        printf("     %s\n", node->fileName);
    }

    FileNode *child = node->fChild;
    while (child)
    {
        displayTree(child, depth + 1);
        child = child->nSibling;
    }
}

int main(void)
{
    // Create root directory
    FileNode *root = createNode("root", "", TYPE_FOLDER);
    if (!root)
    {
        printf("Failed to create root directory\n");
        return 1;
    }

    FileNode *current = root;
    char prompt[MAX_PATH_LENGTH];
    char input[MAX_PATH_LENGTH];

    printf("Welcome to Enhanced File System Simulator\n");
    printf("Type 'man' for help, 'exit' to quit\n\n");

    while (1)
    {
        // Build prompt
        snprintf(prompt, MAX_PATH_LENGTH, "user@filesystem:~/%s$ ", current->fileName);
        printf("%s", prompt);
        fflush(stdout);

        if (!fgets(input, MAX_PATH_LENGTH, stdin))
        {
            break;
        }

        // Remove newline
        input[strcspn(input, "\n")] = 0;

        if (strlen(input) == 0)
            continue;

        char *cmd = strtok(input, " ");

        if (strcmp(cmd, "exit") == 0)
        {
            break;
        }
        else if (strcmp(cmd, "man") == 0 || strcmp(cmd, "help") == 0)
        {
            displayHelp();
        }
        else if (strcmp(cmd, "clear") == 0)
        {
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
        }
        else if (strcmp(cmd, "ls") == 0)
        {
            char *flag = strtok(NULL, " ");
            listDirectory(current, flag && strcmp(flag, "-l") == 0);
        }
        else if (strcmp(cmd, "pwd") == 0)
        {
            printPath(current);
        }
        else if (strcmp(cmd, "cd") == 0)
        {
            char *dir = strtok(NULL, " ");
            if (!dir)
            {
                printf("Usage: cd <directory>\n");
            }
            else if (strcmp(dir, "..") == 0)
            {
                if (current->parent)
                {
                    current = current->parent;
                }
            }
            else if (strcmp(dir, "~") == 0 || strcmp(dir, "/") == 0)
            {
                current = root;
            }
            else
            {
                FileNode *target = NULL;
                FileNode *child = current->fChild;
                while (child)
                {
                    if (strcmp(child->fileName, dir) == 0)
                    {
                        target = child;
                        break;
                    }
                    child = child->nSibling;
                }

                if (target && target->type == TYPE_FOLDER)
                {
                    current = target;
                }
                else if (target)
                {
                    printf("Error: '%s' is not a directory\n", dir);
                }
                else
                {
                    printf("Error: '%s' not found\n", dir);
                }
            }
        }
        else if (strcmp(cmd, "mkdir") == 0)
        {
            char *name = strtok(NULL, " ");
            if (!name)
            {
                printf("Usage: mkdir <directory_name>\n");
            }
            else
            {
                FileNode *newDir = createNode(name, "", TYPE_FOLDER);
                if (newDir)
                {
                    if (!insertNode(current, newDir))
                    {
                        free(newDir);
                    }
                }
            }
        }
        else if (strcmp(cmd, "touch") == 0)
        {
            char *name = strtok(NULL, " ");
            if (!name)
            {
                printf("Usage: touch <filename>\n");
            }
            else
            {
                FileNode *newFile = createNode(name, "", TYPE_FILE);
                if (newFile)
                {
                    if (!insertNode(current, newFile))
                    {
                        free(newFile);
                    }
                }
            }
        }
        else if (strcmp(cmd, "rm") == 0)
        {
            char *name = strtok(NULL, " ");
            if (!name)
            {
                printf("Usage: rm <name>\n");
            }
            else
            {
                deleteNode(root, current, name);
            }
        }
        else if (strcmp(cmd, "cat") == 0)
        {
            char *name = strtok(NULL, " ");
            if (!name)
            {
                printf("Usage: cat <filename>\n");
            }
            else
            {
                FileNode *child = current->fChild;
                while (child)
                {
                    if (strcmp(child->fileName, name) == 0)
                    {
                        if (child->type == TYPE_FILE)
                        {
                            printf("%s\n", child->fileContent);
                        }
                        else
                        {
                            printf("Error: '%s' is a directory\n", name);
                        }
                        break;
                    }
                    child = child->nSibling;
                }
                if (!child)
                {
                    printf("Error: '%s' not found\n", name);
                }
            }
        }
        else if (strcmp(cmd, "echo") == 0)
        {
            char *arrow = strtok(NULL, " ");
            if (arrow && strcmp(arrow, ">") == 0)
            {
                char *name = strtok(NULL, " ");
                if (!name)
                {
                    printf("Usage: echo > <filename>\n");
                }
                else
                {
                    printf("Enter content (press Enter to finish):\n");
                    char content[MAX_CONTENT];
                    if (fgets(content, MAX_CONTENT, stdin))
                    {
                        content[strcspn(content, "\n")] = 0;

                        FileNode *child = current->fChild;
                        while (child)
                        {
                            if (strcmp(child->fileName, name) == 0)
                            {
                                if (child->type == TYPE_FILE)
                                {
                                    strncpy(child->fileContent, content, MAX_CONTENT - 1);
                                    child->modifiedTime = time(NULL);
                                }
                                else
                                {
                                    printf("Error: '%s' is a directory\n", name);
                                }
                                break;
                            }
                            child = child->nSibling;
                        }

                        if (!child)
                        {
                            FileNode *newFile = createNode(name, content, TYPE_FILE);
                            if (newFile)
                            {
                                if (!insertNode(current, newFile))
                                {
                                    free(newFile);
                                }
                            }
                        }
                    }
                }
            }
            else
            {
                printf("Usage: echo > <filename>\n");
            }
        }
        else if (strcmp(cmd, "find") == 0)
        {
            char *name = strtok(NULL, " ");
            if (!name)
            {
                printf("Usage: find <name>\n");
            }
            else
            {
                FileNode *found = findNode(root, name);
                if (found)
                {
                    printPath(found);
                }
                else
                {
                    printf("'%s' not found\n", name);
                }
            }
        }
        else if (strcmp(cmd, "tree") == 0)
        {
            displayTree(current, 0);
        }
        else if (strcmp(cmd, "rename") == 0)
        {
            char *oldName = strtok(NULL, " ");
            char *newName = strtok(NULL, " ");
            if (!oldName || !newName)
            {
                printf("Usage: rename <old_name> <new_name>\n");
            }
            else if (!isValidName(newName))
            {
                printf("Error: Invalid name '%s'\n", newName);
            }
            else
            {
                FileNode *child = current->fChild;
                while (child)
                {
                    if (strcmp(child->fileName, oldName) == 0)
                    {
                        strncpy(child->fileName, newName, MAX_NAME - 1);
                        child->modifiedTime = time(NULL);
                        break;
                    }
                    child = child->nSibling;
                }
                if (!child)
                {
                    printf("Error: '%s' not found\n", oldName);
                }
            }
        }
        else if (strcmp(cmd, "mv") == 0)
        {
            char *src = strtok(NULL, " ");
            char *dst = strtok(NULL, " ");
            if (!src || !dst)
            {
                printf("Usage: mv <source> <destination>\n");
            }
            else
            {
                FileNode *srcNode = findNode(root, src);
                FileNode *dstNode = findNode(root, dst);

                if (!srcNode)
                {
                    printf("Error: '%s' not found\n", src);
                }
                else if (!dstNode || dstNode->type != TYPE_FOLDER)
                {
                    printf("Error: '%s' is not a valid directory\n", dst);
                }
                else
                {
                    // Remove from old location
                    FileNode *srcParent = srcNode->parent;
                    if (srcParent)
                    {
                        FileNode *child = srcParent->fChild;
                        FileNode *prev = NULL;

                        while (child)
                        {
                            if (child == srcNode)
                            {
                                if (prev)
                                {
                                    prev->nSibling = child->nSibling;
                                }
                                else
                                {
                                    srcParent->fChild = child->nSibling;
                                }
                                child->nSibling = NULL;
                                break;
                            }
                            prev = child;
                            child = child->nSibling;
                        }
                    }

                    // Insert in new location
                    insertNode(dstNode, srcNode);
                }
            }
        }
        else if (strcmp(cmd, "cp") == 0)
        {
            char *src = strtok(NULL, " ");
            char *dst = strtok(NULL, " ");
            if (!src || !dst)
            {
                printf("Usage: cp <source> <destination>\n");
            }
            else
            {
                FileNode *child = current->fChild;
                while (child)
                {
                    if (strcmp(child->fileName, src) == 0)
                    {
                        if (child->type == TYPE_FILE)
                        {
                            FileNode *copy = createNode(dst, child->fileContent, TYPE_FILE);
                            if (copy)
                            {
                                if (!insertNode(current, copy))
                                {
                                    free(copy);
                                }
                            }
                        }
                        else
                        {
                            printf("Error: Cannot copy directories\n");
                        }
                        break;
                    }
                    child = child->nSibling;
                }
                if (!child)
                {
                    printf("Error: '%s' not found\n", src);
                }
            }
        }
        else
        {
            printf("Command not found: %s\n", cmd);
            printf("Type 'man' for help\n");
        }
    }

    printf("\nCleaning up...\n");
    freeTree(root);
    printf("Goodbye!\n");

    return 0;
}