<?php
class WDebug extends CApplicationComponent {

    public $dump_method = 'print_r';

    private $times      = array();
    private $tag_count = 0;
    private $profile_last_time = array();
    private $profiles_stack    = array();
    private $is_cli ;

    public function init()
    {
        parent::init();
        $this->is_cli = 'cli' == php_sapi_name();
    }

    public function enable()
    {
        error_reporting(E_ALL & ~E_NOTICE);
        ini_set('display_errors', true);
    }

    public function disable($param)
    {
        ini_set('display_errors', false);
    }
    
    public function beginProfile($label){
        $this->profiles_stack[] = $label;
        $this->profile();
    }

    public function endProfile(){
        array_pop($this->profiles_stack);
    }

    public function profile($tag='')
    {
        if (count($this->profiles_stack) == 0){
            $this->beginProfile('default');
            return;
        }

        $label = array_slice($this->profiles_stack, -1, 1);
        $label = $label[0];
        $time = microtime(true) * 1000;
        if (!isset($this->profile_last_time[$label])){
            $cost = 0;
        }
        else {
            $cost = $time - $this->profile_last_time[$label];
        }
        $cost = round($cost, 3);
        $this->profile_last_time[$label] = $time;
        $this->echoln("[$label] $tag " . ($cost > 0 ? '+' . $cost : $cost) . ' ms');
    }
    
    
    public function tag($label = 'default'){
        $traces = debug_backtrace(DEBUG_BACKTRACE_PROVIDE_OBJECT, 2);
        $function = '{main}';
        $class    = '';
        if (count($traces) >= 2){
            $current  = $traces[1];
            $function = $current['function'];
            $class    = isset($current['class']) ? $current['class'] : '';
        }
         
        $current = $traces[0];

        $file = basename($current['file']);
        $line = $current['line'];
        if ($class){
            $function = "$class" . $current['type'] . "$function";
        }

        $message = "#" . $this->tag_count . " $function() in file $file, line $line";
        $this->tag_count += 1;
        $this->echoln($message);
    }

    public function trace($message=null)
    {
        if ($message !== null){
            $this->block($message);
        }
        $ex = new Exception();
        $this->block($ex->getTraceAsString());
    }


    public function memory($label='default')
    {
        $usage = number_format(memory_get_usage() / 1024 / 1024, 2).' MB';
        $peak = number_format(memory_get_peak_usage() / 1024 / 1024, 2).' MB';
        $name = 'Memory Usage';
        if ($label) {
            $name = $name.': '.$label;
        }
        $msg = array(
            "\tCurrent: \t".$usage,
            "\tPeak: \t\t".$peak
        );
        $msg = implode("\n", $msg);
        $this->echoln($name);
        $this->block($msg);
    }

    //只在url中带有debug参数的时候，输出
    public function echo_on($message, $debug_param='debug')
    {
        if (isset($_REQUEST[$debug_param])){
            $this->block($message);
        }
    }

    public function block($message=''){
        $is_cli = $this->is_cli;

        if (!$is_cli && !headers_sent()){
            header("Content-type: text/html; charset=utf-8");
        }

        echo ($is_cli ? PHP_EOL : "<pre>");
        foreach (func_get_args() as $arg){
            print_r($arg);
            if (!is_array($arg) && !is_object($arg)){
                echo(!$is_cli ? '<br />' : "\n");
            } 
        }
        echo ($is_cli ? PHP_EOL : "</pre>");
    }
    
    public function dump($param)
    {
        if ($this->is_cli){
            var_dump($param);
        }
        else {
            echo "<pre style=\"border: 1px solid #000; overflow: auto; margin: 0.5em;\" >";
            var_dump($param);
            echo "</pre><br />";
        }
    }

    public function block_on($message, $cond=true){
        if ($cond) {
            $this->block($message);
        }
    }

    //输出消息+换行
    public function echoln($message = ''){
        $message .= $this->is_cli ? PHP_EOL : "<br />" ;

        //$fh = fopen('php://stderr', 'w');
        //fwrite($fh, $message);
        echo $message;
    }
}
