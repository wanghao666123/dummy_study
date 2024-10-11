#!/usr/bin/env python3

from __future__ import print_function



import argparse

import os

import sys

import time



sys.path.insert(0, os.path.join(os.path.dirname(os.path.dirname(

    os.path.realpath(__file__))),

    "Firmware", "fibre", "python"))

from fibre import Logger, Event

import ref_tool

from ref_tool.configuration import *



old_print = print





def print(*args, **kwargs):

    kwargs.pop('flush', False)

    old_print(*args, **kwargs)

    file = kwargs.get('file', sys.stdout)

    file.flush() if file is not None else sys.stdout.flush()





script_path = os.path.dirname(os.path.realpath(__file__))



## Parse arguments ##

parser = argparse.ArgumentParser(description='Robot-Embedded-Framework command line utility\n',

                                 formatter_class=argparse.RawTextHelpFormatter)

subparsers = parser.add_subparsers(help='sub-command help', dest='command')

shell_parser = subparsers.add_parser('shell',

                                     help='Drop into an interactive python shell that lets you interact with the ODrive(s)')

shell_parser.add_argument("--no-ipython", action="store_true",

                          help="Use the regular Python shell ")

subparsers.add_parser('liveplotter', help="For plotting of REF parameters (i.e. position) in real time")



# General arguments

parser.add_argument("-p", "--path", metavar="PATH", action="store",

                    help="The path(s) where REF-board(s) should be discovered.\n")

parser.add_argument("-s", "--serial-number", action="store",

                    help="The 12-digit serial number of the device. ")

parser.add_argument("-v", "--verbose", action="store_true",

                    help="print debug information")

parser.add_argument("--version", action="store_true",

                    help="print version information and exit")



parser.set_defaults(path="usb")

args = parser.parse_args()



# Default command

if args.command is None:

    args.command = 'shell'

    args.no_ipython = False

logger = Logger(verbose=args.verbose)



app_shutdown_token = Event()



try:

    if args.command == 'shell':

        # 在调用launch_shell之前打印参数
        print("Args:\r\n", args)
        print("Logger:\r\n", logger)
        print("App Shutdown Token\r\n:", app_shutdown_token)

        print("fan--------import ref_tool.shell")

        import ref_tool.shell



        ref_tool.shell.launch_shell(args, logger, app_shutdown_token)



    elif args.command == 'liveplotter':

        from ref_tool.utils import start_liveplotter



        print("Waiting for ODrive...")

        ref_unit = ref_tool.find_any(path=args.path, serial_number=args.serial_number,

                                     search_cancellation_token=app_shutdown_token,

                                     channel_termination_token=app_shutdown_token)



        # If you want to plot different values, change them here.

        # You can plot any number of values concurrently.

        cancellation_token = start_liveplotter(lambda: [

            ref_unit.axis0.encoder.pos_estimate,

            ref_unit.axis1.encoder.pos_estimate,

        ])



        print("Showing plot. Press Ctrl+C to exit.")

        while not cancellation_token.is_set():

            time.sleep(1)



    else:

        raise Exception("unknown command: " + args.command)



except OperationAbortedException:

    logger.info("Operation aborted.")

finally:

    app_shutdown_token.set()
