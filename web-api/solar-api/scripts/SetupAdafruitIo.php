<?php

require __DIR__ . '/../vendor/autoload.php';
require __DIR__ . '/../configuration/configuration.generated.php';


function createFeed($client, $cfg, $feed) {
    $response = $client->request(
        'POST', 
        "$cfg->user/feeds",
        [
            'query' => ['group_key' => 'solar'],
            'json' => [
                'name' => $feed->name
            ]
        ] 
    );
    var_dump($response->getStatusCode());
}

function createFeeds() {
    $cfg = SolarApiConfiguration::get()->adafruit;  
    
    $client = new GuzzleHttp\Client([
        'base_uri' => 'https://io.adafruit.com/api/v2/',
        'headers' => ['X-AIO-Key' => $cfg->aioKey]
    ]);

    $info = SolarApi\AdafruitIoPublisher::info();   

    foreach($info->feeds as $feed) {
        createFeed($client, $cfg, $feed);   
    }
}

createFeeds();
