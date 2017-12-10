<?php
namespace SolarApi;

use SolarApi\Z85;

class Message
{
    function __construct($startTime, $endTime, $data) {
		$this->startTime = \DateTime::createFromFormat('Y-m-d\TH:i:sP', $startTime);
		$this->endTime = \DateTime::createFromFormat('Y-m-d\TH:i:sP', $endTime);
			
		$values = \array_map(function($v){return $v/1000;}, $data); 
		$valuesLength = count($values);
		if ($valuesLength % 10 != 0 ) { throw new \Exception("Message - values lenght [$valuesLength] must be a multiple of 10"); }		
		$numberOfMeasurements = $valuesLength / 10;
		$timeDiff = $this->endTime->getTimestamp() - $this->startTime->getTimestamp();
		$timeIncrement = $timeDiff;
		if($numberOfMeasurements > 1) {
			$timeIncrement = $timeDiff / ($numberOfMeasurements - 1);
		}	
		$measurements = array();
		for ($i=0 ; $i < $numberOfMeasurements ; $i++) { 
			$timestamp = $this->startTime->getTimestamp() + ($i * $timeIncrement);
			$date = new \DateTime();
			$date = $date->setTimestamp($timestamp);
			$measurements[] = new Measurement($date, array_slice($values, ($i*10) , 10));	
		}

		$this->measurements = $measurements;
    }
	
	
}
