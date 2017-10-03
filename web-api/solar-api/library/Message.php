<?php
namespace SolarApi;

use SolarApi\Z85;

class Message
{
    function __construct($startTime, $endTime, $data) {
        $this->startTime = \DateTime::createFromFormat('Y-m-d\TH:i:s+', $startTime);
        $this->endTime = \DateTime::createFromFormat('Y-m-d\TH:i:s+', $endTime);
		
		if (count($data) % 2 != 0 ) { throw new Exception('Message - data must be a multiple of 2'); }		
		$readings = array(); 
		for ($i=0 ; $i < count($data) ; $i+=2) { 
			$readings[] = new Reading($data[$i]/1000,$data[$i+1]/1000);	
		}

		if (count($readings) % 6 != 0 ) { throw new Exception('Message - readings must be a multiple of 6'); }		
		$numberOfMeasurements = count($readings) / 6;
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
			#error_log(print_r($date, true)); 
			$measurements[] = new Measurement($date, array_slice($readings, ($i*6) , 6));	
		}

		$this->measurements = $measurements;
    }
	
	
}
