#!/usr/bin/env python3

from __future__ import print_function

import os
import sys
from argparse import Namespace  # 导入 Namespace 类
from fibre import Logger, Event
import ref_tool.shell  # 导入 shell 子模块

def main():
    # 创建命名空间对象
    args = Namespace(
        command=None,
        path='usb',
        serial_number=None,
        verbose=False,
        version=False,
        no_ipython=False
    )

    # 连接到 REF-board(s)
    ref_unit = ref_tool.find_any()

    if ref_unit is None:
        print("未找到 REF-board，请检查连接并重试。")
        return

    # 创建 logger 对象
    logger = Logger(verbose=args.verbose)

    # 进入命令行模式
    print("REF-board 连接成功，进入命令行模式。")
    app_shutdown_token = Event()  # 创建一个事件用于控制程序退出
    ref_tool.shell.launch_shell(args, logger, app_shutdown_token)

if __name__ == "__main__":
    main()

