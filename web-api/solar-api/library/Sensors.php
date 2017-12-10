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
            'values' => [
                'panel_p',
                'panel_v',
                'charger_i',
                'charger_v',
                'load_i'   
            ],
            'controllers' => [
                'a',
                'b'
            ]
        ];
        return json_decode(json_encode($info), FALSE);
    }
    
    
	
}
