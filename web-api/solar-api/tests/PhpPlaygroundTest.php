<?php
use PHPUnit\Framework\TestCase;
use SolarApi\MessageFactory;


class PhpPlaygroundTest extends TestCase
{
    protected function setUp()
    {
        date_default_timezone_set('Europe/Zurich');
    }

    public function test_DateTime_CreateFromFormat()
    {
        $timeZulu = \DateTime::createFromFormat('Y-m-d\TH:i:sP', '2017-10-13T15:19:48Z');
        $timeLocal = \DateTime::createFromFormat('Y-m-d\TH:i:sP', '2017-10-13T17:19:48+0200');
        $this->assertEquals($timeZulu, $timeLocal);
    }

    public function test_DateTime_CreateFromFormatInvalid()
    {
        $invalidTime = \DateTime::createFromFormat('Y-m-d\TH:i:sP', '2017-10-13T15:19:48GUGU');
        $this->assertEquals(false, $invalidTime);
    }

    public function test_DateTime_Constructor()
    {
        $timeZulu = new DateTime('2017-10-13T15:19:48Z');
        $timeLocal = new DateTime('2017-10-13T17:19:48+0200');
        $this->assertEquals($timeZulu, $timeLocal);
        $this->assertEquals($timeZulu->getTimestamp(), $timeLocal->getTimestamp());
    }

    public function test_DateTime_ConstructorInvalid()
    {
        $this->expectException(Exception::class);
        $invalidTime = new DateTime('2017-10-13T15:19:48GUGU');
    }

    public function test_date_TimezoneZulu()
    {
        $t = new DateTime('2017-10-13T15:19:48Z');
        $s = date('c', $t->getTimestamp());
        $this->assertEquals('2017-10-13T17:19:48+02:00', $s);
    }

}
?>
