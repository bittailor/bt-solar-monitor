<?php
use PHPUnit\Framework\TestCase;
use SolarApi\AdafruitIoPublisher;

class AdafruitIoPublisherTest extends TestCase
{
    protected function setUp()
    {
        date_default_timezone_set('Europe/Zurich');
    }

    public function testPublish()
    {
        $configuration = new stdClass();
        $configuration->aioKey ="TestCase_AioKey";
        $configuration->user ="TestCase_User";
        
        $message = MessageTest::createExampleMessageWithThreeMeasurements();

        $response = $this->createMock(Psr\Http\Message\ResponseInterface::class);
        $response->method('getStatusCode')
                 ->willReturn(201);

        $httpClient = $this->getMockBuilder(GuzzleHttp\Client::class)
                            ->setMethods(['request'])
                            ->getMock();

        $httpClient->expects($this->exactly(count(AdafruitIoPublisher::info()->feeds)))
                   ->method('request')
                   ->with(
                        $this->equalTo('POST'),
                        $this->matchesRegularExpression('/TestCase_User\/feeds\/solar\.([a-z]|-)*\/data\/batch/'),
                        $this->callback(function($subject){
                            //echo json_encode($subject['json']);
                            return 
                                array_key_exists('json',$subject) &&
                                array_key_exists('data',$subject['json']) &&
                                count($subject['json']['data']) == 3 &&
                                array_key_exists('created_at',$subject['json']['data'][0]) && 
                                array_key_exists('value',$subject['json']['data'][0]) &&
                                json_encode($subject['json']) !== false 
                            ;
                        })
                   )
                   ->willReturn($response);
                   
        $publisher = new AdafruitIoPublisher($configuration, $httpClient);
        
        $publisher->publish($message);
    }
}
