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

    public function tag($label = 'default'){
        static $count;
        $count ++;
        $message = $count . " [" . $label . "]";
        $traces = debug_backtrace(DEBUG_BACKTRACE_PROVIDE_OBJECT, 2);
        $function = '__main__';
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
            $function = "$class::$function";
        }
        else {
        }
        $message = $count . " $function() in file $file, line $line";
        $this->pre($message);
    }

    public function is_cli(){
        return php_sapi_name() == 'cli';
    }

    public function pre($param)
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
        echo self::pre($ex->getTraceAsString());
    }

    public function pred_trace()
    {
        $ex = new Exception();
        echo self::pred($ex->getTraceAsString());
        exit;
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
}
?>

<?php
$debug = new WF_Debug();
function aa(){
    $s = time();
    bb();
    $s = time();
    $debug = new WF_Debug();
    $debug->tag();
}
function bb(){
    $debug = new WF_Debug();
    $debug->tag();
}
class C{
    function cc(){
        aa();
    }
    function ee(){
        $debug = new WF_Debug();
        $debug->tag();
    }
}

$debug->tag();
aa();
$c = new C();
$c->cc();
$c->ee();
$debug->dump_on(array('aaa'), 1);
