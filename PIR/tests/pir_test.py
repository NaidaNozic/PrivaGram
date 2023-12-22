#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import subprocess
import sys
from time import sleep
from os import listdir
import argparse

DATABASE_PATH = "../database/"
SERVER = "../build/server"
CLIENT = "../build/client"
START_PORT = 11112
IMAGES = 40
OUTFILE = "result.jpg"
TIMEOUT = 20

def printPassed(idx):
    print("Test #" + str(idx) + ": Passed")

def printFailed(idx):
    print("Test #" + str(idx) + ": Failed")

def printTimeout(idx):
    print("Test #" + str(idx) + ": ***Timeout")

def test_pir(num_servers, shamir, privacy = 1):
    total = 0
    passed = 0
    if shamir != 0:
        shamir = 1
    for idx in range(len(listdir(DATABASE_PATH))):
        print("Querying image " + str(idx)+ ":")
        total = total + 1
        process_list = []
        port = START_PORT

        args = [CLIENT, "-i", str(idx), "-o", OUTFILE, "--images=" + str(IMAGES), "--shamir=" + str(shamir), "-t", str(privacy),"--servers"]
        try:
            for s in range(num_servers):
                process_list.append(subprocess.Popen([SERVER, "-p", str(port), "-d", DATABASE_PATH, "--images=" + str(IMAGES)]))
                args.append("127.0.0.1:" + str(port))
                port = port + 1
            sleep(0.1)
            client = subprocess.Popen(args)
            client.communicate(timeout=TIMEOUT)[0]
        except subprocess.TimeoutExpired as ex:
            printTimeout(idx)
            for s in range(num_servers):
                process_list[s].terminate()
            print()
            continue
        except Exception as ex:
            print(ex)
            printFailed(idx)
            for s in range(num_servers):
                process_list[s].terminate()
            print()
            continue

        for s in range(num_servers):
            process_list[s].terminate()

        ref = ""
        result = ""
        try:
            with open(OUTFILE, "rb") as f:
                result = f.read()
            with open(DATABASE_PATH + "img" + str(idx) + ".jpg", "rb") as f:
                ref = f.read()
        except Exception as ex:
            print(ex)
            printFailed(idx)
            print()
            continue

        if (ref != result):
            printFailed(idx)
        else:
            printPassed(idx)
            passed = passed + 1
        print()
    return passed, total

if __name__=="__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-n", "--num_server", type=int, help="number of servers")
    parser.add_argument("--shamir", help="activate shamir", action="store_true")
    parser.add_argument("-t", "--privacy", type=int, help="privacy level", default=1)
    args = parser.parse_args()
    shamir = 0
    if not args.num_server:
        print("--num_server is required")
        exit(-1)
    if args.shamir:
        shamir = 1
    passed, total = test_pir(args.num_server, shamir, args.privacy)
    print(str(passed) + "/" + str(total) + " tests passed")
