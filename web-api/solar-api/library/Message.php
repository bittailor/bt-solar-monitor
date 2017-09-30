<?php
namespace SolarApi;

use SolarApi\Z85;

class Message
{
  
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

    //$raw = pack("C*", ...$decoded);
    //$values = array_values(unpack('n*', $raw));
    return  $values;
  }
  

}
