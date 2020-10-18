#pragma once

#include "../Vendor.hpp"

class PanicUtil {

public:

    //Used to separate runtime from internal processes.
    enum class Layers {
        INIT    = 0x0,
        ROUTINE = 0x1
    };

    /**
     * Set important value to variable.
     *
     * @param variable - Pointer to variable.
     * @param value    - Value.
     * @param caller   - Call in method. 
     * @param reason   - Cause reason.
     * @rem - Throws "exception" if value or variable is null)
     **/

    static void SetImportant (DWORD *variable, DWORD value, const char* caller = "Unknown", const char* reason = "Variable or value is null"),

    /**
     * Require value to non-null.
     *
     * @param layer  - Panic level.
     * @param value  - Required value.
     * @param caller - Call in method.
     * @param reason - Cause reason.
     * @rem - Throws "exception" if value or variable is null)
     **/

     RequireNonNull (Layers layer, DWORD value, const char* caller = "Unknown", const char* reason = "Require value is null"),

    /**
     * Call panic.
     *
     * @param layer  - Panic level.
     * @param caller - Call in method.
     * @param reason - Cause reason.
     **/

     Release (Layers layer, const char* caller, const char* reason);

private:

    /**
     * @return Panic level name.
     **/

    static const char* ParseTypeLayers (Layers layer) {

        switch (layer) {

            case Layers::INIT:    return "INITIALIZATION";
            case Layers::ROUTINE: return "ROUTINE";

        }

        return "UNKNOWN";

    }

};