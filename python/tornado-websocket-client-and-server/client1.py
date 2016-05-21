#!/usr/bin/env python
# -*- coding: utf-8 -*-
import json
import logging
import socket
import traceback

import tornado.gen
import tornado.ioloop
import tornado.websocket

logging.basicConfig()

connections = {
    'server1': {
        'description': 'AAA模块服务器',
        'conn': None,
        'url': 'ws://192.168.65.5:8091',
    },
}

@tornado.gen.coroutine
def process_connection(server_type):
    try:
        conn = yield tornado.websocket.websocket_connect(connections[server_type]['url'])
    except socket.error, e:
        # traceback.print_exc()
        print e
        return
    print server_type, 'connected'
    connections[server_type]['conn'] = conn
    while True:
        message = yield conn.read_message()
        if message is None:
            connections[server_type]['conn'] = None
            print server_type, 'closed'
            break
        print server_type, 'recv:', message

def check_connections():
    for server_type in connections.keys():
        if connections[server_type]['conn'] is None:
            print server_type, 'reconnecting...'
            process_connection(server_type)

def send_message_to_server1():
    message = {'category': 'vm-backup', 'operation': 'test', 'data': {'aaa': 'bbb'}}
    if connections['server1']['conn']:
        connections['server1']['conn'].write_message(json.dumps(message))

def close_connection_server1():        
    conn = connections["server1"]["conn"]
    if conn and hasattr(conn, 'close'):
        conn.close()

    print "closed"
    

tornado.ioloop.PeriodicCallback(check_connections, 5 * 1000).start()
tornado.ioloop.PeriodicCallback(send_message_to_server1, 1 * 1000).start()
tornado.ioloop.PeriodicCallback(close_connection_server1, 200 * 1000).start()
tornado.ioloop.IOLoop.instance().start()
