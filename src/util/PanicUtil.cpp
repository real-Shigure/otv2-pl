#include "../Vendor.hpp"
#include "../loader/runtime/RuntimeHandler.hpp"

void PanicUtil::SetImportant (DWORD *variable, DWORD value, const char* caller, const char* reason) {

    if (variable && value) {
        *variable = value;
    }
    else {
        PanicUtil::Release (Layers::ROUTINE, caller, reason);
    }

}

void PanicUtil::RequireNonNull (Layers layer, DWORD value, const char* caller, const char* reason) {

    if (!value) {
        PanicUtil::Release (layer, caller, reason);
    }

}

void PanicUtil::Release (Layers causedInLayer, const char* caller, const char* reason) {

    OutputDebugStringA (std::string ("[Primal-engine] Panic released.").append ("\n\nLayer: ").append (ParseTypeLayers (causedInLayer)).append(".\nReason: ").append (reason).append ("\nCaller: ").append (caller).c_str ());

    if (!reinterpret_cast <DWORD> (Primal::UnCurrentModule)) {
        //Debug to process and watch exit code for get solution.
        ExitProcess (0xFFFA1);
    }

    if (!Primal::UnSegmentAllocate) {
        FreeLibraryAndExitThread (reinterpret_cast<HMODULE> (Primal::UnCurrentModule), NULL);
    }

    VirtualFree (reinterpret_cast<LPVOID> (Primal::UnSegmentAllocate), NULL, MEM_RELEASE);
    //Freeze for read log.
    ExitThread (0);

}