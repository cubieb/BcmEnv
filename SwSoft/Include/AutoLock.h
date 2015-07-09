#ifndef _AutoLock_h_
#define _AutoLock_h_

class AutoLock
{
public:
    AutoLock(pthread_mutex_t& theMutex) : mutex(theMutex)
    {
        pthread_mutex_lock(&mutex);
    }
    ~AutoLock()
    {
        pthread_mutex_unlock(&mutex);
    }

private:
    pthread_mutex_t mutex;
};

#endif
