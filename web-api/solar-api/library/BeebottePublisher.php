<?php
namespace SolarApi;


class BeebottePublisher
{
    private $configuration;
    private $httpClient;

    public function __construct($configuration, $httpClient) {
        $this->configuration = $configuration;
        $this->httpClient = $httpClient;
    }
    
    public function setup() {
        $info = Sensors::info(); 
        $message = [
            'name' => 'solar', 
            'label' => 'solar', 
            'ispublic' => false, 
            'resources' =>  []
        ];
        foreach($info->sensors as $sensor) {
            $description = ucwords(str_replace('_',' ',$sensor));
            $message['resources'][] = [
                "name" => $sensor .'_i', 
                "description" => "$description Current", 
                "vtype" => 'number'
            ];
            $message['resources'][] = [
                "name" => $sensor .'_v', 
                "description" => "$description Voltage", 
                "vtype" => 'number'
            ];
        }
        $this->postData('/v1/channels', json_encode($message), true);
    }

    public function publish($message) {
        $info = Sensors::info();
        $names = [];    
        foreach($info->sensors as $sensor) {
            $names[] = $sensor .'_i';
            $names[] = $sensor .'_v';
        }
        $records = [];
        foreach($message->measurements as $measurement) {  
            foreach ($measurement->readings as $i => $reading) {
                $index = $i * 2; 
                $records[] = [ 'resource' => $names[$index], 'data' => $reading->current, 'ts' => $measurement->timestamp->getTimestamp() * 1000];
                $records[] = [ 'resource' => $names[$index+1], 'data' => $reading->voltage, 'ts' => $measurement->timestamp->getTimestamp() * 1000];
            }           
        }
        $this->writeBulk($records);
    }

    private function writeBulk($records)
    {
        $body = ["records" => $records];
        $response = $this->postData('/v1/data/write/solar', json_encode( $body ), true );
        return $response;
    }  

    private function postData($uri, $body, $auth = true)
    {
        $url  = "http://api.beebotte.com" . $uri;
        $md5  = base64_encode(md5($body, true));
        $date = date(DATE_RFC2822);
        $contentType = 'application/json';
        $verb = 'POST';

        $headers = [ "Content-MD5" => $md5, "Content-Type" => $contentType, "Date" => $date ];              
        if( $auth === true ) {
            $signature = $this->signRequest($verb, $uri, $date, $contentType, $md5);
            $headers["Authorization"] = $signature;
        }
        
        return $this->httpClient->request(
            $verb, 
            $url,
            [
                'headers' => $headers,
                'body' => $body
            ] 
        );
    }
    
    private function signRequest($verb, $uri, $date, $c_type, $c_md5 = null)
    {
        $stringToSign = $verb . "\n" . $c_md5 . "\n" . $c_type . "\n" . $date . "\n" . $uri;
        return ($this->configuration->keyId . ":" . base64_encode(hash_hmac("sha1", $stringToSign, $this->configuration->secretKey, true)));
    }
    
}
