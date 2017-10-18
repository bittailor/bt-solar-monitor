<?php

require __DIR__ . '/../vendor/autoload.php';
require __DIR__ . '/../configuration/configuration.generated.php';

function run() {
    date_default_timezone_set('Europe/Zurich');

    $cfg = SolarApiConfiguration::get()->adafruit;  
    
    $client = new GuzzleHttp\Client([
        'base_uri' => 'https://io.adafruit.com/api/v2/',
        'headers' => ['X-AIO-Key' => $cfg->aioKey]
    ]);

    $user = $cfg->user;

    $json1 = [
        'data' => []
    ];

    $json2 = [
        'data' => []
    ];

    $now = new DateTime();

    $t1 =  clone $now;
    $t1->modify("-4 hour");

    echo date('c',  $now->getTimestamp()) . "\n";
    echo date('c',  $t1->getTimestamp()) . "\n";
    
    for ($i=0; $i < 120; $i++) { 
        $json1['data'][] = [
            'created_at'=> date('c', $t1->getTimestamp()), 
            'value'=> $i
        ];
        $t1->modify("+1 minute");
    }

    for ($i=120; $i < 240; $i++) { 
        $json2['data'][] = [
            'created_at'=> date('c', $t1->getTimestamp()), 
            'value'=> $i
        ];
        $t1->modify("+1 minute");
    }
    
    try{
        $response = $client->request(
            'POST', 
            "$user/feeds/test.batchupload/data/batch",
            [
                'json' => $json1
            ] 
        );

        echo "publish1 OK => " . $response->getStatusCode();
    } catch (\Exception $e) {
        echo "publish1 FAILED: " . $e->getMessage() . "\n";       
    }

    try{
        $response = $client->request(
            'POST', 
            "$user/feeds/test.batchupload/data/batch",
            [
                'json' => $json1
            ] 
        );

        echo "publish2 OK => " . $response->getStatusCode();
    } catch (\Exception $e) {
        echo "publish2 FAILED: " . $e->getMessage() . "\n";       
    }
    
    var_dump($response2);

}

run();
