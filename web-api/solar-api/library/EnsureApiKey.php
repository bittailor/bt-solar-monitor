<?php
namespace SolarApi;

class EnsureApiKey {
    
    private $apiKey;

    public function __construct($apiKey) {
        $this->apiKey = $apiKey;
    }    

    public function __invoke($request, $response, $next)
    {
        if(true === $this->hasValidApiKey($request)) {
			return $next($request, $response);
        }

        return $response
            ->withStatus(401)
            ->withHeader('Content-Type', 'application/json; charset=utf-8')
            ->withHeader('WWW-Authenticate', 'None')
            ->withJson(array(
                    "error" => 401,
		            "message" => "No valid API key provided"
                )
            );
    }	

	public function hasValidApiKey($request) {
        $apiKeys = $request->getHeader('x-api-key');
        $apiKey = array_pop($apiKeys);
		return $apiKey === $this->apiKey;
	}
}
