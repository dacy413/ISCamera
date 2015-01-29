#include "stdafx.h"
#include "ISCameraHandler.h"

ISCameraHandler::ISCameraHandler(CB_FUNC cbfunc)
{
	m_cb_func = cbfunc;
}

void ISCameraHandler::frameReady(DShowLib::Grabber& caller, smart_ptr<DShowLib::MemBuffer> pbuffer, DWORD framenum)
{
	pbuffer->lock();
	I_Mat t_imat;
	t_imat.width = pbuffer->getSize().cx;
	t_imat.height = pbuffer->getSize().cy;
	t_imat.data = pbuffer->getPtr();
	t_imat.data_size = pbuffer->getBufferSize();
	t_imat.channel = ((t_imat.data_size / t_imat.width) / t_imat.height);
	t_imat.bits = (pbuffer->getBitsPerPixel()) / t_imat.channel;
	m_cb_func(t_imat);
	pbuffer->unlock();
}