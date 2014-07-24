<?php
//问题：
// 1. web上的var_dump不友好，经常需要手工加<pre></pre>
// 2. 如何在日志中更好的展现数组, 又尽量不要破坏日志结构，否则不易分析 
// 3. 如何让调试信息和正式内容分开。
//    1) 调试信息会破坏正常的页面布局
//    2) 调试信息可能导致json等不能被解析
//    3) 调试信息分散以致无法获得好的反馈
//    4) 避免一次只能调试一小块。 
// 4. 确定问题的时候，经常会需要连续打点, 以确认问题到底发生在哪里。 
// 5. 调试数据要能够方便的去除。 关闭调试信息和从代码中去除
// 6. 如何能够快速profile一堆函数 以确定瓶颈
// 7. 能够快速的收集当前的trace
// 8. 能够展示当前的内存占用
//
//methods:
//firephp 调试信息输出到webconsole
//Krumo: http://krumo.sourceforge.net/ 更好的var_dump和print_r
//errors
//展现形式：普通文本，按行，按表格. 含颜色等样式
//输出渠道: Web.（页面, console） Termina(stdout, stderr)
//
//
class WF_Debug {

    private static $_instance = null;

    //output: page, cli, webconsole
    public $times      = array();
    private $tag_count = 0;
    private $profile_last_time = array();
    private $profiles_stack    = array();

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

    public static function instance()
    {
        if (self::$_instance === null){
            $className = __CLASS__;
            self::$_instance = new $className();
        }
        return self::$_instance;
    }

    public function enable()
    {
        error_reporting(E_ALL & ~E_NOTICE);
        ini_set('display_errors', true);
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
        $this->pre($message);
    }

    public function is_cli(){
        return php_sapi_name() == 'cli';
    }

    //web页面输出原格式。
    public function pre()
    {
        $is_cli = $this->is_cli();

        if (!$is_cli && !headers_sent()){
            header("Content-type: text/html; charset=utf-8");
        }

        echo ($is_cli ? "" : "<pre>");
        foreach (func_get_args() as $arg){
            print_r($arg);
            if (!is_array($arg) && !is_object($arg)){
                echo(!$is_cli ? '<br />' : "\n");
            } 
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
        self::pre($ex->getTraceAsString());
    }

    public function pred_trace()
    {
        $ex = new Exception();
        self::pred($ex->getTraceAsString());
        exit;
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
        $this->pre($name);
        $this->pre($msg);
    }
    
    

    public function trace($message)
    {
        $this->pre($message);
        $ex = new Exception();
        self::pre($ex->getTraceAsString());
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
        if ($this->is_cli()){
            var_dump($param);
        }
        else {
            echo "<pre style=\"border: 1px solid #000; overflow: auto; margin: 0.5em;\" >";
            var_dump($param);
            echo "</pre><br />";
        }
    }

    public function dump_on($message, $cond=true){
        if ($cond) {
            $this->dump($message);
        }
    }

    //输出消息+换行
    public function echoln($message = ''){
        $is_cli = $this->is_cli();
        echo $message;
        echo $is_cli ? PHP_EOL : "<br />" ;
    }
}
?>

<?php
$debug = WF_Debug::instance();
function aa(){
    $s = time();
    bb();
    $s = time();
    $debug = WF_Debug::instance();
    $debug->tag();
}
function bb(){
    $debug = WF_Debug::instance();
    $debug->tag();
}
class C{
    function cc(){
        aa();
    }
    function ee(){
        $debug = WF_Debug::instance();
        $debug->tag();
    }
}

$debug->tag();
aa();
$c = new C();
$c->cc();
$c->ee();
$debug->echoln('xxx');
$debug->dump_on(array('aaa'), 1);
$debug->memory();
$debug->trace("aaaa");
$debug->profile();
sleep(1);
$debug->profile();
sleep(2);
$debug->profile();
$debug->beginProfile('kkkk');
$debug->profile(1);
$debug->profile(2);
sleep(1);
$debug->profile(3);
$debug->endProfile();
$debug->profile(4);
