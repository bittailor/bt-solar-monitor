<?php
use PHPUnit\Framework\TestCase;
use SolarApi\MessageFactory;
use SolarApi\MessageToXivelyConverter;


class MessageToXivelyConverterTest extends TestCase
{
    protected function setUp() : void
    {
        date_default_timezone_set('Europe/Zurich');
    }

    public function testMessage_x() : void
    {
        $rawData = "0|2017-10-01T13:14:15Z|1wysJ1H6z(1R-Hk1:zOO1(7V}20:+p2Sah52:^oz2(Dv+31bDa3b!KE3mER*3(?5O41Hc}4cfkp|2017-10-01T13:34:15Z";
        $factory = new MessageFactory();
        $converter = new MessageToXivelyConverter();
        $message = $factory->create($rawData);
        $x = $converter->convert($message);
        #error_log("**Xively**\n"); 
        #error_log("**Xively*\n");  
        #error_log(print_r($x, true)); 
        $this->assertEquals(new DateTime('2017-10-01T13:14:15Z'), $message->startTime);
        $this->assertEquals(new DateTime('2017-10-01T13:34:15Z'), $message->endTime);
    }
  
}



