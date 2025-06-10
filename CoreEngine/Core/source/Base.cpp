#include <Core/includes/Base.h>

unsigned int AligneSizeofBy2(const unsigned int Bytes)
{
	unsigned int CurrentBytes = 0;

	int32 i = 1;
	while (CurrentBytes < Bytes)
	{
		CurrentBytes = pow(2, i);
		i++;
	}
	return CurrentBytes;
}