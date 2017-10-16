<?php

require __DIR__ . '/../vendor/autoload.php';
require __DIR__ . '/../configuration/configuration.generated.php';


function createFeed($client, $cfg, $name) {
    
    //var_dump(json_decode($response->getBody()));
}

function createFeeds() {
    $cfg = SolarApiConfiguration::get()->xively;  
    $client = new GuzzleHttp\Client([
        //'base_uri' => 'https://api.xively.com/api/v2/',
        'headers' => ['X-ApiKey' => $cfg->apiKey ]
    ]);

    $info = SolarApi\Sensors::info();   

    $body = [
        'datastreams' => []    
    ];

    foreach($info->sensors as $sensor) {
        $body['datastreams'][] = [
            'id' => $sensor . '_i',
            'tags' => "current",
            'unit' => [
                'symbol' => "A",
                'label' =>  "Ampere"
            ]
        ];
        $body['datastreams'][] = [
            'id' => $sensor . '_v',
            'tags' => "voltage",
            'unit' => [
                'symbol' => "V",
                'label' =>  "Volt"
            ]
        ];
    }
    $response = $client->request(
        'POST', 
        "feeds/$cfg->feed/datastreams",
        [
            'json' => $body
        ] 
    );
    var_dump($response->getStatusCode());
}

createFeeds();
