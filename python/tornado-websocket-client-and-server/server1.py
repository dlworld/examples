#!/usr/bin/env python
# -*- coding: utf-8 -*-
import datetime
import tornado.ioloop
import tornado.web
import tornado.websocket
import threading
import functools

import json


class ThreadWorker(threading.Thread):
    def __init__(self, callback):
        threading.Thread.__init__(self)
        self.callback = callback

    def run(self):
        msg = "return from threadworker"
        tornado.ioloop.IOLoop.instance().add_callback(functools.partial(self.callback, msg))
 

class MainHandler(tornado.websocket.WebSocketHandler):
    # 服务器内部的数据结构，保存所有的client连接
    clients = []

    def log(self, text):
        print datetime.datetime.now().time(), self.request.remote_ip, text

    def check_origin(self, origin):
        return True

    def get_compression_options(self):
        return {}

    def open(self):
        self.log('connected')
        self.clients.append(self)
        message = {"category": "vm-backup", "operation": "backup", "data": {
            "backup-task-id": 333,
            "vm": {
                "control-node": {"ip": "", "user": "", "password": ""},
                "resource-pool": {"id": 1, "name": ""},
                "compute-node": {"id": 2, "ip":"127.0.0.1"},
                "vm": {"id": 1},
            },
            "snapshot":["/home/dlworld/Workspace/nfs/8685c595-3158-4914-9ea2-20897bf5e94a",],
            "storage": {"storage_type":"local",
                        "storage_ip":"127.0.0.1",
                        "device":"",
                        "mount_point":"/home/dlworld/Workspace/nfs",},
            "options": {
                "compute-node-compression":None,
                "backup-server-compression": None,}
        }}

        print message
        self.write_message(json.dumps(message))

    def on_message(self, message):
        self.log('recv: ' + message)
        # d = json.loads(message)
        # dispatch(d)
        t = ThreadWorker(self.update_progress)
        t.setDaemon(True)
        t.start()

    def update_progress(self, message):
        self.write_message(json.dumps(message))

    def on_close(self):
        self.clients.remove(self)
        self.log('close')

if __name__ == '__main__':
    application = tornado.web.Application([
        (r'/', MainHandler),
    ])
    application.listen(8001)
    tornado.ioloop.IOLoop.instance().start()
