#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <argon2.h>
#include <openssl/rand.h>
#include <conio.h>
#include <pqxx/pqxx>
#include <vector>
#include <windows.h>
#include <fstream>
using namespace std;

#define RESET   "\033[0m"
#define CYAN    "\033[36m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define RED     "\033[31m"
#define HOT_PINK "\033[38;2;255;105;180m"
#define BLUE    "\033[1;34m"
#define HOT_GREEN "\033[1;32m"
#define BBLACK   "\x1b[90m"  
#define BRED     "\x1b[91m"  
#define BGREEN   "\x1b[92m"  
#define BYELLOW  "\x1b[93m"  
#define BMAGENTA "\x1b[95m"  
#define BCYAN    "\x1b[96m"  
#define BWHITE   "\x1b[97m"  
#define BEACH "\033[38;5;223m"
#define PINK "\033[38;5;225m"
#define PASTEL_YELLOW "\033[38;5;229m"
#define PASTEL_BLUE   "\033[38;5;153m"
#define LAVENDER "\033[38;5;105m"
void clear()
{
    system("cls");
}
void cursor()
{    HANDLE h;
     CONSOLE_SCREEN_BUFFER_INFO info;  
     GetConsoleScreenBufferInfo(h, &info);
     int cols = info.srWindow.Right + 1; 
     int rows = info.srWindow.Bottom + 1;
}
void cursorable()
{HANDLE h;
     CONSOLE_CURSOR_INFO cursor;  
     cursor.bVisible = true;     
     SetConsoleCursorInfo(h, &cursor); 
}
void gotoxy(int x, int y)
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD c = {x, y};
    SetConsoleCursorPosition(h, c);
}

class UI_main{
  public:
  void intro()
{ clear();
     string a;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

int rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
int cols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    ifstream f("text.txt");
    while(getline(f,a)){
        cout<< HOT_PINK ;
        gotoxy(55,rows -35);
        cout<<a<<endl;
        Sleep(200);
        rows++;
    }    
    gotoxy(66,rows-33);
    cout<<HOT_PINK;
    cout<<"Secure User File Vault";
    gotoxy(72,rows-13);
    cout<<GREEN;
    cout<<"Version 1.0";
    cout<< RESET;
    Sleep(4000);
    clear();
    clear();
 gotoxy(15,5);
    cout<<BYELLOW;
  cout<<"This mini project, Secure User File Vault (SUFV), is a console-based tool to safely store, view, and manage user files. ";
  gotoxy(27,6);
  cout<<"It focuses on simple navigation with an added layer of security for your data.";
  gotoxy(1,7);
  cout<<RESET;
  Sleep(10000);
  clear();
}
void end()
{
     clear();
     string a;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

int rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
int cols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    ifstream f1("thankyou.txt");
    while(getline(f1,a)){
        cout<< HOT_PINK ;
        gotoxy(53,rows -35);
        cout<<a<<endl;
        Sleep(200);
        rows++;
    }    
    Sleep(100);
    f1.close();
    ifstream f2("face.txt");
    while(getline(f2,a)){
        cout<< HOT_PINK ;
        gotoxy(55,rows -28);
        cout<<a<<endl;
        Sleep(200);
        rows++;
    }    

    f2.close();
    gotoxy(64,rows -31);
    cout<<LAVENDER;
    char c[50]= {"I hope you had a great time, come back soon"};
    for(int i=0; i<sizeof(c); i++)
    {
        cout<<c[i];
        Sleep(100);
    }
    gotoxy(75,rows -29);
    char b[30]= {"I'll be waiting!"};
    for(int i=0; i<sizeof(b); i++)
    {
        cout<<b[i];
        Sleep(100);
    }
    cout<<RESET;
    Sleep(1000);
    clear();
}
};
class security{
    public:
    string getpass()
{
string password;
    char ch;

    while (true)
    { ch = _getch();
        if (ch == 13)
            break;
        if (ch == 8)
        {if (!password.empty())
            { password.pop_back();
                cout << "\b \b";
            }
        } else
        {password += ch;
            cout << '*';   
        } }
return password;
}

string hash_password(string input)
{
    uint8_t raw_salt[16];
    if (RAND_bytes(raw_salt, sizeof(raw_salt)) != 1)
    {
        cout << "Failed to generate salt" << endl;
        return "";
    }

    size_t encoded_len = argon2_encodedlen(2, 65536, 1, sizeof(raw_salt), 32, Argon2_id);
    string encoded(encoded_len, '\0');

    int rc = argon2id_hash_encoded(
        2, 65536, 1,
        input.c_str(), input.length(),
        raw_salt, sizeof(raw_salt),
        32,
        &encoded[0], encoded_len
    );
    if (rc != ARGON2_OK)
    {
        cout << "Argon2 error: " << argon2_error_message(rc) << endl;
        return "";
    }
    encoded.resize(encoded.find('\0'));
    return encoded;
}

bool argon2_verify_hash(string hash, string input)
{if(hash.empty() || input.empty()) return false;
    int rc = argon2id_verify(hash.c_str(), input.c_str(), input.length());
    return rc == ARGON2_OK;
}

};

string get_envvar()
{
    const char* val = getenv("DB_PASS");
if (!val) {
    cout << "Missing env variable";
    exit(1);
}
return string(val);
}

class DATA{
    pqxx::connection con;
    public:
    DATA(): con("dbname=login user=postgres password="+ get_envvar() +" host=localhost port=5433")
    {
    if (con.is_open())
    {
        cout<<GREEN<<"Connected to database successfully!"<<RESET<<endl;
    }else{
        cout<<RED<<"Connection failed!"<<RESET<<endl;
        exit(1);
    }
try{ 
    pqxx::work w(con);
   w.exec(R"(
    CREATE TABLE IF NOT EXISTS users (
        id            SERIAL PRIMARY KEY,
        username      TEXT UNIQUE NOT NULL,
        password_hash TEXT NOT NULL,
        created_at    TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
        ques          TEXT,
        ans           TEXT
    )
)");

w.exec(R"(
    CREATE TABLE IF NOT EXISTS files (
        id         SERIAL PRIMARY KEY,
        user_id    INTEGER NOT NULL REFERENCES users(id) ON DELETE CASCADE,
        filename   TEXT NOT NULL,
        extension  TEXT NOT NULL,
        content    BYTEA NOT NULL,
        size_bytes INTEGER NOT NULL DEFAULT 0,
        created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
        updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
        UNIQUE(user_id, filename)
    )
)");

w.commit();

        con.prepare("register","INSERT INTO users(username,password_hash,ques,ans) VALUES($1,$2,$3,$4)");
        con.prepare("pass","SELECT password_hash FROM users WHERE username = $1");
        con.prepare("question","SELECT ques FROM users WHERE username = $1");
        con.prepare("answer","SELECT ans FROM users WHERE username= $1");
        con.prepare("user","SELECT COUNT(*) FROM users WHERE username=$1");
        con.prepare("setpass","UPDATE users SET password_hash = $1 WHERE username = $2");
        con.prepare("get_uid","SELECT id FROM users WHERE username=$1");
        con.prepare("file_insert","INSERT INTO files(user_id,filename,extension,content,size_bytes) VALUES($1,$2,$3,$4,$5)");
        con.prepare("file_list", "SELECT id,filename,extension,size_bytes,updated_at FROM files WHERE user_id=$1 ORDER BY updated_at DESC");
        con.prepare("file_fetch", "SELECT content,filename,extension FROM files WHERE id=$1 AND user_id=$2");
        con.prepare("file_update","UPDATE files SET content=$1,size_bytes=$2, updated_at=CURRENT_TIMESTAMP WHERE id=$3 AND user_id=$4");
        con.prepare("file_delete", "DELETE FROM files WHERE id=$1 AND user_id=$2");
        con.prepare("file_exists","SELECT COUNT(*) FROM files WHERE user_id=$1 AND filename=$2");
    }
catch(const pqxx::sql_error& e)
{
    cout << "Database error: " << e.what() << endl;
    exit(1);
}}
    void reg(string user,string pass,string q,string a)
    {pqxx::work w(con);
    w.exec(pqxx::prepped("register"),pqxx::params{user,pass,q,a});
        w.commit();
    }
    void set_pass(string user,string pass)
    { pqxx::work w(con);
        w.exec(pqxx::prepped("setpass"),pqxx::params{pass,user});
        w.commit();
    }
    string get_pass(string user)
    {
    pqxx::work w(con);
    pqxx::result r = w.exec(pqxx::prepped("pass"),pqxx::params{user});
    if(r.empty()) return "";
    string hash_stored = r[0][0].as<string>();
    return hash_stored;
    }
string sec_ques(string user)
{    pqxx::work w(con);
    pqxx::result q= w.exec(pqxx::prepped("question"),pqxx::params{user});
    if(q.empty()) return "";
    string stored_ques= q[0][0].as<string>();
    return stored_ques;
}
string sec_ans(string user)
{
    pqxx::work w(con);
    pqxx::result a= w.exec(pqxx::prepped("answer"),pqxx::params{user});
    if(a.empty()) return "";
    string stored_ans = a[0][0].as<string>();
    return stored_ans;
}
bool user_check(string username)
{ pqxx::work w(con);
    pqxx::result a= w.exec(pqxx::prepped("user"),pqxx::params{username});
    if(a.empty()) return false;
    int count = a[0][0].as<int>();
    if(count>0)
    return true;
    else 
    return false;
}
int get_user_id(string username)
{
    pqxx::work w(con);
    pqxx::result r = w.exec(pqxx::prepped("get_uid"), pqxx::params{username});
    if(r.empty()) return -1;
    return r[0][0].as<int>();
}
bool file_insert(int user_id, string filename, string ext, vector<unsigned char>& content)
{
    if(content.size() > 10 * 1024 * 1024)
    {
        cout << RED<<"File too large. Maximum size is 10MB."<<RESET << endl;
        return false;
    }
    pqxx::work w(con);
    string_view blob(reinterpret_cast<const char*>(content.data()), content.size());
w.exec(pqxx::prepped("file_insert"),
       pqxx::params{user_id, filename, ext, pqxx::binary_cast(blob), (int)content.size()});
    w.commit();
    return true;
}

void file_list(int user_id)
{
    pqxx::work w(con);
    pqxx::result r = w.exec(pqxx::prepped("file_list"), pqxx::params{user_id});
    if(r.empty())
    {
        cout <<RED<< "No files found."<<RESET << endl;
        return;
    }
    cout <<PASTEL_YELLOW<<"\n ID  | Filename               | Type  | Size     | Last Modified" <<PASTEL_BLUE<< endl;
    cout << string(70, '-') << endl;
    for(auto row : r)
    {
        cout << " " << row["id"].as<string>()
             << "   | " << row["filename"].as<string>()
             << string(max(0, 22 - (int)row["filename"].as<string>().length()), ' ')
             << "| " << row["extension"].as<string>()
             << string(max(0, 6  - (int)row["extension"].as<string>().length()), ' ')
             << "| " << row["size_bytes"].as<string>() << " B"
             << string(max(0, 8  - (int)row["size_bytes"].as<string>().length()), ' ')
             << "| " << row["updated_at"].as<string>()
             <<RESET<< endl;
    }
}

vector<unsigned char> file_fetch(int file_id, int user_id, string& out_filename, string& out_ext)
{
    pqxx::work w(con);
    pqxx::result r = w.exec(pqxx::prepped("file_fetch"), pqxx::params{file_id, user_id});
    if(r.empty())
    {
        cout << RED<<"File not found or access denied."<<RESET << endl;
        return {};
    }
    out_filename = r[0]["filename"].as<string>();
    out_ext      = r[0]["extension"].as<string>();
   auto blob = r[0]["content"].as<pqxx::bytes>();
return vector<unsigned char>(
    reinterpret_cast<const unsigned char*>(blob.data()),
    reinterpret_cast<const unsigned char*>(blob.data()) + blob.size()
);
}
bool file_update(int file_id, int user_id, vector<unsigned char>& content)
{
    if(content.size() > 10 * 1024 * 1024)
    {
        cout <<RED<< "File too large. Maximum size is 10MB."<<RESET << endl;
        return false;
    }
    pqxx::work w(con);
    string_view blob(reinterpret_cast<const char*>(content.data()), content.size());
    w.exec(pqxx::prepped("file_update"),
           pqxx::params{pqxx::binary_cast(blob), (int)content.size(), file_id, user_id});
    w.commit();
    return true;
}   
bool file_delete(int file_id, int user_id)
{
    pqxx::work w(con);
    w.exec(pqxx::prepped("file_delete"), pqxx::params{file_id, user_id});
    w.commit();
    return true;
}

bool file_exists(int user_id, string filename)
{
    pqxx::work w(con);
    pqxx::result r = w.exec(pqxx::prepped("file_exists"), pqxx::params{user_id, filename});
    return r[0][0].as<int>() > 0;
}
};
class FileManager : public DATA
{
public:
    string sanitize(string name)
    {
        string clean;
        for(char c : name)
            if(isalnum(c) || c == '.' || c == '_' || c == '-' || c == ' ')
                clean += c;
        return clean;
    }

    string get_temp_dir()
    {
        char buf[MAX_PATH];
        GetTempPathA(MAX_PATH, buf);
        return string(buf);
    }

    vector<unsigned char> read_file_bytes(string path)
    {
        ifstream f(path, ios::binary);
        if(!f.is_open()) return {};
        return vector<unsigned char>(
            istreambuf_iterator<char>(f),
            istreambuf_iterator<char>()
        );
    }

    bool write_file_bytes(string path, vector<unsigned char>& data)
    {
        ofstream f(path, ios::binary);
        if(!f.is_open()) return false;
        f.write(reinterpret_cast<char*>(data.data()), data.size());
        return true;
    }

    string open_file_dialog()
    {
        char filename[MAX_PATH] = "";
        OPENFILENAMEA ofn;
        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner   = NULL;
        ofn.lpstrFile   = filename;
        ofn.nMaxFile    = MAX_PATH;
        ofn.lpstrFilter =
            "All Files\0*.*\0"
            "Text Files\0*.txt\0"
            "Word Documents\0*.docx\0"
            "PowerPoint\0*.pptx\0"
            "Excel\0*.xlsx\0";
        ofn.lpstrTitle  = "Select a file to import";
        ofn.Flags       = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
        if(GetOpenFileNameA(&ofn))
            return string(filename);
        return "";
    }

    string extract_filename(string full_path)
    {
        size_t pos = full_path.find_last_of("\\/");
        if(pos == string::npos) return full_path;
        return full_path.substr(pos + 1);
    }

    string extract_extension(string filename)
    {
        size_t pos = filename.find_last_of('.');
        if(pos == string::npos) return "";
        return filename.substr(pos);
    }

    void open_and_wait(string filepath)
    {
        SHELLEXECUTEINFOA info = {};
        info.cbSize = sizeof(info);
        info.fMask  = SEE_MASK_NOCLOSEPROCESS;
        info.lpVerb = "open";
        info.lpFile = filepath.c_str();
        info.nShow  = SW_SHOW;

        if(!ShellExecuteExA(&info))
        {
            cout <<RED<< "Failed to open file."<<RESET << endl;
            return;
        }
        cout << GREEN <<"File opened. Save and close the app to continue..."<<RESET << endl;
        WaitForSingleObject(info.hProcess, INFINITE);
        CloseHandle(info.hProcess);
    }
    void import_file(int user_id)
{
    string path = open_file_dialog();
    if(path.empty())
    {
        cout << RED<<"Import cancelled."<<RESET << endl;
        return;
    }

    string fname = sanitize(extract_filename(path));
    string ext   = extract_extension(fname);

    if(fname.empty())
    {
        cout << RED<<"Invalid filename."<<RESET << endl;
        return;
    }

    if(file_exists(user_id, fname))
    {
        cout <<HOT_PINK<< "File '" << fname << "' already exists in your vault." << endl;
        cout << "Overwrite? (y/n): "<<RESET;
        char ch; cin >> ch;
        if(ch != 'y' && ch != 'Y') return;
    }

    vector<unsigned char> data = read_file_bytes(path);
    if(data.empty())
    {
        cout <<RED<< "Could not read file or file is empty."<<RESET << endl;
        return;
    }

    if(file_insert(user_id, fname, ext, data))
        cout <<GREEN<< "File '" << fname << "' imported successfully." <<RESET<< endl;
}

void create_file(int user_id)
{
    cout <<PASTEL_BLUE<< "\nWhat type of file do you want to create?" << endl;
    cout << "1. Text file (.txt)" << endl;
    cout << "2. Word document (.docx)" << endl;
    cout << "3. PowerPoint (.pptx)" << endl;
    cout << "4. Excel spreadsheet (.xlsx)" << endl;
    cout << "Enter choice: "<<PASTEL_YELLOW;

    int ch;
    if(!(cin >> ch))
    {
        cin.clear();
        cin.ignore(1000, '\n');
        return;
    }

    string ext;
    switch(ch)
    {
        case 1: ext = ".txt";  break;
        case 2: ext = ".docx"; break;
        case 3: ext = ".pptx"; break;
        case 4: ext = ".xlsx"; break;
        default:
            cout << RED<<"Invalid choice."<<RESET << endl;
            return;
    }

    cout <<CYAN<< "Enter filename (without extension): "<<RESET;
    string name;
    cin.ignore();
    getline(cin, name);
    name = sanitize(name);

    if(name.empty())
    {
        cout << RED <<"Invalid filename."<<RESET << endl;
        return;
    }

    string fname    = name + ext;
    string temppath = get_temp_dir() + fname;

    if(file_exists(user_id, fname))
    {
        cout << HOT_PINK<<"File '" << fname << "' already exists in your vault."<<RESET << endl;
        return;
    }

    { ofstream f(temppath, ios::binary); }

    struct TempGuard
    {
        string p;
        TempGuard(string path) : p(path) {}
        ~TempGuard() { DeleteFileA(p.c_str()); }
    } guard(temppath);

    open_and_wait(temppath);

    vector<unsigned char> data = read_file_bytes(temppath);

    if(file_insert(user_id, fname, ext, data))
        cout <<HOT_PINK<< "File '" << fname << "' saved to your vault."<<RESET << endl;
}

void open_file(int user_id)
{
    file_list(user_id);

    cout << CYAN<<"\nEnter file ID to open (0 to cancel): "<<BEACH;
    int fid;
    if(!(cin >> fid) || fid == 0)
    {
        cin.clear();
        cin.ignore(1000, '\n');
        return;
    }

    string fname, ext;
    vector<unsigned char> data = file_fetch(fid, user_id, fname, ext);
    if(fname.empty()) return;

    string temppath = get_temp_dir() + fname;

    struct TempGuard
    {
        string p;
        TempGuard(string path) : p(path) {}
        ~TempGuard() { DeleteFileA(p.c_str()); }
    } guard(temppath);

    if(!write_file_bytes(temppath, data))
    {
        cout <<RED<< "Could not write temp file." <<RESET<< endl;
        return;
    }

    open_and_wait(temppath);

    vector<unsigned char> updated = read_file_bytes(temppath);
    if(file_update(fid, user_id, updated))
        cout <<LAVENDER<< "Changes saved to vault." <<RESET<< endl;
}

void delete_file(int user_id)
{
    file_list(user_id);

    cout <<CYAN<<"\nEnter file ID to delete (0 to cancel): "<<BEACH;
    int fid;
    if(!(cin >> fid) || fid == 0)
    {
        cin.clear();
        cin.ignore(1000, '\n');
        return;
    }

    cout << YELLOW<<"Are you sure? This cannot be undone. (y/n): "<<BEACH;
    char ch; cin >> ch;
    if(ch != 'y' && ch != 'Y') return;

    if(file_delete(fid, user_id))
        cout <<GREEN<< "File deleted."<<RESET << endl;
}

int file_menu(int user_id)
{
    while(true)
    {   Sleep(4000);
        clear();
        cout <<PASTEL_BLUE<< "\n-------------- File Vault --------------" << endl;
        cout << "1. My Files" << endl;
        cout << "2. Import a File" << endl;
        cout << "3. Create a New File" << endl;
        cout << "4. Open / Edit a File" << endl;
        cout << "5. Delete a File" << endl;
        cout << "6. Back" << endl;
        cout << "Enter choice: "<<PASTEL_YELLOW;

        int ch;
        if(!(cin >> ch))
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << RED<<"Invalid input." <<RESET<< endl;
            continue;
        }

        switch(ch)
        {
            case 1: file_list(user_id);  
                    Sleep(6000);
                    clear();
                   
                    break;
            case 2: import_file(user_id); Sleep(6000);
                    clear();
                    break;
            case 3: create_file(user_id); Sleep(6000);
                    clear();
                   break;
            case 4: open_file(user_id);  Sleep(6000);
                    clear();
                  break;
            case 5: delete_file(user_id); Sleep(6000);
                    clear();
                   break;
            case 6: return 1;
            default: cout << RED<<"Invalid choice."<<RESET << endl;
        }
    }
}
};

class Auth: public FileManager, public security
{
    public:
    void registration ()
    {  
        clear();
        printf(CYAN "\n=====================================\n          ");
        char temp[25]= {"REGISTRATION"};
    for(int i=0; i<25; i++)
    {
        printf("%c",temp[i]);
        Sleep(40);
    }

        printf("\n=====================================\n" RESET);
        string user,pass,sec_q,sec_a,hash_passw,hash_sec_ans;
        while(1){
        cout<<PASTEL_BLUE<<"Enter username: "<<PASTEL_YELLOW;
        cin>>user;
        if(user_check(user))
        {
            cout<<RED<<"Username already exist!\nTry different username."<<RESET<<endl;
        }
        else{
            break;
        }}
        cout<<CYAN<<"Enter password: "<<RESET;
        cin.ignore(1000,'\n');
        pass = getpass();
        cout<<CYAN<<"\nEnter Security Question : "<<RESET;  
        cin.ignore(); 
        getline(cin,sec_q);
        cout<<CYAN<<"Enter the answer: "<<RESET;
        getline(cin,sec_a);
        hash_passw = hash_password(pass);
        hash_sec_ans = hash_password(sec_a);
        reg(user,hash_passw,sec_q,hash_sec_ans);
        cout <<GREEN<< "Registration successful\n"<<RESET;
        Sleep(4000);
        clear();
    }

    void setpass(string user){
        int count=0;
        string password,new_pass,confirm_pass,old_pass,hash_newpass;

        while (1)
       {cout<<PASTEL_BLUE<<"Enter current password: "<<PASTEL_YELLOW;
       password = getpass();
       old_pass = get_pass(user);
        if(argon2_verify_hash(old_pass, password))
       {count =0;
            while(1)
            {cin.ignore(1000,'\n');
            cout<<PASTEL_BLUE<<"\nSet new password: "<<PASTEL_YELLOW;
            new_pass = getpass();
            cin.ignore(1000,'\n');
            cout<<PASTEL_BLUE<<"\nConfirm password: "<<PASTEL_YELLOW;
            confirm_pass= getpass();
            if(confirm_pass==new_pass && count <=3)
            {
                hash_newpass= hash_password(new_pass);
                set_pass(user,hash_newpass);
                cout<<GREEN<<"\nPassword Changed Successfully.\n"<<RESET;
                break;
    
            }
            else{count++;
                if(count>=3)
            {cout<<RED<<"\nToo Many Attempts!"<<RESET;
            return;
            }
                cout<<RED<<"\nPasswords do not match! Try again.\n You are left with "<<3-count<<" attempts."<<RESET;
            }}
        break;
        }
        else{
            count++;
            if(count>=3)
            {cout<<RED<<"\nToo Many Attempts!"<<RESET;
            return;
            }
            else{
            cout<<RED<<"\nIncorrect Password! Try again. You are left with "<<LAVENDER<<3-count<<RED<<" attempts\n"<<RESET;
            }
        }
    Sleep(4000);
clear();
}}

    void forgot_pass(string user)
    { int count =0;
        string question,answer,new_ans,new_pass,confirm_pass,hash_newpass;
        question= sec_ques(user);
        answer = sec_ans(user);
        if(question.empty()){
    cout<<RED<<"User not found\n"<<RESET;
    return;
}
        while (1)
       {cout<<PASTEL_BLUE<<"Security Question: "<<question<<PASTEL_YELLOW<<endl;
        new_ans= getpass();
        if(argon2_verify_hash(answer, new_ans))
        {
            while(1)
            {
            cout<<PASTEL_BLUE<<"\nSet New Password: "<<PASTEL_YELLOW;
            cin.ignore(1000,'\n');
            new_pass = getpass();
            cout<<PASTEL_BLUE<<"\nConfirm password: "<<PASTEL_YELLOW;
            confirm_pass= getpass();
            if(confirm_pass==new_pass)
            {
                hash_newpass= hash_password(new_pass);
                set_pass(user,hash_newpass);
                cout<<GREEN<<"\nPassword Changed Successfully.\n"<<RESET;
                break;
            }
            else{
                cout<<GREEN<<"\nPasswords do not match! Try again.\n"<<RESET;
            }}
        break;
        }
        else {
            count++;
            if(count>=3)
            {
                cout<<RED<<"\nToo Many Attempts"<<RESET;
                return ;
            } 
            else{
            cout<<RED<<"\nIncorrect Answer! Try Again. You are left with "<<LAVENDER<<3-count<<RED<<" Attempts"<<endl<<RESET;    
        }}
        Sleep(4000);
        clear();
    }
    }

    string user_login()
    {   int count=0;
        string user,pass;
        clear();
          printf(CYAN "\n=====================================\n              ");
        char temp[25]= {"LOGIN"};
    for(int i=0; i<25; i++)
    {
        printf("%c",temp[i]);
        Sleep(40);
    }

        printf("\n=====================================\n" RESET);
        while (count<3){
        cout<<PASTEL_BLUE<<"Enter username: "<<PASTEL_YELLOW;
        cin>>user;
        if(user_check(user))
        {
            break;
        }
       count++;
            cout<<RED<<"Username not found! \n Try again. You are left with "<<LAVENDER<<3-count<<RED" attempts."<<RESET<<endl;
    }
    if(count==3)
             {cout<<RED<<"Too many attempts!\n"<<RESET;
                return "";}
        count =0;
        string getpassword = get_pass(user);
        while (1)
        {
        cout<<PASTEL_BLUE<<"\nEnter password: "<<PASTEL_YELLOW;
        pass = getpass();
        cout << endl;
        if(argon2_verify_hash(getpassword, pass))
        {
            cout<<GREEN<<"Login Successful.\n"<<RESET;
            return user;
        }
         count++;
          cout<<RED<<"Incorrect Password! \nTry Again. You are left with "<<LAVENDER<< 3-count<<RED<<" attempts\n"<<RESET;
          if(count==3)
          {cout<<RED<<"Too Many Attempts!"<<RESET;
            return "";
          }
         cout<<PASTEL_BLUE<<"\n1.Try password Again.\n2.Forgot Password.\nEnter Choice: "<<PASTEL_YELLOW;
         int ch;
         cin>>ch;
         switch(ch)
         {
            case 1:
            break;
            case 2:
            forgot_pass(user);
            return "";
            default:
            cout<<RED<<"Invalid choice! Try again."<<RESET;
         }
         
        Sleep(4000);
        clear();
        }
    }
};
  
int main()
{   UI_main b;
   b.intro();
    Auth a;
    int choice;
    Sleep(2000);
    while (true)
    {
        cout <<PASTEL_BLUE "\n1. Register\n2. Login\n3. Exit\nEnter choice: "<<PASTEL_YELLOW;
        cin >> choice;

        switch (choice)
        {
        case 1:
            a.registration();
            break;

        case 2:
        {
            string logged_user = a.user_login();
if(!logged_user.empty())
{
    int uid = a.get_user_id(logged_user);

    while(true)
    {Sleep(4000);
        clear();
        cout << PASTEL_BLUE<<"\n----------- Welcome " << logged_user << " -----------" << endl;
        cout << "1. My File Vault" << endl;
        cout << "2. Change Password" << endl;
        cout << "3. Logout" << endl;
        cout << "Enter choice: "<<PASTEL_YELLOW;

        int opt;
        if(!(cin >> opt))
        {
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }

        if(opt == 1)    {  a.file_menu(uid);
        }
        else if(opt == 2) {a.setpass(logged_user); }
        else if(opt == 3) { cout <<GREEN<< "Logged out." <<RESET<< endl;
        break ;  }
        else cout <<RED<< "Invalid choice." <<RESET<< endl;
    }
}
            break;
        }

        case 3:
            cout << LAVENDER <<"Exiting...\n"<<RESET;
             b.end();
            return 0;

        default:
            cout << RED<<"Invalid choice\n"<<RESET;
        }
        
    }
    b.end(); 
    return 0;
}
