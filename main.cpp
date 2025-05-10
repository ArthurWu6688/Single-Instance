#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

// 线程不安全的懒汉单例
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

// 线程安全的懒汉单例
/*************************************************************/
class singleClass {
public:
    static singleClass *GetInstance() {
        if (instance == nullptr) { // 第一次检查
            std::unique_lock<std::mutex> ul(_mtx);
            if (instance == nullptr) { // 第二次检查
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
            std::cout << "线程" << i << "的object地址：" << object << std::endl;
        }));
    }

    for (auto &t: threads) {
        t.join();
    }
    return 0;
}