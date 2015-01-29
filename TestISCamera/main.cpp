/*=============================ʹ��˵��========================*/
//����Imagingsource���Windows�µ�DLL����Ϊ���Ƕ�����ӿ�������ƣ�����ʹ������Ҫע�⡣
//��װ�ٷ���������Ҫ������Ӧ�Ŀ�·��
//uid_list �Ƿ��ص����UID�б�ʹ���߿��Ը����Լ��������ʹ���Ǽ���UID����������Ӧ�Ĺ��ܡ�
//Ҳ���Խ�����б������Թ��ܷ����Է���ʹ�á�
//index_list ��ʹ���߸����ҷ��ص��б����ҵ���Ӧ���ܵĶ�Ӧ������±���ɵ����飬
//����ͨ�����������ж�������ơ������򿪹ر�ץͼ���ûص��ȵȡ�
//�ص������ݽṹ�������ʱ������˵������ý����ݿ���������������Ϊ�ص����Ҷ���������ݿ��Ѿ������޷��޸ġ�
/*=========================================*/

#include <iostream>

#include "../ISCamera/ISCamera.h"

#pragma comment(lib,"../Debug/ISCamera.lib")

void testCB(const I_Mat& imat)
{
	std::cout << "���:" << imat.width << std::endl;
	std::cout << "�߶�:" << imat.height << std::endl;
	std::cout << "���ݳ���:" << imat.data_size<< std::endl;
	std::cout << "λ��:" << imat.bits << std::endl;
	std::cout << "ͨ����:" << imat.channel << std::endl;
}

int main()
{
	ISCamera camera;
	std::vector<std::string> uid_list;
	uid_list = camera.getCameraList();
	auto end_uid_itera = uid_list.end();
	for (auto start_uid_itera = uid_list.begin(); start_uid_itera != end_uid_itera; start_uid_itera++)
	{
		std::cout << "UID:" << (*start_uid_itera) << std::endl;
	}
	std::vector<int> index_list = { 0 };
	camera.openCamera(index_list);
	std::map<int, CB_FUNC> testcb;
	testcb.insert(std::pair<int, CB_FUNC>(0, testCB));
	camera.setCameraCB(testcb);
	camera.startGrab(index_list);
	camera.stopGrab(index_list);
	camera.closeCamera(index_list);
	return 0;
}