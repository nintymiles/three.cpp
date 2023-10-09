#ifndef SRC_UTILS_SYS_UTILS_H_
#define SRC_UTILS_SYS_UTILS_H_

#include <stdio.h>
#include <string>
#include <chrono>

#define CMD_RESULT_BUF_SIZE 1024

/*
 * cmd：待执行命令
 * result：命令输出结果
 * 函数返回：0 成功；-1 失败；
 */
int execute_shell_command(const char *cmd, char *result)
{
    int iRet = -1;
    char buf_ps[CMD_RESULT_BUF_SIZE];
    char ps[CMD_RESULT_BUF_SIZE] = {0};
    FILE *ptr;

    strcpy(ps, cmd);

    if((ptr = popen(ps, "r")) != NULL)
    {
        while(fgets(buf_ps, sizeof(buf_ps), ptr) != NULL)
        {
           strcat(result, buf_ps);
           if(strlen(result) > CMD_RESULT_BUF_SIZE)
           {
               break;
           }
        }
        pclose(ptr);
        ptr = NULL;
        iRet = 0;  // 处理成功
    }
    else
    {
        printf("popen %s error\n", ps);
        iRet = -1; // 处理失败
    }

    return iRet;
}

std::string execute_shell_command_with_result(const char *cmd)
{
    char cBuf[CMD_RESULT_BUF_SIZE] = {0};
    std::string sCmdResult;

    execute_shell_command(cmd, cBuf);
    sCmdResult = std::string(cBuf); // char * 转换为 string 类型
    printf("CMD Result: \n%s\n", sCmdResult.c_str());

    return sCmdResult;
}

void delay()
{
    auto start = std::chrono::high_resolution_clock::now();
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    while (true)
    {
        stop = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        if (duration.count() >= 15000)
        {
            return;
        }
        else
            continue;
    }
}

// void renderfps(int framerate) 
// { 
// 	currentTime = glfwGetTime(); 

// 	if(currentTime - lastTime >= 1.0 / framerate) 
// 	{
// 		lastTime = currentTime;
// 		render(); 
// 	}
// }


#endif /* SRC_UTILS_SYS_UTILS_H_ */
