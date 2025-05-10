#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

// �̲߳���ȫ����������
/*************************************************************/
//class singleClass {
//public:
//    static singleClass *GetInstance() {
//        if (instance == nullptr) {
//            instance = new singleClass();
//        }
//        return instance;
//    }
//
//private:
//    static singleClass *instance;
//    singleClass() = default;
//    singleClass(const singleClass &s) = default;
//    singleClass &operator=(const singleClass &s) = default;
//};
//
//singleClass *singleClass::instance = nullptr;
//
//int main() {
//    singleClass *object1 = singleClass::GetInstance();
//    singleClass *object2 = singleClass::GetInstance();
//
//    std::cout << object1 << std::endl;
//    std::cout << object2 << std::endl;
//
//    return 0;
//}
/*************************************************************/

// �̰߳�ȫ����������
/*************************************************************/
class singleClass {
public:
    static singleClass *GetInstance() {
        if (instance == nullptr) { // ��һ�μ��
            std::unique_lock<std::mutex> ul(_mtx);
            if (instance == nullptr) { // �ڶ��μ��
                instance = new singleClass();
            }
        }
        return instance;
    }

private:
    static singleClass *instance;
    static std::mutex _mtx;
    singleClass() = default;
    singleClass(const singleClass &s) = default;
    singleClass &operator=(const singleClass &s) = default;
};

singleClass *singleClass::instance = nullptr;
std::mutex singleClass::_mtx;

int main() {
    std::mutex mtx;
    std::vector<std::thread> threads;
    for (int i = 0; i < 10; ++i) {
        threads.push_back(std::thread([i, &mtx]() {
            singleClass *object = singleClass::GetInstance();
            std::unique_lock<std::mutex> ul(mtx);
            std::cout << "�߳�" << i << "��object��ַ��" << object << std::endl;
        }));
    }

    for (auto &t: threads) {
        t.join();
    }
    return 0;
}