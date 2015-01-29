/*=============================使用说明========================*/
//这是Imagingsource相机Windows下的DLL，因为考虑多相机接口需求设计，所以使用上需要注意。
//安装官方驱动后需要配置相应的库路径
//uid_list 是返回的相机UID列表，使用者可以根据自己需求决定使用那几个UID的相机做相对应的功能。
//也可以将这个列表拷贝后以功能分组以方便使用。
//index_list 是使用者根据我返回的列表中找到对应功能的对应相机的下标组成的数组，
//后面通过这个数组进行多相机控制。包括打开关闭抓图设置回调等等。
//回调中数据结构在输出的时候都做了说明，最好将数据拷贝下来操作，因为回调中我对相机的数据块已经上锁无法修改。
/*=========================================*/

#include <iostream>

#include "../ISCamera/ISCamera.h"

#pragma comment(lib,"../Debug/ISCamera.lib")

void testCB(const I_Mat& imat)
{
	std::cout << "宽度:" << imat.width << std::endl;
	std::cout << "高度:" << imat.height << std::endl;
	std::cout << "数据长度:" << imat.data_size<< std::endl;
	std::cout << "位数:" << imat.bits << std::endl;
	std::cout << "通道数:" << imat.channel << std::endl;
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