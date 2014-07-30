<?php
function wdenable(){
    Yii::app()->debug->enable();
}

function wdpre(){
    Yii::app()->debug->block();
}

function wdtag($label='default'){
    Yii::app()->debug->tag($label);
}

function wdprof($tag=''){
    Yii::app()->debug->tag($tag);
}

function wdonreq($message, $req='debug'){
    Yii::app()->debug->echo_on($tag);
}

function wdtrace($message=null){
    Yii::app()->debug->trace($message);
}
