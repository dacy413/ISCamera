#pragma once
#ifdef ISCAMERA_EXPORTS
#define ISCAMERA_API __declspec(dllexport)
#else
#define ISCAMERA_API __declspec(dllimport)
#endif
#include <map>
#include <vector>

#include "tisudshl.h"
#include "ISCameraHandler.h"

class ISCAMERA_API ISCamera
{
private:
	//禁止拷贝和赋值（无需使用容器）
	ISCamera(const ISCamera&);
	ISCamera& operator = (const ISCamera&);
	
	//外部使用的相机UID列表
	std::vector<std::string> m_CameraList;
	//内部使用的相机自带指针列表
	DShowLib::Grabber::tVidCapDevListPtr m_pCameraList;
	//已经打开的相机UID下标列表
	std::vector<int> m_OpenedCameraList;
	//相机实际指针列表需要手动释放
	std::vector<DShowLib::Grabber*> m_pCameraGrabberList;
	//相机监听者对象指针列表需要手动释放
	std::map<int,ISCameraHandler*> m_ISCameraHandlerMap;
	//相机帧处理使用为smart_ptr无需释放
	std::map<int,DShowLib::tFrameHandlerSinkPtr> m_CamerasFHSMap;

	//相机状态标志
	int m_CameraFlag;
public:
	ISCamera();
	~ISCamera();
	
	//1 获取相机列表
	/*返回一个const的camera list，使用者需要按需组织下标数组*/
	const std::vector<std::string>& getCameraList();
	//2 打开指定相机
	/*参数为指定相机在camera list中的下标*/
	bool openCamera(std::vector<int>);
	//3 设置指定相机回调
	/*参数为指定相机在camera list中的下标和一个函数指针组成的map*/
	bool setCameraCB(std::map<int, CB_FUNC>);
	//4 开始指定相机抓图
	/*参数为指定相机在camera list中的下标*/
	bool startGrab(std::vector<int>);
	//5 停止指定相机抓图
	/*参数为指定相机在camera list中的下标*/
	bool stopGrab(std::vector<int>);
	//6 关闭指定相机
	/*参数为指定相机在camera list中的下标*/
	bool closeCamera(std::vector<int>);

	// 关闭全部相机，用于在析构时关闭打开的相机。
	bool closeCamera();

	////设置触发模式
	//bool setExternalTrigger(bool);
};
