<?php
namespace SolarApi;


class Reading
{
    function __construct($current, $voltage) {
        $this->current = $current;
        $this->voltage = $voltage;
	}
	
}
