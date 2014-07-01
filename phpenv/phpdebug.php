<?php
//methods:
//firephp
//errors
//展现形式：普通文本，按行， 按表格
//
class PhpDebug {

    //output: page, cli, webconsole
    public $times = array();
    private $colors = array(
        array('web' => '#C91C02', 'cli' => "\033[31m"),
        array('web' => '#A0D36E', 'cli' => "\033[32m"),
        array('web' => '#FACB47', 'cli' => "\033[33m"),
        array('web' => '#0687ED', 'cli' => "\033[34m"),
        array('web' => '#aa00ff', 'cli' => "\033[35m"),
        array('web' => '#00f6fe', 'cli' => "\033[36m"),
        array('web' => '#F8F8F8', 'cli' => "\033[37m"),
        array('web' => '#aaaaaa', 'cli' => "\033[30m")
    );

    public static function enable()
    {
        error_reporting(E_ALL & ~E_NOTICE);
        ini_set('display_errors', true);
    }

    public function profile($label='default')
    {
        $time = round(microtime(true) * 1000);
        $cost = $time - $this->last;
        $this->echo("$label " . ($cost > 0 ? '+ ' . $cost : $cost) . ' ms');
        $this->last = $time;
    }

    public function before(){
    }
    public function after(){
    }

    public function pre($param)
    {
        if (!headers_sent()){
            header("Content-type: text/html; charset=utf-8");
        }

        $is_cli = php_sapi_name() == 'cli';
        echo ($is_cli ? "" : "<pre>");
        foreach (func_get_args() as $arg){
            print_r($arg);
            if (!is_array($arr) && !is_object($arr)) echo(!$is_cli ? '<br />' : "\n");
        }
        echo ($is_cli ? "" : "</pre>");
    }

    public function pred()
    {
        call_user_func_array(array(self, 'pre'), func_get_args());
        exit;
    }

    public function pre_trace()
    {
        $ex = new Exception();
        echo self::pre($ex->getTraceAsString());
    }

    public function pred_trace()
    {
        $ex = new Exception();
        echo self::pred($ex->getTraceAsString());
    }
    
    protected function memory($label='default')
    {
        $usage = number_format(memory_get_usage() / 1024 / 1024, 2).' MB';
        $peak = number_format(memory_get_peak_usage() / 1024 / 1024, 2).' MB';
        $name = 'Memory Usage';
        if ($nameRaw) {
            $name = $name.': '.$label;
        }
        $msg = array(
            "Current: \t".$usage,
            "Peak: \t\t".$peak
        );
        $msg = implode("\n", $msg);
        $this->pre($msg);
    }
    
    

    public function trace($message)
    {
        return null;
    }

    //firephp
    public function fire($data, $label)
    {
        return null;
    }
    
    public function xhprof($param)
    {
        return null;
    }

    public function dump($param)
    {
        return null;
    }
    
}
