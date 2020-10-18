#pragma once

#include "../../Vendor.hpp"
#include "../segment/Segment.hpp"

//Just primal loader.
namespace Primal {

	//Unsafe.
    extern DWORD UnSegmentAllocate;
    extern HMODULE UnCurrentModule; 

    /**
	 * @return  Safe-based allocation pointer. 
	 * @remarks Throws "exception" if pointer is null.
	 **/

	static DWORD GetAllocationPointer () {
		PanicUtil::RequireNonNull (PanicUtil::Layers::ROUTINE, UnSegmentAllocate, "Primal::GetAllocationPointer", "Allocation pointer is null.");
		return UnSegmentAllocate;
	}

	/**
	 * @return  Safe-based current module handle. 
	 * @remarks Throws "exception" if pointer is null.
	 **/

	static HMODULE GetCurrentModule () {
		PanicUtil::RequireNonNull (PanicUtil::Layers::ROUTINE, reinterpret_cast <DWORD> (UnCurrentModule), "Primal::GetCurrentModule", "Library pointer is null.");
		return reinterpret_cast <HMODULE> (UnCurrentModule);
	}

};


class RuntimeHandler {

public:

    RuntimeHandler (Segment& segment, int allocAddressTarget, int allocOldAddress, int allocSize, int segCopySize, int oepOffset) 
		
	: m_segment (segment), m_allocAddressTarget (allocAddressTarget), m_allocOldAddress (allocOldAddress), m_allocSize (allocSize), m_segCopySize (segCopySize), m_oepOffset (oepOffset)
	
	{};

    //Extract segment to memory.
    void ExtractSegment (),

    //Patch relocations and imports.
    ReconstructHotPoints (),

    //Invoke segment.
    InvokeOEP ();

private:

    Segment& m_segment;
	int m_allocAddressTarget, m_allocOldAddress, m_allocSize, m_segCopySize, m_oepOffset;

};