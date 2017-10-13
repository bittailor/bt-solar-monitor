<?php
namespace SolarApi;

class WebHookController
{
    private $messageFactory;
    private $converter;
    
    public function __construct(MessageFactory $messageFactory, MessageToXivelyConverter $converter) {
        $this->messageFactory = $messageFactory;
        $this->converter = $converter;
    }

    public function execute($request, $response, $args) {  
        $rawMessage = $request->getBody()->getContents();
        
        $mysqli = new \mysqli(
            ini_get("mysqli.default_host"),
            ini_get("mysqli.default_user"),
            ini_get("mysqli.default_pw"),
            SOLAR_API_DB);
        if ($mysqli->connect_errno) {
            return $response->write("Failed to connect to MySQL: (" . $mysqli->connect_errno . ") " . $mysqli->connect_error);
        }
        if (!$mysqli->set_charset("utf8")) {
            return $response->write("set_charset utf8 failed: (" . $mysqli->errno . ") " . $mysqli->error);
        }
        if (!($stmt = $mysqli->prepare("INSERT INTO RawMessages(message) VALUES (?)"))) {
            return $response->write("Prepare failed: (" . $mysqli->errno . ") " . $mysqli->error);
        }
        if (!$stmt->bind_param("s", $rawMessage)) {
            return $response->write("Binding parameters failed: (" . $stmt->errno . ") " . $stmt->error);
        }
        if (!$stmt->execute()) {
            return $response->write("Execute failed: (" . $stmt->errno . ") " . $stmt->error);
        }
        return $response->write("OK");   
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
