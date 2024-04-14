/*--------------------------------------------------------------------------

TITLE    : Souce code for emulater
NAME     : AKARSH SRIVASTAVA
Roll No. : 2201CS10
Declaration of Authorship
This C++ file, emu.cpp, is part of the miniproject of CS210 at the
Department of Computer Science and Engg, IIT Patna.

----------------------------------------------------------------------------*/


#include <bits/stdc++.h>
using namespace std;

#define ui unsigned int


// memory
const int n = 1 << 24;
vector<ui> mem(n, 0);

// registers
int SP = 0;
int A = 0;
int B = 0;
int PC = 0;

// table for functions
void func(int m_code, int oprd)
{
    if (m_code == 17)
    {
        // br
        PC += oprd;
    }
    else if (m_code == 16)
    {
        // brlz
        if (A < 0)
            PC += oprd;
    }
    else if (m_code == 15)
    {
        // brz
        if (A == 0)
            PC += oprd;
    }
    else if (m_code == 14)
    {
        // return
        PC = A;
        A = B;
    }
    else if (m_code == 13)
    {
        // call
        B = A;
        A = PC;
        PC += oprd;
    }
    else if (m_code == 12)
    {
        // sp2a
        B = A;
        A = SP;
    }
    else if (m_code == 11)
    {
        // a2sp
        SP = A;
        A = B;
    }
    else if (m_code == 10)
    {
        // adj
        SP += oprd;
    }
    else if (m_code == 9)
    {
        // shr
        A = B >> A;
    }
    else if (m_code == 8)
    {
        // shl
        A = B << A;
    }
    else if (m_code == 7)
    {
        // sub
        A = B - A;
    }
    else if (m_code == 6)
    {
        // add
        A += B;
    }
    else if (m_code == 5)
    {
        // stnl
        mem[A + oprd] = B;
    }
    else if (m_code == 4)
    {
        // ldnl
        A = mem[A + oprd];
    }
    else if (m_code == 3)
    {
        // stl
        mem[SP + oprd] = A;
        A = B;
    }
    else if (m_code == 2)
    {
        // ldl
        B = A;
        A = mem[SP + oprd];
    }
    else if (m_code == 1)
    {
        // adc
        A += oprd;
    }
    else if (m_code == 0)
    {
        // ldc
        B = A;
        A = oprd;
    }
}

ui file_reading(fstream &ipt_f);
string string_to_hexadecimal(ui dec);
ui accesing_lines(fstream &fl);
ui access_num(fstream &fl);

// Reading the .o file
ui file_reading(fstream &ipt_f)
{
    ui lns = accesing_lines(ipt_f);
    ui op;
    ui i = 0;
    while (i < lns)
    {
        op = access_num(ipt_f);
        mem[i] = op;
        i++;
    }
    return lns;
}

// converting decimal to hexadecimal
string string_to_hexadecimal(ui dec)
{
    stringstream hex_str;
    hex_str << setfill('0') << setw(8) << hex << dec;
    return hex_str.str();
}

// getting count of number of lines in .o file
ui accesing_lines(fstream &fl)
{
    fl.seekg(0, ios::end);
    ui f_sz = fl.tellg();
    fl.seekg(0, ios::beg);
    return f_sz / 4;
}

// reading integers from the .o file
ui access_num(fstream &fl)
{
    ui num = 0;
    fl.read((char *)&num, sizeof(num));
    return num;
}

// function for printing memmory dump to file

void file_mem_dump(ofstream &fl, ui ln)
{
    ui i = 0;
    while (i < ln)
    {
        if (!(i % 4))
        {
            fl << "\n0x" << string_to_hexadecimal(i);
        }
        fl << "\t0x" << string_to_hexadecimal(mem[i]);
        i++;
    }
    fl << '\n';
}

// function for printing the memory dump

void print_mem_dump(ui ln, bool f)
{
    if (!f)
    {
        printf("Showing memory dump after execution\n");
    }
    else
    {
        printf("Showing memory dump before execution\n");
    }
    ui i = 0;
    while (i < ln)
    {
        if (!(i % 4))
        {
            cout << "\n0x" << string_to_hexadecimal(i);
        }
        cout << "\t0x" << string_to_hexadecimal(mem[i]);
        i++;
    }
    cout << '\n';
}

// function for printing ISA

void __ISA__print()
{
    vector<vector<string>> mn_op;
    mn_op.push_back({"ldc", "val"});
    mn_op.push_back({"adc", "val"});
    mn_op.push_back({"ldl", "off"});
    mn_op.push_back({"stl", "off"});
    mn_op.push_back({"ldnl", "off"});
    mn_op.push_back({"stnl", "off"});
    mn_op.push_back({"add", ""});
    mn_op.push_back({"sub", ""});
    mn_op.push_back({"shl", ""});
    mn_op.push_back({"shr", ""});
    mn_op.push_back({"adj", "val"});
    mn_op.push_back({"a2sp", ""});
    mn_op.push_back({"sp2a", ""});
    mn_op.push_back({"call", "off"});
    mn_op.push_back({"return", ""});
    mn_op.push_back({"brz", "off"});
    mn_op.push_back({"brlz", "off"});
    mn_op.push_back({"br", "off"});
    mn_op.push_back({"HALT", ""});

    printf("Opcode\t");
    printf("Mnemonic\t");
    printf("Operand\n");

    int i = 0;
    while (i < 19)
    {
        cout << i << "\t" << mn_op[i][0] << "\t\t" << mn_op[i][1] << '\n';
        i++;
    }
    return;
}

// function execution

int EX(bool f)
{

    // initialising stack pointer
    SP = 8388607;

    ui oprd_l = 0;
    ui instr_num = 0;
    ui mem_cd = 0;
    ui oprd = 0;

    for (; mem_cd <= 18;)
    {
        // getting last 8 bits of mem_code
        int m = 1 << 8;
        mem_cd = mem[PC] % m;
        if (mem_cd == 18)
        {
            break;
        }
        else if (mem_cd > 18)
            return 1;

        // access operand
        oprd_l = mem[PC] >> 8;
        if (oprd_l > 8388607)
        {
            oprd = (int)oprd_l - n;
        }
        else
        {
            oprd = (int)oprd_l;
        }

        // implemeting trace printing according to bool f
        if (f)
        {
            printf("PC = 0x");
            cout << string_to_hexadecimal(PC);
            printf("\tA = 0x");
            cout << string_to_hexadecimal(A);
            printf("\tB = 0x");
            cout << string_to_hexadecimal(B);
            printf("\tSP = 0x");
            cout << string_to_hexadecimal(SP);
            cout << '\n';
        }

        func(mem_cd, oprd);
        PC++;
        instr_num++;
    }
    cout << '\n';
    printf("%d", instr_num);
    printf(" instruction executed.\n");
    return 0;
}

int main()
{
    printf("Enter file name (for example filename.o) : ");
    string fl;
    cin >> fl;
    printf("Enter option : \n");
    printf("Uses \n");
    cout << "trace : For showing instruction trace.\n";
    cout << "after : For showing memory dump after execution.\n";
    cout << "before : For showing memory dump before execution.\n";
    cout << "isa : For displaying ISA.\n";
    printf("Enter option : \n");
    string arg;
    cin >> arg;
    int tr = 0;
    int bf = 0;
    int af = 0;
    int isa = 0;
    int ln;
    if (arg == "isa")
        isa = 1;
    else if (arg == "trace")
        tr = 1;
    else if (arg == "before")
        bf = 1;
    else if (arg == "after")
        af = 1;
    else
    {
        cout << "Inavalid option given.\n";
        return 0;
    }

    fstream ipt_f(fl, ios::binary | ios::in);

    if (ipt_f.is_open())
    {
        ln = file_reading(ipt_f);
        ipt_f.close();
    }
    else
    {
        cout << "ERROR : File is empty\n";
        return 0;
    }

    if (bf)
        print_mem_dump(ln, 1);
    else if (isa)
        __ISA__print();

    int er = EX(tr);
    if (er == 1)
    {
        printf("ERROR : Invalid mnemonic found.\n");
        return 0;
    }

    if (af)
    {
        print_mem_dump(ln, 0);
    }

    ofstream memd_fl;
    string memd_fl_nm = fl.append("_memdump.txt");
    memd_fl.open(memd_fl_nm, ios::out);

    file_mem_dump(memd_fl, ln);
    memd_fl.close();
    return 0;
}