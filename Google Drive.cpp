#include <iostream>
#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>
using namespace std;

//  User Graph System 
const int MAX_USERS = 10;

enum Role { ADMIN = 1, EDITOR = 2, VIEWER = 3 };

struct User
{
    string username;
    string password;
    string securityQuestion;
    string logoutTime;
    Role role = VIEWER;
};

class Graph
{
    int** adj;
    int size;

public:
    Graph(int n)
    {
        size = n;
        adj = new int* [n];
        for (int i = 0; i < n; i++)
        {
            adj[i] = new int[n];
            for (int j = 0; j < n; j++)
            {
                adj[i][j] = 0;
            }
        }
    }
    ~Graph()
    {
        for (int i = 0; i < size; i++)
        {
            delete[] adj[i];
        }
        delete[] adj;
    }

    void addEdge(int u, int v)
    {
        adj[u][v] = 1; // Directed edge: permission granted
    }

    void removeEdge(int u, int v)
    {
        adj[u][v] = 0;
    }

    void displayGraph(const User users[], int userCount) const
    {
        bool anyConnection = false;
        for (int i = 0; i < userCount; i++)
        {
            for (int j = 0; j < userCount; j++)
            {
                if (adj[i][j])
                {
                    cout << users[i].username << " (" << getRoleName(users[i].role) << ") --> "
                        << users[j].username << " (" << getRoleName(users[j].role) << ")\n";
                    anyConnection = true;
                }
            }
        }
        if (!anyConnection)
        {
            cout << "No sharing connections found." << endl;
        }
    }

    string getRoleName(Role role) const
    {
        switch (role)
        {
        case ADMIN:
            return "Admin";
        case EDITOR:
            return "Editor";
        case VIEWER:
            return "Viewer";
        default:
            return "Unknown";
        }
    }
};

class UserSystem
{
public:
    User users[MAX_USERS];
    int userCount;
    Graph userGraph;
    UserSystem() : userGraph(MAX_USERS)
    {
        userCount = 0;
    }

    int findUserIndex(const string& uname) const
    {
        for (int i = 0; i < userCount; i++)
        {
            if (users[i].username == uname)
                return i;
        }
        return -1;
    }

    void addUser(string uname, string pass, string secQ, Role role)
    {
        try
        {
            if (userCount >= MAX_USERS)
                throw runtime_error("User limit reached.");

            if (findUserIndex(uname) != -1)
                throw invalid_argument("Username already exists.");

            users[userCount++] = { uname, pass, secQ, "", role };
            cout << "User added successfully with role: " << userGraph.getRoleName(role) << "\n";
        }
        catch (const exception& e)
        {
            cout << "Error adding user: " << e.what() << endl;
        }
    }

    bool login(string uname, string pass)
    {
        try
        {
            int idx = findUserIndex(uname);
            if (idx == -1 || users[idx].password != pass)
                throw invalid_argument("Invalid username or password.");

            cout << "Login successful.\n";
            return true;
        }
        catch (const exception& e)
        {
            cout << "Login failed: " << e.what() << endl;
            return false;
        }
    }

    void logout(string uname, string time)
    {
        int idx = findUserIndex(uname);
        if (idx != -1)
        {
            users[idx].logoutTime = time;
            cout << "User logged out at " << time << endl;
        }
    }

    void forgotPassword(string uname, string ans)
    {
        int idx = findUserIndex(uname);
        if (idx != -1)
        {
            if (users[idx].securityQuestion == ans)
            {
                cout << "Enter new password: ";
                string newpass;
                cin >> newpass;
                users[idx].password = newpass;
                cout << "Password updated.\n";
            }
            else
            {
                cout << "Security answer incorrect.\n";
            }
        }
        else
        {
            cout << "Username not found.\n";
        }
    }

    void shareFile(string from, string to)
    {
        try
        {
            int u = findUserIndex(from);
            int v = findUserIndex(to);
            if (u == -1 || v == -1)
                throw invalid_argument("One or both users not found.");

            userGraph.addEdge(u, v);
            cout << "File shared from " << from << " to " << to << endl;
        }
        catch (const exception& e)
        {
            cout << "Error in file sharing: " << e.what() << endl;
        }
    }

    void unshareFile(string from, string to)
    {
        int u = findUserIndex(from);
        int v = findUserIndex(to);
        if (u != -1 && v != -1)
        {
            userGraph.removeEdge(u, v);
            cout << "File unshared from " << from << " to " << to << endl;
        }
        else
        {
            cout << "User not found.\n";
        }
    }

    void showSharingAccess(string uname) const
    {
        int idx = findUserIndex(uname);
        if (idx == -1)
        {
            cout << "User not found.\n";
            return;
        }
        cout << "\nSharing connections for user: " << uname << endl;
        userGraph.displayGraph(users, userCount);
    }

    void displayAllUsers() const
    {
        if (userCount == 0)
        {
            cout << "No users in the system.\n";
            return;
        }
        cout << "\nAll Users in the System:\n";
        cout << "------------------------\n";
        for (int i = 0; i < userCount; i++)
        {
            cout << (i + 1) << ". " << users[i].username << " (" << userGraph.getRoleName(users[i].role) << ")\n";
        }
    }

    Role getCurrentUserRole(const string& uname) const
    {
        int idx = findUserIndex(uname);
        if (idx != -1) 
            return users[idx].role;
        return VIEWER; // Default fallback
    }
};

struct File
{
    string name;
    string content;
    File(string n, string c)
    {
        name = n;
        content = c;
    }
};

const int MAX_RECYCLE = 10;
class RecycleBin
{
    File* bin[MAX_RECYCLE];
    int top;
public:
    RecycleBin()
    {
        top = -1;
        for (int i = 0; i < MAX_RECYCLE; i++)
        {
            bin[i] = nullptr;
        }
    }
    ~RecycleBin()
    {
        emptyRecycleBin();
    }

    bool isEmpty() const
    {
        return top == -1;
    }

    bool isFull() const
    {
        return top == MAX_RECYCLE - 1;
    }

    void push(File* file)
    {
        try
        {
            if (!file)
                throw invalid_argument("Cannot push a null file.");

            if (isFull())
            {
                cout << "Recycle Bin full. Deleting oldest file.\n";
                delete bin[0];
                for (int i = 0; i < top; i++)
                    bin[i] = bin[i + 1];
                top--;
            }
            bin[++top] = file;
        }
        catch (const exception& e)
        {
            cout << "Recycle Bin Error: " << e.what() << endl;
        }
    }

    File* pop()
    {
        if (isEmpty())
        {
            cout << "Recycle Bin is empty.\n";
            return nullptr;
        }
        File* file = bin[top];
        top--;
        cout << "Restoring file '" << file->name << "' from Recycle Bin.\n";
        return file;
    }

    File* restoreFileByName(const string& filename)
    {
        if (isEmpty())
        {
            cout << "Recycle Bin is empty.\n";
            return nullptr;
        }
        for (int i = 0; i <= top; i++)
        {
            if (bin[i]->name == filename)
            {
                File* file = bin[i];
                for (int j = i; j < top; j++)
                {
                    bin[j] = bin[j + 1];
                }
                bin[top] = nullptr;
                top--;
                cout << "Restoring file '" << file->name << "' from Recycle Bin.\n";
                return file;
            }
        }
        cout << "File '" << filename << "' not found in Recycle Bin.\n";
        return nullptr;
    }

    void viewRecycleBin() const
    {
        if (isEmpty())
        {
            cout << "Recycle Bin is empty.\n";
            return;
        }
        cout << "Files in Recycle Bin:\n";
        for (int i = 0; i <= top; i++)
        {
            cout << (i + 1) << ". " << bin[i]->name << endl;
        }
    }

    void emptyRecycleBin()
    {
        if (isEmpty())
        {
            cout << "Recycle Bin is already empty.\n";
            return;
        }
        for (int i = 0; i <= top; i++)
        {
            if (bin[i] != nullptr)
            {
                delete bin[i];
                bin[i] = nullptr;
            }
        }
        top = -1;
        cout << "Recycle Bin emptied successfully.\n";
    }
};

const int MAX_RECENT = 5;
class RecentFiles
{
    File* recent[MAX_RECENT];
    int front, rear, size;
public:
    RecentFiles()
    {
        front = 0;
        rear = -1;
        size = 0;
        for (int i = 0; i < MAX_RECENT; i++)
        {
            recent[i] = nullptr;
        }
    }
    ~RecentFiles()
    {
        for (int i = 0; i < MAX_RECENT; i++)
        {
            recent[i] = nullptr;
        }
    }

    void accessFile(File* file)
    {
        if (size == MAX_RECENT)
        {
            front = (front + 1) % MAX_RECENT;
            size--;
        }
        rear = (rear + 1) % MAX_RECENT;
        recent[rear] = file;
        size++;
        cout << "Accessed file: " << file->name << endl;
    }

    void viewRecentFiles() const
    {
        if (size == 0)
        {
            cout << "No recent files accessed.\n";
            return;
        }
        cout << "Recently Accessed Files:\n";
        for (int i = 0; i < size; i++)
        {
            int index = (front + i) % MAX_RECENT;
            if (recent[index] != nullptr)
            {
                cout << (i + 1) << ". " << recent[index]->name << endl;
            }
        }
    }
};

struct VersionNode
{
    int versionNumber;
    string content;
    VersionNode* next;
    VersionNode* prev;
    string timestamp;
    VersionNode(int vNum, const string& text)
    {
        versionNumber = vNum;
        content = text;
        next = nullptr;
        prev = nullptr;
        time_t now = time(0);
        tm ltm;
        localtime_s(&ltm, &now); // Fixed unsafe localtime call
        stringstream ss;
        ss << 1900 + ltm.tm_year << "-"
            << setw(2) << setfill('0') << 1 + ltm.tm_mon << "-"
            << setw(2) << setfill('0') << ltm.tm_mday << " "
            << setw(2) << setfill('0') << ltm.tm_hour << ":"
            << setw(2) << setfill('0') << ltm.tm_min << ":"
            << setw(2) << setfill('0') << ltm.tm_sec;
        timestamp = ss.str();
    }
};

class FileVersioning
{
private:
    VersionNode* head;
    VersionNode* currentVersion;
    int versionCounter;
public:
    FileVersioning()
    {
        head = nullptr;
        currentVersion = nullptr;
        versionCounter = 0;
    }
    ~FileVersioning()
    {
        VersionNode* temp = head;
        while (temp != nullptr)
        {
            VersionNode* next = temp->next;
            delete temp;
            temp = next;
        }
    }

    void addVersion(const string& content)
    {
        versionCounter++;
        VersionNode* newNode = new VersionNode(versionCounter, content);
        if (head == nullptr)
        {
            head = newNode;
        }
        else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
        currentVersion = newNode;
        cout << "Added version " << versionCounter << " at " << newNode->timestamp << "\n";
    }

    void rollbackToVersion(int versionNumber)
    {
        VersionNode* temp = head;
        while (temp != nullptr && temp->versionNumber != versionNumber)
        {
            temp = temp->next;
        }
        if (temp == nullptr)
        {
            cout << "Version " << versionNumber << " not found.\n";
            return;
        }
        currentVersion = temp;
        cout << "Rolled back to version " << versionNumber << " from " << temp->timestamp << "\n";
    }

    void viewHistory() const
    {
        if (head == nullptr)
        {
            cout << "No version history available.\n";
            return;
        }
        VersionNode* temp = head;
        cout << "\n----- File Version History -----\n";
        while (temp != nullptr)
        {
            cout << "Version " << temp->versionNumber;
            if (temp == currentVersion)
                cout << " (Current)";
            cout << " - " << temp->timestamp << "\n";
            cout << "Content: " << temp->content << "\n";
            cout << "----------------------------\n";
            temp = temp->next;
        }
    }

    string getLatestContent() const
    {
        if (currentVersion != nullptr)
            return currentVersion->content;
        else
            return "";
    }

    int getCurrentVersionNumber() const
    {
        if (currentVersion != nullptr)
            return currentVersion->versionNumber;
        else
            return 0;
    }
};

struct treenode
{
    string name;
    treenode* firstchild;
    treenode* nextsibling;
    treenode* parent;
    FileVersioning* fileVersion;
    bool isFolder;
    treenode(string n, bool isDir = true)
    {
        name = n;
        firstchild = nullptr;
        nextsibling = nullptr;
        parent = nullptr;
        fileVersion = nullptr;
        isFolder = isDir;
    }
    ~treenode()
    {
        if (firstchild != nullptr)
        {
            delete firstchild;
        }
        if (fileVersion != nullptr)
        {
            delete fileVersion;
        }
    }
};

class AVLTree
{
private:
    struct AVLNode
    {
        string key;
        int height;
        AVLNode* left;
        AVLNode* right;

        AVLNode(string k) : key(k), height(1), left(nullptr), right(nullptr) {}
    };

    AVLNode* root;

    int height(AVLNode* N)
    {
        if (N == nullptr)
            return 0;
        return N->height;
    }

    int getBalance(AVLNode* N)
    {
        if (N == nullptr)
            return 0;
        return height(N->left) - height(N->right);
    }

    AVLNode* rightRotate(AVLNode* y)
    {
        AVLNode* x = y->left;
        AVLNode* T2 = x->right;

        x->right = y;
        y->left = T2;

        y->height = max(height(y->left), height(y->right)) + 1;
        x->height = max(height(x->left), height(x->right)) + 1;

        return x;
    }

    AVLNode* leftRotate(AVLNode* x)
    {
        AVLNode* y = x->right;
        AVLNode* T2 = y->left;

        y->left = x;
        x->right = T2;

        x->height = max(height(x->left), height(x->right)) + 1;
        y->height = max(height(y->left), height(y->right)) + 1;

        return y;
    }

    AVLNode* insert(AVLNode* node, string key)
    {
        if (node == nullptr)
            return new AVLNode(key);

        if (key < node->key)
            node->left = insert(node->left, key);
        else if (key > node->key)
            node->right = insert(node->right, key);
        else
            return node; // Equal keys not allowed

        node->height = 1 + max(height(node->left), height(node->right));

        int balance = getBalance(node);

        // Left Left Case
        if (balance > 1 && key < node->left->key)
            return rightRotate(node);

        // Right Right Case
        if (balance < -1 && key > node->right->key)
            return leftRotate(node);

        // Left Right Case
        if (balance > 1 && key > node->left->key)
        {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        // Right Left Case
        if (balance < -1 && key < node->right->key)
        {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    void inOrder(AVLNode* node)
    {
        if (node != nullptr)
        {
            inOrder(node->left);
            cout << node->key << " ";
            inOrder(node->right);
        }
    }

    AVLNode* search(AVLNode* root, string key)
    {
        if (root == nullptr || root->key == key)
            return root;

        if (root->key < key)
            return search(root->right, key);

        return search(root->left, key);
    }

    void cleanup(AVLNode* node)
    {
        if (node != nullptr)
        {
            cleanup(node->left);
            cleanup(node->right);
            delete node;
        }
    }

public:
    AVLTree() : root(nullptr) {}

    ~AVLTree()
    {
        cleanup(root);
    }

    void insert(string key)
    {
        root = insert(root, key);
        cout << "Added '" << key << "' to optimized index.\n";
    }

    bool search(string key)
    {
        AVLNode* result = search(root, key);
        return result != nullptr;
    }

    void display()
    {
        cout << "AVL Tree (Balanced Index): ";
        inOrder(root);
        cout << endl;
    }
};

void garbageCollection()
{
    cout << "Running garbage collection to optimize file system..." << endl;
    cout << "Garbage collection completed. File system optimized." << endl;
}

class Folder
{
public:
    treenode* root;
    treenode* currentfolder;
    Folder(string rootName)
    {
        root = new treenode(rootName);
        currentfolder = root;
    }
    ~Folder()
    {
        delete root;
    }

    bool isEmpty() const
    {
        return root == nullptr;
    }

    void optimizeStructure()
    {
        cout << "Optimizing folder structure using AVL balancing...\n";
        AVLTree optimizedIndex;

        // we'll add current folder contents to AVL tree
        treenode* child = currentfolder->firstchild;
        while (child != nullptr)
        {
            optimizedIndex.insert(child->name);
            child = child->nextsibling;
        }

        optimizedIndex.display();
        garbageCollection();
        cout << "Folder structure optimization complete.\n";
    }

    void createFolder(string foldername)
    {
        treenode* existing = findChildByName(currentfolder, foldername);
        if (existing != nullptr && existing->isFolder)
        {
            cout << "Folder '" << foldername << "' already exists in current directory." << endl;
            return;
        }
        treenode* newfolder = new treenode(foldername, true);
        newfolder->parent = currentfolder;
        if (currentfolder->firstchild == nullptr)
        {
            currentfolder->firstchild = newfolder;
        }
        else
        {
            treenode* sibling = currentfolder->firstchild;
            while (sibling->nextsibling != nullptr)
            {
                sibling = sibling->nextsibling;
            }
            sibling->nextsibling = newfolder;
        }
        cout << "Folder '" << foldername << "' created successfully." << endl;
    }

    void createFile(string filename, const string& content)
    {
        treenode* existing = findChildByName(currentfolder, filename);
        if (existing != nullptr && !existing->isFolder)
        {
            cout << "File '" << filename << "' already exists in current directory." << endl;
            cout << "Would you like to update its content? If yes then enter(Y/y), otherwise anything: ";
            char choice;
            cin >> choice;
            cin.ignore();
            if (choice == 'y' || choice == 'Y')
            {
                updateFile(filename, content);
                return;
            }
            else
            {
                return;
            }
        }
        treenode* newfile = new treenode(filename, false);
        newfile->fileVersion = new FileVersioning();
        newfile->fileVersion->addVersion(content);
        newfile->parent = currentfolder;
        if (currentfolder->firstchild == nullptr)
        {
            currentfolder->firstchild = newfile;
        }
        else
        {
            treenode* sibling = currentfolder->firstchild;
            while (sibling->nextsibling != nullptr)
            {
                sibling = sibling->nextsibling;
            }
            sibling->nextsibling = newfile;
        }
        cout << "File '" << filename << "' created successfully." << endl;
    }

    treenode* findChildByName(treenode* parent, string name) const
    {
        treenode* child = parent->firstchild;
        while (child != nullptr)
        {
            if (child->name == name)
            {
                return child;
            }
            child = child->nextsibling;
        }
        return nullptr;
    }

    bool navigateToFolder(string folderName)
    {
        treenode* child = currentfolder->firstchild;
        while (child != nullptr)
        {
            if (child->name == folderName && child->isFolder)
            {
                currentfolder = child;
                cout << "Changed directory to: " << folderName << endl;
                return true;
            }
            child = child->nextsibling;
        }
        cout << "Folder '" << folderName << "' not found." << endl;
        return false;
    }

    bool navigateUp()
    {
        if (currentfolder->parent != nullptr)
        {
            currentfolder = currentfolder->parent;
            cout << "Changed directory to parent folder." << endl;
            return true;
        }
        cout << "Already at root directory." << endl;
        return false;
    }

    bool deleteFolder(string folderName)
    {
        treenode* prev = nullptr;
        treenode* child = currentfolder->firstchild;
        while (child != nullptr)
        {
            if (child->name == folderName && child->isFolder)
            {
                if (prev == nullptr)
                {
                    currentfolder->firstchild = child->nextsibling;
                }
                else
                {
                    prev->nextsibling = child->nextsibling;
                }
                treenode* next = child->nextsibling;
                child->nextsibling = nullptr;
                delete child;
                cout << "Folder '" << folderName << "' deleted successfully." << endl;
                return true;
            }
            prev = child;
            child = child->nextsibling;
        }
        cout << "Folder '" << folderName << "' not found." << endl;
        return false;
    }

    void deleteFile(string filename, RecycleBin& recycle)
    {
        treenode* prev = nullptr;
        treenode* child = currentfolder->firstchild;
        while (child != nullptr)
        {
            if (child->name == filename && !child->isFolder && child->fileVersion != nullptr)
            {
                File* file = new File(child->name, child->fileVersion->getLatestContent());
                recycle.push(file);
                if (prev == nullptr)
                {
                    currentfolder->firstchild = child->nextsibling;
                }
                else
                {
                    prev->nextsibling = child->nextsibling;
                }
                treenode* next = child->nextsibling;
                child->nextsibling = nullptr;
                delete child;
                cout << "File '" << filename << "' deleted and moved to Recycle Bin." << endl;
                return;
            }
            prev = child;
            child = child->nextsibling;
        }
        cout << "File '" << filename << "' not found." << endl;
    }

    void restoreFile(string filename, RecycleBin& recycle)
    {
        File* file = recycle.restoreFileByName(filename);
        if (file != nullptr)
        {
            createFile(file->name, file->content);
            delete file;
        }
    }

    void listCurrent() const
    {
        if (currentfolder == nullptr)
        {
            cout << "Error: No current folder selected." << endl;
            return;
        }

        treenode* child = currentfolder->firstchild;
        if (child == nullptr)
        {
            cout << "Empty folder." << endl;
            return;
        }

        cout << "\nCurrent directory: ";
        string path = "";
        treenode* temp = currentfolder;
        while (temp != nullptr)
        {
            if (temp->parent != nullptr)
            {
                path = temp->name + "/" + path;
            }
            else
            {
                path = temp->name + path;
            }
            temp = temp->parent;
        }
        cout << path << endl;

        cout << "Contents:\n------------------------" << endl;
        int folderCount = 0, fileCount = 0;
        child = currentfolder->firstchild;

        while (child != nullptr)
        {
            if (child->isFolder)
            {
                cout << "[Folder] " << child->name << endl;
                folderCount++;
            }
            child = child->nextsibling;
        }

        child = currentfolder->firstchild;
        while (child != nullptr)
        {
            if (!child->isFolder)
            {
                cout << "[File] " << child->name;
                if (child->fileVersion != nullptr)
                {
                    cout << " (Version: " << child->fileVersion->getCurrentVersionNumber() << ")";
                }
                cout << endl;
                fileCount++;
            }
            child = child->nextsibling;
        }

        cout << "------------------------" << endl;
        cout << folderCount << " folder(s), " << fileCount << " file(s)" << endl;
    }

    void preOrderTraversal(treenode* node, int depth = 0) const
    {
        if (node == nullptr) return;
        for (int i = 0; i < depth; i++)
        {
            cout << "  ";
        }
        if (node->isFolder)
        {
            cout << "[+] " << node->name << "/" << endl;
        }
        else
        {
            cout << "[-] " << node->name << endl;
        }
        treenode* child = node->firstchild;
        while (child != nullptr)
        {
            preOrderTraversal(child, depth + 1);
            child = child->nextsibling;
        }
    }

    void listAllFolders() const
    {
        cout << "\nComplete Directory Structure:\n=============================" << endl;
        preOrderTraversal(root);
        cout << "=============================" << endl;
    }

    void updateFile(string filename, const string& newContent)
    {
        treenode* child = currentfolder->firstchild;
        while (child != nullptr)
        {
            if (child->name == filename && !child->isFolder && child->fileVersion != nullptr)
            {
                child->fileVersion->addVersion(newContent);
                cout << "File '" << filename << "' updated successfully." << endl;
                return;
            }
            child = child->nextsibling;
        }
        cout << "File '" << filename << "' not found in current directory." << endl;
    }

    void viewFileHistory(string filename)
    {
        treenode* child = currentfolder->firstchild;
        while (child != nullptr)
        {
            if (child->name == filename && !child->isFolder && child->fileVersion != nullptr)
            {
                child->fileVersion->viewHistory();
                return;
            }
            child = child->nextsibling;
        }
        cout << "File '" << filename << "' not found in current directory." << endl;
    }

    void rollbackFile(string filename, int versionNumber)
    {
        treenode* child = currentfolder->firstchild;
        while (child != nullptr)
        {
            if (child->name == filename && !child->isFolder && child->fileVersion != nullptr)
            {
                child->fileVersion->rollbackToVersion(versionNumber);
                return;
            }
            child = child->nextsibling;
        }
        cout << "File '" << filename << "' not found in current directory." << endl;
    }

    treenode* accessFile(string filename, RecentFiles& recent)
    {
        treenode* child = currentfolder->firstchild;
        while (child != nullptr)
        {
            if (child->name == filename && !child->isFolder && child->fileVersion != nullptr)
            {
                File* fileObj = new File(child->name, child->fileVersion->getLatestContent());
                recent.accessFile(fileObj);
                return child;
            }
            child = child->nextsibling;
        }
        cout << "File '" << filename << "' not found in current directory." << endl;
        return nullptr;
    }

    string getCurrentPath() const
    {
        string path = "";
        treenode* temp = currentfolder;
        while (temp != nullptr)
        {
            if (temp->parent != nullptr)
            {
                path = temp->name + "/" + path;
            }
            else
            {
                path = temp->name + path;
            }
            temp = temp->parent;
        }
        return path;
    }

    void searchFile(string filename, treenode* node = nullptr) const
    {
        if (node == nullptr)
        {
            node = root;
            cout << "Searching for file '" << filename << "'..." << endl;
        }
        treenode* child = node->firstchild;
        while (child != nullptr)
        {
            if (!child->isFolder && child->name == filename)
            {
                string path = "";
                treenode* temp = child->parent;
                while (temp != nullptr)
                {
                    if (temp->parent != nullptr)
                    {
                        path = temp->name + "/" + path;
                    }
                    else
                    {
                        path = temp->name + path;
                    }
                    temp = temp->parent;
                }
                cout << "Found: " << path << filename << endl;
            }
            if (child->isFolder)
            {
                searchFile(filename, child);
            }
            child = child->nextsibling;
        }
    }
};

class FileCompression
{
public:
    // Run-Length Encoding (RLE)
    static string encodeRLE(const string& input)
    {
        string encoded;
        if (input.empty()) return encoded;

        char currentChar = input[0];
        int count = 1;

        for (int i = 1; i < input.length(); i++)
        {
            if (input[i] == currentChar)
            {
                count++;
            }
            else
            {
                encoded += to_string(count) + currentChar;
                currentChar = input[i];
                count = 1;
            }
        }

        encoded += to_string(count) + currentChar;
        cout << "File compressed using RLE.\n";
        return encoded;
    }

    static string decodeRLE(const string& input)
    {
        string decoded;
        string countStr;

        for (int i = 0; i < input.length(); i++)
        {
            if (isdigit(input[i]))
            {
                countStr += input[i];
            }
            else
            {
                int count = stoi(countStr);
                for (int j = 0; j < count; j++)
                {
                    decoded += input[i];
                }
                countStr = "";
            }
        }

        cout << "File decompressed from RLE.\n";
        return decoded;
    }

    // Dictionary-based compression replacement 
    static string encodeDictionary(const string& input)
    {
        cout << "Dictionary-based compression unavailable. Using RLE instead." << endl;
        return encodeRLE(input);
    }

    static string compressFile(const string& content, bool useRLE = true)
    {
        return encodeRLE(content);
    }
};

class CloudSync
{
private:
    struct SyncTask
    {
        string operation; // "upload", "download", "delete"
        string filename;
        string content;
        string timestamp;

        SyncTask(string op, string file, string cont = "")
            : operation(op), filename(file), content(cont)
        {
            // Set timestamp
            time_t now = time(0);
            tm ltm;
            localtime_s(&ltm, &now);
            stringstream ss;
            ss << 1900 + ltm.tm_year << "-"
                << setw(2) << setfill('0') << 1 + ltm.tm_mon << "-"
                << setw(2) << setfill('0') << ltm.tm_mday << " "
                << setw(2) << setfill('0') << ltm.tm_hour << ":"
                << setw(2) << setfill('0') << ltm.tm_min << ":"
                << setw(2) << setfill('0') << ltm.tm_sec;
            timestamp = ss.str();
        }
    };

    const int MAX_QUEUE_SIZE = 100;
    SyncTask* queue[100];
    int front, rear, count;
    bool isRunning;

public:
    CloudSync() : front(0), rear(-1), count(0), isRunning(false)
    {
        for (int i = 0; i < MAX_QUEUE_SIZE; i++)
        {
            queue[i] = nullptr;
        }
    }

    ~CloudSync()
    {
        for (int i = 0; i < MAX_QUEUE_SIZE; i++)
        {
            if (queue[i] != nullptr)
            {
                delete queue[i];
            }
        }
    }

    void addSyncTask(const string& operation, const string& filename, const string& content = "")
    {
        if (count == MAX_QUEUE_SIZE)
        {
            cout << "Sync queue is full. Waiting for tasks to complete." << endl;
            return;
        }

        rear = (rear + 1) % MAX_QUEUE_SIZE;
        queue[rear] = new SyncTask(operation, filename, content);
        count++;

        cout << "Added " << operation << " task for file '" << filename << "' to sync queue." << endl;

        // Start background sync if not already running
        if (!isRunning)
        {
            startSync();
        }
    }

    void startSync()
    {
        isRunning = true;
        cout << "Background sync started.\n";
        processSyncQueue();
    }

    void processSyncQueue()
    {
        while (count > 0)
        {
            SyncTask* task = queue[front];
            front = (front + 1) % MAX_QUEUE_SIZE;
            count--;

            // Simulate cloud operations
            cout << "Syncing: " << task->operation << " file '" << task->filename
                << "' at " << task->timestamp << endl;

            // Simulate some processing time
            cout << "Sync completed for file '" << task->filename << "'.\n";

            delete task;
        }

        isRunning = false;
        cout << "All sync tasks completed.\n";
    }
};

class FileHashTable
{
private:
    static const int TABLE_SIZE = 100;
    struct FileMetadata
    {
        string name;
        string path;
        string owner;
        string type;
        long size;
        string creationDate;
        FileMetadata* next; // For collision handling

        FileMetadata(string n, string p, string o, string t, long s, string d)
            : name(n), path(p), owner(o), type(t), size(s), creationDate(d), next(nullptr) {
        }

    };

    FileMetadata* table[TABLE_SIZE];

    // Hash function
    int hashFunction(const string& filename) const
    {
        int hash = 0;
        for (int i = 0; i < filename.length(); i++)
        {
            char c = filename[i];
            hash = (hash * 31 + c) % TABLE_SIZE;
        }
        return hash;
    }

public:
    FileHashTable()
    {
        for (int i = 0; i < TABLE_SIZE; i++)
        {
            table[i] = nullptr;
        }
    }

    ~FileHashTable()
    {
        for (int i = 0; i < TABLE_SIZE; i++)
        {
            FileMetadata* current = table[i];
            while (current != nullptr)
            {
                FileMetadata* temp = current;
                current = current->next;
                delete temp;
            }
        }
    }

    void insert(const string& filename, const string& path, const string& owner,
        const string& type, long size, const string& date)
    {
        int index = hashFunction(filename);
        FileMetadata* newFile = new FileMetadata(filename, path, owner, type, size, date);

        if (table[index] == nullptr)
        {
            table[index] = newFile;
        }
        else
        {
            // Handle collision using chaining
            FileMetadata* current = table[index];
            while (current->next != nullptr)
            {
                current = current->next;
            }
            current->next = newFile;
        }
        cout << "File indexed in hash table.\n";
    }

    FileMetadata* lookup(const string& filename) const
    {
        int index = hashFunction(filename);
        FileMetadata* current = table[index];

        while (current != nullptr)
        {
            if (current->name == filename)
            {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }

    void removeFile(const string& filename)
    {
        int index = hashFunction(filename);
        FileMetadata* current = table[index];
        FileMetadata* prev = nullptr;

        while (current != nullptr)
        {
            if (current->name == filename)
            {
                if (prev == nullptr)
                {
                    table[index] = current->next;
                }
                else
                {
                    prev->next = current->next;
                }
                delete current;
                cout << "File removed from index.\n";
                return;
            }
            prev = current;
            current = current->next;
        }
    }

    void displayFileInfo(const string& filename) const
    {
        FileMetadata* file = lookup(filename);
        if (file != nullptr)
        {
            cout << "\nFile Information:\n";
            cout << "Name: " << file->name << endl;
            cout << "Path: " << file->path << endl;
            cout << "Owner: " << file->owner << endl;
            cout << "Type: " << file->type << endl;
            cout << "Size: " << file->size << " bytes" << endl;
            cout << "Created: " << file->creationDate << endl;
        }
        else
        {
            cout << "File not found in index.\n";
        }
    }
};

void showMenu()
{
    cout << "\n--- Folder & File Versioning System Menu ---" << endl;
    cout << "1. Create Folder" << endl;
    cout << "2. Create File" << endl;
    cout << "3. Update File" << endl;
    cout << "4. View File Version History" << endl;
    cout << "5. Rollback File to Specific Version" << endl;
    cout << "6. Navigate to Folder" << endl;
    cout << "7. Navigate Up" << endl;
    cout << "8. List Current Folder Contents" << endl;
    cout << "9. List All Folders" << endl;
    cout << "10. Access File (Add to Recent)" << endl;
    cout << "11. Delete File (Move to Recycle Bin)" << endl;
    cout << "12. View Recent Files" << endl;
    cout << "13. View Recycle Bin" << endl;
    cout << "14. Empty Recycle Bin" << endl;
    cout << "15. Logout" << endl;
    cout << "16. Share File to Another User" << endl;
    cout << "17. View Who Can Access My Files (Graph)" << endl;
    cout << "18. Search File" << endl;
    cout << "19. Restore File from Recycle Bin" << endl;
    cout << "20. Delete Folder" << endl;
    cout << "21. Display All Users" << endl;
    cout << "22. Forgot Password" << endl;
    cout << "23. View File Metadata (Hash Table)" << endl;
    cout << "24. Compress File" << endl;
    cout << "25. Decompress File" << endl;
    cout << "26. Add File to Cloud Sync Queue" << endl;
    cout << "27. Process Cloud Sync Queue" << endl;
    cout << "28. Optimize File System Structure (AVL + Garbage Collection)" << endl;
    cout << "0. Exit\n";
}

string getCurrentTimestamp()
{
    time_t now = time(0);
    tm ltm;
    localtime_s(&ltm, &now);
    stringstream ss;
    ss << 1900 + ltm.tm_year << "-"
        << setw(2) << setfill('0') << 1 + ltm.tm_mon << "-"
        << setw(2) << setfill('0') << ltm.tm_mday << " "
        << setw(2) << setfill('0') << ltm.tm_hour << ":"
        << setw(2) << setfill('0') << ltm.tm_min << ":"
        << setw(2) << setfill('0') << ltm.tm_sec;
    return ss.str();
}


int main()
{
    Folder drive("Root");
    RecycleBin recycle;
    RecentFiles recent;
    UserSystem userSystem;
    string uname, pass, secQ, ans, logoutTime;
    string name, content;
    int versionNumber = 0;
    bool exit = false;

    cout << "=== Welcome to File System ===" << endl;
    cout << "1. Sign Up" << endl;
    cout << "2. Login" << endl;
    cout << "Choose: "<< endl;
    int authChoice;
    cin >> authChoice;
    cin.ignore();

    system("cls"); 


    if (authChoice == 1)
    {
        char another;
        do
        {
            cout << "Username: ";
            getline(cin, uname);
            cout << "Password: ";
            getline(cin, pass);
            cout << "Security Answer: ";
            getline(cin, secQ);
            cout << "Enter role (1-Admin, 2-Editor, 3-Viewer): ";
            int roleInput;
            cin >> roleInput;
            cin.ignore();
            Role userRole;
            switch (roleInput)
            {
            case 1:
                userRole = ADMIN;
                break;
            case 2:
                userRole = EDITOR;
                break;
            case 3:
                userRole = VIEWER;
                break;
            default:
                cout << "Invalid role, defaulting to Viewer." << endl;
                userRole = VIEWER;
            }
            userSystem.addUser(uname, pass, secQ, userRole);

            cout << "Add another user, if yes please enter (Y/y), otherwise anything: ";
            cin >> another;
            cin.ignore();
        } while (another == 'y' || another == 'Y');
    }

    system("cls");

    cout << "Login to continue..." << endl;
    cout << "Username: ";
    getline(cin, uname);
    cout << "Password: ";
    getline(cin, pass);
    if (!userSystem.login(uname, pass))
    {
        system("pause");
        return 0;
    }

    while (!exit)
    {
        system("cls");
        showMenu();
        // drive.getCurrentPath();

        int choice;
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        if (cin.fail())
        {
            cout << "Invalid input. Please enter a number between 1 and 28." << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            choice = -1; // force the loop to continue
            continue;
        }

        switch (choice)
        {
        case 1:
        {
            if (userSystem.getCurrentUserRole(uname) == VIEWER)
            {
                cout << "Permission denied: Viewers cannot create folders." << endl;
                break;
            }
            cout << "Enter folder name: ";
            getline(cin, name);
            drive.createFolder(name);
            break;
        }
        case 2:
        {
            if (userSystem.getCurrentUserRole(uname) == VIEWER)
            {
                cout << "Permission denied: Viewers cannot create files." << endl;
                break;
            }
            cout << "Enter file name: ";
            getline(cin, name);
            cout << "Enter content: ";
            getline(cin, content);
            drive.createFile(name, content);
            break;
        }
        case 3:
        {
            Role role = userSystem.getCurrentUserRole(uname);
            if (role == VIEWER)
            {
                cout << "Permission denied: Viewers cannot update files." << endl;
                break;
            }
            cout << "Enter file name: ";
            getline(cin, name);
            cout << "Enter new content: ";
            getline(cin, content);
            drive.updateFile(name, content);
            break;
        }
        case 4:
        {
            cout << "Enter file name: ";
            getline(cin, name);
            drive.viewFileHistory(name);
            break;
        }
        case 5:
        {
            if (userSystem.getCurrentUserRole(uname) == VIEWER)
            {
                cout << "Permission denied: Viewers cannot rollback files.";
                break;
            }
            cout << "Enter file name: ";
            getline(cin, name);
            cout << "Enter version number to rollback to: ";
            cin >> versionNumber;
            cin.ignore();
            drive.rollbackFile(name, versionNumber);
            break;
        }
        case 6:
        {
            cout << "Enter folder name to navigate: ";
            getline(cin, name);
            drive.navigateToFolder(name);
            break;
        }
        case 7:
        {
            drive.navigateUp();
            break;
        }
        case 8:
        {
            drive.listCurrent();
            break;
        }
        case 9:
        {
            drive.listAllFolders();
            break;
        }
        case 10:
        {
            cout << "Enter file name to access: ";
            getline(cin, name);
            drive.accessFile(name, recent);
            break;
        }
        case 11:
        {
            if (userSystem.getCurrentUserRole(uname) == VIEWER)
            {
                cout << "Permission denied: Viewers cannot delete files." << endl;
                break;
            }
            cout << "Enter file name to delete: ";
            getline(cin, name);
            drive.deleteFile(name, recycle);
            break;
        }
        case 12:
        {
            recent.viewRecentFiles();
            break;
        }
        case 13:
        {
            recycle.viewRecycleBin();
            break;
        }
        case 14:
        {
            if (userSystem.getCurrentUserRole(uname) != ADMIN)
            {
                cout << "Only Admin can empty recycle bin." << endl;
                break;
            }
            recycle.emptyRecycleBin();
            break;
        }
        case 15:
        {
            cout << "Enter username: ";
            getline(cin, uname);
            logoutTime = getCurrentTimestamp();
            userSystem.logout(uname, logoutTime);
            exit = true;
            break;
        }
        case 16:
        {
            if (userSystem.getCurrentUserRole(uname) == VIEWER)
            {
                cout << "Permission denied: Viewers cannot share files." << endl;
                break;
            }
            cout << "Enter sender username: ";
            getline(cin, uname);
            cout << "Enter receiver username: ";
            getline(cin, name);
            cout << "Enter file name to share (just for confirmation/logging purpose): ";
            string sharedFile;
            getline(cin, sharedFile);
            userSystem.shareFile(uname, name);
            cout << "Shared file: " << sharedFile << " from " << uname << " to " << name << endl;
            break;
        }
        case 17:
        {
            cout << "Enter username to view sharing connections and update a file: ";
            getline(cin, uname);
            userSystem.showSharingAccess(uname);
            if (userSystem.getCurrentUserRole(uname) != VIEWER)
            {
                cout << "\nYou have permission to update a file." << endl;
                cout << "Enter file name to update: ";
                getline(cin, name);
                cout << "Enter new content: ";
                getline(cin, content);
                drive.updateFile(name, content);
            }
            break;
        }
        case 18:
        {
            cout << "Enter file name to search: ";
            getline(cin, name);
            drive.searchFile(name);
            break;
        }
        case 19:
        {
            if (userSystem.getCurrentUserRole(uname) == VIEWER)
            {
                cout << "Permission denied: Viewers cannot restore files." << endl;
                break;
            }
            cout << "Enter file name to restore: ";
            getline(cin, name);
            drive.restoreFile(name, recycle);
            break;
        }
        case 20:
        {
            if (userSystem.getCurrentUserRole(uname) == VIEWER)
            {
                cout << "Permission denied: Viewers cannot delete folders." << endl;
                break;
            }
            cout << "Enter folder name to delete: ";
            getline(cin, name);
            drive.deleteFolder(name);
            break;
        }
        case 21:
        {
            userSystem.displayAllUsers();
            break;
        }
        case 22:
        {
            cout << "Enter username: ";
            getline(cin, uname);
            cout << "Enter security answer: ";
            getline(cin, ans);
            userSystem.forgotPassword(uname, ans);
            break;
        }
        case 23:
        {
            FileHashTable fileIndex;
            cout << "Enter file name to view metadata: ";
            getline(cin, name);

            // Find the file in current directory
            treenode* fileNode = drive.findChildByName(drive.currentfolder, name);
            if (fileNode != nullptr && !fileNode->isFolder)
            {
                // Add file to hash table if not already present
                fileIndex.insert(name, drive.getCurrentPath(), uname,
                    name.substr(name.find_last_of(".") + 1), // Get file extension as type
                    fileNode->fileVersion->getLatestContent().length(), // Content length as size
                    getCurrentTimestamp());

                // Display the metadata
                fileIndex.displayFileInfo(name);
            }
            else
            {
                cout << "File not found in current directory." << endl;
            }
            break;
        }

        case 24:
        {
            if (userSystem.getCurrentUserRole(uname) == VIEWER)
            {
                cout << "Permission denied: Viewers cannot compress files." << endl;
                break;
            }

            cout << "Enter file name to compress: ";
            getline(cin, name);

            treenode* fileNode = drive.findChildByName(drive.currentfolder, name);
            if (fileNode != nullptr && !fileNode->isFolder && fileNode->fileVersion != nullptr)
            {
                string content = fileNode->fileVersion->getLatestContent();
                string compressed = FileCompression::compressFile(content, true); // Use RLE

                // Create a new compressed file
                string compressedName = name + ".compressed";
                drive.createFile(compressedName, compressed);
                cout << "File compressed and saved as '" << compressedName << endl;
            }
            else
            {
                cout << "File not found in current directory." << endl;
            }
            break;
        }

        case 25:
        {
            if (userSystem.getCurrentUserRole(uname) == VIEWER)
            {
                cout << "Permission denied: Viewers cannot decompress files." << endl;
                break;
            }

            cout << "Enter compressed file name: ";
            getline(cin, name);

            treenode* fileNode = drive.findChildByName(drive.currentfolder, name);
            if (fileNode != nullptr && !fileNode->isFolder && fileNode->fileVersion != nullptr)
            {
                string compressed = fileNode->fileVersion->getLatestContent();
                string decompressed = FileCompression::decodeRLE(compressed);

                // Create a new decompressed file
                string decompressedName = name;
                int pos = static_cast<int>(decompressedName.find(".compressed"));

                if (pos != static_cast<int>(string::npos))
                {
                    decompressedName.erase(pos, 11); // 11 characters in ".compressed"
                }

                drive.createFile(decompressedName, decompressed);
                cout << "File decompressed and saved as '" << decompressedName << "'." << endl;
            }
            else
            {
                cout << "File not found in current directory." << endl;
            }
            break;
        }

        case 26:
        {
            if (userSystem.getCurrentUserRole(uname) == VIEWER)
            {
                cout << "Permission denied: Viewers cannot add sync tasks." << endl;
                break;
            }

            CloudSync cloudSync;
            cout << "Enter file name to add to sync queue: ";
            getline(cin, name);

            treenode* fileNode = drive.findChildByName(drive.currentfolder, name);
            if (fileNode != nullptr && !fileNode->isFolder && fileNode->fileVersion != nullptr)
            {
                string content = fileNode->fileVersion->getLatestContent();
                cloudSync.addSyncTask("upload", name, content);
                cout << "File added to cloud sync queue." << endl;
            }
            else
            {
                cout << "File not found in current directory." << endl;
            }
            break;
        }

        case 27:
        {
            if (userSystem.getCurrentUserRole(uname) != ADMIN)
            {
                cout << "Permission denied: Only Admins can manually process sync queue." << endl;
                break;
            }

            CloudSync cloudSync;
            cloudSync.startSync(); // Manually trigger sync
            break;
        }
        case 28:
        {
            if (userSystem.getCurrentUserRole(uname) != ADMIN)
            {
                cout << "Permission denied: Only Admins can optimize the file system." << endl;
                break;
            }
            drive.optimizeStructure();
            break;
        }
        case 0:
            exit = true;
            cout << "Exiting system." << endl;
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    }

    system("pause");
    return 0;
}