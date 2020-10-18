#pragma once

#include "../Vendor.hpp"

//GetVersionExW.
#pragma warning (disable : 4996)

class CommonUtil {

public:

    /**
     * Get module base address from internal process.
     *
     * @param module - Module name.
     * @return The base address of the module in memory.
     **/

    static HMODULE GetModule (const char* module);

    /**
     * Get module info from internal process.
     * 
     * @param module - Module name.
     * @return https://docs.microsoft.com/en-us/windows/win32/api/psapi/ns-psapi-moduleinfo.
     **/

    static MODULEINFO GetModuleInfo (const char* module);

    /**
     * Get function address from module.
     *
     * @param module   - Module name.
     * @param function - Function name.
     *
     * @return Address of function in module.
     **/

    static DWORD GetFunction (const char* module, const char* function),

    /**
     * Search signature in module. 
     *
     * @param module    - Module name.
     * @param signature - IDA-styled signature.
     * @return Success - module base + offset || Error - 0.
     **/

    SearchSignature (const char* module, const char* signature);

    /**
     * Find offsets and fill to vector.
     *
     * @param module      - Module name.
     * @param signatures  - Signatures vector.
     * @param vector      - Vector to fill.
     **/

    static bool FindOffsetsToVec (const char* module, std::vector<const char*> signatures, std::vector<DWORD>& vector);

    /**
     * @return Is OS version to meet the minimum requirements.
     **/

    static bool IsWinXPOrLater (),  IsWinServBuild (),

    /**
     * @return Is installed redist to meet the minimum requirements.
     **/

    IsRedistPackagePresent ();

    /**
     * @return Formatted time. [HH:MM:SS]
     */

    static const std::string GetCurrentTimeAsString ();

};