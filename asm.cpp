/*--------------------------------------------------------------------------

TITLE    : Souce code for emulater
NAME     : AKARSH SRIVASTAVA
Roll No. : 2201CS10
Declaration of Authorship
This C++ file, asm.cpp, is part of the miniproject of CS210 at the
Department of Computer Science and Engg, IIT Patna.

----------------------------------------------------------------------------*/

#include <bits/stdc++.h>
using namespace std;

typedef vector<int> vi;
typedef vector<unsigned int> vii;
typedef vector<string> vs;
typedef map<int, int> mpp;
typedef pair<int, int> pi;
typedef map<string, int> mpsi;
typedef map<int, string> mpis;
typedef pair<int, string> pis;
typedef pair<string, int> psi;

struct table
{
    string lbl;
    string mnm;
    string oprd;
    int oprType;
    bool lblPresent;
};

// data structures used
vector<table> data;
map<string, psi> OPC;
vector<pis> errs;
vs clnd;
mpsi lbls;
vector<pis> mac_Cd;
vi PC;
bool haltPresent = 0;
string fn;

// various functions used
bool isOctal(string s);
bool isHexadecimal(string s);
bool isDecimal(string s);
bool validName(string cur);
string decToHex(int number, int add);
void init();
void generrs(int line, string type);
string remove_blanks(string s, int line);
void insert_inst(vector<string> &ans, string token, string s, int j);
void process_lbl();
void fill_data(int i, string one, string two, string three, int type);
int calType(string s);
void table_f();
void create_instruc();
void Pass_1();
bool check_err();
string appZero(string s,int sz);
void Pass_2();
void file_writing();

int main()
{

    // calling initailising function
    init();
    // calling first pass
    Pass_1();
    int x = check_err();
    if (x != 0)
    {
        Pass_2();
        file_writing();
    }
    return 0;
}

void init()
{
    OPC.insert({"data", {"", 1}});
    OPC.insert({"ldc", {"00", 1}});
    OPC.insert({"adc", {"01", 1}});
    OPC.insert({"ldl", {"02", 2}});
    OPC.insert({"stl", {"03", 2}});
    OPC.insert({"ldnl", {"04", 2}});
    OPC.insert({"stnl", {"05", 2}});
    OPC.insert({"add", {"06", 0}});
    OPC.insert({"sub", {"07", 0}});
    OPC.insert({"shl", {"08", 0}});
    OPC.insert({"shr", {"09", 0}});
    OPC.insert({"adj", {"0A", 1}});
    OPC.insert({"a2sp", {"0B", 0}});
    OPC.insert({"sp2a", {"0C", 0}});
    OPC.insert({"call", {"0D", 2}});
    OPC.insert({"return", {"0E", 0}});
    OPC.insert({"brz", {"0F", 2}});
    OPC.insert({"brlz", {"10", 2}});
    OPC.insert({"br", {"11", 2}});
    OPC.insert({"HALT", {"12", 0}});
    OPC.insert({"SET", {"", 1}});
}

string decToHex(int numb, int opr = 24)
{
    if (opr == 32)
    {
        unsigned int num = numb;
        stringstream ss;
        ss << hex << num;
        return ss.str();
    }
    string ans = "";
    int n = numb;
    while (n)
    {
        unsigned int r = n % 16;
        char c;
        if (r < 10)
            c = r + 48;
        else
            c = r + 55;
        ans += c;
        n /= 16;
    }
    if (numb < 0)
    {
        numb += (1 << opr);
    }

    stringstream ss;
    ss << hex << numb;
    return ss.str();
}

void generrs(int line, string type)
{
    string str = "ERROR found at line: " + to_string(line) + " -- Type of ERROR: " + type;
    errs.push_back({line + 1, str});
}

string remove_blanks(string s, int line)
{
    // This function remove unnecessary spaces
    reverse(s.begin(), s.end());
    while (s.back() == ' ' || s.back() == '\t')
    {
        s.pop_back();
    }
    reverse(s.begin(), s.end());
    while (s.back() == ' ' || s.back() == '\t')
    {
        s.pop_back();
    }
    string ans;
    int i = 0;
    int x = s.size();
    while (i < x)
    {

        if (s[i] == ';')
        {
            break;
        }
        if (s[i] == ':')
        {
            ans += ":";
            if (i == x - 1)
            {
                ans += " ";
                i++;
                continue;
            }
            if (s[i + 1] != ' ')
            {
                ans += " ";
                i++;
                continue;
            }
            i++;
            continue;
        }
        if (s[i] != ' ' && s[i] != '\t')
        {
            ans += s[i];
            i++;
            continue;
        }
        ans += " ";
        int j = i;
        while (s[i] == s[j] && j < x)
        {
            j++;
        }
        i = j - 1;
        i++;
    }
    while (!ans.empty() && (ans.back() == ' ' || ans.back() == '\t'))
    {
        ans.pop_back();
    }
    int spac = 0;
    for (int i = 0; i < (int)ans.size(); i++)
    {
        spac += (ans[i] == ' ');
    }
    if (spac > 2)
    {
        generrs(line + 1, "INVALID SYNTAX FOUND");
    }
    return ans;
}

void insert_inst(vector<string> &ans, string token, string str, int j)
{
    if (j + 5 < str.size())
    {
        string s = "adj 10000";
        ans.push_back(s);
        s.clear();
        s = "stl -1";
        ans.push_back(s);
        s.clear();
        s = "stl 0";
        ans.push_back(s);
        s.clear();
        s = "ldc ";
        string s2 = str.substr(j + 6, str.size() - (j + 6));
        s += s2;
        ans.push_back(s);
        s.clear();
        s = "ldc " + token.substr(0, j);
        s.clear();
        ans.push_back(s);
        s.clear();
        s = "stnl 0";
        ans.push_back(s);
        s.clear();
        s = "ldl 0";
        ans.push_back(s);
        s.clear();
        s = "ldl -1";
        ans.push_back(s);
        s.clear();
        s = "adj -10000";
        ans.push_back(s);
        s.clear();
    }
}

void set_implemen()
{
    vector<string> ans;
    int ind = 0;
    for (auto i1 : clnd)
    {
        string cur;
        bool state = 0;
        int j_ind = 0;
        for (auto j1 : i1)
        {
            cur += j1;
            if (j1 == ':')
            {
                cur.pop_back();
                if (i1.size() > j_ind + 5 && i1.substr(j_ind + 2, 3) == "SET")
                {
                    state = 1;
                    if ((lbls[cur]) == ind)
                    {
                        lbls[cur] = (int)ans.size() - 1;
                        string z = i1.substr(j_ind + 6, (int)i1.size() - (j_ind + 6));
                        string y = i1.substr(0, j_ind + 1) + " data " + z;
                        ans.push_back(y);
                    }
                    else
                    {
                        insert_inst(ans, cur, i1, j_ind);
                    }
                    // j_ind++;
                    break;
                }
                j_ind++;
            }
            
        }
        if (!state and !i1.empty())
        {
            ans.push_back(i1);
        }
        ind++;
    }
    clnd = ans;
}

void process_lbl()
{
    int i_ind = 0;
    for (auto i1 : clnd)
    {
        string cur;
        int j_ind = 0;
        for (auto j1 : i1)
        {
            if (j1 == ':')
            {
                bool is = validName(cur);
                if (!is)
                {
                    string x = "Not a valid label name";
                    generrs(i_ind + 1, x);
                    // j_ind++;
                    break;
                }
                if (lbls.count(cur))
                {
                    if (i1.size() > j_ind + 4 && i1.substr(j_ind + 2, 3) == "SET")
                    {
                        j_ind++;
                        continue;
                    }
                    if (i1.size() > j_ind + 5 && i1.substr(j_ind + 2, 4) == "data" && lbls[cur] < 0)
                    {
                        lbls[cur] = i_ind;
                        j_ind++;
                        continue;
                    }
                    string x = "Redeclaration of label found " + cur;
                    generrs(i_ind + 1, x);
                }
                if (i1.size() > j_ind + 4 && i1.substr(j_ind + 2, 3) == "SET")
                {
                    lbls[cur] = -i_ind;
                    j_ind++;
                    continue;
                }
                lbls[cur] = i_ind;
                // j_ind++;
                break;
            }
            cur += j1;
            j_ind++;
        }
        i_ind++;
    }
}

void fill_data(int i, string one, string two, string three, int type)
{
    data[i].lbl = one;
    data[i].oprType = type;
    data[i].mnm = two;
    data[i].oprd = three;
}

int calType(string s)
{
    if (!s.empty())
    {
        if (s[0] == '+' or s[0] == '-')
        {
            s = s.substr(1, string::npos);
        }
        if (s.empty())
        {
            return -1;
        }

        else if (isDecimal(s))
        {
            return 10;
        }

        else if (isOctal(s))
        {
            return 8;
        }

        else if (isHexadecimal(s))
        {
            return 16;
        }

        else if (validName(s))
        {
            return 1;
        }
    }
    else
    {
        return 0;
    }
    return -1;
}

void table_f()
{
    int pc = 0;
    int i_ind = 0;
    while (i_ind < (int)clnd.size())
    {
        string ans[10] = {"", "", "", ""}, cur = "";
        int ptr = 1;
        int j_ind = 0;
        while (j_ind < (int)clnd[i_ind].size())
        {
            char cl = clnd[i_ind][j_ind];
            if (cl == ':')
            {
                ans[0] = cur;
                cur = "";
                j_ind++;
                j_ind++;
                continue;
            }
            else if (cl == ' ')
            {
                ans[ptr++] = cur;
                cur = "";
                j_ind++;
                continue;
            }
            cur += cl;
            if (j_ind == (int)clnd[i_ind].size() - 1)
            {
                ans[ptr++] = cur;
            }
            j_ind++;
        }
        if (!ans[0].empty())
        {
            data[i_ind].lblPresent = 1;
        }
        else
        {
            data[i_ind].lblPresent = 0;
        }
        if (ans[1] == "HALT")
        {
            haltPresent = 1;
        }

        if (!ans[0].empty())
        {
            lbls[ans[0]] = pc;
        }

        PC[i_ind] = pc;
        if (ptr == 1)
        {
            fill_data(i_ind, ans[0], "", "", 0);
            i_ind++;
            continue;
        }
        pc++;
        if (!OPC.count(ans[1]))
        {
            generrs(i_ind + 1, "Not a valid mnemonic");
            i_ind++;
            continue;
        }
        if (min(OPC[ans[1]].second, 1) != ptr - 2)
        {
            generrs(i_ind + 1, "Not a valid syntax");
            i_ind++;
            continue;
        }
        fill_data(i_ind, ans[0], ans[1], ans[2], calType(ans[2]));
        if (data[i_ind].oprType == 1 && !lbls.count(data[i_ind].oprd))
        {
            generrs(i_ind + 1, "No such variable found");
        }
        else if (data[i_ind].oprType == -1)
        {
            generrs(i_ind + 1, "Not a valid number");
        }
        i_ind++;
    }
}

void create_instruc()
{
    vector<string> instr, dataseg;
    int i_ind = 0;
    for (auto i1 : clnd)
    {
        bool state = 0;
        int j_ind = 0;
        for (auto j1 : i1)
        {
            if (j_ind + 4 < (int)i1.size() && i1.substr(j_ind, 4) == "data")
            {
                dataseg.push_back(clnd[i_ind]);
                state = 1;
                // j_ind++;
                break;
            }
            if (clnd[i_ind].back() == ':' && i_ind + 1 < (int)clnd.size() && clnd[i_ind + 1].substr(0, 4) == "data")
            {
                dataseg.push_back(clnd[i_ind]);
                state = 1;
                // j_ind++;
                break;
            }
            j_ind++;
        }
        if (!state)
        {
            instr.push_back(clnd[i_ind]);
        }
        i_ind++;
    }
    instr.insert(instr.end(), dataseg.begin(), dataseg.end());
    clnd = instr;
}

void func()
{
    data.resize((int)clnd.size());
    PC.resize((int)clnd.size());
    create_instruc();
    table_f();
}

void Pass_1()
{
    ifstream infile;
    printf("Enter file (file format .asm) :\n ");
    cin >> fn;
    string check = "";
    unsigned int sz = fn.size();
    for (unsigned int i = sz - 1; i >= 0; i--)
    {
        check += fn[i];
        if (check.size() == 3)
            break;
    }
    reverse(check.begin(), check.end());
    if (check != "asm")
    {
        printf("INVALID FILE FORMAT (File format should be of .asm)");
        return;
    }
    infile.open(fn);
    if (infile.fail())
    {
        printf("File doesn't exist\n");
        exit(0);
    }
    string s;
    while (getline(infile, s))
    {
        string cur = remove_blanks(s, (int)clnd.size());
        clnd.push_back(cur);
    }
    init();
    process_lbl();
    if (errs.empty())
    {
        set_implemen();
    }
    func();
}

bool check_err()
{
    int s = fn.size();
    int ind = 0;
    for (int i = s - 1; i >= 0; i--)
    {
        if (fn[i] == '.')
        {
            ind = i;
            break;
        }
    }
    string y = "";
    for (int i = 0; i < ind; i++)
    {
        y += fn[i];
    }
    string lg = y + ".log";
    ofstream outerrs(lg);
    outerrs << "Log code generated in : " << lg << endl;
    if (errs.empty())
    {
        printf("NO EERORS DETECTED\n");
        if (haltPresent == 0)
        {
            printf("WARNING FOUND\n ");
            outerrs << "Warning: HALT NOT FOUND" << endl;
        }
        outerrs << "Machine code generated in: " << y + ".o" << endl;
        outerrs << "Listing code generated in: " << y + ".l" << endl;
        outerrs.close();
        return 1;
    }
    sort(errs.begin(), errs.end());
    printf("%d", (int)errs.size());
    printf(" ERRORS\n ");
    cout << lg << endl;

    s = errs.size();
    for (int i = 0; i < s; i++)
    {
        outerrs << errs[i].second << endl;
    }
    outerrs.close();
    return 0;
}

string appZero(string s, int sz = 6)
{
    for (; (int)s.size() < sz;)
    {
        s = '0' + s;
    }

    return s;
}

void Pass_2()
{
    int i_ind = 0;
    for (auto i1 : data)
    {
        if (clnd[i_ind].empty())
        {
            i_ind++;
            continue;
        }
        string location = appZero(decToHex(PC[i_ind]));
        if (i1.mnm == "")
        {
            string curMacCode = "        ";
            mac_Cd.push_back({i_ind, curMacCode});
            i_ind++;
            continue;
        }
        if (i1.oprType == 1)
        {
            int decForm;
            if (OPC[i1.mnm].second == 2)
            {
                int val = lbls[i1.oprd];
                decForm = val - (PC[i_ind] + 1);
            }
            else
            {
                decForm = lbls[i1.oprd];
            }
            string curMacCode = appZero(decToHex(decForm)) + OPC[i1.mnm].first;
            mac_Cd.push_back({i_ind, curMacCode});
        }
        else if (i1.oprType == 0)
        {
            string s = "000000";
            string curMacCode = s + OPC[data[i_ind].mnm].first;
            mac_Cd.push_back({i_ind, curMacCode});
        }
        else
        {
            int sz = 6, add = 24;
            if (i1.mnm == "data")
            {
                sz = 8;
                add = 32;
            }

            int decForm = stoi(i1.oprd, 0, i1.oprType);
            string curMacCode = appZero(decToHex(decForm, add), sz) + OPC[i1.mnm].first;
            mac_Cd.push_back({i_ind, curMacCode});
        }
        i_ind++;
    }
}

void file_writing()
{
    int s = fn.size();
    int ind = 0;
    for (int i = s - 1; i >= 0; i--)
    {
        if (fn[i] == '.')
        {
            ind = i;
            break;
        }
    }
    string y = "";
    for (int i = 0; i < ind; i++)
    {
        y += fn[i];
    }
    ofstream outList(y + ".l");
    s = mac_Cd.size();
    for (int i = 0; i < s; i++)
    {
        outList << appZero(decToHex(PC[mac_Cd[i].first])) << " " << mac_Cd[i].second << " " << clnd[mac_Cd[i].first] << endl;
    }
    outList.close();
    ofstream outmac_Cd;
    outmac_Cd.open(y + ".o", ios::binary | ios::out);
    for (int i = 0; i < s; i++)
    {
        unsigned int x;
        if (mac_Cd[i].second.empty() || mac_Cd[i].second == "        ")
        {
            continue;
        }

        stringstream ss;
        ss << hex << mac_Cd[i].second;
        ss >> x; // output it as a signed type
        static_cast<int>(x);
        outmac_Cd.write((const char *)&x, sizeof(unsigned int));
    }
    outmac_Cd.close();
    printf("Log code created in: ");
    cout << y + ".log" << endl;
    printf("Machine code created in: ");
    cout << y + ".o" << endl;
    printf("Listing code created in: ");
    cout << y + ".l" << endl;
}

unsigned int binary_to_decimal(string bin)
{
    unsigned int ans = 0;
    reverse(bin.begin(), bin.end());
    unsigned int j = 0;
    for (auto itr : bin)
    {
        if (itr == '1')
            ans += (1 << j);
        j++;
    }
    return ans;
}

string decimal_to_binary(unsigned int n)
{
    string ans;
    while (n)
    {
        if ((n & 1))
            ans += '1';
        else
            ans += '0';
        n >>= 1;
    }
    if (ans.empty())
        return "0";
    reverse(ans.begin(), ans.end());
    return ans;
}

unsigned int hex_to_decimal(string bin)
{
    unsigned int ans = 0;
    reverse(bin.begin(), bin.end());
    unsigned int j = 1;
    for (auto itr : bin)
    {
        if (itr >= '0' && itr <= '9')
        {
            ans += (itr - 48) * j;
            j = j * 16;
        }
        else
        {
            ans += (itr - 55) * j;
            j = j * 16;
        }
    }
    return ans;
}

string decimal_to_hex(unsigned int n)
{
    string ans;
    while (n)
    {
        unsigned int r = n % 16;
        char c;
        if (r < 10)
            c = r + 48;
        else
            c = r + 55;
        ans += c;
        n /= 16;
    }
    if (ans.empty())
        return "0";
    reverse(ans.begin(), ans.end());
    return ans;
}

unsigned int oct_to_decimal(string bin)
{
    unsigned int ans = 0;
    reverse(bin.begin(), bin.end());
    unsigned int j = 1;
    for (auto itr : bin)
    {
        ans += (itr - 48) * j;
        j = j * 8;
    }
    return ans;
}

string decimal_to_oct(unsigned int n)
{
    string ans;
    while (n)
    {
        unsigned int r = n % 16;
        char c;
        c = r + 48;
        ans += c;
        n /= 16;
    }
    if (ans.empty())
        return "0";
    reverse(ans.begin(), ans.end());
    return ans;
}

bool isOctal(string str)
{
    if ((int)str.size() < 2)
    {
        return 0;
    }

    if (str[0] != '0')
    {
        return 0;
    }

    for (int i = 1; i < str.size(); i++)
    {
        if (!(str[i] >= '0' && str[i] <= '7'))
        {
            return 0;
        }
    }
    return 1;
}
bool isHexadecimal(string str)
{
    if ((int)str.size() < 3)
    {
        return 0;
    }

    if (str[0] != '0')
        return 0;
    if (str[1] != 'x' && str[1] != 'X')
        return 0;

    for (int i = 2; i < str.size(); i++)
    {
        if (!(((str[i] >= '0' && str[i] <= '9')) || ((str[i] >= 'a' && str[i] <= 'f')) || ((str[i] <= 'A' && str[i] <= 'F'))))
            return 0;
    }
    return 1;
}
bool isDecimal(string str)
{
    for (int i = 0; i < str.size(); i++)
    {
        if (!(str[i] >= '0' && str[i] <= '9'))
            return 0;
    }
    return 1;
}

bool validName(string str)
{

    for (int i = 0; i < (int)str.size(); i++)
    {
        if ((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z') || (str[i] >= '0' && str[i] <= '9') || (str[i] == '_'))
        {
            continue;
        }

        return 0;
    }
    if ((str[0] >= 'a' && str[0] <= 'z') || (str[0] >= 'A' && str[0] <= 'Z') || (str[0] == '_'))
        return 1;
    return 0;
}