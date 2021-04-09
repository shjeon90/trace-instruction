#include <iostream>
#include <fstream>
#include <sstream>
//#include <vector>
#include "pin.H"

using std::cerr;
using std::ofstream;
using std::ios;
using std::string;
using std::endl;

ofstream OutFile;
string target_binary_name;
ADDRINT img_low_address;
ADDRINT img_high_address;
ADDRINT txt_low_address;
ADDRINT txt_high_address;

BOOL check_main_section() {
    return FALSE;
}

VOID record_trace(INS ins) {
    ADDRINT ip = INS_Address(ins);
    if ((txt_low_address <= ip) && (ip < txt_high_address)) {
        OutFile << "0x" << std::hex << ip << " " << INS_Disassemble(ins) << endl;
    }
}

VOID Trace(TRACE trace, VOID* v) {

    for (BBL bbl = TRACE_BblHead(trace);BBL_Valid(bbl);bbl = BBL_Next(bbl)) {
        for (INS ins = BBL_InsHead(bbl); INS_Valid(ins);ins = INS_Next(ins)) {
            record_trace(ins);
        }
    }
}

VOID ImageLoad(IMG img, VOID* v) {
    string image_name = IMG_Name(img);
    if (image_name.find(target_binary_name) == string::npos) return;
    img_low_address = IMG_LowAddress(img);
    img_high_address = IMG_HighAddress(img);

    std::cout << "Iamge name: " << image_name << endl;
    std::cout << "Address space(" << image_name << "): 0x" << std::hex << img_low_address << " - 0x" << std::hex << img_high_address << endl;

    for (SEC sec = IMG_SecHead(img); SEC_Valid(sec);sec = SEC_Next(sec)) {
        std::cout << "Section: " << SEC_Name(sec) << " at address 0x" << std::hex << SEC_Address(sec) << " - 0x" << std::hex << SEC_Address(sec) +
                SEC_Size(sec) << endl;

        if (SEC_Name(sec).find("text") != string::npos) {
            txt_low_address = SEC_Address(sec);
            txt_high_address = SEC_Address(sec) + SEC_Size(sec);
        }
    }
}

KNOB<string> KnobOutputFile(KNOB_MODE_WRITEONCE, "pintool", "o", "inscount.out", "specify output file name");

VOID Fini(INT32 code, VOID* v) {
    OutFile.setf(ios::showbase);
    OutFile.close();
}

INT32 Usage() {
    cerr << "This tool counts the number of dynamic instructions executed" << endl;
    cerr << endl << KNOB_BASE::StringKnobSummary() << endl;
    return -1;
}

VOID parse_binary_name(int argc, char* argv[]) {
    string last_arg = argv[argc-1];
    std::cout << last_arg << endl;
    std::istringstream ss(last_arg);
    string tmp;
    std::vector<string> buffer;
    while(getline(ss, tmp, '/')) buffer.push_back(tmp);

    target_binary_name = buffer[buffer.size()-1];
    std::cout << "binary name: " << target_binary_name << endl;
}

int main(int argc, char* argv[]) {

    if (PIN_Init(argc, argv)) return Usage();

    for (int i = 0;i < argc;++i) {
        std::cout << "argv[" << i << "]: " << argv[i] << endl;
    }

    parse_binary_name(argc, argv);

    OutFile.open(KnobOutputFile.Value().c_str());

    IMG_AddInstrumentFunction(ImageLoad, 0);
    TRACE_AddInstrumentFunction(Trace, 0);
    PIN_AddFiniFunction(Fini, 0);

    PIN_StartProgram();
    return 0;
}
