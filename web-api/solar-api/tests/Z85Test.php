<?php
use PHPUnit\Framework\TestCase;
use SolarApi\Z85;
use SolarApi\Message;


class Z85Test extends TestCase
{
    public function testDecode()
    {
        $decoded = Z85::decode("HelloWorld");
        $this->assertEquals('array', gettype($decoded));
        $this->assertEquals(8, count($decoded));
        $this->assertEquals(array(0x86, 0x4F, 0xD2, 0x6F, 0xB5, 0x59, 0xF7, 0x5B), $decoded);
    }

    public function testD()
    {
        $decoded = Z85::decode("00S(M00S(E00J/A00A-u%n0fi000Cc00S(H00S(y00A-z00A-u%n0fg000Cc00S(H00S(z00J/A00rVu%n0fg000Ca00S(H00S(z00J/C00rVu%n0fi000Cb00S(G00S(x00S(B00A-v%n0fg000Cc00S(H00S(z00J/A00A-u%n0fg000Cb");
        $this->assertEquals('array', gettype($decoded));
        $this->assertEquals(6*24, count($decoded));
    }

    public function testDecode__int16_t()
    {
        $decoded = Z85::decode("5.vUd");
        $this->assertEquals('array', gettype($decoded));
        $this->assertEquals(4, count($decoded));
        $this->assertEquals(array(17, 215, 18 ,103), $decoded);
        $this->assertEquals(array(0x11, 0xd7, 0x12 ,0x67), $decoded);
    }

}
?>
