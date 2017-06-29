#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <vector>
#include <dirent.h>
#include <string>
#include <cstring>
//#include <string.h>
#include <iostream>
#include <pwd.h>
//#include <time.h>
//#include <sys/types.h>
#include <sys/stat.h>

using namespace std;

#define FASTER    326
#define SLOWER    327
#define INTERVAL  0.5
#define TRUE        1


void get_Log(int ben1_1, int ben2_1, int ben1_2, int ben2_2)
{
    cout << "\nBenchmark1 of First PID: " << ben1_1 << endl;
    cout << "Benchmark2 of First PID: " << ben2_1 << endl;
    cout << "Benchmark1 of Second Work?: " << ben1_2 << endl;
    cout << "Benchmark2 of Second Work?: " << ben2_2 << endl;
}


void setWait(int seconds)
{
    clock_t endwait;
    endwait = clock() + seconds * CLOCKS_PER_SEC;
    while (clock() < endwait)
    {}
}


typedef struct _process_info
{
    char who_Is_User[32];
    vector<string> process;
} PCB;

typedef vector<PCB> Plist;

class Procfs
{
public:
    Plist plist;

    void set_Process()
    {
        DIR *fs_dir;
        struct dirent *entry = NULL;
        char proc_file[40];
        vector<string> procinfo;
        PCB lprocess_info;
        struct stat lstat;

        plist.clear();

        if ((fs_dir = opendir("/proc")) == NULL)
        {
            perror("opendir error :");
            exit(0);
        }

        while ((entry = readdir(fs_dir)) != NULL)
        {
            if (strcmp(entry->d_name, ".") != 0 &&
                strcmp(entry->d_name, "..") != 0)
            {
                sprintf(proc_file, "/proc/%s/stat", entry->d_name);

                if (access(proc_file, F_OK) != 0)
                    continue;

                if (IsDigit(entry->d_name))
                {
                    struct passwd *upasswd;
                    stat(proc_file, &lstat);
                    lprocess_info.process = get_ProcessParser(proc_file);

                    upasswd = getpwuid(lstat.st_uid);
                    strncpy(lprocess_info.who_Is_User, upasswd->pw_name, 32);

                    if (atoi(lprocess_info.process[0].c_str()) == atoi(entry->d_name))
                        plist.push_back(lprocess_info);
                }
            }
        }
        closedir(fs_dir);
    }

    vector<string> get_ProcessParser(char *proc_file)
    {
        FILE *file;
        char buf[512] = {0x00,};
        char field[80];
        int index = 0;
        unsigned int i, j = 0;
        vector<string> virtual_Proc;

        file = fopen(proc_file, "r");
        if (file == NULL)
        {
            perror("error : ");
            exit(0);
        }

        memset(field, 0x00, 80);
        fgets(buf, 511, file);

        for (i = 0; i < strlen(buf); i++)
        {
            if (buf[i] != ' ' && i != (strlen(buf) - 1))
                field[j++] = buf[i];

            else
            {
                if (index == 1)
                {
                    field[strlen(field) - 1] = 0x00;
                    virtual_Proc.push_back(field + 1);
                }
                else
                    virtual_Proc.push_back(field);

                memset(field, 0x00, 80);
                j = 0;
                index++;
            }
        }
        fclose(file);
        return virtual_Proc;
    }

    int IsDigit(char *str)
    {
        int i;
        for (i = 0; i < strlen(str); i++)
            if (isdigit(str[i]) == 0)
                return 0;

        return 1;
    }
};

int main(void)
{
    int file_Read_Number = 0;
    int ben1_1 = 0, ben2_1 = 0;
    int ben1_2 = 0, ben2_2 = 0;

    while (TRUE)
    {
        Procfs *mprocfs;
        Plist plist;
        string process_Name;
        int pid, len, i;

        if (ben1_1 && ben2_1 && ben1_2 && ben2_2 && file_Read_Number > 1000)
            break;

        mprocfs = new Procfs;
        mprocfs->set_Process();

        plist = mprocfs->plist;

        len = (int) mprocfs->plist.size();
        file_Read_Number++;
        setWait(INTERVAL);

        for (i = 0; i < len; i++)
        {
            pid = atoi(plist[i].process[0].c_str());

            if (strcmp(plist[i].process[1].c_str(), "benchmark2") == 0 && !ben2_1)
            {
                ben2_1 = pid;
                get_Log(ben1_1, ben2_1, ben1_2, ben2_2);
            }
            if (strcmp(plist[i].process[1].c_str(), "benchmark1") == 0 && !ben1_1)
            {
                ben1_1 = pid;
                get_Log(ben1_1, ben2_1, ben1_2, ben2_2);
            }

            /* CPU */
            if (strcmp(plist[i].process[1].c_str(), "benchmark2") == 0 && ben2_1 && ben2_1 != pid && !ben2_2)
            {
                ben2_2 = 1;
                syscall(FASTER, pid);

                cout << "\n[Second Monitoring Check]\t" << endl;
                cout << "\n[Fast syscall]\t" << pid << endl;
                printf("\n%-10s %7s %7s %2s %16s\n", "USER", "PID", "PPID", "ST", "NAME");
                printf("%-10s %7s %7s %2s %16s\n",
                       plist[i].who_Is_User,                      //User
                       plist[i].process[0].c_str(),            //PID
                       plist[i].process[3].c_str(),            //PPID
                       plist[i].process[2].c_str(),            //ST
                       plist[i].process[1].c_str());           //NAME
            }
                /* IO */
            else if (strcmp(plist[i].process[1].c_str(), "benchmark1") == 0 && ben1_1 && ben1_1 != pid && !ben1_2)
            {
                ben1_2 = 1;
                syscall(SLOWER, pid);

                cout << "\n[Second Monitoring Check]\t" << endl;
                cout << "\n[Slow syscall]\t" << pid << endl;
                printf("\n%-10s %7s %7s %2s %16s\n", "USER", "PID", "PPID", "ST", "NAME");
                printf("%-10s %7s %7s %2s %16s\n",
                       plist[i].who_Is_User,                      //User
                       plist[i].process[0].c_str(),            //PID
                       plist[i].process[3].c_str(),            //PPID
                       plist[i].process[2].c_str(),            //ST
                       plist[i].process[1].c_str());           //NAME
            }
        }
        delete mprocfs;
    }
    return 0;
}
