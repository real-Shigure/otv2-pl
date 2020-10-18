#pragma once

#include "../../Vendor.hpp"

//This class is made to maintain separation between the primal loader and the segment framework.
//Since sometimes you have to change the loader itself, this class is ideal for this purpose.
class SegmentTranslator {

public:

	//Callback at OEP.
	enum class CallbackType {
		BEFORE, AFTER
	};

	/**
	 * This function is called before and after OEP in order for the segment to perform its routine work.
	 *
	 * @param type - Invoke type.
	 * @rem - Best to run binary patches before OEP.
	 **/

	virtual void CallbackWithOEP (CallbackType type) = 0;

};