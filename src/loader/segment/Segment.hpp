#pragma once

#include "SegmentHeader.hpp"
#include "SegmentTranslator.hpp"

//Global variable that points to a segment.
extern const unsigned char SegmentData [];

class Segment {

public:

	//Getters.

	SegmentHeader GetHeader () {
		return m_header;
	};

private:

	SegmentHeader m_header;

};