#include <iostream>
#include <fstream>
#include "pin.H"

using std::cerr;
using std::ofstream;
using std::ios;
using std::string;
using std::endl;

ofstream OutFile;

VOID record_trace(INS ins) {
    ADDRINT ip = INS_Address(ins);
    string format = hexstr(ip) + " " + INS_Disassemble(ins);
    OutFile << format << endl;
}

VOID Trace(TRACE trace, VOID* v) {

    for (BBL bbl = TRACE_BblHead(trace);BBL_Valid(bbl);bbl = BBL_Next(bbl)) {
        for (INS ins = BBL_InsHead(bbl); INS_Valid(ins);ins = INS_Next(ins)) {
            record_trace(ins);
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

int main(int argc, char* argv[]) {

    if (PIN_Init(argc, argv)) return Usage();

    OutFile.open(KnobOutputFile.Value().c_str());

    TRACE_AddInstrumentFunction(Trace, 0);
    PIN_AddFiniFunction(Fini, 0);

    PIN_StartProgram();
    return 0;
}
