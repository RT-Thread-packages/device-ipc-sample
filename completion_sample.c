/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-08-12     chenyingchun the first version
 */

/*
 * 程序清单：完成量例程
 *
 * 程序会初始化线程1，线程2及初始化一个完成量对象
 * 线程1等待线程2发送完成量
*/
#include <rtthread.h>
#include <rtdevice.h>

#define THREAD_PRIORITY 9
#define THREAD_TIMESLICE 5

/* 完成量控制块 */
static struct rt_completion completion;

ALIGN(RT_ALIGN_SIZE)
static char thread1_stack[1024];
static struct rt_thread thread1;

/* 线程1入口函数 */
static void thread1_completion_wait(void *param)
{
    /* 等待完成 */
    rt_kprintf("thread1: completion is waitting\n");
    rt_completion_wait(&completion, RT_WAITING_FOREVER);
    rt_kprintf("thread1: completion waitting done\n");
    rt_kprintf("thread1 leave.\n");
}

ALIGN(RT_ALIGN_SIZE)
static char thread2_stack[1024];
static struct rt_thread thread2;

/* 线程2入口 */
static void thread2_completion_done(void *param)
{
    rt_kprintf("thread2: completion done\n");
    rt_completion_done(&completion);
    rt_kprintf("thread2 leave.\n");
}

int completion_sample(void)
{
    /* 初始化完成量对象 */
    rt_completion_init(&completion);

    rt_thread_init(&thread1,
                   "thread1",
                   thread1_completion_wait,
                   RT_NULL,
                   &thread1_stack[0],
                   sizeof(thread1_stack),
                   THREAD_PRIORITY - 1, THREAD_TIMESLICE);
    rt_thread_startup(&thread1);

    rt_thread_init(&thread2,
                   "thread2",
                   thread2_completion_done,
                   RT_NULL,
                   &thread2_stack[0],
                   sizeof(thread2_stack),
                   THREAD_PRIORITY, THREAD_TIMESLICE);
    rt_thread_startup(&thread2);

    return 0;
}

/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(completion_sample, completion sample);
