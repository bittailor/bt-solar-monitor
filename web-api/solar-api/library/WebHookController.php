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
        $parsedBody = $request->getParsedBody();
        $data = $parsedBody['data'];
        $message = $factory->create($data);
        $xivelyMessage = $converter->convert($message);
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
