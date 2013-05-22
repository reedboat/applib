#!/usr/bin/env python
# coding:utf-8

import struct, socket 

class Segment():
    host = ''
    port = 0
    cs = None
    headLen = 5

    def __init__(self, host, port):
        self.host = host
        self.port = port

    def segment(self, content):
        content = content.decode('utf-8').encode('gbk')

        message = self.encode(content)
        self.cs.send(message)

        messageHead = self.cs.recv(self.headLen)
        messageLen = self.decodeHead(messageHead)
        messageBody = self.cs.recv(messageLen - self.headLen)
        result = self.decodeBody(messageBody)

        result = result.decode('gbk').encode('utf-8')
        return result

    def connect(self):
        self.cs = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.cs.connect((self.host, self.port))

    def encode(self, content):
        totalLen = self.headLen + len(content) + 5
        return struct.pack('!bibi' + str(len(content)) + 's', 0x01, totalLen,  0x01, len(content), content)

    def decodeBody(self, messageBody):
        n, l, result = struct.unpack('!bi6x' + str(len(messageBody)-11) + 's', messageBody)
        return result

    def decodeHead(self, messageHead):
        version, messageLen =  struct.unpack('!bi', messageHead)
        return messageLen

    def parse(self, content):
        keywords = []
        pairs = content.split(';')
        for pair in pairs:
            if len(pair) > 0:
                keywords.append(pair)
        return keywords

    def close(self):
        self.cs.close()






#connect server
if __name__ == '__main__':
    content  = '我相信南方供暖气将是中国的接下来的消费热点之一。我常对外国人说，去看看中国的南方的冬天吧，看看那些没有暖气的房间，你们就知道中国还是发展中国家了。冬天待在有暖气的房间里是中国南方民众的基本人权，无论有多少困难，实现它都应是中国现代化的最重要目标之一'
    segment = Segment('10.1.148.155', 12001)
    result  = segment.segment(content)
    segment.close()

    print result
    print segment.parse(result)

