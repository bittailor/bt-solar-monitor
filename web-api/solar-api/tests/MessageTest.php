<?php
use PHPUnit\Framework\TestCase;
use SolarApi\Message;


class MessageTest extends TestCase
{
    public function testMessage_int16_t()
    {
        $messageString = "5.vUd";
        $values = Message::unpack($messageString);     
        $this->assertEquals('array', gettype($values));
        $this->assertEquals(2, count( $values));
        $this->assertEquals(array(4567, 4711), $values);
    }

    public function testMessage_int16_t_negative()
    {
        $messageString = "%nSb#";
        $values = Message::unpack($messageString);     
        $this->assertEquals('array', gettype($values));
        $this->assertEquals(2, count( $values));
        $this->assertEquals(array(-1, -2), $values);
    }

    public function testMessage_int16_t_borders()
    {
        $messageString = "FbY*70096100960(D1FQ";
        $values = Message::unpack($messageString);     
        $this->assertEquals('array', gettype($values));
        $this->assertEquals(array(32767, 2345 , 1, 0, 0, -1 , -5432 , -32768), $values);
    }


    

    public function testSolarMessage()
    {
        $messageString = "00S(M00S(E00J/A00A-u%n0fi000Cc00S(H00S(y00A-z00A-u%n0fg000Cc00S(H00S(z00J/A00rVu%n0fg000Ca00S(H00S(z00J/C00rVu%n0fi000Cb00S(G00S(x00S(B00A-v%n0fg000Cc00S(H00S(z00J/A00A-u%n0fg000Cb";
        $values = Message::unpack($messageString);     
        $this->assertEquals('array', gettype($values));
        $this->assertEquals(2*6*6, count( $values));
    }
  
}
?>
