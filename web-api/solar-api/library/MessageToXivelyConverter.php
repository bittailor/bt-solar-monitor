<?php
namespace SolarApi;


class MessageToXivelyConverter
{
    function convert(Message $message) {
        $sensors = array(
            'panel_a',
            'panel_b',
            'battery_a',
            'battery_b',
            'load',
            'controller'
        );
        $values = array();
        foreach ($sensors as $sensor) {
            $values[] = $sensor . '_i';
            $values[] = $sensor . '_v';
        }
        
        
        $data = array(
            'version'     => '1.0.0',
            'datastreams' => array(
            ),
        );

        $datastreams = &$data['datastreams'];

        foreach($values as $value) {
            $datastreams[] = array(
                'id'         => $value,
                'datapoints' => array(
                )
            );   
        }        
        
        foreach($message->measurements as $measurement) {  
            foreach ($measurement->readings as $i => $reading) {
                $index = $i * 2; 
                $datastreams[$index]['datapoints'][] = array('at' => date('c', $measurement->timestamp->getTimestamp()), 'value' => $reading->current);
                $datastreams[$index+1]['datapoints'][] = array('at' => date('c', $measurement->timestamp->getTimestamp()), 'value' => $reading->voltage);    
            }           
        }
        //return $data;
        /*
        return array(
            "version" => "1.0.0",
            "datastreams" =>  array( array(
              "id" => "example",
                  "datapoints" => array(
                    array("at"=>"2013-04-22T00:35:43Z","value"=>"41"),
                    array("at"=>"2013-04-22T00:55:43Z","value"=>"84"),
                    array("at"=>"2013-04-22T01:15:43Z","value"=>"41"),
                    array("at"=>"2013-04-22T01:35:43Z","value"=>"83")
                  ),
                "current_value" => "333"
                ),
                array( 
              "id" => "key",
                  "datapoints"=> array(
                  array("at"=>"2013-04-22T00:35:43Z","value"=>"revalue"),
                  array("at"=>"2013-04-22T00:55:43Z","value"=>"string value"),
                  array("at"=>"2013-04-22T01:15:43Z","value"=>"any string"),
                  array("at"=>"2013-04-22T01:35:43Z","value"=>"structured data")
                ),
              "current_value" => "value"      
            ),
            array( 
                "id" => "datastream",
                  "datapoints"=>[
                  array("at"=>"2013-04-22T00:35:43Z","value"=>"51"),
                  array("at"=>"2013-04-22T00:55:43Z","value"=>"102"),
                  array("at"=>"2013-04-22T01:15:43Z","value"=>"32"),
                  array("at"=>"2013-04-22T01:35:43Z","value"=>"16")
                  ],
              "current_value" => "1337"
            )
            ));
        */
        
        /*
        return array(
            "version" => "1.0.0",
            "datastreams" =>  array( array(
              "id" => "blue",
                  "datapoints" => array(
                    array("at"=>"2013-04-22T00:35:43+02:00","value"=>"41"),
                    array("at"=>"2013-04-22T00:55:43+02:00","value"=>"84"),
                    array("at"=>"2013-04-22T01:15:43+02:00","value"=>"41"),
                    array("at"=>"2013-04-22T01:35:43+02:00","value"=>"83")
                  ),
                  "current_value" => "333"
                ),
                array( 
              "id" => "green",
                  "datapoints"=> array(
                  array("at"=>"2013-04-22T00:35:43+02:00","value"=>"revalue"),
                  array("at"=>"2013-04-22T00:55:43+02:00","value"=>"string value"),
                  array("at"=>"2013-04-22T01:15:43+02:00","value"=>"any string"),
                  array("at"=>"2013-04-22T01:35:43+02:00","value"=>"structured data")
                  ), 
                "current_value" => "333"    
            ),
            array( 
                "id" => "yellow",
                  "datapoints"=>array(
                  array("at"=>"2013-04-22T00:35:43+02:00","value"=>"51"),
                  array("at"=>"2013-04-22T00:55:43+02:00","value"=>"102"),
                  array("at"=>"2013-04-22T01:15:43+02:00","value"=>"32"),
                  array("at"=>"2013-04-22T01:35:43+02:00","value"=>"16")
                  ),
                  "current_value" => "333"
            )
            ));
        */
        
        return array(
            "version" => "1.0.0",
            "datastreams" =>  array( array(
              "id" => "blue",
                  "datapoints" => array(
                    array("at"=>"2013-04-22T00:35:43Z","value"=>"41"),
                    array("at"=>"2013-04-22T00:55:43Z","value"=>"84"),
                    array("at"=>"2013-04-22T01:15:43Z","value"=>"41"),
                    array("at"=>"2013-04-22T01:35:43Z","value"=>"83")
                  ),
                "current_value" => "333"
                ),
                array( 
              "id" => "green",
                  "datapoints"=> array(
                  array("at"=>"2013-04-22T00:35:43Z","value"=>"revalue"),
                  array("at"=>"2013-04-22T00:55:43Z","value"=>"string value"),
                  array("at"=>"2013-04-22T01:15:43Z","value"=>"any string"),
                  array("at"=>"2013-04-22T01:35:43Z","value"=>"structured data")
                ),
              "current_value" => "value"      
            ),
            array( 
                "id" => "yellow",
                  "datapoints"=>[
                  array("at"=>"2013-04-22T00:35:43Z","value"=>"51"),
                  array("at"=>"2013-04-22T00:55:43Z","value"=>"102"),
                  array("at"=>"2013-04-22T01:15:43Z","value"=>"32"),
                  array("at"=>"2013-04-22T01:35:43Z","value"=>"16")
                  ],
              "current_value" => "1337"
            )
            ));
        
        
    }
	
}
