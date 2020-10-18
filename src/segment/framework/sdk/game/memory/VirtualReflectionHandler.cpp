#include "../../../../../Vendor.hpp"
#include "VirtualReflectionHandler.hpp"

INT OneSDK::MemoryController::HookData::CallVirtualFunction__OffsetToHook = 0x3B30;

//Naked for remove routine.
__declspec (naked) int __fastcall OneSDK::MemoryController::CallVirtualFunction (int *vTable, int index) {

    /**
     *
     * @registers
     *
     *  ECX -> Virtual table.
     *  EDX -> Index.
     *  EAX -> Virtual Function Address.
     *
     **/

    __asm {

        //Index >= 152?
        cmp edx, 152
        //Jump if less.
        jl getToVirtual
        //Index += 2;
        add edx, 2
        //Index >= 256?
        cmp edx, 256
        //Jump if less.
        jl getToVirtual
        //Index ++;
        add edx, 1
        //Index >= 300?
        cmp edx, 300
        //Jmp if less.
        jl getToVirtual
        //Index += 2;
        add edx, 2
        //Index >= 300?
        cmp edx, 300
        //Jump if less.
        jl getToVirtual
        //Index ++;
        add edx, 1

        //Get pointer to virtual function.
        getToVirtual:

            //Get virtual table from instance.
            mov eax, [ecx]
            //Get virtual function address.
            mov eax, [eax + edx * 4]
            //Return for call eax.
            ret

    }

}