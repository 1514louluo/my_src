#!/bin/env python
import struct

def pack_str_field(k, v):
    len_k = len(k)
    len_v = len(v)
    fmt = 'i%dsi%ds' % (int(len_k), int(len_v))
    len_field = len_k + len_v + 8
    fmt = '!i' + fmt

    return struct.pack(fmt, len_field, len_k, k, len_v, v)

def pack_num_field(k, v):
    len_k = len(k)
    len_v = 4
    fmt = 'i%ssii' % str(len_k)
    len_field = struct.calcsize(fmt)
    fmt = '!i' + fmt

    return struct.pack(fmt, len_field, len_k, k, len_v, v)

def pack_uint64_field(k, v):
    len_k = len(k)
    len_v = 8
    fmt = 'i%ssiQ' % str(len_k)
    len_field = struct.calcsize(fmt)
    fmt = '!i' + fmt

    return struct.pack(fmt, len_field, len_k, k, len_v, v)

def pack_getusrinfor(datagram):
    ''' pack protocol getusrinfor '''
    bytes_buffer = ''
    try:
        bytes_buffer += pack_str_field('request', 'getusrinfor')
        bytes_buffer += pack_str_field('sessionid', str(datagram['sessionid']))

        len_row = len(bytes_buffer)
        len_record = len_row + 8

        fmt = '!ii'
        req = struct.pack(fmt, len_record, len_row)
        bytes_buffer = req + bytes_buffer
    finally:
        return bytes_buffer

def pack_getuserinfo_base(datagram):
    ''' pack protocol getuserinfo_base '''
    bytes_buffer = ''
    try:
        bytes_buffer += pack_str_field('request', 'getuserinfo_base')
        bytes_buffer += pack_str_field('userid', str(datagram['userid']))
        bytes_buffer += pack_str_field('usertype', str(datagram['usertype']))

        len_row = len(bytes_buffer)
        len_record = len_row + 8

        fmt = '!ii'
        req = struct.pack(fmt, len_record, len_row)
        bytes_buffer = req + bytes_buffer
    finally:
        return bytes_buffer

def pack_checksession(datagram):
    ''' pack protocol checksession '''
    bytes_buffer = ''
    try:
        bytes_buffer += pack_str_field('request', 'checksession')
        bytes_buffer += pack_str_field('sessionid', str(datagram['sessionid']))

        len_row = len(bytes_buffer)
        len_record = len_row + 8

        fmt = '!ii'
        req = struct.pack(fmt, len_record, len_row)
        bytes_buffer = req + bytes_buffer
    finally:
        return bytes_buffer

def pack_getuserinfo_client(datagram):
    ''' pack protocol getuserinfo '''
    bytes_buffer = ''
    try:
        bytes_buffer += pack_str_field('request', 'getuserinfo')
        bytes_buffer += pack_str_field('userid', str(datagram['userid']))
        bytes_buffer += pack_str_field('usertype', str(datagram['usertype']))
        bytes_buffer += pack_str_field('clientoperationid', 
                                       str(datagram['clientoperationid']))
        bytes_buffer += pack_str_field('clientsessionid', 
                                       str(datagram['clientsessionid']))

        len_row = len(bytes_buffer)
        len_record = len_row + 8

        fmt = '!ii'
        req = struct.pack(fmt, len_record, len_row)
        bytes_buffer = req + bytes_buffer
    finally:
        return bytes_buffer

def pack_gateway_getuserinfo(datagram):
    ''' pack protocol getuserinfo '''
    bytes_buffer = ''
    try:
        bytes_buffer += pack_str_field('request', 'getuserinfo')
        bytes_buffer += pack_str_field('userid', str(datagram['userid']))
        bytes_buffer += pack_str_field('usertype', str(datagram['usertype']))
        for req_field in list(datagram['req_field']):
            bytes_buffer += pack_str_field(req_field, '')

        len_row = len(bytes_buffer)
        len_record = len_row + 8

        fmt = '!ii'
        req = struct.pack(fmt, len_record, len_row)
        bytes_buffer = req + bytes_buffer
    finally:
        return bytes_buffer

def pack(datagram):
    ''' datagram data type: dictionary
        return bytes_buffer type: string
    '''
    bytes_buffer = ''
    try:
        if datagram['request'] == 'getusrinfor':
            bytes_buffer = pack_getusrinfor(datagram)
        elif datagram['request'] == 'checksession':
            bytes_buffer = pack_checksession(datagram)
        elif datagram['request'] == 'getuserinfo_base':
            bytes_buffer = pack_getuserinfo_base(datagram)
    finally:
        return bytes_buffer

def unpack_field(fields):
    datagram_row = {}
    try:
        for field in fields:
            field_len = field['field_len']
            kv_bytes = field['kv_bytes']
            pos = 0
            name_len = struct.unpack('!i', kv_bytes[pos:pos+4])[0]
            pos += 4
            field_name = struct.unpack('!%ss' % name_len, kv_bytes[pos:pos+int(name_len)])[0]
            pos += int(name_len)
            value_len = struct.unpack('!i', kv_bytes[pos:pos+4])[0]
            pos += 4
            field_value = struct.unpack('!%ss' % value_len, kv_bytes[pos:pos+int(value_len)])[0]
            datagram_row[field_name] = field_value
    finally:
        return datagram_row

def unpack_row(rows):
    datagram = []
    try:
        for row in rows:
            fields = []
            row_len = row['row_len']
            field_bytes = row['field_bytes']
            pos = 0
            while pos < row_len:
                field = {}
                field_len = struct.unpack('!i', field_bytes[pos:pos+4])[0]
                pos += 4
                field['field_len'] = field_len
                field['kv_bytes'] = field_bytes[pos:pos+int(field_len)]
                pos += int(field_len)
                fields.append(field)
            datagram_row = unpack_field(fields)
            datagram.append(datagram_row)
    finally:
        return datagram

def unpack(bytes_buffer):
    ''' bytes_buffer type: string
        return datagram type: list, like: [row, row,...], 
               row type: dictionary, like {'key':'value', 'key':'value',...}
    '''
    rows = []
    try:
        record_len = int(struct.unpack('!i', bytes_buffer[:4])[0]) - 4
        rows_bytes = bytes_buffer[4:]
        pos = 0
        while pos < record_len:
            row = {}
            row_len = struct.unpack('!i', rows_bytes[pos:pos+4])[0]
            pos += 4
            field_bytes = rows_bytes[pos:pos+int(row_len)]
            pos += int(row_len)
            row['row_len'] = row_len
            row['field_bytes'] = field_bytes
            rows.append(row)
    finally:
        return unpack_row(rows)


