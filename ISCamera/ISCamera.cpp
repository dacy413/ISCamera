// ISCamera.cpp : Defines the exported functions for the DLL application.
#include "stdafx.h"
#include "ISCamera.h"

#pragma comment(lib,"TIS_UDSHL10d.lib")

ISCamera::ISCamera()
{
	m_CameraFlag = 0;
	if (!DShowLib::InitLibrary())
	{
		m_CameraFlag = -1;
		printf("====>>CAN NOT INIT LIB!/n");
	}
}

const std::vector<std::string>& ISCamera::getCameraList()
{
	if (m_CameraFlag == -1)
	{
		return m_CameraList;
	}
	//获取相机列表
	DShowLib::Grabber *t_pGrabber = new DShowLib::Grabber();
	m_pCameraList = t_pGrabber->getAvailableVideoCaptureDevices();
	auto end_itera = m_pCameraList->end();
	for (auto start_itera = m_pCameraList->begin(); start_itera != end_itera; start_itera++)
	{
		m_CameraList.push_back(std::string(start_itera->getUniqueName().c_str()));
	}
	delete t_pGrabber;
	return m_CameraList;
}

bool ISCamera::openCamera(std::vector<int> cameralist)
{
	if (m_CameraFlag == -1 )
	{
		return FALSE;
	}
	if (m_OpenedCameraList.size() != 0)
	{
		std::vector<int> t_reset;
		m_OpenedCameraList.swap(t_reset);
	}
	auto t_end_itera = cameralist.end();
	for (auto t_start_itera = cameralist.begin(); t_start_itera != t_end_itera; t_start_itera++)
	{
		//打开相机
		DShowLib::Grabber *t_pCurGrabber = new DShowLib::Grabber();
		t_pCurGrabber->openDev(m_pCameraList->at(*t_start_itera));
		m_pCameraGrabberList.push_back(t_pCurGrabber);
		//更新已打开相机列表
		m_OpenedCameraList.push_back((*t_start_itera));
	}
	return TRUE;
}

bool ISCamera::startGrab(std::vector<int> cameralist)
{
	if (m_CameraFlag == -1)
	{
		return FALSE;
	}
	auto t_end_itera = cameralist.end();
	auto t_start_open_camera_itera = m_OpenedCameraList.begin();
	auto t_end_open_camera_itera = m_OpenedCameraList.end();
	for (auto t_start_itera = cameralist.begin(); t_start_itera != t_end_itera; t_start_itera++)
	{
		auto t_cur_itera = std::find(t_start_open_camera_itera, t_end_open_camera_itera, (*t_start_itera));
		if (t_cur_itera != t_end_open_camera_itera)
		{
			if (m_pCameraGrabberList[t_cur_itera - t_start_open_camera_itera]->isDevOpen())
			{
				m_pCameraGrabberList[t_cur_itera - t_start_open_camera_itera]->startLive(FALSE);
				DShowLib::Error e = m_CamerasFHSMap[(*t_start_itera)]->snapImages(1, 0xFFFFFFFF);
				if (e.isError())
				{
					printf("====>>AN EXCEPTION IN GRAB...%s",e.toString().c_str());
				}
			}
			else
			{
				printf("====>>CAMERA NOT OPENED，SO CANT'T START GRAB!\n");
			}
		}
		else
		{
			printf("====>>CAMERA INDEX %d NOT FOUND IN OPENED LIST，SO CANT'T START GRAB!\n", (*t_start_itera));
		}
	}
	m_CameraFlag = 1;  //改变相机状态标志，用于关闭时检测
	return true;
}

bool ISCamera::setCameraCB(std::map<int,CB_FUNC> camera_cb_map)
{
	int t_res = TRUE;
	if (m_CameraFlag == -1)
	{
		return FALSE;
	}
	auto t_end_itera = camera_cb_map.end();
	auto t_start_open_camera_itera = m_OpenedCameraList.begin();
	auto t_end_open_camera_itera = m_OpenedCameraList.end();
	for (auto t_start_itera = camera_cb_map.begin(); t_start_itera != t_end_itera; t_start_itera++)
	{
		auto t_cur_itera = std::find(t_start_open_camera_itera, t_end_open_camera_itera, (*t_start_itera).first);
		if (t_cur_itera != t_end_open_camera_itera)
		{
			if (m_pCameraGrabberList[t_cur_itera - t_start_open_camera_itera]->isDevOpen())
			{
				ISCameraHandler *iscamera_handler = new ISCameraHandler((*t_start_itera).second);
				m_ISCameraHandlerMap.insert(std::map<int,ISCameraHandler*>::value_type((*t_start_itera).first, iscamera_handler));
				t_res = m_pCameraGrabberList[t_cur_itera - t_start_open_camera_itera]->getOverlay()->setEnable(TRUE);
				t_res = m_pCameraGrabberList[t_cur_itera - t_start_open_camera_itera]->addListener(iscamera_handler, DShowLib::GrabberListener::eFRAMEREADY);
				DShowLib::FrameTypeInfoArray t_AcceptedTypes = DShowLib::FrameTypeInfoArray::createRGBArray();
				smart_ptr<DShowLib::FrameHandlerSink> t_pFrameHandlerSink = DShowLib::FrameHandlerSink::create(t_AcceptedTypes, 1);
				m_CamerasFHSMap.insert(std::map<int, DShowLib::tFrameHandlerSinkPtr>::value_type((*t_start_itera).first, t_pFrameHandlerSink));
				t_pFrameHandlerSink->setSnapMode(TRUE);
				t_res = m_pCameraGrabberList[t_cur_itera - t_start_open_camera_itera]->setSinkType(t_pFrameHandlerSink);
			}
			else
			{
				printf("====>>CAMERA NOT OPENED，SO CANT'T SET CALLBACK!\n");
			}
		}
		else
		{
			printf("====>>CAMERA INDEX %d NOT FOUND IN OPENED LIST，SO CANT'T SET CALLBACK!\n", (*t_start_itera));
		}
	}
	return t_res;
}

bool ISCamera::stopGrab(std::vector<int> cameralist)
{
	if (m_CameraFlag == -1)
	{
		return FALSE;
	}
	auto t_end_itera = cameralist.end();
	auto t_start_open_camera_itera = m_OpenedCameraList.begin();
	auto t_end_open_camera_itera = m_OpenedCameraList.end();
	for (auto t_start_itera = cameralist.begin(); t_start_itera != t_end_itera; t_start_itera++)
	{
		auto t_cur_itera = std::find(t_start_open_camera_itera, t_end_open_camera_itera, (*t_start_itera));
		if (t_cur_itera != t_end_open_camera_itera)
		{
			if (m_pCameraGrabberList[t_cur_itera - t_start_open_camera_itera]->isDevOpen())
			{
				m_pCameraGrabberList[t_cur_itera - t_start_open_camera_itera]->stopLive();
			}
			else
			{
				printf("====>>CAMERA NOT OPENED，SO CANT'T STOP GRAB!\n");
			}
		}
		else
		{
			printf("====>>CAMERA INDEX %d NOT FOUND IN OPENED LIST，SO CANT'T STOP GRAB!\n", (*t_start_itera));
		}
	}
	return TRUE;
}

bool ISCamera::closeCamera()
{
	if (m_CameraFlag == -1)
	{
		return FALSE;
	}
	auto t_start_itera = m_pCameraGrabberList.begin();
	auto t_end_itera = m_pCameraGrabberList.end();
	for (auto t_cur_itera = t_start_itera; t_start_itera != t_end_itera; t_start_itera++)
	{

		/*监听器*/
		(*t_cur_itera)->removeListener(m_ISCameraHandlerMap[(t_cur_itera-t_start_itera)]);
		while ((*t_cur_itera)->isListenerRegistered(m_ISCameraHandlerMap[(t_cur_itera - t_start_itera)]))
		{
			Sleep(1);
		}
		/*相机*/
		if ((*t_cur_itera)->isDevOpen())
			{
				(*t_cur_itera)->closeDev();
			}
			else
			{
				printf("====>>CAMERA NOT OPENED!\n");
			}
			delete (*t_cur_itera);
			delete m_ISCameraHandlerMap[(t_cur_itera - t_start_itera)];
			m_CamerasFHSMap[(t_cur_itera - t_start_itera)].destroy();
	}
	m_CameraFlag = 0;  //改变相机状态标志，用于析构时检测
	return TRUE;
}

bool ISCamera::closeCamera(std::vector<int> cameralist)
{
	if (m_CameraFlag == -1)
	{
		return FALSE;
	}
	auto t_end_itera = cameralist.end();
	auto t_start_open_camera_itera = m_OpenedCameraList.begin();
	auto t_end_open_camera_itera = m_OpenedCameraList.end();
	for (auto t_start_itera = cameralist.begin(); t_start_itera != t_end_itera; t_start_itera++)
	{
		auto t_cur_itera = std::find(t_start_open_camera_itera, t_end_open_camera_itera, (*t_start_itera));
		if (t_cur_itera != t_end_open_camera_itera)
		{
			/*监听器*/
			m_pCameraGrabberList[t_cur_itera - t_start_open_camera_itera]->removeListener(m_ISCameraHandlerMap[(*t_start_itera)]);
			while (m_pCameraGrabberList[t_cur_itera - t_start_open_camera_itera]->isListenerRegistered(m_ISCameraHandlerMap[(*t_start_itera)]))
			{
				Sleep(1);
			}
			/*相机*/
			if (m_pCameraGrabberList[t_cur_itera - t_start_open_camera_itera]->isDevOpen())
			{
				m_pCameraGrabberList[t_cur_itera - t_start_open_camera_itera]->closeDev();
			}
			else
			{
				printf("====>>CAMERA NOT OPENED!\n");
			}
			delete m_pCameraGrabberList[t_cur_itera - t_start_open_camera_itera];
			delete m_ISCameraHandlerMap[(*t_start_itera)];
			m_CamerasFHSMap[(*t_start_itera)].destroy();
		}
		else
		{
			printf("====>>CAMERA INDEX %d NOT FOUND IN OPENED LIST!\n", (*t_start_itera));
		}
	}
	m_CameraFlag = 0;  //改变相机状态标志，用于析构时检测
	return TRUE;
}

ISCamera::~ISCamera()
{
	if (m_CameraFlag == 1)
	{
		closeCamera();
	}
	printf("====>>EXIT LIB");
	DShowLib::ExitLibrary();
}