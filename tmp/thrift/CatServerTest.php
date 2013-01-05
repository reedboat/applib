<?php
$time0 = microtime() * 1000;
error_reporting(E_NONE);
$THRIFT_ROOT= "/home/zhangqm/workspace/library/vender/thrift";
require_once $THRIFT_ROOT.'/Thrift.php';
$time1 = microtime() * 1000;
require_once $THRIFT_ROOT.'/protocol/TBinaryProtocol.php';
require_once $THRIFT_ROOT.'/transport/TSocket.php';
require_once $THRIFT_ROOT.'/transport/THttpClient.php';
require_once $THRIFT_ROOT.'/transport/TBufferedTransport.php';

$GEN_DIR = './gen-php/Cat';
require_once $GEN_DIR.'/CatText.php';
require_once $GEN_DIR.'/Cat_types.php';
error_reporting(E_ALL);

$host = '127.0.0.1';
$port = '9090';

$time2 = microtime() * 1000;
$socket    = new TSocket($host, $port);
$transport = new TBufferedTransport($socket , 1024, 1024);
$protocol  = new TBinaryProtocol($transport);
$client    = new CatTextClient($protocol);
$time3 = microtime() * 1000;
$transport->open();
$res = $client->CatName('java php google 微软 新浪 腾讯 搜狐 盛大');
$time4 = microtime() * 1000;
var_dump($res);
echo ($time1 - $time0) ."\n";
echo ($time2 - $time1) ."\n";
echo ($time3 - $time2) ."\n";
echo ($time4 - $time3) ."\n";
echo "\n";
$res = $client->TextFeature(iconv("utf-8","gbk", 'java php google 微软 新浪 腾讯 搜狐 盛大'));
var_dump(iconv("gbk", "utf-8", $res));
$transport->close();
sleep(10);
