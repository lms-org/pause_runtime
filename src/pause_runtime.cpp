#include "pause_runtime.h"

bool PauseRuntime::initialize() {
    logger.info() << "Press p to pause runtime";
    logger.info() << "Press c to resume runtime";
    logger.info() << "Press w to step one cycle";

    /*tcgetattr gets the parameters of the current terminal
    STDIN_FILENO will tell tcgetattr that it should write the settings
    of stdin to oldt*/
    tcgetattr( STDIN_FILENO, &oldt);
    /*now the settings will be copied*/
    newt = oldt;

    /*ICANON normally takes care that one line at a time will be processed
    that means it will return if it sees a "\n" or an EOF or an EOL*/
    newt.c_lflag &= ~(ICANON);

    /*Those new settings will be set to STDIN
    TCSANOW tells tcsetattr to change attributes immediately. */
    tcsetattr( STDIN_FILENO, TCSANOW, &newt);

    runtimeName = config().get<std::string>("runtime", "default");
    running = true;
    stopNextCycle = false;

    thread = std::thread([this] () {
        while(running) {
            FD_ZERO(&fds);
            FD_SET(STDIN_FILENO, &fds);

            timeval timeout;
            timeout.tv_sec = 0;
            timeout.tv_usec = 10 * 1000; // 10 ms
            int result = select(STDIN_FILENO + 1, &fds, nullptr, nullptr, &timeout);

            if(result == 1 && FD_ISSET(STDIN_FILENO, &fds)) {
                int c = getchar();

                switch(c) {
                case 'p':
                    pauseRuntime(runtimeName);
                    break;
                case 'c':
                    {
                        std::unique_lock<std::mutex> lock(mutex);
                        stopNextCycle = false;
                    }
                    resumeRuntime(runtimeName);
                    break;
                case 'w':
                    {
                        std::unique_lock<std::mutex> lock(mutex);
                        stopNextCycle = true;
                    }
                    resumeRuntime(runtimeName);
                    break;
                }
            } else if(result == -1) {
                logger.perror() << "select failed";
            }
        }
    });


    return true;
}

bool PauseRuntime::deinitialize() {
    running = false;
    thread.join();

    /*restore the old settings*/
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    return true;
}

bool PauseRuntime::cycle() {
    std::unique_lock<std::mutex> lock(mutex);
    if(stopNextCycle) {
        stopNextCycle = false;
        pauseRuntime();
    }

    return true;
}
