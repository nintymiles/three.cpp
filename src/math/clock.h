#ifndef SRC_MATH_CLOCK_H_
#define SRC_MATH_CLOCK_H_

#include <time.h>
#ifdef _MSC_VER //WIN32
    #include <windows.h>
#else
    #include <sys/time.h>
#endif
#ifdef _MSC_VER //WIN32
int gettimeofday(struct timeval *tp, void *tzp)
{
  time_t clock;
  struct tm tm;
  SYSTEMTIME wtm;
  GetLocalTime(&wtm);
  tm.tm_year   = wtm.wYear - 1900;
  tm.tm_mon   = wtm.wMonth - 1;
  tm.tm_mday   = wtm.wDay;
  tm.tm_hour   = wtm.wHour;
  tm.tm_min   = wtm.wMinute;
  tm.tm_sec   = wtm.wSecond;
  tm. tm_isdst  = -1;
  clock = mktime(&tm);
  tp->tv_sec = clock;
  tp->tv_usec = wtm.wMilliseconds * 1000;
  return (0);
}
#endif


class Clock {
    private:
        long now(){
            struct timeval tv = {0};
            gettimeofday(&tv, NULL);
            return tv.tv_sec * 1000 + tv.tv_usec / 1000;  //毫秒
            //return tv.tv_sec * 1000000 + tv.tv_usec；   //微秒
        }
    public:
        bool autoStart,running;
        long startTime,oldTime,elapsedTime;

        Clock(bool autoStart = true):autoStart(autoStart),running(false),
                                startTime(0),oldTime(0),elapsedTime(0){}
        

        Clock& start() {
            // struct timeval tv_end = {0};
            // gettimeofday(&tv_end, NULL);
            // startTime = tv_end.tv_sec * 1000 + tv_end.tv_usec / 1000; 
            startTime = now();

            oldTime = startTime;
            elapsedTime = 0;
            running = true;

            return *this;
        }

        Clock& stop() {
            elapsedTime = getElapsedTime();
            running = false;
            autoStart = false;

            return *this;
        }

        long getElapsedTime() {
            getDelta();
            return elapsedTime;
        }

        long getDelta() {
            long diff = 0;

            if ( autoStart && !running ) {
                start();
                return 0;
            }

            if ( running ) {
                long newTime = now();
                //std::cout << "delta new time =" << newTime << std::endl;
                //std::cout << "delta old time =" << oldTime << std::endl;
                diff = ( newTime - oldTime ) / 1000;
                //std::cout << "delta diff time =" << diff << std::endl;
                oldTime = newTime;
                

                elapsedTime += diff;
            }

            return diff;
        }

};

#endif /* SRC_MATH_CLOCK_H_ */
