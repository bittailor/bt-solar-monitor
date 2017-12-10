<?php
use PHPUnit\Framework\TestCase;
use SolarApi\Message;


class MessageTest extends TestCase
{
    protected function setUp()
    {
        date_default_timezone_set('Europe/Zurich');
    }

    public function testMessage_MeasurementsCount()
    {
        $message = self::createExampleMessageWithThreeMeasurements();
        $this->assertEquals(3, count($message->measurements));
    }

    public function testMessage_MeasurementDates()
    {
        $message = self::createExampleMessageWithThreeMeasurements();
        $this->assertEquals(new DateTime('2017-10-01T15:14:15CEST'), $message->measurements[0]->timestamp);
        $this->assertEquals(new DateTime('2017-10-01T13:24:15Z'),    $message->measurements[1]->timestamp);
        $this->assertEquals(new DateTime('2017-10-01T14:34:15+0100'), $message->measurements[2]->timestamp);
    }

    public function testMessage_MeasurementReadings()
    {
        $message = self::createExampleMessageWithThreeMeasurements();
        $this->assertEquals(0.139, $message->measurements[0]->values[4]);
        $this->assertEquals(1.158, $message->measurements[0]->values[9]);
        $this->assertEquals(0.219, $message->measurements[1]->values[0]);
        $this->assertEquals(1.248, $message->measurements[1]->values[7]);
        $this->assertEquals(0.359, $message->measurements[2]->values[8]);
        $this->assertEquals(1.328, $message->measurements[2]->values[3]);
    }

    public function testMessage_OneEntry()
    {
        $message = self::createExampleMessageWithOneMeasurements();
        $this->assertEquals(new DateTime('2017-10-01T13:14:15Z'), $message->measurements[0]->timestamp);
    }

    public function testMessage_TwoEntry()
    {
        $message = self::createExampleMessageWithTwoMeasurements();
        $this->assertEquals(new DateTime('2017-10-01T13:14:15Z'), $message->measurements[0]->timestamp);
        $this->assertEquals(new DateTime('2017-10-01T13:53:15Z'), $message->measurements[1]->timestamp);
    }

    static function createExampleMessageWithThreeMeasurements() {
         return new Message(
            '2017-10-01T13:14:15Z',
            '2017-10-01T13:34:15Z',
            array(
                119 , 1118,     
                129 , 1128,     
                139 , 1138,     
                149 , 1148,     
                159 , 1158,     
                
                219 , 1218,     
                229 , 1228,     
                239 , 1238,     
                249 , 1248,     
                259 , 1258,     
                
                319 , 1318,     
                329 , 1328,     
                339 , 1338,     
                349 , 1348,     
                359 , 1358,     
                
            ));
    }

    static function createExampleMessageWithOneMeasurements() {
        return new Message(
           '2017-10-01T13:14:15Z',
           '2017-10-01T13:14:15Z',
           array(
               119 , 1118,     
               129 , 1128,     
               139 , 1138,     
               149 , 1148,     
               159 , 1158,     
           ));
   }

   static function createExampleMessageWithTwoMeasurements() {
        return new Message(
        '2017-10-01T13:14:15Z',
        '2017-10-01T13:53:15Z',
        array(
            119 , 1118,     
            129 , 1128,     
            139 , 1138,     
            149 , 1148,     
            159 , 1158,     
            
            319 , 1318,     
            329 , 1328,     
            339 , 1338,     
            349 , 1348,     
            359 , 1358,     
        ));
    }
}
?>
