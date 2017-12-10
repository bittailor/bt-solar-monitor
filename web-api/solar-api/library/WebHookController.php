<?php
namespace SolarApi;

class WebHookController
{
    private $pdo;
    private $messageFactory;
    private $publishers;
    
    public function __construct($pdo,
                                $messageFactory, 
                                $publishers) {
        $this->pdo = $pdo;
        $this->messageFactory = $messageFactory;
        $this->publishers = $publishers;
    }

    public function execute($request, $response, $args) {  
        $stopWatch = new StopWatch();

        try {
            $rawMessage = $request->getBody()->getContents();
            $this->storeRaw($rawMessage);
            $response->write("store raw  [" . $stopWatch->lapStr() . "ms] => OK\n");
        } catch (\Exception $e) {
            $response->write("store raw [" . $stopWatch->lapStr() . "ms] => FAILED: " . $e->getMessage() . "\n");        
        }

        try {
            $parsedBody = $request->getParsedBody();
            $data = $parsedBody['data'];
            $message = $this->messageFactory->create($data);
            $this->storeMessage($message);
            $response->write("store message [" . $stopWatch->lapStr() . "ms] => OK\n");
        } catch (\Exception $e) {
            $response->write("store message [" . $stopWatch->lapStr() . "ms] => FAILED: " . $e->getMessage() . "\n");        
        }

        foreach ($this->publishers as $publisher) {
            try {
                $publisher->publish($message);
                $response->write("publish " . get_class($publisher) . " [" . $stopWatch->lapStr() . "ms] => OK\n");
            } catch (\Exception $e) {
                $response->write("publish " . get_class($publisher) . " [" . $stopWatch->lapStr() . "ms] => FAILED: " . $e->getMessage() . "\n");        
            }
        }
        return $response->write("response => OK");   
    }

    private function storeRaw($rawMessage) {
        $stmt = $this->pdo->prepare("INSERT INTO RawMessages(message) VALUES (:message)");
        $stmt->bindValue(':message', $rawMessage, \PDO::PARAM_STR);
        $stmt->execute(); 
    }

    private function storeMessage($message) {
        $columns = array(
            'ts',
            
            'a_panel_p',
            'a_panel_v',
            'a_charger_i',
            'a_charger_v',
            'a_load_i',
        
            'b_panel_p',
            'b_panel_v',
            'b_charger_i',
            'b_charger_v',
            'b_load_i'
        );

        $values = array_map(function($c) { return ':'.$c; }, $columns);
        
        $sql = 'INSERT INTO Measurements(' . implode(",", $columns). ') VALUES (' . implode(",", $values) .')';
        \error_log("sql is $sql");
        $stmt = $this->pdo->prepare($sql);
        
        foreach($message->measurements as $measurement) {  
            $stmt->bindValue(':ts', date('c', $measurement->timestamp->getTimestamp()), \PDO::PARAM_STR);
            foreach ($measurement->values as $i => $value) {
                \error_log("bind " . $values[$i+1] . " to " . strval($value));
                $stmt->bindValue($values[$i+1] , strval($value), \PDO::PARAM_STR);  
            }
            $stmt->execute();           
        }
    }

    function sendToXively($message) {
        
        $client = new \GuzzleHttp\Client(['base_uri' => 'https://api.xively.com']);
        $xivelyMessage = $this->converter->convert($message);
        $response = $client->request('PUT', '/v2/feeds/'.SOLAR_API_XIVELY_FEED, [
            'headers' => ['X-ApiKey' => SOLAR_API_XIVELY_API_KEY],
            'json' => $xivelyMessage
        ]);
        var_dump($response);
        
    }

    public function executeOld($request, $response, $args) {  
        $parsedBody = $request->getParsedBody();
        $data = $parsedBody['data'];
        $message = $this->messageFactory->create($data);          
        $xivelyMessage = $this->converter->convert($message);
        $xivelyApi = new \Xively\Api(SOLAR_API_XIVELY_API_KEY);
        $feed = $xivelyApi->feeds(SOLAR_API_XIVELY_FEED);
        $result = $feed->update($xivelyMessage)->get();
        if($result) {
            return $response->write("OK");
        } else {
            return $response->write("FAILED");    
        }     
    }

    private function micros(& $start ) {

    } 
}
