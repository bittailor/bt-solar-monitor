<?php
define('SOLAR_API_DEVELOPMENT_ENVIROMENT', array_key_exists("SERVER_NAME",$_SERVER) && ( $_SERVER["SERVER_NAME"] == '127.0.0.1' || $_SERVER["SERVER_NAME"] == 'localhost'));

class SolarApiConfiguration {

    public static function get() {
        static $configuration = null;
        if ($configuration === null) {
            $configuration = self::create();
        }
        return $configuration;
    }

    private static function create(){
        $settings = [
            'db' => [
            ],
            'solarApi' => [ 
                'token'  => '{${solar.webapi.token}$}'
            ],
            'xively' => [
                'apiKey' => '{${solar.xively.apiKey}$}',
                'feed' => '{${solar.xively.feed}$}'      
            ],
            'adafruit' => [
                'aioKey' => '{${solar.adafruit.aioKey}$}',
                'user' => '{${solar.adafruit.user}$}'      
            ],
            'beebotte' => [
                'keyId' => '{${solar.beebotte.keyId}$}',
                'secretKey' => '{${solar.beebotte.secretKey}$}'
            ]     
        ];

        if(SOLAR_API_DEVELOPMENT_ENVIROMENT) {
            $settings['db'] = [
                'dns' => 'mysql:host=127.0.0.1;port=8889;dbname=bockli_solar_db',
                'user' => 'root',
                'pw' => 'root'
            ]; 
        } else {
            $settings['db'] = [
                'dns' => 'mysql:host=localhost;dbname=bockli_solar_db',
                'user' => '{${solar.db.user}$}',
                'pw' => '{${solar.db.pw}$}'
            ]; 
        }
        return json_decode(json_encode($settings), FALSE);
    }
}
