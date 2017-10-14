<?php
namespace SolarApi;

class WebHookController
{
    private $pdo;
    private $messageFactory;
    private $converter;
    
    public function __construct(\PDO $pdo,
                                MessageFactory $messageFactory, 
                                MessageToXivelyConverter $converter) {
        $this->pdo = $pdo;
        $this->messageFactory = $messageFactory;
        $this->converter = $converter;
    }

    public function execute($request, $response, $args) {  
        $rawMessage = $request->getBody()->getContents();
        $this->storeRaw($rawMessage);

        $parsedBody = $request->getParsedBody();
        $data = $parsedBody['data'];
        $message = $this->messageFactory->create($data);

        $this->storeMessage($message);
        
           
        return $response->write("OK");   
    }

    private function storeRaw($rawMessage) {
        $stmt = $this->pdo->prepare("INSERT INTO RawMessages(message) VALUES (:message)");
        $stmt->bindValue(':message', $rawMessage, \PDO::PARAM_STR);
        $stmt->execute(); 
    }

    private function storeMessage($message) {
        $sensors = array(
            'panel_a',
            'panel_b',
            'battery_a',
            'battery_b',
            'load',
            'controller'
        );
        $columns = array('ts');
        foreach ($sensors as $sensor) {
            $columns[] = $sensor . '_i';
            $columns[] = $sensor . '_v';
        }
        $values = array_map(function($c) { return ':'.$c; }, $columns);
        
        $sql = 'INSERT INTO Measurements(' . implode(",", $columns). ') VALUES (' . implode(",", $values) .')';
        $stmt = $this->pdo->prepare($sql);
        
        foreach($message->measurements as $measurement) {  
            $stmt->bindValue(':ts', date('c', $measurement->timestamp->getTimestamp()), \PDO::PARAM_STR);
            foreach ($measurement->readings as $i => $reading) {
                $index = $i * 2;
                $stmt->bindValue(':' . $sensors[$i] . '_i' , strval($reading->current), \PDO::PARAM_STR);
                $stmt->bindValue(':' . $sensors[$i] . '_v' , strval($reading->voltage), \PDO::PARAM_STR);   
            }
            $stmt->execute();           
        }
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
}
