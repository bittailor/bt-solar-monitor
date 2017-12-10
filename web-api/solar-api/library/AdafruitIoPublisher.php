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
            foreach ($measurement->values as $i => $value) {
                foreach($feeds as $feed) {
                    if($feed->valueIndex === $i) {
                        $feed->json['data'][] = [
                            'created_at'=> date('c', $measurement->timestamp->getTimestamp()), 
                            'value'=> $value
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
                ['name' =>  'a-panel-p', 'valueIndex' => 0],   
                ['name' =>  'b-panel-p', 'valueIndex' => 5],   
                ['name' =>  'a-panel-v', 'valueIndex' => 1],   
                ['name' =>  'b-panel-v', 'valueIndex' => 6],   
                
                ['name' =>  'a-load-i', 'valueIndex' => 4], 
                ['name' =>  'b-load-i', 'valueIndex' => 9], 
            ]
        ];
        return json_decode(json_encode($info), FALSE);
    }
	
}
