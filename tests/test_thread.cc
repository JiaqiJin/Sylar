#include "../sylar/sylar.h"

sylar::Logger::ptr p_logger = SYLAR_LOG_ROOT();

int count = 0;
sylar::RWMutex s_mutex;

void func1(){
    SYLAR_LOG_INFO(p_logger) << "name: " << sylar::Thread::GetName()
                             << " this.name: " << sylar::Thread::GetThis()->getName()
                             << " id: " << sylar::GetThreadId()
                             << " this.id: " << sylar::Thread::GetThis()->getID();
    //sleep(20);
   for(int i = 0; i < 100000; ++i) {
        sylar::RWMutex::WriteLock lock(s_mutex);
        ++count;
    }
}

void func2(){

}

int main(int argc, char** argv) {
    SYLAR_LOG_INFO(p_logger) << "pthread init";
    std::vector<sylar::Thread::ptr> v_thread;
    for(int i = 0; i<5;i++){
        sylar::Thread::ptr pt(new sylar::Thread(&func1, "name"+std::to_string(i)));
        v_thread.push_back(pt);
    }

    for(int i = 0; i<5;i++){
        v_thread[i]->join();
    }

    SYLAR_LOG_INFO(p_logger) << "pthread end";
    SYLAR_LOG_INFO(p_logger) << "count==" << count;
    return 0;
}