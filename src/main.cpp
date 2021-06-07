#include "Config.h"
#include "question.h"


int main(int argc, char *args[])
{
    Config config;
    config.load_args(argc, args);

    switch(config.question)
    {
    case 1:
        question::question1(config);
        break;
    case 2:
        question::question2(config);
        break;
    case 3:
        question::question3(config);
        break;
    case 4:
        question::question4(config);
        break;
    case 5:
        question::question5(config);
        break;
    case 6:
        question::question6(config);
        break;
    case 7:
        question::question7(config);
        break;
    default:
        break;
    }

    return 0;
}