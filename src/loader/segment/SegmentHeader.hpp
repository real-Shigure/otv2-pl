#pragma once

#include "../../Vendor.hpp"

class SegmentHeader {

public:

    //DllMain function. (Used for call OEP / Docs: https://docs.microsoft.com/en-us/windows/win32/dlls/dllmain)
    typedef int (__stdcall* DLLMAIN) (HMODULE hModule, DWORD callReason, LPVOID lpReserved);

    enum class ImportType {

        /**
              What is internal ? Let's look at a simple function.

                 sub01_0x1:

                     int myImportSuperValue = (0xFFFFFFFF) (mySuperValue);

              This function receives the value from the import (some dll) and writes it to a variable. This is the Internal.
         **/

        INTERNAL,

        //What is public? This is a global variable that points to function import.
        PUBLIC

    };

    struct ImportInfo {

        //Import function name.
        const char* m_function;
        //Location in segment where need patch. (RVA only)
        std::multimap <ImportType, DWORD> m_offsetsMap;

    };


public:
    
    SegmentHeader () {
        FillImports     ();
        FillRelocations ();
    }

    std::map <const char*, std::vector <ImportInfo>> GetImports () {
        return m_imports;
    }

    std::vector <DWORD> GetRelocations () {
        return m_relocations;
    }

private:

    //Simple functions for fill data-vectors.
    void FillImports (), FillRelocations ();

    //Need for segment can call the necessary functions from libraries. Storage model: Module -> ImportInfo [Function, offsets ^ n].
    std::map <const char*, std::vector <ImportInfo>> m_imports;
    //Need for reconstruct own variables/methods call.
    std::vector <DWORD> m_relocations;

};