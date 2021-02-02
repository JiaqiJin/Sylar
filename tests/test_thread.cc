#include "../sylar/sylar.h"

sylar::Logger::ptr p_logger = SYLAR_LOG_ROOT();

int count = 0;
//sylar::RWMutex s_mutex;
sylar::Mutex m_mutex;

void func1(){
    SYLAR_LOG_INFO(p_logger) << "name: " << sylar::Thread::GetName()
                             << " this.name: " << sylar::Thread::GetThis()->getName()
                             << " id: " << sylar::GetThreadId()
                             << " this.id: " << sylar::Thread::GetThis()->getID();
    //sleep(20);
   for(int i = 0; i < 100000; ++i) {
        sylar::Mutex::Lock lock_(m_mutex);
        ++count;
    }
}

void fun2() {
    while(true) {
        SYLAR_LOG_INFO(p_logger) << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
    }
}

void fun3() {
    while(true) {
        SYLAR_LOG_INFO(p_logger) << "========================================";
    }
}

int main(int argc, char** argv) {
    SYLAR_LOG_INFO(p_logger) << "pthread init";
    //YAML::Node root = YAML::LoadFile("/home/sam/Documents/Sylar/bin/conf/log2.yml");
    //sylar::Config::LoadFromYaml(root);
    std::vector<sylar::Thread::ptr> v_thread;
    for(int i = 0; i<2;i++){
        sylar::Thread::ptr pt(new sylar::Thread(&fun2, "name"+std::to_string(i * 2)));
        sylar::Thread::ptr pt2(new sylar::Thread(&fun3, "name"+std::to_string(i * 2 + 1)));
        v_thread.push_back(pt);
        v_thread.push_back(pt2);
    }

    for(size_t i = 0; i < v_thread.size();i++){
        v_thread[i]->join();
    }

    SYLAR_LOG_INFO(p_logger) << "pthread end";
    SYLAR_LOG_INFO(p_logger) << "count==" << count;
    return 0;
}