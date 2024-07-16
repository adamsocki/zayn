



real64 getCurrentTimeInSeconds() 
{
    #if __APPLE__
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return now.tv_sec + now.tv_nsec / 1e9;
    #endif
}    