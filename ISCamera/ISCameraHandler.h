#pragma once
#include "tisudshl.h"

typedef struct
{
	int width;
	int height;
	int bits;
	int channel;
	int data_size;
	unsigned char* data;
} I_Mat;

typedef void(*CB_FUNC)(const I_Mat&);

class ISCameraHandler :public DShowLib::GrabberListener
{
public:
	ISCameraHandler(CB_FUNC);
	void frameReady(DShowLib::Grabber&, smart_ptr<DShowLib::MemBuffer>, DWORD);
private:
	CB_FUNC m_cb_func;
	ISCameraHandler& operator=(const ISCameraHandler&);
	ISCameraHandler(const ISCameraHandler&);
};