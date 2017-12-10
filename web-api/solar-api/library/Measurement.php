<?php
namespace SolarApi;


class Measurement
{
    function __construct($timestamp , $values) {
        $this->timestamp = $timestamp;
        $this->values = $values;
	}
}
