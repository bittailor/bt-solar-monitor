<?php
namespace SolarApi;


class Measurement
{
    function __construct($timestamp ,$readings) {
        $this->timestamp = $timestamp;
        $this->readings = $readings;
	}
}
