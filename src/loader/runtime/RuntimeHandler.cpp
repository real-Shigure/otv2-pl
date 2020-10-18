#include "RuntimeHandler.hpp"

void RuntimeHandler::ExtractSegment () {

    //Allocate memory for segment, and set pointer.
    PanicUtil::SetImportant   (&Primal::UnSegmentAllocate, reinterpret_cast <DWORD> (VirtualAlloc (reinterpret_cast <LPVOID> (m_allocAddressTarget), m_allocSize, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE)), "RuntimeHandler::ExtractSegment");

    //Small check for not nullable pointer. (In 99% of situations this is not necessary, but who knows what is on the player side)
    PanicUtil::RequireNonNull (PanicUtil::Layers::INIT, reinterpret_cast<DWORD> (memcpy (reinterpret_cast <PVOID> (Primal::GetAllocationPointer ()), SegmentData, m_segCopySize)), "RuntimeHandler::ExtractSegment", "Allocated memory pointer is null");
}

void RuntimeHandler::ReconstructHotPoints () {

    //------RELOCATIONS

    //Iterate relocations in vector.
    for (const auto& relocation : m_segment.GetHeader().GetRelocations ()) {
        //Subtract value with old base address in segment from memory.
        *reinterpret_cast <DWORD*> (Primal::UnSegmentAllocate + relocation) -= m_allocOldAddress;
        //Add new base address in segment to relocations.
        *reinterpret_cast <DWORD*> (Primal::UnSegmentAllocate + relocation) += Primal::UnSegmentAllocate;
    }

    //------IMPORTS

    //Iterate value in imports map.
    for (const auto& importsMap : m_segment.GetHeader().GetImports ()) {

        //Get info about import from value at map.
        for (const auto& importInfo : importsMap.second) {

            //Get function pointer from module. (Supports only ready2export functions)
            DWORD functionPointer = CommonUtil::GetFunction (importsMap.first, importInfo.m_function);

            //Check non-nullptr at function and module.
            if (!functionPointer) {
                PanicUtil::Release (PanicUtil::Layers::INIT, "RuntimeHandler::ReconstructHotPoints", (std::stringstream () << "Can't find module or function. " << "Module: " << importsMap.first << " [<->] " << "Function: " << importInfo.m_function).str().c_str ());
            }

            //Iterate all offset. (Locations where function call in segment)
            for (const auto& parsedOffset : importInfo.m_offsetsMap) {

                //More about ImportType: SegmentHeader.h
                switch (parsedOffset.first) {

                    //Patch functions (and in them) that call import.
                    case SegmentHeader::ImportType::INTERNAL:
                        *reinterpret_cast<DWORD*> (Primal::UnSegmentAllocate + parsedOffset.second) = functionPointer - (Primal::UnSegmentAllocate + parsedOffset.second + 0x4);
                      break;

                    //Patch global variables.
                    case SegmentHeader::ImportType::PUBLIC:
                        *reinterpret_cast<DWORD*> (Primal::UnSegmentAllocate + parsedOffset.second) = functionPointer;
                      break;

                    //Idk why.
                    default:
                        PanicUtil::Release (PanicUtil::Layers::ROUTINE, "RuntimeHandler::ReconstructHotPoints", "Can't find import type");
                      break;
                }

            }

        }

    }

}

void RuntimeHandler::InvokeOEP () {

    //Set function address for call.
    SegmentHeader::DLLMAIN DllMain = reinterpret_cast <SegmentHeader::DLLMAIN> (Primal::GetAllocationPointer () + m_oepOffset);
    //Check for non-null OEP address.
    PanicUtil::RequireNonNull (PanicUtil::Layers::INIT, reinterpret_cast<DWORD> (DllMain), "RuntimeHandler::InvokeOEP", "OEP value is null");
    //Call "OEP" func with arguments.
    DllMain (reinterpret_cast<HMODULE> (Primal::GetAllocationPointer ()), DLL_PROCESS_ATTACH, NULL);

}

DWORD   Primal::UnSegmentAllocate = 0x0;
HMODULE Primal::UnCurrentModule = 0x0;