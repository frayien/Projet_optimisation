#include "Config.h"

void Config::load_args(int argc, char *args[])
{
    for(int i = 1; i<argc; ++i)
    {
        std::string msg(args[i]);
        if(msg == "question")
        {
            i++;
            if(i < argc)
            {
                question = std::stoi(args[i]);
            }
            continue;
        }

        if(msg == "dir")
        {
            i++;
            if(i < argc)
            {
                in_dir = std::string(args[i]);
            }
            continue;
        }

        if(msg == "file")
        {
            i++;
            if(i < argc)
            {
                in_file = std::string(args[i]);
            }
            continue;
        }

        if(msg == "verbose")
        {
            verbose = true;
            continue;
        }
    }
}