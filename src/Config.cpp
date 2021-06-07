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

        if(msg == "objectif")
        {
            i++;
            if(i < argc)
            {
                objectif = std::stoi(args[i]);
            }
            continue;
        }

        if(msg == "initial")
        {
            i++;
            if(i < argc)
            {
                initial = std::stoi(args[i]);
            }
            continue;
        }

        if(msg == "step")
        {
            i++;
            if(i < argc)
            {
                max_step_n = std::stoi(args[i]);
            }
            continue;
        }

        if(msg == "temperature")
        {
            i++;
            if(i < argc)
            {
                temperature_initiale = std::stod(args[i]);
            }
            continue;
        }

        if(msg == "lambda")
        {
            i++;
            if(i < argc)
            {
                lambda = std::stod(args[i]);
            }
            continue;
        }

        if(msg == "buffer")
        {
            i++;
            if(i < argc)
            {
                buffer = std::stoi(args[i]);
            }
            continue;
        }
    }
}