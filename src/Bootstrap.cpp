#include "Vendor.hpp"
#include "loader/BootstrapTaskController.hpp"

/**
 * 
 * CPUID.
 * PRIOR: PUBLIC.
 * PROJECT: OTv2-PL.
 * 
 * 
 * AUTHORS:
 * -
 *  > 0x000cb.     | Primal-loader/Engine.
 *  > sneakyevil.  | Offsets/Netvars/Etc.
 *  > sovve.       | Designer.
 *  > enq.         | Designer.
 *  > keaton.      | ImGui.
 *
 *
 * LEGACY FILES:
 * -
 * LOADER/RUNTIME/*,
 * LOADER/SEGMENT/*,
 * UTIL/*;
 *
 * OUR PROJECTS:
 * -
 * OPEN ONETAP V2 IDB:       https://github.com/cpu-id/oit2.
 * SEGMENT BOOTSTRAP ENGINE: https://github.com/cpu-id/primal-loader.
 * NON-DEPENDENCY LOADER UI: *soon*.
 *
 **/

BOOL APIENTRY DllMain (HMODULE module, DWORD callTrace, LPVOID lpReserved) {

    if (callTrace == DLL_PROCESS_ATTACH) {

        //Setup UI, D3X9/WndProc hooks, setup listener for callback.
        CreateThread (NULL, NULL, BootstrapTask::SetupUIBridge, NULL, NULL, NULL);

        //Setup listener for callback & init engine.
        CreateThread (NULL, NULL, BootstrapTask::InitializeEngine, NULL, NULL, NULL);

    }

    return TRUE;

}