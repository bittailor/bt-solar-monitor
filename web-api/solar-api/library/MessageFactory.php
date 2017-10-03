<?php

namespace SolarApi;

class MessageFactory {

    
    // [data] => 16|1|2|2017-10-03T07:48:45Z|00S(i00S(800J/a00A-4%n0e)000B?00S(i00S(800J/a00A-4%n0e)000B?00S(i00S(800J/a00A-4%n0e)000B?00S(i00S(800J/a00A-4%n0e)000B?00S(i00S(800J/a00A-4%n0e)000B?00S(i00S(800J/a00A-4%n0e)000B?|2017-10-03T08:13:45Z

    function create($rawData) {
        $parts  = explode('|', $rawData);
        $publishId = $parts[0];
        $messageCount = $parts[1];
        $numberOfMessages = $parts[2];
        $startTime = $parts[3];
        $endTime = $parts[5];
        $data = self::unpack($parts[4]);

        return new Message($startTime, $endTime, $data);
    }
    
    

    public static function unpack ($messageString) {
		$decoded = Z85::decode($messageString);
		$values = array();
		for ($i = 0; $i < count($decoded); $i+=2) {
			$value = ( $decoded[$i] & 0xff);
			$value = ($value << 8) | ($decoded[$i+1] & 0xff);
			if($value >= 32768) {
				$value = -65536 + $value;
			}
			$values[] = $value;
		}
		return  $values;
	}

}