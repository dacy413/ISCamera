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
	//��ֹ�����͸�ֵ������ʹ��������
	ISCamera(const ISCamera&);
	ISCamera& operator = (const ISCamera&);
	
	//�ⲿʹ�õ����UID�б�
	std::vector<std::string> m_CameraList;
	//�ڲ�ʹ�õ�����Դ�ָ���б�
	DShowLib::Grabber::tVidCapDevListPtr m_pCameraList;
	//�Ѿ��򿪵����UID�±��б�
	std::vector<int> m_OpenedCameraList;
	//���ʵ��ָ���б���Ҫ�ֶ��ͷ�
	std::vector<DShowLib::Grabber*> m_pCameraGrabberList;
	//��������߶���ָ���б���Ҫ�ֶ��ͷ�
	std::map<int,ISCameraHandler*> m_ISCameraHandlerMap;
	//���֡����ʹ��Ϊsmart_ptr�����ͷ�
	std::map<int,DShowLib::tFrameHandlerSinkPtr> m_CamerasFHSMap;

	//���״̬��־
	int m_CameraFlag;
public:
	ISCamera();
	~ISCamera();
	
	//1 ��ȡ����б�
	/*����һ��const��camera list��ʹ������Ҫ������֯�±�����*/
	const std::vector<std::string>& getCameraList();
	//2 ��ָ�����
	/*����Ϊָ�������camera list�е��±�*/
	bool openCamera(std::vector<int>);
	//3 ����ָ������ص�
	/*����Ϊָ�������camera list�е��±��һ������ָ����ɵ�map*/
	bool setCameraCB(std::map<int, CB_FUNC>);
	//4 ��ʼָ�����ץͼ
	/*����Ϊָ�������camera list�е��±�*/
	bool startGrab(std::vector<int>);
	//5 ָֹͣ�����ץͼ
	/*����Ϊָ�������camera list�е��±�*/
	bool stopGrab(std::vector<int>);
	//6 �ر�ָ�����
	/*����Ϊָ�������camera list�е��±�*/
	bool closeCamera(std::vector<int>);

	// �ر�ȫ�����������������ʱ�رմ򿪵������
	bool closeCamera();

	////���ô���ģʽ
	//bool setExternalTrigger(bool);
};
