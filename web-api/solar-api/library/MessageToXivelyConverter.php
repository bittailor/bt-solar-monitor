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
        return $data;
    }
	
}
