<?php
use PHPUnit\Framework\TestCase;
use SolarApi\MessageFactory;


class MessageFactoryTest extends TestCase
{
    public function testUnpack_int16_t_positive()
    {
        $messageString = "5.vUd";
        $values = MessageFactory::unpack($messageString);     
        $this->assertEquals('array', gettype($values));
        $this->assertEquals(2, count( $values));
        $this->assertEquals(array(4567, 4711), $values);
    }

    public function testUnpack_int16_t_negative()
    {
        $messageString = "%nSb#";
        $values = MessageFactory::unpack($messageString);     
        $this->assertEquals('array', gettype($values));
        $this->assertEquals(2, count( $values));
        $this->assertEquals(array(-1, -2), $values);
    }

    public function testUnpack_int16_t_boundries()
    {
        $messageString = "FbY*70096100960(D1FQ";
        $values = MessageFactory::unpack($messageString);     
        $this->assertEquals('array', gettype($values));
        $this->assertEquals(array(32767, 2345 , 1, 0, 0, -1 , -5432 , -32768), $values);
    }

    public function testtestUnpack_SolarMessage()
    {
        $messageString = "00S(M00S(E00J/A00A-u%n0fi000Cc00S(H00S(y00A-z00A-u%n0fg000Cc00S(H00S(z00J/A00rVu%n0fg000Ca00S(H00S(z00J/C00rVu%n0fi000Cb00S(G00S(x00S(B00A-v%n0fg000Cc00S(H00S(z00J/A00A-u%n0fg000Cb";
        $values = MessageFactory::unpack($messageString);     
        $this->assertEquals('array', gettype($values));
        $this->assertEquals(2*6*6, count( $values));
    }


    public function testCreate_Timestamps()
    {
        $rawData = "0|0|1|2017-10-01T13:14:15Z|1KnXT1KnXT1KnXT1KnXT1KnXT1KnXT2{--z2{--z2{--z2{--z2{--z2{--z4si^f4si^f4si^f4si^f4si^f4si^f|2017-10-01T13:34:15Z";
        $factory = new MessageFactory();
        $message = $factory->create($rawData);
        $this->assertEquals(new DateTime('2017-10-01T13:14:15Z'), $message->startTime);
        $this->assertEquals(new DateTime('2017-10-01T13:34:15Z'), $message->endTime);
    }

    public function testCreate_Measurents()
    {
        $rawData = "0|0|1|2017-10-01T13:14:15Z|1wysJ1H6z(1R-Hk1:zOO1(7V}20:+p2Sah52:^oz2(Dv+31bDa3b!KE3mER*3(?5O41Hc}4cfkp4m&rT4xIz04IgGu|2017-10-01T13:34:15Z";
        $factory = new MessageFactory();
        $message = $factory->create($rawData);
        $this->assertEquals(3, count($message->measurements));
    }
}
?>
