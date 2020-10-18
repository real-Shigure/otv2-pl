#include "../Vendor.hpp"

HMODULE CommonUtil::GetModule (const char* module) {

   HMODULE hModule = GetModuleHandleA (module);

   return !hModule ? LoadLibraryA (module) : hModule;
}

MODULEINFO CommonUtil::GetModuleInfo (const char* module) {

    //Docs: https://docs.microsoft.com/en-us/windows/win32/api/psapi/nf-psapi-getmoduleinformation.
    MODULEINFO modinfo = { 0 };

    GetModuleInformation (GetCurrentProcess (), GetModule (module), &modinfo, sizeof (MODULEINFO));

    return modinfo;
}

DWORD CommonUtil::GetFunction (const char* module, const char* function) {
   return reinterpret_cast <DWORD> (GetProcAddress (GetModule (module), function));
}

DWORD CommonUtil::SearchSignature (const char* module, const char* signature) {

    std::vector <unsigned char> patterns;
    std::string masks;

    auto UnpackSignature = [] (const char* packedSignature, std::vector <unsigned char>& pattern, std::string& mask) {

        std::vector <std::string> tokens;
        std::string signature (packedSignature);

        tokens.clear ();

        size_t start = 0, end = signature.find (" "), delimLength = std::string (" ").length ();

        while (end != std::string::npos) {
            tokens.emplace_back (signature.substr (start, end - start));
            start = end + delimLength, end = signature.find (" ", start);
        }

        tokens.emplace_back (signature.substr (start));

        size_t count = tokens.size (); 
        
        if (!count) {
            PanicUtil::Release (PanicUtil::Layers::ROUTINE, "CommonUtil::SearchSignature", "Tokens size is null.");
            return false;
        }

        pattern.reserve (count);
        mask.reserve    (count);

        for (const auto& token : tokens) {

            if (token == "?") {
                pattern.push_back (0x00);
                mask += "?";
            }

            else {
                pattern.push_back (static_cast <unsigned char> (std::stoi (token, nullptr, 16)));
                mask += ".";
            }

        }

        return true;
    };

    if (UnpackSignature (signature, patterns, masks)) {

        MODULEINFO moduleInfo = GetModuleInfo (module);

        const char* pattern = reinterpret_cast <const char*> (std::data (patterns));
        const char* mask = masks.c_str ();

        DWORD patternLength = strlen (mask), baseAddress = reinterpret_cast <DWORD> (moduleInfo.lpBaseOfDll);

        if (baseAddress == 0 || patternLength > moduleInfo.SizeOfImage) { patterns.clear (); masks.clear (); return NULL; }

        for (DWORD offset = 0; offset < baseAddress - patternLength; offset++) {

            bool isFound = true;

            for (DWORD charIterator = 0; charIterator < patternLength; charIterator++) {
                isFound &= mask [charIterator] == '?' || pattern [charIterator] == *reinterpret_cast <const char*> (baseAddress + offset + charIterator);
            }

            if (isFound) {
                return baseAddress + offset;
            }

        }

    }

    { patterns.clear(); masks.clear(); return NULL; }

}

bool CommonUtil::FindOffsetsToVec (const char* module, std::vector <const char*> signatures, std::vector<DWORD>& vector) {

    for (const auto& signature : signatures) { 

        DWORD offset = SearchSignature (module, signature);

        if (!offset) {
            return false;
        }

        vector.emplace_back (SearchSignature (module, signature));

    }

    return true;

}

bool CommonUtil::IsWinXPOrLater () {

    //Docs: https://docs.microsoft.com/en-us/windows/win32/api/sysinfoapi/nf-sysinfoapi-getversionexa.

    OSVERSIONINFO osVersionInfo;
    ZeroMemory (&osVersionInfo, sizeof (OSVERSIONINFO));

    //This causes a little confused. 
    //
    //If we were unable to fill the structure, then we return true, because the function expects false, 
    //this will lead to the fact that the initialization of the loader simply stops, and through the debugger you can see what is wrong. 
    if (GetVersionEx (&osVersionInfo)) return true;

    return ((osVersionInfo.dwMajorVersion > 5) || ((osVersionInfo.dwMajorVersion == 5) && (osVersionInfo.dwMinorVersion >= 1)));
}

bool CommonUtil::IsWinServBuild () {

    //Docs: https://docs.microsoft.com/en-us/windows/win32/api/sysinfoapi/nf-sysinfoapi-getversionexa.

    OSVERSIONINFOEX osVersionInfo;
    ZeroMemory (&osVersionInfo, sizeof (OSVERSIONINFO));

    //This causes a little confused. 
    //
    //If we were unable to fill the structure, then we return true, because the function expects false, 
    //this will lead to the fact that the initialization of the loader simply stops, and through the debugger you can see what is wrong.
    if (GetVersionEx ((LPOSVERSIONINFO) &osVersionInfo)) return true;

    return osVersionInfo.wProductType == VER_NT_WORKSTATION;
}

bool CommonUtil::IsRedistPackagePresent () {

    //Source: https://stackoverflow.com/questions/12206314/detect-if-visual-c-redistributable-for-visual-studio-2012-is-installed.
    //All packages keys for x32 version.

    std::map <HKEY, std::wstring> regSubKeys = {
        //Visual C++ 2010.
        { HKEY_LOCAL_MACHINE, L"SOFTWARE\\Classes\\Installer\\Products\\67D6ECF5CD5FBA732B8B22BAC8DE1B4D" },
        //Visual C++ 2012.
        { HKEY_LOCAL_MACHINE, L"SOFTWARE\\Classes\\Installer\\Dependencies\\{33d1fd90-4274-48a1-9bc1-97e33d9c2d6f}" },
        //Visual C++ 2013.
        { HKEY_LOCAL_MACHINE, L"SOFTWARE\\Classes\\Installer\\Dependencies\\{f65db027-aff3-4070-886a-0d87064aabb1}" },
        //Visual C++ 2015.
        { HKEY_LOCAL_MACHINE, L"SOFTWARE\\Classes\\Installer\\Dependencies\\{e2803110-78b3-4664-a479-3611a381656a}" },
        //Visual C++ 2017.
        { HKEY_CLASSES_ROOT,  L"Installer\\Dependencies\\VC,redist.x86,x86,14.16,bundle\\Dependents\\{67f67547-9693-4937-aa13-56e296bd40f6}" },
        //Visual C++ 2019. (Pack)
        { HKEY_CLASSES_ROOT,  L"Installer\\Dependencies\\VC,redist.x86,x86,14.21,bundle\\Dependents\\{49697869-be8e-427d-81a0-c334d1d14950}" },
        { HKEY_CLASSES_ROOT,  L"Installer\\Dependencies\\VC,redist.x86,x86,14.22,bundle\\Dependents\\{5bfc1380-fd35-4b85-9715-7351535d077e}" },
    };
   
    for (const auto& key : regSubKeys) {

        if (RegOpenKeyExW (key.first, key.second.c_str (), 0, KEY_READ, nullptr)) { 
            RegCloseKey   (key.first);
            return true;
        }

    }

    return false;
}

const std::string CommonUtil::GetCurrentTimeAsString () {

    time_t currentTime =  time (0);
    struct tm tstruct  = *localtime (&currentTime);
    char rawTime [20];

    strftime (rawTime, sizeof (rawTime), "%H:%M:%S", &tstruct);

    return rawTime;
}