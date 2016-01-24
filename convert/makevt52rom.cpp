#include <fstream>
#include <iomanip>
#include <iostream>

using namespace std;

int main()
{
    fstream f("vt52rom.dat");
    int count = 0;
    int d;
    cout << "#ifndef VT52ROM_H" << endl;
    cout << "#define VT52ROM_H" << endl;
    cout << endl;
    cout << "const int vt52rom[] = {" << endl;
    while(f >> oct >> d) {
        d = ~d & 127;
        if ((count & 7) == 0) cout << "    ";
        cout << "0x" << hex << setw(2) << setfill('0') << d;
        ++count;
        if ((count & 7) == 0) {
            cout << ((count == 1024) ? " " : ",");
            int c = (count-1) >> 3;
            cout << " // " << dec << c;
            if (isprint(c)) cout << " '" << static_cast<char>(c) << "'";
            cout << endl;
        } else {
            cout << ", ";
        }
    }
    cout << "};" << endl;
    cout << endl;
    cout << "#endif // !defined(VT52ROM_H)" << endl;
    cout << endl;
    return 0;
}

