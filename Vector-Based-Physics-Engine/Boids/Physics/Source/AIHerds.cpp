#include "AIHerds.h"

AIHERDS::AIHERDS(AIHERDS_TYPE typeValue)
	: type(typeValue),
	scale(1, 1, 1),
	active(false),
	mass(1.f)
{
}

AIHERDS::~AIHERDS()
{
}