<?php
require __DIR__ . '/phpdebug.class.php';
//headers_list 判断是xml/html/json?

if (!function_exists('_dump')){
    //
    function _dump(){
        if (is_terminal){
            print_r($message);
        }
        else {
            fb($message);
        }
    }

}

if (!function_exists('_log')){
    function _log(){
    }
}
