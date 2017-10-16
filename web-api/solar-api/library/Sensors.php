<?php
namespace SolarApi;


class Sensors
{
    static function info() {
        static $configuration = null;
        if ($configuration === null) {
            $configuration = self::create();
        }
        return $configuration;
    } 

    private static function create(){
        $info = [
            'sensors' => [
                'panel_a',
                'panel_b',
                'battery_a',
                'battery_b',
                'load',
                'controller'    
            ]
        ];

        return json_decode(json_encode($info), FALSE);
    }
    
    
	
}
