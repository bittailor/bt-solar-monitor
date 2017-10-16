<?php
namespace SolarApi;


class AdafruitIoPublisher
{
    private $configuration;
    private $httpClient;

    public function __construct($configuration, $httpClient) {
        $this->configuration = $configuration;
        $this->httpClient = $httpClient;
    }
    
    public function publish($message) {
        $info = self::createInfo();
        $feeds = [];
        foreach($info->feeds as $feed) {           
            $clone = clone $feed; 
            $clone->json = [ 'data' => [] ];
            $feeds[] = $clone;
        }

        foreach($message->measurements as $measurement) {           
            foreach ($measurement->readings as $i => $reading) {
                foreach($feeds as $feed) {
                    if($feed->sensor === $i) {
                        $accessor = $feed->reading;
                        $feed->json['data'][] = [
                            'created_at'=> date('c', $measurement->timestamp->getTimestamp()), 
                            'value'=> $reading->$accessor
                        ];
                    }
                }    
            }     
        }

        foreach($feeds as $feed) {
            $status = $this->publishFeed($feed);
        }  
    }

    private function publishFeed($feed) {
        $user = $this->configuration->user;
        $response = $this->httpClient->request(
            'POST', 
            "$user/feeds/solar.$feed->name/data/batch",
            [
                'json' => $feed->json
            ] 
        );
        return $response->getStatusCode();
    } 

    //=== info === 

    static function info() {
        static $configuration = null;
        if ($configuration === null) {
            $configuration = self::createInfo();
        }
        return $configuration;
    } 

    private static function createInfo(){
        $info = [
            'feeds' => [
                ['name' =>  'panel-a-i', 'sensor' => 0, 'reading' => 'current' ],   
                ['name' =>  'panel-a-v', 'sensor' => 0, 'reading' => 'voltage' ],   
                ['name' =>  'panel-b-i', 'sensor' => 1, 'reading' => 'current' ],   
                ['name' =>  'panel-b-v', 'sensor' => 1, 'reading' => 'voltage' ],   
                
                ['name' =>  'battery-a-v', 'sensor' => 2, 'reading' => 'voltage' ], 
                ['name' =>  'battery-b-v', 'sensor' => 3, 'reading' => 'voltage' ],

                ['name' =>  'load-i', 'sensor' => 4, 'reading' => 'voltage' ], 
            ]
        ];
        return json_decode(json_encode($info), FALSE);
    }
	
}
