using Newtonsoft.Json;

namespace Bt.DeviceApi.Particle;

class EventResponseDto
{
    [JsonProperty("data")]
    public string Data { get; set; } = String.Empty;
    [JsonProperty("ttl")]
    public int TTL { get; set; }
    [JsonProperty("published_at")] 
    public DateTime PublishedAt { get; set; }
    [JsonProperty("coreid")] 
    public string DeviceId { get; set; } = "unknown";
}