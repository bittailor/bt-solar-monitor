<?php
namespace SolarApi;

class StopWatch
{
    private $micros;

    public function __construct() {
        $this->micros = microtime(true);    
    }    
    
    public function lap() {
        $start = $this->micros;
        $this->micros = microtime(true);
        return ($this->micros - $start) * 1000;   
    } 

    public function lapStr() {
        return number_format($this->lap(), 3);   
    } 
}
