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
        $message = $this->createExampleMessageWithThreeMeasurements();
        $this->assertEquals(3, count($message->measurements));
    }

    public function testMessage_MeasurementDates()
    {
        $message = $this->createExampleMessageWithThreeMeasurements();
        $this->assertEquals(new DateTime('2017-10-01T15:14:15CEST'), $message->measurements[0]->timestamp);
        $this->assertEquals(new DateTime('2017-10-01T13:24:15Z'),    $message->measurements[1]->timestamp);
        $this->assertEquals(new DateTime('2017-10-01T14:34:15+0100'), $message->measurements[2]->timestamp);
    }

    public function testMessage_MeasurementReadings()
    {
        $message = $this->createExampleMessageWithThreeMeasurements();
        $this->assertEquals(0.139, $message->measurements[0]->readings[2]->current);
        $this->assertEquals(1.168, $message->measurements[0]->readings[5]->voltage);
        $this->assertEquals(0.219, $message->measurements[1]->readings[0]->current);
        $this->assertEquals(1.248, $message->measurements[1]->readings[3]->voltage);
        $this->assertEquals(0.359, $message->measurements[2]->readings[4]->current);
        $this->assertEquals(1.328, $message->measurements[2]->readings[1]->voltage);
    }

    public function testMessage_OneEntry()
    {
        $message = $this->createExampleMessageWithOneMeasurements();
        $this->assertEquals(new DateTime('2017-10-01T13:14:15Z'), $message->measurements[0]->timestamp);
    }

    public function testMessage_TwoEntry()
    {
        $message = $this->createExampleMessageWithTwoMeasurements();
        $this->assertEquals(new DateTime('2017-10-01T13:14:15Z'), $message->measurements[0]->timestamp);
        $this->assertEquals(new DateTime('2017-10-01T13:53:15Z'), $message->measurements[1]->timestamp);
    }

    function createExampleMessageWithThreeMeasurements() {
         return new Message(
            '2017-10-01T13:14:15Z',
            '2017-10-01T13:34:15Z',
            array(
                119 , 1118,     
                129 , 1128,     
                139 , 1138,     
                149 , 1148,     
                159 , 1158,     
                169 , 1168,
                
                219 , 1218,     
                229 , 1228,     
                239 , 1238,     
                249 , 1248,     
                259 , 1258,     
                269 , 1268,

                319 , 1318,     
                329 , 1328,     
                339 , 1338,     
                349 , 1348,     
                359 , 1358,     
                369 , 1368,
            ));
    }

    function createExampleMessageWithOneMeasurements() {
        return new Message(
           '2017-10-01T13:14:15Z',
           '2017-10-01T13:14:15Z',
           array(
               119 , 1118,     
               129 , 1128,     
               139 , 1138,     
               149 , 1148,     
               159 , 1158,     
               169 , 1168,
           ));
   }

   function createExampleMessageWithTwoMeasurements() {
    return new Message(
       '2017-10-01T13:14:15Z',
       '2017-10-01T13:53:15Z',
       array(
           119 , 1118,     
           129 , 1128,     
           139 , 1138,     
           149 , 1148,     
           159 , 1158,     
           169 , 1168,
        
           319 , 1318,     
           329 , 1328,     
           339 , 1338,     
           349 , 1348,     
           359 , 1358,     
           369 , 1368,
       ));
    }
}
?>
