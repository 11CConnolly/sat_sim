"""
Ctypes: https://docs.python.org/3/library/ctypes.html
"""

from ctypes import *

""" This class defines a C-like struct for a TC(101) """
class TC_101(Structure):
    _fields_ = [("command_id", c_uint32),
                ("checksum", c_uint8)]

""" This class defines a C-like struct for a TC(102) """
class TC_102(Structure):
    _fields_ = [("command_id", c_uint32),
                ("deltaVx", c_float),
                ("deltaVy", c_float),
                ("deltaVz", c_float),
                ("checksum", c_uint8)]
                
""" This class defines a C-like struct for a TC(103) """
class TC_103(Structure):
    _fields_ = [("command_id", c_uint32),
                ("operation_code", c_uint32),
                ("payload_id", c_uint32),
                ("checksum", c_uint8)]

""" This class defines a C-like struct for a TM(201) """
class TM_201(Structure):
    _fields_ = [("status_code", c_uint32),
                ("cpu_usage", c_float),
                ("memory_usage", c_float),
                ("battery_level", c_float),
                ("checksum", c_uint8)]

""" This class defines a C-like struct for a TM(202) """
class TM_202(Structure):
    _fields_ = [("status_code", c_uint32),
                ("Vx", c_float),
                ("Vy", c_float),
                ("Vz", c_float),
                ("checksum", c_uint8)]

""" This class defines a C-like struct for a TM(203) """
class TM_203(Structure):
    _fields_ = [("status_code", c_uint32),
                ("payload_id", c_uint32),
                ("operational_status", c_uint32),
                ("m1", c_float),
                ("m2", c_float),
                ("m3", c_float),
                ("checksum", c_uint8)]
                
""" This class defines a C-like struct for a TM(001) """
class TM_001(Structure):
    _fields_ = [("status_code", c_uint32),
                ("command_id", c_uint32),
                ("checksum", c_uint8)]
                                
""" This class defines a C-like struct for a TM(002) """
class TM_002(Structure):
    _fields_ = [("status_code", c_uint32),
                ("command_id", c_uint32),
                ("checksum", c_uint8)]
