#!/usr/bin/env python3

"""
Sockets: https://docs.python.org/3/library/socket.html
Ctypes: https://docs.python.org/3/library/ctypes.html
"""

import socket
import sys
import random
import time
import threading
import struct
from datetime import datetime
from structures import *

# an integer towards 0 increases packet loss likelihood (realistically in the 10000s)
packet_loss_upper_limit = 20

"""
Quickly compute byte-by-byte checksum
"""
def compute_XOR_checksum(data):
    checksum = 0
    for byte in data:
        checksum ^= byte
    return checksum

"""
Continuously listen for data from the satellite server and print it
"""
def receive_data(sock, server_addr):
    print("Running receive data task")
    while True:
        try:
            data, server_addr = sock.recvfrom(64)
            if data:
                print("\nReceived from satellite:", data)
                print("\nReceived from satellite:", data.hex())

                tc_param = data[0]
                print(" First part of the data is: ", data[0])

                match tc_param:
                    case 201:
                        # compute checksum (removing last 1 byte of checksum and 3 bytes due to padding) and compare
                        tm = TM_201.from_buffer_copy(data)
                        if tm.checksum == compute_XOR_checksum(data[:-4]):  
                            # print data as TM 201
                            print(f'[{datetime.now()}] System Check TM with params: CPU Usage {tm.cpu_usage}%, Memory Usage: {tm.memory_usage}%, Battery Usage: {tm.battery_level}%')
                        else:
                            # print error
                            print(f'[{datetime.now()}] [ERROR] System Check TM with invalid checksum')
                    case 202:
                        # compute checksum (removing last 1 byte of checksum and 3 bytes due to padding) and compare
                        tm = TM_202.from_buffer_copy(data)
                        if tm.checksum == compute_XOR_checksum(data[:-4]):
                            # print data as TM 202
                            print(f'[{datetime.now()}] Orbital Path TM with params: Vx {tm.Vx}m/s, Vy: {tm.Vy}m/s, Vz: {tm.Vz}m/s')
                        else:
                            # print error
                            print(f'[{datetime.now()}] [ERROR] Orbital Path TM with invalid checksum')
                    case 203:
                        # compute checksum (removing last 1 byte of checksum and 3 bytes due to padding) and compare
                        tm = TM_203.from_buffer_copy(data)
                        if tm.checksum == compute_XOR_checksum(data[:-4]):
                            # print data as TM 203
                            print(f'[{datetime.now()}] Payload Data TM with params: Payload ID: {tm.payload_id}, Operation: {tm.operational_status}, measurement_1: {tm.m1}, measurement_2: {tm.m2}, measurement_3: {tm.m3}')
                        else:
                            # print error
                            print(f'[{datetime.now()}] Payload Data TM with invalid checksum')
                    case 0x01:
                        # compute checksum (removing last 1 byte of checksum and 3 bytes due to padding) and compare
                        tm = TM_001.from_buffer_copy(data)
                        if tm.checksum == compute_XOR_checksum(data[:-4]):
                            # print data as TM 001
                            print(f'[{datetime.now()}] Informational Error TM with params: Payload ID: {tm.command_id}')
                        else:
                            # print error
                            print(f'[{datetime.now()}] 001 TM with invalid checksum')
                    case 0x02:
                        # compute checksum (removing last 1 byte of checksum and 3 bytes due to padding) and compare
                        tm = TM_002.from_buffer_copy(data)
                        if tm.checksum == compute_XOR_checksum(data[:-4]):
                            # print data as TM 002
                            print(f'[{datetime.now()}] Significant Error TM with params: Payload ID: {tm.command_id}')
                        else:
                            # print error
                            print(f'[{datetime.now()}] 002 TM with invalid checksum')
                    case _:
                        print("Unrecognized TC")
            else:
                print("\n Satellite Server closed the connection.")
                break
        except Exception as e:
            print(f"\nError receiving data: {e}")
            break

"""
Entry point, setup sockets, provide user menu, and send data
"""
def main():
    # define same port and address as C
    PORT = 2300
    server_addr = ('localhost', PORT)

    # set socket address family and type
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)    

    try:
        # start a thread to listen for data from the server
        recv_thread = threading.Thread(target=receive_data, args=(s,server_addr,))
        recv_thread.daemon = True  # daemon thread will close when main thread exits
        recv_thread.start()

        while True:
            valid_input = False
            user_input = ""
            data = None # declare out data but don't initialize it yet

            # repeat until user input is valid
            while not valid_input:
                user_input = input("Enter your choice of TC to send (or q to exit, any other key to reprint menu):\n1 - TC(101),\n2 - TC(102),\n30 - TC(103(Camera, Deactivate)),\n31 - TC(103(Camera, Activate)), \n40 - TC(103(Thermal, Deactivate)), \n41 - TC(103(Thermal, Activate))\n: ")

                match user_input:
                    case '1':
                        data = TC_101(101, 101)
                        valid_input = True
                    case '2':
                        data = TC_102(102, 10.0, 10.0, 10.0, 7)
                        valid_input = True
                    case '30':
                        data = TC_103(103, 0, 0, 103)
                        valid_input = True
                    case '31':
                        data = TC_103(103, 1, 0, 102)
                        valid_input = True
                    case '40':
                        data = TC_103(103, 0, 1, 102)
                        valid_input = True
                    case '41':
                        data = TC_103(103, 1, 1, 103)
                        valid_input = True
                    case 'q' | 'Q':
                        print("Exiting simulator...")
                        exit()  # quit the program, still execute the finally 
                    case _:
                        print("Not an option, please try again")
            
            # mimic PDU loss, randomly the drop the packet
            if random.randint(0, packet_loss_upper_limit) == 0:
                # for debug purposes, print the packet loss. Realistically, we would think the packet it sent but not know it wasn't received
                print("[DEBUG] Packet Loss")
                continue
            
            nbytes_sent = s.sendto(data, server_addr)
            print("Sent {:d} bytes".format(nbytes_sent))

    except AttributeError as ae:
        print("Error creating the socket: {}".format(ae))
    except socket.error as se:
        print("Exception on socket: {}".format(se))
    except KeyboardInterrupt:
        s.close()
    finally:
        print("Closing socket")
        s.close()


if __name__ == "__main__":
    main()